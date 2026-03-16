#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <bitset>
#include <filesystem>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;

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
				exit(1);
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
		// In v2 we add branching and function type cmds
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
			if (firstWord == "label"){
				return "C_label";
			}
			if (firstWord == "if-goto"){
                                return "C_if";
                        }
                        if (firstWord == "goto"){
                                return "C_goto";
                        }
                        if (firstWord == "function"){
                                return "C_function";
                        }
                        if (firstWord == "call"){
                                return "C_call";
                        }
                        if (firstWord == "return"){
                                return "C_return";
                        }



			return "Unrecognized";
                }
		//arg1: Returns the 1st argument for the current cmd
		// for arithmatic type is the cmd itself
                // Get segment
                string arg1(){
                	stringstream ss(currentCommand);
			string cmd, arg1;
			ss >> cmd >> arg1;
			return arg1;
                }
		//arg2: Returns the 2nd argument of the current cmd
		// Only if cmd is push, pop, function or call type
		// This is only called if it is apporpiate so there is no need to check the cmd type
                // Get segment's position
                string arg2() {
                	stringstream ss(currentCommand);
			string cmd, arg1, arg2;
			ss >> cmd >> arg1 >> arg2;
			return arg2;
                }
};



// Write code
class CodeWriter {
	public:
	// string current_vm_filename; // to keep track for processing folders
    	string static_name; 
	ofstream outputFile;
	// Counter is used for having unique loops  in C_eq, C_lt, C_gt
	long long label_counter = 0;
	long long call_counter = 1;
	string caller = "";
	string newFileName;

	CodeWriter(string path){
		setFileName(path);
	        outputFile.open(newFileName);
    		if (!outputFile.is_open()) {
        		cerr << "Error: could not open file '" << newFileName << "' for writing." << endl;
        		exit(1);
    		}
	}
	void bootstrapInit(){
	        string initCode =
                "@256\n"  // Set SP to 256
                "D=A\n"
                "@SP\n"
                "M=D\n";
                outputFile << initCode << endl;
                WriteCall("Sys.init", "0"); // Call Sys.init
	}

	//setFileName informs that the translation for a new file, has started
	// But I guess we'll just use it for setting the static name
	void setFileName(string path){
		size_t pos = path.find(".vm");
		// static name used to push and pop static segments
                if (pos == string::npos){
			static_name = path;
			newFileName = path + ".asm";
		//	bootstrapInit();
		}
		else {
        
        		// Extract only the filename without directory
        		size_t lastSlash = path.find_last_of("/\\");
        		string filename = (lastSlash == string::npos) ? path : path.substr(lastSlash + 1);

        		// filename = "Class1.vm" → static_name = "Class1"
        		static_name = filename.substr(0, filename.find(".vm"));
			// static_name = path.substr(0, pos);
			newFileName = static_name + ".asm";
		}
		std::cerr << "DEBUG: static_name = '" << static_name << "'" << std::endl;
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

	//writelabel: Takes a string to handle the label command
	void WriteLabel(string labelName){
		string translation =
		"(" + labelName + ")\n";
		outputFile << translation << endl;

	}

	//writeGoto: String as input and writes a goto command
	void WriteGoto(string destination){
		string translation =
		"@" + destination + "\n"
		"0;JMP\n";
		outputFile << translation << endl;
	} 

	//writeIf: Takes a string and translates the if-goto command
	void WriteIf(string destination){
		string translation =
		"@SP\n"
		"M=M-1\n"
		"A=M\n"
		"D=M\n"
		"@" + destination + "\n"
		"D;JNE\n"; // if cond jmp
		outputFile << translation << endl;
	}

	//writeFunction: Takes string and int to write handle functions
	void WriteFunction(string functionName, string nvar){
		caller = functionName;
		WriteLabel(functionName);
		int n = stoi(nvar);
		// Rest of the translation:
		for (int i=0; i<n; i++){
			// WritePushPop("C_push", "constant", 0);
                        string translation =
                        "@0\n"
                        "D=A\n"
                        "@SP\n"
                        "A=M\n"
                        "M=D\n"
                        "@SP\n"
                        "M=M+1\n";
			outputFile << translation << endl;
		}
	}

	//writeCall: Handle call commands with string and int as inputs
	void WriteCall(string calleName, string nargs){
		string retAddrLabel = caller + "$." + to_string(call_counter);
		WritePushPop("C_push", "constant", retAddrLabel); // This work but technically retAddrLabel is not a constant it is a label

		string translation = 
			"@LCL\n" // Push LCL (not RAM[LCL])
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@ARG\n" // Save ARG
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@THIS\n" // Load address of THIS register
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@THAT\n" // Push THAT
			"D=M\n"
			"@SP\n"
			"A=M\n"
			"M=D\n"
			"@SP\n"
			"M=M+1\n"
			"@SP\n"
			"D=M\n"
			"@5\n"
			"D=D-A\n"
			"@" + nargs + "\n"
			"D=D-A\n" // D = SP - 5 - nargs
			"@ARG\n"
			"M=D\n" // ARG = D
			"@SP\n"
			"D=M\n"
			"@LCL\n"
			"M=D\n"; // LCL = SP
		outputFile << translation << endl;
		WriteGoto(calleName);
		WriteLabel(retAddrLabel);
		call_counter++;
	}

	//writeReturn: Effects the return command in assembly
	void WriteReturn(){
		string translation =
		"@LCL\n"
		"D=M\n"
		"@5\n"
		"D=D-A\n" // D = LCL - 5
		"A=D\n"
		"D=M\n" // D = *(LCL - 5)
		"@R15\n"
		"M=D\n"; // retAddr (R15) = D
		outputFile << translation << endl;
		WritePushPop("C_pop", "argument", "0");
		translation =
		"@ARG\n"
		"D=M+1\n"
		"@SP\n"
		"M=D\n" // SP = ARG + 1
		"@1\n"
		"D=A\n"
		"@LCL\n"
		"D=M-D\n"
		"A=D\n"
		"D=M\n"
		"@THAT\n"
		"M=D\n" // THAT = *(LCL - 1)
		"@2\n"
        	"D=A\n"
        	"@LCL\n"
        	"D=M-D\n"
        	"A=D\n"
        	"D=M\n"
        	"@THIS\n"
        	"M=D\n" // THIS = *(LCL - 2)
        	"@3\n"
        	"D=A\n"
        	"@LCL\n"
        	"D=M-D\n"
        	"A=D\n"
        	"D=M\n"
        	"@ARG\n"
        	"M=D\n" // ARG = *(LCL - 3)
        	"@4\n"
        	"D=A\n"
        	"@LCL\n"
        	"D=M-D\n"
        	"A=D\n"
        	"D=M\n"
        	"@LCL\n"
        	"M=D\n" // LCL = *(LCL - 4)
		"@R15\n"
		"A=M\n"
		"0;JMP\n";
		outputFile << translation << endl;
		
	}


	void close(){
		outputFile.close();
	}

};




// Main
int main(int argc, char* argv[]) {
    string inputPath = argv[1];

    if (inputPath.find(".vm") == string::npos) {
        // Directory case
        vector<fs::path> vmFiles;
        for (const auto& entry : fs::directory_iterator(inputPath)) {
            if (entry.path().extension() == ".vm") {
                vmFiles.push_back(entry.path());
            }
        }

        //string dirName = fs::path(inputPath).filename().string();
        //string asmFile = inputPath + "/" + dirName + ".asm";

        CodeWriter codewriter(inputPath);
        // First: Sys.vm
        for (const auto& file : vmFiles) {
            if (file.filename() == "Sys.vm") {
                Parser parser(file.string());
                codewriter.setFileName(file.string());
		codewriter.bootstrapInit();
                while (parser.hasMoreLines()) {
                    parser.advance();
                    string cmd_type = parser.commandType();

                    if (cmd_type == "C_push" || cmd_type == "C_pop") {
                        codewriter.WritePushPop(cmd_type, parser.arg1(), parser.arg2());
                    } else if (cmd_type == "C_label") {
                        codewriter.WriteLabel(parser.arg1());
                    } else if (cmd_type == "C_goto") {
                        codewriter.WriteGoto(parser.arg1());
                    } else if (cmd_type == "C_if") {
                        codewriter.WriteIf(parser.arg1());
                    } else if (cmd_type == "C_call") {
                        codewriter.WriteCall(parser.arg1(), parser.arg2());
                    } else if (cmd_type == "C_function") {
                        codewriter.WriteFunction(parser.arg1(), parser.arg2());
                    } else if (cmd_type == "C_return") {
                        codewriter.WriteReturn();
                    } else {
                        codewriter.WriteArithmatic(cmd_type);
                    }
                }
            }
        }

        // Then: the rest
        for (const auto& file : vmFiles) {
            if (file.filename() != "Sys.vm") {
                Parser parser(file.string());
                codewriter.setFileName(file.string());
                while (parser.hasMoreLines()) {
                    parser.advance();
                    string cmd_type = parser.commandType();

                    if (cmd_type == "C_push" || cmd_type == "C_pop") {
                        codewriter.WritePushPop(cmd_type, parser.arg1(), parser.arg2());
                    } else if (cmd_type == "C_label") {
                        codewriter.WriteLabel(parser.arg1());
                    } else if (cmd_type == "C_goto") {
                        codewriter.WriteGoto(parser.arg1());
                    } else if (cmd_type == "C_if") {
                        codewriter.WriteIf(parser.arg1());
                    } else if (cmd_type == "C_call") {
                        codewriter.WriteCall(parser.arg1(), parser.arg2());
                    } else if (cmd_type == "C_function") {
                        codewriter.WriteFunction(parser.arg1(), parser.arg2());
                    } else if (cmd_type == "C_return") {
                        codewriter.WriteReturn();
                    } else {
                        codewriter.WriteArithmatic(cmd_type);
                    }
                }
            }
        }

        codewriter.close();

    } else {
        // Single file case
        Parser parser(inputPath);
        CodeWriter codewriter(inputPath);
        codewriter.setFileName(inputPath);

        while (parser.hasMoreLines()) {
            parser.advance();
            string cmd_type = parser.commandType();

            if (cmd_type == "C_push" || cmd_type == "C_pop") {
                codewriter.WritePushPop(cmd_type, parser.arg1(), parser.arg2());
            } else if (cmd_type == "C_label") {
                codewriter.WriteLabel(parser.arg1());
            } else if (cmd_type == "C_goto") {
                codewriter.WriteGoto(parser.arg1());
            } else if (cmd_type == "C_if") {
                codewriter.WriteIf(parser.arg1());
            } else if (cmd_type == "C_call") {
                codewriter.WriteCall(parser.arg1(), parser.arg2());
            } else if (cmd_type == "C_function") {
                codewriter.WriteFunction(parser.arg1(), parser.arg2());
            } else if (cmd_type == "C_return") {
                codewriter.WriteReturn();
            } else {
                codewriter.WriteArithmatic(cmd_type);
            }
        }

        codewriter.close();
    }

    return 0;



}
