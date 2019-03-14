import Simulator

def main():
    sim = Simulator.Simulator()
    for i in range(1,6):
        sim.loadMemory("input/input{}".format(i))
        sim.simulate()
        sim.storeMemory("output/output{}".format(i))


if __name__ == '__main__':
    main()
