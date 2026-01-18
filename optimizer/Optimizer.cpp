#include "Optimizer.h"
#include <sstream>

bool Optimizer::isConstant(Expression* expr, int& value) {
    if (auto* numExpr = dynamic_cast<NumberExpression*>(expr)) {
        value = numExpr->value;
        return true;
    }
    
    if (auto* varExpr = dynamic_cast<VariableExpression*>(expr)) {
        auto it = constantValues.find(varExpr->name);
        if (it != constantValues.end()) {
            value = it->second;
            return true;
        }
    }
    
    if (auto* binExpr = dynamic_cast<BinaryExpression*>(expr)) {
        int leftVal, rightVal;
        if (isConstant(binExpr->left.get(), leftVal) && 
            isConstant(binExpr->right.get(), rightVal)) {
            
            if (binExpr->op == "+") value = leftVal + rightVal;
            else if (binExpr->op == "-") value = leftVal - rightVal;
            else if (binExpr->op == "*") value = leftVal * rightVal;
            else if (binExpr->op == "/") {
                if (rightVal != 0) value = leftVal / rightVal;
                else return false;
            }
            else if (binExpr->op == ">") value = leftVal > rightVal;
            else if (binExpr->op == "<") value = leftVal < rightVal;
            else if (binExpr->op == "==") value = leftVal == rightVal;
            else return false;
            
            return true;
        }
    }
    
    return false;
}

std::unique_ptr<Expression> Optimizer::optimizeExpression(std::unique_ptr<Expression> expr) {
    int value;
    if (isConstant(expr.get(), value)) {
        std::ostringstream oss;
        oss << "Constant folding: expression optimized to " << value;
        optimizations.push_back(oss.str());
        modified = true;
        return std::make_unique<NumberExpression>(value);
    }
    return expr;
}

std::unique_ptr<Program> Optimizer::optimize(std::unique_ptr<Program> program) {
    optimizations.clear();
    constantValues.clear();
    modified = false;
    
    program->accept(*this);
    
    if (!modified) {
        optimizations.push_back("No optimizations applied");
    }
    
    return program;
}

void Optimizer::visit(NumberExpression& node) {
    // Nothing to optimize
}

void Optimizer::visit(VariableExpression& node) {
    // Nothing to optimize
}

void Optimizer::visit(BinaryExpression& node) {
    // Visit children first
    node.left->accept(*this);
    node.right->accept(*this);
    
    // Try constant folding
    int leftVal, rightVal;
    if (isConstant(node.left.get(), leftVal) && isConstant(node.right.get(), rightVal)) {
        int result;
        bool canFold = true;
        
        if (node.op == "+") result = leftVal + rightVal;
        else if (node.op == "-") result = leftVal - rightVal;
        else if (node.op == "*") result = leftVal * rightVal;
        else if (node.op == "/") {
            if (rightVal != 0) result = leftVal / rightVal;
            else canFold = false;
        }
        else if (node.op == ">") result = leftVal > rightVal;
        else if (node.op == "<") result = leftVal < rightVal;
        else if (node.op == "==") result = leftVal == rightVal;
        else canFold = false;
        
        if (canFold) {
            std::ostringstream oss;
            oss << "Constant folding: " << leftVal << " " << node.op << " " 
                << rightVal << " = " << result;
            optimizations.push_back(oss.str());
            modified = true;
        }
    }
}

void Optimizer::visit(VariableDeclaration& node) {
    node.initializer->accept(*this);
    
    // Check for constant propagation
    int value;
    if (isConstant(node.initializer.get(), value)) {
        constantValues[node.name] = value;
        std::ostringstream oss;
        oss << "Constant propagation: " << node.name << " = " << value;
        optimizations.push_back(oss.str());
        modified = true;
    }
}

void Optimizer::visit(PrintStatement& node) {
    node.expression->accept(*this);
}

void Optimizer::visit(BlockStatement& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void Optimizer::visit(IfStatement& node) {
    node.condition->accept(*this);
    node.thenBranch->accept(*this);
    if (node.elseBranch) {
        node.elseBranch->accept(*this);
    }
}

void Optimizer::visit(ForStatement& node) {
    node.start->accept(*this);
    node.end->accept(*this);
    node.body->accept(*this);
}

void Optimizer::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

std::string Optimizer::getOptimizationReport() const {
    std::ostringstream oss;
    oss << "Optimization Report:\n";
    for (const auto& opt : optimizations) {
        oss << "  - " << opt << "\n";
    }
    return oss.str();
}
