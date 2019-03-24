#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <cstring>
#define _2_34(num) unsigned reg = (num>>8) & 0xF, target = num & 0xFF
#define _2_3_4(num) unsigned dest = (num>>8) & 0xF, a = (num>>4) & 0xF, b = num & 0xF
#define _3_4(num) unsigned src = (num>>4) & 0xF, dest = num & 0xF
#define _2_4(num) unsigned reg = (num>>8) & 0xF, times = num & 0xF

bool Simulator::loadMemory(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if(!ifs)
        return false;
    ifs.read(reinterpret_cast<char *>(_memory), sizeof(_memory));
    return true;
}

bool Simulator::storeMemory(const std::string& path) {
    std::ofstream ofs(path, std::ios::out | std::ios::binary);
    if(!ofs)
        return false;
    ofs.write(reinterpret_cast<char *>(_memory), sizeof(_memory));
    return true;
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