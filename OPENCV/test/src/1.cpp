#include <iostream>
#include "../inc/quickopencv.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int args,char** argv)
{
    Mat img = imread("../../source/greenscreendog.jpeg");
    namedWindow("Src",WINDOW_FREERATIO);
    if(img.empty()){
        cout << "Couldn't load this image"<< endl;
        return -1;
    }
    imshow("Src",img);
    QuickDemo qd;
    qd.inrange_demo(img);
    waitKey(0);
    destroyAllWindows();
    cout << "Hello world" << endl;
    return 1;
}
