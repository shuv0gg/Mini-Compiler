#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../ast/AST.h"

class SemanticAnalyzer : public ASTVisitor {
private:
    std::map<std::string, bool> symbolTable; // variable name -> is defined
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    void defineVariable(const std::string& name);
    bool isVariableDefined(const std::string& name);
    void error(const std::string& message);
    void warning(const std::string& message);
    
public:
    SemanticAnalyzer() = default;
    
    void analyze(Program& program);
    
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
    
    const std::vector<std::string>& getErrors() const { return errors; }
    const std::vector<std::string>& getWarnings() const { return warnings; }
    bool hasErrors() const { return !errors.empty(); }
    
    std::string getReport() const;
};

#endif // SEMANTIC_ANALYZER_H
