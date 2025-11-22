#include <iostream>
#include <cmath>
#include <cstdlib> 
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <random>
#include <filesystem>
#include <array>
#include <curl/curl.h>
#include <json/json.h>
#include <nlohmann/json.hpp>
#include <optional> 
#include <deque>
#include <map>
#include <string>
#include <ios>











// Also, sorry about the clear and terminal inputs, they only work for Mac and Linux. 😅
// Honestly, I was too lazy. But could you blame me if you made the 1000 lines of code?

// TODO: omg this is my first todo list


using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::map;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::stoi;
using std::stod;
using std::stringstream;
using std::istringstream;
using std::ostringstream;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::time;
using std::pair;
using std::make_pair;
using std::max;
using std::min;


using namespace std;

const string PACKAGE_DB = "packages.db";     // This is important.. yeah.



size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}


struct Task {
    string text;
    bool done;
    
    Task(string t) : text(t), done(false) {}
    
    string toString() {
        std::string status = done ? "[x]" : "[ ]";
        return status + " " + text;
    }
};


string decimalToBase(long number, int base) {
    if (number == 0) {
        return "0";
    }

    string result = "";
    string digits = "0123456789ABCDEF";

    while (number > 0) {
        int remainder = number % base;
        result = digits[remainder] + result;   
        number = number / base;
    }

    return result;
}



vector<string> permission_levels = {"Guest", "User", "Admin", "Co-owner", "Owner"}; // Don't change this.. or change it 🤷


string permission_default = "User";   

int indexOfManualPermission = 0;    // the index of the permission. For example, a index of 5 means that the permission is the 4th element of the list of permission levels.

string manualPermission = permission_levels[indexOfManualPermission - 1];



map<string, string> aliases;
map<string, string> env_vars;

void load_deltacore() {
    string home = getenv("HOME");
    string deltacore_path = home + "/.deltacore";
    ifstream config(deltacore_path);
    if (!config.is_open()) {
        // First time - create default config
        ofstream new_config(deltacore_path);
        new_config << "# DeltaOne Configuration File" << endl;
        new_config << "# Aliases: alias name='command'" << endl;
        new_config << "# Variables: export VAR='value'" << endl;
        new_config << endl;
        new_config << "# Example:" << endl;
        new_config << "# alias ll='ls -la'" << endl;
        new_config.close();
        std::cout << "✓ Created ~/.deltacore config file" << endl;
        return;
    }
    
    string line;
    while (getline(config, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;
        
        // Parse alias: alias name='command'
        if (line.rfind("alias ", 0) == 0) {
            size_t eq = line.find('=');
            if (eq != string::npos) {
                string name = line.substr(6, eq - 6);
                string cmd = line.substr(eq + 1);
                
                // Remove quotes
                if (cmd.size() >= 2 && cmd.front() == '\'' && cmd.back() == '\'') {
                    cmd = cmd.substr(1, cmd.size() - 2);
                }
                
                aliases[name] = cmd;
                std::cout << "Loaded alias: " << name << " -> " << cmd << endl;
            }
        }
        
        // Parse export: export VAR='value'
        else if (line.rfind("export ", 0) == 0) {
            size_t eq = line.find('=');
            if (eq != string::npos) {
                string key = line.substr(7, eq - 7);
                string value = line.substr(eq + 1);
                
                // Remove quotes
                if (value.size() >= 2 && value.front() == '\'' && value.back() == '\'') {
                    value = value.substr(1, value.size() - 2);
                }
                
                env_vars[key] = value;
                std::cout << "Loaded var: " << key << " = " << value << endl;
            }
        }
    }
    
    config.close();
}




// Global todo list
std::vector<Task> todo_list;


void delete_task(std::vector<Task>& list, int index) {
    if (index < 0 || index >= list.size()) {
        std::cout << "Invalid index\n";
        return;
    }
    list.erase(list.begin() + index);
}





vector<string> load_packages() {
    vector<string> packages;
    ifstream file(PACKAGE_DB);
    string line;

    while (getline(file, line)) {
        if (!line.empty()) packages.push_back(line);
    }
    return packages;
}

// Write updated package list
void save_packages(const vector<string>& packages) {
    ofstream file(PACKAGE_DB, std::ios::trunc);

    for (const auto& pkg : packages) {
        file << pkg << endl;
    }
}

// INSTALL COMMAND
void handle_install(const string& package_name) {
    vector<string> packages = load_packages();

    if (find(packages.begin(), packages.end(), package_name) != packages.end()) {
        cout << "❌ Already installed: " << package_name << endl;
        return;
    }

    packages.push_back(package_name);
    save_packages(packages);

    cout << "✅ Installed: " << package_name << endl;
}

// REMOVE COMMAND
void handle_remove(const string& package_name) {
    vector<string> packages = load_packages();

    auto it = find(packages.begin(), packages.end(), package_name);

    if (it == packages.end()) {
        cout << "❌ Not installed: " << package_name << endl;
        return;
    }

    packages.erase(it);
    save_packages(packages);

    cout << "🗑️ Removed: " << package_name << endl;
}

// LIST COMMAND
void handle_list() {
    vector<string> packages = load_packages();

    if (packages.empty()) {
        cout << "📦 No packages installed." << endl;
        return;
    }

    cout << "📦 Installed Packages:" << endl;
    for (const auto& p : packages) {
        cout << "- " << p << endl;
    }
}



// Important stuff and declarations

vector<string> board = {
    " ", " ", " ",
    " ", " ", " ",
    " ", " ", " "
};


vector<vector<int>> WIN_PATTERNS = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6}
};


vector<vector<char>> wonRPSlist = {
    {'R', 'S'},
    {'P', 'R'},
    {'S', 'P'}
};


bool wonRockPaperScissors(char input, char output) {
    bool won = false;   // default = lose

    for (int i = 0; i < 3; i++) {
        if (wonRPSlist[i][0] == input && wonRPSlist[i][1] == output) {
            won = true;
            break;   
        }
    }

    return won;
}









void displayBoard(const vector<string>& board) {
    cout << "-------------\n";
    for (int r = 0; r < 3; r++) {
        cout << "| ";
        for (int c = 0; c < 3; c++) {
            cout << board[r][c] << " | ";
        }
        cout << "\n-------------\n";
    }
}


bool isInList(vector<double> nums, double target) {     
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == target) {
            return true;
        }
    }
    return false;
}


vector<string> history_list;
const size_t MAX_HISTORY = 100;     // Metric for max history entries

const string processTask = "DahleTask";       // Change this string for the input 'process' to output a process like "Task Manager" or "DahleCore" (fake)
const string processStats = "STATS: GREAT, 1.2% RAM FILLED";   // Change the string for the fake stats

auto start_time = std::chrono::steady_clock::now();


// === Global Variables ===
vector<string> operators_calc = {"+", "-", "*", "/", "mod", "**", "^", "//", "==", "!=", ">", "<", ">=", "<=", "&&", "OR", "NOT"};
// truncated or floor division

float delayShutSec = 1.2;    // Metric for the amount of delay when shutting down for realisticness


// === Functions ===

// Finds the username@hostname
string getUserHost() {
    const char* user = getenv("USER");
    if (user == nullptr) user = "unknown";

    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    string host = hostname;
    // Remove ".lan" or anything after first dot
    size_t dotPos = host.find('.');
    if (dotPos != string::npos) {
        host = host.substr(0, dotPos);
    }

    return string(user) + "@" + host;
}
// == Floor Function =====

double floor1(double x) {
    int i = (int)x;
    if (x < 0 && x != i) i -= 1;
    return (double)i;
}

int randomnum(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

// === WEATHER SUPPORT ===


using json = nlohmann::json;

std::string http_get(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string buffer;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "DeltaOne/1.0");
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return buffer;
}

// Convert city to coordinates
struct Geo {
    double lat;
    double lon;
    std::string name;
    std::string country;
};

Geo get_city_coords(const std::string& city) {
    std::string cityEncoded = city;
    for (auto& c : cityEncoded) if (c == ' ') c = '+';
    std::string url = "https://geocoding-api.open-meteo.com/v1/search?count=1&language=en&format=json&name=" + cityEncoded;

    std::string response = http_get(url);
    json j = json::parse(response, nullptr, false);
    Geo g{0, 0, "Unknown", "??"};
    if (!j.is_discarded() && j.contains("results") && !j["results"].empty()) {
        auto r = j["results"][0];
        g.lat = r.value("latitude", 0.0);
        g.lon = r.value("longitude", 0.0);
        g.name = r.value("name", city);
        g.country = r.value("country", "??");
    }
    return g;
}

// Get current weather
void get_weather(const std::string& city) {
    Geo g = get_city_coords(city);
    if (g.lat == 0 && g.lon == 0) {
        std::cout << "City not found: " << city << std::endl;
        return;
    }

    std::string url = "https://api.open-meteo.com/v1/forecast?latitude=" + std::to_string(g.lat)
                    + "&longitude=" + std::to_string(g.lon)
                    + "&current=temperature_2m,weather_code&timezone=auto";

    std::string response = http_get(url);
    json j = json::parse(response, nullptr, false);

    if (j.is_discarded() || !j.contains("current")) {
        std::cout << "Failed to get weather data.\n";
        return;
    }

    double temp = j["current"].value("temperature_2m", 0.0);
    int code = j["current"].value("weather_code", -1);
    std::string desc = "Unknown";
    if (code == 0) desc = "Clear";
    else if (code == 1 || code == 2) desc = "Partly Cloudy";
    else if (code == 3) desc = "Overcast";
    else if (code >= 51 && code <= 67) desc = "Rain/Drizzle";
    else if (code >= 71 && code <= 77) desc = "Snow";
    else if (code >= 80 && code <= 82) desc = "Rain showers";
    else if (code >= 95 && code <= 99) desc = "Thunderstorms";

    std::cout << g.name << ", " << g.country << "\n"
              << "Temp: " << temp << " °C | " << desc << "\n";
}


bool dev_mode = true;      // Dev Mode!!!!!!
const string secret_path = "Vjbdsfjbhsdfbjsadkfjbsadfbasjfjbdsjkbdfe(&8$:+(#9&:#(-";


void mark_as_done(std::vector<Task>& list, int index) {
    if (index < 0 || index >= list.size()) {
        std::cout << "Invalid index\n";
        return;
    }
    list[index].done = true;
}




// Help menu
void help_menu() {
    cout << "=== Help ==========" << endl;
    cout << "Welcome to DeltaOne! It's a little project I made in 9 yrs old. It's supposed to feel like Arch Linux." << endl;
    cout << "You can type different commands. Tip: Type exit to exit" << endl;
    cout << "Maybe added installations and linking 😅 I didn't finish it as of now." << endl;
    cout << "Well, that's it! Have fun and give feedback on DahleChat." << endl;
    cout << "Okay, you probably wondered what Dahle is. It's a language I made up. So I put it in my projects because it sounds cool." << endl;
    cout << "- Dionysios Pei-Chen Yi, 9 yrs old, 2016-???" << endl;
}

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n == 2 || n == 3) {
        return true;
    }
    if (n % 2 == 0) {
        return false;   // Even numbers >2 are not prime.
    }
    // Test divisbility for 3 to sqrt(n)
    for (int i = 3; i <= floor(sqrt(n)); i += 2) {
        if (n % i == 0) {
            return false;    // In this case there is a divisor found, therefore it is not prime
        }
    }
    return true;     // If n was not prime, it would already be returned so we can put this here
}










// === Main ===
int main() {
    load_deltacore();
    string atIdentity = getUserHost();   // Finds the hostname using getUserHost()

    cout << "=== DeltaOne ======" << endl;
    cout << "\033[1;36m\"The spark in your soul only comes out when you do.\"\033[0m\n" << endl;
    
    // We will loop the input/output function using a while true loop. This loop can be broken with the input 'exit'.

    ifstream fin("history.txt");
    string line;
    while (getline(fin, line)) history_list.push_back(line);
    fin.close();

    // ALIASES!!!!!    :)
    while (true) {
        cout << "DeltaOne > ";
        string input;
        getline(cin, input);  // Allows spaces in the string
        if (!input.empty()) {
            history_list.push_back(input);
            if (history_list.size() > MAX_HISTORY)
                history_list.erase(history_list.begin()); // remove oldest
        }
        string original_input = input;
        if (aliases.find(input) != aliases.end()) {
            cout << "Running alias: " << aliases[input] << endl;
            input = aliases[input];
        }
        // Branches of input/output flows

        if (input == "exit") {
            cout << "Exiting DeltaOne..." << endl;
            std::this_thread::sleep_for(std::chrono::duration<double>(delayShutSec)); // sleeps for the metric delayShutSec seconds.
            break; // stop loop
        }
        else if (input.rfind("echo ", 0) == 0) {                // Detects if the user typed "echo"
            string message = input.substr(5);

            // Remove quotes 
            if (message.size() >= 2 && 
            ((message.front() == '"' && message.back() == '"') || 
            (message.front() == '\'' && message.back() == '\''))) {
            message = message.substr(1, message.size() - 2);
        }
            cout << message << endl;
        }
        else if (input == "force") {
            cout << "Forcing Exit Code FRCE.." << endl;
            break;
        }
        else if (input == "clear") {
            cout << "Note: Clearing..";
            system("clear");
        }
        else if (input == "whoami") {
            cout << getUserHost() << endl;
            cout << "You are also a " << manualPermission << ". This means that: " << endl;
            cout << ("You cannot delete or modify any sensitive files or mess with anything.\n" ? manualPermission = "Guest" || "User" : "");
            cout << ("You can modify anything and run tests for any feature.\n" ? manualPermission = "Co-owner" || "Owner" : "");
            cout << ("You are permitted to do anything except mess with the code or harm sensitive info.\n" ? manualPermission = "Admin" : "");                                                             
        }
        else if (input.rfind("sleep ", 0) == 0) {
            int seconds = stoi(input.substr(6));
            std::this_thread::sleep_for(std::chrono::duration<double>(delayShutSec));
            cout << "(slept for " << seconds << "s)" << endl;
        }
        else if (input == "time") {
            time_t now = time(nullptr);
            tm *local = localtime(&now);
            char buffer[100];
            strftime(buffer, sizeof(buffer), "%A, %B %d, %Y %H:%M:%S", local);
            cout << "Local time: " << buffer << endl;
        }
        else if (input == "history") {
            cout << "=== Command History ===" << endl;
            for (int i = history_list.size() - 1; i >= 0; --i) {
                cout << history_list.size() - i << ": " << history_list[i] << endl;
            }
        }
        else if (input == "!!") {
            if (!history_list.empty()) {
                cout << "Re-running: " << history_list.back() << endl;
                input = history_list.back(); // reuse previous command
                continue; // jump to next loop with new input
            } else {
                cout << "No previous command." << endl;
            }
        }
        else if (input.size() > 1 && input[0] == '!') {
            int index = stoi(input.substr(1)) - 1;
            if (index >= 0 && index < history_list.size()) {
                cout << "Re-running: " << history_list[index] << endl;
                input = history_list[index];
                continue;
            } else {
                cout << "Invalid history index." << endl;
            }
        }
        else if (input.rfind("randomnum ", 0) == 0) {
            stringstream ss(input.substr(10));
            int min, max;
            ss >> min >> max;
            if (ss.fail()) {
                cout << "Usage: randomnum <min> <max>" << endl;
            } else {
                cout << "🎲 Random number: " << randomnum(min, max) << endl;
            }
        }
        else if (input.rfind("coderun ", 0) == 0) {
            string filename = input.substr(8);
            if (filename.empty()) {
                cout << "Usage: coderun <file.cpp>" << endl;
                continue;
            }

            if (filename.find(".cpp") == string::npos) {
                cout << "❌ Error: must provide a .cpp file." << endl;
                continue;
            }

            // Create an output name for compiled binary
            string outName = filename.substr(0, filename.find(".cpp")) + "_run";

            // Build the compile command (use -std=c++20 or whatever you like)
            string compileCmd = "g++ -std=c++20 \"" + filename + "\" -o \"" + outName + "\"";
            cout << "🔧 Compiling " << filename << "..." << endl;

            int compileStatus = system(compileCmd.c_str());
            if (compileStatus != 0) {
                cout << "Compilation failed." << endl;
                continue;
            }

            cout << "✅ Compilation successful. Running program..." << endl;
            cout << "======================" << endl;

            // Actually RUN the program
            string runCmd = "./" + outName;
            system(runCmd.c_str()); // <- THIS runs the program from inside DeltaOne!!

            cout << "======================" << endl;
            cout << "🏁 Program finished executing." << endl;
        }
        else if (input == "process") {
            cout << "==== Processor e94h2 ==" << endl;
            cout << "[MANDATORY] [RUNNING] " << processTask << endl;
            cout << "[RUNNING] " << processStats << endl;
        }
        else if (input.rfind("weather ", 0) == 0) {
            std::string city = input.substr(8);
            get_weather(city);
        }
        else if (input == "uptime") {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();

            int hours = elapsed / 3600;
            int minutes = (elapsed % 3600) / 60;
            int seconds = elapsed % 60;

            cout << "Uptime: ";
            if (hours > 0) cout << hours << "h ";
            if (minutes > 0 || hours > 0) cout << minutes << "m ";
            cout << seconds << "s" << endl;

            // Easter egg!! (Wait, it isn't Easter, is it?)
            if (elapsed > 10000) {
                cout << "A long time. Woah." << endl;
            }
        }
        else if (dev_mode && input == "set uptime") {
            cout << "Enter new uptime (seconds): ";
            long long new_time;
            cin >> new_time;
            start_time = std::chrono::steady_clock::now() - std::chrono::seconds(new_time);
            cout << "1" << endl;
            cin.ignore();
        }
        else if (input == secret_path) {
            dev_mode = true;
            cout << "Dev mode activated." << endl;
        }
        else if (input == "lab") {              
            cout << "===== LAB =====" << endl;
            cout << "1: Calculate Expression" << endl;
            cout << "2: Prime Factory" << endl;
            cout << "3: Function Intelligence Lab (FIL)" << endl;
            cout << "Choose: ";
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                string query;
                cout << "Enter your question or expression: ";
                getline(cin, query);
                cout << "🌐 Opening Google: " << query << endl;
                system(("open \"https://www.google.com/search?q=" + query + "\"").c_str());
            }      
            else if (choice == 2) {
                // PRIME FACTORY
                bool factory_running = true;
                while (factory_running) {
                    cout << "\n===== PRIME FACTORY =====" << endl;
                    cout << "1. Prime Analyzer        - Test if a number is prime\n";
                    cout << "2. Prime Generator       - Generate primes up to a limit\n";
                    cout << "3. Prime Decomposer      - Factor a number into primes\n";
                    cout << "4. Pattern Detector      - Find twin primes\n";
                    cout << "5. Prime Counter         - Count primes in range\n";
                    cout << "6. Exit Prime Factory\n";
                    cout << "----------------------------------\n";
                    cout << "Enter your choice (1-6): ";
                    
                    string lab_input;
                    getline(cin, lab_input);
                    
                    if (lab_input == "1") {
                        // PRIME ANALYZER
                        cout << "Enter a positive integer: ";
                        int n;
                        cin >> n;
                        cin.ignore();
                        
                        if (isPrime(n)) {
                            cout << "✓ The number " << n << " is PRIME." << endl;
                        }
                        else {
                            cout << "✗ The number " << n << " is NOT prime." << endl;
                        }
                    }
                    else if (lab_input == "2") {
                        // PRIME GENERATOR
                        cout << "Generate all primes up to: ";
                        int limit;
                        cin >> limit;
                        cin.ignore();
                        
                        if (limit < 2) {
                            cout << "No primes below 2." << endl;
                            continue;
                        }
                        
                        cout << "Primes up to " << limit << ":" << endl;
                        vector<int> primes;
                        
                        for (int i = 2; i <= limit; i++) {
                            if (isPrime(i)) {
                                primes.push_back(i);
                            }
                        }
                        
                        // Print primes nicely
                        for (int i = 0; i < primes.size(); i++) {
                            cout << primes[i];
                            if (i < primes.size() - 1) cout << ", ";
                            if ((i + 1) % 10 == 0) cout << endl;  // New line every 10
                        }
                        cout << endl;
                        cout << "Total: " << primes.size() << " primes found." << endl;
                    }
                    else if (lab_input == "3") {
                        // PRIME DECOMPOSER (Prime Factorization)
                        cout << "Enter a number to factorize: ";
                        int n;
                        cin >> n;
                        cin.ignore();
                        
                        if (n < 2) {
                            cout << "Cannot factorize numbers less than 2." << endl;
                            continue;
                        }
                        
                        cout << n << " = ";
                        vector<int> factors;
                        int temp = n;
                        
                        // Find all prime factors
                        for (int i = 2; i <= temp; i++) {
                            while (temp % i == 0) {
                                factors.push_back(i);
                                temp /= i;
                            }
                        }
                        
                        // Print factors
                        for (int i = 0; i < factors.size(); i++) {
                            cout << factors[i];
                            if (i < factors.size() - 1) cout << " × ";
                        }
                        cout << endl;
                    }
                    else if (lab_input == "4") {
                        // PATTERN DETECTOR (Twin Primes)
                        cout << "Find twin primes up to: ";
                        int limit;
                        cin >> limit;
                        cin.ignore();
                        
                        cout << "Twin primes (primes that differ by 2):" << endl;
                        int count = 0;
                        
                        for (int i = 2; i <= limit - 2; i++) {
                            if (isPrime(i) && isPrime(i + 2)) {
                                cout << "(" << i << ", " << (i + 2) << ")" << endl;
                                count++;
                            }
                        }
                        
                        cout << "Found " << count << " twin prime pairs." << endl;
                    }
                    else if (lab_input == "5") {
                        // PRIME COUNTER
                        cout << "Count primes from: ";
                        int start;
                        cin >> start;
                        cout << "Count primes to: ";
                        int end;
                        cin >> end;
                        cin.ignore();
                        
                        int count = 0;
                        for (int i = start; i <= end; i++) {
                            if (isPrime(i)) count++;
                        }
                        
                        cout << "There are " << count << " primes between " 
                            << start << " and " << end << "." << endl;
                    }
                    else if (lab_input == "6") {
                        // EXIT
                        cout << "Exiting Prime Factory..." << endl;
                        factory_running = false;
                    }
                    else {
                        cout << "Invalid choice. Try again." << endl;
                    }
                }
            }
            else if (choice == 3) {
                // FUNCTION INTELLIGENCE LAB (You can add your derivative/vertex functions here!)
                cout << "🧪 Function Intelligence Lab - Coming Soon!" << endl;
                cout << "This will have calculus functions like:" << endl;
                cout << "- Derivative calculator" << endl;
                cout << "- Vertex finder" << endl;
                cout << "- Root solver" << endl;
                cout << "- Integration" << endl;
            }
            else {
                cout << "Invalid lab choice." << endl;
            }
        }
        else if (input.rfind("nano ", 0) == 0) {
            string filename = input.substr(5);
            system(("nano " + filename).c_str());
            cout << "Back to DeltaOne." << endl;
        }     
        else if (input.rfind("vim ", 0) == 0) {
            string filename = input.substr(4);
            system(("vim " + filename).c_str());
            cout << "Returned to DeltaOne. (no longer a caveman!)" << endl;
        }
        else if (input.rfind("edit ", 0) == 0) {
            string filename = input.substr(5);
            // Use default editor from environment, or fallback to nano
            const char* editor = getenv("EDITOR");
            if (editor == nullptr) editor = "nano";
            system((string(editor) + " " + filename).c_str());
            cout << "✓ Returned to DeltaOne." << endl;
        }
        else if (input == "games") {
            cout << "=== Mini Games ===" << endl;
            cout << "1: Number Guessing" << endl;
            cout << "2: Tic Tac Toe" << endl;
            cout << "3: Rock-Paper-Scissors" << endl;
            cout << "4: Hangman" << endl;
            cout << "5: Snake" << endl;
            cout << "6: Connect Four" << endl;
            int choice;
            cin >> choice;
            cin.ignore();
            
            if (choice == 1) {
                int secret = randomnum(1, 100);
                int guesses = 0;
                cout << "I'm thinking of a number 1-100..." << endl;
                while (true) {
                    int guess;
                    cin >> guess;
                    guesses++;
                    if (guess == secret) {
                        cout << "🎉 Correct in " << guesses << " tries!" << endl;
                        break;
                    } else if (guess < secret) {
                        cout << "📈 Higher!" << endl;
                    } else {
                        cout << "📉 Lower!" << endl;
                    }
                }
            }
            else if (choice == 2) {
                // TIC-TAC-TOE
                board = {" ", " ", " ", " ", " ", " ", " ", " ", " "};  // Reset global board
                
                auto display_board = [](const vector<string>& b) {
                    cout << "\n";
                    cout << " " << b[0] << " | " << b[1] << " | " << b[2] << "\n";
                    cout << "---|---|---\n";
                    cout << " " << b[3] << " | " << b[4] << " | " << b[5] << "\n";
                    cout << "---|---|---\n";
                    cout << " " << b[6] << " | " << b[7] << " | " << b[8] << "\n\n";
                };
                
                auto check_winner = [](const vector<string>& b, string player) -> bool {
                    for (auto& pattern : WIN_PATTERNS) {  // <-- USES YOUR GLOBAL LIST!
                        if (b[pattern[0]] == player && 
                            b[pattern[1]] == player && 
                            b[pattern[2]] == player) {
                            return true;
                        }
                    }
                    return false;
                };
                
                auto is_full = [](const vector<string>& b) -> bool {
                    for (const auto& cell : b) {
                        if (cell == " ") return false;
                    }
                    return true;
                };
                
                // Minimax AI
                auto minimax = [&](vector<string>& b, bool is_maximizing, auto& minimax_ref) -> int {
                    if (check_winner(b, "O")) return 10;
                    if (check_winner(b, "X")) return -10;
                    if (is_full(b)) return 0;
                    
                    if (is_maximizing) {
                        int best = -1000;
                        for (int i = 0; i < 9; i++) {
                            if (b[i] == " ") {
                                b[i] = "O";
                                best = max(best, minimax_ref(b, false, minimax_ref));
                                b[i] = " ";
                            }
                        }
                        return best;
                    } else {
                        int best = 1000;
                        for (int i = 0; i < 9; i++) {
                            if (b[i] == " ") {
                                b[i] = "X";
                                best = min(best, minimax_ref(b, true, minimax_ref));
                                b[i] = " ";
                            }
                        }
                        return best;
                    }
                };
                
                auto find_best_move = [&](vector<string>& b) -> int {
                    int best_val = -1000;
                    int best_move = -1;
                    
                    for (int i = 0; i < 9; i++) {
                        if (b[i] == " ") {
                            b[i] = "O";
                            int move_val = minimax(b, false, minimax);
                            b[i] = " ";
                            
                            if (move_val > best_val) {
                                best_move = i;
                                best_val = move_val;
                            }
                        }
                    }
                    return best_move;
                };
                
                cout << "\n=== TIC-TAC-TOE ===\nYou: X | AI: O\n";
                
                while (true) {
                    display_board(board);
                    
                    cout << "Your move (1-9): ";
                    int move;
                    cin >> move;
                    cin.ignore();
                    
                    int idx = move - 1;
                    if (idx < 0 || idx > 8 || board[idx] != " ") {
                        cout << "❌ Invalid!" << endl;
                        continue;
                    }
                    
                    board[idx] = "X";
                    
                    if (check_winner(board, "X")) {
                        display_board(board);
                        cout << "🎉 You win!" << endl;
                        break;
                    }
                    if (is_full(board)) {
                        display_board(board);
                        cout << "🤝 Tie!" << endl;
                        break;
                    }
                    
                    int ai_move = find_best_move(board);
                    board[ai_move] = "O";
                    cout << "AI → " << (ai_move + 1) << endl;
                    
                    if (check_winner(board, "O")) {
                        display_board(board);
                        cout << "😭 AI wins!" << endl;
                        break;
                    }
                    if (is_full(board)) {
                        display_board(board);
                        cout << "🤝 Tie!" << endl;
                        break;
                    }
                }
            }
            else if (choice == 3) {
                // RPS (a cool acryonym)
                cout << "Rock, Paper, or Scissors? (R/P/S): " << endl;
                char input;
                cin >> input;
                cin.ignore();  // Like the cherry on the cake

                int smart = randomnum(0, 1);
                char output;
                int dumb = randomnum(1, 3);

                if (input == 'R' || input == 'r') {
                    if (smart == 1) {
                        output = 'P';   // Rock loses to Paper
                    } else {
                        if (dumb == 1) output = 'R';
                        else if (dumb == 2) output = 'P';
                        else output = 'S';
                    }
                }
                else if (input == 'P' || input == 'p') {
                    if (smart == 1) {
                        output = 'S';   // Paper loses to Scissors
                    } else {
                        if (dumb == 1) output = 'R';
                        else if (dumb == 2) output = 'P';
                        else output = 'S';
                    }
                }
                else if (input == 'S' || input == 's') {
                    if (smart == 1) {
                        output = 'R';   // Scissors lose to Rock
                    } else {
                        if (dumb == 1) output = 'R';
                        else if (dumb == 2) output = 'P';
                        else output = 'S';
                    }
                }

                cout << "I pick: " << output << endl;
                
                // Fix the win check
                if (input == output) {      // Man, wish there was a triple ternary. 
                    cout << "🤝 TIE!" << endl;
                } else if (wonRockPaperScissors(input, output)) {
                    cout << "🎉 You WIN!" << endl;
                } else {
                    cout << "😞 You LOST!" << endl;
                }
            }
        else if (input == "todo") {
            while (true) {
                cout << "\n=== Todo List ===" << endl;
                cout << "1: Show Tasks" << endl;
                cout << "2: Add Task" << endl;
                cout << "3: Mark Done" << endl;
                cout << "4: Delete Task" << endl;
                cout << "5: Exit" << endl;
                cout << "Choose: ";
                
                int choice;
                cin >> choice;
                cin.ignore();  // ← SUPER IMPORTANT! Clears the newline after cin
                
                if (choice == 1) {
                    // Show all tasks
                    if (todo_list.empty()) {
                        cout << "No tasks yet!" << endl;
                    } else {
                        cout << "\n--- Your Tasks ---" << endl;
                        for (int i = 0; i < todo_list.size(); i++) {
                            cout << (i+1) << ". " << todo_list[i].toString() << endl;
                        }
                    }
                }
                else if (choice == 2) {
                    // Add task
                    cout << "Enter task: ";
                    string task_text;
                    getline(cin, task_text);
                    
                    if (!task_text.empty()) {
                        todo_list.push_back(Task(task_text));
                        cout << "✓ Task added!" << endl;
                    }
                }
                else if (choice == 3) {
                    // Mark as done
                    if (todo_list.empty()) {
                        cout << "No tasks to mark!" << endl;
                        continue;
                    }
                    
                    cout << "\n--- Your Tasks ---" << endl;
                    for (int i = 0; i < todo_list.size(); i++) {
                        cout << (i+1) << ". " << todo_list[i].toString() << endl;
                    }
                    
                    cout << "Which task to mark done? ";
                    int task_num;
                    cin >> task_num;
                    cin.ignore();
                    
                    int index = task_num - 1;
                    if (index >= 0 && index < todo_list.size()) {
                        todo_list[index].done = true;
                        cout << "✓ Marked as done!" << endl;
                    } else {
                        cout << "Invalid task number!" << endl;
                    }
                }
                else if (choice == 4) {
                    // Delete task
                    if (todo_list.empty()) {
                        cout << "No tasks to delete!" << endl;
                        continue;
                    }
                    
                    cout << "\n--- Your Tasks ---" << endl;
                    for (int i = 0; i < todo_list.size(); i++) {
                        cout << (i+1) << ". " << todo_list[i].toString() << endl;
                    }
                    
                    cout << "Which task to delete? ";
                    int task_num;
                    cin >> task_num;
                    cin.ignore();
                    
                    int index = task_num - 1;
                    if (index >= 0 && index < todo_list.size()) {
                        cout << "Deleted: " << todo_list[index].text << endl;
                        todo_list.erase(todo_list.begin() + index);
                    } else {
                        cout << "Invalid task number!" << endl;
                    }
                }
                else if (choice == 5) {
                    // Exit
                    cout << "Exiting Todo..." << endl;
                    break;
                }
                else {
                    cout << "Invalid choice!" << endl;
                }
            }
        }
        else if (input == "sysinfo") {
            cout << "=== System Information ===" << endl;
            system("uname -a");  // OS info
            cout << "\nCPU:" << endl;
            system("sysctl -n machdep.cpu.brand_string");  // macOS
            cout << "\nMemory:" << endl;
            system("top -l 1 | head -n 10 | grep PhysMem");
        }
        else if (input == "disk") {
            system("df -h");
        }
        else if (input == "pwd") {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            cout << cwd << endl;
        }
        else if (input.rfind("cd ", 0) == 0) {
            string path = input.substr(3);
            if (chdir(path.c_str()) == 0) {
                cout << "✓ Changed directory" << endl;
            } else {
                cout << "✗ Directory not found" << endl;
            }
        }
        else if (input == "ls") {
            system("ls --color=auto");
        }
        else if (input.rfind("cat ", 0) == 0) {
            string filename = input.substr(4);
            ifstream file(filename);
            if (file.is_open()) {
                string line;
                while (getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            } else {
                cout << "File not found" << endl;
            }
        }
        else if (input.rfind("mkdir ", 0) == 0) {
            string dirname = input.substr(6);
            system(("mkdir " + dirname).c_str());
            cout << "✓ Created directory" << endl;
        }                      
        if (input.rfind("install ", 0) == 0) {
            string pkg = input.substr(8);
            handle_install(pkg);
        }
        else if (input.rfind("remove ", 0) == 0) {
            string pkg = input.substr(7);
            handle_remove(pkg);
        }
        else if (input == "list") {
            handle_list();
        }        
        else if (input.rfind("convert ", 0) == 0) {
            string args = input.substr(8);
            size_t space_pos = args.find(' ');
            if (space_pos == string::npos) {
                cout << "Usage: conver <number> <base>" << endl;
            }
            else {
                string num_str = args.substr(0, space_pos);        // "12"
                string base_str = args.substr(space_pos + 1);
                long number = stol(num_str);
                int base = stoi(base_str);
                string resultOfConversion = decimalToBase(number, base);
                cout << "Result: " << resultOfConversion << endl;
            }
        }              
        else if (input.rfind("reverse ", 0) == 0) {
            string text = input.substr(8);
            reverse(text.begin(), text.end());
            cout << text << endl;
        }
        else if (input.rfind("pacman -S ", 0) == 0) {       // Just for the fun of it.. I don't know anything about this. Pacman is cool, in any context, though.
            string pkg = input.substr(10);
            
            cout << ":: Synchronizing package databases..." << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            cout << "resolving dependencies..." << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            cout << "looking for conflicting packages..." << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            handle_install(pkg);
        }
        else if (input.rfind("cowsay ", 0) == 0) {
            string message = input.substr(7);
            int len = message.length();
            
            cout << " " << string(len + 2, '_') << endl;
            cout << "< " << message << " >" << endl;
            cout << " " << string(len + 2, '-') << endl;
            cout << R"(        \   ^__^
                \  (oo)\_______
                    (__)\       )\/\
                        ||----w |
                        ||     ||)" << endl;
        }
        else if (input == "date") {
            time_t now = time(nullptr);
            tm *local = localtime(&now);
            char buffer[100];
            strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Z %Y", local);
            cout << buffer << endl;
        }
        else if (input.rfind("python oneline ", 0) == 0) {
            string code = input.substr(10);
            
            // Build Python command
            string cmd = "python3 -c \"" + code + "\"";
            
            cout << "Running Python..." << endl;
            system(cmd.c_str());
        }
        else if (input == "python" || input == "python3") {
            cout << "Starting Python interpreter..." << endl;
            cout << "(Type 'exit()' or Ctrl+D to return to DeltaOne)" << endl;
            cout << endl;
            
            system("python3");
            
            cout << endl;
            cout << "✅ Returned to DeltaOne" << endl;
        }
        else if (input.rfind("python ", 0) == 0) {
            string filename = input.substr(7);
            
            if (filename.find(".py") == string::npos) {
                cout << "❌ Error: Must be a .py file" << endl;
            } else {
                cout << "🐍 Running Python script: " << filename << endl;
                cout << "======================" << endl;
                
                string cmd = "python3 " + filename;
                int status = system(cmd.c_str());
                
                cout << "======================" << endl;
                
                if (status == 0) {
                    cout << "✅ Script finished successfully" << endl;
                } else {
                    cout << "❌ Script exited with error code " << status << endl;
                }
            }
        }
        else if (input == "pyedit" || input.rfind("pyedit ", 0) == 0) {
            string filename = "temp.py";
            
            if (input.length() > 7) {
                filename = input.substr(7);
            }
            
            cout << "📝 Python Editor" << endl;
            cout << "Type your code (type 'END' on a new line to finish)" << endl;
            cout << "======================" << endl;
            
            vector<string> lines;
            string line;
            
            cin.ignore();  // Clear buffer
            
            while (true) {
                getline(cin, line);
                
                if (line == "END") {
                    break;
                }
                
                lines.push_back(line);
            }
            
            // Write to file
            ofstream pyfile(filename);
            for (auto& l : lines) {
                pyfile << l << endl;
            }
            pyfile.close();
            
            cout << "======================" << endl;
            cout << "✅ Saved to " << filename << endl;
            cout << "Running..." << endl;
            cout << "======================" << endl;
            
            string cmd = "python3 " + filename;
            system(cmd.c_str());
            
            cout << "======================" << endl;
        }
        else if (input.rfind("pycalc ", 0) == 0) {
                    string expr = input.substr(7);
                    
                    string cmd = "python3 -c \"print(" + expr + ")\"";
                    
                    cout << "Result: ";
                    system(cmd.c_str());
                }
        else if (input == "py") {
            cout << "=== Python Mini-IDE ===" << endl;
            cout << "Commands:" << endl;
            cout << "  write <file>  - Write Python code" << endl;
            cout << "  run <file>    - Run Python file" << endl;
            cout << "  shell         - Open Python REPL" << endl;
            cout << "  list          - List .py files" << endl;
            cout << "  exit          - Return to DeltaOne" << endl;
            cout << endl;
            
            while (true) {
                cout << "Python-IDE > ";
                string pyinput;
                getline(cin, pyinput);
                
                if (pyinput == "exit") {
                    break;
                }
                else if (pyinput.rfind("write ", 0) == 0) {
                    string filename = pyinput.substr(6);
                    
                    cout << "Enter code (type 'DONE' to finish):" << endl;
                    
                    ofstream file(filename);
                    string line;
                    
                    while (true) {
                        getline(cin, line);
                        if (line == "DONE") break;
                        file << line << endl;
                    }
                    
                    file.close();
                    cout << "✅ Saved " << filename << endl;
                }
                else if (pyinput.rfind("run ", 0) == 0) {
                    string filename = pyinput.substr(4);
                    
                    cout << "=======================" << endl;
                    system(("python3 " + filename).c_str());
                    cout << "=======================" << endl;
                }
                else if (pyinput == "shell") {
                    system("python3");
                }
                else if (pyinput == "list") {
                    system("ls *.py 2>/dev/null");
                }
                else {
                    cout << "Unknown command. Type 'exit' to quit." << endl;
                }
            }
            
            cout << "Returned to DeltaOne" << endl;
        }
        else if (input.rfind("pybuy ", 0) == 0 && input.find(" DeltaOne") != string::npos) {
            // Extract the module name between "pybuy " and " DeltaOne"
            
            size_t start = 6;  // After "pybuy "
            size_t end = input.find(" DeltaOne");
            
            string module = input.substr(start, end - start);
            
            cout << "Installing Python module: " << module << endl;
            
            string cmd = "pip3 install " + module;
            system(cmd.c_str());
        }
        else if (input.find(' ') != string::npos) {             // Detects if there is >1 words or tokens
            stringstream ss(input);
            string token;
            int tokenCount = 0;

            while (ss >> token) {       // Runs a while loop that increments tokenCount when true
                tokenCount++;
            }

            if (tokenCount == 3) {          // Detects if the amount of tokens is 3
                stringstream ss(input);
                string left, op, right;
                ss >> left >> op >> right;  

                if (find(operators_calc.begin(), operators_calc.end(), op) != operators_calc.end()) {
                    auto it = find(operators_calc.begin(), operators_calc.end(), op);

                    // === Handle Operators ===
                    if (*it == "+") {
                        cout << left << " + " << right << " = " << (stod(left) + stod(right)) << endl;
                    }
                    else if (*it == "-") {
                        cout << left << " - " << right << " = " << (stod(left) - stod(right)) << endl;
                    }
                    else if (*it == "*") {
                        cout << left << " * " << right << " = " << (stod(left) * stod(right)) << endl;
                    }
                    else if (*it == "/") {
                        if (stod(right) == 0)
                            cout << "Error: Division by zero!" << endl;
                        else
                            cout << left << " / " << right << " = " << (stod(left) / stod(right)) << endl;
                    }
                    else if (*it == "mod") {
                        double a = stod(left);
                        double b = stod(right);
                        cout << left << " mod " << right << " = " << fmod(a, b) << endl;
                    }
                    else if (*it == "^" || *it == "**") {
                        cout << left << " ^ " << right << " = " << pow(stod(left), stod(right)) << endl;
                    }
                    else if (*it == "//") {
                        cout << left << " // " << right << " = " << floor1(stod(left) / stod(right)) << endl;
                    }
                    else if (*it == "==") {
                        cout << (left == right ? "True" : "False") << endl;
                    }
                    else if (*it == "!=") {
                        cout << (left != right ? "True" : "False") << endl;
                    }
                    else if (*it == ">") {
                        cout << (left > right ? "True" : "False") << endl;
                    }
                    else if (*it == "<") {
                        cout << (left < right ? "True" : "False") << endl;
                    }
                    else if (*it == ">=") {
                        cout << (left >= right ? "True" : "False") << endl;
                    }
                    else if (*it == "<=") {
                        cout << (left <= right ? "True" : "False") << endl;
                    }
                    else if (*it == "&&") {                                                     // Note: For these logical operators, enter 0s and 1s.
                        cout << (left == "1" && right == "1" ? "True" : "False") << endl;
                    }
                    else if (*it == "OR") {
                        bool a = (left == "1" || left == "True");       // I added some lines of code for this OR statement, making a ternary thing would be too messy
                        bool b = (right == "1" || right == "True");
                        cout << (a || b ? "True" : "False") << endl;
                    }
                    else if (*it == "NOT") {
                        cout << (left == "1" ? "False" : "True") << endl;       // Another note: When doing NOT, it'll invert only the first input, just put a random number for the second.
                    }
                    else {
                        cout << "❌ Error: Input(s) invalid or operation not implemented yet." << endl;
                    }
                }
                else {
                    cout << "❌ Invalid operator: " << op << endl;
                }
            }
        }
        else if (input == "help") {
            help_menu(); 
        }
        else {
            cout << "Unknown command or parameters." << endl;
        }
    }

    ofstream fout("history.txt");                   // Save history for next run
    for (auto& cmd : history_list) fout << cmd << endl;
    fout.close();

    return 0;           // THE GRAND FINALE 
}




// Random code (how much code? 150 lines?)




