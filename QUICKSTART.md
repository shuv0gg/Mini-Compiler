# Quick Start Guide

Get up and running with the Educational Mini Compiler in 3 simple steps!

## Step 1: Build the Compiler

### Windows
Open Command Prompt or PowerShell and run:
```bash
cd "f:\Compiler Project Folder"
build.bat
```

### Linux/Mac
Open terminal and run:
```bash
cd /path/to/compiler-project
make
```

## Step 2: Start the Web Server

### Windows
```bash
run.bat
```

### Linux/Mac
```bash
./compiler --server
```

You should see:
```
Starting HTTP server on port 8080...
Open http://localhost:8080 in your browser
```

## Step 3: Open Your Browser

Navigate to: **http://localhost:8080**

## Your First Program

Once the web interface loads, try this simple program:

```
let x = 10;
let y = 20;
print x + y;
```

Click **"Compile & Run"** and you'll see the output: `30`

## Explore the Examples

Click the **"Examples"** dropdown and select any of the 13 built-in programs:

- **Arithmetic Operations** - Basic math
- **If-Else Statement** - Conditionals
- **For Loop** - Iteration
- **Factorial** - More complex logic

## View Compilation Stages

After compiling, click through the tabs to see each stage:

1. **Tokens** - How code is broken into pieces
2. **AST** - The program structure tree
3. **Semantic** - Variable checking
4. **Optimizer** - Code improvements
5. **Bytecode** - Low-level instructions
6. **Output** - Final execution results

## Try These Programs

### Program 1: Hello World Style
```
print 42;
```

### Program 2: Variables
```
let name = 100;
let age = 25;
print name + age;
```

### Program 3: If Statement
```
let score = 85;
if score > 60 {
    print 1;
}
```

### Program 4: Loop
```
for i = 1 to 5 {
    print i;
}
```

### Program 5: Factorial
```
let n = 5;
let result = 1;
for i = 1 to n {
    result = result * i;
}
print result;
```

## Common Commands

### Stop the Server
Press `Ctrl+C` in the terminal

### Restart the Server
```bash
run.bat        # Windows
./compiler --server    # Linux/Mac
```

### Test Without Web Interface
```bash
./compiler
```
This runs a built-in test program.

## Troubleshooting

### Port Already in Use
If you see an error about port 8080:
1. Stop any other programs using that port
2. Or modify `main.cpp` to use a different port

### Compilation Errors
If the build fails:
1. Make sure you have g++ installed: `g++ --version`
2. Check that you're in the correct directory
3. Try `make clean` then `make` again

### Web Page Won't Load
1. Check that the server is running
2. Verify you're going to http://localhost:8080
3. Try refreshing the page (Ctrl+F5)

## Next Steps

1. **Explore Examples** - Try all 13 example programs
2. **Write Your Own** - Create custom programs
3. **View Bytecode** - See how your code compiles
4. **Read Documentation**:
   - `README.md` - Full user guide
   - `USAGE.md` - Complete language reference
   - `ARCHITECTURE.md` - Technical details

## Language Cheat Sheet

| Feature | Syntax |
|---------|--------|
| Variable | `let x = 10;` |
| Print | `print x;` |
| Add | `a + b` |
| Subtract | `a - b` |
| Multiply | `a * b` |
| Divide | `a / b` |
| Greater | `a > b` |
| Less | `a < b` |
| Equal | `a == b` |
| If | `if condition { statements }` |
| If-Else | `if cond { } else { }` |
| For Loop | `for i = 1 to 10 { }` |

## Getting Help

- Check example programs for reference
- Read the error messages carefully
- View bytecode to understand execution
- Consult USAGE.md for detailed syntax

## Have Fun!

You're ready to start learning about compilers! Experiment, break things, and learn how compilation works.

---

**Pro Tip**: Start with simple examples and gradually increase complexity. Use the "Compile" button to see each stage before running.
