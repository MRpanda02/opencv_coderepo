#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
    cv::Mat D,B;
    cv::Mat A = cv::imread("E:/moon/opencv_coderepo/OPENCV/source/watercolor-backgrounds-02.jpg");
    namedWindow("image",WINDOW_FREERATIO);
    namedWindow("image_1",WINDOW_FREERATIO);
    namedWindow("image_2",WINDOW_FREERATIO);
    imshow("image",A);
    threshold(A,D,150,0,THRESH_BINARY);
    threshold(A,B,100,255,THRESH_BINARY);
    imshow("image_1",D);
    imshow("image_2",B);
    if(waitKey(0) >= 0)
        return 0;
}