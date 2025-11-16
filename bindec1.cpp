#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>




using namespace std;

string dec_bin(long long n) {
    string result;
    if (n <= 0) {
        return "0";
    }

    vector<bool> bits;

    while (n > 0) {
        bits.push_back(n % 2 == 1);
        n /= 2;
    }

    for (int i = bits.size() - 1; i >= 0; i--) {
        result += (bits[i] ? '1' : '0');
    }

    return result;
}


int main() {
    cout << "==== Decimal to Binary =====" << endl;
    cout << "Convert? ";
    string input;
    cin >> input;
    if (input == "y") {
        cout << "Decimal (base 10): " << endl;
        string decimal;
        cin >> decimal;
        try {
            cout << "The string " << decimal << " is " << dec_bin(stoll(decimal)) << " in binary." << endl;
        }
        catch (...) {
            cout << "There was an error." << endl;
        }
    }
    else if (input == "n") {
        cout << "Exiting Decimal to Binary Converter.." << endl;
        exit(0);
    }
}