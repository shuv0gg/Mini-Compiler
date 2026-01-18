#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include "Compiler.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#endif

std::string urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            std::istringstream is(str.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                result += static_cast<char>(value);
                i += 2;
            } else {
                result += str[i];
            }
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

std::string extractPostData(const std::string& request) {
    size_t pos = request.find("\r\n\r\n");
    if (pos != std::string::npos) {
        return request.substr(pos + 4);
    }
    return "";
}

std::string getSourceFromPost(const std::string& postData) {
    size_t pos = postData.find("source=");
    if (pos != std::string::npos) {
        std::string encoded = postData.substr(pos + 7);
        size_t endPos = encoded.find('&');
        if (endPos != std::string::npos) {
            encoded = encoded.substr(0, endPos);
        }
        return urlDecode(encoded);
    }
    return "";
}

void openBrowserUrl(const std::string& url) {
    #ifdef _WIN32
        // Windows: Use ShellExecute to open URL
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOW);
    #elif __APPLE__
        // macOS: Use open command
        std::string command = "open " + url;
        system(command.c_str());
    #else
        // Linux: Try xdg-open first, fallback to others
        std::string command = "xdg-open " + url;
        if (system(command.c_str()) != 0) {
            command = "firefox " + url + " &";
            system(command.c_str());
        }
    #endif
}

void handleRequest(int clientSocket, const std::string& request) {
    Compiler compiler;
    std::string response;
    
    if (request.find("GET / ") == 0 || request.find("GET /index.html") == 0) {
        // Serve HTML file
        std::ifstream file("web/index.html");
        if (file.is_open()) {
            std::ostringstream content;
            content << file.rdbuf();
            response = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/html\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "\r\n" + content.str();
        } else {
            response = "HTTP/1.1 404 Not Found\r\n\r\nFile not found";
        }
    } 
    else if (request.find("GET /style.css") == 0) {
        std::ifstream file("web/style.css");
        if (file.is_open()) {
            std::ostringstream content;
            content << file.rdbuf();
            response = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/css\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "\r\n" + content.str();
        }
    }
    else if (request.find("GET /script.js") == 0) {
        std::ifstream file("web/script.js");
        if (file.is_open()) {
            std::ostringstream content;
            content << file.rdbuf();
            response = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: application/javascript\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "\r\n" + content.str();
        }
    }
    else if (request.find("POST /compile") == 0) {
        std::string postData = extractPostData(request);
        std::string source = getSourceFromPost(postData);
        
        std::cout << "Compiling source code..." << std::endl;
        auto result = compiler.compileAndRun(source);
        std::string json = result.toJSON();
        
        std::cout << "JSON Response length: " << json.length() << " bytes" << std::endl;
        std::cout << "First 200 chars: " << json.substr(0, 200) << "..." << std::endl;
        
        response = "HTTP/1.1 200 OK\r\n"
                  "Content-Type: application/json\r\n"
                  "Access-Control-Allow-Origin: *\r\n"
                  "\r\n" + json;
    }
    else if (request.find("GET /examples/") == 0) {
        // Extract example number
        size_t pos = request.find("/examples/");
        std::string examplePath = request.substr(pos);
        size_t spacePos = examplePath.find(' ');
        if (spacePos != std::string::npos) {
            examplePath = examplePath.substr(0, spacePos);
        }
        
        std::ifstream file(examplePath.substr(1)); // Remove leading '/'
        if (file.is_open()) {
            std::ostringstream content;
            content << file.rdbuf();
            response = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/plain\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "\r\n" + content.str();
        } else {
            response = "HTTP/1.1 404 Not Found\r\n\r\nExample not found";
        }
    }
    else {
        response = "HTTP/1.1 404 Not Found\r\n\r\nNot found";
    }
    
    send(clientSocket, response.c_str(), response.length(), 0);
}

int main(int argc, char* argv[]) {
    std::cout << "=== Educational Mini Compiler ===" << std::endl;
    std::cout << "6-Stage Compilation System" << std::endl;
    std::cout << "=================================" << std::endl << std::endl;
    
    // Check if running in server mode
    if (argc > 1 && std::strcmp(argv[1], "--server") == 0) {
        std::cout << "Starting HTTP server on port 8080..." << std::endl;
        std::cout << "Open http://localhost:8080 in your browser" << std::endl;
        
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
        
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Failed to create socket" << std::endl;
        // Open browser automatically
        openBrowserUrl("http://localhost:8080");
        
            return 1;
        }
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(8080);
        
        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            return 1;
        }
        
        listen(serverSocket, 5);
        
        while (true) {
            sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
            
            if (clientSocket < 0) continue;
            
            char buffer[4096] = {0};
            recv(clientSocket, buffer, sizeof(buffer), 0);
            
            std::string request(buffer);
            handleRequest(clientSocket, request);
            
#ifdef _WIN32
            closesocket(clientSocket);
#else
            close(clientSocket);
#endif
        }
        
#ifdef _WIN32
        closesocket(serverSocket);
        WSACleanup();
#else
        close(serverSocket);
#endif
    } else {
        // Command-line mode
        std::cout << "Usage: " << argv[0] << " --server" << std::endl;
        std::cout << "  Start web server on port 8080" << std::endl << std::endl;
        
        // Test example
        std::string testCode = 
            "let x = 10;\n"
            "let y = 20;\n"
            "print x + y;\n";
        
        std::cout << "Running test program:" << std::endl;
        std::cout << testCode << std::endl;
        
        Compiler compiler;
        auto result = compiler.compileAndRun(testCode);
        
        if (result.success) {
            std::cout << "Output: " << result.executionOutput << std::endl;
        } else {
            std::cout << "Error: " << result.errorMessage << std::endl;
        }
    }
    
    return 0;
}
