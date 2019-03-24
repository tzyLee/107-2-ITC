_instructions = []
_asm_name = ['noop', 'lw', 'lb', 'sw', 'mv', 'add', 'addf',
             'or', 'and', 'xor', 'srl', 'beq', 'halt']


class Float:
    def __init__(self, num):
        self.sign = num >> 7
        self.exp = (num & (0b1110000)) >> 4
        self.man = num & 0b1111

    def __scale_up_exp(self, b):
        if self.exp > b.exp:
            b.man >>= self.exp - b.exp
            b.exp = self.exp
        else:
            self.man >>= b.exp - self.exp
            self.exp = b.exp

    def __add__(self, b):
        self.__scale_up_exp(b)
        if self.sign == b.sign:
            sum = self.man + b.man
            exp = self.exp
            if sum > 0b1111 and exp < 0b111:
                sum >>= 1 # truncate
                exp += 1
            return self.sign << 7 | exp << 4 | sum & 0b1111 # exp == 0b111, make it overflow (normalized)
        else:
            if self.man == b.man:
                return self.exp << 4 # 0 with exponent
            else:
                return (self.sign if self.man > b.man else b.sign) << 7 | self.exp << 4 | abs(self.man - b.man)

class Simulator:
    def __init__(self):
        print()
        if not getattr(self, 'instructions', None):
            self.instructions = _instructions
        self.memory = None
        self.registers = [0 for i in range(16)]
        self.pc = 0

    def loadMemory(self, path):
        print("loadMemory")
        with open(path, 'rb') as file:
            self.memory = bytearray(file.read())

    def loadAssembly(self, path):
        print("loadAssembly")
        if not getattr(self, 'asm', None):
            self.asm = {name: index for index, name in enumerate(_asm_name)}
        self.memory = bytearray(256)
        with open(path, 'r') as file:
            for index, line in enumerate(file):
                code = line.strip().lower().split(' ')
                inst = code[0]
                self.memory[2*index] = self.asm[inst] << 4
                if inst == 'mv':
                    self.memory[2*index + 1] = int(''.join(code[1:]), 16)
                elif inst == 'srl':
                    self.memory[2*index] |= int(code[1], 16)
                    self.memory[2*index + 1] = int(code[-1], 16)
                elif inst != 'halt':  # if is halt, C000, inst is already set
                    code = ''.join(code[1:])  # skip first (instruction)
                    self.memory[2*index] |= int(code[0], 16)
                    self.memory[2*index + 1] = int(code[1:], 16)

    def storeMemory(self, path):
        assert self.memory, "Memory is not loaded, call Simulator.loadMemory first!"
        print("storeMemory")
        with open(path, 'wb') as file:
            file.write(self.memory)

    def simulate(self):
        assert self.memory, "Memory is not loaded, call Simulator.loadMemory first!"
        self.registers = [0 for i in range(16)]
        print("simulate")
        self.pc = 0
        while not self.instructions[((self.memory[self.pc] & 0xF0) >> 4)](self, (self.memory[self.pc] & 0xF) << 8 | self.memory[self.pc+1]):
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

    @instruction()
    def noop(self):
        return False
    
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
        self.registers[dest] = self.registers[src]

    @instruction(0xF00, 0xF0, 0xF)
    # Extract 2nd bit, 3rd and 4th bit from max 0xFFFF int
    def add_signed(self, dest, a, b):
        self.registers[dest] = (self.registers[a] + self.registers[b]) & 0xFF

    @instruction(0xF00, 0xF0, 0xF)
    def add_float(self, dest, a, b):
        self.registers[dest] = Float(self.registers[a]) + Float(self.registers[b])

    @instruction(0xF00, 0xF0, 0xF)
    def binary_or(self, dest, a, b):
        self.registers[dest] = self.registers[a] | self.registers[b]

    @instruction(0xF00, 0xF0, 0xF)
    def binary_and(self, dest, a, b):
        self.registers[dest] = self.registers[a] & self.registers[b]

    @instruction(0xF00, 0xF0, 0xF)
    def binary_xor(self, dest, a, b):
        self.registers[dest] = self.registers[a] ^ self.registers[b]

    @instruction(0xF00, 0xF)
    def rotate_r(self, src, times):
        times = times % 8  # rotate a 8-bit int over 8 is useless
        self.registers[src] = self.registers[src] >> times | (
            self.registers[src] & ((1 << times) - 1)) << (8 - times)

    @instruction(0xF00, 0xFF)
    def jeq(self, reg, value):
        if self.registers[reg] == self.registers[0]:
            self.pc = value - 2 # pc += 2 when instruction is executed

    @instruction()
    def halt(self):
        return True  # Other instructions returns None
