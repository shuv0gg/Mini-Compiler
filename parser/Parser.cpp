#include "Parser.h"
#include <sstream>

Parser::Parser(const std::vector<Token>& toks) 
    : tokens(toks), current(0) {}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    error(message);
    return peek();
}

void Parser::error(const std::string& message) {
    Token token = peek();
    std::ostringstream oss;
    oss << "Parse error at line " << token.line << ", column " << token.column 
        << ": " << message;
    errors.push_back(oss.str());
}

std::unique_ptr<Program> Parser::parse() {
    return parseProgram();
}

std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    
    while (!isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            program->statements.push_back(std::move(stmt));
        } else {
            // Skip to next statement on error
            advance();
        }
    }
    
    return program;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (match(TokenType::LET)) {
        return parseVariableDeclaration();
    }
    if (match(TokenType::PRINT)) {
        return parsePrintStatement();
    }
    if (match(TokenType::IF)) {
        return parseIfStatement();
    }
    if (match(TokenType::FOR)) {
        return parseForStatement();
    }
    if (match(TokenType::LBRACE)) {
        return parseBlock();
    }
    
    error("Expected statement");
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseVariableDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::ASSIGN, "Expected '=' after variable name");
    auto initializer = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    
    return std::make_unique<VariableDeclaration>(name.lexeme, std::move(initializer));
}

std::unique_ptr<Statement> Parser::parsePrintStatement() {
    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after print statement");
    return std::make_unique<PrintStatement>(std::move(expr));
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
    auto condition = parseExpression();
    consume(TokenType::LBRACE, "Expected '{' after if condition");
    auto thenBranch = parseBlock();
    
    std::unique_ptr<Statement> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        consume(TokenType::LBRACE, "Expected '{' after else");
        elseBranch = parseBlock();
    }
    
    return std::make_unique<IfStatement>(std::move(condition), 
                                         std::move(thenBranch), 
                                         std::move(elseBranch));
}

std::unique_ptr<Statement> Parser::parseForStatement() {
    Token varName = consume(TokenType::IDENTIFIER, "Expected variable name in for loop");
    consume(TokenType::ASSIGN, "Expected '=' in for loop");
    auto start = parseExpression();
    consume(TokenType::TO, "Expected 'to' in for loop");
    auto end = parseExpression();
    consume(TokenType::LBRACE, "Expected '{' after for loop header");
    auto body = parseBlock();
    
    return std::make_unique<ForStatement>(varName.lexeme, 
                                          std::move(start), 
                                          std::move(end), 
                                          std::move(body));
}

std::unique_ptr<Statement> Parser::parseBlock() {
    auto block = std::make_unique<BlockStatement>();
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            block->statements.push_back(std::move(stmt));
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after block");
    return block;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseComparison();
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto expr = parseTerm();
    
    if (match({TokenType::GREATER, TokenType::LESS, TokenType::EQUAL})) {
        Token op = previous();
        auto right = parseTerm();
        expr = std::make_unique<BinaryExpression>(op.lexeme, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseTerm() {
    auto expr = parseFactor();
    
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = parseFactor();
        expr = std::make_unique<BinaryExpression>(op.lexeme, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseFactor() {
    auto expr = parsePrimary();
    
    while (match({TokenType::MULTIPLY, TokenType::DIVIDE})) {
        Token op = previous();
        auto right = parsePrimary();
        expr = std::make_unique<BinaryExpression>(op.lexeme, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    if (match(TokenType::NUMBER)) {
        int value = std::stoi(previous().lexeme);
        return std::make_unique<NumberExpression>(value);
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<VariableExpression>(previous().lexeme);
    }
    
    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    error("Expected expression");
    return std::make_unique<NumberExpression>(0); // Error recovery
}
