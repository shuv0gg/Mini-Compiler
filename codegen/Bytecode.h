#ifndef BYTECODE_H
#define BYTECODE_H

#include <string>
#include <vector>

enum class OpCode {
    PUSH,        // Push constant onto stack
    LOAD,        // Load variable onto stack
    STORE,       // Store top of stack to variable
    ADD,         // Add top two stack values
    SUB,         // Subtract
    MUL,         // Multiply
    DIV,         // Divide
    GT,          // Greater than
    LT,          // Less than
    EQ,          // Equal
    JMP,         // Unconditional jump
    JMP_IF_FALSE,// Jump if top of stack is false
    PRINT,       // Print top of stack
    HALT         // Stop execution
};

struct Instruction {
    OpCode opcode;
    int operand;  // Used for PUSH (value), LOAD/STORE (var index), JMP (address)
    
    Instruction(OpCode op, int oper = 0) : opcode(op), operand(oper) {}
    
    std::string toString() const;
};

class Bytecode {
private:
    std::vector<Instruction> instructions;
    
public:
    void emit(OpCode opcode, int operand = 0);
    void patchJump(int jumpIndex, int targetAddress);
    int getCurrentAddress() const { return instructions.size(); }
    const std::vector<Instruction>& getInstructions() const { return instructions; }
    
    std::string toString() const;
    std::string toJSON() const;
};

#endif // BYTECODE_H
