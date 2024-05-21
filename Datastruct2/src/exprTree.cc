#include "exprTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

node::~node(){
    if (left!=nullptr){
        delete left;
    }
    if (right!=nullptr){
        delete right;
    }
}

exprTree::exprTree(){
    root = nullptr;
}

exprTree::~exprTree(){
    clear();
}

exprTree::exprTree(std::istringstream& infile){
    constructTree(infile);
}

bool exprTree::constructTree(std::istringstream& infile){
    root = new node;
    if (root == nullptr){
        std::cout << "Allocation error." << std::endl;
        return false;
    }
    if (!constructTreeNode(infile, root)){
        return false;
    }
    return true;
}

bool is_number(std::string input)
{
    try {
        std::stod(input);
    }
    catch(...) {
        return false;
    }
    return true;
}

bool exprTree::makeNode(std::string input, node* subTree){
    if (is_number(input)){
        subTree->type = NUMBER;
        subTree->arity = 0;
        subTree->number = std::stod(input);
    } else if (input == "cos"){
        subTree->type = COSINE;
        subTree->arity = 1;
    } else if (input == "sin") {
        subTree->type = SINE;
        subTree->arity = 1;
    } else if (input == "pi"){
        subTree->type = PI;
        subTree->arity = 0;
    } else {
        char inputChar = input.at(0);
        switch (inputChar){
            case '+':
                subTree->type = PLUS;
                subTree->arity = 2;
                break;
            case '-':
                subTree->type = MINUS;
                subTree->arity = 2;
                break;
            case '*':
                subTree->type = MULTIPLY;
                subTree->arity = 2;
                break;
            case '/':
                subTree->type = DIVIDE;
                subTree->arity = 2;
                break;
            case '^':
                subTree->type = POW;
                subTree->arity = 2;
                break;
            default: // any other number or variable
                if (inputChar >='a' && inputChar <= 'z'){
                    subTree->type = VAR;
                    subTree->arity = 0;
                    subTree->varName = inputChar;
                } else {
                    std::cout << "The character " << inputChar << "is not legal in this context." <<std::endl;
                    return false;
                }
        }
    }
    return true;
}

bool exprTree::constructTreeNode(std::istringstream& infile, node* subTree){
    std::string word;
    infile >> word;
    if (!makeNode(word, subTree)) return false;
    switch (subTree->arity){
        case 2:// plus, minus, multiply, divide or POW
            subTree->left = new node;
            if (subTree->left == nullptr){
                std::cout << "Allocation error." << std::endl;
                return false;
            }
            if (constructTreeNode(infile, subTree->left)){
                subTree->right = new node;
                if (subTree->right == nullptr){
                    std::cout << "Allocation error." << std::endl;
                return false;
                }
                return (constructTreeNode(infile, subTree->right));
            } else return false;
        case 1: // sine or cosine
            subTree->left = new node;
            if (subTree->left == nullptr){
                std::cout << "Allocation error." << std::endl;
                return false;
            }
            return constructTreeNode(infile, subTree->left);
        case 0:
            return true;
        default:
            return false;
    }

}
bool exprTree::print(bool debugMode, std::ofstream& outfile){
    bool noErrors = true;
    if (root != nullptr) {
        if (!debugMode) {
            noErrors = printTree(root);
            std::cout << std::endl;
        }
        else {
            noErrors = printTxtTree(root, outfile);
            outfile << "\n";
        }
    }
    return noErrors;
}

bool exprTree::printTree(node* subTree){
    switch(subTree->arity){
        case 2:
            std::cout << "( ";
            if(!printTree(subTree->left)) return false;
            std::cout << " ";
            std::cout << printNode(subTree);
            std::cout << " ";
            if(!printTree(subTree->right)) return false;
            std::cout << " )";
            break;
        case 1:
            std::cout << printNode(subTree) << "( ";
            if(!printTree(subTree->left)) return false;
            std::cout << " )";
            break;
        case 0:
            std::cout << printNode(subTree);
            break;
        default:
            return false;
    }
    return true;
}

bool exprTree::printTxtTree(node* subTree, std::ofstream& outfile){
    switch(subTree->arity){
        case 2:
            outfile << "( ";
            if(!printTxtTree(subTree->left, outfile)) return false;
            outfile << " ";
            outfile << printNode(subTree);
            outfile << " ";
            if(!printTxtTree(subTree->right, outfile)) return false;
            outfile << " )";
            break;
        case 1:
            outfile << printNode(subTree) << "( ";
            if(!printTxtTree(subTree->left, outfile)) return false;
            outfile << " )";
            break;
        case 0:
            outfile << printNode(subTree);
            break;
        default:
            return false;
    }
    return true;
}

std::string exprTree::printNode(node* subTree){
    std::ostringstream s;
    switch(subTree->type){
            case VAR:
                s << subTree->varName;
                return s.str();
                break;
            case NUMBER: 
                s << subTree->number;
                return s.str();
                break;
            case PLUS:
                return "+";
                break;
            case MINUS:
                return "-";
                break;
            case MULTIPLY:
                return "*";
                break;
            case DIVIDE:
                return "/";
                break;
            case POW:
                return "^";
                break;
            case SINE:
                return "sin";
                break;
            case COSINE:
                return "cos";
                break;
            case PI:
                subTree->number = M_PI;
                return "pi";
                break;
        }
    return "";
}

bool exprTree::dot(std::string input){
    input=input.substr(input.find_first_of(" \t")+1);
    std::string fileName = input.substr(0, input.find(" "));
    std::ofstream outfile (fileName);
    outfile << "digraph G {\n";
    int index = 1;
    bool linkerTak = true;
    outfile << "\t" << index << " [label=\"";
    outfile << printNode(root) << "\"]\n";;    
    if(!printDotTree(root, outfile, index, linkerTak)) return false;
    outfile << "}\n";
    outfile.close();
    return true;
}

void exprTree::makeArrows(node* subTree, std::ofstream& outfile, int & index){
    if (subTree->right != nullptr){
        outfile << index << " -> ";
        makeArrows(subTree->right, outfile, subTree->right->tempId);
    } else {
        outfile << index << "\n";
    }
}

bool exprTree::printDotTree(node* subTree, std::ofstream& outfile, int & index, bool linkerTak){
    int oldIndex = index;
    index++;
    switch(subTree->arity){
        case 2:
            outfile << "\t" << index << " [label=\"";
            outfile << printNode(subTree->left) << "\"]\n";
            outfile << "\t" << oldIndex << " -> " << index << "\n";
    
            printDotTree(subTree->left, outfile, index, true);

            outfile << "\t" << index << " [label=\"";
            outfile << printNode(subTree->right) << "\"]\n";

            printDotTree(subTree->right, outfile, index, false);
            break;
        case 1:
            outfile << "\t" << index << " [label=\"";
            outfile << printNode(subTree->left) << "\"]\n";
            outfile << "\t" << oldIndex << " -> " << index << "\n"; 
            printDotTree(subTree->left, outfile, index, true);
            break;
        case 0:
            break;
        default:
            return false;
    }
    if (linkerTak){
                if (subTree->right != nullptr){
                    outfile << "\t";
                    makeArrows(subTree, outfile, oldIndex);
                }
            } else {
                subTree->tempId = oldIndex;
    }
    return true;
}

bool exprTree::simp(){
    if (root != nullptr){
        subSimplify(root);
        return true;
    }
    return false;
}

bool checkSame(double x, double y){
    return (fabs(x - y) < minDifference);
}

bool exprTree::simplifySinCos(node*& subTree){
    double x;
    if (subTree->left->type == NUMBER
    || subTree->left->type == PI) {
        if (subTree->type == SINE){
            x = subTree->left->number;
            delete subTree->left;
            subTree = makeNumber(sin(x));
            return true;
        } else if (subTree->type == COSINE){
            x = subTree->left->number;
            delete subTree->left;
            subTree = makeNumber(cos(x));
            return true;
        }
    }
    return false;
}

bool exprTree::simplifyLeft(node*& subTree){
    if (subTree->left->type == NUMBER 
    && checkSame(subTree->left->number, 0)){
        switch (subTree->type){
            case PLUS: // 0 + E = E
                delete subTree->left;
                subTree = copy(subTree->right);
                return true;
            case MULTIPLY: // 0 * E = 0
            case DIVIDE:  // 0 / E = 0
            case POW: // 0 ^ E = 0
                if (subTree->right->type == NUMBER 
                && subTree->right->number == 0) return false; // 0 ^ 0 is undefined
                delete subTree->right;
                subTree = copy(subTree->left);
                return true;
            default:
                break;
        }
    } else if (subTree->left->type == NUMBER 
    && checkSame(subTree->left->number, 1)) {
        switch (subTree->type){
            case MULTIPLY: // 1 * E = E
                delete subTree->left;
                subTree = copy(subTree->right);
                return true;
            case POW: // 1 ^ E = 1
                delete subTree->right;
                subTree = copy(subTree->left);
                return true;
            default:
                break;
        }
    } else if ((subTree->right->type == NUMBER ||
    subTree->right->type == PI) &&
    (subTree->left->type == NUMBER ||
    subTree->left->type == PI)) {
        double x = subTree->left->number;
        double y = subTree->right->number;
        switch (subTree->type){
            case PLUS:
                delete subTree->left; delete subTree->right;
                subTree = makeNumber(x+y);  
                return true;
            case MINUS:
                delete subTree->left; delete subTree->right;
                subTree = makeNumber(x-y);
                return true;
            case MULTIPLY:
                delete subTree->left; delete subTree->right;
                subTree = makeNumber(x*y);
                return true;
            case DIVIDE:
                if (y == 0) return false; // Undefined
                delete subTree->left; delete subTree->right;
                subTree = makeNumber(x/y);
                return true;
            default:
                break;
        } 
    } else if ((subTree->right->type == VAR && subTree->left->type == VAR)
    && (subTree->right->varName == subTree->left->varName)) { // Variables are the same
        switch (subTree->type){
            case PLUS: // x + x = 2 * x
                subTree->type = MULTIPLY;
                subTree->left = makeNumber(2);
                return true;
            case MINUS: // x - x = 0
                delete subTree->right; delete subTree->left;
                subTree = makeNumber(0);
                return true;
            case MULTIPLY: // x * x = x ^ 2
                subTree->type = POW;
                subTree->right = makeNumber(2);
                return true;
            case DIVIDE: // x / x = 1
                delete subTree->right; delete subTree->left;
                subTree = makeNumber(1);
                return true;
            default:
                break;
        } 
    }
    return false;
}

node* exprTree::copy(const node* copyTree){
    if (copyTree == nullptr) return nullptr;

    node* newTree = new node;
    newTree->type = copyTree->type;
    newTree->arity = copyTree->arity;
    newTree->varName = copyTree->varName;
    newTree->number = copyTree->number;
    newTree->left = copy(copyTree->left);
    newTree->right = copy(copyTree->right);

    return newTree;
}

node* exprTree::makeNumber(double input){
    node* newTree = new node;
    newTree->type = NUMBER;
    newTree->arity = 0;
    newTree->number = input;
    newTree->left = nullptr;
    newTree->right = nullptr;
    return newTree;
}

bool exprTree::simplifyRight(node*& subTree){
    if (subTree->right->type == NUMBER
    && checkSame(subTree->right->number, 0)){
        switch (subTree->type){
            case PLUS: // E + 0 = E
            case MINUS: // E - 0 = E
                delete subTree->right;
                subTree = copy(subTree->left);
                return true;
            case MULTIPLY: // E * 0 = 0
                delete subTree->left;
                subTree = copy(subTree->right);
                return true;
            case DIVIDE:  // E / 0 = Undefined
                return false;
            case POW: // E ^ 0 = 1
                if ( subTree->left->type == NUMBER 
                && subTree->left->number == 0) return false; // 0 ^ 0 is undefined
                delete subTree->right; delete subTree->left;
                subTree = makeNumber(1);
                return true;
            default:
                break;
        }
    } else if (subTree->right->type == NUMBER 
        && checkSame(subTree->right->number, 1)){
        switch (subTree->type){
            case MULTIPLY: // E * 1 = E
            case DIVIDE:  // E / 1 = E
            case POW: // E ^ 1 = E
                delete subTree->right;
                subTree = copy(subTree->left);
                return true;
            default:
                break;
        }
    }
    return false;
}

void exprTree::subSimplify(node*& subTree){
    if (subTree == nullptr 
    || subTree->arity == 0) return; // Cannot be simplified
    subSimplify(subTree->left);
    if (subTree->arity == 1){
        simplifySinCos(subTree);
    } else if (subTree->arity == 2){
        if (simplifyLeft(subTree)){
            return;
        }
        subSimplify(subTree->right);
        simplifyRight(subTree);
    } // Simplify current token, from left to right.
}

void exprTree::diffMultiply(node*& subTree){
    node * temp = subTree;
    node * temp2 = new node;
    node * a = subTree->left;
    node * b = subTree->right;
    std::string line;

    line = "+ * a b * b a";
    std::istringstream infile(line);
    constructTreeNode(infile, temp2);
    subTree = copy(temp2);
    delete temp2;
    subTree->left->left = copy(a);
    subTree->left->right = copy(b);
    subTree->right->right = copy(a);
    subTree->right->left = copy(b);
    delete temp;
    subDiff(subTree->left->left);
    subDiff(subTree->right->left);
}

void exprTree::diffDivide(node*& subTree){
    node * temp = subTree;
    node * temp2 = new node;
    node * a = subTree->left;
    node * b = subTree->right;
    std::string line;

    line = "/ - * a b * b a ^ b 2";
    std::istringstream infile(line);
    constructTreeNode(infile, temp2);
    subTree = copy(temp2);
    delete temp2;
    subTree->left->left->left = copy(a);
    subTree->left->left->right = copy(b);
    subTree->left->right->right = copy(a);
    subTree->left->right->left = copy(b);
    delete temp;
    subDiff(subTree->left->left->left);
    subDiff(subTree->left->right->left);
}

void exprTree::diffPow(node*& subTree){
    node * temp = subTree;
    node * temp2 = new node;
    node * a = subTree->left;
    node * b = subTree->right;
    std::string line;

    line = "* * b ^ a - b 1 c";
    std::istringstream infile(line);
    constructTreeNode(infile, temp2);
    subTree = copy(temp2);
    delete temp2;
    subTree->left->left = copy(b);
    subTree->left->right->left = copy(a);
    subTree->left->right->right->left = copy(b);
    subTree->right = copy(a);
    subDiff(subTree->right);
    delete temp;
}

void exprTree::diffSin(node*& subTree){
    node * temp = subTree;
    node * temp2 = new node;
    node * a = subTree->left;
    std::string line;

    line = "* cos a c";
    std::istringstream infile(line);
    constructTreeNode(infile, temp2);
    subTree = copy(temp2);
    delete temp2;
    subTree->left->left = copy(a);
    subTree->right = copy(a);
    subDiff(subTree->right);
    delete temp;
}

void exprTree::diffCos(node*& subTree){
    node * temp = subTree;
    node * temp2 = new node;
    node * a = subTree->left;
    std::string line;

    line = "* * -1 sin a c";
    std::istringstream infile(line);
    constructTreeNode(infile, temp2);
    subTree = copy(temp2);
    delete temp2;
    subTree->left->right->left = copy(a);
    subTree->right = copy(a);
    subDiff(subTree->right);
    delete temp;
}

void exprTree::subDiff(node*& subTree){
    if (subTree == nullptr) return;
    node * temp = subTree;
    switch(subTree->type){
        case NUMBER:
        case PI:
            subTree = makeNumber(0);
            delete temp;
            break;
        case VAR:
            if (subTree->varName == varED)
                subTree = makeNumber(1);
            else
                subTree = makeNumber(0);
            delete temp;
            break;
        case PLUS:
        case MINUS:
            subDiff(subTree->left);
            subDiff(subTree->right);
            break;
        case MULTIPLY: 
            diffMultiply(subTree);
            break;
        case DIVIDE:
            diffDivide(subTree);
            break;
        case POW:
            if (subTree->left->type == VAR && subTree->left->varName == varED){
                diffPow(subTree);
            } else {
                subTree = makeNumber(0);
                delete temp;
            }
            break;
        case SINE:
            diffSin(subTree);
            break;
        case COSINE:
            diffCos(subTree);
            break;
    }
}


bool exprTree::diff(){
    if (root != nullptr){
        subDiff(root);
        simp();
        return true;
    }
    return false;
}

void exprTree::replaceVar(node*& subTree, char var, const node* N){
    if (subTree == nullptr) return;
    if (subTree->type == VAR && subTree->varName == var){
        subTree = copy(N);
    }
    replaceVar(subTree->left, var, N);
    replaceVar(subTree->right, var, N);

}

bool exprTree::eval(std::string input){
    node * N;
    if (root == nullptr) return false;
    if (is_number(input)){
        N = makeNumber(std::stod(input));
    } else if (input.size() == 0){
        N = new node;
        N->type = VAR;
        N->varName = input.at(0);
        N->arity = 0;
    } else return false;
    replaceVar(root, varED, N);
    delete N;
    return simp();
}


void exprTree::clear(){
    if (root!=nullptr){
        delete root;
    }
}