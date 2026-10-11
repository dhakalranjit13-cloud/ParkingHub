#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <sstream>


using namespace std;

// total slots, max vehicles and max users
const int TOTAL_SLOTS = 20;
const int FOUR_WHEELER_SLOTS = 10;  // slots 1 to 10
const int TWO_WHEELER_SLOTS = 10;   // slots 11 to 20
const int MAX_VEHICLES = 20;
const int MAX_USERS = 100;

// vehicle class
class Vehicle
{
private:
    string number;
    string phone;
    string type;
    int slot;
    int entryHour;
    int entryMinute;
    string owner;

public:
    // constructor
    Vehicle();

    // getters
    string getNumber();
    string getPhone();
    string getType();
    int getSlot();
    int getEntryHour();
    int getEntryMinute();
    string getOwner();

    // setters
    void setNumber(string n);
    void setPhone(string p);
    void setType(string t);
    void setSlot(int s);
    void setEntryHour(int h);
    void setEntryMinute(int m);
    void setOwner(string o);
};

// parking hub class
class ParkingHub
{
private:
    // all data of parking hub
    Vehicle vehicles[MAX_VEHICLES];
    int vehicleCount;

    bool slotOccupied[TOTAL_SLOTS + 1];
    string userIDs[MAX_USERS];
    string userPass[MAX_USERS];
    int userCount;

    string adminIDs[MAX_USERS];
    string adminPass[MAX_USERS];
    int adminCount;

    double totalRevenue;

    // file names
    const string USERS_FILE = "users.txt";
    const string ADMINS_FILE = "admins.txt";
    const string REVENUE_FILE = "revenue.txt";
    const string HISTORY_FILE = "history.txt";
    const string PARKED_FILE = "parked.txt";

public:
    // constructor
    ParkingHub();

    // getters and setters
    int getVehicleCount();
    void setVehicleCount(int count);

    int getUserCount();
    void setUserCount(int count);

    int getAdminCount();
    void setAdminCount(int count);

    double getTotalRevenue();
    void setTotalRevenue(double revenue);

    bool getSlotOccupied(int slot);
    void setSlotOccupied(int slot, bool status);

    string getUserID(int index);
    void setUserID(int index, string id);
    string getUserPass(int index);
    void setUserPass(int index, string pass);

    string getAdminID(int index);
    void setAdminID(int index, string id);
    string getAdminPass(int index);
    void setAdminPass(int index, string pass);

    // time functions
    int getCurrentHour();
    int getCurrentMinute();
    string formatTime(int hour, int minute);

    // input functions
    int readInt(string prompt);
    int readIntInRange(string prompt, int minimum, int maximum);
    string readLineNonEmpty(string prompt);
    string readPhone(string prompt);

    // file functions
    void loadUsers();
    void loadAdmins();
    void loadRevenue();
    void loadParkedVehicles();
    void saveRevenue();
    void saveUser(string id, string password);
    void saveAdmin(string id, string password);
    void saveParkedVehicles();
    void saveHistory(Vehicle vehicle, int exitHour, int exitMinute, int duration, double bill);

    // default admin and user
    void ensureDefaultAccounts();

    // search functions
    int findVehicle(string vehicleNumber);
    int findUser(string id);
    int findAdmin(string id);
    int findFreeSlot(string type);

    // user functions
    void registerUser();
    bool loginUser(string &currentUser);
    void guestUser();

    // admin functions
    void registerAdmin();
    bool loginAdmin(string &currentAdmin);

    // vehicle functions
    void addVehicle(string owner);
    void removeVehicle(string currentUser, bool isAdmin);
    void searchVehicle();
    void viewAvailableSlots();
    void viewRevenue();
    void resetDailyRecord();
    void showHistory();
    void viewParkedVehicles(string currentUser, bool isAdmin);

    // menus
    void userPanel();
    void userMenu(string currentUser);
    void adminPanel();
    void start();
};


// clears the screen
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// waits for Enter
void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.get();
}

// returns true if text has no space
bool hasNoSpace(string text)
{
    for (size_t i = 0; i < text.length(); i++)
    {
        if (text[i] == ' ')
        {
            return false;
        }
    }
    return true;
}

// vehicle functions
Vehicle::Vehicle()
{
    number = "";
    phone = "";
    type = "";
    slot = 0;
    entryHour = 0;
    entryMinute = 0;
    owner = "";
}

// getters
string Vehicle::getNumber() { return number; }
string Vehicle::getType() { return type; }
int Vehicle::getSlot() { return slot; }
int Vehicle::getEntryHour() { return entryHour; }
int Vehicle::getEntryMinute() { return entryMinute; }
string Vehicle::getOwner() { return owner; }
string Vehicle::getPhone() { return phone; }

// setters
void Vehicle::setNumber(string n) { number = n; }
void Vehicle::setType(string t) { type = t; }
void Vehicle::setSlot(int s) { slot = s; }
void Vehicle::setEntryHour(int h) { entryHour = h; }
void Vehicle::setEntryMinute(int m) { entryMinute = m; }
void Vehicle::setOwner(string o) { owner = o; }
void Vehicle::setPhone(string p) { phone = p; }

// parking hub functions
// constructor sets starting values and loads the files
ParkingHub::ParkingHub()
{
    setVehicleCount(0);
    setUserCount(0);
    setAdminCount(0);
    setTotalRevenue(0.0);
    for (int i = 0; i <= TOTAL_SLOTS; i++)
    {
        setSlotOccupied(i, false);
    }
    loadUsers();
    loadAdmins();
    loadRevenue();
    loadParkedVehicles();
    ensureDefaultAccounts();
}

// getters and setters
int ParkingHub::getVehicleCount()
{
    return vehicleCount;
}
void ParkingHub::setVehicleCount(int count)
{
    vehicleCount = count;
}

int ParkingHub::getUserCount()
{
    return userCount;
}
void ParkingHub::setUserCount(int count)
{
    userCount = count;
}

int ParkingHub::getAdminCount()
{
    return adminCount;
}
void ParkingHub::setAdminCount(int count)
{
    adminCount = count;
}

double ParkingHub::getTotalRevenue()
{
    return totalRevenue;
}
void ParkingHub::setTotalRevenue(double revenue)
{
    totalRevenue = revenue;
}

// check slot number
bool ParkingHub::getSlotOccupied(int slot)
{
    if (slot < 0 || slot > TOTAL_SLOTS)
    {
        return false;
    }
    return slotOccupied[slot];
}
void ParkingHub::setSlotOccupied(int slot, bool status)
{
    if (slot >= 0 && slot <= TOTAL_SLOTS)
    {
        slotOccupied[slot] = status;
    }
}

// check index number
string ParkingHub::getUserID(int index)
{
    if (index < 0 || index >= MAX_USERS)
    {
        return "";
    }
    return userIDs[index];
}
void ParkingHub::setUserID(int index, string id)
{
    if (index >= 0 && index < MAX_USERS)
    {
        userIDs[index] = id;
    }
}
string ParkingHub::getUserPass(int index)
{
    if (index < 0 || index >= MAX_USERS)
    {
        return "";
    }
    return userPass[index];
}
void ParkingHub::setUserPass(int index, string pass)
{
    if (index >= 0 && index < MAX_USERS)
    {
        userPass[index] = pass;
    }
}

string ParkingHub::getAdminID(int index)
{
    if (index < 0 || index >= MAX_USERS)
    {
        return "";
    }
    return adminIDs[index];
}
void ParkingHub::setAdminID(int index, string id)
{
    if (index >= 0 && index < MAX_USERS)
    {
        adminIDs[index] = id;
    }
}
string ParkingHub::getAdminPass(int index)
{
    if (index < 0 || index >= MAX_USERS)
    {
        return "";
    }
    return adminPass[index];
}
void ParkingHub::setAdminPass(int index, string pass)
{
    if (index >= 0 && index < MAX_USERS)
    {
        adminPass[index] = pass;
    }
}

// gives current hour from device
int ParkingHub::getCurrentHour()
{
    time_t now = time(0);
    // change it into hour and minute
    tm *localTime = localtime(&now);
    return localTime->tm_hour;
}
// gives current minute from device
int ParkingHub::getCurrentMinute()
{
    time_t now = time(0);
    tm *localTime = localtime(&now);
    return localTime->tm_min;
}

// adds 0 in front so 9:5 becomes 09:05
string ParkingHub::formatTime(int hour, int minute)
{
    string h = to_string(hour);
    string m = to_string(minute);
    if (hour < 10)
    {
        h = "0" + h;
    }
    if (minute < 10)
    {
        m = "0" + m;
    }
    return h + ":" + m;
}

// keeps asking until user enters a whole number
int ParkingHub::readInt(string prompt)
{
    string line;
    while (true)
    {
        cout << prompt;
        getline(cin, line);

        try
        {
            size_t used = 0;
            int number = stoi(line, &used);

            // skip trailing spaces
            while (used < line.length() && line[used] == ' ')
            {
                used++;
            }
            if (used == line.length())
            {
                return number;
            }
        }
        catch (...)
        {
        }
        cout << "Invalid input. Enter valid number.\n";
    }
}

// keeps asking until number is between min and max
int ParkingHub::readIntInRange(string prompt, int minimum, int maximum)
{
    while (true)
    {
        int number = readInt(prompt);
        if (number >= minimum && number <= maximum)
        {
            return number;
        }
        cout << "Enter a number between " << minimum << " and " << maximum << ".\n";
    }
}

// keeps asking until user types something without spaces
string ParkingHub::readLineNonEmpty(string prompt)
{
    string text;
    while (true)
    {
        cout << prompt;
        getline(cin, text);
        if (text.empty())
        {
            cout << "Input cannot be empty.\n";
        }
        else if (!hasNoSpace(text))
        {
            cout << "Spaces are not allowed. Use - or _ instead.\n";
        }
        else
        {
            return text;
        }
    }
}

// keeps asking until user enters exactly 10 digits
string ParkingHub::readPhone(string prompt)
{
    while (true)
    {
        string phone = readLineNonEmpty(prompt);

        bool valid = (phone.length() == 10);
        for (size_t i = 0; i < phone.length() && valid; i++)
        {
            if (phone[i] < '0' || phone[i] > '9')
            {
                valid = false;
            }
        }
        if (valid)
        {
            return phone;
        }
        cout << "Phone number must be exactly 10 digits.\n";
    }
}

// read all users from users.txt
void ParkingHub::loadUsers()
{
    setUserCount(0);
    ifstream file(USERS_FILE);
    if (!file)
    {
        return;
    }
    string id, pass;
    while (file >> id >> pass)
    {
        setUserID(getUserCount(), id);
        setUserPass(getUserCount(), pass);
        setUserCount(getUserCount() + 1);
        if (getUserCount() >= MAX_USERS)
        {
            break;
        }
    }
    file.close();
}

// read all admins from admins.txt
void ParkingHub::loadAdmins()
{
    setAdminCount(0);
    ifstream file(ADMINS_FILE);
    if (!file)
    {
        return;
    }
    string id, pass;
    while (file >> id >> pass)
    {
        setAdminID(getAdminCount(), id);
        setAdminPass(getAdminCount(), pass);
        setAdminCount(getAdminCount() + 1);
        if (getAdminCount() >= MAX_USERS)
        {
            break;
        }
    }
    file.close();
}

// read total revenue from revenue.txt
void ParkingHub::loadRevenue()
{
    ifstream file(REVENUE_FILE);
    if (!file)
    {
        setTotalRevenue(0.0);
        return;
    }
    double revenue = 0.0;
    file >> revenue;
    setTotalRevenue(revenue);
    file.close();
}

// read all parked vehicles from parked.txt and mark their slots as used
void ParkingHub::loadParkedVehicles()
{
    setVehicleCount(0);
    ifstream file(PARKED_FILE);
    if (!file)
    {
        return;
    }
    string number, type, owner, phone;
    int slot, entryHour, entryMinute;

    while (file >> number >> type >> slot >> entryHour >> entryMinute >> owner >> phone)
    {
        if (getVehicleCount() >= MAX_VEHICLES)
        {
            break;
        }
        int index = getVehicleCount();
        vehicles[index].setNumber(number);
        vehicles[index].setType(type);
        vehicles[index].setSlot(slot);
        vehicles[index].setEntryHour(entryHour);
        vehicles[index].setEntryMinute(entryMinute);
        vehicles[index].setOwner(owner);
        vehicles[index].setPhone(phone);
        setSlotOccupied(slot, true);
        setVehicleCount(index + 1);
    }
    file.close();
}

// write total revenue in revenue.txt
void ParkingHub::saveRevenue()
{
    ofstream file(REVENUE_FILE, ios::trunc);
    file << fixed << setprecision(2) << getTotalRevenue();
    file.close();
}

// add new user at the end of users.txt
void ParkingHub::saveUser(string id, string password)
{
    ofstream file(USERS_FILE, ios::app);
    file << id << " " << password << "\n";
    file.close();
}

// add new admin at the end of admins.txt
void ParkingHub::saveAdmin(string id, string password)
{
    ofstream file(ADMINS_FILE, ios::app);
    file << id << " " << password << "\n";
    file.close();
}

// write all currently parked vehicles in parked.txt
void ParkingHub::saveParkedVehicles()
{
    ofstream file(PARKED_FILE, ios::trunc);
    for (int i = 0; i < getVehicleCount(); i++)
    {
        file << vehicles[i].getNumber() << " "
             << vehicles[i].getType() << " "
             << vehicles[i].getSlot() << " "
             << vehicles[i].getEntryHour() << " "
             << vehicles[i].getEntryMinute() << " "
             << vehicles[i].getOwner() << " "
             << vehicles[i].getPhone() << "\n";
    }
    file.close();
}

// add one vehicle record in history.txt
void ParkingHub::saveHistory(Vehicle vehicle, int exitHour, int exitMinute, int duration, double bill)
{
    ofstream file(HISTORY_FILE, ios::app);
    file << vehicle.getNumber() << " "
         << vehicle.getType() << " "
         << vehicle.getSlot() << " "
         << vehicle.getEntryHour() << " "
         << vehicle.getEntryMinute() << " "
         << exitHour << " "
         << exitMinute << " "
         << duration << " "
         << fixed << setprecision(2) << bill << " "
         << vehicle.getOwner() << " "
         << vehicle.getPhone() << "\n";

    file.close();
}

// if there is no admin or user, make a default one
void ParkingHub::ensureDefaultAccounts()
{
    bool created = false;

    if (getAdminCount() == 0)
    {
        setAdminID(0, "admin");
        setAdminPass(0, "admin123");
        setAdminCount(1);
        saveAdmin("admin", "admin123");
        cout << "Default Admin Created\n";
        cout << "ID: admin\n";
        cout << "Password: admin123\n\n";
        created = true;
    }
    if (getUserCount() == 0)
    {
        setUserID(0, "user1");
        setUserPass(0, "pass1");
        setUserCount(1);
        saveUser("user1", "pass1");
        cout << "Default User Created\n";
        cout << "ID: user1\n";
        cout << "Password: pass1\n";
        created = true;
    }

    if (created)
    {
        pauseScreen();   // so the message can be read before the screen clears
    }
}

// gives position of vehicle, -1 if not found
int ParkingHub::findVehicle(string vehicleNumber)
{
    for (int i = 0; i < getVehicleCount(); i++)
    {
        if (vehicles[i].getNumber() == vehicleNumber)
        {
            return i;
        }
    }
    return -1;
}

// gives position of user, -1 if not found
int ParkingHub::findUser(string id)
{
    for (int i = 0; i < getUserCount(); i++)
    {
        if (getUserID(i) == id)
        {
            return i;
        }
    }
    return -1;
}

// gives position of admin, -1 if not found
int ParkingHub::findAdmin(string id)
{
    for (int i = 0; i < getAdminCount(); i++)
    {
        if (getAdminID(i) == id)
        {
            return i;
        }
    }
    return -1;
}

// gives a free slot number for the vehicle type, 0 if none
int ParkingHub::findFreeSlot(string type)
{
    int start, end;
    if (type == "4-Wheeler")
    {
        start = 1;
        end = FOUR_WHEELER_SLOTS;
    }
    else
    {
        start = FOUR_WHEELER_SLOTS + 1;
        end = FOUR_WHEELER_SLOTS + TWO_WHEELER_SLOTS;
    }

    for (int i = start; i <= end; i++)
    {
        if (!getSlotOccupied(i))
        {
            return i;
        }
    }
    return 0;
}

// make new user account
void ParkingHub::registerUser()
{
    string id = readLineNonEmpty("Enter User ID: ");
    if (findUser(id) != -1)
    {
        cout << "User ID already exists.\n";
        return;
    }

    if (id == "Admin" || id == "guest")
    {
        cout << "This ID is reserved.\n";
        return;
    }

    if (getUserCount() >= MAX_USERS)
    {
        cout << "User limit reached.\n";
        return;
    }

    string password = readLineNonEmpty("Enter Password: ");

    setUserID(getUserCount(), id);
    setUserPass(getUserCount(), password);
    setUserCount(getUserCount() + 1);
    saveUser(id, password);
    cout << "User registered successfully.\n";
}

// guest user
void ParkingHub::guestUser()
{
    while (true)
    {
        clearScreen();
        cout << "\nGUEST MENU\n";
        cout << "1. Add Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. View Available Slots\n";
        cout << "4. Exit\n";

        int option = readIntInRange("Enter Choice: ", 1, 4);
        if (option == 4)
        {
            return;
        }

        clearScreen();
        string guestOwner = "guest";

        switch (option)
        {
        case 1:
            addVehicle(guestOwner);
            break;
        case 2:
            removeVehicle(guestOwner, false);
            break;
        case 3:
            viewAvailableSlots();
            break;
        }
        pauseScreen();

    }
}

// check user id and password
bool ParkingHub::loginUser(string &currentUser)
{
    string id = readLineNonEmpty("Enter User ID: ");
    string password = readLineNonEmpty("Enter Password: ");
    int index = findUser(id);
    if (index != -1 && getUserPass(index) == password)
    {
        currentUser = id;
        cout << "User login successful.\n";
        return true;
    }
    cout << "Invalid User ID or Password.\n";
    return false;
}

// make new admin account
void ParkingHub::registerAdmin()
{
    string id = readLineNonEmpty("Enter New Admin ID: ");
    if (findAdmin(id) != -1)
    {
        cout << "Admin ID already exists.\n";
        return;
    }

    if (id == "Admin" || id == "guest")
    {
        cout << "This ID is reserved.\n";
        return;
    }

    if (getAdminCount() >= MAX_USERS)
    {
        cout << "Admin limit reached.\n";
        return;
    }

    string password = readLineNonEmpty("Enter Password: ");

    setAdminID(getAdminCount(), id);
    setAdminPass(getAdminCount(), password);
    setAdminCount(getAdminCount() + 1);
    saveAdmin(id, password);
    cout << "New admin registered.\n";
}

// check admin id and password
bool ParkingHub::loginAdmin(string &currentAdmin)
{
    string id = readLineNonEmpty("Enter Admin ID: ");
    string password = readLineNonEmpty("Enter Password: ");
    int index = findAdmin(id);
    if (index != -1 && getAdminPass(index) == password)
    {
        currentAdmin = id;
        cout << "Admin login successful.\n";
        return true;
    }
    cout << "Invalid Admin ID or Password.\n";
    return false;
}

// park a new vehicle
void ParkingHub::addVehicle(string owner)
{
    if (getVehicleCount() >= MAX_VEHICLES)
    {
        cout << "Parking is full.\n";
        return;
    }
    string number = readLineNonEmpty("Enter Vehicle Number: ");
    if (findVehicle(number) != -1)
    {
        cout << "Vehicle already exists.\n";
        return;
    }
    string phone = readPhone("Enter Phone Number: ");
    // choose 4-wheeler or 2-wheeler using switch case
    cout << "1. 4-Wheeler\n";
    cout << "2. 2-Wheeler\n";
    int choice = readIntInRange("Enter Vehicle Type: ", 1, 2);
    string type;
    switch (choice)
    {
    case 1:
        type = "4-Wheeler";
        break;
    case 2:
        type = "2-Wheeler";
        break;
    }

    // entry time comes from device automatically
    int entryHour = getCurrentHour();
    int entryMinute = getCurrentMinute();

    int freeSlot = findFreeSlot(type);
    if (freeSlot == 0)
    {
        cout << "No slot available.\n";
        return;
    }

    // save the vehicle in next empty place
    int index = getVehicleCount();

    vehicles[index].setNumber(number);
    vehicles[index].setType(type);
    vehicles[index].setSlot(freeSlot);
    vehicles[index].setEntryHour(entryHour);
    vehicles[index].setEntryMinute(entryMinute);
    vehicles[index].setOwner(owner);
    vehicles[index].setPhone(phone);
    setSlotOccupied(freeSlot, true);
    setVehicleCount(index + 1);

    // save all parked vehicles in file
    saveParkedVehicles();

    cout << "Vehicle parked successfully.\n";
    cout << "Assigned Slot: " << freeSlot << "\n";
    cout << "Entry Time: " << formatTime(entryHour, entryMinute) << "\n";
    cout << "Phone: " << vehicles[index].getPhone() << "\n";
    cout << "Owner: " << owner << "\n";
}

// remove vehicle and make the bill
void ParkingHub::removeVehicle(string currentUser, bool isAdmin)
{
    string number = readLineNonEmpty("Enter Vehicle Number: ");

    int index = findVehicle(number);
    if (index == -1)
    {
        cout << "Vehicle not found.\n";
        return;
    }
    // normal user can remove only his own vehicle
     if (!isAdmin && currentUser == "guest")
    {
        string phone = readPhone("Enter Phone Number used at entry: ");
        if (phone != vehicles[index].getPhone())
        {
            cout << "Phone number does not match.\n";
            return;
        }
    }
    if (!isAdmin && vehicles[index].getOwner() != currentUser)
    {
        cout << "You cannot remove this vehicle.\n";
        return;
    }

    // exit time comes from device automatically
    int exitHour = getCurrentHour();
    int exitMinute = getCurrentMinute();

    // change both times into minutes
    int entryTotal = vehicles[index].getEntryHour() * 60 + vehicles[index].getEntryMinute();
    int exitTotal = exitHour * 60 + exitMinute;

    int duration = exitTotal - entryTotal;
    // if negative, vehicle stayed past midnight
    if (duration < 0)
    {
        duration += 24 * 60;
    }

    // rate per hour (string cant be used in switch so if else)
    double rate;
    if (vehicles[index].getType() == "4-Wheeler")
    {
        rate = 60.0;
    }
    else
    {
        rate = 30.0;
    }

    // minutes to hours, then multiply with rate
    double bill = (duration / 60.0) * rate;
    setTotalRevenue(getTotalRevenue() + bill);
    saveRevenue();
    saveHistory(vehicles[index], exitHour, exitMinute, duration, bill);

    cout << "\nVehicle removed successfully.\n";
    cout << "Entry Time: " << formatTime(vehicles[index].getEntryHour(), vehicles[index].getEntryMinute()) << "\n";
    cout << "Exit Time: " << formatTime(exitHour, exitMinute) << "\n";
    cout << "Duration: " << duration / 60 << " hour(s) " << duration % 60 << " min\n";
    cout << "Bill: Rs. " << fixed << setprecision(2) << bill << "\n";

    // free the slot and move other vehicles one step left
    setSlotOccupied(vehicles[index].getSlot(), false);
    for (int i = index; i < getVehicleCount() - 1; i++)
    {
        vehicles[i] = vehicles[i + 1];
    }
    setVehicleCount(getVehicleCount() - 1);

    // save remaining parked vehicles in file
    saveParkedVehicles();
}

// search vehicle, if not found ask to search again or exit
void ParkingHub::searchVehicle()
{
    while (true)
    {
        string number = readLineNonEmpty("Enter Vehicle Number: ");
        int index = findVehicle(number);

        if (index != -1)
        {
            cout << "\nVehicle Found\n";
            cout << "Number: " << vehicles[index].getNumber() << "\n";
            cout << "Type: " << vehicles[index].getType() << "\n";
            cout << "Slot: " << vehicles[index].getSlot() << "\n";
            cout << "Entry Time: " << formatTime(vehicles[index].getEntryHour(), vehicles[index].getEntryMinute()) << "\n";
            cout << "Owner: " << vehicles[index].getOwner() << "\n";
            cout << "Phone: " << vehicles[index].getPhone() << "\n";
            return;
        }

        // vehicle not found, give choice to search again
        cout << "Vehicle not found.\n";
        cout << "1. Search Again\n";
        cout << "2. Exit\n";
        int choice = readIntInRange("Enter Choice: ", 1, 2);
        switch (choice)
        {
        case 1:
            break; // search again
        case 2:
            return; // exit
        }
    }
}

// show parked vehicles (admin sees all, user sees only his own)
void ParkingHub::viewParkedVehicles(string currentUser, bool isAdmin)
{
    int shown = 0;

    cout << "\nPARKED VEHICLES\n\n";
    cout << left << setw(14) << "Vehicle"
         << setw(12) << "Type"
         << setw(6)  << "Slot"
         << setw(8)  << "Entry"
         << setw(12) << "Owner"
         << setw(12) << "Phone" << "\n";
    cout << string(64, '-') << "\n";

    for (int i = 0; i < getVehicleCount(); i++)
    {
        if (!isAdmin && vehicles[i].getOwner() != currentUser)
        {
            continue;
        }
        cout << left << setw(14) << vehicles[i].getNumber()
             << setw(12) << vehicles[i].getType()
             << setw(6)  << vehicles[i].getSlot()
             << setw(8)  << formatTime(vehicles[i].getEntryHour(), vehicles[i].getEntryMinute())
             << setw(12) << vehicles[i].getOwner()
             << setw(12) << vehicles[i].getPhone() << "\n";
        shown++;
    }

    if (shown == 0)
    {
        cout << "No vehicles parked.\n";
    }
    else
    {
        cout << "\nTotal Parked: " << shown << "\n";
    }
}

// show empty slots
void ParkingHub::viewAvailableSlots()
{
    int fourAvailable = 0;
    int twoAvailable = 0;

    cout << "\nAvailable 4-Wheeler Slots (1-" << FOUR_WHEELER_SLOTS << "): ";
    for (int i = 1; i <= FOUR_WHEELER_SLOTS; i++)
    {
        if (!getSlotOccupied(i))
        {
            cout << i << " ";
            fourAvailable++;
        }
    }
    if (fourAvailable == 0)
    {
        cout << "No 4-Wheeler slots available";
    }
    cout << "\nTotal 4-Wheeler Slots Available: " << fourAvailable << "\n";

    cout << "\nAvailable 2-Wheeler Slots (" << FOUR_WHEELER_SLOTS + 1 << "-"
         << FOUR_WHEELER_SLOTS + TWO_WHEELER_SLOTS << "): ";
    for (int i = FOUR_WHEELER_SLOTS + 1; i <= FOUR_WHEELER_SLOTS + TWO_WHEELER_SLOTS; i++)
    {
        if (!getSlotOccupied(i))
        {
            cout << i << " ";
            twoAvailable++;
        }
    }
    if (twoAvailable == 0)
    {
        cout << "No 2-Wheeler slots available";
    }
    cout << "\nTotal 2-Wheeler Slots Available: " << twoAvailable << "\n";
}

// show total money earned
void ParkingHub::viewRevenue()
{
    cout << "\nTotal Revenue: Rs. " << fixed << setprecision(2) << getTotalRevenue() << "\n";
}

// clear history and set revenue to 0
void ParkingHub::resetDailyRecord()
{
    ofstream file(HISTORY_FILE, ios::trunc);
    file.close();
    setTotalRevenue(0.0);
    saveRevenue();
    cout << "Daily record reset successfully.\n";
}

// show all old vehicle records from history.txt
void ParkingHub::showHistory()
{
    ifstream file(HISTORY_FILE);
    if (!file)
    {
        cout << "No history found.\n";
        return;
    }
    string number;
    string phone;
    string type;
    string owner;
    int slot;
    int entryHour;
    int entryMinute;
    int exitHour;
    int exitMinute;
    int duration;
    double bill;

    cout << "\nVEHICLE HISTORY\n\n";
    cout << left << setw(12) << "Vehicle"
         << setw(12) << "Type"
         << setw(6)  << "Slot"
         << setw(8)  << "Entry"
         << setw(8)  << "Exit"
         << setw(12) << "Duration"
         << setw(10) << "Bill"
         << setw(12) << "Owner"
         << setw(12) << "Phone" << "\n";

    cout << string(92, '-') << "\n";

    while (file >> number >> type >> slot >> entryHour >> entryMinute >> exitHour >> exitMinute >> duration >> bill >> owner >> phone)
    {
        cout << left << setw(12) << number
             << setw(12) << type
             << setw(6)  << slot
             << setw(8)  << formatTime(entryHour, entryMinute)
             << setw(8)  << formatTime(exitHour, exitMinute)
             << setw(12) << to_string(duration) + " min"
             << setw(10) << fixed << setprecision(2) << bill
             << setw(12) << owner
             << setw(12) << phone << "\n";
    }
    file.close();
}

// menu for user
void ParkingHub::userPanel()
{
    while (true)
    {
        clearScreen();
        cout << "\nUSER PANEL\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Guest User\n";
        cout << "4. Exit\n";

        int choice = readIntInRange("Enter Choice: ", 1, 4);

        // switch case for menu
        switch (choice)
        {
        case 1:
            clearScreen();
            registerUser();
            pauseScreen();
            break;
        case 2:
        {
            string currentUser;
            clearScreen();
            bool loggedIn = loginUser(currentUser);
            pauseScreen();
            if (loggedIn)
            {
                userMenu(currentUser);
            }
            break;
        }
        case 3:
            clearScreen();
            guestUser();
            break;
        case 4:
            return;
        }
    }
}

// menu after user login
void ParkingHub::userMenu(string currentUser)
{
    while (true)
    {
        clearScreen();
        cout << "\nUSER MENU\n";
        cout << "1. Add Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. Search Vehicle\n";
        cout << "4. View Available Slots\n";
        cout << "5. My Parked Vehicles\n";
        cout << "6. Exit\n";

        int option = readIntInRange("Enter Choice: ", 1, 6);

        // switch case for menu
        switch (option)
        {
        case 1:
            clearScreen();
            addVehicle(currentUser);
            break;
        case 2:
            clearScreen();
            removeVehicle(currentUser, false);
            break;
        case 3:
            clearScreen();
            searchVehicle();
            break;
        case 4:
            clearScreen();
            viewAvailableSlots();
            break;
        case 5:
            clearScreen();
            viewParkedVehicles(currentUser, false);
            break;
        case 6:
            return;
        }
        pauseScreen();
    }
}

// admin login and admin menu
void ParkingHub::adminPanel()
{
    string currentAdmin;
    bool loggedIn = loginAdmin(currentAdmin);
    pauseScreen();   // so the login message can be read
    if (!loggedIn)
    {
        return;
    }
    while (true)
    {
        clearScreen();
        

        cout << "\nADMIN MENU\n";
        cout << "1. Register Admin\n";
        cout << "2. Add Vehicle\n";
        cout << "3. Remove Vehicle\n";
        cout << "4. Search Vehicle\n";
        cout << "5. View Parked Vehicles\n";
        cout << "6. View Available Slots\n";
        cout << "7. View Revenue\n";
        cout << "8. Reset Daily Record\n";
        cout << "9. View History\n";
        cout << "10. Exit\n";

        int option = readIntInRange("Enter Choice: ", 1, 10);

        switch (option)
        {
        case 1:
            clearScreen();
            registerAdmin();
            break;
        case 2:
            clearScreen();
            addVehicle("Admin");
            break;
        case 3:
            clearScreen();
            removeVehicle(currentAdmin, true);
            break;
        case 4:
            clearScreen();
            searchVehicle();
            break;
        case 5:
            clearScreen();
            viewParkedVehicles(currentAdmin, true);
            break;
        case 6:
            clearScreen();
            viewAvailableSlots();
            break;
        case 7:
            clearScreen();
            viewRevenue();
            break;
        case 8:
            clearScreen();
            resetDailyRecord();
            break;
        case 9:
            clearScreen();
            showHistory();
            break;
        case 10:
            return;
        }
        pauseScreen();
    }
}

// program starts from here
void ParkingHub::start()
{
    while (true)
    {
        clearScreen();
        cout << "\nWELCOME TO PARKING HUB\n";
        cout << "\n1. User\n";
        cout << "2. Admin\n";
        cout << "3. Exit\n";

        int choice = readIntInRange("Enter Choice: ", 1, 3);

        switch (choice)
        {
        case 1:
            clearScreen();
            userPanel();
            break;
        case 2:
            clearScreen();
            adminPanel();
            break;
        case 3:
            cout << "Thank you for using Parking Hub." << endl;
            return;
        }
    }
}

// main function
int main()
{
    // make object of the class
    ParkingHub parking;
    // run the program
    parking.start();
    return 0;
}
