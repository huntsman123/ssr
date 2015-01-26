#include <string>
#include <vector>
#include <queue>
#include <iostream>

namespace ssr {
  enum {
    CHAR,
    ANY_CHAR,
    SPLIT,
    JUMP
  };

  struct Thread {
    unsigned int pp;
    unsigned int sp;
  };

  class Regex {
    std::vector<unsigned int> code; // Where the bytecode is stored
    std::queue<Thread> threads;    // Keep track of the threads to run

    public:
    
    Regex(std::string regex) {
      unsigned int counter = 0;
      unsigned int last_instruction = 0;

      while (counter < regex.size()) {
        if (regex[counter] == '.') {
          code.push_back(ANY_CHAR);
          last_instruction = code.size() - 1;
        }
        else if (regex[counter] == '+') {
          code.push_back(SPLIT);
          code.push_back(last_instruction);
          last_instruction = code.size() - 3;
        }
        else if (regex[counter] == '*') {
          code.insert(code.begin() + last_instruction, SPLIT);
          code.insert(code.begin() + last_instruction + 1, code.size() + 3);
          code.push_back(JUMP);
          code.push_back(last_instruction);
          last_instruction = code.size() - 2;
        }
        else {
          code.push_back(CHAR);
          code.push_back(regex[counter]);
          last_instruction = code.size() - 2;
        }
        counter++;
      }

      // for (auto n : code) std::cout << n << " ";
      // std::cout << std::endl;
    }

    bool match(std::string str) {
      unsigned int sp = 0,
                   pp = 0;
      threads.push(Thread{pp,sp});

      // Loop through each thread to find a match
loop_break: // Ugh, I know
      while (threads.size() > 0) {
        sp = threads.front().sp;
        pp = threads.front().pp;
        threads.pop();

        // While there are characters to test
        while (pp < code.size() && sp <= str.size()) {
          switch(code[pp]) {
            case CHAR:
              pp++;
              if (code[pp] == str[sp]) {
                pp++;
                sp++;
              }
              else {
                goto loop_break;
              }
              break;
            case ANY_CHAR:
              if (str[sp] > 0) {
                pp++;
                sp++;
              }
              else
                goto loop_break;
              break;
            case SPLIT:
              threads.push(Thread{pp+2,sp}); // Next thread to run if this doesn't match
              pp = code[pp+1];
              break;
            case JUMP:
              pp = code[pp+1];
              break;
          }
        }
        if (sp >= str.size() && pp >= code.size()) return true;
        else goto loop_break;
      }
      return false;
    }
  };
}
