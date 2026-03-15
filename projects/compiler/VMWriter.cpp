// VMWriter
#include "VMWriter.h"
#include <iostream>
#include <fstream>
#include <algorithm>


// Constructor: Creates a new ouput .vm file and prepares it for writing
VMWriter::VMWriter(const string& outputFile){
	out.open(outputFile);
}

// writePush: Write a VM push command
void VMWriter::writePush(string segment, int index){
	out << "push " << segment << " " << to_string(index) << "\n";
}

// writePop: Write a VM pop command
void VMWriter::writePop(string segment, int index){
	out << "pop " << segment << " " << to_string(index) << "\n";
}

// writeArithemtic: Write a VM arithmetic-logical command
void VMWriter::writeArithmetic(string command){
	string cmd;
	if (command == "+"){
		cmd = "add"; 
	}
	else if (command == "|"){
		cmd = "or";
	}
	else if (command == "-"){
		cmd = "sub";
	}
	else if (command == "*"){
		cmd = "call Math.multiply 2";
	}
	else if (command == "/"){
		cmd = "call Math.divide 2";
	}
	else if (command == "~"){
		cmd = "not";
	}
	else if (command == "="){
		cmd = "eq";
	}
	else if (command == "<"){
		cmd = "lt";
	}

	else if (command == ">"){
		cmd = "gt";
	}

	else if (command == "&"){
		cmd = "and";
	}
	else {
		cmd = command;
	}
	out << cmd << "\n";
}

// writeLabel: Write a VM label command
void VMWriter::writeLabel(string label){
	out <<"label " << label << "\n";
}

// writeGoto: Write a VM goto command
void VMWriter::writeGoto(string label){
	out <<"goto " << label << "\n";
}

// writeIf: Write a VM if-goto commmand
void VMWriter::writeIf(string label){
	out <<"if-goto " << label << "\n";
}

// writeCall: Writes a VM call command
void VMWriter::writeCall(string name, int nArgs){
	out <<"call " << name << " " << to_string(nArgs) << "\n";
}

// writeFunction: Write a VM function commmand
void VMWriter::writeFunction(string name, int nVars){
	out << "function " << name << " " << to_string(nVars) << "\n";
}

// writeReturn: Write a VM return command
void VMWriter::writeReturn(){
	out << "return \n";
}

// close: Close the output file
void VMWriter::close(){
	out.close();
}



