#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <vector>
#include <string>
#include <map>
#include "../codegen/Bytecode.h"

class VirtualMachine {
private:
    std::vector<int> stack;
    std::map<int, int> variables; // variable index -> value
    std::vector<std::string> output;
    int programCounter;
    bool halted;
    
    void push(int value);
    int pop();
    int peek();
    
public:
    VirtualMachine() : programCounter(0), halted(false) {}
    
    void execute(const Bytecode& bytecode);
    const std::vector<std::string>& getOutput() const { return output; }
    
    std::string getOutputString() const;
};

#endif // VIRTUAL_MACHINE_H
