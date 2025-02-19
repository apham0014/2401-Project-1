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

    Subsystem subsystem;
    
    while (1){
        char name[MAX_STR];
        char newName[MAX_STR];
        unsigned char newStatus;
        unsigned char newValue;

        print_menu(choice);
        switch (*choice) {
            case MENU_ADD:
            
                printf("Enter the name of the new subsystem: ");
                scanf("%31s", newName);

                subsys_init(&subsystem, newName, 0);
                subsys_append(&subsystems, &subsystem);

                printf("The status for this one is: ");
                printBits(subsystem.status);
                
                break;

            case MENU_PRINT:
                subsys_print(&subsystem);
                break;
            case MENU_PRINTALL:
                subsys_collection_print(&subsystems);
                break;
            
            case MENU_STATUS:
                printf("Enter the name of the system you would like to change the status of: ");
                scanf(" %s", name);
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

                printf("What status would you like to set: ");
                scanf(" %c", &newStatus);
                printf("What value would you like to set: ");
                scanf(" %c", &newValue);
                subsys_status_set(subsystem, newStatus, newValue);

                printf("The status for this one is: ");
                printBits(subsystem->status);
                
                break;

            case MENU_EXIT:
                return ERR_SUCCESS;
        }
    }

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