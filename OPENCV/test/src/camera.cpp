#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc,char **argv)
{
    VideoCapture camera(1);
    if(!camera.isOpened()){
        cout <<" camera can't opened! "<< endl;
        return -1;
    }
    namedWindow("hhh",WINDOW_FREERATIO);
    while(true){
        Mat frame;
        camera >> frame;
        imshow("hhh",frame);
        waitKey(30);
    }
    return 0;
}
