#ifndef  TRANSFORMATION_H
#define TRANSFORMATION_H
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
class Transformation {
  private:
  public:
    vector<Line> lines;
    vector<Circle> circles;
    Transformation(vector<Line> l, vector<Circle> c):lines(l),circles(c){};
  };
#endif