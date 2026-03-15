// Same api as the syntax analyzer of project 10
#include "CompilationEngine.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Constructor: Create a new compilation engine and call compileClass
CompilationEngine::CompilationEngine(const string& inputFile, const string& outputFile): tknz(inputFile), writer(outputFile), indentLevel(0){
        compileClass();
}

// compileClass: Compiles a complete class
void CompilationEngine::compileClass(){
        // Structure of class: 'class' className '{' classVarDec* subRoutineDec* '}'
	classTable.reset();   // new class scope
        tknz.advance();
	ifCounter = 0;
	whileCounter = 0;
        tknz.advance();

	currentClassName = tknz.identifier();
        tknz.advance();

        tknz.advance();

        while (tknz.keyWord() == "static" or tknz.keyWord() == "field"){
                compileClassVarDec();
        }

        while (tknz.keyWord() == "constructor" or tknz.keyWord() == "method" or tknz.keyWord() == "function"){
                compileSubroutine();
        }

}

// compileClassvarDec: Compiles a static variable declaration or a field declaration
void CompilationEngine::compileClassVarDec(){
        // Structure of class var dec: ('static' | 'field') type varName (',' varName)* ';'
	string name;
	string type;
	string category;
	string className;
	category = tknz.keyWord();
        tknz.advance();

        if (tknz.tokenType() == "KEYWORD"){ // in case it is a user defined type, aka class name
		type = tknz.keyWord();
	}
	else {
		type = tknz.identifier();
        }
        tknz.advance();

	name = tknz.identifier();
	classTable.define(name, type, category);
        tknz.advance();

        if (tknz.symbol() == ','){
                while(true){
                        tknz.advance();
			name = tknz.identifier();
			classTable.define(name, type, category);
                        tknz.advance();

                        if (tknz.symbol() == ';'){
                                tknz.advance();
                                break;
                        }
                }
        }
        else {
                tknz.advance();
        }
}

// compileSubroutine: Compile a complete method, function or constructor
void CompilationEngine::compileSubroutine(){
        // Structure of Subroutine: ('constructor' | 'function' | 'method') ('void' | type)
        //                           subroutineName '(' parameterList ')' subroutineBody
	subroutineTable.reset();
	string name, subroutineType;
	int nArgs;
	
	subroutineType = tknz.keyWord(); 
    	if (subroutineType == "method") {
        	subroutineTable.define("this", currentClassName, "argument");
    	}


	tknz.advance(); // This gets the return type
	tknz.advance(); // This gets the function name
	name = currentClassName + "." + tknz.identifier();
	tknz.advance(); // This gets the '(' symbol
	tknz.advance(); // This gets either a parameter or a ')' symbol

	nArgs = compileParameterList(); // we need to check how many parameters there are and add them to the symbol table
	tknz.advance();
	compileSubroutineBody(name, subroutineType);
}

// compileParameterList: Compile a possibly empty parameter list
int CompilationEngine::compileParameterList(){
        // Structure of parameter list: ((type varName) (',' type varName)*)?
	string category = "argument";
	string name;
	string type;
	int count = 0;

        if (tknz.symbol() != ')'){
                while(true){
			count++;
                        if (tknz.keyWord() != ""){
				type = tknz.keyWord();
                                tknz.advance();
                        }
                        else {
				type = tknz.keyWord();
                                tknz.advance();
                        }

			name = tknz.identifier();
			subroutineTable.define(name, type, category);
                        tknz.advance();
                        if (tknz.symbol() != ',') {
                                break;
                        }
                        tknz.advance();
                }

        }
	return count;
}

// compileSubroutineBody: Compile a subroutine's body
void CompilationEngine::compileSubroutineBody(string name, string subroutineType){
        // Structure of subroutine body: '{' varDec* statements '}'
	int nVars;
        tknz.advance();
        nVars = compileVarDec();
	writer.writeFunction(name, nVars);

	if(subroutineType == "constructor"){
		int nFields = classTable.varCount("field");
		writer.writePush("constant", nFields);
		writer.writeCall("Memory.alloc", 1);
		writer.writePop("pointer", 0);
	}

	else if (subroutineType == "method") {
    		writer.writePush("argument", 0);
    		writer.writePop("pointer", 0);
	}
	
        while (tknz.keyWord() == "do" or tknz.keyWord() == "let" or tknz.keyWord() == "while"
                or tknz.keyWord() == "if" or tknz.keyWord() == "return"){
                compileStatements();
        }

        tknz.advance();

}

// compileVarDec: Compile a var declaration
int CompilationEngine::compileVarDec(){
        // Structure of var dec: 'var' type varName (',' varName)*';'
	string category = "var";
	string type;
	string name;
	int counter = 0;

	while(tknz.keyWord() == "var"){
		counter++;
        	tknz.advance();

        	if (tknz.keyWord() == "") {
			type = tknz.identifier();
        	}
        	else {
			type = tknz.keyWord();
        	}
        	tknz.advance();
		name = tknz.identifier();
		subroutineTable.define(name, type, category);
        	tknz.advance();

        	if (tknz.symbol() == ','){
                	while(true){
				counter++;
                        	tknz.advance();

				name = tknz.identifier();
				subroutineTable.define(name, type, category);
                        	tknz.advance();

                        	if (tknz.symbol() == ';'){
                                	tknz.advance();
                                	break;
                        	}
                	}
        	}
        	else {
                	tknz.advance();
        	}
	}
	return counter;
}

// compileStatements: Compile a sequece of statements
void CompilationEngine::compileStatements(){
        // Sturcture of statements: statement*
        while (tknz.keyWord() == "do" or tknz.keyWord() == "let" or tknz.keyWord() == "while"
                or tknz.keyWord() == "if" or tknz.keyWord() == "return"){
                if (tknz.keyWord() == "do"){
                        compileDo();
                }
                if (tknz.keyWord() == "let"){
                        compileLet();
                }
                if (tknz.keyWord() == "while"){
			whileCounter++;
                        compileWhile(whileCounter);
                }
                if (tknz.keyWord() == "if"){
			ifCounter++;
                        compileIf(ifCounter);
                }
                if (tknz.keyWord() == "return"){
                        compileReturn();
                }
        }
}

// compileLet: Compile the let statement
void CompilationEngine::compileLet(){
        // Structure of let statement: 'let' varName ('[' expression ']')? '=' expression';'
	string kind, segment;
	int index;
        tknz.advance();

	string name = tknz.identifier();
	if (subroutineTable.indexOf(name) != -1){
		kind = subroutineTable.kindOf(name);
		index = subroutineTable.indexOf(name);
	}
	else if (classTable.indexOf(name) != -1) {
		kind = classTable.kindOf(name);
		index = classTable.indexOf(name);
	}
        tknz.advance();

	if (kind == "var"){
		segment = "local";
	}
	if (kind == "argument" or kind == "static"){
		segment = kind;
	}
	if (kind == "field"){
		segment = "this";
	}

        if (tknz.symbol() == '['){
		writer.writePush(segment, index);
                tknz.advance();	
                compileExpression();
		writer.writeArithmetic("+");
                tknz.advance(); // eats the ']'
		tknz.advance(); // eats the '='
		compileExpression();
		writer.writePop("temp", 0);
		writer.writePop("pointer", 1);
		writer.writePush("temp", 0);
		writer.writePop("that", 0);
		tknz.advance();
		return;
        }
        tknz.advance();

        compileExpression();

	writer.writePop(segment, index);
        tknz.advance();
}

// compileIf: Compile an if statement, and possibly with an else
void CompilationEngine::compileIf(int counts){
        //Structure of if statement: 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

        tknz.advance();

        tknz.advance();

        compileExpression();
	writer.writeArithmetic("not");
	string label1 = "I" + to_string(counts);
	writer.writeIf(label1);
        tknz.advance();

        tknz.advance();

        compileStatements();

        tknz.advance();

        if (tknz.keyWord() == "else"){
                tknz.advance();

                tknz.advance();
		string label2 = "E" + to_string(counts);
		writer.writeGoto(label2);
		writer.writeLabel(label1);
                compileStatements();
		writer.writeLabel(label2);
                tknz.advance();
        }

	else {
		writer.writeLabel(label1);
	}
}

// compileWhile: Compile a while statement
void CompilationEngine::compileWhile(int counts){
        // Structure of while statement: 'while' '(' expression ')' '{' statements '}'

        tknz.advance();

        tknz.advance();
	string label1 = "W" + to_string(counts);
	writer.writeLabel(label1);

        compileExpression();

	writer.writeArithmetic("not");
	string label2 = "NW" + to_string(counts);
	writer.writeIf(label2);
        tknz.advance();

        tknz.advance();

        compileStatements();
	writer.writeGoto(label1);
	writer.writeLabel(label2);
        tknz.advance();

}

// compileDo: Compile a do statement
void CompilationEngine::compileDo(){
        // Structure of do statement: 'do' subroutineCall';'
        // Structure of subroutineCall: subroutineName '(' expressionList ')' | (className| varName)'.'subroutineName'('expressionList')'
	// do subroutineName(exp1, exp2, ...) -> VM code generated by compileExpression and then pop temp 0
	string name;
	int nArgs;

        tknz.advance(); // eats the do key word and goes to the name

	name = tknz.identifier();
        tknz.advance();

	// Implicit method call (e.g., draw())
        if (tknz.symbol() == '(') {
		writer.writePush("pointer", 0);
                tknz.advance();
		string fullName = currentClassName + "." + name;
                int nArgs = compileExpressionList();
		writer.writeCall(fullName, nArgs + 1); // nArgs + 1 because we pushed 'this'
                // tknz.advance();
        }
        else if (tknz.symbol() == '.'){
		tknz.advance(); // eats '.'
		string subName = tknz.identifier();
		tknz.advance(); // eats the id now at '('

		// we check to see if it is a variable
		string type = subroutineTable.typeOf(name);
        	if (type == "NONE") { 
            		type = classTable.typeOf(name);
        	}
		if (type != "NONE") { // in this case it is indeed a variable
			// condition ? expression_if_true : expression_if_false;
            		string kind = (subroutineTable.indexOf(name) != -1) ? subroutineTable.kindOf(name) : classTable.kindOf(name);
            		int index = (subroutineTable.indexOf(name) != -1) ? subroutineTable.indexOf(name) : classTable.indexOf(name);
            
            		if (kind == "var") kind = "local";
            		else if (kind == "field") kind = "this";
            
            		writer.writePush(kind, index);

            		tknz.advance(); // Eat '('
            		int nArgs = compileExpressionList();
            		
			writer.writeCall(type + "." + subName, nArgs + 1);
        	}

		else {
			tknz.advance(); // Gets '('
			// tknz.advance(); // starts at the expression
			int nArgs = compileExpressionList();
			writer.writeCall(name + "." + subName, nArgs);
		}
        }

	tknz.advance(); // consumes ')'
	writer.writePop("temp", 0);
        tknz.advance(); // consumes ';'
}



// compileReturn: Compile a return statement
void CompilationEngine::compileReturn(){
        // Structure of return statement: 'return' expression? ';'
        tknz.advance();

        if (tknz.symbol() != ';'){
                compileExpression();
        }
	else {
		writer.writePush("constant", 0);
	}

	writer.writeReturn();
        tknz.advance();
}

// compileExpression: Compiles and expression
void CompilationEngine::compileExpression(){
        // Structure of expression: term (op term)*
	string command;
        compileTerm();

while (tknz.tokenType() == "SYMBOL" && 
           (tknz.symbol() == '+' || tknz.symbol() == '-' || tknz.symbol() == '*'  || 
            tknz.symbol() == '/' || tknz.symbol() == '|' || tknz.symbol() == '=' || 
            tknz.symbol() == '<' || tknz.symbol() == '>' || tknz.symbol() == '&')) {
		command = tknz.symbol();
                tknz.advance();
                compileTerm();
		writer.writeArithmetic(command);
        }
}


// compileTerm: Compiles a term
// If current token is identifier the routine must resolve it
// A single lookahead token suffices to distinguish the cases
void CompilationEngine::compileTerm(){
        // Structure of a term: integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term

        if (tknz.tokenType() == "INT_CONST" ){
		writer.writePush("constant", tknz.intVal());
                tknz.advance();
        }

        else if (tknz.tokenType() == "STRING_CONST"){
		string str = tknz.stringVal();
		int len = str.length();
		writer.writePush("constant", len);
		writer.writeCall("String.new", 1);
		for (char c : str) {
			writer.writePush("constant", int(c));
			writer.writeCall("String.appendChar", 2); // 2 args: 'this' + 'char'
    		}
                tknz.advance();
        }

        else if (tknz.tokenType() == "KEYWORD" ){
		string key = tknz.keyWord();
    		if (key == "true") {
        		writer.writePush("constant", 0);
        		writer.writeArithmetic("not"); // true is -1 (bitwise not 0)
    		}
    		else if (key == "false" || key == "null") {
        		writer.writePush("constant", 0);
    		}
    		else if (key == "this") {
        		writer.writePush("pointer", 0);
    		}
    		tknz.advance();
        }
        else if (tknz.tokenType() == "IDENTIFIER") {
		string name = tknz.identifier();
		string kind, segment;
		int index;
		if (subroutineTable.indexOf(name) != -1){
			kind = subroutineTable.kindOf(name);
			index = subroutineTable.indexOf(name);
		}
		else if (classTable.indexOf(name) != -1){
			kind = classTable.kindOf(name);
			index = classTable.indexOf(name);
		}

		if (kind == "var"){
			segment = "local";
			// writer.writePush(segment, index);
		}
		if (kind == "argument" or kind == "static"){
			segment = kind;
			// writer.writePush(segment, index);
		}
		if (kind == "field"){
			segment = "this";
			// writer.writePush(segment, index);
		}
                tknz.advance();

                if (tknz.symbol() == '[') { // a[i]
                        tknz.advance();
			writer.writePush(segment, index);
                        compileExpression();
			writer.writeArithmetic("+");
			writer.writePop("pointer", 1);
			writer.writePush("that", 0);
                        tknz.advance();
                }
                else if (tknz.symbol() == '(') {
                        tknz.advance();

                        compileExpressionList();

                        tknz.advance();
                }
          	else if (tknz.symbol() == '.') {
    			tknz.advance(); // Eat '.'
    			string subName = tknz.identifier(); // e.g. "dispose" or "new"
    			tknz.advance(); // Eat subName
    
    			tknz.advance(); // Eat '('

    			// check if we have a variable
    			string type = subroutineTable.typeOf(name);
    			if (type == "NONE") {
        			type = classTable.typeOf(name);
    			}

    			int nArgs = compileExpressionList();

    			if (type != "NONE") {
				writer.writePush(segment, index);
        			writer.writeCall(type + "." + subName, nArgs + 1);
    			} 
    			else {
        			writer.writeCall(name + "." + subName, nArgs);
    			}

    			tknz.advance(); // Eat ')'
		}
		else {
			writer.writePush(segment, index);
		}
        }

        else if (tknz.tokenType() == "SYMBOL"){
                if (tknz.symbol() == '('){
                        tknz.advance();
                        compileExpression();
                        tknz.advance();
                }
                else if (tknz.symbol() == '~') {
			string cmd(1, tknz.symbol());
                        tknz.advance();
                        compileTerm();
			writer.writeArithmetic(cmd);

                }
		else if (tknz.symbol() == '-'){
			tknz.advance();
			compileTerm();
			writer.writeArithmetic("neg");
		}

        }


}

// compileExpressionList: Compiles a (possible empty) list of expressions, returns the number of expressions in the list
int CompilationEngine::compileExpressionList(){
        // Structure of expression list: (expression (',' expression)*)?
        int count = 0;
        // Check if the list is not empty (next token is not ')')
        if (tknz.symbol() != ')') {
                compileExpression();
                count++;
                while (tknz.symbol() == ',') {
                        tknz.advance();
                        compileExpression();
                        count++;
                }

        }
        return count;
}
