import sys
import pickle
import numpy as np
import evolution_strategy as es


def actionToDo(map):
    with open('{}.pickle'.format(sys.argv[2]), 'rb') as f:
        layers = pickle.load(f)
    map = es.processData(map)
    return es.predict(layers, map)


if __name__ == "__main__":
    assert len(sys.argv) > 2, '{}'.format(sys.argv)
    print(actionToDo(sys.argv[1]))
