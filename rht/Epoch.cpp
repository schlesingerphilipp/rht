#include "Epoch.h"
#include <pthread.h>
#include "ImagePart.cpp"
#include "ConvergentMapper.cpp"
#include "Accumulator.cpp"
#include "LinePart.h"
#include <chrono>
#include <map>
#include <math.h>       /* atan */
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<float,float, int, int> LineWithOrigin;
typedef tuple<int,int> Point;
typedef tuple<int,int> Start;
typedef tuple<int,int> End;
typedef vigra::MultiArray<2, int > BinaryArray;

bool sortMatches(tuple<vector<Point>,Line> one,tuple<vector<Point>,Line> two)
{
  return get<0>(one).size() > get<0>(two).size();
}

/**
 * The RHT is an iterative procedure, where one Epoch is one iteration. In one Epoch candidate lines are selected,
 * and it is tested if they are true lines. When true lines are detected a further Epoch is executed, 
 * when no true line is detected the RHT is complete. 
 */
vector<LineWithOrigin> Epoch::lines(ImagePart imagePart, int distanceThreshold, int pointsThreshold , float tolleranceTheta, float tolleranceP)
{
  vector<LineWithOrigin> lines;
  while (true)
  {
    vector<Point> points = imagePart.selectRandomPoints();
    if (points.size() == 0)
    {
      break;
    }    
    vector<Line> convergedLines = ConvergentMapper::lines(points);
    vector< Line > candidateLines = Accumulator::candidateLines(convergedLines, tolleranceTheta, tolleranceP);
    if (candidateLines.size() > 0)
    {
      bool trueLine = false;
      for (Line &line : candidateLines)
      {
	  //For all remaining points, test if they support this line
	  vector<Point> pointsToRemove({});
	  for(Point &p : imagePart.allPoints())
	  {
	    if(linePointDistance(line, p) < distanceThreshold)
	    {
	      pointsToRemove.push_back(p);
	    }
	  }
	  //Is there enough evidence? If so, remove all supporting points
	  if (pointsToRemove.size() > pointsThreshold)
	  {
	    LineWithOrigin lWithO = LineWithOrigin(get<0>(line),get<1>(line), get<0>(imagePart.origin), get<1>(imagePart.origin));
	    lines.push_back(lWithO);
	    imagePart.removePoints(pointsToRemove);
	    trueLine = true;
	  }	  
      }
      if (!trueLine)
      {
	break;
      }
    }
    else
    {
      break;
    }
    
  } 
  return lines;
}
vector<Circle> Epoch::circles(ImagePart imagePart)
{
  vector<Circle> circles;
  return circles;
}

int Epoch::linePointDistance(Line line, Point point)
{
  /*
   *  |\
   *  | \ c (the line)
   * a|h/\
   *  |/__\
   * p  b 
   * 
   * p is the Point
   * h is height of c in triangle abc, or orthogonal vector to c from c to p
   */
  int x = get<0>(point);
  int y = get<1>(point);
  float theta = get<0>(line);
  float p = get<1>(line);
  if (theta == 0)
  {
    return abs(x - p);
  }
  else if(theta == 1.57)
  {
    return abs(y - p);
  }
  //const float  degToRad  = 3.14159265 / 180;
 // double radT = theta*degToRad;
  double cosT = std::cos(theta);
  double sinT = std::sin(theta);
  int yLine = (p - x * cosT) / sinT;
  int xLine = (p - y * sinT) / cosT;
  float a = abs(yLine - y);
  float b = abs(xLine - x);
  float c = sqrt((a*a)+(b*b));
  //a*b*0.5 = 0.5 * c * height
  int height = (a * b) / c;
  return abs(height);
}
