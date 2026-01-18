// Tab switching
document.querySelectorAll('.tab-btn').forEach(btn => {
    btn.addEventListener('click', () => {
        const tabId = btn.dataset.tab;
        
        // Remove active class from all tabs and panels
        document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
        document.querySelectorAll('.tab-panel').forEach(p => p.classList.remove('active'));
        
        // Add active class to clicked tab and corresponding panel
        btn.classList.add('active');
        document.getElementById(tabId).classList.add('active');
    });
});

// Example selector
document.getElementById('examples').addEventListener('change', async (e) => {
    const exampleFile = e.target.value;
    if (!exampleFile) return;
    
    try {
        const response = await fetch(`/examples/${exampleFile}`);
        if (response.ok) {
            const code = await response.text();
            document.getElementById('sourceCode').value = code;
        } else {
            alert('Failed to load example');
        }
    } catch (error) {
        console.error('Error loading example:', error);
        alert('Error loading example');
    }
});

// Compile button
document.getElementById('compileBtn').addEventListener('click', async () => {
    const sourceCode = document.getElementById('sourceCode').value;
    
    if (!sourceCode.trim()) {
        alert('Please enter some code first!');
        return;
    }
    
    try {
        const response = await fetch('/compile', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'source=' + encodeURIComponent(sourceCode)
        });
        
        const responseText = await response.text();
        const result = JSON.parse(responseText);
        
        // Display tokens (result.tokens is already an array from JSON.parse)
        if (result.tokens) {
            const tokens = Array.isArray(result.tokens) ? result.tokens : JSON.parse(result.tokens);
            let tokensHtml = '';
            tokens.forEach((token, idx) => {
                tokensHtml += `Token ${idx + 1}: ${token.type.padEnd(15)} | "${token.lexeme}"${token.lexeme ? ' '.repeat(Math.max(1, 10 - token.lexeme.length)) : '          '} | Line ${token.line}, Col ${token.column}\n`;
            });
            document.getElementById('tokensOutput').textContent = tokensHtml;
        }
        
        // Display AST (result.ast is already an object from JSON.parse)
        if (result.ast) {
            const ast = typeof result.ast === 'object' ? result.ast : JSON.parse(result.ast);
            document.getElementById('astOutput').textContent = JSON.stringify(ast, null, 2);
        }
        
        // Display semantic analysis
        if (result.semantic) {
            document.getElementById('semanticOutput').textContent = result.semantic;
            if (!result.success && result.semantic.includes('error')) {
                document.getElementById('semanticOutput').classList.add('error');
            } else {
                document.getElementById('semanticOutput').classList.remove('error');
            }
        }
        
        // Display optimization
        if (result.optimization) {
            document.getElementById('optimizerOutput').textContent = result.optimization;
        }
        
        // Display bytecode (result.bytecode is already an array from JSON.parse)
        if (result.bytecode) {
            const bytecode = Array.isArray(result.bytecode) ? result.bytecode : JSON.parse(result.bytecode);
            let bytecodeText = '';
            bytecode.forEach(instr => {
                bytecodeText += `${String(instr.address).padStart(4, '0')}: ${instr.opcode.padEnd(15)}`;
                if (instr.operand !== undefined) {
                    bytecodeText += ` ${instr.operand}`;
                }
                bytecodeText += '\n';
            });
            document.getElementById('bytecodeOutput').textContent = bytecodeText;
        }
        
        // Display output (if available from compilation)
        if (result.output) {
            document.getElementById('executionOutput').textContent = result.output || 'No output';
        }
        
        // Show error if compilation failed
        if (!result.success) {
            alert('Compilation failed:\n' + result.error);
        }
        
    } catch (error) {
        console.error('Error:', error);
        alert('Failed to compile. Make sure the server is running.');
    }
});

// Run button
document.getElementById('runBtn').addEventListener('click', async () => {
    const sourceCode = document.getElementById('sourceCode').value;
    
    if (!sourceCode.trim()) {
        alert('Please enter some code first!');
        return;
    }
    
    try {
        const response = await fetch('/compile', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'source=' + encodeURIComponent(sourceCode)
        });
        
        const responseText = await response.text();
        const result = JSON.parse(responseText);
        
        // Display all stages (result.tokens, result.ast, result.bytecode are already parsed)
        if (result.tokens) {
            const tokens = Array.isArray(result.tokens) ? result.tokens : JSON.parse(result.tokens);
            let tokensHtml = '';
            tokens.forEach((token, idx) => {
                tokensHtml += `Token ${idx + 1}: ${token.type.padEnd(15)} | "${token.lexeme}"${token.lexeme ? ' '.repeat(Math.max(1, 10 - token.lexeme.length)) : '          '} | Line ${token.line}, Col ${token.column}\n`;
            });
            document.getElementById('tokensOutput').textContent = tokensHtml;
        }
        
        if (result.ast) {
            const ast = typeof result.ast === 'object' ? result.ast : JSON.parse(result.ast);
            document.getElementById('astOutput').textContent = JSON.stringify(ast, null, 2);
        }
        
        if (result.semantic) {
            document.getElementById('semanticOutput').textContent = result.semantic;
        }
        
        if (result.optimization) {
            document.getElementById('optimizerOutput').textContent = result.optimization;
        }
        
        if (result.bytecode) {
            const bytecode = Array.isArray(result.bytecode) ? result.bytecode : JSON.parse(result.bytecode);
            let bytecodeText = '';
            bytecode.forEach(instr => {
                bytecodeText += `${String(instr.address).padStart(4, '0')}: ${instr.opcode.padEnd(15)}`;
                if (instr.operand !== undefined) {
                    bytecodeText += ` ${instr.operand}`;
                }
                bytecodeText += '\n';
            });
            document.getElementById('bytecodeOutput').textContent = bytecodeText;
        }
        
        // Display execution output
        if (result.output !== undefined) {
            document.getElementById('executionOutput').textContent = result.output || '(No output)';
            document.getElementById('executionOutput').classList.add('success');
        }
        
        // Switch to output tab
        document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
        document.querySelectorAll('.tab-panel').forEach(p => p.classList.remove('active'));
        document.querySelector('[data-tab="output"]').classList.add('active');
        document.getElementById('output').classList.add('active');
        
        if (!result.success) {
            alert('Execution failed:\n' + result.error);
        }
        
    } catch (error) {
        console.error('Error:', error);
        alert('Failed to run. Make sure the server is running.');
    }
});

// Clear button
document.getElementById('clearBtn').addEventListener('click', () => {
    document.getElementById('sourceCode').value = '';
    document.getElementById('tokensOutput').textContent = 'Click "Compile" to see tokens...';
    document.getElementById('astOutput').textContent = 'Click "Compile" to see AST...';
    document.getElementById('semanticOutput').textContent = 'Click "Compile" to see semantic analysis...';
    document.getElementById('optimizerOutput').textContent = 'Click "Compile" to see optimizations...';
    document.getElementById('bytecodeOutput').textContent = 'Click "Compile" to see bytecode...';
    document.getElementById('executionOutput').textContent = 'Click "Compile & Run" to see output...';
    document.getElementById('examples').value = '';
});
