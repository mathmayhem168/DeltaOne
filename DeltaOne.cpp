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


// Also, sorry about the clear and terminal inputs, they only work for Mac and Linux. 😅
// Honestly, I was too lazy. But could you blame me if you made the 1000 lines of code?




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
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::duration;
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::time;
using std::pair;
using std::make_pair;









size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}



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






// Important stuff and declarations

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
        }
        else if (input.rfind("sleep ", 0) == 0) {
            int seconds = stoi(input.substr(6));
            this_thread::sleep_for(chrono::seconds(seconds));
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
            start_time = chrono::steady_clock::now() - chrono::seconds(new_time);
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
            cout << "2: Rock Paper Scissors" << endl;
            cout << "3: Hangman (text)" << endl;
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

    return 0;
}