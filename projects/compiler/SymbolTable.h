#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
	
#include <string>
#include <fstream>
#include <map>
using namespace std;


// Symbol Table class
// Description

class SymbolTable {
public:

	struct Symbol {
        	std::string type;
        	std::string kind;
        	int index;
    	};

	int staticIdx = 0;
	int fieldIdx = 0;
	int argIdx = 0;
	int varIdx = 0;

	
	// constructor /initializer: Creates a new symbol table
	SymbolTable();

	// reset: Empties the symbol table and resets the four indexes to 0
	void reset();

	// define: Adds to the table a new variable with arguments and assigns it an index
	void define(const string& name, const string& type, const string& kind);

	// varCount: Returns the number of variables of a kind in the table
	int varCount(const string& kind);

	// kindOf: Returns the kind of the named identifier or NONE if not found
	string kindOf(const string& name);

	// typeOf: Returns the type of the named variable
	string typeOf(const string& name);

	// indexOf: Returns the index of the named variable
	int indexOf(const string& name);

private:
	map<string, Symbol> Table;

};

#endif // SYMBOL_TABLE_H
