/*
 *
 * Super Simple Regex
 * by Ryan Breen
 * Initial Release: January 27, 2015
 * Current Version: 1.0
 * Website: https://github.com/rufusroflpunch/ssr
 * 
 * Release History:
 * 01/27/2015 - Version 1.0
 *
 */

#include <string>
#include <vector>
#include <queue>
// #include <iostream>

namespace ssr {
  // The instructions for the VM
  enum {
    CHAR,
    ANY_CHAR,
    WHITESPACE,
    DIGIT,
    WORD_CHAR,
    CHAR_CLASS,
    SPLIT,
    JUMP,
    SAVE
  };

  // Each time the there is a SPLIT in the virtual machine, it creates a new thread to begin
  // evaluation later, if no match is already found.
  struct Thread {
    unsigned int pp;
    unsigned int sp;
  };

  // The main class. The magic happens here.
  class Regex {
    std::vector<unsigned int> code;      // Where the bytecode is stored
    std::queue<Thread> threads;          // Keep track of the threads to run
    std::vector<std::string> submatches; // Private variable where submatches are stored
    unsigned int num_submatches = 0;     // This is uses to keep track of the number of submatches

    public:
    
    Regex(std::string regex) {
      unsigned int counter = 0;
      unsigned int last_instruction = 0; // This variable is a pointer to the instruction from the
                                         // the last iteration of the loop. When you come
                                         // across a split, it will either jump back to
                                         // the last_instruction, or move onto the next.

      // Parse the regex in this loop and store the code
      while (counter < regex.size()) {
        // Match any single character
        if (regex[counter] == '.') {
          code.push_back(ANY_CHAR);
          last_instruction = code.size() - 1;
        }
        else if (regex[counter] == '+') {
          // Either jump back to the last instruction, or move on to the next.
          code.push_back(SPLIT);
          code.push_back(last_instruction);
          last_instruction = code.size() - 3;
        }
        else if (regex[counter] == '*') {
          /* We need to insert a SPLIT right before the last instruction
           * For example, regex "a*b":
           * {0: SPLIT 4, 1: CHAR A, 3: JUMP 0, 4: CHAR B}
           */
          code.insert(code.begin() + last_instruction, SPLIT);
          code.insert(code.begin() + last_instruction + 1, code.size() + 3);
          code.push_back(JUMP);
          code.push_back(last_instruction);
          last_instruction = code.size() - 2;
        }
        else if (regex[counter] == '\\') {
          // Escape codes begin with \, so move to the next character in the regex
          counter++;
          if (regex[counter] == 's') {
            // Match only whitespace on \s
            code.push_back(WHITESPACE);
            last_instruction = code.size() - 1;
          }
          else if (regex[counter] == 'd') {
            // Match only digits on \d
            code.push_back(DIGIT);
            last_instruction = code.size() - 1;
          }
          else if (regex[counter] == 'w') {
            // Match only word characters on \w
            code.push_back(WORD_CHAR);
            last_instruction = code.size() - 1;
          }
          else {
            // Match only a specific character
            code.push_back(CHAR);
            code.push_back(regex[counter]);
            last_instruction = code.size() - 2;
          }
        }
        else if (regex[counter] == '(' || regex[counter] == ')') {
          // Insert a SAVE instruction when we need a submatch
          code.push_back(SAVE);
          code.push_back(num_submatches++);
          last_instruction = code.size() - 2;
        }
        else if (regex[counter] == '[') {
          code.push_back(CHAR_CLASS);
          last_instruction = code.size() - 1;

          std::vector<char> chars; // This will temporarily store the chars in the class

          char current = regex[++counter]; // The first character of the class

          // Loop through the class until we close it with ]
          while (current != ']') {
            if (current == '\\') {
              // This escape code will allow us to use - or ] in the character class
              current = regex[++counter];
              chars.push_back(current);
            }
            else if (current == '-') {
              // If the character is a -, then we need to insert all of the characters
              // between the previous one and the next one
              char start = regex[counter - 1] + 1,
                   end = regex[++counter];
              while (start <= end)
                chars.push_back(start++);
            }
            else {
              // If all else fails, just insert the current character
              chars.push_back(current);
            }

            current = regex[++counter];
          }

          code.push_back(code.size() + chars.size() + 1); // The instruction after the char class
          for (auto c : chars) code.push_back(c);         // Insert all of the charaters into the code
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
      unsigned int sp = 0,                                     // Pointer to the current string character
      pp = 0;                                                  // Pointer to the current program instruction
      threads.push(Thread{pp,sp});                             // The first thread to process is the start of the string
                                                               // and the program

      // Stores all submatches. Each SAVE instruction adds to this. It basically saves the START and end END
      // string pointers of submatches
      std::vector<unsigned int> match_counter(num_submatches); 


    loop_break: // Ugh, I know. I need this to be able to break out from nested loops
                // goto loop_break; means move onto the next thread and drop the current one
      while (threads.size() > 0) {
        // Get the next thread then pop it from the queue
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
              // If it's any valid char, just move to the next one
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
            case DIGIT:
              if (str[sp] >= '0' && str[sp] <= '9') {
                pp++;
                sp++;
              }
              else
                goto loop_break;
              break;
            case WORD_CHAR:
              if (( str[sp] >= '0' && str[sp] <= '9' ) ||
                  ( str[sp] >= 'A' && str[sp] <= 'Z') ||
                  ( str[sp] >= 'a' && str[sp] <= 'z') ||
                  ( str[sp] == '_')) {
                pp++;
                sp++;
              }
              else
                goto loop_break;
              break;
            case CHAR_CLASS:
              {
                unsigned int next_ins = code[++pp];
                bool match_class = false;

                pp++;
                while (pp < next_ins) {
                  if (code[pp] == str[sp]) {
                    match_class = true;
                    break;
                  }
                  pp++;
                }

                if (match_class) {
                  pp = next_ins;
                  sp++;
                }
                else
                  goto loop_break;
                break;
              }
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
        // It's a match if we have reached the end of both the string and the regexular expression program
        if (sp >= str.size() && pp >= code.size()) {

          // For eachs submatch, get the beginning and end string pointers, insert them into the
          // submatches vector<string>, then move onto the next set.
          for (unsigned int c = 0; c < match_counter.size(); c+=2) {
            submatches.push_back(str.substr(match_counter[c], match_counter[c+1] - match_counter[c]));
          }

          return true; // Yay, we found a match
        }
        else
          goto loop_break; // No match, goto next thread
      }
      return false; // Should probably never reach here, just need this to avoid compiler warnings
    }

    // Retur the submatches
    const std::vector<std::string>& matches() {
      return submatches;
    }
  };
}
