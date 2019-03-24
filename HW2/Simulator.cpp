#include "Simulator.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cassert>
#define _2_34(num) unsigned reg = (num>>8) & 0xF, target = num & 0xFF
#define _2_3_4(num) unsigned dest = (num>>8) & 0xF, a = (num>>4) & 0xF, b = num & 0xF
#define _3_4(num) unsigned src = (num>>4) & 0xF, dest = num & 0xF
#define _2_4(num) unsigned reg = (num>>8) & 0xF, times = num & 0xF

bool Simulator::loadMemory(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    return ifs && ifs.read(reinterpret_cast<char *>(_memory), sizeof(_memory));
}

bool Simulator::loadAssembly(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if(!ifs)
        return false;
    std::memset(_memory, 0, sizeof(_memory));
    char buffer[5]; // longest word is 4 char
    for(unsigned char pos = 0; ifs >> buffer; pos += 2) {
        for(char* temp=buffer; *temp; ++temp)
            *temp = std::tolower(*temp);
        unsigned index = std::find_if(_instructionLiteral, _instructionLiteral+13, [&](const char* i){ return std::strcmp(i, buffer) == 0;}) - _instructionLiteral;
        assert(index < 13); // Invalid instruction
        _memory[pos] = index << 4; // Store op code
        if(index == 0xc) // halt
            break;
        int a, b, c;
        ifs >> std::hex >> a >> b;
        switch(index) {
            case 4: // mv
                _memory[pos + 1] = a << 4 | b;
                break;
            case 0xa: // srl;
            case 0xb: // beq
                ifs >> c;
                _memory[pos] |= b;
                _memory[pos + 1] = c;
                break;
            case 1:
            case 2:
            case 3:
                _memory[pos] |= a;
                _memory[pos + 1] = b;
                break;
            default:
                ifs >> c;
                _memory[pos] |= a;
                _memory[pos + 1] = b << 4 | c;
        }
    }
    return true;
}

bool Simulator::storeMemory(const std::string& path) {
    std::ofstream ofs(path, std::ios::out | std::ios::binary);
    return ofs && ofs.write(reinterpret_cast<char *>(_memory), sizeof(_memory));
}

bool Simulator::simulate() {
    std::memset(_registers, 0, sizeof(_registers));
    for(_pc = 0; !(this->*_instructions[(_memory[_pc] & 0xF0) >> 4])((_memory[_pc] << 8) | _memory[_pc+1]); _pc += 2);
    return true;
}

bool Simulator::_noop(uint16_t operand) {
    return false;
}

bool Simulator::_lw(uint16_t operand) {
    _2_34(operand);
    _registers[reg] = _memory[target];
    return false;
}

bool Simulator::_lb(uint16_t operand) {
    _2_34(operand);
    _registers[reg] = target;
    return false;
}

bool Simulator::_sw(uint16_t operand) {
    _2_34(operand);
    _memory[target] = _registers[reg];
    return false;
}

bool Simulator::_mv(uint16_t operand) {
    _3_4(operand);
    _registers[dest] = _registers[src];
    return false;
}

bool Simulator::_add(uint16_t operand) {
    _2_3_4(operand);
    _registers[dest] = _registers[a] + _registers[b];
    return false;
}

bool Simulator::_addf(uint16_t operand) {
    _2_3_4(operand);
    _registers[dest] = static_cast<unsigned char>(Float(_registers[a]) + Float(_registers[b]));
    return false;
}

bool Simulator::_or(uint16_t operand) {
    _2_3_4(operand);
    _registers[dest] = _registers[a] | _registers[b];
    return false;
}

bool Simulator::_and(uint16_t operand) {
    _2_3_4(operand);
    _registers[dest] = _registers[a] & _registers[b];
    return false;
}

bool Simulator::_xor(uint16_t operand) {
    _2_3_4(operand);
    _registers[dest] = _registers[a] ^ _registers[b];
    return false;
}

bool Simulator::_srl(uint16_t operand) {
    _2_4(operand);
    times %= 8;
    _registers[reg] = _registers[reg] >> times | (_registers[reg] & ((1 << times) - 1)) << (8 - times);
    return false;
}

bool Simulator::_beq(uint16_t operand) {
    _2_34(operand);
    if(_registers[reg] == _registers[0])
        _pc = target - 2;
    return false;
}

bool Simulator::_halt(uint16_t operand) {
    return true;
}

Simulator::Instruction Simulator::_instructions[] = {
    &Simulator::_noop, &Simulator::_lw, &Simulator::_lb, &Simulator::_sw,
    &Simulator::_mv, &Simulator::_add, &Simulator::_addf, &Simulator::_or,
    &Simulator::_and, &Simulator::_xor, &Simulator::_srl, &Simulator::_beq,
    &Simulator::_halt
};

char Simulator::_instructionLiteral[][5] = {
    "noop", "lw", "lb", "sw", "mv", "add", "addf", "or", "and", "xor", "srl", "beq", "halt"
};