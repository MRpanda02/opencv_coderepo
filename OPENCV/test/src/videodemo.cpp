#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int g_slider_position = 0;
int g_run = 1,g_dontset = 0;
cv::VideoCapture g_cap;

void onTrackbarSlide(int pos,void *)
{
    g_cap.set(cv::CAP_PROP_POS_FRAMES,pos);

    if( !g_dontset )
        g_run = 1;
    g_dontset = 0;
}
int main()
{
    cv::namedWindow("videodemo",WINDOW_FREERATIO);
    g_cap.open("E:/moon/opencv_coderepo/OPENCV/source/Ink.mp4");
    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    int tmpw = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int tmph = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    cout << "Video has " << frames << " frames of dimensions(" << tmpw << ", " << tmph << ")." << endl;
    cout << g_cap.isOpened() << endl;
    
    cv::createTrackbar("barposition","videodemo",&g_slider_position,frames,onTrackbarSlide);

    cv::Mat frame;

    while(1){
        if(g_run != 0){
            g_cap >> frame;
            //cv::Mat picture = cv::imread("E:/moon/opencv_coderepo/OPENCV/source/watercolor-backgrounds-02.jpg");
            //imshow("videodemo",picture);
            if(frame.empty())   break;
            int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            g_dontset = 1;

            cv::setTrackbarPos("barposition","videodemo",current_pos);
            cv::imshow("videodemo",frame);

            g_run -= 1;
        }
        char c = (char)cv::waitKey(10);
        if(c == 's'){
            g_run = 1;cout << "single step,run = " << g_run << endl;
        }
        if(c == 'r'){
            g_run = -1;cout << "Run mode,run = " << g_run << endl;
        }
        if(c == 27){
            break;
        }
    }
    return 0;
}