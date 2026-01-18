#include "AST.h"
#include <sstream>

std::string indent_str(int indent) {
    return std::string(indent * 2, ' ');
}

// NumberExpression
void NumberExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string NumberExpression::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"NumberExpression\",\n";
    oss << indent_str(indent + 1) << "\"value\": " << value << "\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}

// VariableExpression
void VariableExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string VariableExpression::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"VariableExpression\",\n";
    oss << indent_str(indent + 1) << "\"name\": \"" << name << "\"\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}

// BinaryExpression
void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BinaryExpression::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"BinaryExpression\",\n";
    oss << indent_str(indent + 1) << "\"operator\": \"" << op << "\",\n";
    oss << indent_str(indent + 1) << "\"left\": \n" << left->toJSON(indent + 1) << ",\n";
    oss << indent_str(indent + 1) << "\"right\": \n" << right->toJSON(indent + 1) << "\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}

// VariableDeclaration
void VariableDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string VariableDeclaration::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"VariableDeclaration\",\n";
    oss << indent_str(indent + 1) << "\"name\": \"" << name << "\",\n";
    oss << indent_str(indent + 1) << "\"initializer\": \n" << initializer->toJSON(indent + 1) << "\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}

// PrintStatement
void PrintStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string PrintStatement::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"PrintStatement\",\n";
    oss << indent_str(indent + 1) << "\"expression\": \n" << expression->toJSON(indent + 1) << "\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}

// BlockStatement
void BlockStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BlockStatement::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"BlockStatement\",\n";
    oss << indent_str(indent + 1) << "\"statements\": [\n";
    
    for (size_t i = 0; i < statements.size(); ++i) {
        oss << statements[i]->toJSON(indent + 2);
        if (i < statements.size() - 1) oss << ",";
        oss << "\n";
    }
    
    oss << indent_str(indent + 1) << "]\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}

// IfStatement
void IfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string IfStatement::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"IfStatement\",\n";
    oss << indent_str(indent + 1) << "\"condition\": \n" << condition->toJSON(indent + 1) << ",\n";
    oss << indent_str(indent + 1) << "\"thenBranch\": \n" << thenBranch->toJSON(indent + 1);
    
    if (elseBranch) {
        oss << ",\n" << indent_str(indent + 1) << "\"elseBranch\": \n" << elseBranch->toJSON(indent + 1);
    }
    
    oss << "\n" << indent_str(indent) << "}";
    return oss.str();
}

// ForStatement
void ForStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ForStatement::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"ForStatement\",\n";
    oss << indent_str(indent + 1) << "\"variable\": \"" << variable << "\",\n";
    oss << indent_str(indent + 1) << "\"start\": \n" << start->toJSON(indent + 1) << ",\n";
    oss << indent_str(indent + 1) << "\"end\": \n" << end->toJSON(indent + 1) << ",\n";
    oss << indent_str(indent + 1) << "\"body\": \n" << body->toJSON(indent + 1) << "\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}

// Program
void Program::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string Program::toJSON(int indent) const {
    std::ostringstream oss;
    oss << indent_str(indent) << "{\n";
    oss << indent_str(indent + 1) << "\"type\": \"Program\",\n";
    oss << indent_str(indent + 1) << "\"statements\": [\n";
    
    for (size_t i = 0; i < statements.size(); ++i) {
        oss << statements[i]->toJSON(indent + 2);
        if (i < statements.size() - 1) oss << ",";
        oss << "\n";
    }
    
    oss << indent_str(indent + 1) << "]\n";
    oss << indent_str(indent) << "}";
    return oss.str();
}
