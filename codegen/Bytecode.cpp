#include "Bytecode.h"
#include <sstream>
#include <map>

static std::map<OpCode, std::string> opcodeNames = {
    {OpCode::PUSH, "PUSH"},
    {OpCode::LOAD, "LOAD"},
    {OpCode::STORE, "STORE"},
    {OpCode::ADD, "ADD"},
    {OpCode::SUB, "SUB"},
    {OpCode::MUL, "MUL"},
    {OpCode::DIV, "DIV"},
    {OpCode::GT, "GT"},
    {OpCode::LT, "LT"},
    {OpCode::EQ, "EQ"},
    {OpCode::JMP, "JMP"},
    {OpCode::JMP_IF_FALSE, "JMP_IF_FALSE"},
    {OpCode::PRINT, "PRINT"},
    {OpCode::HALT, "HALT"}
};

std::string Instruction::toString() const {
    std::ostringstream oss;
    oss << opcodeNames[opcode];
    if (opcode == OpCode::PUSH || opcode == OpCode::LOAD || 
        opcode == OpCode::STORE || opcode == OpCode::JMP || 
        opcode == OpCode::JMP_IF_FALSE) {
        oss << " " << operand;
    }
    return oss.str();
}

void Bytecode::emit(OpCode opcode, int operand) {
    instructions.push_back(Instruction(opcode, operand));
}

void Bytecode::patchJump(int jumpIndex, int targetAddress) {
    if (jumpIndex >= 0 && jumpIndex < instructions.size()) {
        instructions[jumpIndex].operand = targetAddress;
    }
}

std::string Bytecode::toString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < instructions.size(); ++i) {
        oss << i << ": " << instructions[i].toString() << "\n";
    }
    return oss.str();
}

std::string Bytecode::toJSON() const {
    std::ostringstream oss;
    oss << "[\n";
    for (size_t i = 0; i < instructions.size(); ++i) {
        oss << "  {\n";
        oss << "    \"address\": " << i << ",\n";
        oss << "    \"opcode\": \"" << opcodeNames[instructions[i].opcode] << "\"";
        
        if (instructions[i].opcode == OpCode::PUSH || 
            instructions[i].opcode == OpCode::LOAD || 
            instructions[i].opcode == OpCode::STORE || 
            instructions[i].opcode == OpCode::JMP || 
            instructions[i].opcode == OpCode::JMP_IF_FALSE) {
            oss << ",\n    \"operand\": " << instructions[i].operand;
        }
        
        oss << "\n  }";
        if (i < instructions.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "]";
    return oss.str();
}
