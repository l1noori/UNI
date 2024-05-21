
/**
 * Unambiguous grammar definition:
 * 
 * <Expr>    ::= <Term> [ OR <Expr> ]
 * 
 * <Term>    ::= <Fact> [ <Term> ]
 * 
 * <Fact>    ::= VAR [ STAR ]
 *            | ( <Expr> ) [ STAR ]
 * 
 */

#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>

// Define the State struct
struct State {
    int statenr;
    char character;
    int left;
    int right;

    // Constructor for initializing State with default values
    State(int nr = 0, char ch = '\0', int l = 0, int r = 0) : statenr(nr), character(ch), left(l), right(r) {}
};

// Define the Automaat class
class Automaat {
public:
    std::vector<State> states;

    Automaat ();
    Automaat(const State& State);

    Automaat make(std::string input) const;
    bool dot(std::string input) const;
    bool mat(std::string input) const;
    

private:

    void addState(const State& state);
    Automaat concatenate(const Automaat& other) const;
    Automaat unionOperation (Automaat& other, int & currentState) const;
    Automaat starOperation (int & currentState) const;
    Automaat leftState(int i) const;
    Automaat rightState(int i) const;

    Automaat Expr(std::string & input, size_t & currentPos, int & currentState);
    Automaat Term(std::string & input, size_t & currentPos, int & currentState);
    Automaat Fact(std::string & input, size_t & currentPos, int & currentState);

    bool matchAutom(std::string input) const;
    void enqueue(std::queue<int> & q, std::queue<int> & visited, char & c, int currentState) const;
};

// Empty Constructor
Automaat::Automaat() {
}

// Constructor to initialize the Automaat with a single state
Automaat::Automaat(const State& State) {
    states.push_back(State);
}

// Add a state to the automaton
void Automaat::addState(const State& state) {
    states.push_back(state);
}

// Concatenation operation bb
// 1 2   3 4
// b 3   b ?
// 2 -   4 -
Automaat Automaat::concatenate(const Automaat& other) const {
    Automaat result = *this;
    State temp = *other.states.begin();
    result = result.leftState(temp.statenr);
    result.states.insert(result.states.end(), other.states.begin(), other.states.end());
    return result;
}

// Union operation a|b
// 5  1 2  3 4  6
// -  a -  b -  -
// 1  2 6  4 6  ?
// 3  - -  - -  -
Automaat Automaat::unionOperation (Automaat& other, int & currentState) const {
    Automaat result = *this;
    State tempState = *other.states.begin();

    // Create a state for the front, have it point to 'b'
    Automaat temp(State(currentState, '&', 0, tempState.statenr)); 
    currentState++;

    // Adjust the two Automata to point to the state at the end of the union
    result = result.leftState(currentState);
    other = other.leftState(currentState);

    // Combine the Automata and finish up Union
    result.states.insert(result.states.end(), other.states.begin(), other.states.end());
    result = temp.concatenate(result); // Add the front state to to the union, pointing it to 'a'
    result.addState(State(currentState, '&', 0, 0)); // Add a state at end of union
    currentState++;
    return result;
}

// Star Operation a*
// 3 1 2 4 
// - a - -
// 1 2 4 ?
// 4 - 1 -
Automaat Automaat::starOperation (int & currentState) const {
    Automaat result = *this;
    State tempState = *result.states.begin();

    result = result.rightState(tempState.statenr);
    Automaat temp(State(currentState, '&', 0, currentState+1)); 
    result = temp.concatenate(result);
    currentState++;
    Automaat temp2(State(currentState, '&', 0, 0)); 
    currentState++;
    return result.concatenate(temp2);
}

// Replace left state with i
Automaat Automaat::leftState(int i) const{
    Automaat result = *this;
    if (!result.states.empty()) {
        State lastState = *result.states.rbegin();
        lastState.left = i;
        result.states.pop_back();
        result.states.push_back(lastState);
    }
    return result;
}

// Replace right state with i
Automaat Automaat::rightState(int i) const {
    Automaat result = *this;
    if (!result.states.empty()) {
        State lastState = *result.states.rbegin();
        lastState.right = i;
        result.states.pop_back();
        result.states.push_back(lastState);
    }
    return result;
}



// Function to read the next character
void readNextChar(std::string & input, size_t & currentPos) {
    if (currentPos < input.size()) {
        currentPos++;
    }
}

// Function to check if the current character is a lowercase letter
bool isLowercaseLetter(char c) {
    return (c >= 'a' && c <= 'z');
}

// Recursive descent parser for the expression
Automaat Automaat::Expr(std::string & input, size_t & currentPos, int & currentState) {
    Automaat Aut1 = Term(input, currentPos, currentState);
    
    if (currentPos < input.size() && input[currentPos] == '|') {
        readNextChar(input, currentPos);; // Consume '|'
        Automaat Aut2 = Expr(input, currentPos, currentState); // Recursion
        return Aut1.unionOperation(Aut2, currentState);
    }

    return Aut1;
}

// Recursive descent parser for the term
Automaat Automaat::Term(std::string & input, size_t & currentPos, int & currentState) {
    Automaat Aut = Fact(input, currentPos, currentState);

    if (isLowercaseLetter(input[currentPos]) || input[currentPos] == '(') {
        Automaat Aut2 = Term(input, currentPos, currentState); // Recursion
        Aut = Aut.concatenate(Aut2);
    }

    return Aut;
}

// Recursive descent parser for the factor
Automaat Automaat::Fact(std::string & input, size_t & currentPos, int & currentState) {
    if (currentPos < input.size() && input[currentPos] == '(') {
        readNextChar(input, currentPos); // Consume '('
        Automaat Aut = Expr(input, currentPos, currentState);

        if (currentPos < input.size() && input[currentPos] == ')') {
            readNextChar(input, currentPos); // Consume ')'
        } else {
            std::cerr << "Error: Expected ')'." << std::endl;
        }

        if (currentPos < input.size() && input[currentPos] == '*') {
            readNextChar(input, currentPos);; // Consume '*'
            Aut = Aut.starOperation(currentState);
            return Aut;
        }

        return Aut;
    } else if (isLowercaseLetter(input[currentPos])) {
        char currentChar = input[currentPos];
        Automaat Aut(State(currentState, currentChar, currentState+1, 0)); // Create an automaton with a single state
        currentState++; 
        Aut.addState(State(currentState, '&', 0, 0)); // Add an empty state
        currentState++;
        readNextChar(input, currentPos);; // Consume the current letter

        if (currentPos < input.size() && input[currentPos] == '*') {
            readNextChar(input, currentPos);; // Consume '*'
            Aut = Aut.starOperation(currentState);
        }

        return Aut;
    } else {
        std::cerr << "Error: Invalid character." << std::endl;
        return Automaat(); // Return an automaton with no states (invalid)
    }
}

// Make automaton according to given input
Automaat Automaat::make(std::string input) const{
    size_t currentPos = 0;
    int currentState = 1;
    Automaat Aut = Aut.Expr(input, currentPos, currentState);
    State temp = *Aut.states.rbegin();
    if ((temp.left != 0) || (temp.right != 0)){
        // If the last has a branch, add an empty end-state (see matching function)
        Automaat Aut2(State(currentState, '&', 0, 0));
        Aut = Aut.concatenate(Aut2);
    }
    return Aut;
}

// Dot function to create & fill a file with DOT notation of current saved automaton
bool Automaat::dot(std::string input) const{
    input=input.substr(input.find_first_of(" \t")+1);
    std::string fileName = input.substr(0, input.find(" "));
    std::ofstream outfile (fileName);

    // Check if the file is open
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return false;
    }

    outfile << "digraph G {\n";
    outfile << "rankdir=\"LR\"\n";
    for (const auto& state : states) {
        if (state.left !=0){
            outfile << state.statenr << " -> " << state.left;
            if (state.character != '&')
                outfile << "[label =\"" << state.character << "\"]";
            outfile << '\n';
        }
        if (state.right !=0){
            outfile << state.statenr << " -> " << state.right;
            if (state.character != '&')
                outfile << "[label =\"" << state.character << "\"]";
            outfile << '\n';
        }
        if ((state.left ==0) && (state.right ==0))
            outfile << state.statenr;
    } 
    outfile << "}\n";
    outfile.close();
    return true;
}
bool search(std::queue<int> q, int x){ 
        while(!q.empty()){ 
            if(q.front() == x) 
                return true; 
            q.pop(); 
        } 
        return false; 
} 

void Automaat::enqueue(std::queue<int> & q, std::queue<int> & visited, char & c, int currentState) const{
    if (search(visited, currentState))
        return;
    visited.push(currentState);
    for (const auto& state : states) {
        if (state.statenr == currentState && (state.character == c)) {
            if ((state.character == c))
                c = '&'; // found the character, now we can start saving states
            q.push(state.left);
            enqueue(q, visited, c, state.left);
            q.push(state.right);
            enqueue(q, visited, c, state.right);
        } else if (state.statenr == currentState && (state.character == '&')){
            enqueue(q, visited, c, state.left);
            enqueue(q, visited, c, state.right);
        } // we will keep looking
    }
}

// Matchnode function to match the Automaton breadth-first
bool Automaat::matchAutom(std::string input) const{
    // Initialize the queue for breadth-first traversal
    std::queue<int> bfsQueue;
    State temp = *states.begin();
    State temp2 = *states.rbegin();
    bfsQueue.push(temp.statenr); // Start from the initial state
    std::queue<int> visitedNodes;
    // Traverse the automaton
    for (char c : input) {
        visitedNodes = {};
        int levelSize = bfsQueue.size();

        while (levelSize--) {
            int currentState = bfsQueue.front();
            bfsQueue.pop();
            enqueue(bfsQueue, visitedNodes, c, currentState); // Check transitions for the current state and enqueue next states
        }
    }

    // Check if the automaton reached a final state
    while (!bfsQueue.empty()) {
        int currentState = bfsQueue.front();
        bfsQueue.pop();

        if (currentState == temp2.statenr) {
            return true;
        }
    }
    return false;
}

// Matching function, matching the string to the automaton
bool Automaat::mat(std::string input) const{
    if (states.empty()){
        std::cout << "The current Automaton is empty." << std::endl;
        if (input == "$") return true;
        else return false;
    }
    return matchAutom(input);
}

// Menu with commands
void readMenu(){
    std::cout << "|------------------------------------------------------------------------------------------------------------------------------|" << std::endl;
    std::cout << "| [input]          : [description]                                                                                             |" << std::endl;
    std::cout << "| exp <expression> : reads the expression and creates a corresponding automaton (warning! will delete current saved expression)|" << std::endl;
    std::cout << "| dot <filename>   : saves automaton to given file, in DOT notation                                                            |" << std::endl;
    std::cout << "| mat <string>     : checks whether this string is accepted by the saved automaton                                             |" << std::endl;
    std::cout << "| tab              : gives current automaton in table form, unordered                                                          |" << std::endl;
    std::cout << "| end              : ends program                                                                                              |" << std::endl;
    std::cout << "| help             : shows this menu                                                                                           |" << std::endl;
    std::cout << "|------------------------------------------------------------------------------------------------------------------------------|" << std::endl;
}

bool readLine(std::string input, Automaat & autb){

    // Trim leading and trailing whitespaces from the input
    input.erase(0, input.find_first_not_of(" \t"));
    input.erase(input.find_last_not_of(" \t") + 1);

    // Check for empty string
    if (input.empty()) {
        std::cout << "Error: Empty input. Please enter a valid command." << std::endl;
        return false;
    }

    std::string word;
	std::istringstream infile(input);
	infile >> word;
    if (word == "exp"){
        infile >> word;
        autb.states.clear();
        autb = autb.make(word);
    } else if (word == "dot"){
        infile >> word;
        if(!autb.dot(word)){
            std::cout << "An error occured." << std::endl;
        }
    } else if (word == "end"){
        return true;
    }else if (word == "help"){
        readMenu();
    } else if (word == "mat"){
        infile >> word;
        if(autb.mat(word)){
            std::cout << "Match." << std::endl;
        } else {
            std::cout << "No match." <<std::endl;
        }
    } else if (word =="tab"){
        // Output the states in the resulting automaton. Debug function
        std::cout << "States in the resulting automaton: ";
        for (const auto& state : autb.states) {
            std::cout << std::endl << state.statenr << ": (" << state.character << ", " << state.left << ", " << state.right << ") ";
        }
        std::cout << std::endl;
    } else {
        std::cout << word << " is not a valid input." << std::endl;
        return false;
    }
    return false;
}

int main () {
    bool endProgram = false;
    std::string input;
    Automaat autb;

    // Check for file read errors
    if (std::ferror(stdin)) {
        std::cerr << "Error reading from file." << std::endl;
        return 1;
    }
    readMenu();

    while (!endProgram){
        std::getline(std::cin, input);
        endProgram = readLine(input, autb);
    }
    return 0;
}//main
