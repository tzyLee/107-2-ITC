def extended_euclidean(div, prev_mod):
    mod = prev_mod
    prev_result, result = 0, 1
    while div > 1: 
        div, (q, prev_mod) = prev_mod, divmod(div, prev_mod)
        result, prev_result = prev_result, result - q * prev_result
    if result < 0:
        result += mod
    return result

if __name__ == "__main__":
    with open('cryptan.txt', 'r') as f:
        e = int(f.readline().strip())
        phi = int(f.readline().strip())
    with open('cryptan_result.txt', 'w') as f:
        f.write(str(extended_euclidean(e, phi)))