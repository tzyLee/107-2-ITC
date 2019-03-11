dic = {pattern: chr(key + 65) for key, pattern in zip(range(8), ('000000', '001111', '010011', '011100', '100110', '101001', '110101', '111010'))}

def distance(a, b):
    return sum(i != j for i, j in zip(a, b))

def decode(lst, dist=3):
    global dic
    print('Decoding:', ' '.join(lst))
    half_dist = dist//2
    ret = []
    for code in lst:
        try:
            decoded = dic[code]
        except KeyError:
            decoded = next(key for patt, key in dic.items() if distance(patt, code) <= half_dist)
              
            dic[code] = decoded
        ret.append(decoded)
    return ''.join(ret)

if __name__ == '__main__':
    print(decode(['111010', '110110']))
    print(decode(['101000', '100110', '001100']))
    print(decode(['011101', '000110', '000000', '010100']))
    print(decode(['010010', '001000', '001110', '101111', '000000', '110111', '100110']))
    print(decode(['010011', '000000', '101001', '100110']))

