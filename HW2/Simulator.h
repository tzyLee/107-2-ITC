#ifndef __SIMULATOR__
#define __SIMULATOR__

#include <string>
#include <cstdint>
#include <cmath>
#include <type_traits>

class Simulator{
public:
    using Instruction = bool (Simulator::*)(uint16_t);

    Simulator() : _pc(0), _registers{0}, _memory{0} {}
    bool loadMemory(const std::string&);
    bool loadAssembly(const std::string&);
    bool storeMemory(const std::string&);
    bool simulate();
private:
    unsigned char _pc;
    unsigned char _registers[16];
    unsigned char _memory[256];
    static Instruction _instructions[13];
    static char _instructionLiteral[13][5];

    bool _noop(uint16_t);
    bool _lw(uint16_t);
    bool _lb(uint16_t);
    bool _sw(uint16_t);
    bool _mv(uint16_t);
    bool _add(uint16_t);
    bool _addf(uint16_t);
    bool _or(uint16_t);
    bool _and(uint16_t);
    bool _xor(uint16_t);
    bool _srl(uint16_t);
    bool _beq(uint16_t);
    bool _halt(uint16_t);

    struct Float {
        unsigned char sign : 1;
        unsigned char exp : 3;
        unsigned char man : 4;
        static void _scale_exp(Float& a, Float& b) {
            if(a.exp > b.exp) {
                b.man >>= a.exp - b.exp; // Make it truncate
                b.exp = a.exp;
            } else {
                a.man >>= b.exp - a.exp; // Make it truncate
                a.exp = b.exp;
            }
        }
        Float(char num) : sign(num >> 7), exp((num & 0x70)>> 4), man(num & 0xF) {}
        template<class F, class = typename std::is_convertible<F, Float>::type>
        Float operator+(F&& other) {
            Float result(0);
            _scale_exp(*this, other);
            result.exp = exp;
            if(sign == other.sign) {
                result.sign = sign;
                unsigned sum = man + other.man;
                if(sum > 0xF && exp < 0x7) {
                    sum >>= 1;
                    ++result.exp;
                }
                result.man = sum &= 0xF;
            } else {
                int diff = man - other.man;
                result.sign = man > other.man ? sign : other.sign;
                result.man = std::abs(diff);
            }
            return result;
        }
        explicit operator unsigned char() {
            return sign << 7 | exp << 4 | man;
        }
    };
};

#endif
