# Python-like print for C++ 17

A C++17 single-header library that provides a `print()` function similar to Python's, with support for multiple arguments, custom separators, and automatic formatting for containers.

## Features

- **Single Header:** Just include `pyprint.h` and you're ready to go.
- **Variadic Arguments:** Pass any number of arguments to `print()`, just like in Python.
- **Custom Parameters:** Supports `sep` (separator), `end` (end of line), and `out` (output stream) parameters, mimicking Python's `print` function.
- **Automatic Formatting:**
    - Prints standard types (e.g., `int`, `double`, `const char*`, `std::string`).
    - Prints iterable containers or container adaptors (e.g., `std::vector`, `std::list`, `std::array`, `std::queue`) in a `[item1,item2,...]` format (comma-separated, no spaces).
    - Prints `std::pair` and `std::tuple` in a `(item1,item2,...)` format (comma-separated, no spaces).
- **C++ 17:** Requires a C++17 compatible compiler (uses `if constexpr`, `std::void_t`, etc.).

## How to Use

1. Download `pyprint.h`.
2. Include it in your project: `#include "pyprint.h"`
3. Call the function using `pyprint::print(...)`.

## Usage Examples

To use custom parameters (`sep`, `end`, `out`), pass a `pyprint::params` struct as the **last argument**.

```c++
#include "pyprint.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>

int main() {
    using pyprint::print;
    using pyprint::params;

    // 1. Basic usage with multiple arguments
    print("Hello", "world", 2025);
    // Output: Hello world 2025

    // 2. Using a custom separator (sep)
    print(1, 2, 3, 4, params{.sep = ", "});
    // Output: 1, 2, 3, 4

    // 3. Using a custom end string (end)
    print("No new line here.", params{.end = " "});
    print("See?");
    // Output: No new line here. See?

    // 4. Printing a vector
    std::vector<int> v = {10, 20, 30};
    print("My vector:", v);
    // Output: My vector: [10,20,30]

    // 5. Printing a pair
    std::pair<std::string, int> p = {"Test", 123};
    print("My pair:", p);
    // Output: My pair: (Test,123)

    // 6. Printing a tuple
    auto t = std::make_tuple(1, 'a', 3.14, "tuple!");
    print("My tuple:", t);
    // Output: My tuple: (1,a,3.14,tuple!)

    // 7. Printing a map (which is an iterable of pairs)
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}};
    print("My map:", m);
    // Output: My map: [(one,1),(two,2)]

    // 8. Redirecting output to a stringstream (out)
    std::stringstream ss;
    print("This goes to a string", params{.out = ss});
    print("This goes to cout");
    // Output (to cout): This goes to cout
    // (ss now contains "This goes to a string\n")

    // 9. Combining parameters
    print("Vector", v, "and Tuple", t, params{.sep = " | ", .end = "\n---\n"});
    // Output: Vector | [10,20,30] | and Tuple | (1,a,3.14,tuple!)
    // ---

    return 0;
}
```