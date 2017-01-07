#ifndef  IMAGE_PART_H
#define IMAGE_PART_H
#include <vigra/multi_array.hxx>
#include <tuple>
#include <vector>
  using namespace std;
  typedef vigra::MultiArray<2, int > BinaryArray;
  typedef tuple<int,int> Point;
    typedef tuple<float,float> Line;

class ImagePart {


  private:
    Point origin;
    int polarYOrigin;
    int myrandom (int i);
    BinaryArray image;
    bool iteratorReady = false;
    float iterTheta;
    float iterP;
    int iterY;
    int iterX;
    bool iterHasNext;
    Point topRight;

  public:
    ImagePart(Point &orig, BinaryArray &subimage, int polarY): origin(orig), image(subimage), polarYOrigin(polarY){};
    vector<Point> selectRandomPoints();
    vector<Point> allPoints();
    void removePoints(vector<Point> &points);
    bool isNotZeroAt(int x, int y);
    void initIterator(Line &line);
    bool hasNext();
    Point next();
  };
#endif