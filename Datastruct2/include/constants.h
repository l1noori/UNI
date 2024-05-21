#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const std::string output = "output.txt"; // Standard 'output' file when using debugmode. Default: output.txt
const double minDifference = 0.000001; // Minimal difference for a double to be 'equal' to. Default: 0.000001
const char varED = 'x'; // Var relative to which eval & diff are performed. Default: x

#endif