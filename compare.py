#!/usr/bin/python2
from operator import eq

with open("nesdr.log") as f:
    source = [map(lambda x: int(x, 16), x.split()[1:]) for x in f.readlines()]

with open("nestest.log") as f:
    def parse(x):
        args = x[0:9] + ' '.join(map(lambda x: x.split(':')[1], x[49:74].split()))
        return map(lambda x: int(x, 16), args.split())

    dest = [parse(x) for x in f.readlines()]

for i in xrange(0, len(source)):
  if not all(map(eq, source[i], dest[i])):
    print i + 1
    print " ".join(map(hex, source[i]))
    print " ".join(map(hex, dest[i]))
    break
