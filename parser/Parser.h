#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include <string>
#include "../lexer/Token.h"
#include "../ast/AST.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    std::vector<std::string> errors;
    
    Token peek();
    Token previous();
    Token advance();
    bool isAtEnd();
    bool check(TokenType type);
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    void error(const std::string& message);
    
    // Parsing methods
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseVariableDeclaration();
    std::unique_ptr<Statement> parsePrintStatement();
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseForStatement();
    std::unique_ptr<Statement> parseBlock();
    
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parsePrimary();
    
public:
    Parser(const std::vector<Token>& toks);
    std::unique_ptr<Program> parse();
    const std::vector<std::string>& getErrors() const { return errors; }
    bool hasErrors() const { return !errors.empty(); }
};

#endif // PARSER_H
