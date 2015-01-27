#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>

namespace ssr {
  enum {
    CHAR,
    ANY_CHAR,
    WHITESPACE,
    SPLIT,
    JUMP,
    SAVE
  };

  struct Thread {
    unsigned int pp;
    unsigned int sp;
  };

  class Regex {
    std::vector<unsigned int> code;      // Where the bytecode is stored
    std::queue<Thread> threads;          // Keep track of the threads to run
    std::vector<std::string> submatches; // Private variable where submatches are stored
    unsigned int num_submatches = 0;

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
        else if (regex[counter] == '\\') {
          counter++;
          if (regex[counter] == 'w') {
            code.push_back(WHITESPACE);
            last_instruction = code.size() - 1;
          }
          else {
            code.push_back(CHAR);
            code.push_back(regex[counter]);
            last_instruction = code.size() - 2;
          }
        }
        else if (regex[counter] == '(' || regex[counter] == ')') {
          code.push_back(SAVE);
          code.push_back(num_submatches++);
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
      std::vector<unsigned int> match_counter(num_submatches);

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
            case WHITESPACE:
              if (str[sp] == ' ' || str[sp] == '\t') {
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
            case SAVE:
              match_counter[code[++pp]] = sp;
              pp++;
              break;
          }
        }
        if (sp >= str.size() && pp >= code.size()) {
          for (unsigned int c = 0; c < match_counter.size(); c+=2) {
            submatches.push_back(str.substr(match_counter[c], match_counter[c+1] - match_counter[c]));
          }
          return true;
        }
        else goto loop_break;
      }
      return false;
    }

    const std::vector<std::string>& matches() {
      return submatches;
    }
  };
}
