//
// Created on 2025/11/15.
//

#include "../pyprint.h"
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <array>
#include <bitset>
#include <stack>
#include <queue>
#include <string>

using namespace pyprint;

// Global test counter
int g_failed_tests = 0;
int g_total_tests = 0;

// Helper function to capture output
template<typename Func>
std::string capture_output(Func func) {
    std::ostringstream oss;
    func(oss);
    return oss.str();
}

// Helper function to check test result
void check_result(const std::string& result, const std::string& expected, const std::string& test_name) {
    g_total_tests++;
    if (result != expected) {
        g_failed_tests++;
        std::cout << "========================================\n";
        std::cout << "FAILED: " << test_name << "\n";
        std::cout << "Expected: " << expected;
        std::cout << "Actual:   " << result;
        std::cout << "========================================\n";
    }
}

// Test basic types
void test_basic_types() {
    // Test integers
    std::string result = capture_output([](std::ostream& os) {
        print(42, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "42\n", "integer");

    // Test strings
    result = capture_output([](std::ostream& os) {
        print("Hello", params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "Hello\n", "string");

    // Test floating point
    result = capture_output([](std::ostream& os) {
        print(3.14, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "3.14\n", "floating point");

    // Test boolean
    result = capture_output([](std::ostream& os) {
        print(true, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "1\n", "boolean");

    std::cout << "Basic types tests passed\n";
}

// Test multiple arguments
void test_multiple_args() {
    std::string result = capture_output([](std::ostream& os) {
        print(1, 2, 3, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "1 2 3\n", "multiple integers");

    result = capture_output([](std::ostream& os) {
        print("Hello", "World", params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "Hello World\n", "multiple strings");

    result = capture_output([](std::ostream& os) {
        print(1, "two", 3.0, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "1 two 3\n", "mixed types");

    std::cout << "Multiple arguments tests passed\n";
}

// Test custom separators
void test_custom_separator() {
    std::string result = capture_output([](std::ostream& os) {
        print(1, 2, 3, params{.sep=", ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "1, 2, 3\n", "comma separator");

    result = capture_output([](std::ostream& os) {
        print("a", "b", "c", params{.sep="-", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "a-b-c\n", "dash separator");

    std::cout << "Custom separator tests passed\n";
}

// Test custom end
void test_custom_end() {
    std::string result = capture_output([](std::ostream& os) {
        print(42, params{.sep=" ", .end="", .out=os, .flush=false});
    });
    check_result(result, "42", "empty end");

    result = capture_output([](std::ostream& os) {
        print("test", params{.sep=" ", .end=" END\n", .out=os, .flush=false});
    });
    check_result(result, "test END\n", "custom end");

    std::cout << "Custom end tests passed\n";
}

// Test vectors
void test_vector() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::string result = capture_output([&v](std::ostream& os) {
        print(v, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[1,2,3,4,5]\n", "vector of ints");

    std::vector<std::string> vs = {"hello", "world"};
    result = capture_output([&vs](std::ostream& os) {
        print(vs, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[hello,world]\n", "vector of strings");

    std::vector<int> empty;
    result = capture_output([&empty](std::ostream& os) {
        print(empty, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[]\n", "empty vector");

    std::cout << "Vector tests passed\n";
}

// Test lists
void test_list() {
    std::list<int> l = {10, 20, 30};
    std::string result = capture_output([&l](std::ostream& os) {
        print(l, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[10,20,30]\n", "list of ints");

    std::cout << "List tests passed\n";
}

// Test sets
void test_set() {
    std::set<int> s = {3, 1, 4, 1, 5}; // Will be sorted: 1, 3, 4, 5
    std::string result = capture_output([&s](std::ostream& os) {
        print(s, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[1,3,4,5]\n", "set of ints");

    std::cout << "Set tests passed\n";
}

// Test maps
void test_map() {
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}};
    std::string result = capture_output([&m](std::ostream& os) {
        print(m, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[(one,1),(three,3),(two,2)]\n", "map");

    std::cout << "Map tests passed\n";
}

// Test deques
void test_deque() {
    std::deque<int> d = {5, 4, 3, 2, 1};
    std::string result = capture_output([&d](std::ostream& os) {
        print(d, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[5,4,3,2,1]\n", "deque");

    std::cout << "Deque tests passed\n";
}

// Test arrays
void test_array() {
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::string result = capture_output([&arr](std::ostream& os) {
        print(arr, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[1,2,3,4,5]\n", "array");

    std::cout << "Array tests passed\n";
}

// Test pairs
void test_pair() {
    std::pair<int, std::string> p = {42, "answer"};
    std::string result = capture_output([&p](std::ostream& os) {
        print(p, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "(42,answer)\n", "pair int-string");

    std::pair<double, double> p2 = {3.14, 2.71};
    result = capture_output([&p2](std::ostream& os) {
        print(p2, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "(3.14,2.71)\n", "pair double-double");

    std::cout << "Pair tests passed\n";
}

// Test tuples
void test_tuple() {
    std::tuple<int, std::string, double> t = {1, "two", 3.0};
    std::string result = capture_output([&t](std::ostream& os) {
        print(t, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "(1,two,3)\n", "tuple of 3 elements");

    std::tuple<int> t1 = {42};
    result = capture_output([&t1](std::ostream& os) {
        print(t1, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "(42)\n", "tuple of 1 element");

    std::cout << "Tuple tests passed\n";
}

// Test bitsets
void test_bitset() {
    std::bitset<8> bs(42); // 00101010
    std::string result = capture_output([&bs](std::ostream& os) {
        print(bs, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "00101010\n", "bitset<8>");

    std::bitset<4> bs2(15); // 1111
    result = capture_output([&bs2](std::ostream& os) {
        print(bs2, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "1111\n", "bitset<4>");

    std::cout << "Bitset tests passed\n";
}

// Test stack
void test_stack() {
    std::stack<int> stk;
    stk.push(1);
    stk.push(2);
    stk.push(3);

    std::string result = capture_output([&stk](std::ostream& os) {
        print(stk, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[3,2,1]\n", "stack"); // Stack prints top to bottom

    std::cout << "Stack tests passed\n";
}

// Test queue
void test_queue() {
    std::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    std::string result = capture_output([&q](std::ostream& os) {
        print(q, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[1,2,3]\n", "queue"); // Queue prints front to back

    std::cout << "Queue tests passed\n";
}

// Test priority queue
void test_priority_queue() {
    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(1);
    pq.push(5);

    std::string result = capture_output([&pq](std::ostream& os) {
        print(pq, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[5,4,3,1,1]\n", "priority_queue"); // Priority queue prints in priority order

    std::cout << "Priority queue tests passed\n";
}

// Test nested structures
void test_nested_structures() {
    // Vector of vectors
    std::vector<std::vector<int>> vv = {{1, 2}, {3, 4}, {5, 6}};
    std::string result = capture_output([&vv](std::ostream& os) {
        print(vv, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[[1,2],[3,4],[5,6]]\n", "vector of vectors");

    // Vector of pairs
    std::vector<std::pair<int, std::string>> vp = {{1, "one"}, {2, "two"}};
    result = capture_output([&vp](std::ostream& os) {
        print(vp, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[(1,one),(2,two)]\n", "vector of pairs");

    // Tuple with vector
    std::tuple<int, std::vector<int>> tv = {42, {1, 2, 3}};
    result = capture_output([&tv](std::ostream& os) {
        print(tv, params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "(42,[1,2,3])\n", "tuple with vector");

    std::cout << "Nested structures tests passed\n";
}

// Test empty print
void test_empty_print() {
    std::string result = capture_output([](std::ostream& os) {
        print(params{.sep=" ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "\n", "empty print");

    std::cout << "Empty print tests passed\n";
}

// Test with custom separator in containers
void test_container_custom_separator() {
    std::vector<int> v = {1, 2, 3};
    std::string result = capture_output([&v](std::ostream& os) {
        print(v, params{.sep=", ", .end="\n", .out=os, .flush=false});
    });
    check_result(result, "[1,2,3]\n", "vector with custom sep (sep doesn't affect container internals)");

    std::cout << "Container custom separator tests passed\n";
}

int main() {
    std::cout << "Running pyprint tests...\n\n";

    test_basic_types();
    test_multiple_args();
    test_custom_separator();
    test_custom_end();
    test_vector();
    test_list();
    test_set();
    test_map();
    test_deque();
    test_array();
    test_pair();
    test_tuple();
    test_bitset();
    test_stack();
    test_queue();
    test_priority_queue();
    test_nested_structures();
    test_empty_print();
    test_container_custom_separator();

    std::cout << "\n========================================\n";
    std::cout << "Test Summary:\n";
    std::cout << "Total tests: " << g_total_tests << "\n";
    std::cout << "Passed: " << (g_total_tests - g_failed_tests) << "\n";
    std::cout << "Failed: " << g_failed_tests << "\n";
    std::cout << "========================================\n";

    if (g_failed_tests == 0) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << "Some tests failed!\n";
        return 1;
    }
}
