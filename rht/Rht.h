#ifndef  RHT_H
#define RHT_H
#include <vigra/impex.hxx>
#include <vigra/multi_array.hxx>
#include <future>
#include "ImagePart.h"
#include "Transformation.h"



using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float, int, int> LineWO;
typedef vigra::MultiArray<2, int > BinaryArray;

class Rht {
  private:
    static Transformation summitUp(vector<future<vector<LineWO>>> &futureLines,  vector<future<vector<Circle>>> &futureCircles, float tolleranceTheta, float tolleranceP);
  public:
    static Transformation transform(BinaryArray img, int size, int distanceThreshold, int pointsThreshold , float tolleranceTheta, float tolleranceP, int numberOfThreads);
  };
#endif 