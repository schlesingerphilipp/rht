#include "Rht.h"
#include <vigra/impex.hxx>
#include <pthread.h>
#include <iostream>
#include "ImagePart.h"
#include "Epoch.cpp"
#include "Transformation.h"
#include <vigra/multi_array.hxx>
#include <future>
#include "ThreadPol.h"

typedef vigra::MultiArray<2, int > BinaryArray;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float, int, int> LineWO;

using namespace std;
Transformation Rht::transform(BinaryArray img, int size, int distanceThreshold, int pointsThreshold , float tolleranceTheta, float tolleranceP, int numberOfThreads)
{
  //create the convergers, and start them
  vector<future<vector<LineWO>>> futureLines;
  vector<future<vector<Circle>>> futureCircles;
  ThreadPool pool(numberOfThreads);
  int x = 0;
  int origX = 0;
  while(x < img.width())
  {
    x = x + size > img.width() ? img.width() : x + size;
    int y =0;
    int origY = 0;  
    while (y < img.height())
    {
      y = y + size > img.height() ? img.height() : y + size;
      vigra::Shape2 topLeft(origX,origY);
      vigra::Shape2 bottomRight(x,y);
      int origP = img.height() - (origY + size); //Distance to origin of polar system to vigra origin
      tuple<int,int> orig(origX, origY);
      BinaryArray subImg(img.subarray(topLeft, bottomRight));
      ImagePart imagePart(orig,subImg, origP);
      origY = y;
      auto fLines = pool.enqueue(*Epoch::lines, imagePart, distanceThreshold, pointsThreshold, tolleranceTheta, tolleranceP);
      auto fCircles = pool.enqueue(Epoch::circles, imagePart);

      
      futureLines.push_back(move(fLines));
      futureCircles.push_back(move(fCircles));
    }
    origX = x;
  }
  return collectResults(futureLines,futureCircles, tolleranceTheta, tolleranceP);
}
Transformation Rht::collectResults(vector<future<vector<LineWO>>> &futureLines,  vector<future<vector<Circle>>> &futureCircles, float tolleranceTheta, float tolleranceP )
{
  vector<LineWO> lines;
  for (auto &line : futureLines){
    vector<LineWO> linesFromSeperation(line.get());
    lines.insert(lines.end(), linesFromSeperation.begin(), linesFromSeperation.end());
  }
  vector<Circle> circles;
  for (auto &circle : futureCircles){
    vector<Circle> circlesFromSeperation(circle.get());
    circles.insert(circles.end(), circlesFromSeperation.begin(), circlesFromSeperation.end());
  }
  return Transformation(lines,circles);
}

