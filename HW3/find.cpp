#include <cstdint>
#include <fstream>
#include <string>
#include <type_traits>

template<class T, class U, class = typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value>::type>
T extended_euclidean(T div, T prev_mod) {
    T original_mod = prev_mod, temp = 0, quo = 0;
    U prev_result = 0, result = 1;
    while(div > 1) {
        temp = prev_mod;
        quo = div/prev_mod;
        prev_mod = div%prev_mod;
        div = temp;

        temp = prev_result;
        prev_result = result - quo * prev_result;
        result = temp;
    }
    if(result < 0)
        result += original_mod;
    return result;
}

int main() {
    std::ifstream ifs("cryptan.txt");
    std::ofstream ofs("cryptan_result.txt");
    uint64_t e, phi;
    ifs >> e >> phi;
    ofs << extended_euclidean<uint64_t, int64_t>(e, phi);
    return 0;
}