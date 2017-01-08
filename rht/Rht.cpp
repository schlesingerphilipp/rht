#include "Rht.h"
#include <vigra/impex.hxx>
#include <pthread.h>
#include <iostream>
#include "ImagePart.h"
#include "Epoch.cpp"
#include "Transformation.h"
#include <vigra/multi_array.hxx>
#include <future>


typedef vigra::MultiArray<2, int > BinaryArray;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;

using namespace std;
Transformation Rht::transform(BinaryArray img, int xStep, int yStep)
{
  //create the convergers, and start them
  vector<future<vector<Line>>> futureLines;
  vector<future<vector<Circle>>> futureCircles;
  int x = 0;
  int origX = 0;
  while(x < img.width())
  {
    x = x + xStep > img.width() ? img.width() : x + xStep;
    int y =0;
    int origY = 0;  
    while (y < img.height())
    {
      y = y + yStep > img.height() ? img.height() : y + yStep;
      vigra::Shape2 topLeft(origX,origY);
      vigra::Shape2 bottomRight(x,y);
      int origP = img.height() - (origY + yStep); //Distance to origin of polar system to vigra origin
      tuple<int,int> orig(origX, origY);
      BinaryArray subImg(img.subarray(topLeft, bottomRight));
      ImagePart imagePart(orig,subImg, origP);
      origY = y;
      auto fLines = async(launch::async,*Epoch::lines, imagePart);
      auto fCircles = async(launch::async,*Epoch::circles, imagePart);
      futureLines.push_back(move(fLines));
      futureCircles.push_back(move(fCircles));
    }
    origX = x;
  }
  return summitUp(futureLines,futureCircles);
}
Transformation Rht::summitUp(vector<future<vector<Line>>> &futureLines,  vector<future<vector<Circle>>> &futureCircles )
{
  vector<Line> lines;
  for (auto &line : futureLines){
    vector<Line> linesFromSeperation(line.get());
    lines.insert(lines.end(), linesFromSeperation.begin(), linesFromSeperation.end());
  }
  vector<Circle> circles;
  for (auto &circle : futureCircles){
    vector<Circle> circlesFromSeperation(circle.get());
    circles.insert(circles.end(), circlesFromSeperation.begin(), circlesFromSeperation.end());
  }
  vector< Line > accumulatedLines = Accumulator::candidateLines(lines);
  return Transformation(accumulatedLines,circles);
}

