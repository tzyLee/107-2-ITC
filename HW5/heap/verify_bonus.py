from collections import Counter
import re

with open('bonus.txt', 'r') as f:
    k = int(''.join(i for i in f.readline() if i.isdigit()))
    data = re.split("[ \r\n]+", f.read())

freq = list(Counter(data).items())
freq.sort(key=lambda x: (-x[1], x[0]))
print(freq[:k])
