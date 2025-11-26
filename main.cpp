#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <direct.h>
#include <ctime>

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
// FUNCTION: setColor
// Aim: Sets console text color using Windows API
//======================================================

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

//======================================================
// UTILITY FUNCTIONS
//======================================================
string intToString(int num) {
    if (num == 0) return "0";
    string result = "";
    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num = -num;
    }
    while (num > 0) {
        result = char('0' + (num % 10)) + result;
        num /= 10;
    }
    if (isNegative) result = "-" + result;
    return result;
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
    if (decDigits > 0) {
        result += ".";
        if (decDigits < 10) result += "0";
        result += intToString(decDigits);
    }
    if (isNegative) result = "-" + result;
    return result;
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

string toUpper(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
    }
    return str;
}

//======================================================
// STRUCTURES
//======================================================
struct Utterance {
    string input;
    string response;
};

struct ConversationPair {
    string human1;  // Question/Input
    string human2;  // Response
};


struct ExistingLoan {
    bool hasLoan;
    string loanStatus;
    double totalLoanAmount;
    double amountReturned;
    double amountDue;
    string bankName;
    string loanCategory;
};

struct Reference {
    string name;
    string cnic;
    string cnicIssueDate;
    string phoneNumber;
    string email;
};

struct UserApplication {
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
    ExistingLoan loanInfo;
    Reference reference1;
    Reference reference2;
    string cnicFrontPath;
    string cnicBackPath;
    string electricityBillPath;
    string salarySlipPath;
    string status; // C1, C2, C3, Submitted, Approved, Rejected
    string selectedLoanType;
    string selectedLoanCategory;
    double loanAmount;
    int installmentMonths;
};

struct LoanOption {
    string category;
    string details;
    string installments;
    string price;
    string downPayment;
};

//======================================================
// VALIDATION FUNCTIONS
//======================================================

bool isValidNumber(const string& str) {
    if (str.empty()) return false;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }
    return true;
}

bool isValidAmount(const string& amount) {
    if (amount.empty()) return false;
    for (size_t i = 0; i < amount.length(); i++) {
        if (amount[i] < '0' || amount[i] > '9') return false;
    }
    return true;
}

string isValidEmail(const string& email) {
    if (email.length() < 5 || email.length() > 254)
        return "Email must be between 5 and 254 characters.";

    size_t atPos = email.find('@');
    if (atPos == string::npos)
        return "Email must contain '@' symbol.";
    if (atPos == 0)
        return "Email cannot start with '@'.";
    if (atPos == email.length() - 1)
        return "Email cannot end with '@'.";

    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos)
        return "Email domain must contain a dot ('.') after '@'.";
    if (dotPos == atPos + 1)
        return "Dot cannot immediately follow '@'.";
    if (dotPos == email.length() - 1)
        return "Email cannot end with a dot.";

    string localPart = email.substr(0, atPos);
    if (localPart.empty())
        return "Email local part (before '@') cannot be empty.";

    for (char c : localPart) {
        if (!isalnum(c) && c != '.' && c != '_' && c != '-') {
            return "Local part contains invalid characters (allowed: letters, digits, '.', '_', '-').";
        }
    }

    string domainPart = email.substr(atPos + 1);
    if (domainPart.empty())
        return "Email domain part (after '@') cannot be empty.";

    for (char c : domainPart) {
        if (!isalnum(c) && c != '.' && c != '-') {
            return "Domain part contains invalid characters (allowed: letters, digits, '.', '-').";
        }
    }

    if (email.find("..") != string::npos)
        return "Email cannot contain consecutive dots ('..').";

    return "";
}

bool isValidCNIC(const string& cnic) {
    if (cnic.length() != 13) {
        return false;
    }

    for (size_t i = 0; i < cnic.length(); i++) {
        if (cnic[i] < '0' || cnic[i] > '9') {
            return false;
        }
    }

    return true;
}

bool isValidPhone(const string& phone) {
    string cleanPhone = "";
    for (size_t i = 0; i < phone.length(); i++) {
        if (phone[i] >= '0' && phone[i] <= '9') {
            cleanPhone += phone[i];
        }
    }
    return cleanPhone.length() >= 10 && cleanPhone.length() <= 11;
}

string getCNICExpiryValidationError(const string& expiryDate) {
    if (expiryDate.length() != 10)
        return "Expiry date must be in DD-MM-YYYY format (10 characters).";

    if (expiryDate[2] != '-' || expiryDate[5] != '-')
        return "Expiry date must have dashes at positions 3 and 6 (DD-MM-YYYY).";

    string dayStr = expiryDate.substr(0, 2);
    string monthStr = expiryDate.substr(3, 2);
    string yearStr = expiryDate.substr(6, 4);

    for (char c : dayStr) if (!isdigit(c)) return "Day must contain only digits.";
    for (char c : monthStr) if (!isdigit(c)) return "Month must contain only digits.";
    for (char c : yearStr) if (!isdigit(c)) return "Year must contain only digits.";

    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);

    if (year < 2020 || year > 2050)
        return "Year must be between 2020 and 2050.";

    if (month < 1 || month > 12)
        return "Month must be between 01 and 12.";

    int daysInMonth;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        daysInMonth = isLeap ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    }
    else {
        daysInMonth = 31;
    }

    if (day < 1 || day > daysInMonth)
        return "Invalid day for the given month.";

    return "";
}

int stringToInt(const string& str) {
    return atoi(str.c_str());
}

string getCNICIssueValidationError(const string& date) {
    if (date.length() != 10) return "Date must be in DD-MM-YYYY format.";
    if (date[2] != '-' || date[5] != '-') return "Dashes must be at correct positions (DD-MM-YYYY).";

    string dayStr = date.substr(0, 2);
    string monthStr = date.substr(3, 2);
    string yearStr = date.substr(6, 4);

    for (char c : dayStr) if (!isdigit(c)) return "Day must be numeric.";
    for (char c : monthStr) if (!isdigit(c)) return "Month must be numeric.";
    for (char c : yearStr) if (!isdigit(c)) return "Year must be numeric.";

    int day = stringToInt(dayStr);
    int month = stringToInt(monthStr);
    int year = stringToInt(yearStr);

    if (year < 1950 || year > 2025) return "Year must be between 1950 and 2025.";

    int daysInMonth;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        daysInMonth = isLeap ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    }
    else if (month >= 1 && month <= 12) {
        daysInMonth = 31;
    }
    else {
        return "Month must be between 1 and 12.";
    }

    if (day < 1 || day > daysInMonth) return "Day is invalid for the given month.";

    return "";
}

string capitalizeName(const string& name) {
    string result = "";
    bool capitalizeNext = true;

    for (size_t i = 0; i < name.length(); i++) {
        if (name[i] == ' ') {
            result += ' ';
            capitalizeNext = true;
        }
        else if (capitalizeNext) {
            if (name[i] >= 'a' && name[i] <= 'z') {
                result += char(name[i] - 32);
            }
            else {
                result += name[i];
            }
            capitalizeNext = false;
        }
        else {
            if (name[i] >= 'A' && name[i] <= 'Z') {
                result += char(name[i] + 32);
            }
            else {
                result += name[i];
            }
        }
    }
    return result;
}

bool isValidFullName(const string& name) {
    if (name.empty()) return false;

    int wordCount = 0;
    bool inWord = false;

    for (size_t i = 0; i < name.length(); i++) {
        if (name[i] == ' ') {
            if (inWord) {
                wordCount++;
                inWord = false;
            }
        }
        else if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')) {
            inWord = true;
        }
        else {
            return false;
        }
    }
    if (inWord) wordCount++;

    return wordCount >= 2;
}

string isValidAddress(const string& address) {
    if (address.length() < 10 || address.length() > 200)
        return "Postal address must be between 10 and 200 characters.";

    for (char c : address) {
        if (!isalnum(c) && c != ' ' && c != ',' && c != '.' && c != '-' && c != '/' && c != '#' && c != '\\')
            return "Postal address contains invalid characters.";
    }

    bool hasLetters = false;
    for (char c : address) {
        if (isalpha(c)) {
            hasLetters = true;
            break;
        }
    }
    if (!hasLetters)
        return "Postal address must contain at least one letter.";

    if (address.find("  ") != string::npos)
        return "Postal address cannot contain multiple consecutive spaces.";

    if (address[0] == ' ' || address[address.length() - 1] == ' ')
        return "Postal address cannot start or end with a space.";

    return "";
}

string generateApplicationId() {
    static int counter = -1; // -1 means not initialized yet

    // First time: read last ID from file
    if (counter == -1) {
        ifstream file("last_app_id.txt");
        if (file.is_open()) {
            string lastId;
            getline(file, lastId);
            counter = stringToInt(lastId);
            file.close();
        }
        else {
            counter = 1000; // Start from 1000 if file doesn't exist
        }
    }

    // Increment counter
    counter++;

    // Save new counter to file
    ofstream outFile("last_app_id.txt");
    if (outFile.is_open()) {
        outFile << counter;
        outFile.close();
    }

    return intToString(counter);
}

string getMonthName(int month) {
    string months[] = { "January", "February", "March", "April", "May", "June",
                      "July", "August", "September", "October", "November", "December" };
    if (month >= 1 && month <= 12) return months[month - 1];
    return "Invalid";
}

//======================================================
// LOAN APPLICATION SYSTEM CLASS
//======================================================
class LoanApplicationSystem {
private:
    Utterance* utterances;
    int utteranceCount;
    int utteranceCapacity;

    ConversationPair* conversations;
    int conversationCount;
    int conversationCapacity;


    LoanOption* homeLoanOptions;
    int homeCount;
    int homeCapacity;

    LoanOption* carLoanOptions;
    int carCount;
    int carCapacity;

    LoanOption* bikeLoanOptions;
    int bikeCount;
    int bikeCapacity;

    string defaultResponse;
    string chatbotName;
    UserApplication currentApp;

    double stringToDouble(const string& str) {
        return atof(str.c_str());
    }

    int stringToInt(const string& str) {
        return atoi(str.c_str());
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

    void resizeUtterances() {
        utteranceCapacity *= 2;
        Utterance* newUtterances = new Utterance[utteranceCapacity];
        for (int i = 0; i < utteranceCount; i++) {
            newUtterances[i] = utterances[i];
        }
        delete[] utterances;
        utterances = newUtterances;
    }

    void resizeLoanArray(LoanOption*& array, int& capacity) {
        capacity *= 2;
        LoanOption* newArray = new LoanOption[capacity];
        for (int i = 0; i < capacity / 2; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
    }

    string getValidInput(const string& prompt, bool allowX = true) {
        string input;
        while (true) {
            setColor(LIGHT_YELLOW);
            cout << prompt;
            setColor(BRIGHT_WHITE);
            getline(cin, input);
            input = trim(input);

            if (input.empty()) {
                setColor(LIGHT_RED);
                cout << "  Input cannot be empty. Please try again." << endl;
                setColor(WHITE);
                continue;
            }

            if (allowX && toLower(input) == "x") {
                return "x";
            }

            return input;
        }
    }

    int getValidNumberInput(const string& prompt, int min, int max) {
        string input;
        while (true) {
            setColor(LIGHT_MAGENTA);
            cout << prompt;
            setColor(BRIGHT_WHITE);
            getline(cin, input);
            input = trim(input);

            if (input.empty()) {
                setColor(LIGHT_RED);
                cout << "  Input cannot be empty. Please try again." << endl;
                setColor(WHITE);
                continue;
            }

            if (!isValidNumber(input)) {
                setColor(LIGHT_RED);
                cout << "  Invalid input! Please enter a number between " << min << " and " << max << "." << endl;
                setColor(WHITE);
                continue;
            }

            int value = stringToInt(input);
            if (value < min || value > max) {
                setColor(LIGHT_RED);
                cout << "  Invalid option! Please enter a number between " << min << " and " << max << "." << endl;
                setColor(WHITE);
                continue;
            }

            return value;
        }
    }

    void displayWelcomeScreen() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n";
        cout << "  ========================================================\n";
        cout << "  ||                                                    ||\n";
        setColor(LIGHT_YELLOW);
        cout << "  ||              WELCOME TO LOAN-BUDDY                 ||\n";
        setColor(LIGHT_CYAN);
        cout << "  ||                                                    ||\n";
        setColor(MAGENTA);
        cout << "  ||           Your Smart Loan Assistant                ||\n";
        setColor(LIGHT_CYAN);
        cout << "  ||                                                    ||\n";
        cout << "  ========================================================\n";
        setColor(WHITE);
        cout << "\n";
    }

    void displayGoodbyeScreen() {
        system("cls");
        setColor(LIGHT_MAGENTA);
        cout << "\n\n";
        cout << "  ========================================================\n";
        cout << "  ||                                                    ||\n";
        setColor(LIGHT_YELLOW);
        cout << "  ||                  BYE BYE! :)                       ||\n";
        setColor(LIGHT_MAGENTA);
        cout << "  ||                                                    ||\n";
        setColor(LIGHT_GREEN);
        cout << "  ||          Thank you for using LOAN-BUDDY!           ||\n";
        setColor(LIGHT_MAGENTA);
        cout << "  ||                                                    ||\n";
        cout << "  ========================================================\n";
        setColor(WHITE);
        cout << "\n\n";
        Sleep(2000);
    }

    //======================================================
    // TOKENIZATION AND IoU FUNCTIONS
    //======================================================

    // Tokenize a string into words (split by space)
    void tokenize(const string& text, string tokens[], int& tokenCount) {
        tokenCount = 0;
        string current = "";

        for (size_t i = 0; i < text.length(); i++) {
            char c = text[i];
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                if (!current.empty()) {
                    tokens[tokenCount++] = current;
                    current = "";
                }
            }
            else {
                current += c;
            }
        }

        if (!current.empty()) {
            tokens[tokenCount++] = current;
        }
    }

    // Calculate Intersection over Union (IoU)
    double calculateIoU(const string& userInput, const string& corpusInput) {
        // Tokenize both strings
        string userTokens[1000];
        string corpusTokens[1000];
        int userTokenCount = 0;
        int corpusTokenCount = 0;

        // Convert to lowercase for comparison
        string lowerUser = toLower(userInput);
        string lowerCorpus = toLower(corpusInput);

        tokenize(lowerUser, userTokens, userTokenCount);
        tokenize(lowerCorpus, corpusTokens, corpusTokenCount);

        if (userTokenCount == 0 || corpusTokenCount == 0) {
            return 0.0;
        }

        // Calculate intersection (common tokens)
        int intersection = 0;
        for (int i = 0; i < userTokenCount; i++) {
            for (int j = 0; j < corpusTokenCount; j++) {
                if (userTokens[i] == corpusTokens[j]) {
                    intersection++;
                    break; // Count each token only once
                }
            }
        }

        // Calculate union (unique tokens in both sets)
        // Create a combined array of all tokens
        string allTokens[2000];
        int allCount = 0;

        // Add all user tokens
        for (int i = 0; i < userTokenCount; i++) {
            allTokens[allCount++] = userTokens[i];
        }

        // Add corpus tokens that are not already in the combined array
        for (int i = 0; i < corpusTokenCount; i++) {
            bool found = false;
            for (int j = 0; j < allCount; j++) {
                if (corpusTokens[i] == allTokens[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                allTokens[allCount++] = corpusTokens[i];
            }
        }
        int unionSize = allCount;

        if (unionSize == 0) {
            return 0.0;
        }

        // IoU = intersection / union
        return (double)intersection / (double)unionSize;
    }
    
    //======================================================
    // GET BEST RESPONSE USING IoU
    //======================================================
    string getBestResponseFromCorpus(const string& userInput) {
        if (conversationCount == 0) {
            return "I'm not sure how to respond to that.";
        }

        double maxIoU = 0.0;
        int bestIndex = -1;

        // Find the Human 1 input with highest IoU score
        for (int i = 0; i < conversationCount; i++) {
            double iou = calculateIoU(userInput, conversations[i].human1);

            if (iou > maxIoU) {
                maxIoU = iou;
                bestIndex = i;
            }
        }

        // If we found a reasonable match (IoU > 0), return the response
        if (bestIndex != -1 && maxIoU > 0.1) {
            return conversations[bestIndex].human2;
        }

        // If no good match found, return a default response
        return "That's interesting! Tell me more.";
    }



    // NEW: Load existing application from file
   // MODIFIED: Load existing application from file
    bool loadExistingApplication(const string& appId, const string& cnic) {
        ifstream file("applications.txt");
        if (!file.is_open()) {
            return false;
        }

        string line;
        while (getline(file, line)) {
            string parts[50];
            int count = 0;
            splitString(line, '#', parts, count);

            // Check if we found the right application (appId and cnic match)
            if (count >= 37 && parts[0] == appId && parts[6] == cnic) {
                // Status is at index 36 (0-based indexing)
                string appStatus = parts[36];

                // Check if application is incomplete (C1, C2, or C3)
                if (appStatus != "C1" && appStatus != "C2" && appStatus != "C3") {
                    file.close();
                    setColor(LIGHT_RED);
                    cout << "\n  This application is already completed (Status: " << appStatus << ") and cannot be modified." << endl;
                    setColor(WHITE);
                    return false;
                }

                // Load existing data
                currentApp.applicationId = parts[0];
                currentApp.fullName = parts[1];
                currentApp.fatherName = parts[2];
                currentApp.postalAddress = parts[3];
                currentApp.contactNumber = parts[4];
                currentApp.email = parts[5];
                currentApp.cnic = parts[6];
                currentApp.cnicExpiryDate = parts[7];
                currentApp.employmentStatus = parts[8];
                currentApp.maritalStatus = parts[9];
                currentApp.gender = parts[10];
                currentApp.numberOfDependents = stringToInt(parts[11]);
                currentApp.annualIncome = stringToDouble(parts[12]);
                currentApp.avgElectricityBill = stringToDouble(parts[13]);
                currentApp.currentElectricityBill = stringToDouble(parts[14]);
                currentApp.loanInfo.hasLoan = (parts[15] == "Yes");
                currentApp.loanInfo.loanStatus = parts[16];
                currentApp.loanInfo.totalLoanAmount = stringToDouble(parts[17]);
                currentApp.loanInfo.amountReturned = stringToDouble(parts[18]);
                currentApp.loanInfo.amountDue = stringToDouble(parts[19]);
                currentApp.loanInfo.bankName = parts[20];
                currentApp.loanInfo.loanCategory = parts[21];
                currentApp.reference1.name = parts[22];
                currentApp.reference1.cnic = parts[23];
                currentApp.reference1.cnicIssueDate = parts[24];
                currentApp.reference1.phoneNumber = parts[25];
                currentApp.reference1.email = parts[26];
                currentApp.reference2.name = parts[27];
                currentApp.reference2.cnic = parts[28];
                currentApp.reference2.cnicIssueDate = parts[29];
                currentApp.reference2.phoneNumber = parts[30];
                currentApp.reference2.email = parts[31];
                currentApp.cnicFrontPath = parts[32];
                currentApp.cnicBackPath = parts[33];
                currentApp.electricityBillPath = parts[34];
                currentApp.salarySlipPath = parts[35];
                currentApp.status = parts[36];

                // Handle optional fields (loan selection might not be filled yet)
                if (count > 37) currentApp.selectedLoanType = parts[37];
                if (count > 38) currentApp.selectedLoanCategory = parts[38];
                if (count > 39) currentApp.loanAmount = stringToDouble(parts[39]);
                if (count > 40) currentApp.installmentMonths = stringToInt(parts[40]);

                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    // NEW: Delete incomplete application from file
    void deleteApplicationFromFile(const string& appId) {
        ifstream file("applications.txt");
        if (!file.is_open()) return;

        string lines[1000];
        int lineCount = 0;
        string line;

        while (getline(file, line) && lineCount < 1000) {
            string parts[50];
            int count = 0;
            splitString(line, '#', parts, count);

            if (count == 0 || parts[0] != appId) {
                lines[lineCount++] = line;
            }
        }
        file.close();

        ofstream outFile("applications.txt");
        for (int i = 0; i < lineCount; i++) {
            outFile << lines[i] << endl;
        }
        outFile.close();
    }

    // MODIFIED: Collect Personal Information (Checkpoint 1)
  // FIX 1: Update collectPersonalInformation() to properly handle status
    bool collectPersonalInformation() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "         CHECKPOINT 1: PERSONAL INFORMATION" << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(WHITE);
        cout << "\n  Please provide accurate information." << endl;
        cout << "  (Type 'X' at any time to save and exit)" << endl;

        // Check if continuing existing application
        if (currentApp.status.empty()) {
            currentApp.applicationId = generateApplicationId();
            currentApp.status = "C1"; // Initialize as C1 from start
            setColor(LIGHT_GREEN);
            cout << "\n  Your Application ID: " << currentApp.applicationId << endl;
            setColor(WHITE);
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Continuing Application ID: " << currentApp.applicationId << endl;
            setColor(WHITE);
        }

        // Full Name
        if (currentApp.fullName.empty()) {
            while (true) {
                string name = getValidInput("\n  Enter your full name (First Last): ");
                if (name == "x") {
                    saveApplicationToFile(); // Status remains C1
                    return false;
                }
                if (!isValidFullName(name)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid name! Please enter at least first and last name." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.fullName = capitalizeName(name);
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Full Name (already saved): " << currentApp.fullName << endl;
            setColor(WHITE);
        }
        // Father's Name
        if (currentApp.fatherName.empty()) {
            while (true) {
                string fatherName = getValidInput("  Enter your father's name (First Last): ");
                if (fatherName == "x") {
                    currentApp.status = "C1";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidFullName(fatherName)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid name! Please enter at least first and last name." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.fatherName = capitalizeName(fatherName);
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "  Father's Name (already saved): " << currentApp.fatherName << endl;
            setColor(WHITE);
        }

        // Postal Address
        if (currentApp.postalAddress.empty()) {
            while (true) {
                string postalAddress = getValidInput("\n  Enter your Postal Address: ");
                if (postalAddress == "x") {
                    currentApp.status = "C1";
                    saveApplicationToFile();
                    return false;
                }

                string errorMsg = isValidAddress(postalAddress);
                if (!errorMsg.empty()) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid postal address! " << errorMsg << endl;
                    setColor(WHITE);
                    continue;
                }

                currentApp.postalAddress = postalAddress;
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Postal Address (already saved): " << currentApp.postalAddress << endl;
            setColor(WHITE);
        }

        // Contact Number
        if (currentApp.contactNumber.empty()) {
            while (true) {
                string phone = getValidInput("  Enter your contact number (03XXXXXXXXX): ");
                if (phone == "x") {
                    currentApp.status = "C1";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidPhone(phone)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid phone! Must be 10-11 digits." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.contactNumber = phone;
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "  Contact Number (already saved): " << currentApp.contactNumber << endl;
            setColor(WHITE);
        }

        // Email
        if (currentApp.email.empty()) {
            while (true) {
                string email = getValidInput("\n  Enter your Email Address: ");
                if (email == "x") {
                    currentApp.status = "C1";
                    saveApplicationToFile();
                    return false;
                }

                string errorMsg = isValidEmail(email);
                if (!errorMsg.empty()) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid email! " << errorMsg << endl;
                    setColor(WHITE);
                    continue;
                }

                currentApp.email = email;
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Email Address (already saved): " << currentApp.email << endl;
            setColor(WHITE);
        }

        // CNIC
        if (currentApp.cnic.empty()) {
            while (true) {
                string cnic = getValidInput("  Enter your CNIC (13 digits, no dashes): ");
                if (cnic == "x") {
                    currentApp.status = "C1";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidCNIC(cnic)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid CNIC! Must be exactly 13 digits." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.cnic = cnic;
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "  CNIC (already saved): " << currentApp.cnic << endl;
            setColor(WHITE);
        }

        // CNIC Expiry Date
        if (currentApp.cnicExpiryDate.empty()) {
            while (true) {
                string expiryDate = getValidInput("\n  Enter CNIC Expiry Date (DD-MM-YYYY): ");
                if (expiryDate == "x") {
                    currentApp.status = "C1";
                    saveApplicationToFile();
                    return false;
                }

                string errorMsg = getCNICExpiryValidationError(expiryDate);
                if (!errorMsg.empty()) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid CNIC expiry date! " << errorMsg << endl;
                    setColor(WHITE);
                    continue;
                }

                currentApp.cnicExpiryDate = expiryDate;
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  CNIC Expiry Date (already saved): " << currentApp.cnicExpiryDate << endl;
            setColor(WHITE);
        }

        // Employment Status
        if (currentApp.employmentStatus.empty()) {
            setColor(LIGHT_YELLOW);
            cout << "\n  Select employment status:" << endl;
            setColor(WHITE);
            cout << "    1. Self-employed" << endl;
            cout << "    2. Salaried" << endl;
            cout << "    3. Retired" << endl;
            cout << "    4. Unemployed" << endl;
            int emp = getValidNumberInput("  Enter choice (1-4): ", 1, 4);
            if (emp == 1) currentApp.employmentStatus = "Self-employed";
            else if (emp == 2) currentApp.employmentStatus = "Salaried";
            else if (emp == 3) currentApp.employmentStatus = "Retired";
            else currentApp.employmentStatus = "Unemployed";
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Employment Status (already saved): " << currentApp.employmentStatus << endl;
            setColor(WHITE);
        }

        // Marital Status
        if (currentApp.maritalStatus.empty()) {
            setColor(LIGHT_YELLOW);
            cout << "\n  Select marital status:" << endl;
            setColor(WHITE);
            cout << "    1. Single" << endl;
            cout << "    2. Married" << endl;
            cout << "    3. Divorced" << endl;
            cout << "    4. Widowed" << endl;
            int marital = getValidNumberInput("  Enter choice (1-4): ", 1, 4);
            if (marital == 1) currentApp.maritalStatus = "Single";
            else if (marital == 2) currentApp.maritalStatus = "Married";
            else if (marital == 3) currentApp.maritalStatus = "Divorced";
            else currentApp.maritalStatus = "Widowed";
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Marital Status (already saved): " << currentApp.maritalStatus << endl;
            setColor(WHITE);
        }

        // Gender
        if (currentApp.gender.empty()) {
            setColor(LIGHT_YELLOW);
            cout << "\n  Select gender:" << endl;
            setColor(WHITE);
            cout << "    1. Male" << endl;
            cout << "    2. Female" << endl;
            cout << "    3. Other" << endl;
            int gender = getValidNumberInput("  Enter choice (1-3): ", 1, 3);
            if (gender == 1) currentApp.gender = "Male";
            else if (gender == 2) currentApp.gender = "Female";
            else currentApp.gender = "Other";
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Gender (already saved): " << currentApp.gender << endl;
            setColor(WHITE);
        }
        // Number of Dependents
        if (currentApp.numberOfDependents == -1) {
            currentApp.numberOfDependents = getValidNumberInput("\n  Number of dependents (0-20): ", 0, 20);
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Number of Dependents (already saved): " << currentApp.numberOfDependents << endl;
            setColor(WHITE);
        }

        // Checkpoint 1 FULLY Complete - NOW mark as C2
        currentApp.status = "C2";
        saveApplicationToFile();

        setColor(LIGHT_GREEN);
        cout << "\n  ========================================================" << endl;
        cout << "     CHECKPOINT 1 COMPLETED & SAVED!" << endl;
        cout << "  ========================================================" << endl;
        setColor(WHITE);
        Sleep(1500);

        return true;
    }



    // MODIFIED: Collect Financial Information (Checkpoint 2)
    bool collectFinancialInformation() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "         CHECKPOINT 2: FINANCIAL INFORMATION" << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(WHITE);
        cout << "\n  (Type 'X' at any time to save and exit)" << endl;

        // Annual Income
        if (currentApp.annualIncome == 0) {
            while (true) {
                string income = getValidInput("\n  Enter annual income in PKR: ");
                if (income == "x") {
                    saveApplicationToFile(); // Status stays C2
                    return false;
                }
                if (!isValidAmount(income)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid! Enter numbers only, no commas or symbols." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.annualIncome = stringToDouble(income);
                if (currentApp.annualIncome < 100000) {
                    setColor(LIGHT_RED);
                    cout << "  Income too low for loan eligibility (minimum PKR 100,000)." << endl;
                    setColor(WHITE);
                    continue;
                }
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Annual Income (already saved): Rs. " << formatNumber(currentApp.annualIncome) << endl;
            setColor(WHITE);
        }

        // Average Electricity Bill
        if (currentApp.avgElectricityBill == 0) {
            while (true) {
                string avgBill = getValidInput("  Monthly average electricity bill for last 12 months (PKR, no commas): ");
                if (avgBill == "x") {
                    currentApp.status = "C2";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidAmount(avgBill)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid! Enter numbers only." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.avgElectricityBill = stringToDouble(avgBill);
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "  Avg Electricity Bill (already saved): Rs. " << formatNumber(currentApp.avgElectricityBill) << endl;
            setColor(WHITE);
        }

        // Current Electricity Bill
        if (currentApp.currentElectricityBill == 0) {
            while (true) {
                string currentBill = getValidInput("  Current electricity bill amount (PKR, no commas): ");
                if (currentBill == "x") {
                    currentApp.status = "C2";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidAmount(currentBill)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid! Enter numbers only." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.currentElectricityBill = stringToDouble(currentBill);
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "  Current Electricity Bill (already saved): Rs. " << formatNumber(currentApp.currentElectricityBill) << endl;
            setColor(WHITE);
        }

        // Existing Loans
        if (currentApp.loanInfo.bankName.empty() || currentApp.loanInfo.bankName == "N/A") {
            setColor(LIGHT_CYAN);
            cout << "\n  ========================================================" << endl;
            setColor(LIGHT_YELLOW);
            cout << "                  EXISTING LOANS INFORMATION" << endl;
            setColor(LIGHT_CYAN);
            cout << "  ========================================================" << endl;
            setColor(LIGHT_YELLOW);
            cout << "\n  Do you have any existing loans? (Y/N): ";
            setColor(BRIGHT_WHITE);
            string hasLoans;
            getline(cin, hasLoans);
            hasLoans = trim(hasLoans);

            if (toLower(hasLoans) == "x") {
                currentApp.status = "C2";
                saveApplicationToFile();
                return false;
            }

            if (toLower(hasLoans) == "y" || toLower(hasLoans) == "yes") {
                currentApp.loanInfo.hasLoan = true;

                setColor(LIGHT_YELLOW);
                cout << "\n  Is the loan currently active? (Y/N): ";
                setColor(BRIGHT_WHITE);
                string isActive;
                getline(cin, isActive);

                if (toLower(trim(isActive)) == "x") {
                    currentApp.status = "C2";
                    saveApplicationToFile();
                    return false;
                }

                currentApp.loanInfo.loanStatus = (toLower(trim(isActive)) == "y" || toLower(trim(isActive)) == "yes") ? "Active" : "Inactive";

                while (true) {
                    string totalLoan = getValidInput("  Total loan amount (PKR, no commas): ");
                    if (totalLoan == "x") {
                        currentApp.status = "C2";
                        saveApplicationToFile();
                        return false;
                    }
                    if (!isValidAmount(totalLoan)) {
                        setColor(LIGHT_RED);
                        cout << "  Invalid! Enter numbers only." << endl;
                        setColor(WHITE);
                        continue;
                    }
                    currentApp.loanInfo.totalLoanAmount = stringToDouble(totalLoan);
                    break;
                }

                while (true) {
                    string returned = getValidInput("  Amount already returned (PKR, no commas): ");
                    if (returned == "x") {
                        currentApp.status = "C2";
                        saveApplicationToFile();
                        return false;
                    }
                    if (!isValidAmount(returned)) {
                        setColor(LIGHT_RED);
                        cout << "  Invalid! Enter numbers only." << endl;
                        setColor(WHITE);
                        continue;
                    }
                    currentApp.loanInfo.amountReturned = stringToDouble(returned);
                    if (currentApp.loanInfo.amountReturned > currentApp.loanInfo.totalLoanAmount) {
                        setColor(LIGHT_RED);
                        cout << "  Returned amount cannot exceed total loan!" << endl;
                        setColor(WHITE);
                        continue;
                    }
                    break;
                }

                currentApp.loanInfo.amountDue = currentApp.loanInfo.totalLoanAmount - currentApp.loanInfo.amountReturned;

                currentApp.loanInfo.bankName = getValidInput("  Bank name: ");
                if (currentApp.loanInfo.bankName == "x") {
                    currentApp.loanInfo.bankName = ""; // Reset
                    currentApp.status = "C2";
                    saveApplicationToFile();
                    return false;
                }

                setColor(LIGHT_YELLOW);
                cout << "\n  Loan category:" << endl;
                setColor(WHITE);
                cout << "    1. Car" << endl;
                cout << "    2. Home" << endl;
                cout << "    3. Bike" << endl;
                int loanCat = getValidNumberInput("  Enter choice (1-3): ", 1, 3);
                if (loanCat == 1) currentApp.loanInfo.loanCategory = "Car";
                else if (loanCat == 2) currentApp.loanInfo.loanCategory = "Home";
                else currentApp.loanInfo.loanCategory = "Bike";
            }
            else {
                currentApp.loanInfo.hasLoan = false;
                currentApp.loanInfo.loanStatus = "None";
                currentApp.loanInfo.totalLoanAmount = 0;
                currentApp.loanInfo.amountReturned = 0;
                currentApp.loanInfo.amountDue = 0;
                currentApp.loanInfo.bankName = "N/A";
                currentApp.loanInfo.loanCategory = "N/A";
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Existing Loan Information (already saved)" << endl;
            setColor(WHITE);
        }

        // Checkpoint 2 Complete - NOW mark as ready for C3
        currentApp.status = "C3"; // CHANGED: Mark as ready for C3
        saveApplicationToFile();

        setColor(LIGHT_GREEN);
        cout << "\n  ========================================================" << endl;
        cout << "     CHECKPOINT 2 COMPLETED & SAVED!" << endl;
        cout << "  ========================================================" << endl;
        setColor(WHITE);
        Sleep(1500);

        return true;
    }

    // MODIFIED: Collect References (Checkpoint 3)
    bool collectReferences() {
        if (!currentApp.reference1.name.empty() && !currentApp.reference2.name.empty()) {
            setColor(LIGHT_GREEN);
            cout << "\n  References already collected. Skipping to documents..." << endl;
            setColor(WHITE);
            Sleep(1500);
            return true; // Skip this checkpoint
        }

        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "         CHECKPOINT 3: REFERENCES" << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(WHITE);
        cout << "\n  (Type 'X' at any time to save and exit)" << endl;

        // Reference 1
        if (currentApp.reference1.name.empty()) {
            setColor(LIGHT_CYAN);
            cout << "\n  ========================================================" << endl;
            setColor(LIGHT_YELLOW);
            cout << "                    REFERENCE 1 DETAILS" << endl;
            setColor(LIGHT_CYAN);
            cout << "  ========================================================" << endl;
            setColor(WHITE);

            while (true) {
                string refName = getValidInput("\n  Full name: ");
                if (refName == "x") {
                    currentApp.status = "C3"; // Still at C3
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidFullName(refName)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid name! Enter at least first and last name." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.reference1.name = capitalizeName(refName);
                break;
            }

            while (true) {
                string refCNIC = getValidInput("  CNIC (13 digits, no dashes): ");
                if (refCNIC == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidCNIC(refCNIC)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid CNIC! Must be 13 digits." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.reference1.cnic = refCNIC;
                break;
            }

            while (true) {
                string issueDate = getValidInput("\n  CNIC Issue Date (DD-MM-YYYY): ");
                if (issueDate == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }

                string errorMsg = getCNICIssueValidationError(issueDate);
                if (!errorMsg.empty()) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid CNIC issue date! " << errorMsg << endl;
                    setColor(WHITE);
                    continue;
                }

                currentApp.reference1.cnicIssueDate = issueDate;
                break;
            }

            while (true) {
                string refPhone = getValidInput("  Phone number: ");
                if (refPhone == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidPhone(refPhone)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid phone! Must be 10-11 digits." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.reference1.phoneNumber = refPhone;
                break;
            }

            while (true) {
                string email = getValidInput("\n  Email Address: ");
                if (email == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }

                string errorMsg = isValidEmail(email);
                if (!errorMsg.empty()) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid email! " << errorMsg << endl;
                    setColor(WHITE);
                    continue;
                }

                currentApp.reference1.email = email;
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Reference 1 (already saved): " << currentApp.reference1.name << endl;
            setColor(WHITE);
        }

        // Reference 2
        if (currentApp.reference2.name.empty()) {
            setColor(LIGHT_CYAN);
            cout << "\n  ========================================================" << endl;
            setColor(LIGHT_YELLOW);
            cout << "                    REFERENCE 2 DETAILS" << endl;
            setColor(LIGHT_CYAN);
            cout << "  ========================================================" << endl;
            setColor(WHITE);

            while (true) {
                string refName = getValidInput("\n  Full name: ");
                if (refName == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidFullName(refName)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid name! Enter at least first and last name." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.reference2.name = capitalizeName(refName);
                break;
            }

            while (true) {
                string refCNIC = getValidInput("  CNIC (13 digits, no dashes): ");
                if (refCNIC == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidCNIC(refCNIC)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid CNIC! Must be 13 digits." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.reference2.cnic = refCNIC;
                break;
            }

            while (true) {
                string issueDate = getValidInput("\n  CNIC Issue Date (DD-MM-YYYY): ");
                if (issueDate == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }

                string errorMsg = getCNICIssueValidationError(issueDate);
                if (!errorMsg.empty()) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid CNIC issue date! " << errorMsg << endl;
                    setColor(WHITE);
                    continue;
                }

                currentApp.reference2.cnicIssueDate = issueDate;
                break;
            }

            while (true) {
                string refPhone = getValidInput("  Phone number: ");
                if (refPhone == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }
                if (!isValidPhone(refPhone)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid phone! Must be 10-11 digits." << endl;
                    setColor(WHITE);
                    continue;
                }
                currentApp.reference2.phoneNumber = refPhone;
                break;
            }

            while (true) {
                string email = getValidInput("\n  Email Address: ");
                if (email == "x") {
                    currentApp.status = "C3";
                    saveApplicationToFile();
                    return false;
                }

                string errorMsg = isValidEmail(email);
                if (!errorMsg.empty()) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid email! " << errorMsg << endl;
                    setColor(WHITE);
                    continue;
                }

                currentApp.reference2.email = email;
                break;
            }
        }
        else {
            setColor(LIGHT_GREEN);
            cout << "\n  Reference 2 (already saved): " << currentApp.reference2.name << endl;
            setColor(WHITE);
        }

        // Checkpoint 3 Complete - Mark as completed (no more checkpoints, ready for documents)
        saveApplicationToFile();

        setColor(LIGHT_GREEN);
        cout << "\n  ========================================================" << endl;
        cout << "     CHECKPOINT 3 COMPLETED & SAVED!" << endl;
        cout << "  ========================================================" << endl;
        setColor(WHITE);
        system("cls");
        Sleep(1500);

        return true;
    }

    // NEW: Collect Documents
    bool collectDocuments() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "         DOCUMENT UPLOAD" << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(WHITE);
        cout << "\n  Images will be saved in ./data/" << currentApp.applicationId << "/" << endl;

        // Create directory
        string dataDir = "./data/" + currentApp.applicationId;
        _mkdir("./data");
        _mkdir(dataDir.c_str());

        if (currentApp.cnicFrontPath.empty()) {
            currentApp.cnicFrontPath = dataDir + "/cnic_front.jpg";
        }
        if (currentApp.cnicBackPath.empty()) {
            currentApp.cnicBackPath = dataDir + "/cnic_back.jpg";
        }
        if (currentApp.electricityBillPath.empty()) {
            currentApp.electricityBillPath = dataDir + "/electricity_bill.jpg";
        }
        if (currentApp.salarySlipPath.empty()) {
            currentApp.salarySlipPath = dataDir + "/salary_slip.jpg";
        }

        setColor(LIGHT_GREEN);
        cout << "\n  Please place CNIC front image at: " << currentApp.cnicFrontPath << endl;
        cout << "  Please place CNIC back image at: " << currentApp.cnicBackPath << endl;
        cout << "  Please place electricity bill image at: " << currentApp.electricityBillPath << endl;
        cout << "  Please place salary slip/bank statement at: " << currentApp.salarySlipPath << endl;
        setColor(WHITE);

        setColor(LIGHT_MAGENTA);
        cout << "\n  Press Enter when all documents are ready...";
        setColor(BRIGHT_WHITE);
        cin.get();

        // ADD THIS NEW CODE HERE:
        setColor(LIGHT_YELLOW);
        cout << "\n  Have you placed all documents? (Y/N or X to save and exit): ";
        setColor(BRIGHT_WHITE);
        string docsReady;
        getline(cin, docsReady);
        docsReady = trim(docsReady);

        if (toLower(docsReady) == "x") {
            currentApp.status = "C3"; // Documents not completed, stay at C3
            saveApplicationToFile();
            system("cls"); // Clear screen
            setColor(LIGHT_YELLOW);
            cout << "\n  Application saved. Documents still pending." << endl;
            cout << "  You can resume later using 'resume' command." << endl;
            setColor(WHITE);
            Sleep(2000);
            return false;
        }

        if (toLower(docsReady) != "y" && toLower(docsReady) != "yes") {
            setColor(LIGHT_RED);
            cout << "\n  Please place all documents before proceeding." << endl;
            setColor(WHITE);
            Sleep(1500);
            return false;
        }

        // Documents confirmed - ready for submission
        currentApp.status = "DOCS_READY"; // New status indicating documents are collected
        saveApplicationToFile();

        return true;
    }

    void displayApplicationSummary() {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "              APPLICATION SUMMARY" << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(WHITE);

        setColor(LIGHT_GREEN);
        cout << "\n  Application ID: ";
        setColor(BRIGHT_WHITE);
        cout << currentApp.applicationId << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  Personal Information:" << endl;
        setColor(WHITE);
        cout << "    Full Name: " << currentApp.fullName << endl;
        cout << "    Father's Name: " << currentApp.fatherName << endl;
        cout << "    Postal Address: " << currentApp.postalAddress << endl;
        cout << "    Contact Number: " << currentApp.contactNumber << endl;
        cout << "    Email: " << currentApp.email << endl;
        cout << "    CNIC: " << currentApp.cnic << endl;
        cout << "    CNIC Expiry: " << currentApp.cnicExpiryDate << endl;
        cout << "    Employment Status: " << currentApp.employmentStatus << endl;
        cout << "    Marital Status: " << currentApp.maritalStatus << endl;
        cout << "    Gender: " << currentApp.gender << endl;
        cout << "    Dependents: " << currentApp.numberOfDependents << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  Financial Information:" << endl;
        setColor(WHITE);
        cout << "    Annual Income: Rs. " << formatNumber(currentApp.annualIncome) << endl;
        cout << "    Avg. Electricity Bill: Rs. " << formatNumber(currentApp.avgElectricityBill) << endl;
        cout << "    Current Bill: Rs. " << formatNumber(currentApp.currentElectricityBill) << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  Existing Loan Information:" << endl;
        setColor(WHITE);
        if (currentApp.loanInfo.hasLoan) {
            cout << "    Status: " << currentApp.loanInfo.loanStatus << endl;
            cout << "    Total Amount: Rs. " << formatNumber(currentApp.loanInfo.totalLoanAmount) << endl;
            cout << "    Amount Returned: Rs. " << formatNumber(currentApp.loanInfo.amountReturned) << endl;
            cout << "    Amount Due: Rs. " << formatNumber(currentApp.loanInfo.amountDue) << endl;
            cout << "    Bank: " << currentApp.loanInfo.bankName << endl;
            cout << "    Category: " << currentApp.loanInfo.loanCategory << endl;
        }
        else {
            cout << "    No existing loans" << endl;
        }

        setColor(LIGHT_GREEN);
        cout << "\n  References:" << endl;
        setColor(WHITE);
        cout << "    Reference 1: " << currentApp.reference1.name << endl;
        cout << "      CNIC: " << currentApp.reference1.cnic << endl;
        cout << "      Issue Date: " << currentApp.reference1.cnicIssueDate << endl;
        cout << "      Phone: " << currentApp.reference1.phoneNumber << endl;
        cout << "      Email: " << currentApp.reference1.email << endl;
        cout << "\n    Reference 2: " << currentApp.reference2.name << endl;
        cout << "      CNIC: " << currentApp.reference2.cnic << endl;
        cout << "      Issue Date: " << currentApp.reference2.cnicIssueDate << endl;
        cout << "      Phone: " << currentApp.reference2.phoneNumber << endl;
        cout << "      Email: " << currentApp.reference2.email << endl;

        setColor(LIGHT_GREEN);
        cout << "\n  Document Paths:" << endl;
        setColor(WHITE);
        cout << "    CNIC Front: " << currentApp.cnicFrontPath << endl;
        cout << "    CNIC Back: " << currentApp.cnicBackPath << endl;
        cout << "    Electricity Bill: " << currentApp.electricityBillPath << endl;
        cout << "    Salary Slip: " << currentApp.salarySlipPath << endl;
    }

    bool saveApplicationToFile() {
        // First delete any existing incomplete application with same ID
        deleteApplicationFromFile(currentApp.applicationId);

        // Then append the new/updated application
        ofstream file("applications.txt", ios::app);
        if (!file.is_open()) {
            setColor(LIGHT_RED);
            cout << "\n  Error: Could not save application!" << endl;
            setColor(WHITE);
            return false;
        }

        // Save in order: 41 fields total (indices 0-40)
        file << currentApp.applicationId << "#"              // 0
            << currentApp.fullName << "#"                    // 1
            << currentApp.fatherName << "#"                  // 2
            << currentApp.postalAddress << "#"               // 3
            << currentApp.contactNumber << "#"               // 4
            << currentApp.email << "#"                       // 5
            << currentApp.cnic << "#"                        // 6
            << currentApp.cnicExpiryDate << "#"              // 7
            << currentApp.employmentStatus << "#"            // 8
            << currentApp.maritalStatus << "#"               // 9
            << currentApp.gender << "#"                      // 10
            << currentApp.numberOfDependents << "#"          // 11
            << currentApp.annualIncome << "#"                // 12
            << currentApp.avgElectricityBill << "#"          // 13
            << currentApp.currentElectricityBill << "#"      // 14
            << (currentApp.loanInfo.hasLoan ? "Yes" : "No") << "#"  // 15
            << currentApp.loanInfo.loanStatus << "#"         // 16
            << currentApp.loanInfo.totalLoanAmount << "#"    // 17
            << currentApp.loanInfo.amountReturned << "#"     // 18
            << currentApp.loanInfo.amountDue << "#"          // 19
            << currentApp.loanInfo.bankName << "#"           // 20
            << currentApp.loanInfo.loanCategory << "#"       // 21
            << currentApp.reference1.name << "#"             // 22
            << currentApp.reference1.cnic << "#"             // 23
            << currentApp.reference1.cnicIssueDate << "#"    // 24
            << currentApp.reference1.phoneNumber << "#"      // 25
            << currentApp.reference1.email << "#"            // 26
            << currentApp.reference2.name << "#"             // 27
            << currentApp.reference2.cnic << "#"             // 28
            << currentApp.reference2.cnicIssueDate << "#"    // 29
            << currentApp.reference2.phoneNumber << "#"      // 30
            << currentApp.reference2.email << "#"            // 31
            << currentApp.cnicFrontPath << "#"               // 32
            << currentApp.cnicBackPath << "#"                // 33
            << currentApp.electricityBillPath << "#"         // 34
            << currentApp.salarySlipPath << "#"              // 35
            << currentApp.status << "#"                      // 36 (THIS IS THE STATUS!)
            << currentApp.selectedLoanType << "#"            // 37
            << currentApp.selectedLoanCategory << "#"        // 38
            << currentApp.loanAmount << "#"                  // 39
            << currentApp.installmentMonths << endl;         // 40

        file.close();
        return true;
    }

    void checkApplicationStatus() {
        string cnic = getValidInput("\n  Enter CNIC to check (13 digits, no dashes): ", false);
        if (!isValidCNIC(cnic)) {
            setColor(LIGHT_RED);
            cout << "  Invalid CNIC format!" << endl;
            setColor(WHITE);
            return;
        }

        ifstream file("applications.txt");
        if (!file.is_open()) {
            setColor(LIGHT_YELLOW);
            cout << "\n  No applications found." << endl;
            setColor(WHITE);
            return;
        }
        int submitted = 0, approved = 0, rejected = 0, incomplete = 0;
        string line;

        while (getline(file, line)) {
            string parts[50];
            int count = 0;
            splitString(line, '#', parts, count);

            if (count > 6 && parts[6] == cnic) {
                submitted++;
                if (count > 36) {
                    if (parts[36] == "Approved") approved++;
                    else if (parts[36] == "Rejected") rejected++;
                    else if (parts[36] == "C1" || parts[36] == "C2" || parts[36] == "C3") incomplete++;
                }
            }
        }

        file.close();

        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "              APPLICATION STATUS FOR CNIC: " << cnic << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(LIGHT_GREEN);
        cout << "\n  Total Submitted: ";
        setColor(BRIGHT_WHITE);
        cout << submitted << endl;
        setColor(LIGHT_GREEN);
        cout << "  Approved: ";
        setColor(BRIGHT_WHITE);
        cout << approved << endl;
        setColor(LIGHT_GREEN);
        cout << "  Rejected: ";
        setColor(BRIGHT_WHITE);
        cout << rejected << endl;
        setColor(LIGHT_GREEN);
        cout << "  Incomplete: ";
        setColor(BRIGHT_WHITE);
        cout << incomplete << endl;
        setColor(LIGHT_GREEN);
        cout << "  Pending: ";
        setColor(BRIGHT_WHITE);
        cout << (submitted - approved - rejected - incomplete) << endl;
        setColor(WHITE);
    }

    void generateMonthlyPlanWithMonths(double loanAmount, int installments, int startMonth) {
        system("cls");
        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "           APPROVED LOAN - MONTHLY PAYMENT PLAN" << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(WHITE);

        double monthlyPayment = loanAmount / installments;
        double remaining = loanAmount;

        setColor(LIGHT_GREEN);
        cout << "\n  Loan Amount: Rs. " << formatNumber(loanAmount) << endl;
        cout << "  Number of Installments: " << installments << " months" << endl;
        cout << "  Monthly Payment: Rs. " << formatNumber(monthlyPayment) << endl;
        setColor(WHITE);

        setColor(LIGHT_BLUE);
        cout << "\n  --------------------------------------------------------" << endl;
        setColor(LIGHT_YELLOW);
        cout << "  Month         Payment Due       Total Paid      Remaining" << endl;
        setColor(LIGHT_BLUE);
        cout << "  --------------------------------------------------------" << endl;
        setColor(WHITE);

        int currentMonth = startMonth;
        int currentYear = 2025;
        double totalPaid = 0;

        for (int i = 1; i <= installments; i++) {
            string monthName = getMonthName(currentMonth);

            cout << "  " << monthName << " " << currentYear << "  ";
            setColor(LIGHT_GREEN);
            cout << "Rs. " << setw(12) << left << formatNumber(monthlyPayment);
            setColor(WHITE);

            totalPaid += monthlyPayment;
            remaining -= monthlyPayment;
            if (remaining < 0.01) remaining = 0;

            setColor(LIGHT_CYAN);
            cout << "Rs. " << setw(12) << left << formatNumber(totalPaid);
            setColor(LIGHT_MAGENTA);
            cout << "Rs. " << formatNumber(remaining) << endl;
            setColor(WHITE);

            currentMonth++;
            if (currentMonth > 12) {
                currentMonth = 1;
                currentYear++;
            }
        }

        setColor(LIGHT_BLUE);
        cout << "  --------------------------------------------------------" << endl;
        setColor(WHITE);
    }

    int displayLoanOptions(LoanOption* options, int count, const string& category, const string& loanType) {
        setColor(LIGHT_CYAN);
        cout << "\n  ========================================================" << endl;
        setColor(LIGHT_YELLOW);
        cout << "          " << loanType << " Loan Options - " << category << endl;
        setColor(LIGHT_CYAN);
        cout << "  ========================================================" << endl;
        setColor(WHITE);

        int optionNum = 0;
        for (int i = 0; i < count; i++) {
            if (toLower(options[i].category) == toLower(category)) {
                optionNum++;
                double price = stringToDouble(options[i].price);
                double downPayment = stringToDouble(options[i].downPayment);

                setColor(LIGHT_YELLOW);
                cout << "\n  Option " << optionNum << ":" << endl;
                setColor(LIGHT_GREEN);
                cout << "    Category: ";
                setColor(BRIGHT_WHITE);
                cout << options[i].category << endl;
                setColor(LIGHT_GREEN);
                cout << "    Details: ";
                setColor(BRIGHT_WHITE);
                cout << options[i].details << endl;
                setColor(LIGHT_GREEN);
                cout << "    Price: ";
                setColor(LIGHT_CYAN);
                cout << "Rs. " << formatNumber(price) << endl;
                setColor(LIGHT_GREEN);
                cout << "    Down Payment: ";
                setColor(LIGHT_CYAN);
                cout << "Rs. " << formatNumber(downPayment) << endl;
                setColor(LIGHT_GREEN);
                cout << "    Installments: ";
                setColor(BRIGHT_WHITE);
                cout << options[i].installments << " months" << endl;
                setColor(LIGHT_BLUE);
                cout << "  --------------------------------------------------------" << endl;
                setColor(WHITE);
            }
        }

        if (optionNum == 0) {
            setColor(LIGHT_RED);
            cout << "  No loan options available." << endl;
            setColor(WHITE);
        }

        return optionNum;
    }

    void handleLoanSelection(LoanOption* options, int count, const string& loanType) {
        string categories[100];
        int categoryCount = 0;

        for (int i = 0; i < count; i++) {
            bool found = false;
            for (int j = 0; j < categoryCount; j++) {
                if (categories[j] == options[i].category) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                categories[categoryCount++] = options[i].category;
            }
        }

        if (categoryCount == 0) {
            setColor(LIGHT_RED);
            cout << "\n  No " << loanType << " loan options available." << endl;
            setColor(WHITE);
            return;
        }

        setColor(LIGHT_CYAN);
        cout << "\n  Available " << loanType << " Categories:" << endl;
        setColor(WHITE);
        for (int i = 0; i < categoryCount; i++) {
            setColor(LIGHT_GREEN);
            cout << "    " << (i + 1) << ". " << categories[i] << endl;
        }
        setColor(WHITE);

        int selection = getValidNumberInput("\n  Select category (1-" + intToString(categoryCount) + "): ", 1, categoryCount);
        string selectedCategory = categories[selection - 1];
        int displayedCount = displayLoanOptions(options, count, selectedCategory, loanType);

        if (displayedCount == 0) return;

        int optionSel = getValidNumberInput("\n  Select loan option (1-" + intToString(displayedCount) + "): ", 1, displayedCount);

        // Find selected loan
        int currentOption = 0;
        LoanOption selectedLoan;
        for (int i = 0; i < count; i++) {
            if (toLower(options[i].category) == toLower(selectedCategory)) {
                currentOption++;
                if (currentOption == optionSel) {
                    selectedLoan = options[i];
                    break;
                }
            }
        }

        double price = stringToDouble(selectedLoan.price);
        double downPayment = stringToDouble(selectedLoan.downPayment);
        double loanAmount = price - downPayment;
        int installments = stringToInt(selectedLoan.installments);

        currentApp.selectedLoanType = loanType;
        currentApp.selectedLoanCategory = selectedCategory;
        currentApp.loanAmount = loanAmount;
        currentApp.installmentMonths = installments;

        setColor(LIGHT_GREEN);
        cout << "\n  Loan amount: Rs. " << formatNumber(loanAmount) << endl;
        cout << "  Installments: " << installments << " months" << endl;
        cout << "  Monthly payment: Rs. " << formatNumber(loanAmount / installments) << endl;
        setColor(WHITE);
    }

    // NEW: Initialize empty application
    void initializeEmptyApplication() {
        currentApp.applicationId = "";
        currentApp.fullName = "";
        currentApp.fatherName = "";
        currentApp.postalAddress = "";
        currentApp.contactNumber = "";
        currentApp.email = "";
        currentApp.cnic = "";
        currentApp.cnicExpiryDate = "";
        currentApp.employmentStatus = "";
        currentApp.maritalStatus = "";
        currentApp.gender = "";
        currentApp.numberOfDependents = -1;
        currentApp.annualIncome = 0;
        currentApp.avgElectricityBill = 0;
        currentApp.currentElectricityBill = 0;
        currentApp.loanInfo.hasLoan = false;
        currentApp.loanInfo.loanStatus = "";
        currentApp.loanInfo.totalLoanAmount = 0;
        currentApp.loanInfo.amountReturned = 0;
        currentApp.loanInfo.amountDue = 0;
        currentApp.loanInfo.bankName = "";
        currentApp.loanInfo.loanCategory = "";
        currentApp.reference1.name = "";
        currentApp.reference1.cnic = "";
        currentApp.reference1.cnicIssueDate = "";
        currentApp.reference1.phoneNumber = "";
        currentApp.reference1.email = "";
        currentApp.reference2.name = "";
        currentApp.reference2.cnic = "";
        currentApp.reference2.cnicIssueDate = "";
        currentApp.reference2.phoneNumber = "";
        currentApp.reference2.email = "";
        currentApp.cnicFrontPath = "";
        currentApp.cnicBackPath = "";
        currentApp.electricityBillPath = "";
        currentApp.salarySlipPath = "";
        currentApp.status = "";
        currentApp.selectedLoanType = "";
        currentApp.selectedLoanCategory = "";
        currentApp.loanAmount = 0;
        currentApp.installmentMonths = 0;
    }
public:
    LoanApplicationSystem() {
        utteranceCapacity = 10;
        utteranceCount = 0;
        utterances = new Utterance[utteranceCapacity];

        conversationCapacity = 5000;
        conversationCount = 0;
        conversations = new ConversationPair[conversationCapacity];

        homeCapacity = 10;
        homeCount = 0;
        homeLoanOptions = new LoanOption[homeCapacity];

        carCapacity = 10;
        carCount = 0;
        carLoanOptions = new LoanOption[carCapacity];

        bikeCapacity = 10;
        bikeCount = 0;
        bikeLoanOptions = new LoanOption[bikeCapacity];

        defaultResponse = "";
        chatbotName = "LOAN-BUDDY";

        initializeEmptyApplication();
    }

    ~LoanApplicationSystem() {
        delete[] utterances;
        delete[] conversations;
        delete[] homeLoanOptions;
        delete[] carLoanOptions;
        delete[] bikeLoanOptions;
    }

    bool loadUtterances(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            setColor(LIGHT_RED);
            cerr << "Error: Could not open " << filename << endl;
            setColor(WHITE);
            return false;
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find('#');
            if (pos != string::npos) {
                string input = trim(line.substr(0, pos));
                string response = trim(line.substr(pos + 1));

                if (input == "*") {
                    defaultResponse = response;
                }
                else {
                    if (utteranceCount >= utteranceCapacity) {
                        resizeUtterances();
                    }
                    utterances[utteranceCount].input = toLower(input);
                    utterances[utteranceCount].response = response;
                    utteranceCount++;
                }
            }
        }
        file.close();
        return true;
    }

    bool loadLoanData(const string& filename, LoanOption*& options, int& count, int& capacity) {
        ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        string line;
        bool firstLine = true;

        while (getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            if (line.empty() || trim(line).empty()) continue;

            if (count >= capacity) {
                resizeLoanArray(options, capacity);
            }

            string parts[5];
            int partCount = 0;
            splitString(line, '#', parts, partCount);

            if (partCount >= 5) {
                options[count].category = trim(parts[0]);
                options[count].details = trim(parts[1]);
                options[count].installments = trim(parts[2]);
                options[count].price = trim(parts[3]);
                options[count].downPayment = trim(parts[4]);
                count++;
            }
        }
        file.close();
        return true;
    }
    //======================================================
    // LOAD HUMAN CHAT CORPUS
    //======================================================
    bool loadHumanChatCorpus(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            setColor(LIGHT_RED);
            cerr << "Error: Could not open " << filename << endl;
            setColor(WHITE);
            return false;
        }

        conversationCapacity = 5000;
        conversationCount = 0;
        conversations = new ConversationPair[conversationCapacity];

        string line;
        string currentHuman1 = "";

        while (getline(file, line)) {
            line = trim(line);

            if (line.empty()) {
                continue;
            }

            // Check if it's Human 1 (question/input)
            if (line.find("Human 1:") == 0) {
                currentHuman1 = line.substr(9); // Remove "Human 1: "
                currentHuman1 = trim(currentHuman1);
            }
            // Check if it's Human 2 (response)
            else if (line.find("Human 2:") == 0 && !currentHuman1.empty()) {
                string human2 = line.substr(9); // Remove "Human 2: "
                human2 = trim(human2);

                if (!human2.empty()) {
                    if (conversationCount >= conversationCapacity) {
                        // Resize if needed
                        conversationCapacity *= 2;
                        ConversationPair* newConversations = new ConversationPair[conversationCapacity];
                        for (int i = 0; i < conversationCount; i++) {
                            newConversations[i] = conversations[i];
                        }
                        delete[] conversations;
                        conversations = newConversations;
                    }

                    conversations[conversationCount].human1 = currentHuman1;
                    conversations[conversationCount].human2 = human2;
                    conversationCount++;
                }

                currentHuman1 = ""; // Reset for next pair
            }
        }

        file.close();
        setColor(LIGHT_GREEN);
        cout << "  Loaded " << conversationCount << " conversation pairs from corpus." << endl;
        setColor(WHITE);

        return true;
    }


    bool loadHomeLoanData(const string& filename) {
        return loadLoanData(filename, homeLoanOptions, homeCount, homeCapacity);
    }

    bool loadCarLoanData(const string& filename) {
        return loadLoanData(filename, carLoanOptions, carCount, carCapacity);
    }

    bool loadBikeLoanData(const string& filename) {
        return loadLoanData(filename, bikeLoanOptions, bikeCount, bikeCapacity);
    }

    string getResponse(const string& input) {
        string lowerInput = toLower(trim(input));
        for (int i = 0; i < utteranceCount; i++) {
            if (utterances[i].input == lowerInput) {
                return utterances[i].response;
            }
        }
        return defaultResponse;
    }

    void run() {
        string input;
        bool running = true;
        bool infoCollected = false;

        displayWelcomeScreen();

        setColor(LIGHT_CYAN);
        cout << chatbotName << ": ";
        setColor(MAGENTA);
        cout << getResponse("hi") << endl;
        setColor(WHITE);

        setColor(LIGHT_YELLOW);
        cout << "\n   Type 'status' to check your application status" << endl;
        cout << "     Type 'resume' to continue an incomplete application" << endl;
        setColor(WHITE);

        while (running) {
            setColor(LIGHT_YELLOW);
            cout << "\nYou: ";
            setColor(BRIGHT_WHITE);
            getline(cin, input);
            input = trim(input);

            if (input.empty()) continue;

            string lowerInput = toLower(input);

            if (lowerInput == "x") {
                displayGoodbyeScreen();
                running = false;
                continue;
            }

            // Check application status
            if (lowerInput == "check status" || lowerInput == "status") {
                checkApplicationStatus();
                continue;
            }

            // Resume section ko aise update karo (without vector):
            if (lowerInput == "resume" || lowerInput == "continue") {
                string cnic = getValidInput("\n  Enter your CNIC (13 digits, no dashes): ", false);

                if (!isValidCNIC(cnic)) {
                    setColor(LIGHT_RED);
                    cout << "  Invalid CNIC format!" << endl;
                    setColor(WHITE);
                    continue;
                }

                // Search for incomplete applications with this CNIC using arrays
                string incompleteAppIds[100];  // Maximum 100 applications
                string incompleteAppStatus[100];
                int incompleteCount = 0;

                ifstream file("applications.txt");
                if (file.is_open()) {
                    string line;
                    while (getline(file, line) && incompleteCount < 100) {
                        string parts[50];
                        int count = 0;
                        splitString(line, '#', parts, count);

                        if (count >= 37 && parts[6] == cnic) {
                            string appStatus = parts[36];
                            if (appStatus == "C1" || appStatus == "C2" || appStatus == "C3") {
                                incompleteAppIds[incompleteCount] = parts[0];
                                incompleteAppStatus[incompleteCount] = appStatus;
                                incompleteCount++;
                            }
                        }
                    }
                    file.close();
                }

                if (incompleteCount == 0) {
                    setColor(LIGHT_RED);
                    cout << "\n  No incomplete applications found for CNIC: " << cnic << endl;
                    setColor(WHITE);
                    continue;
                }

                // Let user choose which application to resume
                string appIdToResume;
                if (incompleteCount == 1) {
                    appIdToResume = incompleteAppIds[0];
                    setColor(LIGHT_GREEN);
                    cout << "\n  Found incomplete application: " << appIdToResume << endl;
                    cout << "  Status: " << incompleteAppStatus[0] << endl;
                    setColor(WHITE);
                }
                else {
                    setColor(LIGHT_YELLOW);
                    cout << "\n  Multiple incomplete applications found:" << endl;
                    for (int i = 0; i < incompleteCount; i++) {
                        cout << "  " << (i + 1) << ". Application ID: " << incompleteAppIds[i]
                            << " (Status: " << incompleteAppStatus[i] << ")" << endl;
                    }
                    int choice = getValidNumberInput("\n  Select application to resume (1-" +
                        intToString(incompleteCount) + "): ",
                        1, incompleteCount);
                    appIdToResume = incompleteAppIds[choice - 1];
                }

                if (loadExistingApplication(appIdToResume, cnic)) {
                    setColor(LIGHT_GREEN);
                    cout << "\n  Application loaded successfully!" << endl;
                    cout << "  Current Status: " << currentApp.status << endl;
                    setColor(WHITE);
                    Sleep(1500);

                    // Continue from where user left
                    if (currentApp.status == "C1") {
                        if (!collectPersonalInformation()) continue;
                        if (!collectFinancialInformation()) continue;
                        if (!collectReferences()) continue;
                        if (!collectDocuments()) continue;
                    }
                    else if (currentApp.status == "C2") {
                        if (!collectFinancialInformation()) continue;
                        if (!collectReferences()) continue;
                        if (!collectDocuments()) continue;
                    }
                    else if (currentApp.status == "C3") {
                        if (!collectReferences()) continue;
                        if (!collectDocuments()) continue;
                    }

                    // Show summary and submit
                    displayApplicationSummary();

                    setColor(LIGHT_MAGENTA);
                    cout << "\n  Do you want to submit this application? (Y/N): ";
                    setColor(BRIGHT_WHITE);
                    string confirm;
                    getline(cin, confirm);

                    if (toLower(trim(confirm)) == "y" || toLower(trim(confirm)) == "yes") {
                        currentApp.status = "Submitted";
                        if (saveApplicationToFile()) {
                            setColor(LIGHT_GREEN);
                            cout << "\n  ========================================================" << endl;
                            cout << "     APPLICATION SUBMITTED SUCCESSFULLY!" << endl;
                            cout << "     Your Application ID: " << currentApp.applicationId << endl;
                            cout << "  ========================================================" << endl;
                            setColor(WHITE);
                            Sleep(2000);
                        }
                    }

                    initializeEmptyApplication();
                }
                else {
                    setColor(LIGHT_RED);
                    cout << "\n  Failed to load application!" << endl;
                    setColor(WHITE);
                }
                continue;
            }

            string response = getResponse(input);

            setColor(LIGHT_CYAN);
            cout << "\n" << chatbotName << ": ";
            setColor(LIGHT_GREEN);
            cout << response << endl;
            // General conversation mode
            if (lowerInput == "chat" || lowerInput == "talk" || lowerInput == "conversation") {
                setColor(LIGHT_CYAN);
                cout << "\n" << chatbotName << ": ";
                setColor(LIGHT_GREEN);
                cout << "Great! Let's chat. Type 'exit' to return." << endl;
                setColor(WHITE);

                bool inChatMode = true;
                while (inChatMode) {
                    setColor(LIGHT_YELLOW);
                    cout << "\nYou: ";
                    setColor(BRIGHT_WHITE);
                    string chatInput;
                    getline(cin, chatInput);
                    chatInput = trim(chatInput);

                    if (chatInput.empty()) continue;

                    string lowerChat = toLower(chatInput);

                    if (lowerChat == "exit" || lowerChat == "quit" || lowerChat == "back") {
                        setColor(LIGHT_CYAN);
                        cout << "\n" << chatbotName << ": ";
                        setColor(LIGHT_GREEN);
                        cout << "Back to main menu!" << endl;
                        setColor(WHITE);
                        inChatMode = false;
                            continue;
                    }

                    // Get response using IoU
                    string response = getBestResponseFromCorpus(chatInput);

                    setColor(LIGHT_CYAN);
                    cout << "\n" << chatbotName << ": ";
                    setColor(LIGHT_GREEN);
                    cout << response << endl;
                    setColor(WHITE);
                }
                continue;
            }

                      // setColor(WHITE);
            if (lowerInput == "h" || lowerInput == "c" || lowerInput == "e" || lowerInput == "b") {
                // Check if this is a fresh start (no application in progress)
                if (currentApp.applicationId.empty() || currentApp.status == "Submitted") {
                    setColor(LIGHT_CYAN);
                    cout << "\n" << chatbotName << ": ";
                    setColor(LIGHT_YELLOW);
                    cout << "Let's start with your loan application form." << endl;
                    setColor(WHITE);

                    Sleep(1000);

                    // Initialize new application
                    initializeEmptyApplication();

                    // Collect all checkpoints
                    if (!collectPersonalInformation()) {
                        setColor(LIGHT_YELLOW);
                        cout << "\n  Application saved at Checkpoint 1. You can resume later using 'resume' command." << endl;
                        setColor(WHITE);
                        Sleep(2000);
                        system("cls");
                        continue;
                    }

                    if (!collectFinancialInformation()) {
                        setColor(LIGHT_YELLOW);
                        cout << "\n  Application saved at Checkpoint 2. You can resume later using 'resume' command." << endl;
                        setColor(WHITE);
                        Sleep(2000);
                        system("cls");
                        continue;
                    }

                    if (!collectReferences()) {
                        setColor(LIGHT_YELLOW);
                        cout << "\n  Application saved at Checkpoint 3. You can resume later using 'resume' command." << endl;
                        setColor(WHITE);
                        Sleep(2000);
                        system("cls");
                        continue;
                    }

                    if (!collectDocuments()) {
                        setColor(LIGHT_YELLOW);
                        cout << "\nDocument collection cancelled." << endl;
                        setColor(WHITE);
                        Sleep(2000);
                        system("cls");
                        continue;
                    }
                }

                // Select loan type
                if (lowerInput == "h") {
                    handleLoanSelection(homeLoanOptions, homeCount, "Home");
                }
                else if (lowerInput == "c") {
                    if (carCount > 0) {
                        handleLoanSelection(carLoanOptions, carCount, "Car");
                    }
                    else {
                        setColor(LIGHT_YELLOW);
                        cout << "(Car loans coming soon)" << endl;
                        setColor(WHITE);
                    }
                }
                else if (lowerInput == "e" || lowerInput == "b") {
                    if (bikeCount > 0) {
                        handleLoanSelection(bikeLoanOptions, bikeCount, "Bike");
                    }
                    else {
                        setColor(LIGHT_YELLOW);
                        cout << "(Bike loans coming soon)" << endl;
                        setColor(WHITE);
                    }
                }

                // Show summary and confirm
                displayApplicationSummary();

                setColor(LIGHT_MAGENTA);
                cout << "\n  Do you want to submit this application? (Y/N): ";
                setColor(BRIGHT_WHITE);
                string confirm;
                getline(cin, confirm);

                if (toLower(trim(confirm)) == "y" || toLower(trim(confirm)) == "yes") {
                    currentApp.status = "Submitted";
                    if (saveApplicationToFile()) {
                        setColor(LIGHT_GREEN);
                        cout << "\n  ========================================================" << endl;
                        cout << "     APPLICATION SUBMITTED SUCCESSFULLY!" << endl;
                        cout << "     Your Application ID: " << currentApp.applicationId << endl;
                        cout << "  ========================================================" << endl;
                        setColor(WHITE);

                        // Simulate approval for demo
                        setColor(LIGHT_YELLOW);
                        cout << "\n  For approved applications, would you like to see" << endl;
                        cout << "  monthly payment plan with actual months? (Y/N): ";
                        setColor(BRIGHT_WHITE);
                        string showPlan;
                        getline(cin, showPlan);

                        if (toLower(trim(showPlan)) == "y" || toLower(trim(showPlan)) == "yes") {
                            setColor(LIGHT_YELLOW);
                            cout << "\n  Enter starting month (1-12, where 1=January): ";
                            setColor(BRIGHT_WHITE);
                            int startMonth = getValidNumberInput("", 1, 12);

                            generateMonthlyPlanWithMonths(currentApp.loanAmount, currentApp.installmentMonths, startMonth);
                        }

                        Sleep(3000);
                    }
                    else {
                        setColor(LIGHT_RED);
                        cout << "\n  Failed to save application!" << endl;
                        setColor(WHITE);
                    }
                }
                else {
                    setColor(LIGHT_YELLOW);
                    cout << "\n  Application not submitted." << endl;
                    setColor(WHITE);
                }

                // Don't initialize empty application here - let it stay "Submitted" 
 // so next loan selection triggers fresh collection
                currentApp.status = "Submitted"; // Mark as submitted to trigger new collection

                setColor(LIGHT_MAGENTA);
                cout << "\n  Press X to exit or Enter to continue: ";
                setColor(BRIGHT_WHITE);
                getline(cin, input);
                if (toLower(trim(input)) == "x") {
                    displayGoodbyeScreen();
                    running = false;
                }
            }
        }
    }
};
//======================================================
// MAIN FUNCTION
//======================================================
int main() {
    LoanApplicationSystem chatbot;
    if (!chatbot.loadUtterances("Utterances.txt")) {
        setColor(LIGHT_RED);
        cout << "\nPress any key to exit...";
        setColor(WHITE);
        cin.get();
        return 1;
    }

    chatbot.loadHomeLoanData("Home.txt");
    chatbot.loadCarLoanData("Car.txt");
    chatbot.loadBikeLoanData("Bike.txt");

    if (!chatbot.loadHumanChatCorpus("human_chat_corpus.txt")) {
        setColor(LIGHT_YELLOW);
        cout << "Warning: Could not load conversation corpus.\n";
        setColor(WHITE);
    }

   
    chatbot.run();
    return 0;
}