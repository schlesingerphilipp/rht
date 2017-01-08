#ifndef  EPOCH_H
#define EPOCH_H
#include "ImagePart.h"
#include "LinePart.h"
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<int,int> Point;
typedef vigra::MultiArray<2, int > BinaryArray;


using namespace std;
class Epoch {
  
  private:
    static int linePointDistance(Line line, Point point);
    static tuple<vector<Point>,vector<LinePart>, int> mapPointsToLine(Line line, ImagePart &imagePart);
  public:
    static vector<Line> lines(ImagePart imagePart, int distanceThreshold, int pointsThreshold , float tolleranceTheta, float tolleranceP);
    static vector<Circle> circles(ImagePart imagePart);
  };
#endif