#ifndef  CONVERGENT_MAPPRER_H
#define CONVERGENT_MAPPRER_H
#include <tuple>
#include <vector>
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<int,int> Point;
class ConvergentMapper {
  private:

  public:
    static vector<Line> lines(vector<Point> points);
    static vector<Circle> circles(vector<Point> points);
  };
#endif 