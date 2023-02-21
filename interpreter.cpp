#include <unordered_map>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <array>
#include <cstdint>
#include <fstream>
#include <stack>
#include <set>
#include <cassert>
#include <map>

int main(int argc, char **argv)
{
    std::vector<unsigned char> tape(30000, 0);
    int matchingParentheses[100000] = {0};

    int instructionPointer = 0;
    int dataPointer = 0;

    if (argc != 2)
    {
        std::cerr << "No file specified!" << std::endl;
        return -1;
    }

    std::vector<int> leftBracketPositions;

    std::vector<char> v = {'>', '<', '+', '-', '.', ',', '[', ']'};
    robin_hood::unordered_set<char> allowedChars(v.begin(), v.end());


    std::ifstream file(argv[1]);
    std::vector<char> program;
    char c;
    while (file.get(c)) {
        if (allowedChars.count(c))
        {
            program.push_back(c);
            if (c == '[')
            {
                leftBracketPositions.push_back(program.size() - 1);
            }
            else if (c == ']')
            {
                if (leftBracketPositions.empty())
                {
                    std::cerr << "Invalid Program: Right bracket with no matching left bracket!" << std::endl;
                    return -1;
                }

                int leftPos = leftBracketPositions.back();
                leftBracketPositions.pop_back();

                int rightPos = program.size() - 1;

                matchingParentheses[leftPos] = rightPos;
                matchingParentheses[rightPos] = leftPos;
            }
        }
    }
    file.close();
    if (!leftBracketPositions.empty())
    {
        std::cerr << "Invalid Program: Left bracket with no matching right bracket!" << std::endl;
        return -1;
    }

    instructionPointer = 0;

    while (instructionPointer < program.size())
    {
        switch(program[instructionPointer]) {
            case '+':
                ++tape[dataPointer];
                break;
            case '-':
                --tape[dataPointer];
                break;
            case '>':
                ++dataPointer;
                // if (tape.size() <= dataPointer) {
                //     tape.resize(tape.size() * 2, 0);
                // }
                break;
            case '<':
                // if (dataPointer > 0) {
                    --dataPointer;
                // }
                break;
            case '.':
                std::cout << tape[dataPointer] << std::flush;
                break;
            case ',':
                char c;
                std::cin >> c;
                tape[dataPointer] = c;
                break;
            case '[':
                if (tape[dataPointer] == 0) {
                    instructionPointer = matchingParentheses[instructionPointer];
                }
                break;
            case ']':
                if (tape[dataPointer] != 0) {
                    instructionPointer = matchingParentheses[instructionPointer];
                }
                break;
        }
        ++instructionPointer;
    }

    return 0;
}
