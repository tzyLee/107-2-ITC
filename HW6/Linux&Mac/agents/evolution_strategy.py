import os
import multiprocessing
import shutil
from subprocess import PIPE, Popen
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
blockHash = {ord(block): index for index, block in enumerate(blockList)}
eye = np.eye(len(blockList))
WIDTH = 120
HEIGHT = 40
# hyperparameters
npop = 28      # population size
sigma = 0.35    # noise standard deviation
alpha = 2.5  # learning rate
iteration = 100
nprocess = 4  # number of process
chunksize = 4
layers = [(7*7 - 1)*len(blockList), 128, 4]
# -1 for the center block, which is always '@'


def ReLU(array):
    return np.maximum(0, array)


def processData(mapStr):
    # 7*7 - 1
    # assert len(mapStr) == 7*7-1, mapStr
    raw_data = np.fromiter((blockHash[block] for block in mapStr), int)
    return eye[raw_data].flatten()  # (7*7 - 1) * len(blockList)


def predict(layers, map):
    # assert len(map) == (7*7 - 1) * len(blockList)
    output = ReLU(layers[0] @ map + layers[1])
    output = ReLU(layers[2] @ output + layers[3])  # U D L R
    return np.argmax(output) + 1  # +1 for pyb0X901XXX.h::getMove
    # return np.random.choice(range(1, 5), p=result/np.sum(result)) # sample based on weight


def reward(layers):
    reward_filename = str(id(layers))
    # with open('debug.log', 'a') as f, Popen(['../start', reward_filename], stdin=PIPE, stdout=PIPE, stderr=f) as p:
    with Popen(['../start', reward_filename], stdin=PIPE, stdout=PIPE) as p:
        i = 0
        while p.returncode is None:
            # cannot use strip because ' ' may be in map
            map = ''
            map = p.stdout.read(49)[:48]
            while not map and p.poll() is None:
                map = p.stdout.read(49)[:48]
            if p.poll() is not None:
                break
            map = processData(map)
            action = predict(layers, map)
            p.stdin.write(bytes(str(action), 'ascii'))
            if p.poll() is None:
                p.stdin.flush()
    try:
        with open('result/{}'.format(reward_filename), 'r') as f:
            return int(f.read())
    except:
        with open('result/{}'.format(reward_filename), 'r') as f:
            print(f.read())
        return 0


def train():
    try:
        with open('training_result.pickle', 'rb') as f:
            weights = pickle.load(f)
    except FileNotFoundError:
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
        shutil.rmtree('result')
        os.makedirs('result')


if __name__ == "__main__":
    train()
