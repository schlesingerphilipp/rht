#include "Epoch.h"
#include <pthread.h>
#include "ImagePart.cpp"
#include "ConvergentMapper.cpp"
#include "Accumulator.cpp"
#include "LinePart.h"
#include <chrono>

#include <math.h>       /* atan */
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<int,int> Point;
typedef tuple<int,int> Start;
typedef tuple<int,int> End;
typedef vigra::MultiArray<2, int > BinaryArray;


vector<Line> Epoch::lines(ImagePart imagePart, int distanceThreshold, int pointsThreshold , float tolleranceTheta, float tolleranceP)
{
  vector<Line> lines;
  while (true)
  {
    vector<Point> points = imagePart.selectRandomPoints();
    if (points.size() == 0)
    {
      break;
    }
    auto start = std::chrono::high_resolution_clock::now();
    
    vector<Line> convergedLines = ConvergentMapper::lines(points);
    vector< Line > candidateLines = Accumulator::candidateLines(convergedLines, tolleranceTheta, tolleranceP);
    if (candidateLines.size() > 0)
    {
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> fp_ms = end - start;
      cout << fp_ms.count() << "," << "converge n accumulate" << endl;
      auto start2 = std::chrono::high_resolution_clock::now();
      bool trueLine = false;
      for (Line &line : candidateLines)
      {
	  /*tuple<vector<Point>,vector<LinePart>, int> mapping = mapPointsToLine(line, imagePart);
	  vector<LinePart>parts(get<1>(mapping));
	  if (get<2>(mapping)>30)
	  {
	    imagePart.removePoints(get<0>(mapping));
	    trueLine = true;
	    lines.push_back(line);
	  }*/
	  vector<Point> pointsToRemove({});
	  for(Point &p : imagePart.allPoints())
	  {
	    if(linePointDistance(line, p) < distanceThreshold)
	    {
	      pointsToRemove.push_back(p);
	    }
	  }
	  
	  if (pointsToRemove.size() > pointsThreshold)
	  {
	    imagePart.removePoints(pointsToRemove);
	    lines.push_back(line);
	    trueLine = true;
	  }
	  
      }
      auto end2 = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> fp_ms2 = end2 - start2;
      cout << fp_ms2.count() << "," << "check true line" << endl;
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
tuple< vector< Point >,vector<LinePart>, int > Epoch::mapPointsToLine(Line line, ImagePart &imagePart)
{
  vector<LinePart> parts({});
  vector<Point> pointsToRemove({});
  vector<Point> potentialRemove({});
  Start start;
  bool hasStart = false;
  End end;
  int tollerance = 3;
  int maximum = 10;
  int len = 0;
  int allLength = 0;
  imagePart.initIterator(line);
  while (imagePart.hasNext()) 
  {
    Point next = imagePart.next();
    int x = get<0>(next);
    int y = get<1>(next);
    len +=1;
    if (imagePart.isNotZeroAt(x,y))
    {
      tollerance = tollerance == maximum ? maximum : tollerance + 1; 
      if (!hasStart)
      {
	len = 0;
	tollerance = 3;
	start = Start(x,y);
	hasStart = true;
      }
      end = End(x,y);
      potentialRemove.push_back(Point(x,y));
      for (int xt = -2; xt < 3; xt++)
      {
	for(int yt = -2; yt < 3; yt++)
	{
	  if(imagePart.isNotZeroAt(x+xt,y+yt))
	  {
	    potentialRemove.push_back(Point(x+xt,y+yt));
	    tollerance = tollerance == maximum ? maximum : tollerance + 1;
	  }
	}
      }
    }
    else if (tollerance == 0)
    {
      if (hasStart)
      {
	LinePart part(start,end,line, len);
	parts.push_back(part);
	pointsToRemove.insert(pointsToRemove.end(), potentialRemove.begin(),potentialRemove.end());
	potentialRemove.clear();
	hasStart = false;
	allLength =  + len;
      }
    }
    else
    {
      tollerance -=1;
    }
  }
  return tuple< vector< Point >,vector<LinePart>, int > (pointsToRemove, parts, allLength);
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
  else if(theta == 90)
  {
    return abs(y - p);
  }
  const float  degToRad  = 3.14159265 / 180;
  double radT = theta*degToRad;
  double cosT = std::cos(radT);
  double sinT = std::sin(radT);
  int yLine = round((p - x * cosT) / sinT);
  int xLine = round((p - y * sinT) / cosT);
  float a = abs(yLine - y);
  float b = abs(xLine - x);
  float c = sqrt((a*a)+(b*b));
  //a*b*0.5 = 0.5 * c * height
  int height = a * b / c;
  return abs(height);
}
