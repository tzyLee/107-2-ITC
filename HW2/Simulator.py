_instructions = []


class Simulator:
    def __init__(self):
        print()
        if not getattr(self, 'instructions', None):
            self.instructions = _instructions
        self.memory = None
        self.registers = [0 for i in range(16)]
        self.spc = 0

    def loadMemory(self, path):
        print("loadMemory")
        with open(path, 'rb') as file:
            self.memory = bytearray(file.read())

    def storeMemory(self, path):
        assert self.memory, "Memory is not loaded, call Simulator.loadMemory first!"
        print("storeMemory")
        with open(path, 'wb') as file:
            file.write(self.memory)

    def simulate(self):
        assert self.memory, "Memory is not loaded, call Simulator.loadMemory first!"
        print("simulate")
        self.pc = 0
        while not self.instructions[((self.memory[self.pc] & 0xF0) >> 4) - 1](self, (self.memory[self.pc] & 0xF) << 4 | self.memory[self.pc+1]):
            self.pc += 2

    # Helper functions
    def instruction(*masks):  # Concatenate 2 bytes, use argument as mask
        masks = [(mask, 4*'{:x}'.format(mask).count('0')) for mask in masks]

        def wrapper(func):
            global _instructions

            def ret(self, operand):
                return func(self, *((operand & mask) >> shift for mask, shift in masks))
            _instructions.append(ret)
            return ret
        return wrapper

    @staticmethod
    def __scale_up_to_exp(value, exp):
        old_exp = (value & 0b1110000) >> 4
        mantissa = value & 0b1111
        mantissa >>= exp - old_exp
        return (value & 1 << 7) | exp << 4 | mantissa

    @instruction(0xF00, 0xFF)
    # Extract 2nd bit, 3rd to 4th bit from max 0xFFFF int
    def laod_addr(self, reg, addr):
        self.registers[reg] = self.memory[addr]

    @instruction(0xF00, 0xFF)
    def load_const(self, reg, value):
        self.registers[reg] = value

    @instruction(0xF00, 0xFF)
    def store(self, reg, addr):
        self.memory[addr] = self.registers[reg]

    @instruction(0xF0, 0xF)
    def move(self, src, dest):
        self.memory[dest] = self.memory[src]

    @instruction(0xF00, 0xF0, 0xF)
    # Extract 2nd bit, 3rd and 4th bit from max 0xFFFF int
    def add_signed(self, dest, a, b):
        self.memory[dest] = (a + b) & 0xFF

    @instruction(0xF00, 0xF0, 0xF)
    def add_float(self, dest, a, b):
        sign_a, sign_b = (a & 1 << 7) >> 7, (b & 1 << 7) >> 7
        exp_a, exp_b = (a & 0b1110000) >> 4, (b & 0b1110000) >> 4
        man_a, man_b = a & 0b111, b & 0b111
        max_exp = max(exp_a, exp_b)
        if exp_a != max_exp:
            a = self.__scale_up_to_exp(a, max_exp)
        elif exp_b != max_exp:
            b = self.__scale_up_to_exp(a, max_exp)
        if sign_a ^ sign_b:  # Sign are different
            # TODO do exp need to be changed?
            # TODO if == 0, sign may became 1
            self.memory[dest] = (sign_a if man_a > man_b else sign_b) << 7 | max_exp | (
                sign_a - sign_b if man_a > man_b else sign_b - sign_a)
        else:
            # TODO do exp need to be changed?
            self.memory[dest] = sign_a << 7 | max_exp | (
                man_a + man_b) & 0b1111

    @instruction(0xF00, 0xF0, 0xF)
    def binary_or(self, dest, a, b):
        self.memory[dest] = self.registers[a] | self.registers[b]

    @instruction(0xF00, 0xF0, 0xF)
    def binary_and(self, dest, a, b):
        self.memory[dest] = self.registers[a] & self.registers[b]

    @instruction(0xF00, 0xF0, 0xF)
    def binary_xor(self, dest, a, b):
        self.memory[dest] = self.registers[a] ^ self.registers[b]

    @instruction(0xF00, 0xF)
    def rotate_r(self, src, times):
        times = times % 8  # rotate a 8-bit int over 8 is useless
        self.registers[src] = (self.registers[src] >>
                               times) | self.registers[src] & (1 << (times - 1))  # TODO check correctness

    @instruction(0xF00, 0xFF)
    def jeq(self, reg, value):
        if self.registers[reg] == value:
            self.pc = value

    @instruction()
    def halt(self):
        return True  # Other instructions returns None
