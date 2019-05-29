import os
import multiprocessing
import subprocess
import pickle
import numpy as np

# constants
blockList = [
    'x',  # Out of bound
    '#',  # Wall
    '@',  # Other's head
    '*',  # Snake body
    ' ',  # Empty
    '.'  # Food
]
blockHash = {block: index for index, block in enumerate(blockList)}
eye = np.eye(len(blockList))
WIDTH = 120
HEIGHT = 40
# hyperparameters
npop = 20      # population size
sigma = 0.1    # noise standard deviation
alpha = 1.0  # learning rate
iteration = 10
nprocess = 4  # number of process
chunksize = 4
layers = [(7*7 - 1)*len(blockList), 128, 4]
# -1 for the center block, which is always '@'


def ReLU(array):
    return np.maximum(0, array)


def processData(mapStr):
    # 7*7 - 1
    raw_data = np.fromiter((blockHash[block] for block in mapStr), int)
    return eye[raw_data].flatten()  # (7*7 - 1) * len(blockList)


def predict(layers, map):
    output = ReLU(layers[0] @ map + layers[1])
    output = ReLU(layers[2] @ output + layers[3])  # U D L R
    return np.argmax(output) + 1  # +1 for pyb0X901XXX.h::getMove
    # return np.random.choice(range(1, 5), p=result/np.sum(result)) # sample based on weight


def reward(layers):
    with open('{}.pickle'.format(id(layers)), 'wb') as f:
        pickle.dump(layers, f)
    ret_code = subprocess.call(['../start', str(id(layers))])
    assert ret_code == 0
    with open("{}".format(id(layers)), 'r') as f:
        return int(f.read())
    return 0


def train():
    weights = np.array([np.random.randn(layers[1], layers[0]), np.random.randn(layers[1]),
                        np.random.randn(layers[2], layers[1]), np.random.randn(layers[2])])
    try:
        for i in range(iteration):
            noises = np.array([[np.random.randn(layers[1], layers[0]), np.random.randn(layers[1]),
                                np.random.randn(layers[2], layers[1]), np.random.randn(layers[2])] for i in range(npop)])  # shape == (npop, len(layers))
            with multiprocessing.Pool(processes=nprocess) as pool:
                rewards = pool.map(reward, weights + noises, chunksize)
                pool.close()
                pool.join()
            rewards = np.array(rewards)
            A = (rewards - np.mean(rewards))
            std = np.std(rewards)
            if std:  # if std != 0
                A /= std  # standardize
            for i in range(4):
                delta = alpha/(npop*sigma) * (noises[:, i] * A)
                weights[i] += delta[0]
            print('Iteration end, max reward is', np.max(rewards))
            if i % 10 == 0:
                with open('training_result.pickle', 'wb') as f:
                    pickle.dump(weights, f)
    finally:
        with open('training_result.pickle', 'wb') as f:
            pickle.dump(weights, f)


if __name__ == "__main__":
    train()
