#ifndef  ACCUMULATOR_H
#define ACCUMULATOR_H
#include <tuple>
#include <vector>
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<int,int> Point;
class Accumulator {
  private:
    static bool compareLines(Line one, Line two);
    static bool compareCircles(Circle one, Circle two);
  public:
    static vector<Line> candidateLines(vector<Line> lines);
    static vector<Circle> circles(vector<Circle> circles);
  };
#endif 