#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <bitset>
#include <algorithm>
using namespace std;


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
		bool hasMoreLines() {
        		return  inputFile.peek() != EOF;
    		}

    		// Advance to the next command
    		void advance() {
        		string lineBuffer;
			if (hasMoreLines()){
        			getline(inputFile, currentCommand);
        		}
    		}
                // What is the type of the current command?
                string commandType(){
			// Treat a string as a stream
    			stringstream ss(currentCommand);
    			string firstWord;
    			ss >> firstWord;  // extracts the first token before the first space
                        if (firstWord == "push"){
                                return "C_push";
                        }
                        if (firstWord == "pop"){
                                return "C_pop";
                        }
			if (firstWord == "add"){
				return "C_add";
			}
                        if (firstWord == "sub"){
                                return "C_sub";
                        }
                        if (firstWord == "neg"){
                                return "C_neg";
                        }
                        if (firstWord == "eq"){
                                return "C_eq";
                        }
                        if (firstWord == "gt"){
                                return "C_gt";
                        }
                        if (firstWord == "lt"){
                                return "C_lt";
                        }
                        if (firstWord == "and"){
                                return "C_and";
                        }
                        if (firstWord == "or"){
                                return "C_or";
                        }
                        if (firstWord == "not"){
                                return "C_not";
                        }
			return "Unrecognized";
                }

                // Get segment
                string segment(){
                        if (commandType() == "C_push" or commandType() == "C_pop"){
                                stringstream ss(currentCommand);
				string cmd, seg;
				ss >> cmd >> seg;
				return seg;
                        }
			return "None";
                }

                // Get segment's position
                string dest() {
                        if (commandType() == "C_push" or commandType() == "C_pop"){
                                stringstream ss(currentCommand);
				string cmd, seg, idx;
				ss >> cmd >> seg >> idx;
				return idx;
                        }
			return "None";
                }
};



// Write code
class CodeWriter {
	public:
    	string static_name; 
	ofstream outputFile;
	// Counter is used for having unique loops  in C_eq, C_lt, C_gt
	long long label_counter = 0;
	CodeWriter(string filename){
		size_t pos = filename.find(".vm");
		// Used to push and pop static
	        static_name = filename.substr(0, pos);
		string newFileName = static_name + ".asm";
	        outputFile.open(newFileName);
	}

	void WriteArithmatic(string cmdType){
		string translation;
        	if (cmdType == "C_add"){
        		translation =
			"@SP\n"
			"M=M-1\n"
			"A=M\n"
			"D=M\n"
			"@SP\n"
			"M=M-1\n"
			"@SP\n"
			"A=M\n"
			"M=D+M\n"
			"@SP\n"
			"M=M+1\n";
        	}
        	else if (cmdType == "C_sub"){
			translation = 
                        "@SP\n"
                        "M=M-1\n"
                        "A=M\n"
                        "D=M\n"
                        "@SP\n"
                        "M=M-1\n"
                        "@SP\n"
                        "A=M\n"
                        "M=M-D\n"
                        "@SP\n"
                        "M=M+1\n";
        	}
        	else if (cmdType == "C_neg"){
                        translation =
			"@SP\n"
			"A=M-1\n"
			"M=-M\n";
        	}
        	else if (cmdType == "C_eq" || cmdType == "C_gt" || cmdType == "C_lt"){
        		string jmp_cmd; // JEQ, JGT, JLT
        		if (cmdType == "C_eq") jmp_cmd = "JEQ";
        		else if (cmdType == "C_gt") jmp_cmd = "JGT";
        		else if (cmdType == "C_lt") jmp_cmd = "JLT";

        		translation =
            		"@SP\n"	
            		"M=M-1\n"
            		"A=M\n"
            		"D=M\n"
            		"@SP\n"
            		"M=M-1\n"
            		"A=M\n"
            		"D=M-D\n"
            		"@TRUE_" + to_string(label_counter) + "\n"
            		"D;" + jmp_cmd + "\n"
            		"@SP\n"
            		"A=M\n"
            		"M=0\n"
            		"@END_" + to_string(label_counter) + "\n"
            		"0;JMP\n"
            		"(TRUE_" + to_string(label_counter) + ")\n"
            		"@SP\n"
            		"A=M\n"
            		"M=-1\n"
            		"(END_" + to_string(label_counter) + ")\n"
            		"@SP\n"
            		"M=M+1\n";

        		label_counter++;
    		}
        	else if (cmdType == "C_and"){
        		translation =
                        "@SP\n"
                        "M=M-1\n"
                        "A=M\n"
                        "D=M\n"
                        "@SP\n"
                        "A=M-1\n"
                        "M=D&M\n";
        	}
        	else if (cmdType == "C_or"){
        		translation =
                        "@SP\n"
                        "M=M-1\n"
                        "A=M\n"
                        "D=M\n"
                        "@SP\n"
                        "A=M-1\n"
                        "M=D|M\n";
        	}
        	else if (cmdType == "C_not"){
       			translation =
                                "@SP\n"
                                "A=M-1\n"
                                "M=!M\n";
        	}
		outputFile << translation <<endl;
	}
	void WritePushPop(string cmdType, string segment, string i){
		string translation;
		if (cmdType == "C_push"){
			if (segment == "constant"){
				translation =
				"@" + i + "\n"
				"D=A\n"
				"@SP\n"
				"A=M\n"
				"M=D\n"
				"@SP\n"
				"M=M+1\n";
				
			}
                        else if (segment == "temp"){
                                int result = 5 + stoi(i);
                                translation =
                                "@" + to_string(result) + "\n"
                                "D=M\n"
                                "@SP\n"
                                "A=M\n"
                                "M=D\n"
                                "@SP\n"
                                "M=M+1\n";
			}
			else if (segment == "pointer") {
				string base;
				if (i == "0") base = "R3";
				else if (i == "1") base = "R4";
            			translation =
                		"@" + base + "\n"
                		"D=M\n"
                		"@SP\n"
                		"A=M\n"
                		"M=D\n"
                		"@SP\n"
                		"M=M+1\n";
        		}
			else if (segment == "static") {
            			translation =
                		"@" + static_name + "." + i + "\n"
                		"D=M\n"
                		"@SP\n"
                		"A=M\n"
                		"M=D\n"
                		"@SP\n"
                		"M=M+1\n";
        		}
			else {
            			string base_register;
            			if (segment == "local") base_register = "LCL";
            			else if (segment == "argument") base_register = "ARG";
            			else if (segment == "this") base_register = "THIS";
            			else if (segment == "that") base_register = "THAT";

            			translation =
				"@" + base_register + "\n"
				"D=M\n"
                		"@" + i + "\n"
				"A=D+A\n"
                		"D=M\n"
                		"@SP\n"
                		"A=M\n"
                		"M=D\n"
                		"@SP\n"
                		"M=M+1\n";
        		}
			outputFile << translation << endl;
		}
		else if (cmdType == "C_pop"){
            		if (segment == "temp"){
                		int target_addr_val = 5 + std::stoi(i);
                		translation =
                    		"@" + to_string(target_addr_val) + "\n"
                    		"D=A\n"
                    		"@R13\n"
                    		"M=D\n"
                    		"@SP\n"
                    		"M=M-1\n"
                    		"A=M\n"
                    		"D=M\n"
                    		"@R13\n"
                    		"A=M\n"
                    		"M=D\n";
            		}
            		else if (segment == "pointer") {
                		string base_reg;
				if (i == "0") base_reg = "R3";
				else if ( i== "1") base_reg = "R4";
                		translation =
                    		"@" + base_reg + "\n"
                    		"D=A\n"
                    		"@R13\n"
                    		"M=D\n"
                    		"@SP\n"
                    		"M=M-1\n"
                    		"A=M\n"
                    		"D=M\n"
                    		"@R13\n"
                    		"A=M\n"
                    		"M=D\n";
            		}
            		else if (segment == "static"){
                		string static_var_label = static_name + "." + i;
                		translation =
                    		"@" + static_var_label + "\n"
                    		"D=A\n"
                    		"@R13\n"
                    		"M=D\n"
                    		"@SP\n"
                    		"M=M-1\n"
                    		"A=M\n"
                    		"D=M\n"
                    		"@R13\n"
                    		"A=M\n"
                    		"M=D\n";
            		}
            		else {
                		string base_register;
                		if (segment == "local") base_register = "LCL";
                		else if (segment == "argument") base_register = "ARG";
                		else if (segment == "this") base_register = "THIS";
                		else if (segment == "that") base_register = "THAT";

                		translation =
				"@" + base_register + "\n"
				"D=M\n"
                    		"@" + i + "\n"
                    		"A=D+A\n"
                    		"D=A\n"
                    		"@R13\n"
                    		"M=D\n"
                    		"@SP\n"
                    		"M=M-1\n"
                    		"A=M\n"
                    		"D=M\n"
                    		"@R13\n"
                    		"A=M\n"
                   		"M=D\n";
            		}
			outputFile << translation << endl;
        	}
	}
	void close(){
		outputFile.close();
	}

};




// Main
int main(int argc, char* argv[]) {

        // Begin parser
        Parser parser(argv[1]);

	// Begin writer
	CodeWriter codewriter(argv[1]);

        while (parser.hasMoreLines()) {
		parser.advance();
                string cmd_type = parser.commandType();

                if (cmd_type == "C_push" or cmd_type == "C_pop"){
			string segment, dest;
			segment = parser.segment();
			dest = parser.dest();
			codewriter.WritePushPop(cmd_type, segment, dest);
                }
                else {
			codewriter.WriteArithmatic(cmd_type);
                }
        }
        // Close hack file
        codewriter.close();

}
