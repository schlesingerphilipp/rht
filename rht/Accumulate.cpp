#include "Accumulate.h"

using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
void Accumulate::voteFor(Line voter)
{
  float theta = (get<0>(line) * votes + get<0>(voter)) / (votes +1);
  float p = (get<1>(line) * votes + get<1>(voter)) / (votes +1);
  line = Line(theta,p);
  votes +=1;
}
bool Accumulate::isSimilarTo(Line l, float tolleranceTheta,  float tolleranceP)
{
  if (abs(get<0>(line) - get<0>(l)) < tolleranceTheta) {
    if (abs(get<1>(line) - get<1>(l)) < tolleranceP) {
      return true;
    }
  }   
  return false;
}



