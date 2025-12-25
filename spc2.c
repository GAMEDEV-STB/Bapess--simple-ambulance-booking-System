#include <stdio.h>
#include <string.h>

// Structure to store booking details
struct AmbulanceBooking {
    char patientName[50];
    int age;
    char emergencyType[50];
    char location[100];
    char contactNumber[15];
};

// Function to book ambulance
void bookAmbulance() {
    struct AmbulanceBooking booking;

    printf("\n----- BAPESS Ambulance Booking -----\n");

    printf("Enter Patient Name: ");
    getchar(); // clear buffer
    fgets(booking.patientName, sizeof(booking.patientName), stdin);

    printf("Enter Age: ");
    scanf("%d", &booking.age);

    printf("Enter Emergency Type (Accident / Cardiac / Trauma etc): ");
    getchar();
    fgets(booking.emergencyType, sizeof(booking.emergencyType), stdin);

    printf("Enter Pickup Location: ");
    fgets(booking.location, sizeof(booking.location), stdin);

    printf("Enter Contact Number: ");
    fgets(booking.contactNumber, sizeof(booking.contactNumber), stdin);

    printf("\n🚑 Ambulance Booked Successfully!\n");
    printf("----------------------------------\n");
    printf("Patient Name   : %s", booking.patientName);
    printf("Age            : %d\n", booking.age);
    printf("Emergency Type : %s", booking.emergencyType);
    printf("Location       : %s", booking.location);
    printf("Contact Number : %s", booking.contactNumber);
    printf("Status         : Ambulance Dispatched \n");
}

// Main function
int main() {
    int choice;

    printf("====================================\n");
    printf("   BApESS Ambulance Booking System   \n");
    printf("====================================\n");

    while (1) {
        printf("\n1. Book Ambulance");
        printf("\n2. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                bookAmbulance();
                break;
            case 2:
                printf("\nThank you for using BApESS 🚑\n");
                return 0;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    }
}
