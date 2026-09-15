#include "hospital.h"

int main() {
    loadBedStatusFromFile(); 
    int choice;

    do {
        displayMenu();
        printf("Enter your choice (1-5): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayTriageSortedPatients();
                break;
            case 3:
                generatePerformanceReport();
                break;
            case 4:
                saveBedStatusToFile();
                printf("Bed statuses saved successfully.\n");
                break;
            case 5:
                saveBedStatusToFile();
                printf("Exiting system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
