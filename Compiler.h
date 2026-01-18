#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <memory>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semantic/SemanticAnalyzer.h"
#include "optimizer/Optimizer.h"
#include "codegen/CodeGenerator.h"
#include "vm/VirtualMachine.h"

struct CompilationResult {
    bool success;
    std::string errorMessage;
    
    // Stage outputs
    std::string tokensJSON;
    std::string astJSON;
    std::string semanticReport;
    std::string optimizationReport;
    std::string bytecodeJSON;
    std::string bytecodeText;
    std::string executionOutput;
    
    std::string toJSON() const;
};

class Compiler {
private:
    std::string sourceCode;
    CompilationResult result;
    
public:
    Compiler() = default;
    
    CompilationResult compile(const std::string& source);
    CompilationResult compileAndRun(const std::string& source);
    
    // Individual stage access for step-by-step visualization
    std::string tokenize(const std::string& source);
    std::string parse(const std::string& source);
    std::string analyze(const std::string& source);
    std::string optimize(const std::string& source);
    std::string generateCode(const std::string& source);
    std::string execute(const std::string& source);
};

#endif // COMPILER_H
