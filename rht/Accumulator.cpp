#include "Accumulator.h"
#include <math.h>       
#include "Accumulate.cpp"
#include <vector>
#include <chrono>


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
/**
 * This method returns the candidate lines
 * This method accumulates similar lines in an accumulate. The accumulate is the average of all lines of this accumulate.
 * The accumulate has as many votes as it has lines. When an accumulate has more than 2 or three votes, it will be 
 * a candidate line.
 */
vector< Line > Accumulator::candidateLines(vector< Line > lines, float tolleranceTheta, float tolleranceP)
{
  vector<Accumulate> accumulator({});
  for (Line &line : lines)
  {
    bool contains = false;
    for (Accumulate &accumulate : accumulator)
    {
      if (accumulate.isSimilarTo(line, tolleranceTheta, tolleranceP))
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
  //In the reference paper: Only return candidate Lines (more than 2 or 3 occurances)
  //Here there are less points, due to splitting the image, and like that less lines in every part, 
  //and it is neccessary to check each converged line
  vector< Line > canditates = {};
  for (Accumulate &accumulate : accumulator)
  {
     if (accumulate.votes > 0)
     {
       canditates.push_back(accumulate.line);
     }
     else
     {
       break;
     }
  }
  return canditates;
}



