#!/usr/bin/env python
import subprocess
import sys
import numpy as np
import matplotlib.pyplot as plt
import csv

def printGraphic(means, labels, ylabel, title, path):
  ind = np.arange(len(means)) 
  width = 0.35       # the width of the bars
  fig, ax = plt.subplots()
  rects = ax.bar(ind, means, width, color='r')
  ax.set_ylabel(ylabel)
  ax.set_title(title)
  ax.set_xticks(ind + width)
  ax.set_xticklabels(labels)
  plt.savefig(path)

def collectData(lines, folder, experimentPrefix):
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
  meanAll = allSum / 20 #allCount
  meanConverge = convergeSum / 20# convergeCount
  meanTrueLine = trueLineSum / 20 #trueLineCount
  meanAccumulate = accumulateSum / 20#accumulateCount
  means = (meanAll, meanConverge, meanAccumulate, meanTrueLine)
  return means


def evaluateParameters(lines, folder, experimentPrefix):
  means = collectData(lines, folder, experimentPrefix)
  path = "./../images/" + folder + "/" + experimentPrefix + "eval.png"
  labels = ('Converging', 'Accumulating', 'True Line check')
  title = 'Mean time consumed by different parts'
  ylabel = 'Time in ms'
  printGraphic(means[1:4], labels, ylabel, title, path)
  
def numberOfThreads(folder, points, smoothing):
  means = []
  labels = []
  for threads in range(4,5):
    cmd1 = ["./../build/main",  folder, "50", "50", "1", points, "15","15", str(threads), str(smoothing)]
    print(cmd1)
    p1 = subprocess.Popen(cmd1, stdout=subprocess.PIPE)
    (out1, code) = p1.communicate()
    output1 = out1.decode('utf-8')
    mean = collectData(output1.split("\n"), folder, str(threads))[0]
    means.append(mean)
    labels.append(str(threads))
  path = "./../images/" + folder + "/eval.png"
  title = 'Mean time consumed with given number of threads'
  ylabel = 'Time in ms'
  printGraphic(means, labels, ylabel, title, path)

def largeImageExperiment():
  folder = "experiment1"
  #folderCrossing = str(i) + "shortCrossing" 
  xyStep = 20
  points = 30
  for xyScale in range(1,10):
    xySteps = str(xyStep * xyScale)
    pointss = str(points +  3 * xyScale * xyScale)# + (10 * xyScale))
    #for short
    cmd1 = ["./../build/main",  folder, xySteps, xySteps, "10", pointss, "10","15"]
    print(cmd1)
    p1 = subprocess.Popen(cmd1, stdout=subprocess.PIPE)
    (out1, code) = p1.communicate()
    output1 = out1.decode('utf-8')
    f = open("../images/" +folder + "/" + xySteps + "out.csv",'wb')
    f.write(out1)
    evaluateParameters(output1.split("\n"), folder, xySteps)

if __name__ == "__main__":
  #numberOfThreads("experiment1", "40", 0.0)
  numberOfThreads("real", "30", 3.0)

