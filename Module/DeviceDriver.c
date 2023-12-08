#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_MAJOR_NUMBER 415
#define DEVICE_MINOR_NUMBER 0
#define DRIVER_NAME "DeviceDriver"
#define BUFFER_SIZE 32

int TRANSLATION_MODE = 0;
uint8_t *device_buffer;
dev_t device_id = 0;

static ssize_t writeToDevice(struct file *fs, const char __user *buf, size_t hsize, loff_t *off);
static int openDevice(struct inode *inode, struct file *fs);
static ssize_t readFromDevice(struct file *fs, char __user *buf, size_t hsize, loff_t *off);
static int closeDevice(struct inode *inode, struct file *fs);
static long deviceIoControl(struct file *fs, unsigned int command, unsigned long data);

// file operations structure
static struct file_operations deviceFileOps = {
    .owner = THIS_MODULE,
    .open = openDevice,
    .read = readFromDevice,
    .write = writeToDevice,
    .release = closeDevice,
    .unlocked_ioctl = deviceIoControl,
};