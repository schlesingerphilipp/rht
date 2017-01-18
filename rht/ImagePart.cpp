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
 /**
  * Returns Points relative to Polar coordiante system origin of the whole image (not only this part)
  */
vector< Point > ImagePart::allPoints()
{
  vector<Point> points = {};
   for( int x = 0; x < image.width(); x++)
   {
     for( int y = 0; y < image.height(); y++)
     {

      if (image[vigra::Shape2(x,y)] != 0)
      {
	//Vigra multiarray has its origin on top left, but polar coordinate on bottom left
	//So X is correct, but Y is different
	int yPolar = polarYOrigin +  image.height() - y;
	tuple<int,int> point(x + get<0>(origin), yPolar);
	points.push_back(point);
      }
     }
   }
   return points;
}
Point ImagePart::next()
{
  if (iterTheta == 0)
  {
    iterX = std::round(iterP);
    iterY += 1;
  }
  else if (iterTheta == 1.57) 
  {
    iterY =  std::round(iterP);
    iterX += 1;
  }
  else
  {
    float cosT = cos(iterTheta);
    float sinT = sin(iterTheta);
    iterX += 1;
    iterY = std::round((iterP - (iterX * cosT)) / sinT);
  }
  int imgX = iterX - get<0>(origin);
  int imgY = image.height() - (iterY - polarYOrigin);
  iterHasNext = imgX < image.width() || imgY < image.height();  
  return Point(iterX, iterY);
}

bool ImagePart::hasNext()
{
  return iterHasNext;
}
void ImagePart::initIterator(Line &line)
{
  iterTheta = get<0>(line);
  iterP = get<1>(line);
  iterX = get<0>(origin);
  iterY = polarYOrigin;
  iterHasNext = true;
  iteratorReady = true;
}
/**
 * Takes Points relative to polar origin of whole image.
 */
bool ImagePart::isNotZeroAt(int x, int y)
{
  int xImg = x - get<0>(origin);
  int yImg = image.height() - (y - polarYOrigin);
  if (xImg > 0 && xImg < image.width() && yImg > 0 && yImg < image.height())
  {
      return image[vigra::Shape2(xImg,yImg)] != 0;
  }
  return false;
}


/**
 * Takes Points relative to polar origin of whole image.
 */
void ImagePart::removePoints(vector< Point > &points)
{
  for (Point &p : points)
  {
    int x = get<0>(p) - get<0>(origin);
    int y = image.height() - (get<1>(p) - polarYOrigin);
    image[vigra::Shape2(x,y)] = 0;
  }
}



