#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>

using namespace std;

//======================================================
// CONSOLE COLOR CODES
//======================================================
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

//======================================================
// UTILITY FUNCTIONS
//======================================================
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

string toLower(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
    return str;
}

int stringToInt(const string& str) {
    return atoi(str.c_str());
}

double stringToDouble(const string& str) {
    return atof(str.c_str());
}

string doubleToString(double num) {
    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num = -num;
    }
    long long intPart = (long long)num;
    double decPart = num - intPart;
    int decDigits = (int)((decPart * 100) + 0.5);
    if (decDigits >= 100) {
        intPart++;
        decDigits = 0;
    }
    string result = "";
    if (intPart == 0) {
        result = "0";
    }
    else {
        long long temp = intPart;
        while (temp > 0) {
            result = char('0' + (temp % 10)) + result;
            temp /= 10;
        }
    }
    result += ".";
    if (decDigits < 10) result += "0";
    if (decDigits == 0) result += "0";
    else {
        string dec = "";
        int temp = decDigits;
        while (temp > 0) {
            dec = char('0' + (temp % 10)) + dec;
            temp /= 10;
        }
        result += dec;
    }
    if (isNegative) result = "-" + result;
    return result;
}

string formatNumber(double num) {
    string result = doubleToString(num);

    int insertPos = result.find('.');
    if (insertPos == string::npos) {
        insertPos = result.length();
    }
    insertPos -= 3;

    while (insertPos > 0) {
        result.insert(insertPos, ",");
        insertPos -= 3;
    }
    return result;
}

void splitString(const string& str, char delimiter, string result[], int& count) {
    count = 0;
    string current = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == delimiter) {
            result[count++] = current;
            current = "";
        }
        else {
            current += str[i];
        }
    }
    if (!current.empty() || str[str.length() - 1] == delimiter) {
        result[count++] = current;
    }
}

//======================================================
// LENDER INTERFACE CLASS
//======================================================
class LenderInterface {
private:
    struct Application {
        string applicationId;
        string fullName;
        string fatherName;
        string postalAddress;
        string contactNumber;
        string email;
        string cnic;
        string cnicExpiryDate;
        string employmentStatus;
        string maritalStatus;
        string gender;
        int numberOfDependents;
        double annualIncome;
        double avgElectricityBill;
        double currentElectricityBill;
        bool hasLoan;
        string loanStatus;
        double totalLoanAmount;
        double amountReturned;
        double amountDue;
        string bankName;
        string loanCategory;
        string ref1Name;
        string ref1CNIC;
        string ref1IssueDate;
        string ref1Phone;
        string ref1Email;
        string ref2Name;
        string ref2CNIC;
        string ref2IssueDate;
        string ref2Phone;
        string ref2Email;
        string cnicFrontPath;
        string cnicBackPath;
        string electricityBillPath;
        string salarySlipPath;
        string status;
        string selectedLoanType;
        string selectedLoanCategory;
        double loanAmount;
        int installmentMonths;
    };

    Application* applications;
    int appCount;
    int appCapacity;

    void displayWelcomeScreen() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n";
        cout << "  ============================================================\n";
        cout << "  ||                                                        ||\n";
        setColor(LIGHT_YELLOW);
        cout << "  ||        LOAN-BUDDY LENDER MANAGEMENT SYSTEM             ||\n";
        setColor(LIGHT_CYAN);
        cout << "  ||                                                        ||\n";
        setColor(MAGENTA);
        cout << "  ||          Manage Loan Applications                      ||\n";
        setColor(LIGHT_CYAN);
        cout << "  ||                                                        ||\n";
        cout << "  ============================================================\n";
        setColor(WHITE);
        cout << "\n";
    }

    void loadApplications() {
        ifstream file("applications.txt");
        if (!file.is_open()) {
            setColor(LIGHT_YELLOW);
            cout << "  No applications file found.\n";
            setColor(WHITE);
            return;
        }

        appCount = 0;
        string line;

        while (getline(file, line)) {
            if (appCount >= appCapacity) {
                appCapacity *= 2;
                Application* newApps = new Application[appCapacity];
                for (int i = 0; i < appCount; i++) {
                    newApps[i] = applications[i];
                }
                delete[] applications;
                applications = newApps;
            }

            string parts[50];
            int count = 0;
            splitString(line, '#', parts, count);

            if (count >= 37) {
                applications[appCount].applicationId = parts[0];
                applications[appCount].fullName = parts[1];
                applications[appCount].fatherName = parts[2];
                applications[appCount].postalAddress = parts[3];
                applications[appCount].contactNumber = parts[4];
                applications[appCount].email = parts[5];
                applications[appCount].cnic = parts[6];
                applications[appCount].cnicExpiryDate = parts[7];
                applications[appCount].employmentStatus = parts[8];
                applications[appCount].maritalStatus = parts[9];
                applications[appCount].gender = parts[10];
                applications[appCount].numberOfDependents = stringToInt(parts[11]);
                applications[appCount].annualIncome = stringToDouble(parts[12]);
                applications[appCount].avgElectricityBill = stringToDouble(parts[13]);
                applications[appCount].currentElectricityBill = stringToDouble(parts[14]);
                applications[appCount].hasLoan = (parts[15] == "Yes");
                applications[appCount].loanStatus = parts[16];
                applications[appCount].totalLoanAmount = stringToDouble(parts[17]);
                applications[appCount].amountReturned = stringToDouble(parts[18]);
                applications[appCount].amountDue = stringToDouble(parts[19]);
                applications[appCount].bankName = parts[20];
                applications[appCount].loanCategory = parts[21];
                applications[appCount].ref1Name = parts[22];
                applications[appCount].ref1CNIC = parts[23];
                applications[appCount].ref1IssueDate = parts[24];
                applications[appCount].ref1Phone = parts[25];
                applications[appCount].ref1Email = parts[26];
                applications[appCount].ref2Name = parts[27];
                applications[appCount].ref2CNIC = parts[28];
                applications[appCount].ref2IssueDate = parts[29];
                applications[appCount].ref2Phone = parts[30];
                applications[appCount].ref2Email = parts[31];
                applications[appCount].cnicFrontPath = parts[32];
                applications[appCount].cnicBackPath = parts[33];
                applications[appCount].electricityBillPath = parts[34];
                applications[appCount].salarySlipPath = parts[35];
                applications[appCount].status = parts[36];

                if (count > 37) applications[appCount].selectedLoanType = parts[37];
                if (count > 38) applications[appCount].selectedLoanCategory = parts[38];
                if (count > 39) applications[appCount].loanAmount = stringToDouble(parts[39]);
                if (count > 40) applications[appCount].installmentMonths = stringToInt(parts[40]);

                appCount++;
            }
        }
        file.close();
    }

    void saveApplications() {
        ofstream file("applications.txt");
        if (!file.is_open()) {
            setColor(LIGHT_RED);
            cout << "  Error: Could not save applications!\n";
            setColor(WHITE);
            return;
        }

        for (int i = 0; i < appCount; i++) {
            file << applications[i].applicationId << "#"
                << applications[i].fullName << "#"
                << applications[i].fatherName << "#"
                << applications[i].postalAddress << "#"
                << applications[i].contactNumber << "#"
                << applications[i].email << "#"
                << applications[i].cnic << "#"
                << applications[i].cnicExpiryDate << "#"
                << applications[i].employmentStatus << "#"
                << applications[i].maritalStatus << "#"
                << applications[i].gender << "#"
                << applications[i].numberOfDependents << "#"
                << applications[i].annualIncome << "#"
                << applications[i].avgElectricityBill << "#"
                << applications[i].currentElectricityBill << "#"
                << (applications[i].hasLoan ? "Yes" : "No") << "#"
                << applications[i].loanStatus << "#"
                << applications[i].totalLoanAmount << "#"
                << applications[i].amountReturned << "#"
                << applications[i].amountDue << "#"
                << applications[i].bankName << "#"
                << applications[i].loanCategory << "#"
                << applications[i].ref1Name << "#"
                << applications[i].ref1CNIC << "#"
                << applications[i].ref1IssueDate << "#"
                << applications[i].ref1Phone << "#"
                << applications[i].ref1Email << "#"
                << applications[i].ref2Name << "#"
                << applications[i].ref2CNIC << "#"
                << applications[i].ref2IssueDate << "#"
                << applications[i].ref2Phone << "#"
                << applications[i].ref2Email << "#"
                << applications[i].cnicFrontPath << "#"
                << applications[i].cnicBackPath << "#"
                << applications[i].electricityBillPath << "#"
                << applications[i].salarySlipPath << "#"
                << applications[i].status << "#"
                << applications[i].selectedLoanType << "#"
                << applications[i].selectedLoanCategory << "#"
                << applications[i].loanAmount << "#"
                << applications[i].installmentMonths << endl;
        }
        file.close();
    }

    void displayAllApplications() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ============================================================\n";
        setColor(LIGHT_YELLOW);
        cout << "                   ALL LOAN APPLICATIONS\n";
        setColor(LIGHT_CYAN);
        cout << "  ============================================================\n";
        setColor(WHITE);

        if (appCount == 0) {
            setColor(LIGHT_YELLOW);
            cout << "\n  No applications found.\n";
            setColor(WHITE);
            return;
        }

        setColor(LIGHT_GREEN);
        cout << "\n  " << left << setw(12) << "App ID"
            << setw(25) << "Name"
            << setw(15) << "CNIC"
            << setw(15) << "Status"
            << setw(15) << "Loan Type" << endl;
        setColor(LIGHT_BLUE);
        cout << "  " << string(82, '-') << endl;
        setColor(WHITE);

        for (int i = 0; i < appCount; i++) {
            cout << "  " << left << setw(12) << applications[i].applicationId
                << setw(25) << applications[i].fullName
                << setw(15) << applications[i].cnic
                << setw(15) << applications[i].status
                << setw(15) << applications[i].selectedLoanType << endl;
        }
        cout << endl;
    }

    void viewPendingApplications() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ============================================================\n";
        setColor(LIGHT_YELLOW);
        cout << "                  PENDING LOAN APPLICATIONS\n";
        setColor(LIGHT_CYAN);
        cout << "  ============================================================\n";
        setColor(WHITE);

        int pendingCount = 0;
        for (int i = 0; i < appCount; i++) {
            if (applications[i].status == "Submitted") {
                pendingCount++;
            }
        }

        if (pendingCount == 0) {
            setColor(LIGHT_YELLOW);
            cout << "\n  No pending applications found.\n";
            setColor(WHITE);
            return;
        }

        setColor(LIGHT_GREEN);
        cout << "\n  " << left << setw(12) << "App ID"
            << setw(25) << "Name"
            << setw(15) << "CNIC"
            << setw(15) << "Loan Type"
            << setw(15) << "Amount" << endl;
        setColor(LIGHT_BLUE);
        cout << "  " << string(82, '-') << endl;
        setColor(WHITE);

        for (int i = 0; i < appCount; i++) {
            if (applications[i].status == "Submitted") {
                cout << "  " << left << setw(12) << applications[i].applicationId
                    << setw(25) << applications[i].fullName
                    << setw(15) << applications[i].cnic
                    << setw(15) << applications[i].selectedLoanType
                    << "Rs. " << formatNumber(applications[i].loanAmount) << endl;
            }
        }
        cout << endl;
    }

    void viewApplicationDetails(const string& appId) {
        int index = -1;
        for (int i = 0; i < appCount; i++) {
            if (applications[i].applicationId == appId) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            setColor(LIGHT_RED);
            cout << "\n  Application not found!\n";
            setColor(WHITE);
            return;
        }

        system("cls");
        Application& app = applications[index];

        setColor(LIGHT_CYAN);
        cout << "\n  ============================================================\n";
        setColor(LIGHT_YELLOW);
        cout << "              APPLICATION DETAILS - " << app.applicationId << "\n";
        setColor(LIGHT_CYAN);
        cout << "  ============================================================\n";
        setColor(WHITE);

        setColor(LIGHT_GREEN);
        cout << "\n  PERSONAL INFORMATION:\n";
        setColor(WHITE);
        cout << "    Full Name: " << app.fullName << endl;
        cout << "    Father's Name: " << app.fatherName << endl;
        cout << "    CNIC: " << app.cnic << " (Expires: " << app.cnicExpiryDate << ")" << endl;
        cout << "    Contact: " << app.contactNumber << endl;
        cout << "    Email: " << app.email << endl;
        cout << "    Address: " << app.postalAddress << endl;
        cout << "    Employment: " << app.employmentStatus << endl;
        cout << "    Marital Status: " << app.maritalStatus << endl;
        cout << "    Gender: " << app.gender << endl;
        cout << "    Dependents: " << app.numberOfDependents << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  FINANCIAL INFORMATION:\n";
        setColor(WHITE);
        cout << "    Annual Income: Rs. " << formatNumber(app.annualIncome) << endl;
        cout << "    Avg. Electricity Bill: Rs. " << formatNumber(app.avgElectricityBill) << endl;
        cout << "    Current Electricity Bill: Rs. " << formatNumber(app.currentElectricityBill) << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  EXISTING LOAN:\n";
        setColor(WHITE);
        if (app.hasLoan) {
            cout << "    Has Loan: Yes (" << app.loanStatus << ")" << endl;
            cout << "    Total Amount: Rs. " << formatNumber(app.totalLoanAmount) << endl;
            cout << "    Amount Returned: Rs. " << formatNumber(app.amountReturned) << endl;
            cout << "    Amount Due: Rs. " << formatNumber(app.amountDue) << endl;
            cout << "    Bank: " << app.bankName << endl;
            cout << "    Category: " << app.loanCategory << endl;
        }
        else {
            cout << "    No existing loans" << endl;
        }

        setColor(LIGHT_GREEN);
        cout << "\n  REFERENCES:\n";
        setColor(WHITE);
        cout << "    Reference 1: " << app.ref1Name << " (" << app.ref1CNIC << ")" << endl;
        cout << "    Reference 2: " << app.ref2Name << " (" << app.ref2CNIC << ")" << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  LOAN REQUEST:\n";
        setColor(WHITE);
        cout << "    Loan Type: " << app.selectedLoanType << endl;
        cout << "    Category: " << app.selectedLoanCategory << endl;
        cout << "    Amount: Rs. " << formatNumber(app.loanAmount) << endl;
        cout << "    Installments: " << app.installmentMonths << " months" << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  STATUS: ";
        if (app.status == "Submitted") setColor(LIGHT_YELLOW);
        else if (app.status == "Approved") setColor(LIGHT_GREEN);
        else if (app.status == "Rejected") setColor(LIGHT_RED);
        cout << app.status << endl;
        setColor(WHITE);
    }

    void changeApplicationStatus() {
        setColor(LIGHT_YELLOW);
        cout << "\n  Enter Application ID: ";
        setColor(BRIGHT_WHITE);
        string appId;
        getline(cin, appId);
        appId = trim(appId);

        int index = -1;
        for (int i = 0; i < appCount; i++) {
            if (applications[i].applicationId == appId) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            setColor(LIGHT_RED);
            cout << "\n  Application not found!\n";
            setColor(WHITE);
            return;
        }

        viewApplicationDetails(appId);

        setColor(LIGHT_YELLOW);
        cout << "\n  Change status to:\n";
        setColor(WHITE);
        cout << "    1. Approve\n";
        cout << "    2. Reject\n";
        cout << "    3. Cancel\n";

        setColor(LIGHT_MAGENTA);
        cout << "\n  Enter choice (1-3): ";
        setColor(BRIGHT_WHITE);
        string choice;
        getline(cin, choice);
        choice = trim(choice);

        if (choice == "1") {
            applications[index].status = "Approved";
            saveApplications();
            setColor(LIGHT_GREEN);
            cout << "\n  Application APPROVED successfully!\n";
            setColor(WHITE);
        }
        else if (choice == "2") {
            applications[index].status = "Rejected";
            saveApplications();
            setColor(LIGHT_RED);
            cout << "\n  Application REJECTED.\n";
            setColor(WHITE);
        }
        else {
            setColor(LIGHT_YELLOW);
            cout << "\n  Status change cancelled.\n";
            setColor(WHITE);
        }
    }

    void displayStatistics() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ============================================================\n";
        setColor(LIGHT_YELLOW);
        cout << "                    APPLICATION STATISTICS\n";
        setColor(LIGHT_CYAN);
        cout << "  ============================================================\n";
        setColor(WHITE);

        int total = appCount;
        int submitted = 0, approved = 0, rejected = 0, incomplete = 0;

        for (int i = 0; i < appCount; i++) {
            if (applications[i].status == "Submitted") submitted++;
            else if (applications[i].status == "Approved") approved++;
            else if (applications[i].status == "Rejected") rejected++;
            else if (applications[i].status == "C1" || applications[i].status == "C2" ||
                applications[i].status == "C3") incomplete++;
        }

        setColor(LIGHT_GREEN);
        cout << "\n  Total Applications: ";
        setColor(BRIGHT_WHITE);
        cout << total << endl;

        setColor(LIGHT_YELLOW);
        cout << "  Pending (Submitted): ";
        setColor(BRIGHT_WHITE);
        cout << submitted << endl;

        setColor(LIGHT_GREEN);
        cout << "  Approved: ";
        setColor(BRIGHT_WHITE);
        cout << approved << endl;

        setColor(LIGHT_RED);
        cout << "  Rejected: ";
        setColor(BRIGHT_WHITE);
        cout << rejected << endl;

        setColor(LIGHT_MAGENTA);
        cout << "  Incomplete: ";
        setColor(BRIGHT_WHITE);
        cout << incomplete << endl;

        setColor(WHITE);
        cout << endl;
    }

public:
    LenderInterface() {
        appCapacity = 100;
        appCount = 0;
        applications = new Application[appCapacity];
    }

    ~LenderInterface() {
        delete[] applications;
    }

    void run() {
        displayWelcomeScreen();
        loadApplications();

        setColor(LIGHT_GREEN);
        cout << "  Loaded " << appCount << " application(s) from database.\n";
        setColor(WHITE);
        Sleep(1500);

        bool running = true;
        while (running) {
            system("cls");
            setColor(LIGHT_CYAN);
            cout << "\n  ============================================================\n";
            setColor(LIGHT_YELLOW);
            cout << "                    LENDER DASHBOARD\n";
            setColor(LIGHT_CYAN);
            cout << "  ============================================================\n";
            setColor(WHITE);

            setColor(LIGHT_GREEN);
            cout << "\n  1. View All Applications\n";
            cout << "  2. View Pending Applications\n";
            cout << "  3. View Application Details\n";
            cout << "  4. Change Application Status\n";
            cout << "  5. View Statistics\n";
            cout << "  6. Refresh Data\n";
            cout << "  7. Exit\n";
            setColor(WHITE);

            setColor(LIGHT_MAGENTA);
            cout << "\n  Enter your choice (1-7): ";
            setColor(BRIGHT_WHITE);
            string choice;
            getline(cin, choice);
            choice = trim(choice);

            if (choice == "1") {
                displayAllApplications();
                cout << "\n  Press Enter to continue...";
                cin.get();
            }
            else if (choice == "2") {
                viewPendingApplications();
                cout << "\n  Press Enter to continue...";
                cin.get();
            }
            else if (choice == "3") {
                setColor(LIGHT_YELLOW);
                cout << "\n  Enter Application ID: ";
                setColor(BRIGHT_WHITE);
                string appId;
                getline(cin, appId);
                viewApplicationDetails(trim(appId));
                cout << "\n  Press Enter to continue...";
                cin.get();
            }
            else if (choice == "4") {
                changeApplicationStatus();
                cout << "\n  Press Enter to continue...";
                cin.get();
            }
            else if (choice == "5") {
                displayStatistics();
                cout << "\n  Press Enter to continue...";
                cin.get();
            }
            else if (choice == "6") {
                loadApplications();
                setColor(LIGHT_GREEN);
                cout << "\n  Data refreshed! Loaded " << appCount << " applications.\n";
                setColor(WHITE);
                Sleep(1500);
            }
            else if (choice == "7") {
                setColor(LIGHT_MAGENTA);
                cout << "\n\n  Thank you for using the Lender Management System!\n";
                cout << "  Goodbye!\n\n";
                setColor(WHITE);
                Sleep(2000);
                running = false;
            }
            else {
                setColor(LIGHT_RED);
                cout << "\n  Invalid choice! Please try again.\n";
                setColor(WHITE);
                Sleep(1500);
            }
        }
    }
};

//======================================================
// MAIN FUNCTION
//======================================================
int main() {
    LenderInterface lender;
    lender.run();
    return 0;
}