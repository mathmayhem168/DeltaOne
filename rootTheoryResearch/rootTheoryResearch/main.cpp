#include <iostream>
#include <cmath>

using namespace std;

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
    root_research(1, 10 * pow(10, 9));
}

