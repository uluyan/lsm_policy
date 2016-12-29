#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/security.h>
#include <linux/fs.h>
#include <linux/sched.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//#include <linux/mm.h>
//#include <linux/moduleparam.h>


#define MAX_LENGTH 256
char controlleddir[256];
int enable_flag = 0;

/*
static int get_fullpath(struct dentry *dentry, char *full_path){
	struct dentry *tmp_dentry = dentry;
	char tmp_path[MAX_LENGTH];
	char local_path[MAX_LENGTH];
	memset(tmp_path,0,MAX_LENGTH);
	memset(local_path,0,MAX_LENGTH);
	while (tmp_dentry != NULL)
	{
		if (!strcmp(tmp_dentry->d_iname,"/"))
			break;
		strcpy(tmp_path,"/");
		strcat(tmp_path,tmp_dentry->d_iname);
		strcat(tmp_path,local_path);
		strcpy(local_path,tmp_path);

		tmp_dentry = tmp_dentry->d_parent;
	}
	strcpy(full_path,local_path);
	return 0;
}
*/

/*
int myown_check(char *full_name){
	if (enable_flag == 0)
		return 0;
	if (strncmp(full_name, controlleddir,strlen(controlleddir)) == 0)
	{
		printk("remove denied of the file: %s \n",full_name);
		return 1;
	} 
	else return 0;
}
*/

char get_flag(int current_pid, int flagid){
    char config[256];
    char current_id[256];
    char flags[256];
    char pid[256];
    //struct stat buf;
    int fd;

    sprintf(pid,"%i",current_pid);

	//fd =open("/dev/controlfile",O_RDWR,S_IRUSR|S_IWUSR);
	//read(fd,config,strlen(config)); //Read
    
    strcpy(config,"4321:10000000");
	
	int i;
	int j;
	int index = 0;
	for (i = 0; i<strlen(config); i++) {
		if (config[i] == ':') {
			for(j = index;j<i;j++){
				current_id[j] = config[j];
			}
			if(strncmp(current_id,pid,i) == 0){
				for(j=i+1;j<=i+8;j++){
					flags[j] = config[j];
				}
				break;
			}
			else{
				index=i+9;
			}
		}
	}

	printk("Current i:%d\n",i);
	if(i == strlen(config))
		return 0;
	return flags[flagid];
}

static int lsm_inode_rmdir(struct inode *dir, struct dentry *dentry){
	int flagid = 0;
	int flag = 0;
	flag = get_flag(current->pid,flagid);
        printk("pid:%i ",current->pid);
        printk("flagid:%i\n",flagid);
	return flag;
}


static int lsm_inode_unlink(struct inode *dir, struct dentry *dentry)
{
	int flagid = 0;
	int flag = 0;
	flag = get_flag(current->pid,flagid);
        printk("pid:%i ",current->pid);
        printk("flagid:%i\n",flagid);
	return flag;
}

/*
static int lsm_task_create(unsigned long clone_flags)
{
	const int n = snprintf(NULL, 0, "%lu", clone_flags);
	char buf[n + 1];
	int c = snprintf(buf, n + 1, "%lu", clone_flags);
	strcpy(conpath, buf);
	printk("conpath is %s\n", conpath);
	return 0;
}
*/

/*
static int lsm_inode_create(struct inode *dir, struct dentry *dentry, int mode)
{
	char full_name[MAX_LENGTH];
	//	printk(KERN_INFO"Function 'inode_unlink' has been called\n");
	memset(full_name, 0, MAX_LENGTH);
	get_fullpath(dentry, full_name);
	//	printk("fullname:%s  controlleddir:%s \n",full_name,controlleddir);
	if (myown_check(full_name) != 0)
	{
		printk("remove denied of the file: %s \n", full_name);
		return 1;
	}
	return 0;
}
*/

int write_controlleddir(int fd, char *buf, ssize_t len)
{
	if (len == 0){
		enable_flag = 0;
		printk("Cancel the protect mechanism sucessfullly! \n");
		return len;
	}
	if (copy_from_user(controlleddir, buf, len) != 0){
		printk("Can't get the controlled directory's name! \n");
		printk("Something may be wrong, please check it! \n");
		enable_flag = 0;
	}
	controlleddir[len] = '\0';
	enable_flag = 1;
	printk("Controlleddir name: %s \n",controlleddir);
	return len;
}


struct file_operations fops = {
	owner:THIS_MODULE, 
	//open: this_open,
	//read: this_read,
	//write: this_write,
	//write: write_controlleddir, 
}; 

int this_open(int fd, char *buf, ssize_t len){
	return 0;
}

int this_read(int fd, char *buf, ssize_t len){
	return 0;
}

int this_write(int fd, char *buf, ssize_t len){
	return 0;
}

static struct security_operations lsm_ops=
{
	.inode_rmdir = lsm_inode_rmdir,
	.inode_unlink = lsm_inode_unlink,
//	.task_create = lsm_task_create,
//	.inode_mkdir = lsm_inode_mkdir,
//	.inode_create = lsm_inode_create,
//	.file_alloc_security = lsm_file_alloc,
//	.file_free_security = lsm_file_free_security,
//	.file_permission = lsm_file_permission,
};


static int secondary=0;


static int __init lsm_init(void)
{
	int ret;
    if(register_security(&lsm_ops))
          {
        printk(KERN_INFO"Failure registering LSM module with kernel\n");
        secondary=1;

           }
    printk(KERN_INFO"LSM Module Init Success! \n");
	 ret = register_chrdev(123, "/dev/controlfile", &fops);
	if (ret != 0) printk("Can't register device file! \n"); 	
//	strcpy(controlleddir,"/root/lsm-check");
   return 0;
}


static void __exit lsm_exit(void)
{
  /*  if(secondary)
          {
        mod_unreg_security(KBUILD_MODNAME,&lsm_ops);
          }
    else
          {
        unregister_security(&lsm_ops);
          }

  */
    printk(KERN_INFO"LSM Module unregistered.....\n");
	unregister_chrdev(123, "procinfo");	 // ÏòÏµÍ³×¢ÏúÉè±¸½áµãÎÄ¼þ 
	
}
//security_initcall(lsm_init);


module_init(lsm_init);
module_exit(lsm_exit);
MODULE_LICENSE("GPL");
