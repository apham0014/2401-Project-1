// Ahmad Baytamouni 101335293
// Austin Pham 101333594

#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

/*
    Constructor for SubsystemCollection.

    in/out *subsystems: Pointer to the SubsystemCollection to initialize

    Returns:
        - ERR_NULL_POINTER if subsystems is NULL
        - ERR_SUCCESS if initialization is successful
*/
int subsys_collection_init(SubsystemCollection *subsystems) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    // initialize the size of the collection to 0
    subsystems->size = 0;
    
    // return success code after initializing the subsystem collection
    return ERR_SUCCESS;
}

/*
    Adds a new Subsystem to the SubsystemCollection.

    in *subsystems: Pointer to the SubsystemCollection to append to
    in *subsystem: Pointer to the Subsystem to add

    Returns:
        - ERR_NULL_POINTER if subsystems or subsystem is NULL
        - ERR_MAX_CAPACITY if the subsystemCollection is full
        - ERR_SUCCESS if the subsystem is successfully added
*/
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem) {
    // check if the subsystem pointer or subsystem collection pointer is NULL and return an error if so
    if (subsystems == NULL || subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // check if the collection is full
    if (subsystems->size != MAX_ARR) {
        // add the new subsystem to the collection
        subsystems->subsystems[subsystems->size] = *subsystem;
        subsystems->size++;
        return ERR_SUCCESS;
    } else {
        // return error if the collection is at maximum capacity
        return ERR_MAX_CAPACITY;
    }
}

/*
    Finds a Subsystem by its name.

    in *subsystems: Pointer to the SubsystemCollection to search in
    in *name: Name of the Subsystem to find

    Returns:
        - ERR_NULL_POINTER if subsystems or name is NULL
        - ERR_SYS_NOT_FOUND if no subsystem with the given name is found
        - Index of the found subsystem if it exists
*/
int subsys_find(const SubsystemCollection *subsystems, const char *name) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystems == NULL || name == NULL) {
        return ERR_NULL_POINTER;
    }

    // iterate through the subsystems collection to find the matching name
    for (int i = 0; i < subsystems->size; i++) {
        // compare the subsystem name with the provided name
        if (strcmp(subsystems->subsystems[i].name, name) == 0) {
            // return the index of the found subsystem
            return i;  
        }
    }
    // return error if subsystem not found in the subsystem collection   
    return ERR_SYS_NOT_FOUND;  
}

/*
    Prints each subsystem from the SubsystemCollection.

    in *subsystems: Pointer to the SubsystemCollection to print

    Returns:
        - ERR_NULL_POINTER if subsystems is NULL
        - ERR_NO_DATA if no subsystems exist in the collection
        - ERR_SUCCESS if the subsystems are printed successfully
*/
int subsys_collection_print(SubsystemCollection *subsystems) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    // check if the collection is empty
    if (subsystems->size == 0) {
        return ERR_NO_DATA;
    }
    
    // iterate through the collection and print each subsystem
    for (int i = 0; i < subsystems->size; i++) {
        subsys_print(&(subsystems->subsystems[i]));
    }
    // return success code after printing the subsystem collection
    return ERR_SUCCESS;
}

/*
    Removes a subsystem from the SubsystemCollection by its index.

    in/out *subsystems: Pointer to the SubsystemCollection to modify
    in index: Index of the Subsystem to remove

    Returns:
        - ERR_NULL_POINTER if subsystems is NULL
        - ERR_INVALID_INDEX if the index is out of range
        - ERR_SUCCESS if the subsystem is successfully removed
*/
int subsys_remove(SubsystemCollection *subsystems, int index) {
    // check if the subsystem pointer is NULL and return an error if so
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    // check if the index exists in the subsystem 
    if (index >= subsystems->size || index < 0) {
        return ERR_NO_DATA;
    }

    // shift all subsystems after the removed one to the left
    for (int i = index; i < subsystems->size - 1; i++) {
        subsystems->subsystems[i] = subsystems->subsystems[i + 1];
    }

    // clear the last subsystem in the subsystem collection
    Subsystem *lastSubsystem = &subsystems->subsystems[subsystems->size - 1];
    lastSubsystem->name[0] = '\0';
    lastSubsystem->status = 0;
    lastSubsystem->data = 0;

    // decrease the size of the subsystem collection by 1
    subsystems->size--;

    // return success code after removing the subsystem from the subsystem collection
    return ERR_SUCCESS;
}

/*
    Filters Subsystems based on the provided filter.

    in *src: Pointer to the source SubsystemCollection to filter
    in/out *dest: Pointer to the destination SubsystemCollection to store filtered subsystems
    in *filter: Filter to apply (binary string and wildcard)

    Returns:
        - ERR_SYS_NOT_FOUND if the source or destination collections are NULL or filter is invalid
        - ERR_SUCCESS if filtering is done successfully
*/
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter){
    // check for NULL parameters
    if (src == NULL || dest == NULL || filter == NULL) {
        return ERR_SYS_NOT_FOUND;
    }

    // declare a variable of type unsigned char to store the mask for '1' in the filter
    unsigned char filterMask = 0;
    // declare a variable of type unsighed char to store the mask for '*' (wildcard) in the filter
    unsigned char wildCardMask = 0;

    // initialize the filter and wildcard masks
    for (int i = 0; i<8; i++) {
        // if the filter has a '1', set the corresponding bit in filterMask
        if (filter[i] == '1') {
            // set bit in filterMask for '1' at position (7 - i)
            filterMask = setBit(filterMask, 7 - i);
        }
        // if the filter has a '*', set the corresponding bit in wildCardMask
        if (filter[i] == '*') {
            // set bit in wildCardMask for '*' at position (7 - i)
            wildCardMask = setBit(wildCardMask, 7 - i);
        }
    }
    // invert the filterMask, as we want to match '0' in the filter (negated)
    filterMask = ~filterMask;
    
    // copy all subsystems from the source to the destination collection
    dest->size = 0;
    for (int i = 0; i < src->size; i++) {
        dest->subsystems[i] = src->subsystems[i];
        // increment the destination collection size
        dest->size++;
    }

    // iterate backward through the destination collection to handle possible shifting issues when removing subsystems
    for (int i = dest->size - 1; i >= 0; i--) {
        // compare the status of the current subsystem with the filterMask, applying the wildcard mask as needed
        unsigned char result = (filterMask ^ dest->subsystems[i].status) | wildCardMask;
        // negate the result
        result = ~result;

        // if result is non-zero, the subsystem does not match the filter, so remove it from the destination collection
        if (result != 0) {  
            // remove the subsystem at index i
            subsys_remove(dest, i); 
        }
    }
    // return success after filtering is complete
    return ERR_SUCCESS;
}
