#include <type_traits>

template<class T, class U, class = typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<U>::value>::type>
T pow(T base, U exp) {
    T result = 1;
    for(; exp; exp >>= 1) {
        if(exp & 1)
            result *= base;
        base *= base;
    }
    return result;
}

template<class T, class U, class = typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<U>::value>::type>
T pow(T base, U exp, T mod) {
    T result = 1;
    for(base %= mod; exp; exp >>= 1) {
        if(exp & 1)
            result = (result*base) % mod;
        base = (base*base) % mod;
    }
    return result;
}