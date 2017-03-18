#include <iostream>
#include <vigra/multi_array.hxx>
#include <vigra/impex.hxx>
#include <vigra/edgedetection.hxx>
#include "rht/Rht.cpp"
#include "rht/Transformation.h"
#include "math.h"
#include <string.h>
#include <chrono>
#include <vigra/multi_convolution.hxx> 
#include <vigra/multi_blockwise.hxx>


using namespace vigra;
typedef MultiArray<2, int > BinaryArray;
typedef std::tuple<float,float, int, int> LineWO;

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


void printResults(BinaryArray edgeArray, Transformation t, string path, int xyStep)
{
  
  MultiArray<2, RGBValue<UInt8> > rgb_image(edgeArray.shape());
  for (int x = 0; x < edgeArray.width(); x++)
  {
    for (int y = 0; y < edgeArray.height(); y++)
    {
      rgb_image[Shape2(x,y)]= edgeArray[Shape2(x,y)] == 1 ? RGBValue<UInt8>(0,255,0) : RGBValue<UInt8>(0,0,0) ;
    }
  }
  for (LineWO l : t.lines)
  {
    float theta = std::get<0>(l);
    float p = std::get<1>(l);
    int xStart = get<2>(l);
    int yStart = get<3>(l);
    int xEnd = xStart + xyStep;
    int yEnd = yStart + xyStep;
    if (theta == 0)
    {
      int x = std::round(p);
      for (int y = yStart; y < yEnd; y ++)
      {
	rgb_image[Shape2(x,y)] = RGBValue<UInt8>(255,0,0);
      }
    }
    else if (theta == 1.57)
    {
      int y = std::round(p);
      int imgY = edgeArray.height() - y; 
      for (int x = xStart; x < xEnd; x ++)
      {
	rgb_image[Shape2(x,imgY)] = RGBValue<UInt8>(255,0,0);
      }
    }
    else
    {
      //float radT = theta*degToRad;
      float cosT = cos(theta);
      float sinT = sin(theta);
      int lastY = -1;
      for (int x = xStart; x < xEnd; x++)
      {   
	int y = std::round((p - (x * cosT)) / sinT);
	int imgY = edgeArray.height() - y;
	if (imgY < yEnd && imgY > 0 && imgY > yStart)
	{
	  rgb_image[Shape2(x,imgY)] = RGBValue<UInt8>(255,0,0);
	  while (lastY != -1 && lastY > yStart && lastY < yEnd && ::abs(imgY - lastY) > 1)
	  {	    
	    lastY = ((lastY - imgY > 0) ? lastY - 1 : lastY + 1);
	    rgb_image[Shape2(x-1,lastY)] = RGBValue<UInt8>(255,0,0);
	  }
	  lastY = imgY;
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
 MultiArray<2, float > smoothed(imageArray.shape());
 double smoothing = atoi(argv[9]);
 gaussianSmoothMultiArray(imageArray, smoothed, smoothing);
 BinaryArray edgeArray(smoothed.shape());
 cannyEdgeImage(smoothed, edgeArray, 0.8, 4.0, 1);
 //BinaryArray edgeArray(getEdgeArray(smoothed));
 int xStep = atoi(argv[2]);
 int yStep = atoi(argv[3]);
 int distanceThreshold = atoi(argv[4]);
 int pointsThreshold = atoi(argv[5]);
 int tolleranceTheta = atoi(argv[6]);
 int tolleranceP = atoi(argv[7]);
 int numberOfThreads = atoi(argv[8]);
 for (int i = 1; i < 21; i++)
 {
   Transformation t = Rht::transform(edgeArray, xStep, distanceThreshold, pointsThreshold, tolleranceTheta, tolleranceP, numberOfThreads);
   string path = "./../images/"+folder+"/out/"+std::to_string(xStep)+"run"+std::to_string(i)+".png";
   printResults(edgeArray, t, path.c_str(), xStep);
 }
 return 0;
}
int debugging(char** argv) {
    ImageImportInfo imageInfo("../images/realSmaller.png");  
    MultiArray<2, float > imageArray(imageInfo.shape()); 
    importImage(imageInfo, imageArray);
    MultiArray<2, float > smoothed(imageArray.shape()); 
    gaussianSmoothMultiArray(imageArray, smoothed, 3.0);
    BinaryArray edgeArray(imageArray.shape());
    cannyEdgeImage(smoothed, edgeArray, 0.8, 4.0, 1);
    //BinaryArray edgeArray(getEdgeArray(smoothed));
    //exportImage(edgeArray, "../images/edge.png");
    int xyStep = 80;
    Transformation t = Rht::transform(edgeArray, xyStep, 3, 80, 15, 30, 999);
    std::cout << "Lines: ";
    std::cout <<  t.lines.size()<< endl;;
    printResults(edgeArray, t, "../images/rht.png", xyStep);
    return 0;
}
int main(int argc, char **argv) {
    if (argc <= 1) {
        return debugging(argv);
    }
    return evaluating(argv);
}
