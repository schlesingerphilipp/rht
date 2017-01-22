#ifndef  TRANSFORMATION_H
#define TRANSFORMATION_H
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float, int, int> LineWO;
class Transformation {
  private:
  public:
    vector<LineWO> lines;
    vector<Circle> circles;
    Transformation(vector<LineWO> l, vector<Circle> c):lines(l),circles(c){};
  };
#endif