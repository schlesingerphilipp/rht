#include "Epoch.h"
#include <pthread.h>
#include "ImagePart.cpp"
#include "ConvergentMapper.cpp"
#include "Accumulator.cpp"
#include "LinePart.h"

#include <math.h>       /* atan */
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<int,int> Point;
typedef tuple<int,int> Start;
typedef tuple<int,int> End;
typedef vigra::MultiArray<2, int > BinaryArray;


vector<Line> Epoch::lines(ImagePart imagePart)
{
  vector<Line> lines;
  while (true)
  {
    vector<Point> points = imagePart.selectRandomPoints();
    if (points.size() == 0)
    {
      break;
    }
    vector<Line> convergedLines = ConvergentMapper::lines(points);
    vector< Line > candidateLines = Accumulator::candidateLines(convergedLines);
    if (candidateLines.size() > 0)
    {
      bool trueLine = false;
      for (Line &line : candidateLines)
      {
	  /*tuple<vector<Point>,vector<LinePart>, int> mapping = mapPointsToLine(line, imagePart);
	  vector<LinePart>parts(get<1>(mapping));
	  if (get<2>(mapping)>3)
	  {
	    imagePart.removePoints(get<0>(mapping));
	    trueLine = true;
	    lines.push_back(line);
	  }*/
	  vector<Point> pointsToRemove({});
	  for(Point &p : imagePart.allPoints())
	  {
	    if(linePointDistance(line, p) < 3)
	    {
	      pointsToRemove.push_back(p);
	    }
	  }
	  if (pointsToRemove.size() > 20)
	  {
	    imagePart.removePoints(pointsToRemove);
	    lines.push_back(line);
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
  int yLine = round((p - x * cos(theta)) / sin(theta));
  int xLine = round((p - y * sin(theta)) / cos(theta));
  float a = abs(yLine - y);
  float b = abs(xLine - x);
  float c = sqrt((a*a)+(b*b));
  //a*b*0.5 = 0.5 * c * height
  int height = ((a * b) * 0.5) / (0.5*c);
  return abs(height);
}
