#include <iostream>
#include <vigra/multi_array.hxx>
#include <vigra/impex.hxx>
#include <vigra/edgedetection.hxx>
#include "rht/Rht.cpp"
#include "rht/Transformation.h"
#include "math.h"


using namespace vigra;
typedef MultiArray<2, int > BinaryArray;
typedef std::tuple<float,float> Line;

int main(int argc, char **argv) {
    ImageImportInfo imageInfo("../images/synt6.png");  
    MultiArray<2, float > imageArray(imageInfo.shape());  
    importImage(imageInfo, imageArray);
    BinaryArray edgeArray(imageInfo.shape());
    cannyEdgeImage(imageArray, edgeArray, 0.8, 4.0, 1);
    Transformation t = Rht::transform(edgeArray);
    std::cout << "Lines: ";
    std::cout <<  t.lines.size()<< endl;;
    const float  degToRad  = 3.14159265 / 180;
       // exportImage(edgeArray, "./../images/edge.png");
    //edgeArray = 0;
    for (Line l : t.lines)
    {
      float theta = std::get<0>(l);
      float p = std::get<1>(l);
      std::cout << theta;
      std::cout << ",";
      std::cout << p<< endl;
      if (theta == 0)
      {
	int x = std::round(p);
	for (int y = 0; y < edgeArray.height(); y ++)
	{
	  edgeArray[Shape2(x,y)] = 1;
	}
      }
      else if (theta == 90)
      {
	int y = std::round(p);
	int imgY = edgeArray.height() - y; 
	for (int x = 0; x < edgeArray.width(); x ++)
	{
	  edgeArray[Shape2(x,imgY)] = 1;
	}
      }
      else
      {
	float radT = theta*degToRad;
	float cosT = cos(radT);
	float sinT = sin(radT);
	for (int x = 0; x < edgeArray.width(); x++)
	{   
	  int y = std::round((p - (x * cosT)) / sinT);
	  int imgY = edgeArray.height() - y;
	  if (imgY < edgeArray.height() && imgY > 0)
	  {
	    edgeArray[Shape2(x,imgY)] = 1;
	  }
	}
      }
    }
    exportImage(edgeArray, "./../images/rht.png");
    return 0;
}
