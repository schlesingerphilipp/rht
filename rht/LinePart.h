#ifndef  LINEPART_H
#define LINEPART_H
#include <tuple>
#include <vector>
using namespace std;
typedef tuple<int,int> Start;
typedef tuple<int,int> End;
typedef tuple<float,float> Line;

class LinePart {
  private:
  public:
    Start start;
    End end;
    Line line;
    int length;
     LinePart(Start s, End e, Line l, int len):start(s),end(e), line(l), length(len){};
  };
#endif 