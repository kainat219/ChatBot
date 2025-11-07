#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
    string area;
    string size;
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
    int loanCount;
    int loanCapacity;

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
    // FUNCTION: resizeLoanOptions
    // Aim: Expands the home loan options array dynamically
    //      when capacity is reached.
    //======================================================
    void resizeLoanOptions() {
        loanCapacity *= 2;
        LoanOption* newOptions = new LoanOption[loanCapacity];
        for (int i = 0; i < loanCount; i++) {
            newOptions[i] = homeLoanOptions[i];
        }
        delete[] homeLoanOptions;
        homeLoanOptions = newOptions;
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

public:
    //======================================================
    // CONSTRUCTOR: LoanApplicationSystem
    // Aim: Initializes data members and allocates memory
    //      for utterances and home loan arrays.
    //======================================================
    LoanApplicationSystem() {
        utteranceCapacity = 10;
        utteranceCount = 0;
        utterances = new Utterance[utteranceCapacity];

        loanCapacity = 10;
        loanCount = 0;
        homeLoanOptions = new LoanOption[loanCapacity];

        defaultResponse = "";
        chatbotName = "LOAN-BUDDY";
    }

    //======================================================
    // DESTRUCTOR: ~LoanApplicationSystem
    // Aim: Frees the dynamically allocated memory when
    //      the object is destroyed.
    //======================================================
    ~LoanApplicationSystem() {
        delete[] utterances;
        delete[] homeLoanOptions;
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


            if (lowerInput == "h") {
                setColor(LIGHT_CYAN);
                cout << "\n" << chatbotName << ": ";
                setColor(MAGENTA);
                cout << getResponse("h") << endl;
                setColor(WHITE);

                setColor(LIGHT_YELLOW);
                cout << "\nYou: ";
                setColor(BRIGHT_WHITE);
                getline(cin, input);
                input = trim(input);

                if (toLower(input) == "x") {
                    displayGoodbyeScreen();
                    running = false;
                    continue;
                }


                if (input == "1" || input == "2" || input == "3" || input == "4") {
                    setColor(GREEN); 
                    cout << "Home Loan Starts here......" << endl; 
                    setColor(WHITE);
                }
                  else {
                    setColor(LIGHT_RED);
                    cout << "Invalid area selection. Please try again." << endl;
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
                continue;
            }


            string response = getResponse(input);
            setColor(LIGHT_CYAN);
            cout << "\n" << chatbotName << ": ";
            setColor(MAGENTA);
            cout << response << endl;
            setColor(WHITE);
        }
    }
};

//======================================================
// FUNCTION: main
// Aim: Program entry point. Loads data files and runs the
//      loan application chatbot.
//======================================================
int main() {
    LoanApplicationSystem chatbot;


    if (!chatbot.loadUtterances("Utterances.txt")) {
        return 1;
    }


    chatbot.run();

    return 0;
}