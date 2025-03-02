// Ahmad Baytamouni 101335293
// Austin Pham 101333594

#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

/*
    Initializes the Subsystem with a given name and status.

    in *subsystem: Pointer to the Subsystem to initialize
    in *name: Name to assign to the Subsystem
    in status: Status value to assign to the Subsystem
    
    Returns:
        - ERR_NULL_POINTER if subsystem is NULL
        - ERR_SUCCESS if initialization is successful
*/
int subsys_init(Subsystem *subsystem, const char *name, char status) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // set the status of the subsystem to the provided status value
    subsystem->status = status;

    // copy the given name into the subsystem's name field, ensuring it doesn't exceed the max length
    strncpy(subsystem->name, name, MAX_STR-1);
    subsystem->name[MAX_STR - 1] = '\0';

    // initialize the subsystem's data field to 0
    subsystem->data = 0;

    // return success code after initializing the subsystem
    return ERR_SUCCESS;
}

/*
    Prints the attributes of a Subsystem in a formatted manner.

    in *subsystem: Pointer to the Subsystem to print

    Returns:
        - ERR_NULL_POINTER if subsystem is NULL
        - ERR_SUCCESS if printing is successful
*/
int subsys_print(Subsystem *subsystem) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // print the subsystem's name with a fixed width format
    printf("- [Name: ");
    printf("%-20s", subsystem->name);

    // print the subsystem's status using the subsys_status_print function
    subsys_status_print(subsystem);
    
    // declare a variable of type unsigned int to hold the subsystem's data
    unsigned int data = 0; 

    // get the subsystem's data and store it in the variable 'data'
    int output = subsys_data_get(subsystem, &data);

    // if there is data, print it in hexadecimal format
    if (output != ERR_NO_DATA){
        printf("            ");
        printf("Data: 0x");
        printf("%08X", data);
    }
    // close the formatted output with a bracket and new line
    printf("]\n");

    // return success code after printing the subsystem
    return ERR_SUCCESS;
}

/*
    Sets the status of a Subsystem based on given status and value.

    in *subsystem: Pointer to the Subsystem to modify
    in status: Status value to modify
    in value: Value to set the status to

    Returns:
        - ERR_NULL_POINTER if subsystem is NULL
        - ERR_INVALID_STATUS if the provided status or value is invalid
        - ERR_SUCCESS if status is set successfully
*/
int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // convert the status and value from character to integer
    int intStatus = status - '0';
    int intValue = value - '0';

    // check if the status is invalid (either 1, 3, or outside the valid range)
    if (intStatus == 1 || intStatus == 3 || intStatus > STATUS_POWER || intStatus < STATUS_RESOURCE) {
        return ERR_INVALID_STATUS;
    }

    // check if the value is invalid (outside the range 0 to 3)
    if (intValue < 0 || intValue > 3) {
        return ERR_INVALID_STATUS;
    }

    // perform the appropriate actions based on the status
    switch (intStatus) {
        // if status is RESOURCE, modify the corresponding bits based on the value
        case STATUS_RESOURCE: 
            // check if value is 0, then clear both bits 0 and 1 for resource
            if (intValue == 0) {
                subsystem->status = clearBit(subsystem->status, 0);
                subsystem->status = clearBit(subsystem->status, 1);
            } 
            // check if value is 1, then set bit 0 and clear bit 1 for resource
            else if (intValue == 1) {
                subsystem->status = setBit(subsystem->status, 0);
                subsystem->status = clearBit(subsystem->status, 1);
            } 
            // check if value is 2, then clear bit 0 and set bit 1 for resource
            else if (intValue == 2) {
                subsystem->status = clearBit(subsystem->status, 0);
                subsystem->status = setBit(subsystem->status, 1);
            } 
            // check if value is 3, then set both bits 0 and 1 for resource
            else if (intValue == 3) {
                subsystem->status = setBit(subsystem->status, 0);
                subsystem->status = setBit(subsystem->status, 1);
            }
            break;

        // if status is PERFORMANCE, modify the corresponding bits based on the value
        case STATUS_PERFORMANCE: 
            // check if value is 0, then clear both bits 2 and 3 for performance
            if (intValue == 0) {
                subsystem->status = clearBit(subsystem->status, 2);
                subsystem->status = clearBit(subsystem->status, 3);
            } 
            // check if value is 1, then set bit 2 and clear bit 3 for performance
            else if (intValue == 1) {
                subsystem->status = setBit(subsystem->status, 2);
                subsystem->status = clearBit(subsystem->status, 3);
            } 
            // check if value is 2, then clear bit 2 and set bit 3 for performance
            else if (intValue == 2) {
                subsystem->status = clearBit(subsystem->status, 2);
                subsystem->status = setBit(subsystem->status, 3);
            } 
            // check if value is 3, then set both bits 2 and 3 for performance
            else if (intValue == 3) {
                subsystem->status = setBit(subsystem->status, 2);
                subsystem->status = setBit(subsystem->status, 3);
            }
            break;

        // if status is ERROR, modify the corresponding bit based on the value
        case STATUS_ERROR: 
            // check if value is 0, then clear bit 4 for error
            if (intValue == 0) {
                subsystem->status = clearBit(subsystem->status, 4);
            } 
            // check if value is 1, then set bit 4 for error
            else if (intValue == 1) {
                subsystem->status = setBit(subsystem->status, 4);
            }
            break;

        // if status is ACTIVITY, modify the corresponding bit based on the value
        case STATUS_ACTIVITY: 
            // check if value is 0, then clear bit 5 for activity
            if (intValue == 0) {
                subsystem->status = clearBit(subsystem->status, 5);
            } 
            // check if value is 1, then set bit 5 for activity
            else if (intValue == 1) {
                subsystem->status = setBit(subsystem->status, 5);
            }
            break;

        // if status is DATA, modify the corresponding bit based on the value
        case STATUS_DATA: 
            // check if value is 0, then clear bit 6 for data
            if (intValue == 0) {
                subsystem->status = clearBit(subsystem->status, 6);
            } 
            // check if value is 1, then set bit 6 for data
            else if (intValue == 1) {
                subsystem->status = setBit(subsystem->status, 6);
            }
            break;

        // if status is POWER, modify the corresponding bit based on the value
        case STATUS_POWER:
            // check if value is 0, then clear bit 7 for power
            if (intValue == 0) {
                subsystem->status = clearBit(subsystem->status, 7);
            } 
            // check if value is 1, then set bit 7 for power
            else if (intValue == 1) {
                subsystem->status = setBit(subsystem->status, 7);
            }
            break;

        // if the status is not valid, return an error
        default:
            return ERR_INVALID_STATUS;
    }
    // return success if status is set successfully
    return ERR_SUCCESS;
}

/*
    Prints the status of a Subsystem in a compact, readable format.

    in *subsystem: Pointer to the Subsystem to display the status

    Returns:
        - ERR_NULL_POINTER if subsystem is NULL
        - ERR_SUCCESS if status is printed successfully
*/
int subsys_status_print(const Subsystem *subsystem) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // declare variables to store the status values for each subsystem attribute
    int power;
    int data;
    int activity;
    int error;
    int performance;
    int resource;

    // check the bits for resource and set the corresponding value
    if (getBit(subsystem->status, 0) == 0 && getBit(subsystem->status, 1) == 0) {
        // both bits 0 and 1 are cleared, so resource is 0
        resource = 0;  
    } else if (getBit(subsystem->status, 0) == 1 && getBit(subsystem->status, 1) == 0) {
        // bit 0 is set, bit 1 is cleared, so resource is 1
        resource = 1;  
    } else if (getBit(subsystem->status, 0) == 0 && getBit(subsystem->status, 1) == 1) {
        // bit 0 is cleared, bit 1 is set, so resource is 2
        resource = 2;  
    } else if (getBit(subsystem->status, 0) == 1 && getBit(subsystem->status, 1) == 1) {
        // both bits 0 and 1 are set, so resource is 3
        resource = 3; 
    }

    // check the bits for performance and set the corresponding value
    if (getBit(subsystem->status, 2) == 0 && getBit(subsystem->status, 3) == 0) {
        // both bits 2 and 3 are cleared, so performance is 0
        performance = 0;  
    } else if (getBit(subsystem->status, 2) == 1 && getBit(subsystem->status, 3) == 0) {
        // bit 2 is set, bit 3 is cleared, so performance is 1
        performance = 1;  
    } else if (getBit(subsystem->status, 2) == 0 && getBit(subsystem->status, 3) == 1) {
        // bit 2 is cleared, bit 3 is set, so performance is 2
        performance = 2;  
    } else if (getBit(subsystem->status, 2) == 1 && getBit(subsystem->status, 3) == 1) {
        // both bits 2 and 3 are set, so performance is 3
        performance = 3;  
    }

    // check the bit for error and set the corresponding value
    if (getBit(subsystem->status, 4) == 1) {
        // bit 4 is set, so error is 1
        error = 1;  
    } else {
        // bit 4 is cleared, so error is 0
        error = 0;  
    }

    // check the bit for activity and set the corresponding value
    if (getBit(subsystem->status, 5) == 1) {
        // bit 5 is set, so activity is 1
        activity = 1;  
    } else {
        // bit 5 is cleared, so activity is 0
        activity = 0;  
    }

    // check the bit for data and set the corresponding value
    if (getBit(subsystem->status, 6) == 1) {
        // bit 6 is set, so data is 1
        data = 1;  
    } else {
        // bit 6 is cleared, so data is 0
        data = 0;  
    }

    // check the bit for power and set the corresponding value
    if (getBit(subsystem->status, 7) == 1) {
        // bit 7 is set, so power is 1
        power = 1;  
    } else {
        // bit 7 is cleared, so power is 0
        power = 0;  
    }

    // print the status in a readable format with all the attribute values
    printf("Status: (PWR: %d| DATA: %d|ERR: %d|ACT: %d|PERF: %d|RES: %d)", power, data, error, activity, performance, resource);

    // return success after printing the status
    return ERR_SUCCESS;
}

/*
    Sets new data for a Subsystem and stores the old data if needed.

    in *subsystem: Pointer to the Subsystem to modify
    in new_data: New data to assign to the Subsystem
    in/out *old_data: Pointer to store the old data

    Returns:
        - ERR_NULL_POINTER if subsystem is NULL
        - ERR_SUCCESS if data is successfully set
*/
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // check if the old data pointer is not NULL, and store the current data of the subsystem in old_data
    if (old_data != NULL) {
        // store old data before updating
        *old_data = subsystem->data;  
    }

    // set the new data for the subsystem
    subsystem->data = new_data;

    // set the STATUS_DATA bit to indicate that new data has been set
    subsystem->status = setBit(subsystem->status, STATUS_DATA);

    // return success after data is set
    return ERR_SUCCESS;
}

/*
    Gets and resets the data of a Subsystem.

    in *subsystem: Pointer to the Subsystem to get data from
    in/out *dest: Pointer to store the retrieved data
    Returns:
        - ERR_NULL_POINTER if subsystem is NULL
        - ERR_NO_DATA if no data is available
        - ERR_SUCCESS if data is retrieved successfully
*/
int subsys_data_get(Subsystem *subsystem, unsigned int *dest) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // check if the STATUS_DATA bit is not set (indicating no data is available)
    if (getBit(subsystem->status, STATUS_DATA) == 0) {
        // set the destination to 0 since no data is available
        *dest = 0;  
        // return error indicating no data
        return ERR_NO_DATA;  
    } else {
        // store the current data in the destination pointer
        *dest = subsystem->data;  
        // reset the data after retrieval
        subsystem->data = 0;  
        // clear the STATUS_DATA bit to indicate no data
        subsystem->status = clearBit(subsystem->status, STATUS_DATA);  
        // return success after data retrieval
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
