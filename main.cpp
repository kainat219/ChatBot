#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<iomanip>
#include <windows.h>

using namespace std;

//======================================================
// CONSOLE COLOR CODES FOR WINDOWS
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
// STRUCTURE: Utterance
// Purpose: Stores a chatbot input and its corresponding response
//======================================================
struct Utterance {
    string input;
    string response;
};

//======================================================
// STRUCTURE: LoanOption
// Purpose: Stores information about a specific home loan option
//======================================================
struct LoanOption {
    string category;
    string details;
    string installments;
    string price;
    string downPayment;
};

//======================================================
// CLASS: LoanApplicationSystem
// Purpose: Handles chatbot interaction and home loan information
//======================================================
class LoanApplicationSystem {
private:
    Utterance* utterances;
    int utteranceCount;
    int utteranceCapacity;

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

    //======================================================
    // FUNCTION: trim
    // Aim: Removes whitespace (spaces, tabs, newlines) from
    //      the beginning and end of a string.
    //======================================================
    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }

    //======================================================
    // FUNCTION: toLower
    // Aim: Converts all uppercase characters in a string
    //      to lowercase for case-insensitive comparison.
    //======================================================
    string toLower(string str) {
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] >= 'A' && str[i] <= 'Z') {
                str[i] = str[i] + 32;
            }
        }
        return str;
    }
    //======================================================
    // FUNCTION: isValidNumber
    // Aim: Checks if string contains only digits
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

//======================================================
// FUNCTION: stringToDouble
// Aim: Converts string to double, removing commas
//======================================================
double stringToDouble(const string& str) {
    string numStr = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] != ',') {
            numStr += str[i];
        }
    }
    return atof(numStr.c_str());
}

//======================================================
// FUNCTION: stringToInt
// Aim: Converts string to integer
//======================================================
int stringToInt(const string& str) {
    return atoi(str.c_str());
}

//======================================================
// FUNCTION: formatNumber
// Aim: Formats number with commas for better readability
//======================================================
string formatNumber(double num) {
    stringstream ss;
    ss << fixed << setprecision(2) << num;
    string result = ss.str();

    if (result.substr(result.length() - 3) == ".00") {
        result = result.substr(0, result.length() - 3);
    }

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

 //======================================================
 // FUNCTION: calculateMonthlyInstallment
 // Aim: Calculates monthly installment amount
 //      Formula: (Price - Down Payment) / Number of Installments
 //======================================================
 double calculateMonthlyInstallment(double price, double downPayment, int installments) {
     return (price - downPayment) / installments;
 }

 //======================================================
 // FUNCTION: resizeUtterances
 // Aim: Expands the utterances array size dynamically
 //      when capacity is reached.
 //======================================================
    void resizeUtterances() {
        utteranceCapacity *= 2;
        Utterance* newUtterances = new Utterance[utteranceCapacity];
        for (int i = 0; i < utteranceCount; i++) {
            newUtterances[i] = utterances[i];
        }
        delete[] utterances;
        utterances = newUtterances;
    }

   //======================================================
   // FUNCTION: resizeLoanArray
  // Aim: Generic function to resize loan option arrays
  //======================================================
    void resizeLoanArray(LoanOption*& array, int& capacity) {
     capacity *= 2;
     LoanOption* newArray = new LoanOption[capacity];
     for (int i = 0; i < capacity / 2; i++) {
         newArray[i] = array[i];
     }
     delete[] array;
     array = newArray;
 
    }

    //======================================================
    // FUNCTION: displayWelcomeScreen
    // Aim: Displays an attractive welcome screen with chatbot name
    //======================================================
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

    //======================================================
    // FUNCTION: displayGoodbyeScreen
    // Aim: Displays a farewell message when user exits
    //======================================================
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
        Sleep(2000); // Pause for 2 seconds
    }

    //======================================================
    // FUNCTION: generateInstallmentPlan
    // Aim: Generates and displays complete installment plan
    //======================================================
void generateInstallmentPlan(const LoanOption& loan, const string& loanType, int userInstallments) {
    double price = stringToDouble(loan.price);
    double downPayment = stringToDouble(loan.downPayment);
    int installments = userInstallments;

    double monthlyAmount = calculateMonthlyInstallment(price, downPayment, installments);
    double remainingBalance = price - downPayment;

    setColor(LIGHT_CYAN);
    cout << "\n  ========================================================" << endl;
    setColor(LIGHT_YELLOW);
    cout << "                  INSTALLMENT PLAN" << endl;
    setColor(LIGHT_CYAN);
    cout << "  ========================================================" << endl;
    setColor(WHITE);

    setColor(LIGHT_GREEN);
    cout << "\n  Loan Summary:" << endl;
    setColor(WHITE);
    cout << "    Loan Type: " << loanType << endl;
    cout << "    Category: " << loan.category << endl;
    cout << "    Details: " << loan.details << endl;
    cout << "    Total Price: Rs. " << formatNumber(price) << endl;
    cout << "    Down Payment: Rs. " << formatNumber(downPayment) << endl;
    cout << "    Loan Amount: Rs. " << formatNumber(remainingBalance) << endl;
    cout << "    Number of Installments: " << installments << " months" << endl;
    setColor(LIGHT_CYAN);
    cout << "    Monthly Installment: Rs. " << formatNumber(monthlyAmount) << endl;
    setColor(WHITE);

    setColor(LIGHT_BLUE);
    cout << "\n  --------------------------------------------------------" << endl;
    setColor(LIGHT_YELLOW);
    cout << "  Month      Monthly Payment      Remaining Balance" << endl;
    setColor(LIGHT_BLUE);
    cout << "  --------------------------------------------------------" << endl;
    setColor(WHITE);

    for (int month = 1; month <= installments; month++) {
        cout << "  " << setw(5) << month << "      ";
        setColor(LIGHT_GREEN);
        cout << "Rs. " << setw(15) << left << formatNumber(monthlyAmount);
        setColor(WHITE);
        cout << "  ";

        remainingBalance -= monthlyAmount;
        if (remainingBalance < 0.01) remainingBalance = 0;

        setColor(LIGHT_CYAN);
        cout << "Rs. " << formatNumber(remainingBalance) << endl;
        setColor(WHITE);
    }

    setColor(LIGHT_BLUE);
    cout << "  --------------------------------------------------------" << endl;
    setColor(LIGHT_GREEN);
    cout << "\n  Total Amount Paid: Rs. " << formatNumber(price) << endl;
    setColor(WHITE);
}

    //======================================================
    // FUNCTION: getValidInput
    // Aim: Gets valid input from user with custom validation
    //======================================================
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
                cout << "Input cannot be empty. Please try again." << endl;
                continue;
            }

            if (allowX && toLower(input) == "x") {
                return "x";
            }

            return input;
        }
    }

    //======================================================
    // FUNCTION: getValidNumberInput
    // Aim: Gets valid numeric input from user
    //======================================================
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
                continue;
            }

            if (!isValidNumber(input)) {
                setColor(LIGHT_RED);
                cout << "  Invalid input! Please enter a number between " << min << " and " << max << "." << endl;
                continue;
            }

            int value = stringToInt(input);
            if (value < min || value > max) {
                setColor(LIGHT_RED);
                cout << "  Invalid option! Please enter a number between " << min << " and " << max << "." << endl;
                continue;
            }

            return value;
        }
    }

//======================================================
// FUNCTION: displayLoanOptions
// Aim: Displays all loan options for specific type and category
//======================================================
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
            cout << "    Available Installment Plans: ";
            setColor(BRIGHT_WHITE);
            cout << options[i].installments << " months (or custom)" << endl;
            setColor(LIGHT_BLUE);
            cout << "  --------------------------------------------------------" << endl;
            setColor(WHITE);
        }
    }

    if (optionNum == 0) {
        setColor(LIGHT_RED);
        cout << "  No loan options available for " << category << endl;
        setColor(WHITE);
    }

    return optionNum;
}

//======================================================
// FUNCTION: selectAndShowInstallmentPlan
// Aim: Allows user to select option, choose installments, and view plan
//======================================================
void selectAndShowInstallmentPlan(LoanOption* options, int count, const string& category, const string& loanType, int displayedCount) {
    if (displayedCount == 0) {
        return;
    }

    int selection = getValidNumberInput("\n  Enter option number to view installment plan (1-" +
        to_string(displayedCount) + "), or 0 to skip: ", 0, displayedCount);

    if (selection == 0) {
        return;
    }

    // Find the selected loan option
    int currentOption = 0;
    LoanOption selectedLoan;
    for (int i = 0; i < count; i++) {
        if (toLower(options[i].category) == toLower(category)) {
            currentOption++;
            if (currentOption == selection) {
                selectedLoan = options[i];
                break;
            }
        }
    }

    // Show available installment options and let user choose
    setColor(LIGHT_CYAN);
    cout << "\n  ========================================================" << endl;
    setColor(LIGHT_YELLOW);
    cout << "              SELECT NUMBER OF INSTALLMENTS" << endl;
    setColor(LIGHT_CYAN);
    cout << "  ========================================================" << endl;
    setColor(WHITE);

    double price = stringToDouble(selectedLoan.price);
    double downPayment = stringToDouble(selectedLoan.downPayment);

    setColor(LIGHT_GREEN);
    cout << "\n  Suggested installment plans for this loan:" << endl;
    setColor(WHITE);

    // Show suggested plan from file
    int suggestedInstallments = stringToInt(selectedLoan.installments);
    double suggestedMonthly = calculateMonthlyInstallment(price, downPayment, suggestedInstallments);

    setColor(LIGHT_YELLOW);
    cout << "    Suggested: " << suggestedInstallments << " months ";
    setColor(WHITE);
    cout << "=> Monthly Payment: ";
    setColor(LIGHT_CYAN);
    cout << "Rs. " << formatNumber(suggestedMonthly) << endl;
    setColor(WHITE);

    // Show some common alternatives
    int alternatives[] = { 12, 24, 36, 48, 60 };
    setColor(LIGHT_GREEN);
    cout << "\n  Common alternatives:" << endl;
    setColor(WHITE);

    for (int i = 0; i < 5; i++) {
        if (alternatives[i] != suggestedInstallments) {
            double altMonthly = calculateMonthlyInstallment(price, downPayment, alternatives[i]);
            setColor(LIGHT_YELLOW);
            cout << "    " << alternatives[i] << " months ";
            setColor(WHITE);
            cout << "=> Monthly Payment: ";
            setColor(LIGHT_CYAN);
            cout << "Rs. " << formatNumber(altMonthly) << endl;
            setColor(WHITE);
        }
    }

    // Get user's choice
    setColor(LIGHT_MAGENTA);
    cout << "\n  Enter your preferred number of installments (1-120 months): ";
    setColor(BRIGHT_WHITE);

    int userInstallments = getValidNumberInput("", 1, 120);

    // Calculate and display monthly installment
    double monthlyAmount = calculateMonthlyInstallment(price, downPayment, userInstallments);

    setColor(LIGHT_CYAN);
    cout << "\n  ========================================================" << endl;
    setColor(LIGHT_YELLOW);
    cout << "               YOUR MONTHLY INSTALLMENT" << endl;
    setColor(LIGHT_CYAN);
    cout << "  ========================================================" << endl;
    setColor(LIGHT_GREEN);
    cout << "\n  For " << userInstallments << " months, your monthly payment will be: ";
    setColor(LIGHT_YELLOW);
    cout << "Rs. " << formatNumber(monthlyAmount) << endl;
    setColor(WHITE);

    // Ask if user wants detailed installment plan
    setColor(LIGHT_MAGENTA);
    cout << "\n  Would you like to see a detailed installment plan? (Y/N): ";
    setColor(BRIGHT_WHITE);

    string response;
    getline(cin, response);
    response = trim(response);

    if (toLower(response) == "y" || toLower(response) == "yes") {
        generateInstallmentPlan(selectedLoan, loanType, userInstallments);
    }
}

//======================================================
// FUNCTION: handleLoanSelection
// Aim: Generic function to handle loan selection for any type
//======================================================
void handleLoanSelection(LoanOption* options, int count, const string& loanType) {
    // Get unique categories
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
        cout << "\n  No " << loanType << " loan options available at this time." << endl;
        setColor(WHITE);
        return;
    }

    // Display categories
    setColor(LIGHT_CYAN);
    cout << "\n  Available " << loanType << " Categories:" << endl;
    setColor(WHITE);
    for (int i = 0; i < categoryCount; i++) {
        setColor(LIGHT_GREEN);
        cout << "    " << (i + 1) << ". " << categories[i] << endl;
    }
    setColor(WHITE);

    int selection = getValidNumberInput("\n  Select category (1-" + to_string(categoryCount) + "): ",
        1, categoryCount);

    string selectedCategory = categories[selection - 1];
    int displayedCount = displayLoanOptions(options, count, selectedCategory, loanType);
    selectAndShowInstallmentPlan(options, count, selectedCategory, loanType, displayedCount);
}
public:
    //======================================================
// CONSTRUCTOR: LoanApplicationSystem
// Aim: Initializes data members and allocates memory
//======================================================
LoanApplicationSystem() {
    utteranceCapacity = 10;
    utteranceCount = 0;
    utterances = new Utterance[utteranceCapacity];

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
}

   //======================================================
 // DESTRUCTOR: ~LoanApplicationSystem
 // Aim: Frees dynamically allocated memory
 //======================================================
 ~LoanApplicationSystem() {
     delete[] utterances;
     delete[] homeLoanOptions;
     delete[] carLoanOptions;
     delete[] bikeLoanOptions;
 }

    //======================================================
    // FUNCTION: loadUtterances
    // Aim: Loads chatbot input-response pairs from file.
    //      Stores default response if input is '*'.
    //======================================================
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
//======================================================
// FUNCTION: loadLoanData
// Aim: Generic function to load loan data from file
//======================================================
bool loadLoanData(const string& filename, LoanOption*& options, int& count, int& capacity) {
    ifstream file(filename);
    if (!file.is_open()) {
        setColor(LIGHT_RED);
        cerr << "Error: Could not open " << filename << endl;
        setColor(WHITE);
        return false;
    }

    string line;
    bool firstLine = true;

    while (getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }

        if (line.empty() || trim(line).empty()) {
            continue;
        }

        if (count >= capacity) {
            resizeLoanArray(options, capacity);
        }

        stringstream ss(line);
        getline(ss, options[count].category, '#');
        getline(ss, options[count].details, '#');
        getline(ss, options[count].installments, '#');
        getline(ss, options[count].price, '#');
        getline(ss, options[count].downPayment, '#');

        options[count].category = trim(options[count].category);
        options[count].details = trim(options[count].details);
        options[count].installments = trim(options[count].installments);
        options[count].price = trim(options[count].price);
        options[count].downPayment = trim(options[count].downPayment);

        count++;
    }
    file.close();
    return true;
}
 //======================================================
// FUNCTION: loadHomeLoanData
// Aim: Loads home loan data from file
//======================================================
bool loadHomeLoanData(const string& filename) {
    return loadLoanData(filename, homeLoanOptions, homeCount, homeCapacity);
}

//======================================================
// FUNCTION: loadCarLoanData
// Aim: Loads car loan data from file
//======================================================
bool loadCarLoanData(const string& filename) {
    return loadLoanData(filename, carLoanOptions, carCount, carCapacity);
}

//======================================================
// FUNCTION: loadBikeLoanData
// Aim: Loads electric bike loan data from file
//======================================================
bool loadBikeLoanData(const string& filename) {
    return loadLoanData(filename, bikeLoanOptions, bikeCount, bikeCapacity);
}
    //======================================================
    // FUNCTION: getResponse
    // Aim: Returns chatbot response for user input by
    //      matching against stored utterances.
    //======================================================
    string getResponse(const string& input) {
        string lowerInput = toLower(trim(input));

        for (int i = 0; i < utteranceCount; i++) {
            if (utterances[i].input == lowerInput) {
                return utterances[i].response;
            }
        }
        return defaultResponse;
    }   

    //======================================================
    // FUNCTION: run
    // Aim: Runs the chatbot application loop.
    //      Handles user input, area selection, and system exit.
    //======================================================
    void run() {
        string input;
        bool running = true;

        displayWelcomeScreen();

        setColor(LIGHT_CYAN);
        cout << chatbotName << ": ";
        setColor(MAGENTA);
        cout << getResponse("hi") << endl;
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
            string response = getResponse(input);

 setColor(LIGHT_CYAN);
 cout << "\n" << chatbotName << ": ";
 setColor(LIGHT_GREEN);
 cout << response << endl;
 setColor(WHITE);

           // Handle loan type selection
        if (lowerInput == "h") {
            handleLoanSelection(homeLoanOptions, homeCount, "Home");

            setColor(LIGHT_MAGENTA);
            cout << "\nPress X to exit or any other key to continue: ";
            setColor(BRIGHT_WHITE);
            getline(cin, input);
            if (toLower(trim(input)) == "x") {
                displayGoodbyeScreen();
                running = false;
            }
        }
        else if (lowerInput == "c") {
            if (carCount > 0) {
                handleLoanSelection(carLoanOptions, carCount, "Car");
            }
            else {
                setColor(LIGHT_YELLOW);
                cout << "(Car loan options will be available in future updates)" << endl;
                setColor(WHITE);
            }

            setColor(LIGHT_MAGENTA);
            cout << "\nPress X to exit or any other key to continue: ";
            setColor(BRIGHT_WHITE);
            getline(cin, input);
            if (toLower(trim(input)) == "x") {
                displayGoodbyeScreen();
                running = false;
            }
        }
        else if (lowerInput == "e" || lowerInput == "b") {
            if (bikeCount > 0) {
                handleLoanSelection(bikeLoanOptions, bikeCount, "Electric Bike");
            }
            else {
                setColor(LIGHT_YELLOW);
                cout << "(Electric bike loan options will be available in future updates)" << endl;
                setColor(WHITE);
            }

            setColor(LIGHT_MAGENTA);
            cout << "\nPress X to exit or any other key to continue: ";
            setColor(BRIGHT_WHITE);
            getline(cin, input);
            if (toLower(trim(input)) == "x") {
                displayGoodbyeScreen();
                running = false;
            }
        }
        else if (lowerInput == "a") {
           
            continue;
        }
    }
}
};

//======================================================
// FUNCTION: main
// Aim: Program entry point. Loads data files and runs the
//      loan application chatbot.
//======================================================

int main() {
    LoanApplicationSystem chatbot ; 
   
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

 
 chatbot.run();
 return 0;
}