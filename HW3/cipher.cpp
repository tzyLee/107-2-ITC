#include <cstdint>
#include <fstream>
#include <string>
#include "pow.h"

template<class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
void encrypt(std::ofstream& ofs, std::string& message, T N, T e) {
    auto end = message.length() - 1;
    T temp = 0;
    for(unsigned i=0; i<end; i+=2) {
        temp = (message[i] << 8) | message[i+1];
        ofs << '\n' << pow(temp, e, N);
    }
    if (end%2 == 0) { // end+1 is odd => end is even
        temp = message[end] << 8;
        ofs << '\n' << pow(temp, e, N);
    }
    ofs.flush();
}

int main() {
    std::ifstream msg("plain.txt"), pub("public_key.txt");
    std::ofstream ofs("secret.txt");
    std::string message;
    msg.seekg(0, std::ios::end);
    message.resize(msg.tellg());
    msg.seekg(0, std::ios::beg);
    msg.read(&message[0], message.length());
    uint64_t N, e;
    pub >> N >> e;
    encrypt(ofs, message, N, e);
    return 0;
}