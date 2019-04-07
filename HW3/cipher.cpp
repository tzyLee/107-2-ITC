#include <cstdint>
#include <fstream>
#include <string>
#include "pow.h"

template<class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
void encrypt(std::ofstream& ofs, std::string& message, T N, T e) {
    auto end = message.length();
    T temp = 0;
    for(unsigned i=0; i<end; i+=2) {
        temp = (message[i] << 8) | message[i+1];
        ofs << pow(temp, e, N) << '\n';
    }
    ofs.flush();
}

int main() {
    std::ifstream msg("plain.txt"), pub("public_key.txt");
    std::ofstream ofs("secret.txt");
    std::string message;
    message.reserve(10000);
    std::getline(msg, message);
    uint64_t N, e;
    pub >> N >> e;
    encrypt(ofs, message, N, e);
    return 0;
}