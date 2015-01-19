#include <string>
#include <iostream>

namespace ssr {
  enum {
    ANY_CHAR = 256,
    ANY_WHITESPACE,
    ANY_NON_WHITESPACE,
    ANY_DIGIT,
    ANY_NON_DIGIT,
    ANY_WORD_CHAR,
    ANY_NON_WORD_CHAR
  };

  bool sigil_match(unsigned int sigil, unsigned char input) {

    // If sigil is a regular char
    if (sigil < 256) {
      if (sigil == input) return true;
      else return false;
    }

    if (sigil < 1 || input < 1) return false; // End of input or regex

    if (sigil == ANY_CHAR) return true;

    return false;
  }
  
  bool match(std::string regex, std::string input) {
  
    auto current_r = regex.begin(), // The current character in the regex to match
    lookahead = ++regex.begin(),    // The next character in the regex after the current
    current_i = input.begin();      // The current character to match in the input string

    // Advance the regex counters
    auto advance_r = [&] (int step = 1) {
      current_r += step;
      lookahead += step;
    };

    // Advance the input counter
    auto advance_i = [&] (int step = 1) {
      current_i += step;
    };

    while (true) {

      unsigned int current_sigil;
      switch (*current_r) {
        case '.':
          current_sigil = ANY_CHAR;
          break;
        default:
          current_sigil = *current_r;
      }

      if (*lookahead == '*') {
        while (sigil_match(current_sigil, *current_i))
          advance_i();
        advance_r(2); // Next sigil
      }
      else if (sigil_match(current_sigil, *current_i)) {
        advance_r();
        advance_i();
      }
      else
        return false;
    
      // If both are consumed, it's a match
      if (current_r >= regex.end() && current_i >= input.end()) return true;

      // If the string is consumed but the regex isn't, then back it up.
      // If you back it all the to the beginning and there is still no match,
      // then it's not a match.
      if (current_i >= input.end()) {
        if (current_r > regex.begin()) advance_r(-1);
        if (current_r <= regex.begin()) return false;
      }
    }

    return false;
  }

}
