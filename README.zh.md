# Python-like print for C++ 17

一个 C++17 单头文件库，提供了类似于 Python 的 `print()` 函数，支持多参数、自定义分隔符以及对容器的自动格式化。

## 特性

- **单头文件:** 只需包含 `pyprint.h` 即可使用。
- **可变参数:** 可以像 Python 一样向 `print()` 传递任意数量的参数。
- **自定义参数:** 支持 `sep` (分隔符), `end` (行尾) 和 `out` (输出流) 参数，模仿 Python 的 `print` 函数。
- **自动格式化:**
    - 打印标准类型 (例如 `int`, `double`, `const char*`, `std::string`)。
    - 以 `[item1, item2, ...]` 格式打印可迭代容器 (例如 `std::vector`, `std::list`, `std::array`)。
    - 以 `(item1, item2, ...)` 格式打印 `std::pair` 和 `std::tuple`。
- **C++ 17:** 需要支持 C++17 的编译器 (使用了 `if constexpr`, `std::void_t` 等特性)。

## 如何使用

1. 下载 `pyprint.h`。
2. 在你的项目中包含它: `#include "pyprint.h"`
3. 使用 C++17 标准进行编译 (例如 `g++ -std=c++17 main.cpp`)。
4. 通过 `pyprint::print(...)` 调用该函数。

## 使用示例

要使用自定义参数 (`sep`, `end`, `out`)，请将一个 `pyprint::params` 结构体作为**最后一个参数**传递。

```
#include "pyprint.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>

int main() {
    using pyprint::print;
    using pyprint::params;

    // 1. 基本用法，支持多参数
    print("你好", "世界", 2025);
    // 输出: 你好 世界 2025

    // 2. 使用自定义分隔符 (sep)
    print(1, 2, 3, 4, params{.sep = ", "});
    // 输出: 1, 2, 3, 4

    // 3. 使用自定义行尾字符串 (end)
    print("这里没有换行。", params{.end = " "});
    print("看到了吗？");
    // 输出: 这里没有换行。 看到了吗？

    // 4. 打印 vector
    std::vector<int> v = {10, 20, 30};
    print("我的 vector:", v);
    // 输出: 我的 vector: [10, 20, 30]

    // 5. 打印 pair
    std::pair<std::string, int> p = {"测试", 123};
    print("我的 pair:", p);
    // 输出: 我的 pair: (测试, 123)

    // 6. 打印 tuple
    auto t = std::make_tuple(1, 'a', 3.14, "tuple!");
    print("我的 tuple:", t);
    // 输出: 我的 tuple: (1, a, 3.14, tuple!)

    // 7. 打印 map (它是一个可迭代的 pair 容器)
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}};
    print("我的 map:", m);
    // 输出: 我的 map: [(one, 1), (two, 2)]

    // 8. 重定向输出到 stringstream (out)
    std::stringstream ss;
    print("这行内容进入 string", params{.out = ss});
    print("这行内容进入 cout");
    // (cout) 输出: 这行内容进入 cout
    // (ss 变量中现在包含 "这行内容进入 string\n")

    // 9. 组合使用参数
    print("Vector", v, "和 Tuple", t, params{.sep = " | ", .end = "\n---\n"});
    // 输出: Vector | [10, 20, 30] | 和 Tuple | (1, a, 3.14, tuple!)
    // ---

    return 0;
}
```