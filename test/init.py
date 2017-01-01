import os

def init():
    Files = ['read','write','delete']
    for f in Files:
        if os.path.exists(f):
            os.remove(f)
        newf = open(f,'w')
        newf.close()
    newf2 = open('read','w')
    newf2.writelines('Hello!')
    newf2.close()
    if os.path.exists('create') :
        os.remove('create')

if __name__ == '__main__':
    init()