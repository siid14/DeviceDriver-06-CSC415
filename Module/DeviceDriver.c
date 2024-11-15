/**************************************************************
 * Class:  CSC-415-01 Fall 2023
 * Name: Sidney Thomas
 * Student ID: 918656419
 * GitHub: siid14
 * Project: Assignment 6 – Device Driver
 *
 * File: DeviceDriver.c
 *
 * Description: This file implements a custom device driver with functionalities
 *              for reading, writing, and controlling an associated device.
 *              It defines file operations like open, read, write, ioctl (input/output control),
 *              and close. The driver interacts with user-space applications
 *              to perform translations of single words based on specific rules,
 *              such as translating words starting with vowels
 *              and ending with 'y' to a Igpay Atinlay-like format.
 **************************************************************/

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

// global variables
int TRANSLATION_MODE = 0;
uint8_t *device_buffer;
dev_t device_id = 0;

struct cdev device_cdev;

// function prototypes
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

// write data to the device
static ssize_t writeToDevice(struct file *fs, const char __user *buf, size_t hsize, loff_t *off)
{
    if (copy_from_user(device_buffer, buf, hsize))
    {
        printk(KERN_ERR "Cannot write data.\n");
        return -EFAULT;
    }
    printk(KERN_INFO "Data written : %s", device_buffer);
    return hsize;
}

// open the device
static int openDevice(struct inode *inode, struct file *fs)
{
    printk(KERN_INFO "Device opened!\n");
    return 0;
}

// read data from the device
static ssize_t readFromDevice(struct file *fs, char __user *buf, size_t hsize, loff_t *off)
{
    char test[BUFFER_SIZE];
    int end = 3;

    int j;
    if ((test[0] == 'a' || test[0] == 'e' || test[0] == 'i' || test[0] == 'o' || test[0] == 'u' ||
         test[0] == 'A' || test[0] == 'E' || test[0] == 'I' || test[0] == 'O' || test[0] == 'U') &&
        test[end - 3] == 'y')
    {
        // translation logic for words starting with vowels and ending with 'y'
        for (j = 0; j < BUFFER_SIZE - 2; ++j)
        {
            // translation: Move first letter to end and add "ay"
            char firstLetter = test[0];
            for (int i = 0; i < BUFFER_SIZE - 2; ++i)
            {
                test[i] = test[i + 1];
            }
            test[BUFFER_SIZE - 2] = firstLetter;
            test[BUFFER_SIZE - 1] = 'a';
            test[BUFFER_SIZE] = 'y';
            test[BUFFER_SIZE + 1] = '\0';
            break;
        }
        printk(KERN_INFO "Translated: %s", test);
    }
    else
    {
        // translation logic for words not starting with vowels or not ending with 'y'
        int len = strlen(test);
        for (int i = 0; i < len / 2; ++i)
        {
            char temp = test[i];
            test[i] = test[len - i - 1];
            test[len - i - 1] = temp;
        }
        printk(KERN_INFO "Reversed: %s", test);
    }

    if (copy_to_user(buf, test, hsize))
    {
        printk(KERN_ERR "Cannot read data.\n");
        return -EFAULT;
    }
    printk(KERN_INFO "Data read : %s", buf);
    return hsize;
}

// close the device
static int closeDevice(struct inode *inode, struct file *fs)
{
    printk(KERN_INFO "Device closed!\n");
    return 0;
}

// device control
static long deviceIoControl(struct file *fs, unsigned int command, unsigned long data)
{
    printk(KERN_INFO "command is: %d", command);

    if (command == 0)
    {
        printk(KERN_INFO "Switching to ENG -> IA");
        TRANSLATION_MODE = 0;
    }
    else if (command == 1)
    {
        printk(KERN_INFO "Switching to IA -> ENG");
        TRANSLATION_MODE = 1;
    }
    else
    {
        printk(KERN_INFO "Invalid command!");
        return -EINVAL;
    }

    return 0;
}

// initialization function for the device driver
static int __init deviceDriverInit(void)
{
    int result;
    dev_t devno = MKDEV(DEVICE_MAJOR_NUMBER, DEVICE_MINOR_NUMBER);

    result = register_chrdev_region(devno, 1, DRIVER_NAME);
    if (result < 0)
    {
        printk(KERN_WARNING "Can't get major number %d\n", DEVICE_MAJOR_NUMBER);
        return result;
    }

    cdev_init(&device_cdev, &deviceFileOps);

    result = cdev_add(&device_cdev, devno, 1);
    if (result < 0)
    {
        unregister_chrdev_region(devno, 1);
        printk(KERN_WARNING "Error %d adding char device\n", result);
        return result;
    }

    device_buffer = vmalloc(sizeof(uint8_t) * BUFFER_SIZE);
    if (!device_buffer)
    {
        unregister_chrdev_region(devno, 1);
        cdev_del(&device_cdev);
        printk(KERN_ERR "Error: failed to allocate memory\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Device driver loaded successfully!\n");
    return 0;
}

// exit function for the device driver
static void __exit deviceDriverExit(void)
{
    dev_t devno = MKDEV(DEVICE_MAJOR_NUMBER, DEVICE_MINOR_NUMBER);
    unregister_chrdev_region(devno, 1);
    cdev_del(&device_cdev);
    vfree(device_buffer);

    printk(KERN_INFO "Exit of device driver!\n");
}

module_init(deviceDriverInit);
module_exit(deviceDriverExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sidney Thomas");
MODULE_DESCRIPTION("Assignment 6 – Device Driver");
MODULE_VERSION("1.0");
