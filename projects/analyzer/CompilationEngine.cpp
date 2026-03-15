#include "CompilationEngine.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Add indent to each compile block
void CompilationEngine::printIndent() {
    for (int i = 0; i < indentLevel; ++i) {
        out << "\t";
    }
}

// Constructor: Create a new compilation engine and call compileClass
CompilationEngine::CompilationEngine(const string& inputFile, const string& outputFile): tknz(inputFile), indentLevel(0){
	out.open(outputFile);
	compileClass();
}

// compileClass: Compiles a complete class
void CompilationEngine::compileClass(){
	// Structure of class: 'class' className '{' classVarDec* subRoutineDec* '}'
	out << "<class>\n";
	indentLevel++;
	tknz.advance();

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	printIndent();
	out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	tknz.advance();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	while (tknz.keyWord() == "static" or tknz.keyWord() == "field"){
		compileClassVarDec();
	}

	while (tknz.keyWord() == "constructor" or tknz.keyWord() == "method" or tknz.keyWord() == "function"){
		compileSubroutine();
	}

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";

	out << "</class>\n";
}

// compileClassvarDec: Compiles a static variable declaration or a field declaration
void CompilationEngine::compileClassVarDec(){
	// Structure of class var dec: ('static' | 'field') type varName (',' varName)* ';'
	
	printIndent();
	out << "<classVarDec>\n";
	indentLevel++;

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	if (tknz.keyWord() == ""){
		printIndent();
		out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	}
	else {
		printIndent();
		out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	}
	tknz.advance();

	printIndent();
	out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	tknz.advance();

	if (tknz.symbol() == ','){
		while(true){
			printIndent();
			out << "<symbol> " << tknz.symbol() << " </symbol>\n";
			tknz.advance();

			printIndent();
			out << "<identifier> " << tknz.identifier() << " </identifier>\n";
			tknz.advance();

			if (tknz.symbol() == ';'){
				printIndent();
				out << "<symbol> " << tknz.symbol() << " </symbol>\n";
				tknz.advance();
				break;
			}
		}
	}
	else {
		printIndent();	
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();
	}

	indentLevel--;
	printIndent();
	out << "</classVarDec>\n";
}

// compileSubroutine: Compile a complete method, function or constructor
void CompilationEngine::compileSubroutine(){
	// Structure of Subroutine: ('constructor' | 'function' | 'method') ('void' | type)
	//			     subroutineName '(' parameterList ')' subroutineBody
	printIndent();
	out << "<subroutineDec>\n";
	indentLevel++;

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	if (tknz.keyWord() == ""){
		printIndent();
		out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	}
	else {
		printIndent();
		out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	}
	tknz.advance();

	printIndent();
	out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	tknz.advance();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	compileParameterList();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	compileSubroutineBody();

	indentLevel--;
	printIndent();
	out<< "</subroutineDec>\n";


}

// compileParameterList: Compile a possibly empty parameter list
void CompilationEngine::compileParameterList(){
	// Structure of parameter list: ((type varName) (',' type varName)*)?
	printIndent();
	out << "<parameterList>\n";
	indentLevel++;

	if (tknz.symbol() != ')'){
		while(true){
			if (tknz.keyWord() != ""){
				printIndent();
				out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
				tknz.advance();
			}
			else {
				printIndent();
				out << "<identifier> " << tknz.identifier() << " </identifier>\n";
				tknz.advance();
			}

			printIndent();
			out << "<identifier> " << tknz.identifier() << " </identifier>\n";
			tknz.advance();
			if (tknz.symbol() != ',') {
				break;
			}
			printIndent();
			out << "<symbol> " << tknz.symbol() << " </symbol>\n";
			tknz.advance();
		}

	}

	indentLevel--;
	printIndent();
	out << "</parameterList>\n";
}

// compileSubroutineBody: Compile a subroutine's body
void CompilationEngine::compileSubroutineBody(){
	// Structure of subroutine body: '{' varDec* statements '}'
	printIndent();
	out << "<subroutineBody>\n";
	indentLevel++;

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	while (tknz.keyWord() != "do" and tknz.keyWord() != "let" and tknz.keyWord() != "while"
		and tknz.keyWord() != "if" and tknz.keyWord() != "return" ){
		compileVarDec();
	}
	while (tknz.keyWord() == "do" or tknz.keyWord() == "let" or tknz.keyWord() == "while"
		or tknz.keyWord() == "if" or tknz.keyWord() == "return"){
		compileStatements();
	}
	
	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	indentLevel--;
	printIndent();
	out <<"</subroutineBody>\n";
}

// compileVarDec: Compile a var declaration
void CompilationEngine::compileVarDec(){
	// Structure of var dec: 'var' type varName (',' varName)*';'
	printIndent();
	out << "<varDec>\n";
	indentLevel++;


	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	if (tknz.keyWord() == "") {	
		printIndent();
		out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	}
	else {
		printIndent();
		out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	}
	tknz.advance();

	printIndent();
	out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	tknz.advance();

	if (tknz.symbol() == ','){
		while(true){
			printIndent();
			out << "<symbol> " << tknz.symbol() << " </symbol>\n";
			tknz.advance();

			printIndent();
			out << "<identifier> " << tknz.identifier() << " </identifier>\n";
			tknz.advance();

			if (tknz.symbol() == ';'){
				printIndent();
				out << "<symbol> " << tknz.symbol() << " </symbol>\n";
				tknz.advance();
				break;
			}
		}
	}
	else {
		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();
	}

	indentLevel--;
	printIndent();
	out <<"</varDec>\n";
}

// compileStatements: Compile a sequece of statements
void CompilationEngine::compileStatements(){
	// Sturcture of statements: statement*
	printIndent();
	out << "<statements>\n";
	indentLevel++;

	while (tknz.keyWord() == "do" or tknz.keyWord() == "let" or tknz.keyWord() == "while"
		or tknz.keyWord() == "if" or tknz.keyWord() == "return"){
		if (tknz.keyWord() == "do"){
			compileDo();
		}
		if (tknz.keyWord() == "let"){
			compileLet();
		}
		if (tknz.keyWord() == "while"){
			compileWhile();
		}
		if (tknz.keyWord() == "if"){
			compileIf();
		}
		if (tknz.keyWord() == "return"){
			compileReturn();
		}
	}

	indentLevel--;
	printIndent();
	out << "</statements>\n";

}

// compileLet: Compile the let statement
void CompilationEngine::compileLet(){
	// Structure of let statement: 'let' varName ('[' expression ']')? '=' expression';'
	printIndent();
	out << "<letStatement>\n";
	indentLevel++;

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	printIndent();
	out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	tknz.advance();

	if (tknz.symbol() == '['){
		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();

		compileExpression();

		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();
	}
	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	compileExpression();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	indentLevel--;
	printIndent();
	out << "</letStatement>\n";

}

// compileIf: Compile an if statement, adn possibly with an else
void CompilationEngine::compileIf(){
	//Structure of if statement: 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
	printIndent();
	out << "<ifStatement>\n";
	indentLevel++;

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	compileExpression();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	compileStatements();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	if (tknz.keyWord() == "else"){
		printIndent();
		out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
		tknz.advance();

		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();

		compileStatements();

		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();
	}

	indentLevel--;
	printIndent();
	out <<"</ifStatement>\n";
}

// compileWhile: Compile a while statement
void CompilationEngine::compileWhile(){
	// Structure of while statement: 'while' '(' expression ')' '{' statements '}'
	printIndent();
	out << "<whileStatement>\n";
	indentLevel++;

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	compileExpression();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	compileStatements();

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	indentLevel--;
	printIndent();
	out << "</whileStatement>\n";

}

// compileDo: Compile a do statement
void CompilationEngine::compileDo(){
	// Structure of do statement: 'do' subroutineCall';'
	// Structure of subroutineCall: subroutineName '(' expressionList ')' | (className| varName)'.'subroutineName'('expressionList')'
	printIndent();
	out << "<doStatement>\n";
	indentLevel++;

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	printIndent();	
	out << "<identifier> " << tknz.identifier() << " </identifier>\n";
	tknz.advance();


	if (tknz.symbol() == '(') {
		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();

		compileExpressionList();

		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();
	}
	else if (tknz.symbol() == '.'){
		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();

		printIndent();
		out << "<identifier> " << tknz.identifier() << " </identifier>\n";
		tknz.advance();

		printIndent();	
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();

		compileExpressionList();

		printIndent();
		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
		tknz.advance();
	}

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	indentLevel--;
	printIndent();
	out << "</doStatement>\n";
}



// compileReturn: Compile a return statement
void CompilationEngine::compileReturn(){
	// Structure of return statement: 'return' expression? ';'
	printIndent();
	out << "<returnStatement>\n";
	indentLevel++;

	printIndent();
	out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
	tknz.advance();

	if (tknz.symbol() != ';'){
		compileExpression();
	}

	printIndent();
	out << "<symbol> " << tknz.symbol() << " </symbol>\n";
	tknz.advance();

	indentLevel--;
	printIndent();
	out << "</returnStatement>\n";

}

// compileExpression: Compiles and expression
void CompilationEngine::compileExpression(){
	// Structure of expression: term (op term)*
	printIndent();
	out << "<expression>\n";
	indentLevel++;

	compileTerm();

	if (tknz.symbol() == '+' or tknz.symbol() == '-' or tknz.symbol() == '*'  or tknz.symbol() == '/'
		or tknz.symbol() == '|' or tknz.symbol() == '=') {
    		printIndent();
    		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
    		tknz.advance();
    		compileTerm();
	}
	else if (tknz.symbol() == '<'){	
    		printIndent();
    		out << "<symbol> &lt; </symbol>\n";
    		tknz.advance();
    		compileTerm();

	}

	else if (tknz.symbol() == '>'){
    		printIndent();
    		out << "<symbol> &gt; </symbol>\n";
    		tknz.advance();
    		compileTerm();
	}

	else if (tknz.symbol() == '&'){
    		printIndent();
    		out << "<symbol> &amp; </symbol>\n";
    		tknz.advance();
    		compileTerm();
	}


	indentLevel--;
	printIndent();
	out << "</expression>\n";
}


// compileTerm: Compiles a term
// If current token is identifier the routine must resolve it
// A single lookahead token suffices to distinguish the cases
void CompilationEngine::compileTerm(){
	// Structure of a term: integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
	printIndent();
	out << "<term>\n";
	indentLevel++;

	if (tknz.tokenType() == "INT_CONST" ){
		printIndent();
		out << "<integerConstant> " << to_string(tknz.intVal()) << " </integerConstant>\n";
		tknz.advance();
	}

	else if (tknz.tokenType() == "STRING_CONST"){
		printIndent();
		out << "<stringConstant> " << tknz.stringVal() << " </stringConstant>\n";
		tknz.advance();
	}

	else if (tknz.tokenType() == "KEYWORD" ){
		printIndent();
		out << "<keyword> " << tknz.keyWord() << " </keyword>\n";
		tknz.advance();
	}
	else if (tknz.tokenType() == "IDENTIFIER") {
    		printIndent();
    		out << "<identifier> " << tknz.identifier() << " </identifier>\n";
    		tknz.advance();

    		if (tknz.symbol() == '[') {
        		printIndent();
        		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
        		tknz.advance();

        		compileExpression();

        		printIndent();
        		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
        		tknz.advance();
    		}
    		else if (tknz.symbol() == '(') {
        		printIndent();
        		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
        		tknz.advance();

        		compileExpressionList();

       		 	printIndent();
        		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
        		tknz.advance();
    		}
    		else if (tknz.symbol() == '.') {
        		printIndent();
        		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
        		tknz.advance();

        		printIndent();
        		out << "<identifier> " << tknz.identifier() << " </identifier>\n";
        		tknz.advance();

        		printIndent();
        		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
        		tknz.advance();

        		compileExpressionList();

        		printIndent();
        		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
        		tknz.advance();
    		}
	}

	else if (tknz.tokenType() == "SYMBOL"){
		if (tknz.symbol() == '('){
			printIndent();
			out << "<symbol> " << tknz.symbol() << " </symbol>\n";
			tknz.advance();

			compileExpression();

			printIndent();
			out << "<symbol> " << tknz.symbol() << " </symbol>\n";
			tknz.advance();
		}
		else if (tknz.symbol() == '~' or tknz.symbol() == '-') {
			printIndent();
			out << "<symbol> " << tknz.symbol() << " </symbol>\n";
			tknz.advance();
			compileTerm();
			
		}	

	}

	indentLevel--;
	printIndent();
	out << "</term>\n";

}

// compileExpressionList: Compiles a (possible empty) list of expressions, returns the number of expressions in the list
int CompilationEngine::compileExpressionList(){
	// Structure of expression list: (expression (',' expression)*)?
	int count = 0;
	// Check if the list is not empty (next token is not ')')
	if (tknz.symbol() != ')') {
		printIndent();
		out << "<expressionList>\n";
		indentLevel++;

        	compileExpression();
        	count++;
        	while (tknz.symbol() == ',') {
			printIndent();
            		out << "<symbol> " << tknz.symbol() << " </symbol>\n";
            		tknz.advance();
            		compileExpression();
            		count++;
        	}

		indentLevel--;
		printIndent();
    		out << "</expressionList>\n";
	}
    	return count;
} 
