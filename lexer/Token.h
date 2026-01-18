#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>

enum class TokenType {
    // Keywords
    LET,
    PRINT,
    IF,
    ELSE,
    FOR,
    TO,
    
    // Identifiers and Literals
    IDENTIFIER,
    NUMBER,
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    EQUAL,
    GREATER,
    LESS,
    
    // Delimiters
    SEMICOLON,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    
    // Special
    END_OF_FILE,
    INVALID
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    Token(TokenType t = TokenType::INVALID, const std::string& lex = "", int l = 0, int c = 0)
        : type(t), lexeme(lex), line(l), column(c) {}
    
    std::string getTypeName() const {
        static const std::map<TokenType, std::string> typeNames = {
            {TokenType::LET, "LET"},
            {TokenType::PRINT, "PRINT"},
            {TokenType::IF, "IF"},
            {TokenType::ELSE, "ELSE"},
            {TokenType::FOR, "FOR"},
            {TokenType::TO, "TO"},
            {TokenType::IDENTIFIER, "IDENTIFIER"},
            {TokenType::NUMBER, "NUMBER"},
            {TokenType::PLUS, "PLUS"},
            {TokenType::MINUS, "MINUS"},
            {TokenType::MULTIPLY, "MULTIPLY"},
            {TokenType::DIVIDE, "DIVIDE"},
            {TokenType::ASSIGN, "ASSIGN"},
            {TokenType::EQUAL, "EQUAL"},
            {TokenType::GREATER, "GREATER"},
            {TokenType::LESS, "LESS"},
            {TokenType::SEMICOLON, "SEMICOLON"},
            {TokenType::LBRACE, "LBRACE"},
            {TokenType::RBRACE, "RBRACE"},
            {TokenType::LPAREN, "LPAREN"},
            {TokenType::RPAREN, "RPAREN"},
            {TokenType::END_OF_FILE, "EOF"},
            {TokenType::INVALID, "INVALID"}
        };
        
        auto it = typeNames.find(type);
        return it != typeNames.end() ? it->second : "UNKNOWN";
    }
};

#endif // TOKEN_H
