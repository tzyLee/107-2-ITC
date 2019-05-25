import os
import multiprocessing
import subprocess
import numpy as np

# hyperparameters
npop = 1      # population size
sigma = 0.1    # noise standard deviation
alpha = 0.001  # learning rate
iteration = 1
nprocess = 4  # number of process
chunksize = 4

blockList = [
    ord('x'),  # Out of bound
    ord('#'),  # Wall
    ord('@'),  # Other's head
    ord('*'),  # Snake body
    ord('+'),  # self.head
    ord('?'),  # Unknown
    ord(' '),  # Empty
    ord('.')  # Food
]
blockHash = {block: index for index, block in enumerate(blockList)}


def ReLU(array):
    return np.maximum(0, array)


class Layer:
    def __init__(self, input, output, activation):
        self.w = np.random.randn(output, input)
        self.b = np.random.randn(output)
        self.activation = activation

    def forward(self, input):
        return self.activation(self.w @ input + self.b)

    def __add__(self, other):
        ret = Layer(0, 0, self.activation)
        ret.w = self.w + other
        ret.b = self.b + other
        return ret

    def __radd__(self, other):
        return self.__add__(other)

    def __iadd__(self, other):
        np.add(self.w, other, out=self.w, casting='unsafe')
        np.add(self.b, other, out=self.b, casting='unsafe')
        return self

    def __mul__(self, other):
        ret = Layer(0, 0, self.activation)
        ret.w = self.w * other
        ret.b = self.b * other
        return ret

    def __rmul_(self, other):
        return self.__mul__(other)

    def __imul__(self, other):
        np.multiply(self.w, other, out=self.w, casting='unsafe')
        np.multiply(self.b, other, out=self.b, casting='unsafe')
        return self


class NN:
    def __init__(self, layers):
        self.layers = np.array(layers)

    def forward(self, input):
        for layer in self.layers:
            input = layer.forward(input)
        return input

    def __add__(self, other):  # overload list.__add__
        if type(other) is NN:
            # assume all layer have same dimension
            return NN(list(a + b for a, b in zip(self.layers, other.layers)))
        else:
            return NN(list(layer + other for layer in self.layers))

    def __radd__(self, other):
        return self.__add__(other)

    def __iadd__(self, other):
        if type(other) is NN:
            # assume all layer have same dimension
            for a, b in zip(self.layers, other.layers):
                a += b
        else:
            for i in self.layers:
                i += other
        return self

    def __mul__(self, other):
        if type(other) is NN:
            # assume all layer have same dimension
            return NN(list(a * b for a, b in zip(self.layers, other.layers)))
        else:
            return NN(list(layer * other for layer in self.layers))

    def __rmul__(self, other):
        return self.__mul__(other)

    def __imul__(self, other):
        if type(other) is NN:
            # assume all layer have same dimension
            for a, b in zip(self.layers, other.layers):
                a *= b
        else:
            for layer in self.layers:
                layer *= other
        return self


def processData(mapStr):
    return np.fromiter((blockHash[block] for block in mapStr), int)  # 120*40


def predict(nn, map):
    assert len(map) == 120*40
    result = nn.forward(map)  # U D L R
    ret = np.argmax(result) + 1  # +1 for pyb0X901XXX.h::getMove
    return ret
    # return np.random.choice(range(1, 5), p=result/np.sum(result)) # sample based on weight


def reward(nn):
    pipe = subprocess.Popen(
        ['./start'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    it = 0
    try:
        while not pipe.returncode:
            pipe.poll()
            mapString = pipe.stdout.readline().strip()
            if len(mapString) != 120*40:  # Snake is dead
                pipe.stdin.write(bytes('{}\n'.format(0), 'ascii'))
            else:
                map = processData(mapString)
                pipe.stdin.write(
                    bytes('{}\n'.format(predict(nn, map)), 'ascii'))
            pipe.stdin.flush()
            it += 1
        with open('test.csv', 'r') as f:
            return int(f.read())
    except:
        return 0


def train():
    weights = NN([Layer(120*40, 128, ReLU),
                  Layer(128, 4, ReLU)])
    for i in range(iteration):
        noises = np.array([NN([Layer(120*40, 128, ReLU),
                               Layer(128, 4, ReLU)]) for i in range(npop)])
        with multiprocessing.Pool(processes=nprocess) as pool:
            rewards = pool.map(reward, noises, chunksize)
            pool.close()
            pool.join()
        rewards = np.array(rewards)
        # rewards = np.fromiter(rewards, dtype=int)
        std = np.std(rewards)
        if std:
            A = (rewards - np.mean(rewards)) / np.std(rewards)  # standardize
        else:
            A = (rewards - np.mean(rewards))
        weights += alpha/(npop*sigma) * \
            np.sum([i*A for i, j in zip(noises.T, A)])  # prevent numpy warning
        print('Iteration end, max reward is', np.max(reward))
    with open('training_result.pickle', 'wb') as f:
        pickle.dump(weights, f)


if __name__ == "__main__":
    train()
