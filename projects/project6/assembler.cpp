#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <algorithm>
using namespace std;

// Computation command maps
const unordered_map<string, string> destDict = {
    {"", "000"},
    {"M", "001"},
    {"D", "010"},
    {"MD", "011"},
    {"A", "100"},
    {"AM", "101"},
    {"AD", "110"},
    {"AMD", "111"}
};

const unordered_map<string, string> compDict = {
    {"0", "0101010"},
    {"1", "0111111"},
    {"-1", "0111010"},
    {"D", "0001100"},
    {"A", "0110000"},
    {"M", "1110000"},
    {"!D", "0001101"},
    {"!A", "0110001"},
    {"!M", "1110001"},
    {"-D", "0001111"},
    {"-A", "0110011"},
    {"-M", "1110011"},
    {"D+1", "0011111"},
    {"A+1", "0110111"},
    {"M+1", "1110111"},
    {"D-1", "0001110"},
    {"A-1", "0110010"},
    {"M-1", "1110010"},
    {"D+A", "0000010"},
    {"D+M", "1000010"},
    {"D-A", "0010011"},
    {"D-M", "1010011"},
    {"A-D", "0000111"},
    {"M-D", "1000111"},
    {"D&A", "0000000"},
    {"D&M", "1000000"},
    {"D|A", "0010101"},
    {"D|M", "1010101"}
};

const unordered_map<string, string> jumpDict = {
    {"", "000"},   // No jump
    {"JGT", "001"},
    {"JEQ", "010"},
    {"JGE", "011"},
    {"JLT", "100"},
    {"JNE", "101"},
    {"JLE", "110"},
    {"JMP", "111"}
};


// Parser
class Parser {
	private:
		ifstream inputFile;
		string currentCommand;
	public:
		// Constructor
		Parser(const string& filename) {
			inputFile.open(filename);
			if (!inputFile.is_open()) {
				cerr << "Error: File could not be opened\n";
			}
		}

		// Does the file has more commands? (T/F)
		bool hasMoreCommands() {
			return !inputFile.eof();
		}

		// Helper function to trim whitespace from both ends
		string trim(const string& s) {
    			size_t start = s.find_first_not_of(" \t\r\n");
    			size_t end = s.find_last_not_of(" \t\r\n");
    			if (start == string::npos) return "";
    			return s.substr(start, end - start + 1);
		}

		// Advance to the next command
		void advance() {
    			while (getline(inputFile, currentCommand)) {
        			// Remove inline comments
        			size_t comment_pos = currentCommand.find("//");
        			if (comment_pos != string::npos) {
            				currentCommand = currentCommand.substr(0, comment_pos);
        			}
        			// Trim whitespace
        			currentCommand = trim(currentCommand);
        			// Skip if line is empty after trimming
        			if (currentCommand.empty()) continue;
        			// Valid cleaned line found
        			break;
    			}
		}

		// What is the type of command in this line?
		string commandType(){
			if (currentCommand[0] == '@'){
				return "A_command";
			}
			if (currentCommand[0] == '('){
				return "L_command";
			}
			else return "C_command";

		}

		// Get xxx from @xxx or (xxx)
		string symbol(){
			if (commandType() == "A_command"){
				return currentCommand.substr(1);
			}
			else if (commandType() == "L_command"){
				return currentCommand.substr(1, currentCommand.length()-2);
			}
			return "";
		}	

		// Get the dest string from C command
		string dest() {
			size_t eq_pos = currentCommand.find('=');
			if (currentCommand.find('=') != string::npos){
				return currentCommand.substr(0, eq_pos); 
			}
			return ""; // no dest
		}

		// Get the comp string from C command
		string comp(){
			size_t eq_pos = currentCommand.find('=');
			size_t sc_pos = currentCommand.find(';');
			if (sc_pos != string::npos && eq_pos != string::npos){
				return currentCommand.substr(eq_pos + 1, sc_pos - (eq_pos + 1)); 
			}
			else if (eq_pos != string::npos){
				return currentCommand.substr(eq_pos + 1);
			}
			else if (sc_pos != string::npos){
				return currentCommand.substr(0, sc_pos);
			}
			return currentCommand;
		}

		// Get the jump string from C command
		string jump() {
			size_t sc_pos = currentCommand.find(';');
			if (sc_pos != string::npos){
				return currentCommand.substr(sc_pos+1);
			}
			else return "";

		}
};



// Code
class Code {
	public:
		// Turn dest string to binary
		string dest(const string& dest){
			auto it = destDict.find(dest);
			return it->second;
		}

		// Turn comp string to binary
		string comp(const string& comp){
			auto it = compDict.find(comp);
			return it->second;
		}

		// Turn jump string to binary
		string jump(const string& jump){
			auto it = jumpDict.find(jump);
			return it->second;
		}
};


// SymbolTable
class SymbolTable {
	// Can't modify the basic table outside of this class
	private:
		unordered_map<string, int> table;
	public:
		// Constructor
		SymbolTable(){
			table["SP"] = 0;
			table["LCL"] = 1;
			table["ARG"] = 2;
			table["THIS"] = 3;
			table["THAT"] = 4;
			for (int i = 0; i <= 15; ++i) {
				table["R" + to_string(i)] = i;
			}
			table["SCREEN"] = 16384;
			table["KBD"] = 24576;

		}

		// Add entry to the symbol table
		void addEntry(const string& symbol, int address) {
			table[symbol] = address;
		}

		// Does the symbol table contain this symbol? (T/F)
		bool contains(const string& symbol) {
			return (table.count(symbol) > 0);
		}

		// Get the address associated to that symbol
		int GetAddress(const string& symbol) {
			return table[symbol];
		}

};


// Main
int main(int argc, char* argv[]) {
	// Initialize the symbol table
	SymbolTable symbolTable;

	// Begin parser
	Parser parser1(argv[1]);

	// Count lines with commands (non comment or label)
	int line_counter = 0;
	// 1st pass, fill symbol table with labels
	while (parser1.hasMoreCommands()) {
        	parser1.advance();  // Move to the next command
        	if (parser1.commandType() == "L_command") {
            		string label = parser1.symbol();
            		if (!symbolTable.contains(label)) {
                		symbolTable.addEntry(label, line_counter);
            		}
        	}
       		line_counter++;
    	}

	// Restart parser
	Parser parser2(argv[1]);
	// Open output file for writing
	string inputFileName = argv[1];
	size_t pos = inputFileName.find(".asm");
	string out = inputFileName.replace(pos, 4, ".hack"); // ".asm" has 4 characters
	ofstream outputFile(out);  // Use the new file name with .hack

	int variable_count = 16;
	Code code;
	// 2nd pass, add variables to symbol table and translate to hack file
	while (parser2.hasMoreCommands()) {
		parser2.advance();  // Move to the next command
		string cmd_type = parser2.commandType();
		string cmd;
		if (cmd_type == "A_command"){
			string sym = parser2.symbol();
			if (all_of(sym.begin(), sym.end(), ::isdigit)) {
    				cmd = "0" + bitset<15>(stoi(sym)).to_string();
			}
			else if (symbolTable.contains(sym)){
				int addr = symbolTable.GetAddress(sym);
				cmd = "0" + bitset<15>(addr).to_string();
			}
			else {
				symbolTable.addEntry(sym,variable_count);
				variable_count++;
				int addr = symbolTable.GetAddress(sym);
				cmd = "0" + bitset<15>(addr).to_string();
			}
		}
		if (cmd_type == "C_command"){
			string d = parser2.dest();
			string c = parser2.comp();
			string j = parser2.jump();
			cmd = "111";
			cmd += code.comp(c);
			cmd += code.dest(d);
			cmd += code.jump(j);
		}
		if (cmd_type == "L_command"){
			continue;
		}
		outputFile << cmd << endl;
        }
	// Close hack file
	outputFile.close();

}
