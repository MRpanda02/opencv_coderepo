#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace std;


int main()
{
    cout<<"hello opncv4.2.0"<<endl;
    cv::namedWindow("image",0);
    cv::Mat img=cv::imread("E:/moon/opencv_coderepo/OPENCV/source/watercolor-backgrounds-02.jpg");
    cv::imshow("image",img);
    if(cv::waitKey(0)>= 0)
        return 0;
}
