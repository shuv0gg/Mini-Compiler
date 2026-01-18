#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>
#include "Token.h"

class Lexer {
private:
    std::string source;
    size_t position;
    int line;
    int column;
    std::vector<Token> tokens;
    std::map<std::string, TokenType> keywords;
    
    char currentChar();
    char peek();
    void advance();
    void skipWhitespace();
    void skipComment();
    Token makeNumber();
    Token makeIdentifierOrKeyword();
    
public:
    Lexer(const std::string& src);
    std::vector<Token> tokenize();
    const std::vector<Token>& getTokens() const { return tokens; }
};

#endif // LEXER_H
