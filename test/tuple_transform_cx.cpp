
// Copyright 2020 Hans Dembinski.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/mp11/tuple.hpp>
#include <boost/mp11/detail/config.hpp>

// Technically std::tuple isn't constexpr enabled in C++11, but it works with libstdc++

#if defined( BOOST_MP11_NO_CONSTEXPR ) || ( !defined( __GLIBCXX__ ) && __cplusplus < 201400L )

int main() {}

#else

#include <tuple>
#include <type_traits>

// family of test types with state
template <int N>
struct T {
    int value;
    constexpr T() : value{N} {};
    constexpr explicit T(int n) : value{n} {}
};

struct F
{
    template<int N, int M=1> constexpr T<N+M> operator()( T<N> a, T<M> b={} ) const
    {
        return T<N+M>{a.value + b.value + 1};
    }
};

int main()
{
    {
        constexpr std::tuple<T<1>, T<2>> tp;
        constexpr std::tuple<T<3>, T<4>> tp2;

        {
            constexpr std::tuple<T<2>, T<3>> r = boost::mp11::tuple_transform(
                F{}, tp
            );
            static_assert(std::get<0>(r).value == 3, "get<0>(r).value == 3" );
            static_assert(std::get<1>(r).value == 4, "get<1>(r).value == 4" );
        }

        {
            constexpr std::tuple<T<4>, T<6>> r = boost::mp11::tuple_transform(
                F{}, tp, tp2
            );
            static_assert(std::get<0>(r).value == 5, "get<0>(r).value == 5" );
            static_assert(std::get<1>(r).value == 7, "get<1>(r).value == 7" );
        }
    }

#if defined( __clang_major__ ) && __clang_major__ == 3 && __clang_minor__ < 9
// "error: default initialization of an object of const type 'const std::tuple<>' without a user-provided default constructor"
#else

    {
        constexpr std::tuple<> tp;
        constexpr std::tuple<> r = boost::mp11::tuple_transform( F{}, tp );
        (void)r;
    }

#endif
}

#endif