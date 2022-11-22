/* Name of Program : BenyamainYacoob_RPN.cpp
Purpose of Program: Convert RPN into Infix expression
Author: B. Yacoob */

#include <iostream>
#include <stack>    // Stack STL
#include <string>
using namespace std;

class Node {
public:
    Node* leftNode, * rightNode; // Left node and right node pointers
    string info; // What is going to be used for input and stores data inside a node

    Node() {    // Default constructor of Node class if an instance of the class is made
        leftNode = rightNode = 0;
    }

    Node(string input, Node* left = 0, Node* right = 0) {  // Constructor that looks for char input as a parameter
        info = input; leftNode = left; rightNode = right;
    }
};

// Global variables
stack<Node*> stackNode; 
string multiNumbers[100];
bool runFlag;

class ExpressionTree {
private:
    Node* root; // Root node of the expression tree(s)
public:
    ExpressionTree() {  // Public constructor that acts as the default constructor for this class
        root = 0;
    }

    // Some method that checks if the input operand is either a # or variable
    bool checkIfOperand(string operand) {
        for (int a = 0; a < operand.length(); a++) {
            if (operand[a] >= '0' && operand[a] <= '9') {
                return true;
            }
            else return false;
        }
    }

    // Checks if the input operator is either four of the operator signs (+,-,/,*)
    bool checkIfOperator(string isOperator) {
        // Considers all possible combinations: +, -, /, *
        for (int a = 0; a < isOperator.length(); a++) {
            if (isOperator[a] == '/' || isOperator[a] == '*' || isOperator[a] == '+' || isOperator[a] == '-')
                return true;
            else return false;
        }
    }

    // The evaluation of the expression
    double expResult(string userInput[], int count) {
        double topOne, topTwo, result = 0;
        stack<double> stackOperands;
        string temp;

        // Loops through input expression
        for (int a = 0; a < count; a++) {
            if (checkIfOperand(multiNumbers[a])) { // Check if operand
                stackOperands.push(stod((multiNumbers[a])));
            }
            else if (checkIfOperator(multiNumbers[a])) {   // Check if operator
                topOne = stackOperands.top();   // Reveal most recent value, then pop, get the second to first value in stack
                stackOperands.pop();
                topTwo = stackOperands.top();
                stackOperands.pop();

                temp = multiNumbers[a];

                // Get the operator, evaluate two operands, then push the result into the stack
                switch (temp[0]) {
                case 43: result = topTwo + topOne;    // Add
                    stackOperands.push(result);
                    break;
                case 45: result = topTwo - topOne;    // Subtract
                    stackOperands.push(result);
                    break;
                case 47: result = topTwo / topOne;    // Divide
                    stackOperands.push(result);
                    break;
                case 42: result = topTwo * topOne;    // Multiply
                    stackOperands.push(result);
                    break;
                default:
                    break;
                }
            }
        }
        cout << "\nThe result is " << result;
        return result;
    }

    int separateNumbers(string expression, string arr[]) {   // Separate the numbers to identify what are the multidigits 
        string multiNum = "";
        int count = 0;
        runFlag = true;

        for (int a = 0; a < expression.length(); a++) {
            if (isdigit(expression[a]) || expression[a] == '.') {
                multiNum += expression[a];
            }
            else if (expression[a] == ' ') {    // Get the multidigits
                if (multiNum.size() != 0) {
                    arr[count] = multiNum;
                    count++;
                }
                multiNum = "";    // Reset the string
            }
            else if (expression[a] == '/' || expression[a] == '*' || expression[a] == '+' || expression[a] == '-') {    // Get the operators
                arr[count] = expression[a];
                count++;
            }
            else
            {   // Still evaluate the expression even if invalid characters are in there
                cout << "Error! Invalid operator!\n\n";
                runFlag = false;
                break;
            }
        }
        return count;
    }

    // Setting root of the expression tree as an operator
    void setRoot(Node* currentRoot) {
        root = currentRoot;
    }

    void createExpTree(string userInput) {  // String but parsed as individual characters
        ExpressionTree* expTree = new ExpressionTree();
        int countOfNum = separateNumbers(userInput, multiNumbers);

        // Loop through the string since it acts as an array of characters
        for (int index = 0; index < countOfNum; index++) {
            if (checkIfOperand(multiNumbers[index])) {
                root = new Node(multiNumbers[index]);
                stackNode.push(root); // Pushing the operands into the stack
            }
            // When an operator is encountered, create a new node, pop the last two nodes, update pointers, and push operator
            else if (checkIfOperator(multiNumbers[index])) {
                Node* x = new Node(multiNumbers[index]);

                x->rightNode = stackNode.top();
                stackNode.pop();  // Popping
                x->leftNode = stackNode.top(); // Get top value of stack before popping
                stackNode.pop();

                expTree->setRoot(x);
                stackNode.push(expTree->root);   // Push the tree into the stack
            }
        }
        if (runFlag) {
            cout << "Infix expression: ";
            rpnToInfix(expTree->root);  // Make operands children of the operator
            expTree->expResult(multiNumbers, countOfNum);    // Return the evaluation of expression
            cout << endl << endl;
        }
    }

    // Traverse through the tree using inOrder (infix)
    void rpnToInfix(Node* node) {
        if (node != nullptr) {  // Left-Root-Right
            rpnToInfix(node->leftNode);
            cout << node->info;
            rpnToInfix(node->rightNode);
        }
        else return;    // When node is null
    }
};

string tokenizeInput(string input) {    // Ignore any whitespace character
    for (int i = 0; i < input.length(); i++)
    {
        if (isspace(input[i])) { input.erase(i, 1); --i; }   // Decrement to check the string again
    }
    return input;
}

int main()
{
    // Declaration of variables
    ExpressionTree* expTree = new ExpressionTree();
    string inputExpression;

    // Display message to user asking for input
    cout << "Enter an postfix expression (* for termination):\n";
    getline(cin, inputExpression);

    while (true) {
        if (inputExpression == "*") {
            cout << "End.";
            break;
        }
        else {
            expTree->createExpTree(inputExpression); // Method call that constructs our expression tree

            cout << "Enter an postfix expression (* for termination):\n";
            getline(cin, inputExpression);
        }
    }
}