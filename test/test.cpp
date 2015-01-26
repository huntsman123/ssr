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

// Match special character \w
TEST("'\\w' should match ' '",
    [] { return match("\\w", " "); });
TEST("'\\w' should match '\\t'",
    [] { return match("\\w", "\t"); });
TEST("'\\w' should not match 'a'",
    [] { return !match("\\w", "a"); });
TEST("'\\w*' should match '      '",
    [] { return match("\\w*", "     "); });

END_TEST();
