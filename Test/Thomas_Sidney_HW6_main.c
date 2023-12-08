#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

char write_buffer[1024];       // buffer to store user input for writing to the device
char read_buffer[1024];        // buffer to store data read from the device
char translation_buffer[1024]; // buffer to hold translated text

int main(int argc, char *argv[])
{
    int device_file, info;
    int language_switch = 0;
    char switcher[8];
    char user_option[8];
    printf("---------------------------------------------------\n");
    printf(" - - -  Welcome to Igpay Atinlay Translator - - -   \n");
    printf("---------------------------------------------------\n");
}