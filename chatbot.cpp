#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <sstream>

/*
 * Example chatbot GUI using SFML and HuggingFace Inference API.
 *
 * NOTE: Most models on HuggingFace require authentication via an API token.
 * Replace HUGGING_FACE_TOKEN and MODEL_ENDPOINT with your own token and model
 * endpoint. You can sign up for a free token at https://huggingface.co/.
 */

// Adjust these constants for your chosen model.  For example,
// MODEL_ENDPOINT could be "https://api-inference.huggingface.co/models/microsoft/DialoGPT-medium".
static const char *HUGGING_FACE_TOKEN = "YOUR_HUGGINGFACE_API_TOKEN";
static const char *MODEL_ENDPOINT    = "https://api-inference.huggingface.co/models/microsoft/DialoGPT-small";

// Helper callback for libcurl to accumulate response data into a std::string
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    std::string *buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<char*>(contents), total);
    return total;
}

// Submit a prompt to the HuggingFace text-generation API and return the generated text.
// Returns an empty string on error.
std::string query_huggingface(const std::string &prompt) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL\n";
        return "";
    }

    // Prepare payload JSON. For basic models, an "inputs" field is expected.
    nlohmann::json payload;
    payload["inputs"] = prompt;
    std::string payload_str = payload.dump();

    std::string response_data;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // Add authorization header if token is provided
    if (HUGGING_FACE_TOKEN && std::string(HUGGING_FACE_TOKEN).size() > 0) {
        std::string authHeader = std::string("Authorization: Bearer ") + HUGGING_FACE_TOKEN;
        headers = curl_slist_append(headers, authHeader.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_URL, MODEL_ENDPOINT);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
        return "";
    }

    try {
        // Response from the Inference API is a JSON array; the first element has
        // key "generated_text" for text generation models.
        auto j = nlohmann::json::parse(response_data);
        if (j.is_array() && !j.empty()) {
            auto &obj = j.at(0);
            if (obj.contains("generated_text")) {
                return obj["generated_text"].get<std::string>();
            }
        }
    } catch (...) {
        std::cerr << "Failed to parse JSON response\n";
    }
    return "";
}

// Chatbot UI using SFML.  Presents a simple text chat window, accepts user input,
// sends it to the HuggingFace API, and displays the response.  You can scroll
// using the mouse wheel if the conversation is long.
void chatbotRun() {
    const unsigned int window_width  = 800;
    const unsigned int window_height = 600;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "DeltaOne Chatbot", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load a default font (Arial from macOS).  Change this path if running on other OS.
    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Font failed to load. Please adjust the path.\n";
        return;
    }

    // Chat log and input text UI.
    std::ostringstream chat_stream;
    chat_stream << "🤖 Chat ready. Type your message and press Enter.\n\n";

    sf::Text chat_text;
    chat_text.setFont(font);
    chat_text.setCharacterSize(18);
    chat_text.setFillColor(sf::Color::White);
    chat_text.setPosition(10.f, 10.f);
    chat_text.setString(chat_stream.str());

    sf::Text input_text;
    input_text.setFont(font);
    input_text.setCharacterSize(18);
    input_text.setFillColor(sf::Color::Cyan);
    input_text.setPosition(10.f, window_height - 40.f);

    std::string current_input;
    float scroll_offset = 0.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                // handle newline (return) to send input
                if (event.text.unicode == '\n' || event.text.unicode == '\r') {
                    std::string trimmed_input = current_input;
                    // Trim whitespace
                    trimmed_input.erase(trimmed_input.find_last_not_of(" \t\n\r") + 1);
                    if (!trimmed_input.empty()) {
                        chat_stream << "You: " << trimmed_input << "\n";
                        chat_text.setString(chat_stream.str());

                        // Query the model
                        std::string reply = query_huggingface(trimmed_input);
                        if (reply.empty()) {
                            reply = "[error generating reply]";
                        }
                        chat_stream << "Bot: " << reply << "\n\n";
                        chat_text.setString(chat_stream.str());
                    }
                    current_input.clear();
                } else if (event.text.unicode == 8) {
                    // Backspace
                    if (!current_input.empty()) current_input.pop_back();
                } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    current_input += static_cast<char>(event.text.unicode);
                }
                input_text.setString(current_input);
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                // Scroll the chat log by adjusting scroll_offset
                scroll_offset -= event.mouseWheelScroll.delta * 25.f;
                if (scroll_offset < 0.f) scroll_offset = 0.f;
            }
        }

        // Position chat text based on scroll offset
        chat_text.setPosition(10.f, 10.f - scroll_offset);

        window.clear(sf::Color(15, 15, 15));
        window.draw(chat_text);
        // draw input background
        sf::RectangleShape input_box(sf::Vector2f(window_width - 20.f, 30.f));
        input_box.setPosition(10.f, window_height - 40.f);
        input_box.setFillColor(sf::Color(40, 40, 40));
        window.draw(input_box);
        window.draw(input_text);
        window.display();
    }
}

int main() {
    // Initialize curl once
    curl_global_init(CURL_GLOBAL_DEFAULT);
    chatbotRun();
    curl_global_cleanup();
    return 0;
}
