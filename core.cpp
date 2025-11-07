#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <random>




using namespace std;


// Functions



// --------- Lists And Data --------


int randint(int a, int b) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(a, b);
    return dist(gen);
}

int highest(const vector<int>& list) {
    int highest_int = list[0];
    for (int element = list[0]; element < list.size(); element += 1) {
        if (list[element] > highest_int) {
            highest_int = element;
        }
        else {
            continue;
        }
    }
    return highest_int;
}

int lowest(const vector<int>& list) {
    int lowest_int = list[0];
    for (int element = list[0]; element < list.size(); element += 1) {
        if (list[element] < lowest_int) {
            lowest_int = element;
        }
        else {
            continue;
        }
    }
    return lowest_int;
}


bool detectPattern(const vector<int>& list) {
    if (list.size() < 2)
        return false; 

    int diff = list[1] - list[0];

    for (size_t i = 2; i < list.size(); ++i) {
        if (list[i] - list[i - 1] != diff)
            return false; 
    }

    return true; 
}


bool isin(const vector<int>& list, int element) {
    for (int i = 0; i < list.size(); i++) {
        if (element == list[i]) {
            return true;
        }
    }
    return false;
}






















// ======== MATH ========


void quadratic_solver(double a, double b, double c) {
    double discriminant = (b * b) - (4 * (a * c));
    double x1 = (-b + sqrt(((b * b) - (4 * (a * c))))) / (2 * a);
    double x2 = (-b - sqrt(((b * b) - (4 * (a * c))))) / (2 * a);
    if (discriminant == 0) {
        cout << "Detected 1 real root. Discriminant: " << discriminant << endl;
    }
    else if (discriminant < 0) {
        cout << "Detected 2 complex roots. Discriminant: " << discriminant << endl;
    }
    else {
        cout << "Detected 2 real roots. Discriminant: " << discriminant << endl;
    }
    cout << "First Solution: " << x1 << endl;
    cout << "Second Solution: " << x2 << endl;
}

// Here it goes...

double riemannSumApprox(double a, double b, int n, function<double(double)> func, string method = "midpoint") {
    double deltaX = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double x_i;

        if (method == "left") {
            x_i = a + i * deltaX;
        } 
        else if (method == "right") {
            x_i = a + (i + 1) * deltaX;
        } 
        else { // midpoint
            x_i = a + (i + 0.5) * deltaX;
        }

        double y_i = func(x_i);
        sum += y_i * deltaX;
    }

    return sum;
}


void rootResearch(long long lower, long long higher) {
    int error_counter = 0;
    for (long long i = lower; i <= higher; i++) {
        double result = i + sqrt(i);
        if (isnan(result)) {
            cout << "Error detected at i = " << i << endl;
            error_counter += 1;
        }
    }
    cout << "==== Research =====" << endl;
    cout << "Errors: " << error_counter << endl;
    cout << "Research Successful? " << (error_counter == 0) << endl;
    cout << "===================" << endl;
}

int variantLaw(const vector<int>& list, bool subPattern) {
    if (!subPattern) {
        int r = randint(lowest(list), highest(list));
        return isin(list, r) ? r : list[0];
    }

    if (detectPattern(list)) {
        return list[3];
    }

    return list[0];
}










































int main() {
    return 0;
}