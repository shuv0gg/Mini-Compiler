#include "Compiler.h"
#include <sstream>
#include <iomanip>

// Helper function to escape strings for JSON
static std::string escapeJSON(const std::string& str) {
    std::ostringstream oss;
    for (char c : str) {
        switch (c) {
            case '"':  oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b"; break;
            case '\f': oss << "\\f"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    oss << c;
                }
        }
    }
    return oss.str();
}

std::string CompilationResult::toJSON() const {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"success\": " << (success ? "true" : "false") << ",\n";
    
    if (!success) {
        oss << "  \"error\": \"" << escapeJSON(errorMessage) << "\",\n";
    }
    
    oss << "  \"tokens\": " << tokensJSON << ",\n";
    oss << "  \"ast\": " << astJSON << ",\n";
    oss << "  \"semantic\": \"" << escapeJSON(semanticReport) << "\",\n";
    oss << "  \"optimization\": \"" << escapeJSON(optimizationReport) << "\",\n";
    oss << "  \"bytecode\": " << bytecodeJSON << ",\n";
    oss << "  \"bytecodeText\": \"" << escapeJSON(bytecodeText) << "\",\n";
    oss << "  \"output\": \"" << escapeJSON(executionOutput) << "\"\n";
    oss << "}";
    
    return oss.str();
}

CompilationResult Compiler::compile(const std::string& source) {
    result = CompilationResult();
    result.success = true;
    
    try {
        // Stage 1: Lexical Analysis
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        std::ostringstream tokensOss;
        tokensOss << "[\n";
        for (size_t i = 0; i < tokens.size(); ++i) {
            tokensOss << "  {\"type\": \"" << tokens[i].getTypeName() 
                     << "\", \"lexeme\": \"" << tokens[i].lexeme 
                     << "\", \"line\": " << tokens[i].line 
                     << ", \"column\": " << tokens[i].column << "}";
            if (i < tokens.size() - 1) tokensOss << ",";
            tokensOss << "\n";
        }
        tokensOss << "]";
        result.tokensJSON = tokensOss.str();
        
        // Stage 2: Syntax Analysis
        Parser parser(tokens);
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            result.success = false;
            std::ostringstream errOss;
            for (const auto& err : parser.getErrors()) {
                errOss << err << "\\n";
            }
            result.errorMessage = errOss.str();
            result.astJSON = "{}";
            return result;
        }
        
        result.astJSON = program->toJSON();
        
        // Stage 3: Semantic Analysis
        SemanticAnalyzer analyzer;
        analyzer.analyze(*program);
        result.semanticReport = analyzer.getReport();
        
        if (analyzer.hasErrors()) {
            result.success = false;
            result.errorMessage = result.semanticReport;
            return result;
        }
        
        // Stage 4: Optimization
        Optimizer optimizer;
        program = optimizer.optimize(std::move(program));
        result.optimizationReport = optimizer.getOptimizationReport();
        
        // Stage 5: Code Generation
        CodeGenerator codegen;
        Bytecode bytecode = codegen.generate(*program);
        result.bytecodeJSON = bytecode.toJSON();
        result.bytecodeText = bytecode.toString();
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errorMessage = std::string("Compilation error: ") + e.what();
    }
    
    return result;
}

CompilationResult Compiler::compileAndRun(const std::string& source) {
    result = compile(source);
    
    if (!result.success) {
        return result;
    }
    
    try {
        // Stage 6: Execution
        // Recompile to get bytecode
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto program = parser.parse();
        SemanticAnalyzer analyzer;
        analyzer.analyze(*program);
        Optimizer optimizer;
        program = optimizer.optimize(std::move(program));
        CodeGenerator codegen;
        Bytecode bytecode = codegen.generate(*program);
        
        VirtualMachine vm;
        vm.execute(bytecode);
        result.executionOutput = vm.getOutputString();
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errorMessage = std::string("Execution error: ") + e.what();
    }
    
    return result;
}

std::string Compiler::tokenize(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    std::ostringstream oss;
    oss << "[\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
        oss << "  {\"type\": \"" << tokens[i].getTypeName() 
           << "\", \"lexeme\": \"" << tokens[i].lexeme 
           << "\", \"line\": " << tokens[i].line 
           << ", \"column\": " << tokens[i].column << "}";
        if (i < tokens.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "]";
    return oss.str();
}

std::string Compiler::parse(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto program = parser.parse();
    return program->toJSON();
}

std::string Compiler::analyze(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto program = parser.parse();
    SemanticAnalyzer analyzer;
    analyzer.analyze(*program);
    return analyzer.getReport();
}

std::string Compiler::optimize(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto program = parser.parse();
    SemanticAnalyzer analyzer;
    analyzer.analyze(*program);
    Optimizer optimizer;
    program = optimizer.optimize(std::move(program));
    return optimizer.getOptimizationReport();
}

std::string Compiler::generateCode(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto program = parser.parse();
    SemanticAnalyzer analyzer;
    analyzer.analyze(*program);
    Optimizer optimizer;
    program = optimizer.optimize(std::move(program));
    CodeGenerator codegen;
    Bytecode bytecode = codegen.generate(*program);
    return bytecode.toString();
}

std::string Compiler::execute(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto program = parser.parse();
    SemanticAnalyzer analyzer;
    analyzer.analyze(*program);
    Optimizer optimizer;
    program = optimizer.optimize(std::move(program));
    CodeGenerator codegen;
    Bytecode bytecode = codegen.generate(*program);
    VirtualMachine vm;
    vm.execute(bytecode);
    return vm.getOutputString();
}
