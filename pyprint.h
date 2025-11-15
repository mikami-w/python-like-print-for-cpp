//
// Created on 2025/11/15.
//

#ifndef PYPRINT_PYPRINT_H
#define PYPRINT_PYPRINT_H

#include <iostream>
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
        template<typename T, typename = void>
        struct is_plain_printable: std::false_type {};

        template<typename T>
        struct is_plain_printable<T, std::void_t<
            decltype(std::declval<std::ostream>() << std::declval<T>())
        >>: std::true_type {};

        template<typename T>
        inline constexpr bool is_plain_printable_v = is_plain_printable<T>::value;

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

        template<typename T, typename = void>
        struct is_pair: std::false_type {};

        template<typename T>
        struct is_pair<T, std::void_t<
           decltype(T::first),
           decltype(T::second)
       >>: std::true_type {};

        template<typename T>
        inline constexpr bool is_pair_v = is_pair<T>::value;

        template<typename T, typename = void>
        struct is_tuple: std::false_type {};

        template<typename T>
        struct is_tuple<T, std::void_t<
            decltype(std::tuple_size<T>::value)
        >>: std::true_type {};

        template<typename T>
        inline constexpr bool is_tuple_v = is_tuple<T>::value;
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
            auto p = std::get<sizeof...(args) - 1>(std::tuple<Ts...>(args...));

            // if constexpr (is_pair_v<T>)
            // {
            //     _print(p.first, p.second, p);
            //     return;
            // }

            if constexpr (is_pair_v<T> || is_tuple_v<T>)
            {
                p.out << '(';
                std::apply([&p](auto const&& obj) ->void { _print(obj, p); }, arg);
                p.out << ')';
            }
            else
            if constexpr (is_iterable_v<T> && !std::is_same_v<T, std::string>)
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
                    p.out << item;
                }
                p.out << ']';
            }
            else
            if constexpr (is_plain_printable_v<T>)
            {
                p.out << arg;
            }

            // Print separator or end
            if constexpr (sizeof...(args) > 1)
            {
                p.out << p.sep;
                _print(args...);
            }
            else print(std::get<0>(std::tuple<Ts...>(args...)));
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