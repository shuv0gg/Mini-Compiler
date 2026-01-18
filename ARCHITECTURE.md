# Compiler Architecture

## Overview

This educational mini compiler implements a complete 6-stage compilation pipeline for a simple imperative programming language. The compiler processes source code through lexical analysis, syntax analysis, semantic analysis, optimization, code generation, and finally execution on a stack-based virtual machine.

## Pipeline Stages

### 1. Lexical Analysis (Lexer)

**Location**: `lexer/Lexer.cpp`

**Purpose**: Converts raw source code into a stream of tokens.

**Process**:
- Scans source code character by character
- Recognizes keywords (let, if, else, for, to, print)
- Identifies operators (+, -, *, /, >, <, ==, =)
- Extracts identifiers and numeric literals
- Tracks line and column numbers for error reporting

**Output**: Vector of Token objects with type, lexeme, and position information

### 2. Syntax Analysis (Parser)

**Location**: `parser/Parser.cpp`

**Purpose**: Builds an Abstract Syntax Tree (AST) from tokens using recursive descent parsing.

**Grammar**:
```
Program     → Statement*
Statement   → VarDecl | PrintStmt | IfStmt | ForStmt | Block
VarDecl     → 'let' IDENTIFIER '=' Expression ';'
PrintStmt   → 'print' Expression ';'
IfStmt      → 'if' Expression '{' Statement* '}' ('else' '{' Statement* '}')?
ForStmt     → 'for' IDENTIFIER '=' Expression 'to' Expression '{' Statement* '}'
Expression  → Comparison
Comparison  → Term (('>' | '<' | '==') Term)?
Term        → Factor (('+' | '-') Factor)*
Factor      → Primary (('*' | '/') Primary)*
Primary     → NUMBER | IDENTIFIER | '(' Expression ')'
```

**Output**: AST representing program structure

### 3. Semantic Analysis

**Location**: `semantic/SemanticAnalyzer.cpp`

**Purpose**: Validates program semantics and builds symbol table.

**Checks**:
- Undefined variable detection
- Variable redeclaration warnings
- Type consistency (integer-only system)
- Scope validation

**Output**: Error/warning list and validated AST

### 4. Code Optimization

**Location**: `optimizer/Optimizer.cpp`

**Purpose**: Improves code efficiency through compile-time optimizations.

**Optimizations**:
- **Constant Folding**: Evaluates constant expressions at compile time
  - Example: `2 + 3` → `5`
  - Example: `10 * 2` → `20`
- **Constant Propagation**: Substitutes known constant values
  - Example: `let x = 5; y = x + 1` → `y = 6`

**Output**: Optimized AST and optimization report

### 5. Code Generation

**Location**: `codegen/CodeGenerator.cpp`

**Purpose**: Generates stack-based bytecode from AST.

**Bytecode Instructions**:
- `PUSH n` - Push constant n onto stack
- `LOAD i` - Load variable i onto stack
- `STORE i` - Store top of stack to variable i
- `ADD/SUB/MUL/DIV` - Arithmetic operations
- `GT/LT/EQ` - Comparison operations
- `JMP addr` - Unconditional jump to address
- `JMP_IF_FALSE addr` - Jump if top of stack is 0
- `PRINT` - Print top of stack
- `HALT` - Stop execution

**Example** (for `let x = 5; print x;`):
```
0: PUSH 5      # Push constant 5
1: STORE 0     # Store to variable 0 (x)
2: LOAD 0      # Load variable 0
3: PRINT       # Print value
4: HALT        # Stop
```

**Output**: Bytecode object with instruction sequence

### 6. Execution (Virtual Machine)

**Location**: `vm/VirtualMachine.cpp`

**Purpose**: Executes bytecode on a stack-based virtual machine.

**Components**:
- **Stack**: Stores intermediate computation results
- **Variables**: Maps variable indices to values
- **Program Counter**: Tracks current instruction
- **Output**: Collects print statements

**Execution Model**:
1. Load instruction at program counter
2. Execute instruction (modify stack/variables/PC)
3. Repeat until HALT or end of program

**Output**: Program execution results

## Data Structures

### AST Nodes

- `Program` - Root node containing statements
- `VariableDeclaration` - Variable definition with initializer
- `PrintStatement` - Output statement
- `IfStatement` - Conditional with optional else
- `ForStatement` - Loop with range
- `BlockStatement` - Statement grouping
- `BinaryExpression` - Binary operations
- `NumberExpression` - Numeric literal
- `VariableExpression` - Variable reference

### Visitor Pattern

The compiler uses the Visitor pattern for AST traversal, allowing different operations (semantic analysis, optimization, code generation) without modifying AST node classes.

## Control Flow Implementation

### If-Else Statements

```
Condition evaluation
JMP_IF_FALSE else_label    # Jump if condition false
  Then-branch instructions
  JMP end_label             # Skip else branch
else_label:
  Else-branch instructions
end_label:
  Continue...
```

### For Loops

```
Initialize loop variable
loop_start:
  LOAD loop_var
  PUSH end_value
  GT                        # Check if loop_var > end
  JMP_IF_FALSE body        # Continue if false
  JMP after_loop           # Exit if true
body:
  Loop body instructions
  Increment loop variable
  JMP loop_start           # Repeat
after_loop:
  Continue...
```

## Error Handling

### Lexical Errors
- Invalid characters
- Malformed tokens

### Syntax Errors
- Unexpected tokens
- Missing delimiters
- Invalid statement structure

### Semantic Errors
- Undefined variables
- Type mismatches
- Invalid operations

### Runtime Errors
- Division by zero
- Stack underflow
- Invalid jumps

## Web Interface

**Location**: `web/`

**Components**:
- `index.html` - UI layout and structure
- `style.css` - Styling and responsive design
- `script.js` - Frontend logic and API calls

**Features**:
- Code editor with example programs
- Real-time compilation
- Stage-by-stage visualization
- Execution output display

**API**:
- `POST /compile` - Compiles and runs source code
- `GET /examples/` - Loads example programs
- Returns JSON with all pipeline stage outputs

## HTTP Server

**Location**: `main.cpp`

**Implementation**: Simple socket-based HTTP server for Windows/Linux

**Endpoints**:
- `/` - Serves index.html
- `/style.css` - Serves CSS
- `/script.js` - Serves JavaScript
- `/compile` - Compilation API
- `/examples/*` - Example files

## Performance Characteristics

- **Lexer**: O(n) where n is source code length
- **Parser**: O(n) for well-formed code
- **Semantic Analysis**: O(n) where n is number of nodes
- **Optimization**: O(n) per pass
- **Code Generation**: O(n)
- **Execution**: O(i) where i is number of instructions

## Memory Management

- Smart pointers (unique_ptr) for AST nodes
- Automatic cleanup via RAII
- No manual memory management required

## Extensibility

### Adding New Operators

1. Add token type in `Token.h`
2. Update lexer in `Lexer.cpp`
3. Add parsing logic in `Parser.cpp`
4. Add bytecode instruction in `Bytecode.h`
5. Implement VM execution in `VirtualMachine.cpp`

### Adding New Statements

1. Create AST node class in `AST.h`
2. Add visitor method
3. Implement parsing in `Parser.cpp`
4. Add semantic checks in `SemanticAnalyzer.cpp`
5. Implement code generation in `CodeGenerator.cpp`

### Adding New Data Types

1. Extend Token and AST for type information
2. Update semantic analyzer for type checking
3. Extend VM for new type operations
4. Update optimization passes

## Testing Strategy

- Unit tests for each stage
- Integration tests for full pipeline
- Example programs covering all features
- Error case validation
- Performance benchmarks

## Build System

- **CMake**: Cross-platform build configuration
- **Makefile**: Direct compilation option
- **build.bat**: Windows convenience script
- **Requirements**: C++17 compiler, standard library

## Documentation

- `README.md` - User guide and getting started
- `ARCHITECTURE.md` - This file, technical details
- Inline code comments for implementation details
- Example programs with explanations
