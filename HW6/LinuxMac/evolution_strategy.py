import os
import multiprocessing
from subprocess import Popen, PIPE
import pickle
import numpy as np

# hyperparameters
npop = 2      # population size
sigma = 0.1    # noise standard deviation
alpha = 0.001  # learning rate
iteration = 1
nprocess = 4  # number of process
chunksize = 4
layers = [120*40, 128, 4]

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


def processData(mapStr):
    return np.fromiter((blockHash[block] for block in mapStr), int)  # 120*40


def predict(layers, map):
    assert len(map) == 120*40
    output = ReLU(layers[0] @ map + layers[1])
    output = ReLU(layers[2] @ output + layers[3])  # U D L R
    ret = np.argmax(output) + 1  # +1 for pyb0X901XXX.h::getMove
    return ret
    # return np.random.choice(range(1, 5), p=result/np.sum(result)) # sample based on weight


def reward(layers):
    pipe = Popen(['./start'], stdout=PIPE, stdin=PIPE)
    while pipe.returncode is None:
        pipe.poll()
        pipe.stdin.write(bytes('0\n', 'ascii'))
        mapString = pipe.stdout.readline().strip()
        if len(mapString) != 120*40:  # Snake is dead (Maybe)
            pipe.stdin.write(bytes('0\n', 'ascii'))
        else:
            map = processData(mapString)
            pipe.stdin.write(
                bytes('{}\n'.format(predict(layers, map)), 'ascii'))
        pipe.poll()
        if pipe.returncode is None:
            pipe.stdin.flush()
    if pipe.returncode == 0:
        with open('test.csv', 'r') as f:
            return int(f.read())
    print('Error')
    return 0


def train():
    weights = np.abs(np.array([np.random.randn(layers[1], layers[0]), np.random.randn(layers[1]),
                               np.random.randn(layers[2], layers[1]), np.random.randn(layers[2])]))
    for i in range(iteration):
        noises = np.array([[np.random.randn(layers[1], layers[0]), np.random.randn(layers[1]),
                            np.random.randn(layers[2], layers[1]), np.random.randn(layers[2])] for i in range(npop)])  # shape == (npop, len(layers))
        with multiprocessing.Pool(processes=nprocess) as pool:
            rewards = pool.map(reward, noises, chunksize)
            pool.close()
            pool.join()
        rewards = np.array(rewards)
        std = np.std(rewards)
        if std:  # if std != 0
            A = (rewards - np.mean(rewards)) / std  # standardize
        else:
            A = (rewards - np.mean(rewards))
        for i in range(4):
            delta = alpha/(npop*sigma) * (noises.T[i, :] * A)
            weights[i] += delta[0]
        print('Iteration end, max reward is', np.max(rewards))
    with open('training_result.pickle', 'wb') as f:
        pickle.dump(weights, f)


if __name__ == "__main__":
    train()
