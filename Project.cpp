#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <limits> // included for numeric_limits

using namespace std;

// Define a struct to hold the survey responses
struct Response {
    string name;      // Name of the respondent
    string answer1;   // Response to the first question
    string answer2;   // Response to the second question
    string answer3;   // Response to the third question
};

// Check if the answer to the first question is valid
bool isValidAnswer1(const string& answer) {
    return answer == "yes" || answer == "no"; // Valid answers are "yes" or "no"
}

// Check if the answer to the second question is valid
bool isValidAnswer2(const string& answer) {
    return answer == "C++" || answer == "Python" || answer == "Java"; // Valid answers are "C++", "Python", or "Java"
}

// Check if the input string is a valid number
bool isValidNumber(const string& str) {
    for (char const &c : str) {
        if (isdigit(c) == 0) return false; // If any character is not a digit, return false
    }
    return true; // All characters are digits
}

// Function to ask the survey questions and collect responses
void askQuestions(vector<Response>& responses) {
    Response response;
    cout << "What is your name? ";
    while (getline(cin, response.name), response.name.empty()) {
        cout << "Name cannot be empty. Please enter your name: "; // Ensure name is not empty
    }

    do {
        cout << "Question 1: Do you like programming? (yes/no): ";
        getline(cin, response.answer1);
        if (!isValidAnswer1(response.answer1)) {
            cout << "Invalid answer. Please enter 'yes' or 'no'.\n"; // Ensure valid answer
        }
    } while (!isValidAnswer1(response.answer1));

    do {
        cout << "Question 2: Which language do you prefer? (C++/Python/Java): ";
        getline(cin, response.answer2);
        if (!isValidAnswer2(response.answer2)) {
            cout << "Invalid answer. Please enter 'C++', 'Python', or 'Java'.\n"; // Ensure valid answer
        }
    } while (!isValidAnswer2(response.answer2));

    do {
        cout << "Question 3: How many hours do you code per week? ";
        getline(cin, response.answer3);
        if (!isValidNumber(response.answer3) || response.answer3.empty()) {
            cout << "Invalid answer. Please enter a numeric value.\n"; // Ensure valid numeric answer
        }
    } while (!isValidNumber(response.answer3) || response.answer3.empty());

    responses.push_back(response); // Add the response to the list of responses
}

// Function to save responses to a file
bool saveResponses(const vector<Response>& responses, const string& filename) {
    ofstream file(filename, ios::app); // Open the file in append mode
    if (file.is_open()) {
        for (const auto& response : responses) {
            file << response.name << ","
                 << response.answer1 << ","
                 << response.answer2 << ","
                 << response.answer3 << endl; // Write each response to the file
        }
        file.close();
        return true;
    } else {
        cerr << "Unable to open file for writing!" << endl;
        return false;
    }
}

// Function to load responses from a file
vector<Response> loadResponses(const string& filename) {
    vector<Response> responses;
    ifstream file(filename); // Open the file for reading
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            Response response;
            stringstream ss(line); // Parse each line
            getline(ss, response.name, ',');
            getline(ss, response.answer1, ',');
            getline(ss, response.answer2, ',');
            getline(ss, response.answer3, ',');

            if (isValidNumber(response.answer3)) {
                responses.push_back(response); // Add valid response to the list
            } else {
                cerr << "Invalid number in the file for coding hours: " << response.answer3 << endl;
            }
        }
        file.close();
    } else {
        cerr << "Unable to open file for reading!" << endl;
    }

    return responses;
}

// Function to display statistics from the responses
void displayStatistics(const vector<Response>& responses) {
    if (responses.empty()) {
        cout << "No responses available to display statistics.\n";
        return;
    }

    map<string, int> answer1Count; // To count yes/no responses
    map<string, int> answer2Count; // To count language preferences
    int totalCodingHours = 0;
    int validResponsesCount = 0;

    for (const auto& response : responses) {
        if (isValidNumber(response.answer3)) {
            answer1Count[response.answer1]++;
            answer2Count[response.answer2]++;
            totalCodingHours += stoi(response.answer3); // Convert string to int
            validResponsesCount++;
        } else {
            cout << "Invalid data found for " << response.name << ". Skipping this response.\n";
        }
    }

    cout << "\nStatistics:\n";
    cout << "Question 1: Do you like programming? (yes/no):\n";
    for (const auto& pair : answer1Count) {
        cout << pair.first << ": " << pair.second << endl; // Display counts for yes/no
    }

    cout << "Question 2: Which language do you prefer? (C++/Python/Java):\n";
    for (const auto& pair : answer2Count) {
        cout << pair.first << ": " << pair.second << endl; // Display counts for languages
    }

    if (validResponsesCount > 0) {
        cout << "Average hours of coding per week: " 
             << static_cast<double>(totalCodingHours) / validResponsesCount 
             << endl; // Display average coding hours
    } else {
        cout << "No valid responses to calculate average coding hours.\n";
    }
}

// Function to display the menu
void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Take survey\n";
    cout << "2. View statistics\n";
    cout << "3. Save responses to file\n";
    cout << "4. Load responses from file\n";
    cout << "5. Exit\n";
    cout << "Choose an option: ";
}

// Main function
int main() {
    vector<Response> responses; // Vector to hold all responses
    string filename = "responses.txt"; // Filename to save/load responses
    int choice;

    while (true) {
        displayMenu(); // Show the menu
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // To ignore the newline character left in the input buffer

        if (choice == 1) {
            askQuestions(responses); // Ask survey questions
        } else if (choice == 2) {
            displayStatistics(responses); // Display statistics
        } else if (choice == 3) {
            if (saveResponses(responses, filename)) {
                cout << "Responses saved successfully.\n";
            } else {
                cout << "Failed to save responses.\n";
            }
        } else if (choice == 4) {
            responses = loadResponses(filename);
            if (!responses.empty()) {
                cout << "Responses loaded successfully.\n";
            } else {
                cout << "No responses found in file or file could not be read.\n";
            }
        } else if (choice == 5) {
            cout << "Thank you for using the program. Goodbye!\n";
            break; // Exit the loop and end the program
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
