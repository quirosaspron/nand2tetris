#ifndef VM_WRITER_H
#define VM_WRITER_H

#include <string>
#include <fstream>
#include <map>
using namespace std;

class VMWriter {
public:
	// Constructor: Creates a new ouput .vm file and prepares it for writing
	VMWriter(const string& outputFile);
	
	// writePush: Write a VM push command
	void writePush(string segment, int index);

	// writePop: Write a VM pop command
	void writePop(string segment, int index);

	// writeArithemtic: Write a VM arithmetic-logical command
	void writeArithmetic(string command);

	// writeLabel: Write a VM label command
	void writeLabel(string label);

	// writeGoto: Write a VM goto command
	void writeGoto(string label);

	// writeIf: Write a VM if-goto commmand
	void writeIf(string label);

	// writeCall: Writes a VM call command
	void writeCall(string name, int nArgs);

	// writeFunction: Write a VM function commmand
	void writeFunction(string name, int nVars);

	// writeReturn: Write a VM return command
	void writeReturn();

	// close: Close the output file / stream
	void close();
private:
    ofstream out;
};
#endif 
