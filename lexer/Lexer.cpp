#include "Lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src) 
    : source(src), position(0), line(1), column(1) {
    // Initialize keywords
    keywords["let"] = TokenType::LET;
    keywords["print"] = TokenType::PRINT;
    keywords["if"] = TokenType::IF;
    keywords["else"] = TokenType::ELSE;
    keywords["for"] = TokenType::FOR;
    keywords["to"] = TokenType::TO;
}

char Lexer::currentChar() {
    if (position >= source.length()) {
        return '\0';
    }
    return source[position];
}

char Lexer::peek() {
    if (position + 1 >= source.length()) {
        return '\0';
    }
    return source[position + 1];
}

void Lexer::advance() {
    if (position < source.length()) {
        if (source[position] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        position++;
    }
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar())) {
        advance();
    }
}

void Lexer::skipComment() {
    if (currentChar() == '/' && peek() == '/') {
        while (currentChar() != '\n' && currentChar() != '\0') {
            advance();
        }
    }
}

Token Lexer::makeNumber() {
    int startLine = line;
    int startColumn = column;
    std::string number;
    
    while (std::isdigit(currentChar())) {
        number += currentChar();
        advance();
    }
    
    return Token(TokenType::NUMBER, number, startLine, startColumn);
}

Token Lexer::makeIdentifierOrKeyword() {
    int startLine = line;
    int startColumn = column;
    std::string identifier;
    
    while (std::isalnum(currentChar()) || currentChar() == '_') {
        identifier += currentChar();
        advance();
    }
    
    // Check if it's a keyword
    auto it = keywords.find(identifier);
    if (it != keywords.end()) {
        return Token(it->second, identifier, startLine, startColumn);
    }
    
    return Token(TokenType::IDENTIFIER, identifier, startLine, startColumn);
}

std::vector<Token> Lexer::tokenize() {
    tokens.clear();
    
    while (currentChar() != '\0') {
        skipWhitespace();
        skipComment();
        
        if (currentChar() == '\0') break;
        
        int startLine = line;
        int startColumn = column;
        char ch = currentChar();
        
        // Numbers
        if (std::isdigit(ch)) {
            tokens.push_back(makeNumber());
            continue;
        }
        
        // Identifiers and keywords
        if (std::isalpha(ch) || ch == '_') {
            tokens.push_back(makeIdentifierOrKeyword());
            continue;
        }
        
        // Single character tokens
        switch (ch) {
            case '+':
                tokens.push_back(Token(TokenType::PLUS, "+", startLine, startColumn));
                advance();
                break;
            case '-':
                tokens.push_back(Token(TokenType::MINUS, "-", startLine, startColumn));
                advance();
                break;
            case '*':
                tokens.push_back(Token(TokenType::MULTIPLY, "*", startLine, startColumn));
                advance();
                break;
            case '/':
                tokens.push_back(Token(TokenType::DIVIDE, "/", startLine, startColumn));
                advance();
                break;
            case ';':
                tokens.push_back(Token(TokenType::SEMICOLON, ";", startLine, startColumn));
                advance();
                break;
            case '{':
                tokens.push_back(Token(TokenType::LBRACE, "{", startLine, startColumn));
                advance();
                break;
            case '}':
                tokens.push_back(Token(TokenType::RBRACE, "}", startLine, startColumn));
                advance();
                break;
            case '(':
                tokens.push_back(Token(TokenType::LPAREN, "(", startLine, startColumn));
                advance();
                break;
            case ')':
                tokens.push_back(Token(TokenType::RPAREN, ")", startLine, startColumn));
                advance();
                break;
            case '=':
                advance();
                if (currentChar() == '=') {
                    tokens.push_back(Token(TokenType::EQUAL, "==", startLine, startColumn));
                    advance();
                } else {
                    tokens.push_back(Token(TokenType::ASSIGN, "=", startLine, startColumn));
                }
                break;
            case '>':
                tokens.push_back(Token(TokenType::GREATER, ">", startLine, startColumn));
                advance();
                break;
            case '<':
                tokens.push_back(Token(TokenType::LESS, "<", startLine, startColumn));
                advance();
                break;
            default:
                tokens.push_back(Token(TokenType::INVALID, std::string(1, ch), startLine, startColumn));
                advance();
                break;
        }
    }
    
    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, column));
    return tokens;
}
