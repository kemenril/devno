#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>


#define __NONAME "Nooooooooo"
#define __NOVER "0.2"
#define __NODEV "no"


MODULE_LICENSE("GPL\0 or WTFPL");
MODULE_AUTHOR("Chris Smith");
MODULE_DESCRIPTION("Implements character special /dev/no device.");
MODULE_VERSION(__NOVER);

//Prototypes make the file_operations struct not break.
static int noopen(struct inode *, struct file *);
static ssize_t noread(struct file *, char *, size_t, loff_t *);
static int noclose(struct inode *, struct file *);

//Here's a list of file operations we can perform on /dev/no
static struct file_operations noops = {
	.open = noopen,
	.read = noread,
	.release = noclose
};


char pos = 0;	//Only track whether we're at the very beginning.
int major;
static struct class * noclass;

static int __init devno_init(void) {
	pos = 0;
	printk(KERN_INFO "%s v%s INIT ", __NONAME, __NOVER);
	major = register_chrdev(0,__NODEV,&noops);
	if(major<0) {
		printk(KERN_ALERT "%s, Can't register device.",__NONAME);
		return major;
	}
	printk(KERN_INFO "%s, registered.",__NONAME);
	noclass = class_create(THIS_MODULE,__NODEV);
	if(IS_ERR(noclass)) {
		return PTR_ERR(noclass);
	}
	device_create(noclass,NULL,MKDEV(major,0),NULL,__NODEV);
	return 0;
}




static void __exit devno_exit(void) {
	device_destroy(noclass,MKDEV(major,0));
	class_unregister(noclass);
	class_destroy(noclass);
	unregister_chrdev(major, __NODEV);
	printk(KERN_INFO "%s v%s removed.",__NONAME, __NOVER);
	return;
}

//Hooks for insertion and removal.
module_init(devno_init);
module_exit(devno_exit);


//Handle /dev/no
static int noopen (struct inode *in, struct file *fl) { //We're always open.
	pos = 0;
	return 0;
}

static ssize_t noread(struct file *fl, char *buf, size_t len, loff_t *off) {
	int err = 0;
	long outbytes = 0;

	if (pos == 0) {
		err += copy_to_user(buf,"N",1);
		pos = 1;
		len--;
		outbytes++;
	}
	while (len-- > 0) {
		err = copy_to_user(buf + outbytes++,"o",1);
	}
	if (err > 0) {return -EFAULT;} else {return outbytes;}
}

static int noclose(struct inode *in,struct file *fi) {
	return 0;
}

