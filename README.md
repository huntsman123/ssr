# Super Simple Regex
## Regular Expressions for C++

Regular expression support in C++ across compilers at this time is quite spotty. Even in compilers that have good C++11 support otherwise, regex might be only partially operable. SSR (Super Simple Regex) is a small, header-only regex implementation that is written in standard, cross platform C++.

## Usage

Just put ssr.h in your project and include it.

```cpp
#include "ssr.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
  string phone = "(502) 817-6990";
  string matcher = "\\((\\d\\d\\d)\\) (\\d\\d\\d)-(\\d\\d\\d\\d)";
  
  // Create and compile the regular expression
  ssr::Regex regex(matcher);

  // Test the match, should return true
  if (regex.match(phone)) {
    // Should print "5028176990"
    for (auto m : regex.matches()) cout << m;
  }
}
```

## API

`ssr::Regex` This is the primary object for the library. Create one, using the regex string as the constructor argument. This compiles the regex into bytecode for faster interpretation.

`ssr::Regex::match(std::string)` This tests string argument against the compiled regular expression and returns true if it matches. It **must** match the entire string. Any submatches are stored internally and can be accessed with the `matches()` member function.

`ssr::Regex::matches()` Returns a `const std::vector<std::string>&` containing all of the submatches from the previous match.

## Supported Features

* `.` Special character that matches against ANY character.
* `*` Wildcard that matches zero or more of the previous character.
* `+` Wildcard that matches one or more of the previous character.
* `\d` Special escape sequence that matches any digit.
* `\w` Special escape sequence that matches any digit, letter or underscore.
* `\s` will match against whitespace (space and tab characters).
* Generic escape sequences, like '\\(', to allow you to use special characters.
* You can return get submatches within the string by surrounding them in parenthese. I.E., `(\d+)abc` matched against `1234abc` will return the submatch `1234`.

## License

This is free to use in the public domain. While obviously not required, credit at least would be appreciated. Or just drop me a line so that I know someone cares. ;-)

## Release History

**01/31/2015** `1.1` Support for character classes, i.e. [A-Z123]
**01/27/2015** `1.0` Initial release. Support for `.`, `*`, `+`, `\d`, `\w`, `\s`, submatches using `()`.
