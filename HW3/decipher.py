def decipher(secret, N, d):
    result = []
    for chunk in secret:
        chunk = pow(chunk, d, N)
        result.append(chr(chunk >> 8))
        result.append(chr(chunk & 0b11111111))
    if result[-1] == chr(0):
        result.pop()
    return result

if __name__ == '__main__':
    with open('secret.txt', 'r') as f:
        secret = map(int, f.read().strip().split('\n'))
    with open('private_key.txt', 'r') as f:
        N = int(f.readline().strip())
        d = int(f.readline().strip())
    with open('message.txt', 'w') as f:
        for char in decipher(secret, N, d):
            f.write(char)