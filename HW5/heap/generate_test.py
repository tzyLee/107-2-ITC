import pickle
import sys
import random


def main(N, k, words):
    result = []
    ind = 0
    while N > 0:
        selected_word = words[ind]
        freq = random.randrange(0, N//2)
        N -= freq
        ind += 1
        if ind >= len(words):
            ind = 0
        for _ in range(freq):
            result.append(selected_word)
    print("Selected all words")
    random.shuffle(result)
    print("Shuffled")
    with open('input.txt', 'w') as f:
        f.write(str(k))
        f.write('\n')
        f.write(' '.join(result))
        print("Finished")


if len(sys.argv) > 2:
    with open('word.pickle', 'rb') as f:
        words = list(pickle.load(f))  # set of all word to list
    N = int(sys.argv[1])  # 0th == py filename
    k = int(sys.argv[2])
    main(N, k, words)
else:
    print("Please enter N and k, exiting")
