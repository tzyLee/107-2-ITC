import os
import pickle
import multiprocessing
import numpy as np

# hyperparameters
npop = 10      # population size
sigma = 0.1    # noise standard deviation
alpha = 0.001  # learning rate
iteration = 300
nprocess = 4  # number of process
chunksize = 4

blockList = [b'#',  # Wall
             b'@',  # Other's head
             b'*',  # Snake body
             b'+',  # self.head
             b'?',  # Unknown
             b' ',  # Empty
             b'.'  # Food
             ]
blockHash = dict(enumerate(blockList))


def ReLU(array):
    return np.maximum(0, array)


class Layer:
    def __init__(self, input, output, activation):
        self.w = np.random.randn(output, input)
        self.b = np.random.randn(output)
        self.activation = activation

    def forward(self, input):
        return self.activation(self.w * input + self.b)

    def __add__(self, other):
        ret = Layer(0, 0, self.activation)
        ret.w = self.w + other
        ret.b = self.b + other
        return ret

    def __mul__(self, other):
        ret = Layer(0, 0, self.activation)
        ret.w = self.w * other
        ret.b = self.b * other
        return ret


class NN(list):
    def __init__(self, *layers):
        self = layers

    def forward(self, input):
        output = input
        for layer in self:
            output = layer.forward(input)
        return output

    def __add__(self, other):  # overload list.__add__
        if type(other) is NN:
            # assume all layer have same dimension
            return NN(list(a + b for a, b in zip(self, other)))
        else:
            return NN(list(layer + other for layer in self))

    def __mul__(self, other):
        if type(other) is NN:
            # assume all layer have same dimension
            return NN(list(a * b for a, b in zip(self, other)))
        else:
            return NN(list(layer * other for layer in self))


def processData(mapStr):
    return np.fromiter((blockHash[block] for block in mapStr), int)  # 120*40


def predict(mapStr, filename):
    input = processData(mapStr)
    with open(filename, 'rb') as f:
        nn = pickle.load(f)
    result = nn.forward(input)  # U D L R
    return np.argmax(result) + 1  # +1 for pyb0X901XXX.h::getMove
    # return np.random.choice(range(1, 5), p=result/np.sum(result)) # sample based on weight


def reward(nn):
    filename = 'data/{}'.format(id(nn))
    with open(filename, 'wb') as f:
        pickle.dump(nn, f)
    # TODO Execute c++, pass in data file name, get reward


def train():
    weights = NN(Layer(120*40, 128, ReLU),
                 Layer(128, 4, ReLU))
    for i in range(iteration):
        noises = np.fromiter((NN(Layer(120*40, 128, ReLU),
                                 Layer(128, 4, ReLU)) for i in range(npop)), dtype=np.object)
        with multiprocessing.Pool(processes=nprocess) as pool:
            rewards = pool.imap(reward, noises, chunksize)
        rewards = np.fromiter(rewards, dtype=int)
        A = (rewards - np.mean(rewards)) / np.std(rewards)  # standardize
        weights += alpha/(npop*sigma) * (noises.T @ A)
    with open('training_result.pickle', 'wb') as f:
        pickle.dump(weights, f)
