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

        if (subsystems->size == 0) {
            return ERR_NO_DATA;
        } else {
            subsys_print(&(subsystems->subsystems[i]));
        }
    }
    return ERR_SUCCESS;
}


