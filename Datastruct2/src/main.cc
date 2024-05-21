#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "exprTree.h"
#include "constants.h"

bool readLine(std::string input, bool debugMode, exprTree & tree, std::ofstream& outfile){
    std::string word;
	std::istringstream infile(input);
	infile >> word;
    if (word == "exp"){
        tree.clear();
        if(!tree.constructTree(infile)){
            std::cout << "An error occured." << std::endl;
            return true;
        }
    } else if (word == "print"){
        if(!tree.print(debugMode, outfile)){
            std::cout << "An error occured." << std::endl;
        }
    } else if (word == "dot"){
        if(!tree.dot(input)){
            std::cout << "An error occured." << std::endl;
        }
    } else if (word == "end"){
        return true;
    } else if (word == "eval"){
        infile >> word;
        if(!tree.eval(word)){
            std::cout << "An error occured." << std::endl;
        }
    } else if (word == "diff"){
        if(!tree.diff()){
            std::cout << "An error occured." << std::endl;
        }
    } else if (word == "simp"){
        if(!tree.simp()){
            std::cout << "Could not simplify empty tree." << std::endl;
        }
    } else {
        if (!debugMode)
            std::cout << word << " is not a valid input." << std::endl;
        return false;
    }
    return false;
}

int main(int argc, const char** argv){
    bool debugMode = false;
    std::string input;
    exprTree tree;
    bool endProgram = false;
    std::ofstream outfile (output);
    if (argc < 2){
        debugMode = false;
        while (!endProgram){
            std::getline(std::cin, input);
            endProgram = readLine(input, debugMode, tree, outfile);
        };
    } else if (std::string(argv[1]) == "d") {
        debugMode = true;
        std::ifstream inputfile(argv[2], std::ios::binary);
        if (inputfile) {
            while (std::getline(inputfile, input)){
                endProgram = readLine(input, debugMode, tree, outfile);
            }
            inputfile.close();            
        } else {
            std::cerr << "File cannot be opened." << std::endl;
            return EXIT_FAILURE;
        }
    }
    outfile.close();
};