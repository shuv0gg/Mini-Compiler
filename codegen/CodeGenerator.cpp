#include "CodeGenerator.h"

int CodeGenerator::getVariableIndex(const std::string& name) {
    auto it = variableIndices.find(name);
    if (it != variableIndices.end()) {
        return it->second;
    }
    
    int index = nextVariableIndex++;
    variableIndices[name] = index;
    return index;
}

Bytecode CodeGenerator::generate(Program& program) {
    bytecode = Bytecode();
    variableIndices.clear();
    nextVariableIndex = 0;
    
    program.accept(*this);
    bytecode.emit(OpCode::HALT);
    
    return bytecode;
}

void CodeGenerator::visit(NumberExpression& node) {
    bytecode.emit(OpCode::PUSH, node.value);
}

void CodeGenerator::visit(VariableExpression& node) {
    int index = getVariableIndex(node.name);
    bytecode.emit(OpCode::LOAD, index);
}

void CodeGenerator::visit(BinaryExpression& node) {
    // Evaluate left and right operands
    node.left->accept(*this);
    node.right->accept(*this);
    
    // Emit operation
    if (node.op == "+") {
        bytecode.emit(OpCode::ADD);
    } else if (node.op == "-") {
        bytecode.emit(OpCode::SUB);
    } else if (node.op == "*") {
        bytecode.emit(OpCode::MUL);
    } else if (node.op == "/") {
        bytecode.emit(OpCode::DIV);
    } else if (node.op == ">") {
        bytecode.emit(OpCode::GT);
    } else if (node.op == "<") {
        bytecode.emit(OpCode::LT);
    } else if (node.op == "==") {
        bytecode.emit(OpCode::EQ);
    }
}

void CodeGenerator::visit(VariableDeclaration& node) {
    // Evaluate initializer
    node.initializer->accept(*this);
    
    // Store to variable
    int index = getVariableIndex(node.name);
    bytecode.emit(OpCode::STORE, index);
}

void CodeGenerator::visit(PrintStatement& node) {
    node.expression->accept(*this);
    bytecode.emit(OpCode::PRINT);
}

void CodeGenerator::visit(BlockStatement& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void CodeGenerator::visit(IfStatement& node) {
    // Evaluate condition
    node.condition->accept(*this);
    
    // Jump to else if condition is false
    int jumpToElse = bytecode.getCurrentAddress();
    bytecode.emit(OpCode::JMP_IF_FALSE, 0); // Placeholder
    
    // Then branch
    node.thenBranch->accept(*this);
    
    if (node.elseBranch) {
        // Jump over else branch
        int jumpOverElse = bytecode.getCurrentAddress();
        bytecode.emit(OpCode::JMP, 0); // Placeholder
        
        // Patch jump to else
        int elseStart = bytecode.getCurrentAddress();
        bytecode.patchJump(jumpToElse, elseStart);
        
        // Else branch
        node.elseBranch->accept(*this);
        
        // Patch jump over else
        int afterElse = bytecode.getCurrentAddress();
        bytecode.patchJump(jumpOverElse, afterElse);
    } else {
        // Patch jump to end
        int afterIf = bytecode.getCurrentAddress();
        bytecode.patchJump(jumpToElse, afterIf);
    }
}

void CodeGenerator::visit(ForStatement& node) {
    // Initialize loop variable
    node.start->accept(*this);
    int loopVarIndex = getVariableIndex(node.variable);
    bytecode.emit(OpCode::STORE, loopVarIndex);
    
    // Loop start - check condition
    int loopStart = bytecode.getCurrentAddress();
    
    // Check condition: loopVar <= end
    // We compute: loopVar > end, and if true (1), we exit
    bytecode.emit(OpCode::LOAD, loopVarIndex);
    node.end->accept(*this);
    bytecode.emit(OpCode::GT); // Stack now has: (loopVar > end) ? 1 : 0
    
    // If loopVar > end is true (1), we should exit the loop
    // JMP_IF_FALSE jumps when stack is 0 (false)
    // So if stack is 0 (loopVar <= end), we jump to body (continue loop)
    // If stack is 1 (loopVar > end), we don't jump, fall through to exit
    int jumpToBody = bytecode.getCurrentAddress();
    bytecode.emit(OpCode::JMP_IF_FALSE, 0); // Will be patched to body address
    
    // If we reach here, loopVar > end, so exit loop
    int exitJump = bytecode.getCurrentAddress();
    bytecode.emit(OpCode::JMP, 0); // Will be patched to after-loop address
    
    // Loop body starts here
    int bodyStart = bytecode.getCurrentAddress();
    bytecode.patchJump(jumpToBody, bodyStart);
    
    // Execute loop body
    node.body->accept(*this);
    
    // Increment loop variable
    bytecode.emit(OpCode::LOAD, loopVarIndex);
    bytecode.emit(OpCode::PUSH, 1);
    bytecode.emit(OpCode::ADD);
    bytecode.emit(OpCode::STORE, loopVarIndex);
    
    // Jump back to loop start
    bytecode.emit(OpCode::JMP, loopStart);
    
    // After loop
    int afterLoop = bytecode.getCurrentAddress();
    bytecode.patchJump(exitJump, afterLoop);
}

void CodeGenerator::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}
