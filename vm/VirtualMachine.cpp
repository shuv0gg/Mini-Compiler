#include "VirtualMachine.h"
#include <sstream>
#include <stdexcept>

void VirtualMachine::push(int value) {
    stack.push_back(value);
}

int VirtualMachine::pop() {
    if (stack.empty()) {
        throw std::runtime_error("Stack underflow");
    }
    int value = stack.back();
    stack.pop_back();
    return value;
}

int VirtualMachine::peek() {
    if (stack.empty()) {
        throw std::runtime_error("Stack is empty");
    }
    return stack.back();
}

void VirtualMachine::execute(const Bytecode& bytecode) {
    stack.clear();
    variables.clear();
    output.clear();
    programCounter = 0;
    halted = false;
    
    const auto& instructions = bytecode.getInstructions();
    
    while (programCounter < instructions.size() && !halted) {
        const Instruction& instr = instructions[programCounter];
        
        switch (instr.opcode) {
            case OpCode::PUSH:
                push(instr.operand);
                programCounter++;
                break;
                
            case OpCode::LOAD:
                push(variables[instr.operand]);
                programCounter++;
                break;
                
            case OpCode::STORE: {
                int value = pop();
                variables[instr.operand] = value;
                programCounter++;
                break;
            }
                
            case OpCode::ADD: {
                int right = pop();
                int left = pop();
                push(left + right);
                programCounter++;
                break;
            }
                
            case OpCode::SUB: {
                int right = pop();
                int left = pop();
                push(left - right);
                programCounter++;
                break;
            }
                
            case OpCode::MUL: {
                int right = pop();
                int left = pop();
                push(left * right);
                programCounter++;
                break;
            }
                
            case OpCode::DIV: {
                int right = pop();
                int left = pop();
                if (right == 0) {
                    throw std::runtime_error("Division by zero");
                }
                push(left / right);
                programCounter++;
                break;
            }
                
            case OpCode::GT: {
                int right = pop();
                int left = pop();
                push(left > right ? 1 : 0);
                programCounter++;
                break;
            }
                
            case OpCode::LT: {
                int right = pop();
                int left = pop();
                push(left < right ? 1 : 0);
                programCounter++;
                break;
            }
                
            case OpCode::EQ: {
                int right = pop();
                int left = pop();
                push(left == right ? 1 : 0);
                programCounter++;
                break;
            }
                
            case OpCode::JMP:
                programCounter = instr.operand;
                break;
                
            case OpCode::JMP_IF_FALSE: {
                int condition = pop();
                if (condition == 0) {
                    programCounter = instr.operand;
                } else {
                    programCounter++;
                }
                break;
            }
                
            case OpCode::PRINT: {
                int value = pop();
                std::ostringstream oss;
                oss << value;
                output.push_back(oss.str());
                programCounter++;
                break;
            }
                
            case OpCode::HALT:
                halted = true;
                break;
                
            default:
                throw std::runtime_error("Unknown opcode");
        }
    }
}

std::string VirtualMachine::getOutputString() const {
    std::ostringstream oss;
    for (const auto& line : output) {
        oss << line << "\n";
    }
    return oss.str();
}
