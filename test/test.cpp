#include "../ssr.h"
#include "dryrun.h"

using namespace ssr;

BEGIN_TEST();

// Test sigil_match()
TEST("Any Char (sigil)",
    [] { return sigil_match(ssr::ANY_CHAR, 'a'); });
TEST("Any Whitespace (sigil) #1",
    [] { return sigil_match(ssr::ANY_WHITESPACE, ' '); });
TEST("Any Whitespace (sigil) #2",
    [] { return sigil_match(ssr::ANY_WHITESPACE, '\t'); });
TEST("Any Whitespace (sigil) #3",
    [] { return !sigil_match(ssr::ANY_WHITESPACE, 'f'); });

// Test match()
TEST("'.' should match 'a'",
    [] { return match(".", "a"); });
TEST("'a' should not match 'b'",
    [] { return !match("a", "b"); });
TEST("'..' should not match 'a'",
    [] { return !match("..", "a"); });
TEST("'..' should match 'ab'",
    [] { return match("..", "ab"); });
TEST("'.*' should match 'aaaaa'",
    [] { return match(".*", "aaaaa"); });
TEST("'.*' should match 'bbaab4k3'",
    [] { return match(".*","bbaab4k3"); });
TEST("'.*a' should match 'bbbbba'",
    [] { return match(".*a", "bbbbba"); });
TEST("'.*b' should not match 'bbbbba'",
    [] { return !match(".*b", "bbbbba"); });
TEST("'\\w' should match ' '",
    [] { return match("\\w", " "); });
TEST("'\\w' should match '\\t'",
    [] { return match("\\w", "\t"); });
TEST("'\\w' should not match 'a'",
    [] { return !match("\\w", "a"); });
TEST("'\\w*' should match '      '",
    [] { return match("\\w*", "     "); });

END_TEST();
