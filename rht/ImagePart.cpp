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
/**
  * Returns Points relative to Polar coordiante system origin of the whole image (not only this part)
  * These are all points of this part, but they are shuffled
  */
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


/**
 * We opate an an binary edge represenatation. Here we check if this point is a point of interest, 
 * which are only points belonging to an edge. We are not interested in zero (empty space) points
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
 * Permanently remove these points , so they can not be evidence for other lines.
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



