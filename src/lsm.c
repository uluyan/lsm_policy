#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/security.h>
#include <linux/fs.h>
#include <linux/sched.h>


#define MAX_LENGTH 256
char controlleddir[256];
int enable_flag = 0;


char get_flag(int current_pid, int flagid){
    char current_id[256];
    char flags[256];
    char pid[256];
    int fd;

    sprintf(pid,"%i",current_pid);

	int i;
	int j;
	int index = 0;
	for (i = 0; i<strlen(controlleddir); i++) {
		if (controlleddir[i] == ':') {
			for(j = index;j<i;j++){
				current_id[j] = controlleddir[j];
			}
			if(strncmp(current_id,pid,i) == 0){
				for(j=i+1;j<=i+8;j++){
					flags[j-i-1] = controlleddir[j];
				}
				break;
			}
			else{
				index=i+9;
			}
		}
	}

	if(i == strlen(controlleddir))
		return 0;
	return flags[flagid] - 48;
}


static int lsm_inode_rmdir(struct inode *dir, struct dentry *dentry){
	int flagid = 0;
	int flag = 0;
	flag = get_flag(current->pid,flagid);
	if( flag == 1)
		printk("Delete Failed ! Permission Denied.\n");
	return flag;
}


static int lsm_inode_unlink(struct inode *dir, struct dentry *dentry){
	int flagid = 0;
	int flag = 0;
	flag = get_flag(current->pid,flagid);
	if( flag == 1)
		printk("Delete Failed ! Permission Denied.\n");
	return flag;
}

static int lsm_inode_create(struct inode *dir, struct dentry *dentry, int mode){
	int flagid = 1;
	int flag = 0;
	flag = get_flag(current->pid,flagid);
	if( flag == 1)
		printk("Create Failed ! Permission Denied.\n");
	return flag;
}

static int lsm_inode_mkdir(struct inode *dir, struct dentry *dentry, int mode){
	int flagid = 1;
	int flag = 0;
	flag = get_flag(current->pid,flagid);
	if( flag == 1)
		printk("Create Failed ! Permission Denied.\n");
	return flag;
}



int write_controlleddir(int fd, char *buf, ssize_t len){
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
	return len;
}


struct file_operations fops = {
	owner:THIS_MODULE, 
	write: write_controlleddir, 
}; 

static struct security_operations lsm_ops={
	.inode_rmdir = lsm_inode_rmdir,
	.inode_unlink = lsm_inode_unlink,
	.inode_mkdir = lsm_inode_mkdir,
	.inode_create = lsm_inode_create,
//	.file_alloc_security = lsm_file_alloc,
//	.file_free_security = lsm_file_free_security,
//	.file_permission = lsm_file_permission,
//	.task_create = lsm_task_create,
};


static int secondary=0;


static int __init lsm_init(void){
	int ret;
    if(register_security(&lsm_ops)){
        printk(KERN_INFO"Failure registering LSM module with kernel\n");
        secondary=1;
    }
    printk(KERN_INFO"LSM Module Init Success! \n");
	ret = register_chrdev(123, "/dev/controlfile", &fops);
	if (ret != 0)
		printk("Can't register device file! \n"); 	
    return 0;
}


static void __exit lsm_exit(void){
    printk(KERN_INFO"LSM Module unregistered.....\n");
	unregister_chrdev(123, "procinfo");
}


module_init(lsm_init);
module_exit(lsm_exit);
MODULE_LICENSE("GPL");
