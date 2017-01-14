#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
#cat out.txt | python3 evaluate.py
allCount = 0
allSum = 0
convergeCount = 0
convergeSum = 0
trueLineCount = 0
trueLineSum = 0
accumulateCount = 0
accumulateSum = 0
folder = "./../images/"
experimentPrefix = ""
for input_line in sys.stdin:
  input_line = input_line.strip()
  line = input_line.split(",")
  if len(line) != 2:
    if len(line) == 3 and "xyScale" in line[0]:
      folder += line[2] + "/"
      experimentPrefix = line[1]
    continue
  times, key = input_line.split(",")
  try:
    time = float(times)
  except:
    continue
  if key == "all":
    allSum += time
    allCount +=1
  elif key == "converging":
    convergeSum += time
    convergeCount +=1
  elif key == "trueline":
    trueLineSum += time
    trueLineCount +=1
  elif key == "accumulating":
    accumulateSum += time
    accumulateCount +=1
meanAll = allSum / allCount
meanConverge = convergeSum / convergeCount
meanTrueLine = trueLineSum / trueLineCount
meanAccumulate = accumulateSum / accumulateCount
means = (meanAll,meanConverge, meanAccumulate, meanTrueLine)


ind = np.arange(4)  # the x locations for the groups
width = 0.35       # the width of the bars

fig, ax = plt.subplots()
rects = ax.bar(ind, means, width, color='r')


# add some text for labels, title and axes ticks
ax.set_ylabel('Time in ms')
ax.set_title('Mean time consumed by different parts')
ax.set_xticks(ind + width)
ax.set_xticklabels(('Total Time', 'Converging', 'Accumulating', 'True Line check'))
plt.savefig(imagePath + experimentPrefix + "eval.png")
