#include "ImagePart.h"
#include <pthread.h>
#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <cstdlib> 
#include <ctime>        // std::time
#include <random>
#include <vigra/multi_array.hxx>
#include <vigra/impex.hxx>

using namespace std;
typedef vigra::MultiArray<2, int > BinaryArray;
typedef tuple<int,int> Point;
typedef tuple<float,float> Line;


 int ImagePart::myrandom (int i) 
{ 
  return rand()%i;
  
}

 vector<Point> ImagePart::selectRandomPoints()
 {
   vector<tuple<int,int>> points = allPoints();
   srand(time(0));
   random_shuffle( points.begin(), points.end());
   return points;
 }
vector< Point > ImagePart::allPoints()
{
  vector<Point> points = {};
   for( int x = 0; x < image.width(); x++)
   {
     for( int y = 0; y < image.height(); y++)
     {
      if (isNotZeroAt(x,y))
      {
	//Vigra multiarray has its origin on top left, but polar coordinate on bottom left
	//So X is correct, but Y is different
	tuple<int,int> point(x + get<0>(origin), image.height() + get<1>(origin) - y);
	points.push_back(point);
      }
     }
   }
   return points;
}
Point ImagePart::next()
{
  if (iteratorReady)
  {
    if (iterTheta == 0)
    {
      iterX = std::round(iterP);
      iterY += 1;
    }
    else
    {
      const float  degToRad  = 3.14159265 / 180;
      float radT = iterTheta*degToRad;
      float cosT = cos(radT);
      float sinT = sin(radT);
      iterX += 1;
      iterY = std::round((iterP - (iterX * cosT)) / sinT);
    }
    iterHasNext = iterX < image.width() && iterY < image.height();  
    return Point(iterX, iterY);
  }
  else
  {
    std::cout << "Not initialized";
  }
}

bool ImagePart::hasNext()
{
  return iterHasNext;
}
void ImagePart::initIterator(Line &line)
{
  iterTheta = get<0>(line);
  iterP = get<1>(line);
  iterX = 0;
  iterY = 0;
  iterHasNext = iterX < image.width() && iterY < image.height();
  iteratorReady = true;
}
bool ImagePart::isNotZeroAt(int x, int y)
{
  if (x > 0 && x < image.width() && y > 0 && y < image.height())
  {
      return image[vigra::Shape2(x,y)] != 0;
  }
  return false;
}



void ImagePart::removePoints(vector< Point > &points)
{
  int imgH = image.height();
  int imgW = image.width();
  for (Point &p : points)
  {
    int x = get<0>(p) - get<0>(origin);
    int y = image.height() - get<1>(p) - get<1>(origin);
    image[vigra::Shape2(x,y)] = 0;
  }
    int all = allPoints().size();
  std::string name = "./../images/edge/" + std::to_string (all) + ".png";
  vigra::exportImage(image, name);
}



