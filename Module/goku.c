#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#include <linux/string.h>

#include <linux/uaccess.h>
#include <linux/random.h>

#include <linux/sched.h>