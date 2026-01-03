#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Structure to store booking details
struct AmbulanceBooking {
    char patientName[50];
    int age;
    char emergencyType[50];
    char location[100];
    char contactNumber[15];
    int ambulanceNumber;
    int isActive; // 1 for active, 0 for cancelled
};

// Function to book ambulance
void bookAmbulance(struct AmbulanceBooking bookings[], int *bookingCount) {
    int numAmbulances;
    char temp[100];
    
    printf("\n----- BAPESSS Ambulance Booking -----\n");
    
    printf("How many ambulances are required? ");
    scanf("%d", &numAmbulances);
    getchar(); // Clear buffer
    
    for(int i = 0; i < numAmbulances; i++) {
        printf("\n--- Booking Ambulance %d/%d ---\n", i+1, numAmbulances);
        
        struct AmbulanceBooking booking;
        
        printf("Enter Patient Name (or type 'cancel' to cancel booking): ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0; // Remove newline
        
        if(strcmp(temp, "cancel") == 0) {
            printf("Booking cancelled.\n");
            return;
        }
        strcpy(booking.patientName, temp);
        
        printf("Enter Age: ");
        scanf("%d", &booking.age);
        getchar(); // Clear buffer
        
        printf("Enter Emergency Type (Accident / Cardiac / Trauma etc): ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;
        if(strcmp(temp, "cancel") == 0) {
            printf("Booking cancelled.\n");
            return;
        }
        strcpy(booking.emergencyType, temp);
        
        printf("Enter Pickup Location: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;
        if(strcmp(temp, "cancel") == 0) {
            printf("Booking cancelled.\n");
            return;
        }
        strcpy(booking.location, temp);
        
        printf("Enter Contact Number: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;
        if(strcmp(temp, "cancel") == 0) {
            printf("Booking cancelled.\n");
            return;
        }
        strcpy(booking.contactNumber, temp);
        
        // Generate dummy ambulance number
        booking.ambulanceNumber = 1000 + (*bookingCount * 7) % 9000; // Simple algorithm for dummy number
        
        booking.isActive = 1;
        
        // Add to bookings array
        bookings[*bookingCount] = booking;
        (*bookingCount)++;
        
        printf("\n Ambulance Booked Successfully!\n");
        printf("Ambulance Number: AMB-%04d\n", booking.ambulanceNumber);
        printf("Driver phone number: +91-98765%04d\n", booking.ambulanceNumber);
        printf("----------------------------------\n");
        printf("Patient Name   : %s\n", booking.patientName);
        printf("Age            : %d\n", booking.age);
        printf("Emergency Type : %s\n", booking.emergencyType);
        printf("Location       : %s\n", booking.location);
        printf("Contact Number : %s\n", booking.contactNumber);
        printf("Status         : Ambulance Dispatched \n");
    }
}

// Function to cancel ambulances
void cancelAmbulance(struct AmbulanceBooking bookings[], int bookingCount) {
    int cancelCount;
    
    printf("\n----- Cancel Ambulance Booking -----\n");
    
    if(bookingCount == 0) {
        printf("No active bookings to cancel.\n");
        return;
    }
    
    printf("Active Bookings:\n");
    for(int i = 0; i < bookingCount; i++) {
        if(bookings[i].isActive) {
            printf("%d. Ambulance Number: AMB-%04d | Patient: %s\n", 
                   i+1, bookings[i].ambulanceNumber, bookings[i].patientName);
        }
    }
    
    printf("\nHow many ambulances would you like to cancel? ");
    scanf("%d", &cancelCount);
    
    if(cancelCount <= 0) {
        printf("No ambulances cancelled.\n");
        return;
    }
    
    for(int i = 0; i < cancelCount; i++) {
        int ambulanceNum;
        printf("\nEnter ambulance number to cancel (without AMB- prefix, e.g., 1001): ");
        scanf("%d", &ambulanceNum);
        
        int found = 0;
        for(int j = 0; j < bookingCount; j++) {
            if(bookings[j].ambulanceNumber == ambulanceNum && bookings[j].isActive) {
                bookings[j].isActive = 0;
                printf("Cancelled booking for Ambulance AMB-%04d\n", ambulanceNum);
                printf("Patient: %s | Contact: %s\n", 
                       bookings[j].patientName, bookings[j].contactNumber);
                found = 1;
                break;
            }
        }
        
        if(!found) {
            printf("Ambulance AMB-%04d not found or already cancelled.\n", ambulanceNum);
        }
    }
}

// Main function
int main() {
    int choice;
    struct AmbulanceBooking bookings[100]; // Array to store bookings
    int bookingCount = 0;
    
    printf("====================================\n");
    printf("   BAPESSS Ambulance Booking System   \n");
    printf("====================================\n");

    while (1) {
        printf("\n1. Book Ambulance");
        printf("\n2. Cancel Ambulance");
        printf("\n3. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                bookAmbulance(bookings, &bookingCount);
                break;
            case 2:
                cancelAmbulance(bookings, bookingCount);
                break;
            case 3:
                printf("\nThank you for using BAPESSS \n");
                return 0;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    }
}