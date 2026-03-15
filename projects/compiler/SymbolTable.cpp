#include "SymbolTable.h"
#include <string>
#include <iostream>

// constructor /initializer: Creates a new symbol table
SymbolTable::SymbolTable(){
	reset();
}

// reset: Empties the symbol table and resets the four indexes to 0
void SymbolTable::reset(){
	Table.clear();

	staticIdx = 0;
        fieldIdx = 0;
        argIdx = 0;
        varIdx = 0;
}

// define: Adds to the table a new entry
void SymbolTable::define(const string& name, const string& type, const string& kind){
	int index = -1; // an invalid value just for safety
	if (kind == "static"){
		index = staticIdx;
		staticIdx++;
	}
	else if (kind == "field"){
		index = fieldIdx;
		fieldIdx++;
	}
	else if (kind == "argument"){
		index = argIdx;
		argIdx++;
	}
	else if (kind == "var"){
		index = varIdx;
		varIdx++;
	}

	else return;

	Table[name] = {type, kind, index};

}

// varCount: Returns the number of variables of a kind in the table
int SymbolTable::varCount(const string& kind){
	if (kind == "static"){
		return staticIdx;
	}
	if (kind == "field"){
		return fieldIdx;
	}
	if (kind == "argument"){
		return argIdx;
	}
	if (kind == "var"){
		return varIdx;
	}
	return 0;
}


// kindOf: Returns the kind of the named identifier or NONE if not found
string SymbolTable::kindOf(const string& name){
	auto it = Table.find(name);

	if (it != Table.end()) {
    		// The name was found! 'it' points to the map entry (key + struct).
    		return it->second.kind;

	}
	else {
    		// name was not found in the symbol table
    		return "NONE";
	}	

}

// typeOf: Returns the type of the named variable
string SymbolTable::typeOf(const string& name){
	auto it = Table.find(name);

	if (it != Table.end()) {
    		// The name was found! 'it' points to the map entry (key + struct).
    		return it->second.type;

	}
	else {
    		// name was not found in the symbol table
    		return "NONE";
	}
}

// indexOf: Returns the index of the named variable
int SymbolTable::indexOf(const string& name){
	auto it = Table.find(name);

	if (it != Table.end()) {
    		// The name was found! 'it' points to the map entry (key + struct).
    		return it->second.index;

	}
	else {
    		// name was not found in the symbol table
    		return -1;
	}
}
