#include <iostream>
#include <filesystem>
#include <string>
#include "CompilationEngine.h"

using namespace std;
namespace fs = std::filesystem;

class JackAnalyzer {
public:
    void run(const string& inputPath) {
        // Check if the input is a single file or a directory
        if (fs::is_regular_file(inputPath)) {
            processFile(inputPath);
        }
	else if (fs::is_directory(inputPath)) {
            for (const auto& entry : fs::directory_iterator(inputPath)) {
                if (entry.path().extension() == ".jack") {
                    processFile(entry.path().string());
                }
            }
        }
    }

private:
    void processFile(const string& jackFile) {
        // Create the output filename (.xml)
        string outputFile = fs::path(jackFile).replace_extension(".xml").string();

        cout << "Compiling: " << jackFile << " → " << outputFile << endl;

        // Create CompilationEngine (constructor runs compileClass)
        CompilationEngine engine(jackFile, outputFile);

        // Close output file explicitly (CompilationEngine could also close in its destructor)
        // engine has already done compileClass in its constructor
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: JackAnalyzer <inputFile.jack | folder>" << endl;
        return 1;
    }

    JackAnalyzer analyzer;
    analyzer.run(argv[1]);
    return 0;
}
