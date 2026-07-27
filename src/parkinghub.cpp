#include <iostream>
#include <string>
#include <cctype>
using namespace std;

void mainMenu();
void addVehicle();
void removeVehicle();
void searchVehicle();
void availableSlot();
void adminPanel();

int main() {
    mainMenu();
    cout << "\nProgram Stopped.\n";
    return 0;
}

// Main menu loop
void mainMenu() {
    int choice;
    do {
        cout << "\n PARKING HUB \n" << endl;
        cout << "1. Add Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. Search Vehicle\n";
        cout << "4. Available Slot\n";
        cout << "5. Admin Panel\n";
        cout << "6. Exit\n";
        cout << "==============================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // avoid crash on letter input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: addVehicle(); break;
            case 2: removeVehicle(); break;
            case 3: searchVehicle(); break;
            case 4: availableSlot(); break;
            case 5: adminPanel(); break;
            case 6: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 6);
}

// Add vehicle
void addVehicle() {
    string ownerName, phoneNumber, numberPlate;

    cout << "\n----- ADD VEHICLE -----\n";
    cin.ignore();
    cout << "Enter Vehicle Owner Name: ";
    getline(cin, ownerName);

    // phone must be 10 digits
    bool validPhone;
    do {
        cout << "Enter Owner Phone Number (10 digits): ";
        getline(cin, phoneNumber);

        validPhone = phoneNumber.length() == 10;
        for (int i = 0; i < phoneNumber.length(); i++)
            if (!isdigit(phoneNumber[i])) { validPhone = false; break; }

        if (!validPhone) cout << "Invalid phone number! Must be exactly 10 digits.\n";
    } while (!validPhone);

    // plate must be alphanumeric
    bool validPlate;
    do {
        cout << "Enter Vehicle Number Plate (alphanumeric): ";
        getline(cin, numberPlate);

        validPlate = numberPlate.length() > 0;
        for (int i = 0; i < numberPlate.length(); i++)
            if (!isalnum(numberPlate[i])) { validPlate = false; break; }

        if (!validPlate) cout << "Invalid number plate! Use letters and digits only.\n";
    } while (!validPlate);

    cout << "Vehicle added successfully!\n";
}

// Remove vehicle
void removeVehicle() {
    string numberPlate, phoneNumber;

    cout << "\n----- REMOVE VEHICLE -----\n";
    cin.ignore();

    // plate must be alphanumeric
    bool validPlate;
    do {
        cout << "Enter Vehicle Number Plate (alphanumeric): ";
        getline(cin, numberPlate);

        validPlate = numberPlate.length() > 0;
        for (int i = 0; i < numberPlate.length(); i++)
            if (!isalnum(numberPlate[i])) { validPlate = false; break; }

        if (!validPlate) cout << "Invalid number plate! Use letters and digits only.\n";
    } while (!validPlate);

    // phone must be 10 digits
    bool validPhone;
    do {
        cout << "Enter Phone Number (10 digits): ";
        getline(cin, phoneNumber);

        validPhone = phoneNumber.length() == 10;
        for (int i = 0; i < phoneNumber.length(); i++)
            if (!isdigit(phoneNumber[i])) { validPhone = false; break; }

        if (!validPhone) cout << "Invalid phone number! Must be exactly 10 digits.\n";
    } while (!validPhone);

    cout << "Vehicle removed successfully!\n";
}

// Search vehicle
void searchVehicle() {
    string numberPlate;

    cout << "\n----- SEARCH VEHICLE -----\n";
    cin.ignore();
    cout << "Enter Vehicle Number Plate: ";
    getline(cin, numberPlate);

    cout << "Searching for vehicle: " << numberPlate << "\n";
}

// Show available slots
void availableSlot() {
    cout << "\n----- AVAILABLE SLOTS -----\n";
    cout << "Displaying available parking slots...\n";
}

// Admin panel loop
void adminPanel() {
    int choice;
    do {
        cout << "\n===== ADMIN PANEL =====\n";
        cout << "1. Revenue Statistics\n";
        cout << "2. Reset Daily Record\n";
        cout << "3. Vehicle History\n";
        cout << "4. Exit\n";
        cout << "========================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: cout << "Displaying Revenue Statistics...\n"; break;
            case 2: cout << "Daily Record Reset Successfully!\n"; break;
            case 3: cout << "Displaying Vehicle History...\n"; break;
            case 4: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);
}
