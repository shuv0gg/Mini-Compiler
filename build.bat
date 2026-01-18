@echo off
echo Building Educational Mini Compiler...
echo.

g++ -std=c++17 -I. main.cpp Compiler.cpp lexer/Lexer.cpp ast/AST.cpp parser/Parser.cpp semantic/SemanticAnalyzer.cpp optimizer/Optimizer.cpp codegen/Bytecode.cpp codegen/CodeGenerator.cpp vm/VirtualMachine.cpp -o compiler.exe -lws2_32

if %errorlevel% == 0 (
    echo.
    echo ✓ Build successful!
    echo.
    echo To run the web server:
    echo   compiler.exe --server
    echo.
    echo Then open http://localhost:8080 in your browser
) else (
    echo.
    echo ✗ Build failed!
)

pause
