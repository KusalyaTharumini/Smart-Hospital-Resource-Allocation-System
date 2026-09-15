#include "hospital.h"

const char *SPECIALTY_NAMES[NUM_SPECIALTIES] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const double BASE_FEES[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5000.00};
const int CONSULTATION_TIMES[NUM_SPECIALTIES] = {15, 20, 30, 30};

const char *WARD_NAMES[NUM_WARDS] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const double WARD_RATES[NUM_WARDS] = {3000.00, 6000.00, 12000.00, 25000.00};
const int WARD_CAPACITIES[NUM_WARDS] = {20, 10, 10, 5};


int bedOccupancy[4][20] = {0}; 
int specialtyQueueCounts[NUM_SPECIALTIES] = {0};

int patientIDs[MAX_PATIENTS];
char patientNames[MAX_PATIENTS][50];
int patientAges[MAX_PATIENTS];
int urgencyLevels[MAX_PATIENTS];
int selectedSpecialties[MAX_PATIENTS]; 
int isAdmitted[MAX_PATIENTS]; 
int assignedWards[MAX_PATIENTS]; 
int daysAdmitted[MAX_PATIENTS];

double baseFees[MAX_PATIENTS];
double surcharges[MAX_PATIENTS];
double wardCosts[MAX_PATIENTS];
double grossTotals[MAX_PATIENTS];
double discounts[MAX_PATIENTS];
double finalAmounts[MAX_PATIENTS];
double waitTimes[MAX_PATIENTS];

int totalPatients = 0;

void displayMenu() {
    printf("\n=============================================\n");
    printf("   SMART HOSPITAL RESOURCE ALLOCATION SYSTEM \n");
    printf("=============================================\n");
    printf("1. Register New Patient Intake & Bill\n");
    printf("2. Display Emergency Triage Priority List\n");
    printf("3. Generate Analytics & Performance Report\n");
    printf("4. Save Bed Status to File\n");
    printf("5. Exit System\n");
    printf("=============================================\n");
}

void registerPatient() {
    if (totalPatients >= MAX_PATIENTS) {
        printf("Error: System total patient capacity reached!\n");
        return;
    }

    int idx = totalPatients;
    patientIDs[idx] = 1001 + idx;

    printf("\n--- Patient Registration (ID: PAT-%d) ---\n", patientIDs[idx]);

    printf("Enter Patient Name: ");
    getchar(); 
    fgets(patientNames[idx], sizeof(patientNames[idx]), stdin);
    patientNames[idx][strcspn(patientNames[idx], "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &patientAges[idx]);

    do {
        printf("Enter Urgency Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
        scanf("%d", &urgencyLevels[idx]);
    } while (urgencyLevels[idx] < 1 || urgencyLevels[idx] > 3);

    printf("\nSpecialties:\n");
    for (int i = 0; i < NUM_SPECIALTIES; i++) {
        printf("  %d. %s (Fee: LKR %.2f)\n", i + 1, SPECIALTY_NAMES[i], BASE_FEES[i]);
    }
    do {
        printf("Select Specialty ID (1-%d): ", NUM_SPECIALTIES);
        scanf("%d", &selectedSpecialties[idx]);
    } while (selectedSpecialties[idx] < 1 || selectedSpecialties[idx] > NUM_SPECIALTIES);
    selectedSpecialties[idx] -= 1;

    printf("\nIs Admitted to Ward? (1 = Yes, 0 = No): ");
    scanf("%d", &isAdmitted[idx]);

    if (isAdmitted[idx] == 1) {
        printf("Wards:\n");
        for (int i = 0; i < NUM_WARDS; i++) {
            printf("  %d. %s (Rate: LKR %.2f/day, Cap: %d)\n", i + 1, WARD_NAMES[i], WARD_RATES[i], WARD_CAPACITIES[i]);
        }
        do {
            printf("Select Ward ID (1-%d): ", NUM_WARDS);
            scanf("%d", &assignedWards[idx]);
        } while (assignedWards[idx] < 1 || assignedWards[idx] > NUM_WARDS);
        assignedWards[idx] -= 1;

        printf("Enter Days Admitted: ");
        scanf("%d", &daysAdmitted[idx]);

        int bedAssigned = -1;
        int wardIndex = assignedWards[idx];
        for (int b = 0; b < WARD_CAPACITIES[wardIndex]; b++) {
            if (bedOccupancy[wardIndex][b] == 0) {
                bedOccupancy[wardIndex][b] = 1;
                bedAssigned = b + 1;
                break;
            }
        }
        if (bedAssigned == -1) {
            printf("Warning: No free beds available in %s! Ward admission canceled.\n", WARD_NAMES[wardIndex]);
            isAdmitted[idx] = 0;
            assignedWards[idx] = -1;
            daysAdmitted[idx] = 0;
        }
    } else {
        assignedWards[idx] = -1;
        daysAdmitted[idx] = 0;
    }

    calculateBillingInfo(idx);
    displayPatientBill(idx);
    appendPatientRecordToFile(idx);

    totalPatients++;
}

void calculateBillingInfo(int idx) {
    int specIdx = selectedSpecialties[idx];

    
    waitTimes[idx] = specialtyQueueCounts[specIdx] * CONSULTATION_TIMES[specIdx];
    specialtyQueueCounts[specIdx]++;

    
    baseFees[idx] = BASE_FEES[specIdx];
    if (urgencyLevels[idx] == 1) {
        surcharges[idx] = 0.0;
    } else if (urgencyLevels[idx] == 2) {
        surcharges[idx] = baseFees[idx] * 0.20;
    } else {
        surcharges[idx] = baseFees[idx] * 0.50;
    }

    
    if (isAdmitted[idx] == 1) {
        wardCosts[idx] = daysAdmitted[idx] * WARD_RATES[assignedWards[idx]];
    } else {
        wardCosts[idx] = 0.0;
    }

    
    grossTotals[idx] = baseFees[idx] + surcharges[idx] + wardCosts[idx];

    
    if (patientAges[idx] < 5 || patientAges[idx] > 65) {
        discounts[idx] = grossTotals[idx] * 0.15;
    } else {
        discounts[idx] = 0.0;
    }

    
    finalAmounts[idx] = grossTotals[idx] - discounts[idx];
}

void displayPatientBill(int idx) {
    printf("\n-------------------------------------------------------------\n");
    printf("             SMART HOSPITAL ADMISSION & BILL                 \n");
    printf("-------------------------------------------------------------\n");
    printf("Patient ID              : PAT-%d\n", patientIDs[idx]);
    printf("Patient Name            : %s\n", patientNames[idx]);
    printf("Age                     : %d Years %s\n", patientAges[idx],
           (patientAges[idx] < 5 || patientAges[idx] > 65) ? "(15% Subsidy Eligible)" : "");
    printf("Specialty               : %s\n", SPECIALTY_NAMES[selectedSpecialties[idx]]);

    if (isAdmitted[idx] == 1) {
        printf("Assigned Ward           : %s\n", WARD_NAMES[assignedWards[idx]]);
    } else {
        printf("Assigned Ward           : Outpatient (OPD)\n");
    }

    printf("Urgency Level           : Level %d (%s)\n", urgencyLevels[idx],
           urgencyLevels[idx] == 3 ? "Critical" : (urgencyLevels[idx] == 2 ? "Urgent" : "Normal"));
    printf("Base Consultation Fee   : LKR %.2f\n", baseFees[idx]);
    printf("Emergency Surcharge     : LKR %.2f\n", surcharges[idx]);
    printf("Ward Stay Cost (%d Days): LKR %.2f\n", daysAdmitted[idx], wardCosts[idx]);
    printf("Gross Total Bill        : LKR %.2f\n", grossTotals[idx]);
    printf("Age Subsidy Discount    : LKR -%.2f\n", discounts[idx]);
    printf("Final Payable Amount    : LKR %.2f\n", finalAmounts[idx]);

    if (urgencyLevels[idx] == 3) {
        printf("Estimated Waiting Time  : 0.00 mins (Immediate Attention)\n");
    } else {
        printf("Estimated Waiting Time  : %.2f mins\n", waitTimes[idx]);
    }
    printf("-------------------------------------------------------------\n");
}

void displayTriageSortedPatients() {
    if (totalPatients == 0) {
        printf("\nNo patient records available.\n");
        return;
    }

    int order[MAX_PATIENTS];
    for (int i = 0; i < totalPatients; i++) order[i] = i;

    // Selection Sort by Urgency Level
    for (int i = 0; i < totalPatients - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < totalPatients; j++) {
            if (urgencyLevels[order[j]] > urgencyLevels[order[maxIdx]]) {
                maxIdx = j;
            }
        }
        int temp = order[i];
        order[i] = order[maxIdx];
        order[maxIdx] = temp;
    }

    printf("\n=========================================================================\n");
    printf("                     TRIAGE SORTED PATIENT QUEUE                         \n");
    printf("=========================================================================\n");
    printf("%-10s %-20s %-10s %-15s %-15s\n", "ID", "Name", "Urgency", "Specialty", "Final Bill (LKR)");
    printf("-------------------------------------------------------------------------\n");

    for (int i = 0; i < totalPatients; i++) {
        int idx = order[i];
        char levelStr[15];
        sprintf(levelStr, "L%d (%s)", urgencyLevels[idx],
                urgencyLevels[idx] == 3 ? "Critical" : (urgencyLevels[idx] == 2 ? "Urgent" : "Normal"));

        printf("PAT-%-6d %-20s %-10s %-15s %-15.2f\n",
               patientIDs[idx], patientNames[idx], levelStr, SPECIALTY_NAMES[selectedSpecialties[idx]], finalAmounts[idx]);
    }
    printf("-------------------------------------------------------------------------\n");
}

void generatePerformanceReport() {
    if (totalPatients == 0) {
        printf("\nNo data available to generate performance report.\n");
        return;
    }

    int countL1 = 0, countL2 = 0, countL3 = 0;
    double totalRevenue = 0.0, totalDiscounts = 0.0;
    int highestBillIdx = 0;

    for (int i = 0; i < totalPatients; i++) {
        if (urgencyLevels[i] == 1) countL1++;
        else if (urgencyLevels[i] == 2) countL2++;
        else if (urgencyLevels[i] == 3) countL3++;

        totalRevenue += finalAmounts[i];
        totalDiscounts += discounts[i];

        if (finalAmounts[i] > finalAmounts[highestBillIdx]) {
            highestBillIdx = i;
        }
    }

    printf("\n=====================================================\n");
    printf("             PERFORMANCE & ANALYTICS REPORT          \n");
    printf("=====================================================\n");
    printf("Total Patients Registered: %d\n", totalPatients);
    printf("  - Level 1 (Normal)     : %d\n", countL1);
    printf("  - Level 2 (Urgent)     : %d\n", countL2);
    printf("  - Level 3 (Critical)   : %d\n", countL3);
    printf("-----------------------------------------------------\n");
    printf("Total Revenue Earned     : LKR %.2f\n", totalRevenue);
    printf("Total Discounts Granted  : LKR %.2f\n", totalDiscounts);
    printf("-----------------------------------------------------\n");
    printf("Bed Occupancy Rate per Ward:\n");

    for (int w = 0; w < NUM_WARDS; w++) {
        int occupiedCount = 0;
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupiedCount++;
        }
        double percentage = ((double)occupiedCount / WARD_CAPACITIES[w]) * 100.0;
        printf("  - %-15s: %.2f%% (%d/%d beds)\n", WARD_NAMES[w], percentage, occupiedCount, WARD_CAPACITIES[w]);
    }

    printf("-----------------------------------------------------\n");
    printf("Highest Paying Patient   : %s (LKR %.2f)\n", patientNames[highestBillIdx], finalAmounts[highestBillIdx]);
    printf("=====================================================\n");
}

void saveBedStatusToFile() {
    FILE *fp = fopen("beds_status.txt", "w");
    if (fp == NULL) return;

    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void loadBedStatusFromFile() {
    FILE *fp = fopen("beds_status.txt", "r");
    if (fp == NULL) return;

    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            if (fscanf(fp, "%d", &bedOccupancy[w][b]) != 1) {
                bedOccupancy[w][b] = 0;
            }
        }
    }
    fclose(fp);
}

void appendPatientRecordToFile(int idx) {
    FILE *fp = fopen("patient_records.txt", "a");
    if (fp == NULL) return;

    fprintf(fp, "PAT-%d | %s | Age: %d | Urgency: %d | Specialty: %s | Final Bill: LKR %.2f\n",
            patientIDs[idx], patientNames[idx], patientAges[idx], urgencyLevels[idx],
            SPECIALTY_NAMES[selectedSpecialties[idx]], finalAmounts[idx]);

    fclose(fp);
}
