#include "SemanticAnalyzer.h"
#include <sstream>

void SemanticAnalyzer::defineVariable(const std::string& name) {
    if (symbolTable.find(name) != symbolTable.end()) {
        warning("Variable '" + name + "' redeclared");
    }
    symbolTable[name] = true;
}

bool SemanticAnalyzer::isVariableDefined(const std::string& name) {
    return symbolTable.find(name) != symbolTable.end();
}

void SemanticAnalyzer::error(const std::string& message) {
    errors.push_back("Semantic error: " + message);
}

void SemanticAnalyzer::warning(const std::string& message) {
    warnings.push_back("Warning: " + message);
}

void SemanticAnalyzer::analyze(Program& program) {
    errors.clear();
    warnings.clear();
    symbolTable.clear();
    program.accept(*this);
}

void SemanticAnalyzer::visit(NumberExpression& node) {
    // Numbers are always valid
}

void SemanticAnalyzer::visit(VariableExpression& node) {
    if (!isVariableDefined(node.name)) {
        error("Undefined variable '" + node.name + "'");
    }
}

void SemanticAnalyzer::visit(BinaryExpression& node) {
    node.left->accept(*this);
    node.right->accept(*this);
}

void SemanticAnalyzer::visit(VariableDeclaration& node) {
    // First check the initializer
    node.initializer->accept(*this);
    // Then define the variable
    defineVariable(node.name);
}

void SemanticAnalyzer::visit(PrintStatement& node) {
    node.expression->accept(*this);
}

void SemanticAnalyzer::visit(BlockStatement& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(IfStatement& node) {
    node.condition->accept(*this);
    node.thenBranch->accept(*this);
    if (node.elseBranch) {
        node.elseBranch->accept(*this);
    }
}

void SemanticAnalyzer::visit(ForStatement& node) {
    node.start->accept(*this);
    node.end->accept(*this);
    
    // Define loop variable
    defineVariable(node.variable);
    
    // Analyze body
    node.body->accept(*this);
}

void SemanticAnalyzer::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

std::string SemanticAnalyzer::getReport() const {
    std::ostringstream oss;
    
    if (errors.empty() && warnings.empty()) {
        oss << "Semantic analysis passed with no errors or warnings.\n";
    } else {
        if (!errors.empty()) {
            oss << "Errors:\n";
            for (const auto& err : errors) {
                oss << "  - " << err << "\n";
            }
        }
        if (!warnings.empty()) {
            oss << "Warnings:\n";
            for (const auto& warn : warnings) {
                oss << "  - " << warn << "\n";
            }
        }
    }
    
    return oss.str();
}
