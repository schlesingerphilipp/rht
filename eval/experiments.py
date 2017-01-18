#!/usr/bin/env python
import subprocess
import sys
import numpy as np
import matplotlib.pyplot as plt
import csv


def evaluate(lines, folder, experimentPrefix):
  allCount = 0
  allSum = 0
  convergeCount = 0
  convergeSum = 0
  trueLineCount = 0
  trueLineSum = 0
  accumulateCount = 0
  accumulateSum = 0
  for input_line in lines:
    input_line = input_line.strip()
    line = input_line.split(",")
    if len(line) != 2:
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
  plt.savefig("./../images/" + folder + experimentPrefix + "eval.png")

#i = 1
#while i < 11:
def largeImageExperiment():
  folder = "experiment1"
  #folderCrossing = str(i) + "shortCrossing" 
  xyStep = 100
  points = 33
  for xyScale in range(1,5):
    xySteps = str(xyStep * xyScale)
    pointss = str(points * xyScale)
    #for short
    cmd1 = ["./../build/main",  folder, xySteps, xySteps, "5", pointss, "15","15"]
    print(cmd1)
    p1 = subprocess.Popen(cmd1, stdout=subprocess.PIPE)
    (out1, code) = p1.communicate()
    output1 = out1.decode('utf-8')
    f = open("../images/" +folder + "/" + xySteps + "out.csv",'wb')
    f.write(out1)
    evaluate(output1.split("\n"), folder, xySteps)
    #for shortCrossing
    #cmd2 = ["./../build/main",  folderCrossing, xySteps, xySteps, "5", pointss, "5","5"]
    #p2 = subprocess.Popen(cmd2, stdout=subprocess.PIPE)
    #(out2, code) = p2.communicate()
    #outPut2 = out2.decode('utf-8')
    #f = open("../images/" +folderCrossing + "/" + xySteps + "out.csv",'wb')
    #f.write(out2)
    #evaluate(output2.split("\n"), folderCrossing, xySteps)
  #i +=1
largeImageExperiment()

