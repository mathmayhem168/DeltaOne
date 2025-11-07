// My first C++ program made at 9 yrs old!
// Pro tip: always include and import at the top of the file.


#include <cmath>
#include <iostream>

using namespace std;

void root_test(float x) {
    float result = x + sqrt(x);
    bool testTrue = (result >= 0 || result < 0);
    if (testTrue) {
        cout << "The output is: " << result << endl;
        cout << "Does Root Theory work? " << "Yes!" << endl;
    }
    else {
        cout << "The output is: " << result << endl;
        cout << "Does Root Theory work? " << "No." << endl;
    }
}

void root_research(int lower, int higher) {
    for (int i = lower; i <= higher; i++) {
        double result = i + sqrt(i);
        if (isnan(result)) {
            cout << "Error at: " << i << endl;
            continue;
        
        
        
        }
            
            
        }
        
        
    }


    





int main() {
    cout << "Welcome to Experiments!" << endl;
    cout << "So, I'll try making a conditional branch for an input." << endl;
    cout << "1: Print Hello, World!" << endl;
    cout << "2: See If 2+2=4" << endl;
    cout << "3: Both" << endl;
    int input;
    cout << "Pick something, anything. ";
    cin >> input;
    if (input == 1) {
        cout << "Hello, World!" << endl;
    }
    else if (input == 2) {
        bool result = (2 + 2 == 4);
        cout << "It'll say 1 or 0 for True or False." << endl << "Is 2+2=4 true? " << result << endl;
    }
    else if (input == 3) {
        cout << "Hello, World!" << endl;
        bool result = (2 + 2 == 4);
        cout << "It'll say 1 or 0 for True or False." << endl << "Is 2+2=4 true? " << result << endl;
    }
    else {
        cout << "Oops, C++ couldn't understand that. Maybe try again?";
    }
    }

    


