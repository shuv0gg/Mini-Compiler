CXX = g++
CXXFLAGS = -std=c++17 -Wall -I.

# Windows-specific settings
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lws2_32
    RM = del /Q
    MKDIR = mkdir
    TARGET = compiler.exe
else
    LDFLAGS = 
    RM = rm -f
    MKDIR = mkdir -p
    TARGET = compiler
endif

SOURCES = main.cpp \
          Compiler.cpp \
          lexer/Lexer.cpp \
          ast/AST.cpp \
          parser/Parser.cpp \
          semantic/SemanticAnalyzer.cpp \
          optimizer/Optimizer.cpp \
          codegen/Bytecode.cpp \
          codegen/CodeGenerator.cpp \
          vm/VirtualMachine.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(TARGET) 2>nul
	$(RM) *.o 2>nul
	$(RM) lexer\*.o 2>nul
	$(RM) ast\*.o 2>nul
	$(RM) parser\*.o 2>nul
	$(RM) semantic\*.o 2>nul
	$(RM) optimizer\*.o 2>nul
	$(RM) codegen\*.o 2>nul
	$(RM) vm\*.o 2>nul
else
	$(RM) $(TARGET) $(OBJECTS)
endif

run: $(TARGET)
	./$(TARGET) --server

.PHONY: all clean run
