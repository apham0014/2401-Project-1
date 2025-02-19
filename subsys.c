#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

// Constructor for subsystem.
int subsys_init(Subsystem *subsystem, const char *name, char status){

    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    subsystem->status = status;

    strncpy(subsystem->name, name, MAX_STR-1);
    subsystem->name[MAX_STR - 1] = '\0';

    subsystem->data = 0;

    return ERR_SUCCESS;
}

// Function that prints, on a single line, the attributes of a substem instance.
int subsys_print(Subsystem *subsystem){

    printf("- [Name: ");
    printf("%-20s", subsystem->name);
    subsys_status_print(subsystem);
    
    unsigned int data = 0; 

    int output = subsys_data_get(subsystem, &data);
    if (output != ERR_NO_DATA){
        printf("            ");
        printf("Data: 0x");
        printf("%08X", data);
    }
    printf("]\n");
    return ERR_SUCCESS;
}

int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value) {
    int intStatus = status - '0';
    int intValue = value - '0';

    if (intStatus == 1 || intStatus == 3 || intStatus >= 8 || intStatus < 0) { 
        return ERR_INVALID_STATUS;

    } else if (intStatus == 0) {
        switch (intValue) {
            
            case 0:
                subsystem->status = clearBit(subsystem->status, 0);
                subsystem->status = clearBit(subsystem->status, 1);
                break;
            case 1:
                subsystem->status = setBit(subsystem->status, 0);
                subsystem->status = clearBit(subsystem->status, 1);
                break;
            case 2:
                subsystem->status = setBit(subsystem->status, 1);
                subsystem->status = clearBit(subsystem->status, 0);
                break;
            case 3:
                printf("TEST1");
                printBits(subsystem->status);
                subsystem->status = setBit(subsystem->status, 0);
                printf("TEST2");
                printBits(subsystem->status);
                subsystem->status = setBit(subsystem->status, 1);
                printf("TEST3");
                printBits(subsystem->status);
                break;
        }
        
    } else if (intStatus == 2) {
        switch (intValue) {
            
            case 0:
                subsystem->status = clearBit(subsystem->status, 2);
                subsystem->status = clearBit(subsystem->status, 3);
                break;
            case 1:
                subsystem->status = setBit(subsystem->status, 2);
                subsystem->status = clearBit(subsystem->status, 3);
                break;
            case 2:
                printf("TEST1");
                printBits(subsystem->status);
                subsystem->status = setBit(subsystem->status, 3);
                printf("TEST2");
                printBits(subsystem->status);
                subsystem->status = clearBit(subsystem->status, 2);
                printf("TEST3");
                printBits(subsystem->status);
                break;
            case 3:
                subsystem->status = setBit(subsystem->status, 2);
                subsystem->status = setBit(subsystem->status, 3);
                break;
        }
        
    } else if (intStatus == 4) {
        switch (intValue) {
            
            case 0:
                subsystem->status = clearBit(subsystem->status, 4);
                break;

            case 1:
                subsystem->status = setBit(subsystem->status, 4);
                break;

        }
    } else if (intStatus == 5) {
        switch (intValue) {
            
            case 0:
                subsystem->status = clearBit(subsystem->status, 5);
                break;

            case 1:
                subsystem->status = setBit(subsystem->status, 5);
                break;
        }
    } else if (intStatus == 6) {
        switch (intValue) {
            
            case 0:
                subsystem->status = clearBit(subsystem->status, 6);
                break;

            case 1:
                subsystem->status = setBit(subsystem->status, 6);
                break;
        }
    } else {
        switch (intValue) {
            
            case 0:
                subsystem->status = clearBit(subsystem->status, 7);
                break;

            case 1:
                subsystem->status = setBit(subsystem->status, 7);
                break;
        }
    }
    return ERR_SUCCESS;
}

int subsys_status_print(const Subsystem *subsystem){

    int power;
    int data;
    int activity;
    int error;
    int performance;
    int resource;

    if (getBit(subsystem->status, 0) == 0 && getBit(subsystem->status, 1) == 0) {
        resource = 0;
    } else if (getBit(subsystem->status, 0) == 1 && getBit(subsystem->status, 1) == 0) {
        resource = 1;
    } else if (getBit(subsystem->status, 0) == 0 && getBit(subsystem->status, 1) == 1) {
        resource = 2;
    } else if (getBit(subsystem->status, 0) == 1 && getBit(subsystem->status, 1) == 1) {
        resource = 3;
    }

 if (getBit(subsystem->status, 2) == 0 && getBit(subsystem->status, 3) == 0) {
        performance = 0;
    } else if (getBit(subsystem->status, 2) == 1 && getBit(subsystem->status, 3) == 0) {
        performance = 1;
    } else if (getBit(subsystem->status, 2) == 0 && getBit(subsystem->status, 3) == 1) {
        performance = 2;
    } else if (getBit(subsystem->status, 2) == 1 && getBit(subsystem->status, 3) == 1) {
        performance = 3;
    }


    if (getBit(subsystem->status, 4) == 1){
        error = 1;
    }else{
        error = 0;
    }
    
    if (getBit(subsystem->status, 5) == 1){
        activity = 1;
    }else{
        activity = 0;
    }
    
    if (getBit(subsystem->status, 6) == 1){
        data = 1;
    }else{
        data = 0;
    }
    
    if (getBit(subsystem->status, 7) == 1){
        power = 1;
    }else{
        power = 0;
    }
    
    printf("Status: (PWR: %d| DATA: %d|ERR: %d|ACT: %d|PERF: %d|RES: %d)", power, data, error, activity, performance, resource);
    return ERR_SUCCESS;
}

int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data){
    if (old_data != NULL) {
        *old_data = subsystem->data;
    }

    subsystem->data = new_data;

    printBits(subsystem->status);
    subsystem->status = setBit(subsystem->status, STATUS_DATA);
    printBits(subsystem->status);

    return ERR_SUCCESS;
}

// function that returns the data value of a subsystem and resets it to 0.
int subsys_data_get(Subsystem *subsystem, unsigned int *dest){
    if (getBit(subsystem->status, STATUS_DATA) == 0){
        return ERR_NO_DATA;
    }else{
        *dest = subsystem->data;
        subsystem->data = 0;
        subsystem->status = clearBit((subsystem->status), STATUS_DATA);
        return ERR_SUCCESS;
    }
}


// HELPER FUNCTIONS
unsigned char getBit(unsigned char c, int n) {
    return ((c & (1 << n)) >> n);
  }
  
  unsigned char setBit(unsigned char c, int n) {
    return c | (1<<n);
  }
  
  unsigned char clearBit(unsigned char c, int n) {
    return c & ~(1 << n);
  }
  
  void printBits(unsigned char c) {
  for (int i = 7; i >= 0; i--) {  
          printf("%d", (c >> i) & 1);
      }
      printf("\n"); 
  }



