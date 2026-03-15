#ifndef COMPILATION_ENGINE_H
#define COMPILATION_ENGINE_H
#include "JackTokenizer.h"
#include <string>
#include <fstream>
using namespace std;

// Compilation engine class: Gets the input from JackTokenizer and emits its ouput to an output file. 
class CompilationEngine {
	private: 
		ofstream out;
		Tokenizer tknz;
		int indentLevel;   // For indentation
    		void printIndent(); // Helper for indentation
	public:
		// Constructor: Create a new compilation engine and call compileClass
		CompilationEngine(const string& input, const string& ouputFile);

		// compileClass: Compiles a complete class
		void compileClass();

		// compileClassvarDec: Compiles a static variable declaration or a field declaration
		void compileClassVarDec();

		// compileSubroutine: Compile a complete method, function or constructor
		void compileSubroutine();

		// compileParameterList: Compile a possibly empty parameter list
		void compileParameterList();

		// compileSubroutineBody: Compile a subroutine's body
		void compileSubroutineBody();

		// compileVarDec: Compile a var declaration
		void compileVarDec();

		// compileStatements: Compile a sequece of statements
		void compileStatements();

		// compileLet: Compile the let statement
		void compileLet();

		// compileIf: Compile an if statement, adn possibly with an else
		void compileIf();

		// compileWhile: Compile a while statement
		void compileWhile();

		// compileDo: Compile a do statement
		void compileDo();

		// compileReturn: Compile a return statement
		void compileReturn();

		// compileExpression: Compiles and expression
		void compileExpression();

		// compileTerm: Compiles a term
		// If current token is identifier the routine must resolve it
		// A single lookahead token suffices to distinguish the cases
		void compileTerm();

		// compileExpressionList: Compiles a (possible empty) list of expressions, returns the number of expressions in the list
		int compileExpressionList();

};
#endif // COMPILATION_ENGINE_H
