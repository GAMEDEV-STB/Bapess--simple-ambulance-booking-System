#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

// =============================================
// STRUCTURE DEFINITIONS
// =============================================

// Structure to represent an Ambulance
typedef struct {
    int ambulance_id;          // Unique ID for the ambulance
    char vehicle_number[20];   // License plate number
    char driver_name[50];      // Name of the driver
    char driver_contact[15];   // Driver's contact number
    int type;                  // 1=Basic, 2=Advanced, 3=Mobile ICU
    int status;                // 0=Available, 1=Booked, 2=On Trip, 3=Maintenance
    float location_x;          // Current location coordinates
    float location_y;
} Ambulance;

// Structure to represent a Booking
typedef struct {
    int booking_id;            // Unique booking ID
    char patient_name[100];    // Name of the patient
    char patient_contact[15];  // Patient's contact number
    char pickup_location[200]; // Pickup address
    char hospital[100];        // Destination hospital
    int ambulance_id;          // Ambulance assigned
    char booking_time[50];     // Time of booking
    char pickup_time[50];      // Time of pickup
    int emergency_level;       // 1=Normal, 2=Urgent, 3=Critical
    int status;                // 0=Pending, 1=Confirmed, 2=Dispatched, 3=Completed, 4=Cancelled
} Booking;

// Structure to manage the system
typedef struct {
    Ambulance* ambulances;     // Dynamic array of ambulances
    Booking* bookings;         // Dynamic array of bookings
    int ambulance_count;       // Current number of ambulances
    int booking_count;         // Current number of bookings
    int ambulance_capacity;    // Capacity of ambulances array
    int booking_capacity;      // Capacity of bookings array
} BAPESSS_System;

// =============================================
// FUNCTION PROTOTYPES
// =============================================
BAPESSS_System* create_system();
void free_system(BAPESSS_System* system);
void display_menu();
void add_sample_data(BAPESSS_System* system);
void book_ambulance(BAPESSS_System* system);
void view_bookings(BAPESSS_System* system);
void view_ambulances(BAPESSS_System* system);
void update_booking_status(BAPESSS_System* system);
void cancel_booking(BAPESSS_System* system);
void add_ambulance(BAPESSS_System* system);
void find_nearest_ambulance(BAPESSS_System* system, float loc_x, float loc_y);
void generate_report(BAPESSS_System* system);
void save_data(BAPESSS_System* system);
void load_data(BAPESSS_System* system);
int get_choice();
void clear_input_buffer();
void get_current_time(char* buffer, int size);
int find_available_ambulance(BAPESSS_System* system, int emergency_level);

// =============================================
// MAIN FUNCTION
// =============================================
int main() {
    printf("========================================\n");
    printf("   BAPESSS AMBULANCE BOOKING SYSTEM\n");
    printf("========================================\n");
    printf("B - Book Ambulance\n");
    printf("A - View Ambulances\n");
    printf("P - Patient Bookings\n");
    printf("E - Emergency Services\n");
    printf("S - System Management\n");
    printf("S - Status Updates\n");
    printf("S - Statistics & Reports\n");
    printf("========================================\n\n");
    
    // Create the system
    BAPESSS_System* system = create_system();
    if (!system) {
        printf("Error: Could not initialize system!\n");
        return 1;
    }
    
    // Add sample data for demonstration
    add_sample_data(system);
    
    int choice;
    do {
        display_menu();
        choice = get_choice();
        
        switch(choice) {
            case 1:
                book_ambulance(system);
                break;
            case 2:
                view_bookings(system);
                break;
            case 3:
                view_ambulances(system);
                break;
            case 4:
                update_booking_status(system);
                break;
            case 5:
                cancel_booking(system);
                break;
            case 6:
                add_ambulance(system);
                break;
            case 7:
                {
                    float loc_x, loc_y;
                    printf("Enter your location coordinates:\n");
                    printf("X coordinate: ");
                    scanf("%f", &loc_x);
                    printf("Y coordinate: ");
                    scanf("%f", &loc_y);
                    clear_input_buffer();
                    find_nearest_ambulance(system, loc_x, loc_y);
                }
                break;
            case 8:
                generate_report(system);
                break;
            case 9:
                save_data(system);
                break;
            case 10:
                load_data(system);
                break;
            case 11:
                printf("\nThank you for using BAPESSS Ambulance Service!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
        
        printf("\nPress Enter to continue...");
        clear_input_buffer();
        getchar();
        
    } while (choice != 11);
    
    // Clean up memory
    free_system(system);
    
    return 0;
}

// =============================================
// SYSTEM INITIALIZATION FUNCTIONS
// =============================================

/**
 * Creates and initializes the BAPESSS system
 * Returns: Pointer to the initialized system
 */
BAPESSS_System* create_system() {
    // Allocate memory for the system structure
    BAPESSS_System* system = (BAPESSS_System*)malloc(sizeof(BAPESSS_System));
    
    if (system == NULL) {
        printf("Memory allocation failed for system!\n");
        return NULL;
    }
    
    // Initial capacities
    system->ambulance_capacity = 10;
    system->booking_capacity = 50;
    
    // Initialize counts
    system->ambulance_count = 0;
    system->booking_count = 0;
    
    // Allocate memory for arrays
    system->ambulances = (Ambulance*)malloc(system->ambulance_capacity * sizeof(Ambulance));
    system->bookings = (Booking*)malloc(system->booking_capacity * sizeof(Booking));
    
    if (system->ambulances == NULL || system->bookings == NULL) {
        printf("Memory allocation failed for arrays!\n");
        free(system);
        return NULL;
    }
    
    printf("System initialized successfully!\n");
    return system;
}

/**
 * Frees all allocated memory for the system
 */
void free_system(BAPESSS_System* system) {
    if (system != NULL) {
        if (system->ambulances != NULL) {
            free(system->ambulances);
        }
        if (system->bookings != NULL) {
            free(system->bookings);
        }
        free(system);
    }
    printf("System memory freed.\n");
}

// =============================================
// MENU AND DISPLAY FUNCTIONS
// =============================================

/**
 * Displays the main menu options
 */
void display_menu() {
    system("clear || cls"); // Clear screen (works for both Linux and Windows)
    
    printf("\n=== BAPESSS AMBULANCE BOOKING SYSTEM ===\n");
    printf("1.  Book an Ambulance\n");
    printf("2.  View All Bookings\n");
    printf("3.  View Available Ambulances\n");
    printf("4.  Update Booking Status\n");
    printf("5.  Cancel Booking\n");
    printf("6.  Add New Ambulance\n");
    printf("7.  Find Nearest Ambulance\n");
    printf("8.  Generate Report\n");
    printf("9.  Save Data to File\n");
    printf("10. Load Data from File\n");
    printf("11. Exit\n");
    printf("=======================================\n");
    printf("Enter your choice (1-11): ");
}

/**
 * Gets user choice with validation
 * Returns: Integer choice
 */
int get_choice() {
    int choice;
    char input[100];
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%d", &choice) == 1) {
            return choice;
        }
    }
    return -1; // Invalid input
}

/**
 * Clears the input buffer to prevent issues with scanf
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// =============================================
// SAMPLE DATA FOR DEMONSTRATION
// =============================================

/**
 * Adds sample ambulances and bookings for demonstration
 */
void add_sample_data(BAPESSS_System* system) {
    // Add sample ambulances
    Ambulance ambulance1 = {1, "MH01AB1234", "Rajesh Kumar", "9876543210", 2, 0, 12.5, 15.3};
    Ambulance ambulance2 = {2, "MH01CD5678", "Suresh Patel", "9876543211", 1, 0, 15.2, 18.7};
    Ambulance ambulance3 = {3, "MH01EF9012", "Amit Sharma", "9876543212", 3, 0, 10.1, 12.5};
    
    system->ambulances[0] = ambulance1;
    system->ambulances[1] = ambulance2;
    system->ambulances[2] = ambulance3;
    system->ambulance_count = 3;
    
    // Add sample bookings
    char time_buffer[50];
    get_current_time(time_buffer, sizeof(time_buffer));

    Booking booking1;
    booking1.booking_id = 1001;
    strcpy(booking1.patient_name, "John Doe");
    strcpy(booking1.patient_contact, "9123456789");
    strcpy(booking1.pickup_location, "123 Main St, Mumbai");
    strcpy(booking1.hospital, "Apollo Hospital");
    booking1.ambulance_id = 1;
    strcpy(booking1.booking_time, time_buffer);
    strcpy(booking1.pickup_time, "Not picked up yet");
    booking1.emergency_level = 2;
    booking1.status = 1;

    Booking booking2;
    booking2.booking_id = 1002;
    strcpy(booking2.patient_name, "Jane Smith");
    strcpy(booking2.patient_contact, "9123456790");
    strcpy(booking2.pickup_location, "456 Park Ave, Delhi");
    strcpy(booking2.hospital, "Fortis Hospital");
    booking2.ambulance_id = 2;
    strcpy(booking2.booking_time, time_buffer);
    strcpy(booking2.pickup_time, "Not picked up yet");
    booking2.emergency_level = 3;
    booking2.status = 2;
    
    system->bookings[0] = booking1;
    system->bookings[1] = booking2;
    system->booking_count = 2;
    
    printf("Sample data loaded successfully!\n");
}

// =============================================
// CORE BOOKING FUNCTION
// =============================================

/**
 * Books an ambulance based on user input
 */
void book_ambulance(BAPESSS_System* system) {
    printf("\n=== BOOK AMBULANCE ===\n");
    
    // Check if we have capacity for new booking
    if (system->booking_count >= system->booking_capacity) {
        printf("Error: Booking system is at full capacity!\n");
        return;
    }
    
    // Get patient details
    Booking new_booking;
    
    // Generate booking ID
    new_booking.booking_id = 1000 + system->booking_count + 1;
    
    printf("Enter patient name: ");
    clear_input_buffer();
    fgets(new_booking.patient_name, sizeof(new_booking.patient_name), stdin);
    new_booking.patient_name[strcspn(new_booking.patient_name, "\n")] = 0;
    
    printf("Enter contact number: ");
    fgets(new_booking.patient_contact, sizeof(new_booking.patient_contact), stdin);
    new_booking.patient_contact[strcspn(new_booking.patient_contact, "\n")] = 0;
    
    printf("Enter pickup location: ");
    fgets(new_booking.pickup_location, sizeof(new_booking.pickup_location), stdin);
    new_booking.pickup_location[strcspn(new_booking.pickup_location, "\n")] = 0;
    
    printf("Enter hospital name: ");
    fgets(new_booking.hospital, sizeof(new_booking.hospital), stdin);
    new_booking.hospital[strcspn(new_booking.hospital, "\n")] = 0;
    
    // Get emergency level
    printf("\nEmergency Level:\n");
    printf("1. Normal (Non-critical)\n");
    printf("2. Urgent (Serious but stable)\n");
    printf("3. Critical (Life-threatening)\n");
    printf("Select emergency level (1-3): ");
    scanf("%d", &new_booking.emergency_level);
    
    if (new_booking.emergency_level < 1 || new_booking.emergency_level > 3) {
        printf("Invalid emergency level! Setting to Normal.\n");
        new_booking.emergency_level = 1;
    }
    
    // Find available ambulance
    new_booking.ambulance_id = find_available_ambulance(system, new_booking.emergency_level);
    
    if (new_booking.ambulance_id == -1) {
        printf("\nSorry! No ambulances available at the moment.\n");
        printf("Your request has been queued. We'll notify you when available.\n");
        new_booking.status = 0; // Pending
        new_booking.ambulance_id = 0;
    } else {
        // Update ambulance status
        for (int i = 0; i < system->ambulance_count; i++) {
            if (system->ambulances[i].ambulance_id == new_booking.ambulance_id) {
                system->ambulances[i].status = 1; // Booked
                break;
            }
        }
        new_booking.status = 1; // Confirmed
        printf("\nAmbulance ID %d has been assigned!\n", new_booking.ambulance_id);
    }
    
    // Set timestamps
    char time_buffer[50];
    get_current_time(time_buffer, sizeof(time_buffer));
    strcpy(new_booking.booking_time, time_buffer);
    strcpy(new_booking.pickup_time, "Not picked up yet");
    
    // Add booking to system
    system->bookings[system->booking_count] = new_booking;
    system->booking_count++;
    
    printf("\n=== BOOKING CONFIRMED ===\n");
    printf("Booking ID: %d\n", new_booking.booking_id);
    printf("Patient: %s\n", new_booking.patient_name);
    printf("Status: %s\n", new_booking.status == 1 ? "Confirmed" : "Pending");
    if (new_booking.ambulance_id > 0) {
        printf("Assigned Ambulance: %d\n", new_booking.ambulance_id);
    }
}

// =============================================
// VIEW FUNCTIONS
// =============================================

/**
 * Displays all bookings
 */
void view_bookings(BAPESSS_System* system) {
    printf("\n=== ALL BOOKINGS ===\n");
    printf("Total Bookings: %d\n\n", system->booking_count);
    
    if (system->booking_count == 0) {
        printf("No bookings found.\n");
        return;
    }
    
    printf("ID    Patient Name         Contact       Status      Ambulance\n");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < system->booking_count; i++) {
        char* status_str;
        switch(system->bookings[i].status) {
            case 0: status_str = "Pending"; break;
            case 1: status_str = "Confirmed"; break;
            case 2: status_str = "Dispatched"; break;
            case 3: status_str = "Completed"; break;
            case 4: status_str = "Cancelled"; break;
            default: status_str = "Unknown";
        }
        
        printf("%-6d%-21s%-14s%-12s%d\n",
               system->bookings[i].booking_id,
               system->bookings[i].patient_name,
               system->bookings[i].patient_contact,
               status_str,
               system->bookings[i].ambulance_id);
    }
    
    // Option to view detailed booking
    printf("\nEnter Booking ID to view details (0 to skip): ");
    int view_id;
    scanf("%d", &view_id);
    
    if (view_id > 0) {
        for (int i = 0; i < system->booking_count; i++) {
            if (system->bookings[i].booking_id == view_id) {
                printf("\n=== BOOKING DETAILS ===\n");
                printf("Booking ID: %d\n", system->bookings[i].booking_id);
                printf("Patient: %s\n", system->bookings[i].patient_name);
                printf("Contact: %s\n", system->bookings[i].patient_contact);
                printf("Pickup: %s\n", system->bookings[i].pickup_location);
                printf("Hospital: %s\n", system->bookings[i].hospital);
                printf("Booking Time: %s\n", system->bookings[i].booking_time);
                printf("Pickup Time: %s\n", system->bookings[i].pickup_time);
                
                char* emergency_str;
                switch(system->bookings[i].emergency_level) {
                    case 1: emergency_str = "Normal"; break;
                    case 2: emergency_str = "Urgent"; break;
                    case 3: emergency_str = "Critical"; break;
                    default: emergency_str = "Unknown";
                }
                printf("Emergency Level: %s\n", emergency_str);
                break;
            }
        }
    }
}

/**
 * Displays all ambulances and their status
 */
void view_ambulances(BAPESSS_System* system) {
    printf("\n=== AMBULANCE FLEET ===\n");
    printf("Total Ambulances: %d\n\n", system->ambulance_count);
    
    if (system->ambulance_count == 0) {
        printf("No ambulances registered.\n");
        return;
    }
    
    printf("ID  Vehicle No.   Driver         Contact      Type        Status      Location\n");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < system->ambulance_count; i++) {
        char* type_str;
        switch(system->ambulances[i].type) {
            case 1: type_str = "Basic"; break;
            case 2: type_str = "Advanced"; break;
            case 3: type_str = "Mobile ICU"; break;
            default: type_str = "Unknown";
        }
        
        char* status_str;
        switch(system->ambulances[i].status) {
            case 0: status_str = "Available"; break;
            case 1: status_str = "Booked"; break;
            case 2: status_str = "On Trip"; break;
            case 3: status_str = "Maintenance"; break;
            default: status_str = "Unknown";
        }
        
        printf("%-3d%-14s%-15s%-13s%-12s%-12s(%.1f, %.1f)\n",
               system->ambulances[i].ambulance_id,
               system->ambulances[i].vehicle_number,
               system->ambulances[i].driver_name,
               system->ambulances[i].driver_contact,
               type_str,
               status_str,
               system->ambulances[i].location_x,
               system->ambulances[i].location_y);
    }
}

// =============================================
// UTILITY FUNCTIONS
// =============================================

/**
 * Gets current time as string
 */
void get_current_time(char* buffer, int size) {
    time_t rawtime;
    struct tm* timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

/**
 * Finds an available ambulance based on emergency level
 * Returns: Ambulance ID or -1 if none available
 */
int find_available_ambulance(BAPESSS_System* system, int emergency_level) {
    // First, try to find ambulance matching emergency level requirements
    for (int i = 0; i < system->ambulance_count; i++) {
        if (system->ambulances[i].status == 0 &&  // Available
            system->ambulances[i].type >= emergency_level) { // Type matches or exceeds requirement
            return system->ambulances[i].ambulance_id;
        }
    }
    
    // If no exact match, find any available ambulance
    for (int i = 0; i < system->ambulance_count; i++) {
        if (system->ambulances[i].status == 0) {
            return system->ambulances[i].ambulance_id;
        }
    }
    
    return -1; // No ambulance available
}

// =============================================
// MANAGEMENT FUNCTIONS
// =============================================

/**
 * Updates the status of a booking
 */
void update_booking_status(BAPESSS_System* system) {
    printf("\n=== UPDATE BOOKING STATUS ===\n");
    
    if (system->booking_count == 0) {
        printf("No bookings to update.\n");
        return;
    }
    
    int booking_id;
    printf("Enter Booking ID to update: ");
    scanf("%d", &booking_id);
    
    // Find the booking
    int found = -1;
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].booking_id == booking_id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("Booking ID %d not found!\n", booking_id);
        return;
    }
    
    printf("\nCurrent Status: ");
    switch(system->bookings[found].status) {
        case 0: printf("Pending\n"); break;
        case 1: printf("Confirmed\n"); break;
        case 2: printf("Dispatched\n"); break;
        case 3: printf("Completed\n"); break;
        case 4: printf("Cancelled\n"); break;
    }
    
    printf("\nSelect new status:\n");
    printf("1. Confirmed\n");
    printf("2. Dispatched\n");
    printf("3. Completed\n");
    printf("4. Cancelled\n");
    printf("Enter choice (1-4): ");
    
    int new_status;
    char time_buffer[50];
    scanf("%d", &new_status);
    
    if (new_status < 1 || new_status > 4) {
        printf("Invalid status!\n");
        return;
    }
    
    // Update status
    system->bookings[found].status = new_status;
    
    // If completed or cancelled, free up the ambulance
    if (new_status == 3 || new_status == 4) {
        for (int i = 0; i < system->ambulance_count; i++) {
            if (system->ambulances[i].ambulance_id == system->bookings[found].ambulance_id) {
                system->ambulances[i].status = 0; // Available
                break;
            }
        }
    }
    
    // Update pickup time if dispatched
    if (new_status == 2) {
        get_current_time(time_buffer, sizeof(time_buffer));
        strcpy(system->bookings[found].pickup_time, time_buffer);
    }
    
    printf("Booking status updated successfully!\n");
}

/**
 * Cancels a booking
 */
void cancel_booking(BAPESSS_System* system) {
    printf("\n=== CANCEL BOOKING ===\n");
    
    if (system->booking_count == 0) {
        printf("No bookings to cancel.\n");
        return;
    }
    
    int booking_id;
    printf("Enter Booking ID to cancel: ");
    scanf("%d", &booking_id);
    
    // Find the booking
    int found = -1;
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].booking_id == booking_id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("Booking ID %d not found!\n", booking_id);
        return;
    }
    
    printf("\nBooking Details:\n");
    printf("Patient: %s\n", system->bookings[found].patient_name);
    printf("Contact: %s\n", system->bookings[found].patient_contact);
    printf("Pickup: %s\n", system->bookings[found].pickup_location);
    
    char confirm;
    printf("\nAre you sure you want to cancel this booking? (y/n): ");
    clear_input_buffer();
    scanf("%c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        system->bookings[found].status = 4; // Cancelled
        
        // Free up the ambulance
        for (int i = 0; i < system->ambulance_count; i++) {
            if (system->ambulances[i].ambulance_id == system->bookings[found].ambulance_id) {
                system->ambulances[i].status = 0; // Available
                break;
            }
        }
        
        printf("Booking cancelled successfully!\n");
    } else {
        printf("Cancellation aborted.\n");
    }
}

/**
 * Adds a new ambulance to the fleet
 */
void add_ambulance(BAPESSS_System* system) {
    printf("\n=== ADD NEW AMBULANCE ===\n");
    
    // Check capacity
    if (system->ambulance_count >= system->ambulance_capacity) {
        // Resize array
        system->ambulance_capacity *= 2;
        system->ambulances = (Ambulance*)realloc(system->ambulances, 
                                                system->ambulance_capacity * sizeof(Ambulance));
        if (!system->ambulances) {
            printf("Error: Memory allocation failed!\n");
            return;
        }
    }
    
    Ambulance new_ambulance;
    
    // Generate ID
    new_ambulance.ambulance_id = system->ambulance_count + 1;
    
    printf("Enter vehicle number: ");
    clear_input_buffer();
    fgets(new_ambulance.vehicle_number, sizeof(new_ambulance.vehicle_number), stdin);
    new_ambulance.vehicle_number[strcspn(new_ambulance.vehicle_number, "\n")] = 0;
    
    printf("Enter driver name: ");
    fgets(new_ambulance.driver_name, sizeof(new_ambulance.driver_name), stdin);
    new_ambulance.driver_name[strcspn(new_ambulance.driver_name, "\n")] = 0;
    
    printf("Enter driver contact: ");
    fgets(new_ambulance.driver_contact, sizeof(new_ambulance.driver_contact), stdin);
    new_ambulance.driver_contact[strcspn(new_ambulance.driver_contact, "\n")] = 0;
    
    printf("\nSelect ambulance type:\n");
    printf("1. Basic Life Support\n");
    printf("2. Advanced Life Support\n");
    printf("3. Mobile ICU\n");
    printf("Enter type (1-3): ");
    scanf("%d", &new_ambulance.type);
    
    if (new_ambulance.type < 1 || new_ambulance.type > 3) {
        printf("Invalid type! Setting to Basic.\n");
        new_ambulance.type = 1;
    }
    
    printf("Enter current location (X Y coordinates): ");
    scanf("%f %f", &new_ambulance.location_x, &new_ambulance.location_y);
    
    new_ambulance.status = 0; // Available
    
    // Add to system
    system->ambulances[system->ambulance_count] = new_ambulance;
    system->ambulance_count++;
    
    printf("\nAmbulance added successfully!\n");
    printf("Ambulance ID: %d\n", new_ambulance.ambulance_id);
}

/**
 * Finds the nearest available ambulance
 */
void find_nearest_ambulance(BAPESSS_System* system, float loc_x, float loc_y) {
    printf("\n=== FINDING NEAREST AMBULANCE ===\n");
    printf("Your location: (%.2f, %.2f)\n", loc_x, loc_y);
    
    int nearest_id = -1;
    float min_distance = 1000000; // Large initial value
    
    for (int i = 0; i < system->ambulance_count; i++) {
        if (system->ambulances[i].status == 0) { // Available
            // Calculate distance (simplified Euclidean distance)
            float distance = (loc_x - system->ambulances[i].location_x) * 
                           (loc_x - system->ambulances[i].location_x) +
                           (loc_y - system->ambulances[i].location_y) * 
                           (loc_y - system->ambulances[i].location_y);
            
            if (distance < min_distance) {
                min_distance = distance;
                nearest_id = i;
            }
        }
    }
    
    if (nearest_id != -1) {
        printf("\nNearest available ambulance found:\n");
        printf("Ambulance ID: %d\n", system->ambulances[nearest_id].ambulance_id);
        printf("Vehicle: %s\n", system->ambulances[nearest_id].vehicle_number);
        printf("Driver: %s\n", system->ambulances[nearest_id].driver_name);
        printf("Contact: %s\n", system->ambulances[nearest_id].driver_contact);
        printf("Distance: %.2f units\n", sqrt(min_distance));
        printf("Location: (%.2f, %.2f)\n", 
               system->ambulances[nearest_id].location_x,
               system->ambulances[nearest_id].location_y);
    } else {
        printf("\nNo available ambulances found near your location.\n");
    }
}

// =============================================
// REPORT AND DATA PERSISTENCE
// =============================================

/**
 * Generates a system report
 */
void generate_report(BAPESSS_System* system) {
    printf("\n=== SYSTEM REPORT ===\n");
    printf("Generated on: ");
    char time_buffer[50];
    get_current_time(time_buffer, sizeof(time_buffer));
    printf("%s\n\n", time_buffer);
    
    printf("Fleet Summary:\n");
    printf("Total Ambulances: %d\n", system->ambulance_count);
    
    int available = 0, booked = 0, on_trip = 0, maintenance = 0;
    int basic = 0, advanced = 0, icu = 0;
    
    for (int i = 0; i < system->ambulance_count; i++) {
        switch(system->ambulances[i].status) {
            case 0: available++; break;
            case 1: booked++; break;
            case 2: on_trip++; break;
            case 3: maintenance++; break;
        }
        
        switch(system->ambulances[i].type) {
            case 1: basic++; break;
            case 2: advanced++; break;
            case 3: icu++; break;
        }
    }
    
    printf("  Available: %d\n", available);
    printf("  Booked: %d\n", booked);
    printf("  On Trip: %d\n", on_trip);
    printf("  Maintenance: %d\n", maintenance);
    
    printf("\nAmbulance Types:\n");
    printf("  Basic: %d\n", basic);
    printf("  Advanced: %d\n", advanced);
    printf("  Mobile ICU: %d\n", icu);
    
    printf("\nBooking Summary:\n");
    printf("Total Bookings: %d\n", system->booking_count);
    
    int pending = 0, confirmed = 0, dispatched = 0, completed = 0, cancelled = 0;
    int normal = 0, urgent = 0, critical = 0;
    
    for (int i = 0; i < system->booking_count; i++) {
        switch(system->bookings[i].status) {
            case 0: pending++; break;
            case 1: confirmed++; break;
            case 2: dispatched++; break;
            case 3: completed++; break;
            case 4: cancelled++; break;
        }
        
        switch(system->bookings[i].emergency_level) {
            case 1: normal++; break;
            case 2: urgent++; break;
            case 3: critical++; break;
        }
    }
    
    printf("  Pending: %d\n", pending);
    printf("  Confirmed: %d\n", confirmed);
    printf("  Dispatched: %d\n", dispatched);
    printf("  Completed: %d\n", completed);
    printf("  Cancelled: %d\n", cancelled);
    
    printf("\nEmergency Levels:\n");
    printf("  Normal: %d\n", normal);
    printf("  Urgent: %d\n", urgent);
    printf("  Critical: %d\n", critical);
    
    printf("\nUtilization Rate: %.1f%%\n", 
           system->ambulance_count > 0 ? 
           (float)(booked + on_trip) / system->ambulance_count * 100 : 0);
}

/**
 * Saves system data to files
 */
void save_data(BAPESSS_System* system) {
    FILE *amb_file = fopen("ambulances.dat", "wb");
    FILE *book_file = fopen("bookings.dat", "wb");
    
    if (!amb_file || !book_file) {
        printf("Error opening files for saving!\n");
        return;
    }
    
    // Save ambulances
    fwrite(&system->ambulance_count, sizeof(int), 1, amb_file);
    fwrite(system->ambulances, sizeof(Ambulance), system->ambulance_count, amb_file);
    
    // Save bookings
    fwrite(&system->booking_count, sizeof(int), 1, book_file);
    fwrite(system->bookings, sizeof(Booking), system->booking_count, book_file);
    
    fclose(amb_file);
    fclose(book_file);
    
    printf("Data saved successfully!\n");
}

/**
 * Loads system data from files
 */
void load_data(BAPESSS_System* system) {
    FILE *amb_file = fopen("ambulances.dat", "rb");
    FILE *book_file = fopen("bookings.dat", "rb");
    
    if (!amb_file || !book_file) {
        printf("No saved data found or error opening files!\n");
        return;
    }
    
    // Free existing data
    free(system->ambulances);
    free(system->bookings);
    
    // Load ambulances
    fread(&system->ambulance_count, sizeof(int), 1, amb_file);
    system->ambulance_capacity = system->ambulance_count + 10;
    system->ambulances = (Ambulance*)malloc(system->ambulance_capacity * sizeof(Ambulance));
    fread(system->ambulances, sizeof(Ambulance), system->ambulance_count, amb_file);
    
    // Load bookings
    fread(&system->booking_count, sizeof(int), 1, book_file);
    system->booking_capacity = system->booking_count + 50;
    system->bookings = (Booking*)malloc(system->booking_capacity * sizeof(Booking));
    fread(system->bookings, sizeof(Booking), system->booking_count, book_file);
    
    fclose(amb_file);
    fclose(book_file);
    
    printf("Data loaded successfully!\n");
    printf("Ambulances: %d, Bookings: %d\n", system->ambulance_count, system->booking_count);
}