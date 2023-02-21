#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <array>
#include <cstdint>
#include <fstream>
#include <algorithm>

class Tape {
    std::vector<char> tape;
    std::size_t dataPointer;

    public:
    Tape() {
        tape = std::vector<char>(30000, 0);
        dataPointer = 0;    
    }

    void forward_n(int n) {
        dataPointer += n;
        if (dataPointer > tape.size()) {
            tape.resize(tape.size() * 2, 0);
        }
    }

    void backwards_n(int n) {
        dataPointer = std::max(dataPointer - n, (size_t) 0);
    }

    void increment_n(int n) {
        tape[dataPointer] += n;
    }

    void decrement_n(int n) {
        tape[dataPointer] -= n;
    }

    void input() {
        std::cin >> tape[dataPointer]; 
    }

    void print() {
        std::cout << tape[dataPointer] << std::flush;
    }

    char read() {
        return tape[dataPointer];
    }
};

class Operation {
    protected:
    size_t nextOperation = -1;
    public:
    virtual void operate(Tape& tape) {}

    size_t getNextOperation() {
        return nextOperation;
    }
};

class RightArrow : public Operation {
    int num_times;
    public:
    RightArrow(int num_times, size_t currentOperationIndex) {
        this->num_times = num_times;
        nextOperation = currentOperationIndex + 1;
    }

    void operate(Tape& tape) override {
        tape.forward_n(num_times);
    }
};

class LeftArrow : public Operation {
    int num_times;

    public:
    LeftArrow(int num_times, size_t currentOperationIndex) {
        this->num_times = num_times;
        nextOperation = currentOperationIndex + 1;
    }

    void operate(Tape& tape) override {
        tape.backwards_n(num_times);
    }
};

class PlusSign : public Operation {
    int num_times;

    public:
    PlusSign(int num_times, size_t currentOperationIndex) {
        this->num_times = num_times;
        nextOperation = currentOperationIndex + 1;
    }

    void operate(Tape& tape) override {
        tape.increment_n(num_times);
    }
};


class MinusSign : public Operation {
    int num_times;

    public:
    MinusSign(int num_times, size_t currentOperationIndex) {
        this->num_times = num_times;
        nextOperation = currentOperationIndex + 1;
    }

    void operate(Tape& tape) override {
        tape.decrement_n(num_times);
    }
};


class Comma : public Operation {
    public:
    Comma(size_t currentOperationIndex) {
        nextOperation = currentOperationIndex + 1;
    }

    void operate(Tape& tape) override {
        tape.input();
    }
};

class Period : public Operation {
    public:
    Period(size_t currentOperationIndex) {
        nextOperation = currentOperationIndex + 1;
    }

    void operate(Tape& tape) override {
        tape.print();
    }
};

class LeftBracket : public Operation {
    size_t currentOperationIndex;

    size_t rightBracketIndex = -1;
    public:
    LeftBracket(size_t currentOperationIndex) {
        this->currentOperationIndex = currentOperationIndex;
    }

    void setRightBracketIndex(size_t rightBracketIndex) {
        this->rightBracketIndex = rightBracketIndex;
    }

    void operate(Tape& tape) override {
        if (tape.read() == 0) {
            nextOperation = rightBracketIndex + 1;
        } else {
            nextOperation = currentOperationIndex + 1;
        }
    }
};

class RightBracket : public Operation {
    size_t currentOperationIndex;

    size_t leftBracketIndex;
    public:
    RightBracket(size_t currentOperationIndex) {
        this->currentOperationIndex = currentOperationIndex;
    }

    void setLeftBracketIndex(size_t leftBracketIndex) {
        this->leftBracketIndex = leftBracketIndex;
    }

    void operate(Tape& tape) {
        if (tape.read() != 0) {
            nextOperation = leftBracketIndex + 1;
        } else {
            nextOperation = currentOperationIndex + 1;
        }
    }
};

class Interpreter {
    Tape tape;
    std::vector<Operation *> operations;

    public:
    Interpreter() {}

    ~Interpreter() {
        for (Operation * operation : operations) {
            free(operation);
        }
    }

    void parseFile(std::string& filename) {
        std::ifstream file(filename);

        std::stringstream sstream;
        sstream << file.rdbuf();

        std::string program = sstream.str();

        std::vector<size_t> leftBracketStack;
        size_t programInd = 0;
        size_t programSize = program.size();
        while (programInd < programSize) {
            switch (program.at(programInd)) {
                case '+': {
                    int numPlus = 1;
                    while (program.at(programInd) == '+' && program.at(programInd + 1) == '+') {
                        numPlus++;
                        programInd++;
                    }

                    operations.push_back(new PlusSign(numPlus, operations.size()));
                } break;
                case '-': {
                    int numMinus = 1;

                    while (program.at(programInd) == '-' && program.at(programInd + 1) == '-') {
                        numMinus++;
                        programInd++;
                    }

                    operations.push_back(new MinusSign(numMinus, operations.size()));
                } break;
                case '>': {
                    int numRight = 1;

                    while (program.at(programInd) == '>' && program.at(programInd + 1) == '>') {
                        numRight++;
                        programInd++;
                    }

                    operations.push_back(new RightArrow(numRight, operations.size()));
                } break;
                case '<': {
                    int numLeft = 1;

                    while (program.at(programInd) == '<' && program.at(programInd + 1) == '<') {
                        numLeft++;
                        programInd++;
                    }

                    operations.push_back(new LeftArrow(numLeft, operations.size()));
                } break;
                case '.': {
                    operations.push_back(new Period(operations.size()));
                } break;
                case ',': {
                    operations.push_back(new Comma(operations.size()));
                } break;
                case '[': {
                    leftBracketStack.push_back(operations.size());
                    operations.push_back(new LeftBracket(operations.size()));
                } break;
                case ']': {
                    if (leftBracketStack.size() == 0) {
                        std::cerr << "Too many right brackets!" << std::endl;
                        exit(1);
                    }
                    
                    size_t correspBracketIndex = leftBracketStack.back();
                    LeftBracket * correspBracket = static_cast<LeftBracket*>(operations.at(correspBracketIndex));
                    RightBracket * newBracket = new RightBracket(operations.size());

                    correspBracket->setRightBracketIndex(operations.size());
                    newBracket->setLeftBracketIndex(correspBracketIndex);

                    leftBracketStack.pop_back();
                    operations.push_back(static_cast<Operation *>(newBracket));
                } break;
            }

            programInd++;
        }

        if (leftBracketStack.size() != 0) {
            std::cerr << "Right bracket with no matching left bracket!" << std::endl;
            exit(1);
        }
    }

    void interpret() {
        size_t operationIndex = 0;

        size_t operationSize = operations.size();
        while (operationIndex < operationSize) {
            Operation * currentOperation = operations[operationIndex];
            currentOperation->operate(tape);
            operationIndex = currentOperation->getNextOperation();
        }
    }
};

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "No filename specified!" << std::endl;
    }

    Interpreter interpreter;
    std::string filename = argv[1];
    interpreter.parseFile(filename);
    interpreter.interpret();

    return 0;
}
