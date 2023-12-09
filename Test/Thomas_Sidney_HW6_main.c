/**************************************************************
 * Class:  CSC-415-01 Fall 2023
 * Name: Sidney Thomas
 * Student ID: 918656419
 * GitHub: siid14
 * Project: Assignment 6 – Device Driver
 *
 * File: Thomas_Sidney_HW6_main.c
 *
 * Description: This file serves as a test script for the custom device driver.
 *              It interacts with the device's file operations,including functionalities like opening,
 *              reading, writing, performing input/output control (ioctl), and closing the device.
 *              Furthermore, it enables users to input single words
 *              for translation through the device driver.
 **************************************************************/
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
    printf("*******************************************\n");
    printf("        Welcome to Igpay Atinlay Translator       \n");
    printf("*******************************************\n");

    // open the device file for read/write access
    device_file = open("/dev/DeviceDriver", O_RDWR);
    if (device_file < 0)
    {
        printf("Cannot open device file...\n");
        return 0;
    }

    // utilize ioctl to switch between languages (ENG <-> PL)
    printf("**************** Language Options ****************\n");
    printf("0. Translate from English to Igpay Atinlayn\n");
    printf("1. Translate from Igpay Atinlay to English\n");
    printf("***************************************************\n");
    printf("\nEnter your desired translation direction (0 or 1): ");

    fgets(switcher, 24, stdin);
    language_switch = atoi(switcher);
    ioctl(device_file, language_switch, &info);
    while (1)
    {
        // display user options and take input
        printf("\n**** Menu ****\n");
        printf("1. Write data to the device\n");
        printf("2. Read data from the device\n");
        printf("3. Switch translation language\n");
        printf("4. Exit\n");
        printf("***************\n");
        printf("\nEnter your choice (1-4): ");
        fgets(user_option, 8, stdin);
        printf("Your Option = %s\n", user_option);

        switch (user_option[0])
        {
        case '1':
            // writing data to the driver
            printf("Enter the string to write into driver: ");
            fgets(write_buffer, 1024, stdin);
            printf("Data Writing ...");
            write(device_file, write_buffer, strlen(write_buffer) + 1);
            printf("Done!\n");
            break;
        case '2':
            // reading data from the driver
            read(device_file, read_buffer, 1024);
            printf("Data = %s\n", read_buffer);
            break;
        case '3':
            // switching between ENG and PL
            printf("*****************************************\n");
            printf("         Translation Options:            \n");
            printf("*****************************************\n");
            printf("  Option 0: Translate English to Igpay Atinlay\n");
            printf("  Option 1: Translate Igpay Atinlay to English\n");
            printf("*****************************************\n");
            printf("\nEnter your choice (0 or 1): ");
            fgets(switcher, 24, stdin);
            language_switch = atoi(switcher);
            ioctl(device_file, language_switch, &info);
            break;
        case '4':
            // closing the device file and exiting
            close(device_file);
            printf("\nThank you for using my Igpay Atinlay translator!\n");
            exit(0);
            break;
        default:
            // handling invalid options
            printf("Enter Valid user_option = %s\n", user_option);
            break;
        }
    }
    // close the device file
    close(device_file);
    return 0;
}
