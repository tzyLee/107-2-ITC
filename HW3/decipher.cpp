#include <cstdint>
#include <fstream>
#include <string>
#include "pow.h"

template<class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
void decrypt(std::ofstream& ofs, std::ifstream& ifs, T N, T d) {
    T chunk = 0;
    while(ifs >> chunk) {
        chunk = pow(chunk, d, N);
        ofs << static_cast<char>(chunk >> 8) << static_cast<char>(chunk & 0xFF);
    }
    ofs.flush();
}

int main() {
    std::ifstream msg("secret.txt"), pub("private_key.txt");
    std::ofstream ofs("message.txt");
    uint64_t N, d;
    pub >> N >> d;
    decrypt(ofs, msg, N, d);
    return 0;
}