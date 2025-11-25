#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>



using namespace std;












// This is a program with a lot of algorithms having to do with math/data.

// ===== CS ==========

long binaryToDecimal(string binary) {
    long value = 0;
    for (char c : binary) {
        int bit = c - '0';
        value = value * 2 + bit;
    }
    return value;
}

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

int countOnes(string n) {
    int count = 0;
    
    for (char c : n) {
        if (c == '1') {
            count++;
        }
    }
    
    return count;
}


bool isInList(vector<double> nums, double target) {
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == target) {
            return true;
        }
    }
    return false;
}

















// ======= MATH ============

// ==== NUMERICAL/CALCULUS =====

long gcd(long a, long b) {
    while (b != 0) {
        long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


long amounting(int x, int y) {
    if (y < 0) {
        cout << "Error: y is smaller than 0." << endl;
    }
    if (y == 0) {
        return 1;
    }
    if (y % 2 == 0) {
        long new_x = x * x;
        long new_y = y / 2;
        return amounting(new_x, new_y);
    }
    else {
        long new_x = x * x;
        long new_y = (y - 1) / 2;
        return amounting(new_x, new_y) * x;
    }
}

int lastDigit(int a, int b) {
    return (a * b) % 10;
}


function<double(double)> derivative(function<double(double)> f, double h = 0.0001) {
    return [f, h](double x) {
        return (f(x + h) - f(x - h)) / (2.0 * h);
    };
}







long findVertexX(function<double(double)> f, double initialGuess, double tol = 1e-6) {
    // Get derivative
    auto df = derivative(f);
    
    double x = initialGuess;
    int maxIter = 100;
    double h = 0.0001;
    
    for (int i = 0; i < maxIter; i++) {
        double dfx = df(x);
        
        // Found critical point where derivative = 0
        if (abs(dfx) < tol) {
            return (long)round(x);  // Convert to long and return
        }
        
        // Numerical second derivative
        double d2fx = (df(x + h) - df(x - h)) / (2.0 * h);
        
        if (abs(d2fx) < 1e-10) {
            break;
        }
        
        // Newton-Raphson step
        x = x - dfx / d2fx;
    }
    
    return (long)round(x);  // Return best approximation
}







double sumOfList(vector<double> nums) {
    double sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
    }
    return sum;
}



double mean(vector<double> nums) {
    return (sumOfList(nums)) / (nums.size());
}


double average(double a, double b) {
    return (a + b) / 2;
}





double median(vector<double> nums) {
    long length = nums.size();
    if (length == 0) {
        return 0;
    }
    if (isSorted(nums)) {
        if (length % 2 == 1) {
            return nums[ceil(length / 2)];
        }
        else {
            return average(nums[length / 2 - 1], nums[length / 2]);
        }
    }
    else {
        selectionSort(nums);
        return median(nums);
    }
}

long frequency(vector<double> nums, double target) {
    long result = 0;
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == target) {
            result += 1;
        }
    }
    return result;
}




double midrange(vector<double> nums) { 
    return ((maximum(nums) + minimum(nums))) / 2;
}


double range(vector<double> nums) {
    return maximum(nums) - minimum(nums);
}


double variance(const std::vector<double>& nums, bool population) {
    double meanOfSet = mean(nums);
    double sum = 0.0;
    size_t size = nums.size();

    for (double x : nums) {
        double diff = x - meanOfSet;
        sum += diff * diff;
    }

    return population ? sum / size : sum / (size - 1);
}

double standardDeviation(const std::vector<double>& nums, bool population) {
    return std::sqrt(variance(nums, population));
}











// =========== DATA ========

// === Sorting =========

vector<double> selectionSort(vector<double> nums) {
    for (int i = 0; i < nums.size() - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < nums.size(); j++) {
            if (nums[j] < nums[minIndex]) {
                minIndex = j;
            }
        }

        double temp = nums[i];        // must be double
        nums[i] = nums[minIndex];
        nums[minIndex] = temp;
    }

    return nums;
}



bool isSorted(const vector<double>& nums) {
    // A list with 0 or 1 elements is always sorted
    if (nums.size() <= 1) {
        return true;
    }

    for (size_t i = 0; i < nums.size() - 1; i++) {
        if (nums[i] > nums[i + 1]) {
            return false;   
        }
    }

    return true;    
}


vector<double> pogoSort(vector<double> nums) {
    bool sorted = false;
    double max = maximum(nums);
    double min = maximum(nums);
    int length = nums.size();
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    
    std::cout << "Starting Pogo Sort..." << std::endl;
    long long attempts = 0;

    // Shuffle repeatedly until sorted
    while (!isSorted(nums)) {
        std::shuffle(nums.begin(), nums.end(), rng);
        attempts++;
    }

}








// ===== Searching ===== 

double linearSearch(vector<double> nums, double target) {
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == target) {
            cout << "The target " << target << " was found at index: " << i << endl;
            return 0;
        }
    }
    return 1;
}


int binarySearch(vector<double> nums, double target) {
    if (!isSorted(nums)) {
        nums = selectionSort(nums);
    }

    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (nums[mid] == target) {
            return mid;
        }
        else if (nums[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

double maximum(vector<double> nums) {
    double temp = nums[0];
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] > temp) {
            temp = nums[i];
        }
    }
    return temp;
}



double minimum(vector<double> nums) {
    double temp = nums[0];
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] < temp) {
            temp = nums[i];
        }
    }
    return temp;
}








// temporary test


int main() {
    auto f = [](double x) {
        return x * x;
    };
    auto df = derivative(f);
    vector<double> list = {1, 2, 3, 4};
    vector<double> list2 = {1, 9, 4, 5};
    cout << "The last digit of 7 * 9 is: " << lastDigit(7, 9) << endl;
    cout << "Searching for 1 in the list of 1, 2, 3, 4.." << endl;
    cout << linearSearch(list, 1) << endl;
    cout << "Sorting the list 1, 9, 4, 5.." << endl;
    vector<double> list3 = selectionSort(list2);
    cout << list3[0] << list3[1] << list3[2] << list3[3] << endl;
    cout << "The GCD of 12 and 8 is: " << gcd(12, 8) << endl;
    cout << "The derivative of x squared is: " << df(4) << " at 4." << endl;
}












