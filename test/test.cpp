#include "../ssr.h"
#include "dryrun.h"
#include <string>

using namespace ssr;
using namespace std;

// Helper function
bool match(string regex, string test) {
  auto rgx = Regex(regex);
  return rgx.match(test);
}

BEGIN_TEST();

// No special characters
TEST("'a' should not match 'b'",
    [] { return !match("a", "b"); });
TEST("'aabca' should match 'aabca'",
    [] { return match("aabca", "aabca"); });
TEST("'a' should not match 'abc'",
    [] { return !match("a", "abc"); });
TEST("'abc' should not match 'a'",
    [] { return !match("abc", "a"); });

// Match special '.' character
TEST("'.' should match 'a'",
    [] { return match(".", "a"); });
TEST("'..' should not match 'a'",
    [] { return !match("..", "a"); });
TEST("'..' should match 'ab'",
    [] { return match("..", "ab"); });
TEST("'.v1.a' should match 'Iv1,a'",
    [] { return match(".v1.a", "Iv1,a"); });
TEST("''.v1.a' should not match 'Iv1,b'",
    [] { return !match(".v1.a", "Iv1,b"); });

// Match special character '+'
TEST("'.+' should match 'aaaaa'",
    [] { return match(".+", "aaaaa"); });
TEST("'.+' should match 'bbaab4k3'",
    [] { return match(".+","bbaab4k3"); });
TEST("'.+a' should match 'bbbbba'",
    [] { return match(".+a", "bbbbba"); });
TEST("'b+' should match 'b'",
    [] { return match("b+", "b"); });
TEST("'ba+' should not match 'b'",
    [] { return !match("ba+", "b"); });
TEST("'.+b' should not match 'bbbbba'",
    [] { return !match(".+b", "bbbbba"); });
TEST("'b+' should match 'bbbbb'",
    [] { return match("b+", "bbbbb"); });
TEST("'ba+b' should match 'baaaab'",
    [] { return match("ba+b", "baaaab"); });
TEST("'.+ba+c' should match 'df9dfjdbaaac'",
    [] { return match(".+ba+c", "df9dfjdbaaac"); });

// Match special character '*'
TEST("'a*b' should match 'aaaab'",
    [] { return match("a*b", "aaaab"); });
TEST("'a*b' should match 'b'",
    [] { return match("a*b", "b"); });
TEST("'.*' should match 'aaaaa'",
    [] { return match(".*", "aaaaa"); });
TEST("'.*' should match 'bbaab4k3'",
    [] { return match(".*","bbaab4k3"); });
TEST("'.*a' should match 'bbbbba'",
    [] { return match(".*a", "bbbbba"); });
TEST("'b*' should match 'b'",
    [] { return match("b*", "b"); });
TEST("'ba*' should match 'b'",
    [] { return match("ba*", "b"); });
TEST("'.*b' should not match 'bbbbba'",
    [] { return !match(".*b", "bbbbba"); });
TEST("'b*' should match 'bbbbb'",
    [] { return match("b*", "bbbbb"); });
TEST("'ba*b' should match 'baaaab'",
    [] { return match("ba*b", "baaaab"); });
TEST("'.*ba+c' should match 'df9dfjdbaaac'",
    [] { return match(".*ba+c", "df9dfjdbaaac"); });

// Match special character \s
TEST("'\\s' should match ' '",
    [] { return match("\\s", " "); });
TEST("'\\s' should match '\\t'",
    [] { return match("\\s", "\t"); });
TEST("'\\s' should not match 'a'",
    [] { return !match("\\s", "a"); });
TEST("'\\s*' should match '      '",
    [] { return match("\\s*", "     "); });

// Match special character \d
TEST("'\\d' should match '9'",
    [] { return match("\\d", "9"); });
TEST("'\\d+' should match '0123459876'",
    [] { return match("\\d+", "0123459876"); });
TEST("'\\d' should not match 'a'",
    [] { return !match("\\d", "a"); });

// Match special character \w
TEST("'\\w+' should match '324jdkfmv3345f__343'",
    [] { return match("\\w+", "324jdkfmv3345f__343"); });
TEST("'\\w' should not match '&'",
    [] { return !match("\\w", "&"); });

// Make sure you can escape the required characters
TEST("''\\\\' should match '\\",
    [] { return match("\\\\", "\\"); });
TEST("'\\*' should match '*'",
    [] { return match("\\*", "*"); });

// Test submatching
TEST("'aa(b+)123' should match aabbb123 and return 'bbb' for the submatch",
    [] { Regex regex("aa(b+)123"); regex.match("aabbb123"); return regex.matches()[0] == "bbb"; });
TEST("'Hello (.*)!' should match 'Hello Ryan!' and return 'Ryan' for the submatch",
    [] { Regex regex("Hello (.*)!"); regex.match("Hello Ryan!"); return regex.matches()[0] == "Ryan"; });
TEST("'(Hello) (a*)!' should match 'Hello Bob!' and return 'Hello' and '' submatches",
    [] { Regex regex("(Hello) Bob(a*)!"); regex.match("Hello Bob!");
    return regex.matches()[0] == "Hello" && regex.matches()[1] == ""; });

// Test character classes
TEST("'[abc]+' should match 'abca'",
    [] { return match("[abc]+", "abca"); });
TEST("'[abc]+' should not match 'abcz'",
    [] { return !match("[abc]+", "abcz"); });
TEST("'[A-Z]' should match 'F'",
    [] { return match("[A-Z]", "F"); });
TEST("'[A-F]' should not match 'Z'",
    [] { return !match("[A-F]", "Z"); });;
TEST("'[\\-Af]+' should match 'A--f'",
    [] { return match("[\\-Af]+", "A--f"); });
TEST("'1*[A-F]+' should match 'ABCDFFA'",
    [] { return match("1*[A-F]+", "ABCDFFA"); });

END_TEST();
