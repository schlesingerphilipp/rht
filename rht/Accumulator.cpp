#include "Accumulator.h"
#include <math.h>       
#include "Accumulate.cpp"

using namespace std;
typedef tuple<int,int,float> Circle;
typedef tuple<float,float> Line;
typedef tuple<int,int> Point;
bool sortCandidates(Accumulate one, Accumulate two)
{
  return one.votes > two.votes;
}
vector< Circle > Accumulator::circles(vector< Circle > circles)
{

}
vector< Line > Accumulator::candidateLines(vector< Line > lines)
{
  vector<Accumulate> accumulator({});
  for (Line &line : lines)
  {
    bool contains = false;
    for (Accumulate &accumulate : accumulator)
    {
      if (accumulate.isSimilarTo(line))
      {
	accumulate.voteFor(line);
	contains = true;
	break;
      }
    }
    if (!contains)
    {
      accumulator.push_back(line);
    }
  }
  //Only return candidate Lines (more than 2 or 3 occurances)
  vector< Line > canditates = {};
  for (Accumulate &accumulate : accumulator)
  {
     if (accumulate.votes > 5)
     {
       canditates.push_back(accumulate.line);
     }
  }
  sort(canditates.begin(),canditates.end(),sortCandidates);
  return canditates;
}



