#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
class ASTVisitor;

// Base AST Node
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual std::string toJSON(int indent = 0) const = 0;
};

// Expression Nodes
class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

class NumberExpression : public Expression {
public:
    int value;
    
    NumberExpression(int val) : value(val) {}
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

class VariableExpression : public Expression {
public:
    std::string name;
    
    VariableExpression(const std::string& n) : name(n) {}
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

class BinaryExpression : public Expression {
public:
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    
    BinaryExpression(const std::string& operation, 
                     std::unique_ptr<Expression> l, 
                     std::unique_ptr<Expression> r)
        : op(operation), left(std::move(l)), right(std::move(r)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

// Statement Nodes
class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

class VariableDeclaration : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> initializer;
    
    VariableDeclaration(const std::string& n, std::unique_ptr<Expression> init)
        : name(n), initializer(std::move(init)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

class PrintStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    
    PrintStatement(std::unique_ptr<Expression> expr)
        : expression(std::move(expr)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

class BlockStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    BlockStatement() = default;
    
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;
    
    IfStatement(std::unique_ptr<Expression> cond,
                std::unique_ptr<Statement> thenBr,
                std::unique_ptr<Statement> elseBr = nullptr)
        : condition(std::move(cond)), 
          thenBranch(std::move(thenBr)), 
          elseBranch(std::move(elseBr)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

class ForStatement : public Statement {
public:
    std::string variable;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
    std::unique_ptr<Statement> body;
    
    ForStatement(const std::string& var,
                 std::unique_ptr<Expression> s,
                 std::unique_ptr<Expression> e,
                 std::unique_ptr<Statement> b)
        : variable(var), start(std::move(s)), end(std::move(e)), body(std::move(b)) {}
    
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    Program() = default;
    
    void accept(ASTVisitor& visitor) override;
    std::string toJSON(int indent = 0) const override;
};

// Visitor Pattern
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(NumberExpression& node) = 0;
    virtual void visit(VariableExpression& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(PrintStatement& node) = 0;
    virtual void visit(BlockStatement& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(ForStatement& node) = 0;
    virtual void visit(Program& node) = 0;
};

#endif // AST_H
