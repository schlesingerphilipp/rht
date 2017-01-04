#include <iostream>
#include <vigra/multi_array.hxx>
#include <vigra/impex.hxx>
#include <vigra/edgedetection.hxx>
#include "rht/Rht.cpp"
#include "rht/Transformation.h"


using namespace vigra;
typedef MultiArray<2, int > BinaryArray;
typedef std::tuple<float,float> Line;

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    ImageImportInfo imageInfo("../images/5.png");  
    MultiArray<2, float > imageArray(imageInfo.shape());  
    importImage(imageInfo, imageArray);
    BinaryArray edgeArray(imageInfo.shape());
    cannyEdgeImage(imageArray, edgeArray, 0.8, 4.0, 1);
    exportImage(edgeArray, "./../images/3out.png");
    Transformation t = Rht::transform(edgeArray);
    std::cout << "Lines: ";
    std::cout <<  t.lines.size()<< endl;;
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
      else
      {    
	for (int x = 0; x < edgeArray.width(); x++)
	{   
	  int y = std::round((p - (x * cos(theta))) / sin(theta));
	  if (y < edgeArray.height() && y > 0)
	  {
	    edgeArray[Shape2(x,edgeArray.height() -y)] = 1;
	  }
	}
      }
    }
    exportImage(edgeArray, "./../images/rht.png");
    return 0;
}
