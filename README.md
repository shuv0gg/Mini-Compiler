# 🎓 Educational Mini Compiler

A complete 6-stage educational compiler built from scratch in C++, with an interactive web interface for visualizing each compilation stage.

## 📌 Overview

This project demonstrates how real compilers work by implementing a mini programming language compiler that processes source code through six distinct stages and executes it on a stack-based virtual machine.

## 🌟 Features

### Compiler Pipeline

1. **Lexical Analysis** - Tokenizes source code into meaningful tokens
2. **Syntax Analysis** - Parses tokens into an Abstract Syntax Tree (AST)
3. **Semantic Analysis** - Checks for semantic errors (undefined variables, type errors)
4. **Code Optimization** - Performs constant folding and propagation
5. **Code Generation** - Generates stack-based bytecode
6. **Execution** - Runs bytecode on a virtual machine

### Web Interface

- Modern, responsive UI
- Monaco-style code editor
- Real-time visualization of all compilation stages
- 13+ built-in example programs
- Step-by-step execution

## 📚 Language Syntax

### Variable Declaration
```
let x = 10;
let y = 20;
```

### Print Statement
```
print x + y;
```

### If-Else Statement
```
if x > 5 {
    print x;
} else {
    print 0;
}
```

### For Loop
```
for i = 1 to 10 {
    print i;
}
```

### Operators
- Arithmetic: `+`, `-`, `*`, `/`
- Comparison: `>`, `<`, `==`
- Assignment: `=`

## 🛠️ Building the Project

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Make (optional)
- CMake (optional)

### Using Make

```bash
# Build the compiler
make

# Run the server
make run
```

### Using CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Manual Compilation (Windows)

```bash
g++ -std=c++17 -I. main.cpp Compiler.cpp lexer/Lexer.cpp ast/AST.cpp parser/Parser.cpp semantic/SemanticAnalyzer.cpp optimizer/Optimizer.cpp codegen/Bytecode.cpp codegen/CodeGenerator.cpp vm/VirtualMachine.cpp -o compiler.exe -lws2_32
```

### Manual Compilation (Linux/Mac)

```bash
g++ -std=c++17 -I. main.cpp Compiler.cpp lexer/Lexer.cpp ast/AST.cpp parser/Parser.cpp semantic/SemanticAnalyzer.cpp optimizer/Optimizer.cpp codegen/Bytecode.cpp codegen/CodeGenerator.cpp vm/VirtualMachine.cpp -o compiler
```

## 🚀 Running the Compiler

### Start the Web Server

```bash
./compiler --server
```

Then open your browser to: `http://localhost:8080`

### Command-Line Mode

```bash
./compiler
```

This runs a test program to verify the compiler works correctly.

## 📂 Project Structure

```
compiler/
├── lexer/           # Lexical analysis (tokenization)
│   ├── Token.h
│   ├── Lexer.h
│   └── Lexer.cpp
├── ast/             # Abstract Syntax Tree
│   ├── AST.h
│   └── AST.cpp
├── parser/          # Syntax analysis (parsing)
│   ├── Parser.h
│   └── Parser.cpp
├── semantic/        # Semantic analysis
│   ├── SemanticAnalyzer.h
│   └── SemanticAnalyzer.cpp
├── optimizer/       # Code optimization
│   ├── Optimizer.h
│   └── Optimizer.cpp
├── codegen/         # Code generation
│   ├── Bytecode.h
│   ├── Bytecode.cpp
│   ├── CodeGenerator.h
│   └── CodeGenerator.cpp
├── vm/              # Virtual Machine
│   ├── VirtualMachine.h
│   └── VirtualMachine.cpp
├── examples/        # Example programs
│   ├── 01_arithmetic.txt
│   ├── 02_simple_if.txt
│   └── ... (13 examples total)
├── web/             # Web interface
│   ├── index.html
│   ├── style.css
│   └── script.js
├── Compiler.h       # Main compiler interface
├── Compiler.cpp
├── main.cpp         # Entry point with HTTP server
├── Makefile
├── CMakeLists.txt
└── README.md
```

## 🎯 Example Programs

1. **Arithmetic Operations** - Basic math operations
2. **Simple If** - Basic conditional statement
3. **If-Else** - Conditional with else branch
4. **For Loop** - Basic loop
5. **Nested Loops** - Loops within loops
6. **Complex Expression** - Multiple operations
7. **Comparison** - Equality testing
8. **Optimization Demo** - Demonstrates constant folding
9. **Loop Arithmetic** - Sum calculation
10. **Factorial** - Iterative factorial calculation
11. **Conditional Loop** - If inside loop
12. **Error Demo** - Shows undefined variable error
13. **Parentheses** - Expression grouping

## 🧠 Technical Details

### Bytecode Instructions

- `PUSH n` - Push constant n onto stack
- `LOAD i` - Load variable i onto stack
- `STORE i` - Store top of stack to variable i
- `ADD/SUB/MUL/DIV` - Arithmetic operations
- `GT/LT/EQ` - Comparison operations
- `JMP addr` - Unconditional jump
- `JMP_IF_FALSE addr` - Conditional jump
- `PRINT` - Print top of stack
- `HALT` - Stop execution

### Optimizations

- **Constant Folding**: `2 + 3` → `5`
- **Constant Propagation**: `let x = 5; y = x + 1` → `y = 6`

### Semantic Checks

- Undefined variable detection
- Variable redeclaration warnings
- Type checking (integers only)

## 🔧 Extending the Compiler

To add new features:

1. **New Operators**: Modify `Lexer.cpp` and add cases in `Parser.cpp`
2. **New Statements**: Add AST nodes in `AST.h` and parsing logic in `Parser.cpp`
3. **New Optimizations**: Extend `Optimizer.cpp`
4. **New Data Types**: Extend semantic analyzer and VM

## 📝 License

This is an educational project. Feel free to use, modify, and learn from it!

## 🙏 Acknowledgments

This compiler demonstrates fundamental concepts taught in compiler design courses, including:
- Lexical analysis and tokenization
- Recursive descent parsing
- Abstract Syntax Trees
- Symbol tables
- Code optimization techniques
- Bytecode generation
- Stack-based virtual machines

## 🐛 Known Limitations

- Only supports integer arithmetic
- No function definitions
- No arrays or complex data structures
- Limited error recovery in parser
- Single-pass optimization

## 📚 Learning Resources

To understand the compiler better, study the files in this order:

1. `lexer/Token.h` - Token definitions
2. `lexer/Lexer.cpp` - Tokenization logic
3. `ast/AST.h` - AST node structure
4. `parser/Parser.cpp` - Parsing logic
5. `semantic/SemanticAnalyzer.cpp` - Semantic checks
6. `optimizer/Optimizer.cpp` - Optimization passes
7. `codegen/CodeGenerator.cpp` - Bytecode generation
8. `vm/VirtualMachine.cpp` - Execution engine

---

**Built with ❤️ for education and learning**
