#include "ConvergentMapper.h"
#include <math.h>       
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<int,int> Point;

vector< Circle > ConvergentMapper::circles(vector< Point > points)
{

}
vector< Line > ConvergentMapper::lines(vector< Point > points)
{
  vector<Line> lines;
  const float radToDeg  = 180 / 3.14159265;
  for (int i = 0; i < points.size() -1; i +=2) {
    Point one(points[i]);
    Point two(points[i+1]);
    int x1 = get<0>(one);
    int x2 = get<0>(two);
    int y1 = get<1>(one);
    int y2 = get<1>(two);
    float theta;
    float p;
    if (y1 -y2 == 0)
    {
      theta = 90;
    }
    else
    {
      theta = atan((abs(x1-x2)/abs(y1 - y2))) * radToDeg;
    }
    if (x1 - x2 == 0)
    {
      p = x1; //vertical line going through y1 and y2
    }
    else
    {
       p = abs(0.5 * (((x2 - x1) * (y1 + y2) - (y2 - y1) * (x1 + x2)) / sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))));
    }
    /*if (p == 0) 
    {
      continue;//TODO: Infinitly many posible lines
    }*/
    Line line(theta,p);
    lines.push_back(line);
  }
  return lines;
}
