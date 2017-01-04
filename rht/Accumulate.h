#ifndef  ACCUMULATE_H
#define ACCUMULATE_H
#include <tuple>
#include <vector>
using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
class Accumulate {
  private:
  public:
      int votes;
      Line line;
      Circle circle;
      void voteFor(Line voter);
      bool isSimilarTo(Line l);
      Accumulate(Line voter):line(voter),votes(1){};
      Accumulate(Circle voter):circle(voter),votes(1){};
  };
#endif 