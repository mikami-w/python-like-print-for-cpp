//
// Created on 2025/11/15.
//

#ifndef PYPRINT_PYPRINT_H
#define PYPRINT_PYPRINT_H

#include <bitset>
#include <iostream>
#include <queue>
#include <stack>
#include <tuple>


namespace pyprint
{
    struct params
    {
        char const* sep = " ";
        char const* end = "\n";
        std::ostream& out = std::cout;
        bool flush = false;
    };

    namespace traits
    {
        // Check if T can be printed directly to ostream
        template<typename T, typename = void>
        struct is_plain_printable: std::false_type {};

        template<typename T>
        struct is_plain_printable<T, std::void_t<
            decltype(std::declval<std::ostream>() << std::declval<T>())
        >>: std::true_type {};

        template<typename T>
        inline constexpr bool is_plain_printable_v = is_plain_printable<T>::value;

        // Check if T is iterable (has begin() and end())
        template<typename T, typename = void>
        struct is_iterable: std::false_type {};

        template<typename T>
        struct is_iterable<T, std::void_t<
           decltype(std::begin(std::declval<T&>())),
           decltype(std::end(std::declval<T&>())),
           typename std::iterator_traits<decltype(std::begin(std::declval<T&>()))>::iterator_category,
           typename std::iterator_traits<decltype(std::end(std::declval<T&>()))>::iterator_category
       >>: std::true_type {};

        template<typename T>
        inline constexpr bool is_iterable_v = is_iterable<T>::value;

        // Check if T is std::pair
        template<typename T>
        struct is_pair: std::false_type {};

        template<typename T, typename U>
        struct is_pair<std::pair<T, U>>: std::true_type {};

        template<typename T>
        inline constexpr bool is_pair_v = is_pair<T>::value;

        // Check if T is std::array
        template<typename T>
        struct is_std_array: std::false_type {};

        template<typename T, size_t N>
        struct is_std_array<std::array<T, N>>: std::true_type {};

        template<typename T>
        inline constexpr bool is_std_array_v = is_std_array<T>::value;

        // Check if T is std::tuple (excluding std::pair and std::array)
        template<typename T, typename = void>
        struct is_tuple: std::false_type {};

        template<typename T>
        struct is_tuple<T, std::void_t<
            decltype(std::tuple_size<T>::value),
            std::enable_if_t<!is_std_array_v<T>>,
            std::enable_if_t<!is_pair_v<T>>
        >>: std::true_type {};

        template<typename T>
        inline constexpr bool is_tuple_v = is_tuple<T>::value;

        // Check if T is std::bitset
        template<typename T>
        struct is_bitset: std::false_type {};

        template<size_t N>
        struct is_bitset<std::bitset<N>>: std::true_type {};

        template<typename T>
        inline constexpr bool is_bitset_v = is_bitset<T>::value;

        // Check if T is a container adapter (stack, queue, priority_queue)
        template<typename T>
        struct is_container_adapter : std::false_type {};

        template<typename T, typename Container>
        struct is_container_adapter<std::stack<T, Container>> : std::true_type {};

        template<typename T, typename Container>
        struct is_container_adapter<std::queue<T, Container>> : std::true_type {};

        template<typename T, typename Container, typename Compare>
        struct is_container_adapter<std::priority_queue<T, Container, Compare>> : std::true_type {};

        template<typename T>
        inline constexpr bool is_container_adapter_v = is_container_adapter<T>::value;

        // Check if T is std::queue
        template<typename T>
        struct is_std_queue: std::false_type {};

        template<typename T, typename Container>
        struct is_std_queue<std::queue<T, Container>>: std::true_type {};

        template<typename T>
        inline constexpr bool is_std_queue_v = is_std_queue<T>::value;
    }

    inline void print(params const& p = {})
    {
        p.out << p.end;
    }

    namespace details
    {
        using namespace traits;
        // Ensure that params is passed as the last argument

        template <typename T, typename... Ts>
        void _print(T const& arg, Ts const&... args)
        {
            auto const& p = std::get<sizeof...(args) - 1>(std::make_tuple(args...));

            // Iterables except string
            if constexpr (is_iterable_v<T> && !std::is_convertible_v<T, std::string>)
            {
                p.out << '[';
                bool first = true;
                for (const auto& item : arg)
                {
                    if (!first)
                    {
                        p.out << p.sep;
                    }
                    first = false;
                    _print(item, p);
                }
                p.out << ']';
            }
            else // Pair and tuple
            if constexpr (is_pair_v<T> || is_tuple_v<T>)
            {
                p.out << '(';
                std::apply([&p](auto const&& obj) ->void { _print(obj, p); }, arg);
                p.out << ')';
            }
            else // Bitset
            if constexpr (is_bitset_v<T>)
            {
                p.out << arg.to_string();
            }
            else // Container adapters
            if constexpr (is_container_adapter_v<T>)
            {
                T copy = arg; // Make a copy to preserve the original
                p.out << '[';
                bool first = true;
                while (!copy.empty())
                {
                    if (!first)
                    {
                        _print(p.sep, p);
                    }
                    first = false;
                    if constexpr (is_std_queue_v<T>)
                    { // Queue uses front()
                        _print(copy.front(), p);
                    }
                    else
                    { // Stack and priority_queue use top()
                        _print(copy.top(), p);
                    }
                    copy.pop();
                }
                p.out << ']';
            }
            else // Plain printable, the recursion ends here
            if constexpr (is_plain_printable_v<T>)
            {
                p.out << arg;
            }
            else static_assert(false, "Object is not printable.");

            // Print separator or end
            if constexpr (sizeof...(args) > 1) // NOLINT(*-misleading-indentation)
            {
                _print(p.sep, p);
                _print(args...);
            }
            else
            {
                static_assert(std::is_same_v<std::tuple_element_t<0, std::tuple<Ts...>>, params>,
                    "Last argument must be params, but it is not. Why?");
                print(std::get<0>(std::make_tuple(args...)));
            }
        }

    }

    template <typename T, typename... Ts>
    void print(T const& arg, Ts const&... args)
    {
        if constexpr (sizeof...(args) > 0)
        {
            // Check if the last argument is params
            if constexpr (std::is_same_v<std::tuple_element_t<sizeof...(args) - 1, std::tuple<Ts...>>, params>)
            {
                details::_print(arg, args...);
            }
            else details::_print(arg, args..., params());
        }
        else details::_print(arg, params());
    }

}

#endif //PYPRINT_PYPRINT_H