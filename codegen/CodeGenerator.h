#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <map>
#include <string>
#include <vector>
#include "../ast/AST.h"
#include "Bytecode.h"

class CodeGenerator : public ASTVisitor {
private:
    Bytecode bytecode;
    std::map<std::string, int> variableIndices;
    int nextVariableIndex;
    
    int getVariableIndex(const std::string& name);
    
public:
    CodeGenerator() : nextVariableIndex(0) {}
    
    Bytecode generate(Program& program);
    
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
};

#endif // CODE_GENERATOR_H
