#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <memory>
#include <map>
#include <string>
#include <vector>
#include "../ast/AST.h"

class Optimizer : public ASTVisitor {
private:
    std::map<std::string, int> constantValues; // For constant propagation
    std::vector<std::string> optimizations; // Log of optimizations performed
    bool modified;
    
    // Helper methods
    bool isConstant(Expression* expr, int& value);
    std::unique_ptr<Expression> optimizeExpression(std::unique_ptr<Expression> expr);
    
public:
    Optimizer() : modified(false) {}
    
    std::unique_ptr<Program> optimize(std::unique_ptr<Program> program);
    
    // Visitor methods
    void visit(NumberExpression& node) override;
    void visit(VariableExpression& node) override;
    void visit(BinaryExpression& node) override;
    void visit(VariableDeclaration& node) override;
    void visit(PrintStatement& node) override;
    void visit(BlockStatement& node) override;
    void visit(IfStatement& node) override;
    void visit(ForStatement& node) override;
    void visit(Program& node) override;
    
    const std::vector<std::string>& getOptimizations() const { return optimizations; }
    std::string getOptimizationReport() const;
};

#endif // OPTIMIZER_H
