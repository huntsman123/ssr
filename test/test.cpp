#include "../ssr.h"
#include "dryrun.h"

using namespace ssr;

BEGIN_TEST();

TEST("Any Char (sigil)",
    [] { return sigil_match(ssr::ANY_CHAR, 'a'); });
TEST("'.' matches 'a'",
    [] { return match(".", "a"); });
TEST("'a' does not match 'b'",
    [] { return !match("a", "b"); });
TEST("'..' does not match 'a'",
    [] { return !match("..", "a"); });
TEST("'..' matches 'ab'",
    [] { return match("..", "ab"); });
TEST("'.*' matches 'aaaaa'",
    [] { return match(".*", "aaaaa"); });

END_TEST();
