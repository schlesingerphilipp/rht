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



void printResults(BinaryArray edgeArray, Transformation t, string path)
{
   const float  degToRad  = 3.14159265 / 180;
       // exportImage(edgeArray, "./../images/edge.png");
    //edgeArray = 0;
    for (Line l : t.lines)
    {
      float theta = std::get<0>(l);
      float p = std::get<1>(l);
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
    exportImage(edgeArray, path.c_str());
}
int evaluating(char **argv)
{
 string folder = argv[1]; 
 string imgPath = "../images/" + folder + "/image.png";
 ImageImportInfo imageInfo(imgPath.c_str());  
 MultiArray<2, float > imageArray(imageInfo.shape());  
 importImage(imageInfo, imageArray);
 BinaryArray edgeArray(imageInfo.shape());
 cannyEdgeImage(imageArray, edgeArray, 0.8, 4.0, 1);
 int xStep = atoi(argv[2]);
 int yStep = atoi(argv[3]);
 for (int i = 1; i < 21; i++)
 {
   auto start = std::chrono::high_resolution_clock::now();
   Transformation t = Rht::transform(edgeArray, xStep, yStep);
   auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - start;
   cout << i << "," << fp_ms.count() << "," << "all" << endl;
   string path = "./../images/"+folder+"/out/"+std::to_string(i)+".png";
   printResults(edgeArray, t, path.c_str());
 }
 return 0;
}
int debugging(char** argv) {
    ImageImportInfo imageInfo("../images/4.png");  
    MultiArray<2, float > imageArray(imageInfo.shape());  
    importImage(imageInfo, imageArray);
    BinaryArray edgeArray(imageInfo.shape());
    cannyEdgeImage(imageArray, edgeArray, 0.8, 4.0, 1);
    Transformation t = Rht::transform(edgeArray, 100, 100);
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
