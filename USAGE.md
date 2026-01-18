# Usage Guide

## Quick Start

### 1. Build the Compiler

**Windows**:
```bash
build.bat
```

**Linux/Mac**:
```bash
make
```

**Or using CMake**:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 2. Run the Web Server

```bash
# Windows
run.bat

# Linux/Mac
./compiler --server
```

Then open your browser to: http://localhost:8080

### 3. Command-Line Usage

```bash
# Run the test program
./compiler

# Start web server
./compiler --server
```

## Language Tutorial

### Variables

Variables must be declared with the `let` keyword and initialized:

```
let x = 10;
let y = 20;
let sum = x + y;
```

### Arithmetic Operations

Supported operators: `+` `-` `*` `/`

```
let a = 10;
let b = 3;
let sum = a + b;      // 13
let diff = a - b;     // 7
let prod = a * b;     // 30
let quot = a / b;     // 3 (integer division)
```

**Operator Precedence** (highest to lowest):
1. Parentheses `( )`
2. Multiplication `*` and Division `/`
3. Addition `+` and Subtraction `-`

Examples:
```
let result1 = 2 + 3 * 4;        // 14 (not 20)
let result2 = (2 + 3) * 4;      // 20
let result3 = 10 - 2 * 3;       // 4 (not 24)
```

### Print Statement

Print expressions to output:

```
print 42;
print x + y;
print (a + b) * c;
```

### Comparison Operators

Supported: `>` `<` `==`

Comparisons return 1 (true) or 0 (false):

```
let a = 10;
let b = 5;
let isGreater = a > b;    // 1
let isLess = a < b;       // 0
let isEqual = a == b;     // 0
```

### If Statements

Basic if:
```
let x = 10;
if x > 5 {
    print x;
}
```

If-else:
```
let x = 3;
if x > 5 {
    print 1;
} else {
    print 0;
}
```

Nested if:
```
let x = 10;
let y = 20;
if x > 5 {
    if y > 15 {
        print 1;
    }
}
```

### For Loops

Basic for loop (inclusive range):
```
for i = 1 to 5 {
    print i;
}
// Prints: 1 2 3 4 5
```

Loop with expressions:
```
let start = 1;
let end = 10;
for i = start to end {
    print i;
}
```

Nested loops:
```
for i = 1 to 3 {
    for j = 1 to 3 {
        print i;
    }
}
```

Loop with arithmetic:
```
let sum = 0;
for i = 1 to 10 {
    sum = sum + i;
}
print sum;  // Prints 55
```

## Web Interface Features

### Code Editor

- Syntax highlighting
- Line numbers
- Multi-line editing
- Copy/paste support

### Example Programs

Select from 13 built-in examples:
1. Arithmetic Operations
2. Simple If Statement
3. If-Else Statement
4. For Loop
5. Nested Loops
6. Complex Expression
7. Comparison Operator
8. Optimization Demo
9. Loop with Arithmetic
10. Factorial (Iterative)
11. Conditional in Loop
12. Error: Undefined Variable
13. Parentheses Expression

### Compilation Stages

View each stage of compilation:

1. **Tokens** - Lexical analysis output
2. **AST** - Abstract Syntax Tree (JSON format)
3. **Semantic** - Symbol table and error checking
4. **Optimizer** - Optimization report
5. **Bytecode** - Generated VM instructions
6. **Output** - Program execution results

### Buttons

- **Compile** - Run compilation and show all stages
- **Compile & Run** - Compile and execute, showing final output
- **Clear** - Reset editor and output panels

## Programming Examples

### Example 1: Calculate Sum

```
let a = 15;
let b = 25;
let sum = a + b;
print sum;
```

Output: `40`

### Example 2: Find Maximum

```
let a = 10;
let b = 20;
if a > b {
    print a;
} else {
    print b;
}
```

Output: `20`

### Example 3: Factorial

```
let n = 5;
let fact = 1;
for i = 1 to n {
    fact = fact * i;
}
print fact;
```

Output: `120`

### Example 4: Sum of Range

```
let sum = 0;
for i = 1 to 100 {
    sum = sum + i;
}
print sum;
```

Output: `5050`

### Example 5: Multiplication Table

```
let n = 5;
for i = 1 to 10 {
    let result = n * i;
    print result;
}
```

Output: `5 10 15 20 25 30 35 40 45 50`

### Example 6: Conditional Sum

```
let sum = 0;
for i = 1 to 10 {
    if i > 5 {
        sum = sum + i;
    }
}
print sum;
```

Output: `40` (6+7+8+9+10)

## Optimization Examples

The compiler automatically performs optimizations:

### Constant Folding

**Input**:
```
let x = 2 + 3;
print x;
```

**Optimization**: `2 + 3` is evaluated at compile time to `5`

### Constant Propagation

**Input**:
```
let x = 5;
let y = x + 10;
print y;
```

**Optimization**: Since `x` is constant, `x + 10` becomes `15`

## Common Errors

### Undefined Variable

```
let x = 5;
print y;  // Error: Undefined variable 'y'
```

### Missing Semicolon

```
let x = 5  // Error: Expected ';' after variable declaration
print x;
```

### Missing Braces

```
let x = 10;
if x > 5
    print x;  // Error: Expected '{' after if condition
}
```

### Type Mismatch

This compiler only supports integers. All values are treated as integers.

## Tips and Best Practices

1. **Always initialize variables**: Every variable must be declared with `let` and given a value

2. **Use descriptive names**: Variable names can be any combination of letters, numbers, and underscores

3. **Indent your code**: While not required, proper indentation makes code more readable

4. **Test incrementally**: Start with simple programs and gradually add complexity

5. **Check all compilation stages**: If something doesn't work, check each stage to see where the issue occurs

6. **Use parentheses for clarity**: Even when not required, parentheses make expressions clearer

## Advanced Topics

### Understanding Bytecode

When you compile a program, you can view the generated bytecode. For example:

**Source**:
```
let x = 5;
print x;
```

**Bytecode**:
```
0: PUSH 5      # Push constant 5
1: STORE 0     # Store to variable 0 (x)
2: LOAD 0      # Load variable 0
3: PRINT       # Print value
4: HALT        # Stop execution
```

### Execution Flow

The Virtual Machine executes instructions sequentially, using a stack for intermediate values:

1. PUSH operations add values to the stack
2. Operations (ADD, SUB, etc.) pop operands, compute, and push result
3. STORE pops a value and stores it to a variable
4. LOAD pushes a variable's value onto the stack

### Performance Considerations

- The compiler performs single-pass optimization
- For loops are implemented with conditional jumps
- All arithmetic is integer-based
- No recursion or function calls are supported

## Troubleshooting

**Server won't start**:
- Check if port 8080 is already in use
- Make sure the `web/` directory exists with all files
- Try running with administrator privileges

**Compilation fails**:
- Check for syntax errors in your code
- Ensure all variables are declared before use
- Verify that braces and semicolons are balanced

**Wrong output**:
- Review the bytecode to understand what's being generated
- Check the optimization report to see if constant folding affected your code
- Verify your logic with simpler test cases

## Getting Help

1. Check the example programs for reference
2. Review the ARCHITECTURE.md for technical details
3. Examine the generated bytecode to understand execution
4. Start with simple programs and add complexity gradually
