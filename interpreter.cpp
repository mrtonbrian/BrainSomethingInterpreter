#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <array>
#include <cstdint>
#include <fstream>
#include <algorithm>

enum OperationType {
    OperationLeftArrow,
    OperationRightArrow,
    OperationPlusSign,
    OperationMinusSign,
    OperationComma,
    OperationPeriod,
    OperationLeftBracket,
    OperationRightBracket
};

class Interpreter {
    std::vector<OperationType> operations;
    std::vector<int> num_times;
    std::vector<size_t> corresponding_index;

    std::vector<char> tape;
    std::size_t dataPointer;

    public:
    Interpreter() {
        tape = std::vector<char>(30000, 0);
        dataPointer = 0;
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
                    while (programInd < programSize - 1 && program.at(programInd) == '+' && program.at(programInd + 1) == '+') {
                        numPlus++;
                        programInd++;
                    }

                    operations.push_back(OperationPlusSign);
                    num_times.push_back(numPlus);
                    corresponding_index.push_back(-1);
                } break;
                case '-': {
                    int numMinus = 1;

                    while (programInd < programSize - 1 && program.at(programInd) == '-' && program.at(programInd + 1) == '-') {
                        numMinus++;
                        programInd++;
                    }

                    operations.push_back(OperationMinusSign);
                    num_times.push_back(numMinus);
                    corresponding_index.push_back(-1);
                } break;
                case '>': {
                    int numRight = 1;

                    while (programInd < programSize - 1 && program.at(programInd) == '>' && program.at(programInd + 1) == '>') {
                        numRight++;
                        programInd++;
                    }

                    operations.push_back(OperationRightArrow);
                    num_times.push_back(numRight);
                    corresponding_index.push_back(-1);
                } break;
                case '<': {
                    int numLeft = 1;

                    while (programInd < programSize - 1 && program.at(programInd) == '<' && program.at(programInd + 1) == '<') {
                        numLeft++;
                        programInd++;
                    }

                    operations.push_back(OperationLeftArrow);
                    num_times.push_back(numLeft);
                    corresponding_index.push_back(-1);
                } break;
                case '.': {
                    operations.push_back(OperationPeriod);
                    num_times.push_back(1);
                    corresponding_index.push_back(-1);
                } break;
                case ',': {
                    operations.push_back(OperationComma);
                    num_times.push_back(1);
                    corresponding_index.push_back(-1);
                } break;
                case '[': {
                    leftBracketStack.push_back(operations.size());
                    operations.push_back(OperationLeftBracket);
                    num_times.push_back(1);
                    corresponding_index.push_back(-1);
                } break;
                case ']': {
                    if (leftBracketStack.size() == 0) {
                        std::cerr << "Too many right brackets!" << std::endl;
                        exit(1);
                    }
                    
                    size_t correspBracketIndex = leftBracketStack.back();
        
                    leftBracketStack.pop_back();

                    corresponding_index.push_back(correspBracketIndex);
                    corresponding_index[correspBracketIndex] = operations.size();
                    
                    operations.push_back(OperationRightBracket);
                    num_times.push_back(1);
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
            switch (operations[operationIndex]) {
            case OperationRightArrow:
                dataPointer += num_times[operationIndex];
                if (dataPointer > tape.size()) {
                    tape.resize(tape.size() * 2, 0);
                }

                break;
            case OperationLeftArrow:
                dataPointer = std::max(dataPointer - num_times[operationIndex], (size_t) 0);
                break;
            case OperationPlusSign:
                tape[dataPointer] += num_times[operationIndex];
                break;
            case OperationMinusSign:
                tape[dataPointer] -= num_times[operationIndex];
                break;
            case OperationComma:
                std::cin >> tape[dataPointer]; 
                break;
            case OperationPeriod:
                std::cout << tape[dataPointer] << std::flush;
                break;
            case OperationLeftBracket:
                if (tape[dataPointer] == 0) {
                    operationIndex = corresponding_index[operationIndex];
                }
                break;
            case OperationRightBracket:
                if (tape[dataPointer] != 0) {
                    operationIndex = corresponding_index[operationIndex];
                }
                break;
            default:
                break;
            }

            operationIndex++;
        }
    }
};

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Error: No filename specified!" << std::endl;
        return 1;
    }

    Interpreter interpreter;
    std::string filename = argv[1];
    interpreter.parseFile(filename);
    interpreter.interpret();

    return 0;
}
