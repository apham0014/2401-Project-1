#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7

int print_menu(int *choice);

int main() {
    // Use a while loop to print the menu for the user and call the appropriate functions.
    // The definitions above are provided to assist with this.

    SubsystemCollection subsystems;
    subsys_collection_init(&subsystems);

    int space = 0;
    int *choice = &space;
    unsigned int oldValue= 0;
    unsigned int *oldData = &oldValue;
    Subsystem subsystem;
    SubsystemCollection filteredSubsystem;
    int index = 0;
    
    while (1){
        char name[MAX_STR];
        char newName[MAX_STR];
        unsigned char newStatus;
        unsigned char newValue;
        unsigned int newData;
        char filterInput[9];

        print_menu(choice);
        switch (*choice) {

            // add a new subsystem
            case MENU_ADD:
            
                printf("Enter a name for the new subsystem (no spaces): ");
                scanf("%31s", newName);

                subsys_init(&subsystem, newName, 0);
                subsys_append(&subsystems, &subsystem);

                printBits(subsystem.status);
                
                break;

            // print the info of a specific subsys
            case MENU_PRINT:
                printf("Enter subsystem name to print: ");
                scanf(" %s", name);
                Subsystem *subsystem1 = NULL;
                int flag1 = 0;
                // iterate through collection to match name
                for (int i = 0; i < subsystems.size; i++){
                    if (strcmp(subsystems.subsystems[i].name, name) == 0){
                        subsystem1 = &subsystems.subsystems[i];
                        flag1 = 1;
                        break;
                    }
                }

                if (flag1 == 0){
                    return ERR_SYS_NOT_FOUND;
                }else{
                    subsys_print(subsystem1);
                }                
                break;

            // print the info of all subsys in a collection
            case MENU_PRINTALL:
                subsys_collection_print(&subsystems);
                break;
            
            // change the status of a subsys
            case MENU_STATUS:
                printf("Enter: <Subsystem Name> <Status ID (7,6,5,4,2, or 0)> <New Value (0-3)>: ");

                scanf("%s %c %c", name, &newStatus, &newValue);

                Subsystem *subsystem = NULL;
                int flag = 0;
                // iterate through collection to match name
                for (int i = 0; i < subsystems.size; i++){
                    if (strcmp(subsystems.subsystems[i].name, name) == 0){
                        subsystem = &subsystems.subsystems[i];
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0){
                    return ERR_SYS_NOT_FOUND;
                }

                if (newValue == 2 || newValue == 3){
                    if (newStatus == 0 || newStatus == 2){
                        continue;
                    }else{
                        return ERR_INVALID_STATUS;
                    }
                }

                subsys_status_set(subsystem, newStatus, newValue);
                printBits(subsystem->status);
                printf("Status updated successfully.");
                break;

            case MENU_REMOVE:
            printf("Enter subsystem name to remove: ");
            scanf("%s", name);
            printf("%s\n", name);
            index = subsys_find(&subsystems, name);

            if (index == ERR_SYS_NOT_FOUND) {
                printf("Error: Subsystem '%s' not found.", name);
            }

            printf("%d\n", index);
            subsys_remove(&subsystems, index);
            printf("Subsystem '%s' removed successfully.", name);
            break;

            // filter out 
            case MENU_FILTER:
                printf("Enter filter string (8 characters of 1, 0, *)");
                scanf("%8s", filterInput);
                
                // check the string is validly formatted.
                if (strlen(filterInput) != 8) {
                    return ERR_INVALID_INDEX;
                }
                
                for (int i = 0; i<8; i++){
                    if (filterInput[i] != '1' && filterInput[i] != '0' && filterInput[i] != '*'){
                        return ERR_INVALID_INDEX;
                    }
                }

                // SubsystemCollection filteredSubsystem;
                subsys_collection_init(&filteredSubsystem);

                subsys_filter(&subsystems, &filteredSubsystem, (const unsigned char *)filterInput);
                subsystems = filteredSubsystem;
                break;

            // set the data of a subsys to a new value.
            case MENU_DATA:
                printf("Enter: <Subsystem Name> <Data, uppercase hex without 0x>: ");
                // scanf(" %s", name);

                // max 8 characters + null terminator
                char hexInput[9];
                scanf(" %s %8s", name, hexInput);

                int isValid = 1;

                for (int i = 0; hexInput[i] != '\0'; i++) {
                    if (!(hexInput[i] >= '0' && hexInput[i] <= '9') &&  // Check 0-9
                        !(hexInput[i] >= 'A' && hexInput[i] <= 'F')) {  // Check A-F (uppercase)
                        isValid = 0;
                    }
                }

                if (!isValid) {
                    printf("Error: Hex value does not exist. ");
                    break;
                }
                
                sscanf(hexInput, "%X", &newData);

                //scanf(" %s %08x", name, &newData);

                index = subsys_find(&subsystems, name);

                if (index == ERR_SYS_NOT_FOUND) {
                    printf("Error: Subsystem '%s' not found.", name);
                }
                
                Subsystem *subsystem2 = NULL;
                int flag2 = 0;
                // iterate through collection to match name
                for (int i = 0; i < subsystems.size; i++){
                    if (strcmp(subsystems.subsystems[i].name, name) == 0){
                        subsystem2 = &subsystems.subsystems[i];
                        flag2 = 1;
                        break;
                    }
                }

                if (flag2 == 0){
                    return ERR_SYS_NOT_FOUND;
                }else{
                    // now if there was data queued, we must print the subsystem before calling the set function.
                    if (getBit(subsystem2->status, 6) != 0){
                        printf("The subsystem data before the change is: ");
                        subsys_print(subsystem2);
                    }
                    subsys_data_set(subsystem2, newData, oldData);
                    printf("The data has now been sucessfully changed to the new value. \n");
                }                
                break;
                
            case MENU_EXIT:
                return ERR_SUCCESS;
        }
    }
    printf("The main function has terminated.");
    return 0;
}


/* 
    Prints a menu to the user and prompts them until they enter a valid menu option.

    out: *choice, an integer corresponding to a MENU_ definition defining the choice made by the user.

    Returns: 
        - ERR_SUCCESS if there is no error
        - ERR_NULL_POINTER if `choice` is NULL

*/
int print_menu(int *choice) {
    if (choice == NULL) {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if (*choice < MENU_MIN || *choice > MENU_MAX) {
            printf("Error: Invalid choice. Please try again.\n");
        }
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}