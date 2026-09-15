# Smart Hospital & Resource Allocation System 🏥

A modular, menu-driven Smart Hospital Patient & Resource Allocation System implemented in standard C language. This system simulates daily patient intake, bed allocations across wards, emergency triage sorting, and medical billing estimations with age-based subsidies.

Developed for the CSC 1012: Introduction to Computer Programming, course at the Faculty of Applied Sciences, University of Sri Jayewardenepura.

Features

Data Lookups & Bed Matrix: Pre-defined lookup tables for doctor specialties, base fees, and ward daily rates with a 2D matrix tracking bed availability.
Patient Intake & Registration: Interactive registration collecting patient demographics, specialty choices, urgency levels, and ward stays.
Automated Billing Calculations: Computes waiting times, emergency surcharges, daily ward stay costs, age-based discounts (15% subsidy for age < 5 or > 65), and net payable amounts.
Emergency Triage Sorting: Uses a priority sorting algorithm (Selection Sort) to queue patients based on Urgency Levels (Critical → Urgent → Normal).
Analytics & Performance Reports: Generates system statistics including urgency breakdowns, revenue summaries, bed occupancy rates, and identifies the highest paying patient.
File Persistence: Automatically saves and loads bed states from `beds_status.txt` and appends patient billing histories to `patient_records.txt`.

## Project Structure

## Data Specifications & Rules

### Doctor Specialties
| ID | Specialty Name | Base Fee (LKR) | Consultation Time | Daily Cap |
| :---: | :--- | :---: | :---: | :---: |
| 1 | General Practice (OPD) | 1,500.00 | 15 mins | 30 |
| 2 | Paediatrics | 2,500.00 | 20 mins | 20 |
| 3 | Cardiology | 4,500.00 | 30 mins | 12 |
| 4 | Neurology | 5,000.00 | 30 mins | 10 |

### Hospital Wards
| ID | Ward Name | Daily Bed Rate (LKR) | Bed Capacity |
| :---: | :--- | :---: | :---: |
| 1 | General Ward | 3,000.00 | 20 |
| 2 | Paediatric Ward | 6,000.00 | 10 |
| 3 | Surgical Ward | 12,000.00 | 10 |
| 4 | ICU (Intensive Care Unit) | 25,000.00 | 05 |

### Emergency Surcharge
  Level 1 (Normal):** 0% Surcharge
  Level 2 (Urgent):** 20% of Base Fee
  Level 3 (Critical):** 50% of Base Fee
