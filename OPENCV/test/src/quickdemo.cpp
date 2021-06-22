#include "../inc/quickopencv.h"

void QuickDemo::colorSpace_demo(Mat &image)
{
    Mat gray,hsv;
    cvtColor(image,hsv,COLOR_BGR2HSV);
    // H 0 ~ 180 , S/V 0~255
    cvtColor(image,gray,COLOR_BGR2GRAY);
    namedWindow("hsv",WINDOW_FREERATIO);
    namedWindow("gray",WINDOW_FREERATIO);
    imshow("hsv",hsv);
    imshow("gray",gray);
}

void QuickDemo::mat_creation_demo(Mat &image)
{
    Mat m1,m2;
    m1 = image.clone();
    image.copyTo(m2);

    //create
    Mat m3 = Mat::zeros(Size(8,8),CV_8UC3);
    m3 = Scalar(0,0,127);
    std::cout <<  "width:" << m3.cols << "height:" << m3.rows << "channels:" << m3.channels()<< std::endl;
    std::cout << m3 << std::endl;
    namedWindow("chuangjiantuxiang",WINDOW_FREERATIO);
    imshow("chuangjiantuxiang",m3);
}

void QuickDemo::pixel_visit_demo(Mat &image)
{
    int w = image.cols;
    int r = image.rows;
    int dims = image.channels();
    //std::cout << dims;
    /*for(int row = 0;row < r;row++){
        for(int col = 0;col < w;col++){
            if(dims == 1){      //gray
                int pv = image.at<uchar>(row,col);
                image.at<uchar>(row,col) = 255 - pv;
            }
            if(dims == 3){          //BGR
                Vec3b bgr = image.at<Vec3b>(row,col);
                image.at<Vec3b>(row,col)[0] = 255 - bgr[0];
                image.at<Vec3b>(row,col)[1] = 255 - bgr[1];
                image.at<Vec3b>(row,col)[2] = 255 - bgr[2];
            }
        }*/

        for(int row = 0;row < r;row++){
            uchar* current_row = image.ptr<uchar>(row);
            for(int col = 0;col < w;col++){
                if(dims == 1){      //gray
                    int pv = *current_row;
                    *current_row++ = 255 - pv;
                }
                if(dims == 3){          //BGR
                    *current_row++ = 255 - *current_row;
                    *current_row++ = 255 - *current_row;
                    *current_row++ = 255 - *current_row;
                }
        }
        namedWindow("xiangsufanxie",WINDOW_FREERATIO);
        imshow("xiangsufanxie",image);
    }
}

void QuickDemo::operators_demo(Mat& image)
{
    Mat dst;
    Mat m = Mat::zeros(image.size(),image.type());
    m = Scalar(2,2,2);
    dst = image - Scalar(10,10,10);        
    multiply(image,m,dst);  
    namedWindow("add operator",WINDOW_FREERATIO);
    imshow("add operator",dst);


    // original operator
    int w = image.cols;
    int h = image.rows;
    int dims = image.channels();
    for(int row = 0;row < h;row++){
        for(int col = 0;col < w;col++){
            Vec3b p1 = image.at<Vec3b>(row,col);
            Vec3b p2 = m.at<Vec3b>(row,col);
            dst.at<Vec3b>(row,col)[0] = saturate_cast<uchar>(p1[0] - p2[0]);
            dst.at<Vec3b>(row,col)[1] = saturate_cast<uchar>(p1[1] - p2[1]);
            dst.at<Vec3b>(row,col)[2] = saturate_cast<uchar>(p1[2] - p2[2]);
        }
        }
    add(image,m,dst);
    subtract(image,m,dst);
    divide(image,m,dst);
}


static void on_luminance(int x,void* userdata)
{   
    Mat image = *((Mat*)userdata);
    Mat b = Mat::zeros(image.size(),image.type());
    Mat m = Mat::zeros(image.size(),image.type());
    addWeighted(image,1.0,m,0,x,b);
    imshow("Contrast and luminance regulate",b);
}

static void on_contrast(int x,void* userdata)
{   
    Mat image = *((Mat*)userdata);
    Mat b = Mat::zeros(image.size(),image.type());
    Mat m = Mat::zeros(image.size(),image.type());
    double contrast = x / 100.0;
    addWeighted(image,contrast,m,0,0,b);
    imshow("Contrast and luminance regulate",b);
}

void QuickDemo::tracking_bar_demo(Mat &image)
{
    namedWindow("Contrast and luminance regulate",WINDOW_FREERATIO);
    int max_value = 100;
    int contrast_value = 100;
    int lightness = 50;
    createTrackbar("Value Bar:","Contrast and luminance regulate",&lightness,max_value,on_luminance,(void*)(&image));
    createTrackbar("Contrast Bar:","Contrast and luminance regulate",&contrast_value,200,on_contrast,(void*)(&image));
    on_luminance(lightness,&image);
    on_contrast(contrast_value,&image);
}

void QuickDemo::key_demo(Mat &image)
{
    Mat dst = Mat::zeros(image.size(),image.type());
    while(true){
        char c = waitKey(100);
        if(c == 52)
            break;
        if(c == 49){
            std::cout << "you print key # 1" << std::endl;
            cvtColor(image,dst,COLOR_BGR2GRAY);
        }
        if(c == 50){
            std::cout << "you print key # 2" << std::endl;
            cvtColor(image,dst,COLOR_BGR2HSV);
        }
        if(c == 51){
            std::cout << "you print key # 3" << std::endl;
            dst = Scalar(50,50,50);
            add(image,dst,dst);
        }
        namedWindow("jianpanxiangying",WINDOW_FREERATIO);
        imshow("jianpanxiangying",dst);
    }
}

void QuickDemo::color_style_demo(Mat &image)
{
    int colormap[]= {
    COLORMAP_AUTUMN,
    COLORMAP_BONE,
    COLORMAP_JET,
    COLORMAP_WINTER,
    COLORMAP_RAINBOW,
    COLORMAP_OCEAN,
    COLORMAP_SUMMER,
    COLORMAP_SPRING,
    COLORMAP_COOL,
    COLORMAP_PINK,
    COLORMAP_HOT,
    COLORMAP_PARULA,
    COLORMAP_MAGMA,
    COLORMAP_INFERNO,
    COLORMAP_PLASMA,
    COLORMAP_VIRIDIS,
    COLORMAP_CIVIDIS,
    COLORMAP_TWILIGHT,
    COLORMAP_TWILIGHT_SHIFTED};
    
    Mat dst;
    int index = 0;
    while(true){
        char c = waitKey(2000);
        if(c == 52)
            break;
        applyColorMap(image,dst,colormap[index%19]);
        index++;
        namedWindow("color style",WINDOW_FREERATIO);
        imshow("color style",dst);
    }
}