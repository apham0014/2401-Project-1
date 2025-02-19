#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

// constructor for subsystemCollection.
int subsys_collection_init(SubsystemCollection *subsystems){
    subsystems->size = 0;

    return ERR_SUCCESS;
}

// function for adding a new subsystem to the subsystemCollection.
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem){
    
    if (subsystems->size != MAX_ARR){
        subsystems->subsystems[subsystems->size] = *subsystem;
        subsystems->size++;
        return ERR_SUCCESS;
        
    }else{
        return ERR_MAX_CAPACITY;
    }
}

// function that looks through the subsystem collection and trys to find if there is matching name in the subsystem to the name parameter
int subsys_find(const SubsystemCollection *subsystems, const char *name) {
    for (int i = 0; i < subsystems->size; i++) {
        if (subsystems->subsystems[i].name == name) {
            return i;
        } else {
            return ERR_SYS_NOT_FOUND;
        }
    }
    return ERR_SUCCESS;       
}

// function that prints each subsystem from the subsystem collection
int subsys_collection_print(SubsystemCollection *subsystems) {
    for (int i = 0; i < subsystems->size; i++) {
        subsys_print(&(subsystems->subsystems[i]));
    }
    
    return ERR_SUCCESS;
}

// function that removes a subsystems from the subsystem collection
int subsys_remove(SubsystemCollection *subsystems, int index) {

    // check if the index exists in the subsystem 
    if (index >= subsystems->size || index < 0) {
        return ERR_INVALID_INDEX;
    }

    // shift all the subsystems to the left of the removed index
    for (int i = index; i < subsystems->size - 1; i++) {
        subsystems->subsystems[i] = subsystems->subsystems[i+1];
    }

    // clear the last element in the subsystem collection
    Subsystem *lastSubsystem = &subsystems->subsystems[subsystems->size - 1];
    lastSubsystem->name[0] = '\0';
    lastSubsystem->status = 0;
    lastSubsystem->data = 0;

    // decrease the size of the subsystem collection by 1
    subsystems->size--;

    return ERR_SUCCESS;
}

int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter){
    char filterMask = 0;
    char wildCardMask = 0;

    // initialise both masks
    for (int i = 0; i<8; i++){
        if (filter[i] == '1'){
            filterMask = setBit(filterMask, 7-i);
        }
        if (filter[i] == '*'){
            wildCardMask = setBit(wildCardMask, 7-i);
        }
    }
    filterMask = ~filterMask;
    
    // copy src to dest
    dest->size = 0;
    for (int i = 0; i < src->size; i++){
        dest->subsystems[i] = src->subsystems[i];
        dest->size++;
    }

    // now we complete the operation.
    int counter = 0;
    for (int i = 0; i < src->size; i++){
        char result = (filterMask ^ dest->subsystems[counter].status) | (wildCardMask);
        result = ~result;

        if (result == 0){
            subsys_remove(dest, counter);
            
        }else{
            counter++;
        }
    }
    return ERR_SUCCESS;
    
}



