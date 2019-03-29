def encrypt(message, N, e):
    result = []
    for i in range(0, len(message), 2):
        chunk = ord(message[i]) << 8
        try:
            chunk |= ord(message[i+1])
        except IndexError:
            pass
        chunk = pow(chunk, e, N)
        result.append(chunk)
    return result


if __name__ == '__main__':
    with open('plain.txt', 'r') as f:
        message = f.read().strip()
    with open('public_key.txt', 'r') as f:
        N = int(f.readline().strip())
        e = int(f.readline().strip())
    with open('secret.txt', 'w') as f:
        for chunk in encrypt(message, N, e):
            f.write(str(chunk))
            f.write('\n')
