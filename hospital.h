#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4


const char *SPECIALTY_NAMES[NUM_SPECIALTIES];
const double BASE_FEES[NUM_SPECIALTIES];
const int CONSULTATION_TIMES[NUM_SPECIALTIES];

const char *WARD_NAMES[NUM_WARDS];
const double WARD_RATES[NUM_WARDS];
const int WARD_CAPACITIES[NUM_WARDS];

// Function Prototypes
void displayMenu();
void registerPatient();
void calculateBillingInfo(int index);
void displayPatientBill(int index);
void displayTriageSortedPatients();
void generatePerformanceReport();
void saveBedStatusToFile();
void loadBedStatusFromFile();
void appendPatientRecordToFile(int index);

#endif // HOSPITAL_H
