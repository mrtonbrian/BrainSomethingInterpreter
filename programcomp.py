# Simply measures the composition of the file (how many occurrences of each character, sorted)

import sys
from collections import Counter
fname = sys.argv[1]

with open(fname, 'r') as f:
    res = dict(Counter(f.read()))
total_sum = 0
for x in res:
    total_sum += res[x]
for x in res:
    res[x] = res[x] / total_sum

kvs = []
for x in res:
    kvs.append((x, res[x]))
kvs.sort(key = lambda x: x[1], reverse=True)
for kv in kvs:
    print(kv[0], kv[1])