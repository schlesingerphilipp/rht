#!/usr/bin/env python
import sys
import numpy 
import matplotlib.pyplot as plt
samples = []
cNa = 0
cTl = 0
for input_line in sys.stdin:
  input_line = input_line.strip()
  times, key = input_line.split(",")
  time = float(times)
  if key == "all":
    sample = [time,cNa,cTl]
    samples.append(sample)
  elif key == "converge n accumulate":
    cNa += time
  elif key == "check true line":
    cTl += time
i = 0
for sample in samples:
  i += 1
  sumAll = sample[0]
  sumcNa = sample[1]
  sumcTl = sample[2]
meanAll = sumAll / i;
meancNa = sumsNa / i;
meancTl = sumcTl / i; 
N = 3
means = (meanAll,meancNa,meancTl)


ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

fig, ax = plt.subplots()
rects = ax.bar(ind, means, width, color='r')


# add some text for labels, title and axes ticks
ax.set_ylabel('Time in ms')
ax.set_title('Mean time consumed in total, for Converging and accumulating, Checking for true lines')
ax.set_xticks(ind + width)
ax.set_xticklabels(('Total', 'Converging n Accumulating', 'True Line check'))
plt.show()
plt.savefig('eval.png')
