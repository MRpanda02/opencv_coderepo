#include <iostream>
#include "../inc/quickopencv.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int args,char** argv)
{
    Mat img = imread("../watercolor-backgrounds-01.jpg");
    namedWindow("Src",WINDOW_FREERATIO);
    imshow("Src",img);
    if(img.empty()){
        cout << "Couldn't load this image"<< endl;
        return -1;
    }

    QuickDemo qd;
    qd.color_style_demo(img);
    waitKey(0);
    destroyAllWindows();
    cout << "Hello world" << endl;
    return 1;
}
