#include <iostream>
#include <vigra/multi_array.hxx>
#include <vigra/impex.hxx>
#include <vigra/edgedetection.hxx>
#include "rht/Rht.cpp"
#include "rht/Transformation.h"
#include "math.h"
#include <string.h>
#include <chrono>


using namespace vigra;
typedef MultiArray<2, int > BinaryArray;
typedef std::tuple<float,float> Line;

BinaryArray getEdgeArray(MultiArray<2, float > imageArray)
{
  BinaryArray edgeArray(imageArray.shape());
  for (int x = 0; x < imageArray.width(); x++)
  {
    for (int y = 0; y < imageArray.height(); y++)
    {
      edgeArray[Shape2(x,y)]= imageArray[Shape2(x,y)] < 200 ? 1 : 0;
    }
  }
  return edgeArray;
}


void printResults(BinaryArray edgeArray, Transformation t, string path)
{
  
  MultiArray<2, RGBValue<UInt8> > rgb_image(edgeArray.shape());
  for (int x = 0; x < edgeArray.width(); x++)
  {
    for (int y = 0; y < edgeArray.height(); y++)
    {
      rgb_image[Shape2(x,y)]= edgeArray[Shape2(x,y)] == 1 ? RGBValue<UInt8>(0,255,0) : RGBValue<UInt8>(0,0,0) ;
    }
  }
  for (Line l : t.lines)
  {
    float theta = std::get<0>(l);
    float p = std::get<1>(l);
    if (theta == 0)
    {
      int x = std::round(p);
      for (int y = 0; y < edgeArray.height(); y ++)
      {
	rgb_image[Shape2(x,y)] = RGBValue<UInt8>(255,0,0);
      }
    }
    else if (theta == 1.57)
    {
      int y = std::round(p);
      int imgY = edgeArray.height() - y; 
      for (int x = 0; x < edgeArray.width(); x ++)
      {
	rgb_image[Shape2(x,imgY)] = RGBValue<UInt8>(255,0,0);
      }
    }
    else
    {
      //float radT = theta*degToRad;
      float cosT = cos(theta);
      float sinT = sin(theta);
      for (int x = 0; x < edgeArray.width(); x++)
      {   
	int y = std::round((p - (x * cosT)) / sinT);
	int imgY = edgeArray.height() - y;
	if (imgY < edgeArray.height() && imgY > 0)
	{
	  rgb_image[Shape2(x,imgY)] = RGBValue<UInt8>(255,0,0);
	}
      }
    }
  }
  exportImage(rgb_image, path.c_str());
}
int evaluating(char **argv)
{
 string folder = argv[1]; 
 string imgPath = "../images/" + folder + "/image.png";
 ImageImportInfo imageInfo(imgPath.c_str());  
 MultiArray<2, float > imageArray(imageInfo.shape());  
 importImage(imageInfo, imageArray);
 BinaryArray edgeArray(getEdgeArray(imageArray));
 int xStep = atoi(argv[2]);
 int yStep = atoi(argv[3]);
 int distanceThreshold = atoi(argv[4]);
 int pointsThreshold = atoi(argv[5]);
 int tolleranceTheta = atoi(argv[6]);
 int tolleranceP = atoi(argv[7]);
 //cout << "xyScale," << std::to_string(xStep) << "," << folder << endl;
 for (int i = 1; i < 21; i++)
 {
   auto start = std::chrono::high_resolution_clock::now();
   Transformation t = Rht::transform(edgeArray, xStep, yStep, distanceThreshold, pointsThreshold, tolleranceTheta, tolleranceP);
   auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - start;
   cout << fp_ms.count() << "," << "all" << endl;
   string path = "./../images/"+folder+"/out/"+std::to_string(xStep)+"run"+std::to_string(i)+".png";
   printResults(edgeArray, t, path.c_str());
 }
 return 0;
}
int debugging(char** argv) {
    ImageImportInfo imageInfo("../images/easy4.png");  
    MultiArray<2, float > imageArray(imageInfo.shape());  
    importImage(imageInfo, imageArray);
    BinaryArray edgeArray(getEdgeArray(imageArray));
    exportImage(edgeArray, "../images/edge.png");
    Transformation t = Rht::transform(edgeArray, 100, 100, 7, 33, 15, 15);
    std::cout << "Lines: ";
    std::cout <<  t.lines.size()<< endl;;
    for (Line l : t.lines)
    {
      float theta = std::get<0>(l);
      float p = std::get<1>(l);
      std::cout << theta << "," << p << endl;
    }
    printResults(edgeArray, t, "../images/rht.png");
    return 0;
}
int main(int argc, char **argv) {
    if (argc <= 1) {
        return debugging(argv);
    }
    return evaluating(argv);
}
