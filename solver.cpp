#include <iostream>
#include <cmath>
#include <complex>
#include <numbers>
#include <vector>
#include <limits>


// Variables and configs

namespace constants {
    // MATH
    const double pi = 3.141592653589793;
    const double e = 2.718281828459045;
    const double phi = 1.618033988749895;
    const double sqrt2 = 1.414213562373095;
    const double sqrt3 = 1.732050807568877;
    const double ln2 = 0.693147180559945;
    const double ln10 = 2.302585092994046;
    const double DEG_TO_RAD = pi / 180.0;
    const double RAD_TO_DEG = 180.0 / pi;

    // PHYSICS
    const double g = 9.80665;
    const double c = 299792458.0;
    const double G = 6.67430e-11;
    const double R = 8.314462618;
    const double Na = 6.02214076e23;
    const double qe = 1.602176634e-19;

    // ENGINEERING
    const double EPSILON = 1e-9;
    const double AIR_DENSITY = 1.225;
    const double WATER_DENSITY = 1000.0;
}






using namespace std;




// === Headers ===========
void lineqsol(float a, float b);
void quadraticFormula(float a, float b, float c);
float discriminant(float a, float b, float c);
void cubicSolver(float a, float b, float c, float d);
double expLogTools(double base, double exponent);
float absValue(float x);

float areaCircle(float r);
float circumference(float r);
float pythagorean(float a, float b);
float distanceFormula(float x1, float y1, float x2, float y2);
void midpoint(float x1, float y1, float x2, float y2);
float volumeSphere(float r);
float surfaceCube(float s);

float trigBasic(char func, float x);
float trigReciprocal(char func, float x);
float trigInverse(char func, float x);
void lawOfSines(float a, float A, float B);
float degToRad(float deg);
float radToDeg(float rad);

double derivative(double (*f)(double), double x);
double integral(double (*f)(double), double a, double b, int n);
double limit(double (*f)(double), double x0);
double areaUnder(double (*f)(double), double a, double b);

double arithmeticTerm(double a1, double d, int n);
double geometricTerm(double a1, double r, int n);
double arithmeticSum(double a1, double d, int n);
double geometricSum(double a1, double r, int n);
void fibonacci(int terms);

long long factorial(int n);
long long nCr(int n, int r);
long long nPr(int n, int r);
void pascalTriangle(int row);
double probability(double success, double trials);

double ohmsLaw(double I, double R);
double force(double m, double a);
double kineticEnergy(double m, double v);
double potentialEnergy(double m, double h);
double workDone(double F, double d);
double powerCalc(double W, double t);
double pressure(double F, double A);

void rootTheory(double x);
void formulaAmounting(double x, double y);
void testAnswerChangeTheory();
void eliminationMethodTheory();
void variantLaw();
void sanityCorrelationSystem(double sanityLevel);
void perceptionTheory();
void butterflyTheoremParadox();
void randomFunFeatures();

// === MAIN PROGRAM ===
int main() {
    cout << "===== FORMULA SOLVER =====" << endl;
    cout << "1: Algebra" << endl;
    cout << "2: Geometry" << endl;
    cout << "3: Trigonometry" << endl;
    cout << "4: Calculus" << endl;
    cout << "5: Sequences And Series" << endl;
    cout << "6: Combinatorics" << endl;
    cout << "7: Physics And Engineering" << endl;
    cout << "8: Other" << endl;
    cout << "0: Exit" << endl;

    int choice;
    cout << "Choose one: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer

    // === ALGEBRA ===
    if (choice == 1) {
        cout << "===== ALGEBRA MENU =====" << endl;
        cout << "1. Linear Equation Solver" << endl;
        cout << "2. Quadratic Formula" << endl;
        cout << "3. Discriminant Checker" << endl;
        cout << "4. Cubic Equation Solver" << endl;
        cout << "5. Exponent / Logarithm Tools" << endl;
        cout << "6. Absolute Value" << endl;
        cout << "7. Back" << endl;

        int sub; cin >> sub;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer
        if (sub == 1) { float a,b; cin>>a>>b; lineqsol(a,b); }
        else if (sub == 2) { float a,b,c; cin>>a>>b>>c; quadraticFormula(a,b,c); }
        else if (sub == 3) { float a,b,c; cin>>a>>b>>c; discriminant(a,b,c); }
        else if (sub == 4) { float a,b,c,d; cin>>a>>b>>c>>d; cubicSolver(a,b,c,d); }
        else if (sub == 5) { double base,exp; cin>>base>>exp; expLogTools(base,exp); }
        else if (sub == 6) { float x; cin>>x; absValue(x); }
    }

    // === GEOMETRY ===
    else if (choice == 2) {
        cout << "===== GEOMETRY MENU =====" << endl;
        cout << "1. Area Of Circle" << endl;
        cout << "2. Circumference" << endl;
        cout << "3. Pythagorean Theorem" << endl;
        cout << "4. Distance Formula" << endl;
        cout << "5. Midpoint Formula" << endl;
        cout << "6. Volume / Surface Area" << endl;
        cout << "7. Back" << endl;

        int sub; cin >> sub;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer
        if (sub == 1) { float r; cin>>r; areaCircle(r); }
        else if (sub == 2) { float r; cin>>r; circumference(r); }
        else if (sub == 3) { float a,b; cin>>a>>b; pythagorean(a,b); }
        else if (sub == 4) { float x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2; distanceFormula(x1,y1,x2,y2); }
        else if (sub == 5) { float x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2; midpoint(x1,y1,x2,y2); }
        else if (sub == 6) { float r,s; cin>>r>>s; volumeSphere(r); surfaceCube(s); }
    }

    // === TRIGONOMETRY ===
    else if (choice == 3) {
        cout << "===== TRIGONOMETRY MENU =====" << endl;
        cout << "1. sin/cos/tan" << endl;
        cout << "2. sec/csc/cot" << endl;
        cout << "3. Inverse Trig" << endl;
        cout << "4. Law of Sines" << endl;
        cout << "5. Degree↔Radian" << endl;
        cout << "6. Back" << endl;

        int sub; cin >> sub;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer
        if (sub == 1) { char f; float x; cin>>f>>x; trigBasic(f,x); }
        else if (sub == 2) { char f; float x; cin>>f>>x; trigReciprocal(f,x); }
        else if (sub == 3) { char f; float x; cin>>f>>x; trigInverse(f,x); }
        else if (sub == 4) { float a,A,B; cin>>a>>A>>B; lawOfSines(a,A,B); }
        else if (sub == 5) { float v; cin>>v; degToRad(v); radToDeg(v); }
    }

    // === CALCULUS ===
    else if (choice == 4) {
        cout << "===== CALCULUS MENU =====" << endl;
        cout << "1. Derivative" << endl;
        cout << "2. Integral" << endl;
        cout << "3. Limit" << endl;
        cout << "4. Area Under Curve" << endl;
        cout << "5. Back" << endl;

        int sub; cin >> sub;
        if (sub == 1) cout << "derivative(...)" << endl;
        else if (sub == 2) cout << "integral(...)" << endl;
        else if (sub == 3) cout << "limit(...)" << endl;
        else if (sub == 4) cout << "areaUnder(...)" << endl;
    }

    // === SEQUENCES & SERIES ===
    else if (choice == 5) {
        cout << "===== SEQUENCES & SERIES =====" << endl;
        cout << "1. Arithmetic Term" << endl;
        cout << "2. Geometric Term" << endl;
        cout << "3. Arithmetic Sum" << endl;
        cout << "4. Geometric Sum" << endl;
        cout << "5. Fibonacci" << endl;
        cout << "6. Back" << endl;

        int sub; cin >> sub;
        if (sub == 1) { double a1,d; int n; cin>>a1>>d>>n; arithmeticTerm(a1,d,n); }
        else if (sub == 2) { double a1,r; int n; cin>>a1>>r>>n; geometricTerm(a1,r,n); }
        else if (sub == 3) { double a1,d; int n; cin>>a1>>d>>n; arithmeticSum(a1,d,n); }
        else if (sub == 4) { double a1,r; int n; cin>>a1>>r>>n; geometricSum(a1,r,n); }
        else if (sub == 5) { int t; cin>>t; fibonacci(t); }
    }

    // === COMBINATORICS ===
    else if (choice == 6) {
        cout << "===== COMBINATORICS =====" << endl;
        cout << "1. Factorial" << endl;
        cout << "2. nCr" << endl;
        cout << "3. nPr" << endl;
        cout << "4. Pascal Triangle" << endl;
        cout << "5. Probability" << endl;
        cout << "6. Back" << endl;

        int sub; cin >> sub;
        if (sub == 1) { int n; cin>>n; factorial(n); }
        else if (sub == 2) { int n,r; cin>>n>>r; nCr(n,r); }
        else if (sub == 3) { int n,r; cin>>n>>r; nPr(n,r); }
        else if (sub == 4) { int row; cin>>row; pascalTriangle(row); }
        else if (sub == 5) { double s,t; cin>>s>>t; probability(s,t); }
    }

    // === PHYSICS ===
    else if (choice == 7) {
        cout << "===== PHYSICS =====" << endl;
        cout << "1. Ohm's Law" << endl;
        cout << "2. Force" << endl;
        cout << "3. Kinetic Energy" << endl;
        cout << "4. Potential Energy" << endl;
        cout << "5. Work/Power/Pressure" << endl;
        cout << "6. Back" << endl;

        int sub; cin >> sub;
        if (sub == 1) { double I,R; cin>>I>>R; ohmsLaw(I,R); }
        else if (sub == 2) { double m,a; cin>>m>>a; force(m,a); }
        else if (sub == 3) { double m,v; cin>>m>>v; kineticEnergy(m,v); }
        else if (sub == 4) { double m,h; cin>>m>>h; potentialEnergy(m,h); }
        else if (sub == 5) { double F,d; cin>>F>>d; workDone(F,d); }
    }

    // === OTHER ===
    else if (choice == 8) {
        cout << "===== OTHER =====" << endl;
        cout << "1. Root Theory" << endl;
        cout << "2. Formula Amounting" << endl;
        cout << "3. Test Answer Change Theory" << endl;
        cout << "4. Elimination Method Theory" << endl;
        cout << "5. Variant Law" << endl;
        cout << "6. Sanity Correlation System" << endl;
        cout << "7. Perception Theory" << endl;
        cout << "8. Butterfly Theorem Paradox" << endl;
        cout << "9. Random Fun Features" << endl;
        cout << "10. Back" << endl;

        int sub; cin >> sub;
        if (sub == 1) { double x; cin>>x; rootTheory(x); }
        else if (sub == 2) { double x,y; cin>>x>>y; formulaAmounting(x,y); }
        else if (sub == 3) testAnswerChangeTheory();
        else if (sub == 4) eliminationMethodTheory();
        else if (sub == 5) variantLaw();
        else if (sub == 6) { double s; cin>>s; sanityCorrelationSystem(s); }
        else if (sub == 7) perceptionTheory();
        else if (sub == 8) butterflyTheoremParadox();
        else if (sub == 9) randomFunFeatures();
    }

    else if (choice == 0) {
        cout << "Exiting Formula Solver..." << endl;
    }

    else {
        cout << "Invalid option. Try again." << endl;
    }

    return 0;
}



void lineqsol(float a, float b) {
    float x; // declare once at the top so it's visible everywhere

    if (a == 0) {
        if (b == 0)
            cout << "Infinite solutions (0x = 0)" << endl;
        else
            cout << "No solution (0x = " << b << ")" << endl;
        return; // exit early
    }

    if (b > 0) {
        x = -1 * (b / a);
    }
    else if (b == 0) {
        x = 0;
    }
    else {
        x = b / a;
    }

    cout << "x = " << x << endl;
}

void quadraticFormula(float a, float b, float c) {
    float quadratic1 = (-b + sqrt(b*b - 4*a*c)) / (2*a);
    float quadratic2 = (-b - sqrt(b*b - 4*a*c)) / (2*a);
    cout << "Solution 1: " << quadratic1 << endl;
    cout << "Solution 2: " << quadratic2 << endl;
}


float discriminant(float a, float b, float c) {
    float discriminant = b*b - (4 * a * c);
    return discriminant;
}

void cubicSolver(float a, float b, float c, float d) {
    if (a == 0) {
        cout << "Not a cubic equation. Try quadratic instead." << endl;
        return;
    }

    // Convert to depressed cubic: x = t - b/(3a)
    float p = (3*a*c - b*b) / (3*a*a);
    float q = (2*b*b*b - 9*a*b*c + 27*a*a*d) / (27*a*a*a);

    float discriminant = (q*q / 4) + (p*p*p / 27);

    cout << "Discriminant = " << discriminant << endl;

    if (discriminant > 0) {
        // One real root
        float u = cbrt(-q/2 + sqrt(discriminant));
        float v = cbrt(-q/2 - sqrt(discriminant));
        float x1 = u + v - (b / (3*a));
        cout << "One real root: x = " << x1 << endl;
    }
    else if (discriminant == 0) {
        // Multiple real roots
        float u = cbrt(-q/2);
        float x1 = 2*u - (b / (3*a));
        float x2 = -u - (b / (3*a));
        cout << "Multiple real roots: x1 = " << x1 << ", x2 = " << x2 << endl;
    }
    else {
        // Three real roots (complex cube roots)
        float r = sqrt(-p*p*p / 27);
        float phi = acos(-q / (2 * sqrt(-p*p*p / 27)));
        float x1 = 2 * sqrt(-p/3) * cos(phi/3) - b/(3*a);
        float x2 = 2 * sqrt(-p/3) * cos((phi + 2*M_PI)/3) - b/(3*a);
        float x3 = 2 * sqrt(-p/3) * cos((phi + 4*M_PI)/3) - b/(3*a);

        cout << "Three real roots:" << endl;
        cout << "x1 = " << x1 << endl;
        cout << "x2 = " << x2 << endl;
        cout << "x3 = " << x3 << endl;
    }
}


double expLogTools(double base, double exponent) {
    double result = pow(base, exponent);
    cout << base << "^" << exponent << " = " << result << endl;
    return result;
}



float absValue(float x) {
    return (x < 0) ? -x : x;
}
float areaCircle(float r) {
    return constants::pi * (r * r);
}

float circumference(float r) {
    return 2 * constants::pi * r;
}

float pythagorean(float a, float b) {
    return sqrt((a * a) + (b * b));
}

float distanceFormula(float x1, float y1, float x2, float y2) {
    return sqrt(((y2 - y1) * (y2 - y1))) + ((x2 - x1) * (x2 - x1));
}

void midpoint(float x1, float y1, float x2, float y2) {
    float midX = (x1 + x2) / 2.0;
        float midY = (y1 + y2) / 2.0;
        cout << "Midpoint between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ") = ("
             << midX << ", " << midY << ")" << endl;
}

float volumeSphere(float r) {
    return (4.0 / 3.0) * constants::pi * (r * r * r);
}


float surfaceCube(float s) {
    return 6 * (s * s);
}

float trigBasic(char func, float x) {
    if (func == 's') {
        cout << "sin(" << x << ") = " << sin(x) << endl;
        return sin(x);
    }
    else if (func == 'c') {
        cout << "cos(" << x << ") = " << cos(x) << endl;
        return cos(x);
    }
    else if (func == 't') {
        cout << "tan(" << x << ") = " << tan(x) << endl;
        return tan(x);
    }
    else {
        cout << "Invalid trig function. Use s, c, or t." << endl;
        return 0;
    }
}

float trigReciprocal(char func, float x) {
    if (func == 's') { // csc(x) = 1/sin(x)
        if (sin(x) == 0) {
            cout << "csc(" << x << ") is undefined (division by zero)." << endl;
            return 0;
        }
        float result = 1 / sin(x);
        cout << "csc(" << x << ") = " << result << endl;
        return result;
    }
    else if (func == 'c') { // sec(x) = 1/cos(x)
        if (cos(x) == 0) {
            cout << "sec(" << x << ") is undefined (division by zero)." << endl;
            return 0;
        }
        float result = 1 / cos(x);
        cout << "sec(" << x << ") = " << result << endl;
        return result;
    }
    else if (func == 't') { // cot(x) = 1/tan(x)
        if (tan(x) == 0) {
            cout << "cot(" << x << ") is undefined (division by zero)." << endl;
            return 0;
        }
        float result = 1 / tan(x);
        cout << "cot(" << x << ") = " << result << endl;
        return result;
    }
    else {
        cout << "Invalid reciprocal trig function. Use s (csc), c (sec), or t (cot)." << endl;
        return 0;
    }
}

float trigInverse(char func, float x) {
    if (func == 's') {
        cout << "arcsin(" << x << ") = " << asin(x) << " radians" << endl;
        return asin(x);
    }
    else if (func == 'c') {
        cout << "arccos(" << x << ") = " << acos(x) << " radians" << endl;
        return acos(x);
    }
    else if (func == 't') {
        cout << "arctan(" << x << ") = " << atan(x) << " radians" << endl;
        return atan(x);
    }
    else {
        cout << "Invalid inverse trig function. Use s, c, or t." << endl;
        return 0;
    }
}


void lawOfSines(float a, float A, float B) {
    // A and B in radians
    float result = (a * sin(B)) / sin(A);
    cout << "Using Law of Sines:" << endl;
    cout << "Given side a = " << a << ", angle A = " << A << " rad, and angle B = " << B << " rad" << endl;
    cout << "Side b = " << result << endl;
}

float degToRad(float deg) {
    return deg * constants::DEG_TO_RAD;
}

float radToDeg(float rad) {
    return rad * constants::RAD_TO_DEG;
}

double derivative(double (*f)(double), double x) {
    double h = 1e-6; // small change in x
    return (f(x + h) - f(x - h)) / (2 * h); // centered difference formula
}

double integral(double (*f)(double), double a, double b, int n) {
    double dx = (b - a) / n;   // width of each rectangle
    double sum = 0.0;          // total area

    for (int i = 0; i < n; i++) {
        double x = a + i * dx; // left endpoint
        sum += f(x) * dx;
    }

    return sum;
}


double limit(double (*f)(double), double x0) {
    double h = 1e-6; // initial step
    double prev_result = numeric_limits<double>::quiet_NaN();

    for (int i = 0; i < 12; ++i) { // refine up to 12 times (from 1e-6 down to ~1e-18)
        double left = f(x0 - h);
        double right = f(x0 + h);

        // If both sides are invalid, shrink h and retry
        if (isnan(left) || isnan(right) || isinf(left) || isinf(right)) {
            h /= 10.0;
            continue;
        }

        // Average left and right for smoother result
        double result = (left + right) / 2.0;

        // If value stabilized enough, return it
        if (!isnan(prev_result) && fabs(result - prev_result) < 1e-10) {
            return result;
        }

        prev_result = result;
        h /= 10.0; // get closer
    }

    // If nothing worked, try a fallback using nearby average
    double near_left = f(x0 - 1e-12);
    double near_right = f(x0 + 1e-12);
    if (!isnan(near_left) && !isnan(near_right))
        return (near_left + near_right) / 2.0;

    cout << "⚠️ Limit did not converge near x = " << x0 << endl;
    return numeric_limits<double>::quiet_NaN();
}



double areaUnder(double (*f)(double), double a, double b) {
    int n = 100000;              // number of subintervals
    double dx = (b - a) / n;     // width of each trapezoid
    double sum = 0.5 * (f(a) + f(b)); // endpoints count half

    for (int i = 1; i < n; ++i) {
        double x = a + i * dx;
        sum += f(x);
    }

    return sum * dx; // total area approximation
}



double arithmeticTerm(double a1, double d, int n) {
    return a1 + (n - 1) * d;
}



double geometricTerm(double a1, double r, int n) {
    return a1 * pow(r, (n - 1));
}

double arithmeticSum(double a1, double d, int n) {
    return (n / 2) * (2 * a1 + (n - 1) * d);
}
double geometricSum(double a1, double r, int n) {
    return (a1) * ((pow(r, n) - 1) / (r-1));
}

void fibonacci(int terms) {
    long long a = 0, b = 1, next;
    
    cout << "Fibonacci Sequence (" << terms << " terms): ";
    
    for (int i = 1; i <= terms; i++) {
        cout << a << " ";
        next = a + b;
        b = next;
    }
    cout << endl;
}

long long factorial(int n) {
    return tgamma(n + 1.0);
}

long long nCr(int n, int r) {
    if (r > n || n < 0 || r < 0) {
        return 1;
    }
    long long result = factorial(n) / (factorial(r) * factorial(n - r));
    return result;
}



long long nPr(int n, int r) {
    if (r > n || n < 0 || r < 0) {
        return 1;
    }
    long long result = factorial(n) / factorial(n - r);
    return result;
}


void pascalTriangle(int row) {
    cout << "Pascal's Triangle up to row " << row << ":" << endl;

    for (int i = 0; i < row; i++) {
        // spacing for shape
        for (int j = 0; j < row - i; j++) cout << " ";

        // each value = nCr(i, j)
        for (int j = 0; j <= i; j++) {
            cout << nCr(i, j) << " ";
        }
        cout << endl;
    }
}


double probability(double success, double trials) {
    if (trials == 0) {
        cout << "Error: trials cannot be zero." << endl;
        return 0;
    }
    double result = success / trials;
    cout << "Probability = " << result << endl;
    return result;
}


double ohmsLaw(double I, double R) {
    double V = I * R;
    cout << "Ohm's Law: V = " << V << " volts" << endl;
    return V;
}

double force(double m, double a) {
    double F = m * a;
    cout << "Force: F = " << F << " N (Newtons)" << endl;
    return F;
}

double kineticEnergy(double m, double v) {
    double KE = 0.5 * m * v * v;
    cout << "Kinetic Energy: KE = " << KE << " J (Joules)" << endl;
    return KE;
}

double potentialEnergy(double m, double h) {
    const double g = 9.80665; // standard gravity
    double PE = m * g * h;
    cout << "Potential Energy: PE = " << PE << " J (Joules)" << endl;
    return PE;
}

double workDone(double F, double d) {
    double W = F * d;
    cout << "Work Done: W = " << W << " J (Joules)" << endl;
    return W;
}

double powerCalc(double W, double t) {
    if (t == 0) {
        cout << "Error: Time cannot be zero." << endl;
        return 0;
    }
    double P = W / t;
    cout << "Power: P = " << P << " W (Watts)" << endl;
    return P;
}

double pressure(double F, double A) {
    if (A == 0) {
        cout << "Error: Area cannot be zero." << endl;
        return 0;
    }
    double P = F / A;
    cout << "Pressure: P = " << P << " Pa (Pascals)" << endl;
    return P;
}

void rootTheory(double x) {
    try {
        double result = x + sqrt(x);
        cout << "Result: " << result << endl;
    }
    catch (const exception& e) {
        // maybe negative values or data errors
        cout << "Error: " << e.what() << endl;
    }
}

void formulaAmounting(double x, double y) {
    if (x < 0 || y < 0 || x == 0 || y == 0) {
        cout << "Error during runtime: Inputs are weak." << endl;
        return;
    }

    // Even exponent case
    if (fmod(y, 2) == 0) {
        y /= 2;
        x = x * x;
        cout << "Simplified: " << x << "^" << y << endl;
    } 
    // Odd exponent case
    else {
        double half = floor(y / 2.0);
        cout << "Simplified: " << (x * x) << "^" << half << " * " << sqrt(x) << endl;
    }
}


void testAnswerChangeTheory() {
    cout << "If you are on a test, and you pick an answer choice, then instead choose another one, it is likely your first choice will be correct." << endl;
    cout << "Lesson: Trust your first choice and your gut." << endl;
}

void eliminationMethodTheory() {
    cout << "=== Elimination Method Theory ===" << endl;
    cout << "When unsure, eliminate impossible choices first." << endl;
    cout << "Mathematically proven to raise accuracy by removing noise." << endl;
}

void variantLaw() {
    cout << "=== Variant Law ===" << endl;
    cout << "Randomness isn't absence of pattern — it's repetition disguised as chaos." << endl;
    cout << "When picking with a small number, let's say n - amount of numbers, always go through a Turing Algorithm sub1 in your head to maximize efficiency and timing." << endl;
    cout << "Keep in mind the Pattern Avoidness, the Chance Theories, and the Solution Thought. This will help you choose between many choices fast and efficient, having a failure rate of ~18%." << endl;
    cout << "Example: Set: (1, 2, 3, 4) Previous: (1, 2) Sub 1: When choosing an option, choose 4. (1, 2, 4, 3). Using the Solution Thought and the Pattern Avoidness." << endl;
}


void sanityCorrelationSystem(double sanityLevel) {
    cout << "=== Sanity Correlation System ===" << endl;
    cout << "Sanity level: " << sanityLevel << "%" << endl;
    if (sanityLevel > 70)
        cout << "Stable." << endl;
    else if (sanityLevel > 30)
        cout << "Fluctuating. Watch for sanity strikes." << endl;
    else
        cout << "Unstable. Correlation lost — system breakdown likely." << endl;

}

void perceptionTheory() {
    string text = "There is a straight line from point A to B. Imagine a small object going from object A to B to A to.. at a certain speed. Now, our brains only perceive things with a little of inaccuracy. If a human is to look at one of these objects with a speed >= to the Perception Constant(180 meters per sec), it'll become blurry or not even appear! This is because of the Perception Theory.";
    cout << text << endl;
}

void butterflyTheoremParadox() {
    cout << "=== Butterfly Theorem Paradox ===" << endl;
    cout << "Just because everyone believes something is true doesn't make it true." << endl;
    cout << "Example: 'If everyone says the Earth is flat, it's still round.'" << endl;
}

void randomFunFeatures() {
    cout << "=== Random Fun Features ===" << endl;
    srand(time(0));
    int random = rand() % 5;
    switch (random) {
        case 0: cout << "💡 Random fact: The square root of 2 is irrational." << endl; break;
        case 1: cout << "⚡ Science says: Energy can't be created or destroyed." << endl; break;
        case 2: cout << "🎲 Random number: " << rand() % 100 << endl; break;
        case 3: cout << "🧠 Tip: Code like you're discovering physics." << endl; break;
        default: cout << "🌌 Quote: 'Knowledge is the universe looking at itself.'" << endl; break;
    }
}


