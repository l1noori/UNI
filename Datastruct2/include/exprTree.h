#ifndef EXPR_TREE_H
#define EXPR_TREE_H

#include <iostream>
#include <string>
#include "constants.h"

enum nodeType {
    VAR, NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, POW, SINE, COSINE, PI
};

struct node {
    nodeType type;
    int arity;
    char varName;
    double number;
    int tempId;
    node* left = nullptr;
    node* right = nullptr;

    ~node(); // destructor
};

bool checkSame(double x, double y);

class exprTree{
    private:
        node* root;
        node* parent;
        bool constructTreeNode(std::istringstream& infile, node* subTree);
        bool makeNode(std::string input, node* subTree);
    public:
        exprTree();
        exprTree(std::istringstream& infile);
        ~exprTree();

        bool constructTree(std::istringstream& infile);
        void clear();
    
        bool print(bool debugMode, std::ofstream& outfile);
        bool printTree(node* subTree);
        bool printTxtTree(node* subTree, std::ofstream& outfile);
        std::string printNode(node* subTree);

        bool dot(std::string input);
        bool printDotTree(node* subTree, std::ofstream& outfile, int & index, bool linkerTak);
        void makeArrows(node* subTree, std::ofstream& outfile, int & index);

        bool simp();
        void subSimplify(node*& subTree);
        bool simplifyLeft(node*& subTree);
        bool simplifyRight(node*& subTree);
        bool simplifySinCos(node*& subTree);

        node* copy(const node* copyTree);
        node* makeNumber(double input);

        bool diff();
        void subDiff(node*& subTree);
        void diffMultiply(node*& subTree);
        void diffDivide(node*& subTree);
        void diffPow(node*& subTree);
        void diffSin(node*& subTree);
        void diffCos(node*& subTree);

        bool eval(std::string input);
        void replaceVar(node*& subTree, char var, const node* N);

};

#endif