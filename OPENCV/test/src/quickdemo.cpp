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

void QuickDemo::bitwise_demo(Mat &image)
{
    Mat m1 = Mat::zeros(Size(256,256),CV_8UC3);
    Mat m2 = Mat::zeros(Size(256,256),CV_8UC3);
    Mat dst = Mat::zeros(Size(256,256),CV_8UC3);
    rectangle(m1,Rect(100,100,80,80),Scalar(255,255,0),-1,LINE_8,0);
    rectangle(m2,Rect(150,150,80,80),Scalar(0,255,255),-1,LINE_8,0);
    namedWindow("m1",WINDOW_FREERATIO);
    namedWindow("m2",WINDOW_FREERATIO);
    namedWindow("dst",WINDOW_FREERATIO);
    bitwise_and(m1,m2,dst);
    imshow("m1",m1);
    imshow("m2",m2);
    imshow("dst",dst);
}

void QuickDemo::channels_demo(Mat &image)
{
    std::vector<Mat> mv;
    split(image,mv);
    //bgr
    namedWindow("blue",WINDOW_FREERATIO);
    namedWindow("green",WINDOW_FREERATIO);
    namedWindow("red",WINDOW_FREERATIO);
    namedWindow("merge",WINDOW_FREERATIO);
    Mat dst;
    //mv[0] = Scalar(0);
    //mv[1] = Scalar(0);
    //mv[2] = Scalar(0);
    //merge(mv,dst);
    dst = Mat::zeros(image.size(),CV_8UC3);
    int from_to[] = {0,2,1,1,2,0};
    mixChannels(&image,1,&dst,1,from_to,3); 
    imshow("merge",dst);
    imshow("blue",mv[0]);
    imshow("green",mv[1]);
    imshow("red",mv[2]);
}

void QuickDemo::inrange_demo(Mat &image)
{
    Mat hsv;
    cvtColor(image,hsv,COLOR_BGR2HSV);
    Mat mask;
    inRange(hsv,Scalar(35,43,46),Scalar(77,255,255),mask);
    namedWindow("mask",WINDOW_FREERATIO);
    namedWindow("redback",WINDOW_FREERATIO);

    Mat redback = Mat::zeros(image.size(),image.type());
    redback = Scalar(40,40,200);
    bitwise_not(mask,mask);
    imshow("mask",mask);
    image.copyTo(redback,mask);
    imshow("redback",redback);
}

void QuickDemo::pixel_statistic_demo(Mat &image)
{
    double minv,maxv;
    Point minLoc,maxLoc;
    std::vector<Mat> mv;
    split(image,mv);
    for(int i=0;i<mv.size();i++){
        minMaxLoc(mv[i],&minv,&maxv,&minLoc,&maxLoc,Mat());
        std::cout << "No channels:" << i << " min value:" << minv << " max value:" << maxv << std::endl;
        std::cout << "MinLoc:" << minLoc << "  MaxLoc:" << maxLoc << std::endl;
    }
    Mat mean,stddev;
    meanStdDev(image,mean,stddev);
    std::cout << "means:" << mean << std::endl;
    std::cout << "stddev:" << stddev << std::endl;
}

void QuickDemo::drawing_demo(Mat &image)
{
    Rect rect;
    Mat bg = Mat::zeros(image.size(),image.type());
    Mat dst;
    rect.x = 200;
    rect.y = 200;
    rect.width = 100;
    rect.height = 150;
    rectangle(image,rect,Scalar(0,0,255),2,8,0);
    circle(image,Point(300,350),15,Scalar(255,0,0),2,8,0);
    line(image,Point(100,100),Point(350,400),Scalar(0,0,255),2,8,0);
    RotatedRect rrt;
    rrt.center = Point(200,200);
    rrt.size = Size(100,200);
    rrt.angle = 0.0;
    ellipse(image,rrt,Scalar(0,0,255),2,8);
    addWeighted(image,0.7,bg,0.3,0,dst);
    namedWindow("drawing demo",WINDOW_FREERATIO);
    imshow("drawing demo",image);
}

void QuickDemo::random_drawing(Mat &image)
{
    Mat canvas = Mat::zeros(Size(512,512),CV_8UC3);
    RNG rng(12345);
    namedWindow("canvas",WINDOW_FREERATIO);
    while(true){
        int c = waitKey(10);
        if(c == 27){
            break;
        }
        int x1 = rng.uniform(0,canvas.cols);
        int y1 = rng.uniform(0,canvas.rows); 
        int x2 = rng.uniform(0,canvas.cols);
        int y2 = rng.uniform(0,canvas.rows); 
        int b = rng.uniform(0,255);
        int g = rng.uniform(0,255);
        int r = rng.uniform(0,255);
        canvas = Scalar(0,0,0);
        line(canvas,Point(x1,y1),Point(x2,y2),Scalar(b,g,r),1,LINE_AA,0);
        imshow("canvas",canvas);
    }
}

void QuickDemo::polyline_drawing_demo(Mat &image)
{
    Mat canvas = Mat::zeros(Size(512,512),CV_8UC3);
    Point p1(100,100);
    Point p2(350,100);
    Point p3(450,280);
    Point p4(320,540);
    Point p5(80,400);
    std::vector <Point> pts;
    pts.push_back(p1);
    pts.push_back(p2);
    pts.push_back(p3);
    pts.push_back(p4);
    pts.push_back(p5);
    std::vector <std::vector<Point>> contours;
    contours.push_back(pts);
    drawContours(canvas,contours,-1,Scalar(255,0,0),-1);
    namedWindow("polyline",WINDOW_FREERATIO);
    imshow("polyline",canvas);
}

Point sp(-1,-1);
Point ep(-1,-1);
Mat temp;
static void on_draw(int event,int x,int y,int flags,void *userdata)
{
    Mat image = *((Mat*)userdata);
    if(event == EVENT_LBUTTONDOWN){
        sp.x = x;
        sp.y = y;
        std::cout << "sp.x : " << sp.x << std::endl;
    }
    else if(event == EVENT_LBUTTONUP){
        ep.x = x;
        ep.y = y;
        int dx = ep.x - sp.x;
        int dy = ep.y - sp.y;
        if(dx > 0 && dy > 0){
            Rect box(sp.x,sp.y,dx,dy);
            rectangle(image,box,Scalar(255,0,0),2,8,0);
            namedWindow("shubiao",WINDOW_FREERATIO);
            imshow("shubiao",image);
            namedWindow("fenli",WINDOW_FREERATIO);
            imshow("fenli",image(box));
            sp.x = -1;
            sp.y = -1;
        }
    }
    else if(event == EVENT_MOUSEMOVE){
        if(sp.x > 0 && sp.y > 0){
            ep.x = x;
            ep.y = y;
            int dx = ep.x - sp.x;
            int dy = ep.y - sp.y;
        if(dx > 0 && dy > 0){
            Rect box(sp.x,sp.y,dx,dy);
            temp.copyTo(image);
            rectangle(image,box,Scalar(255,0,0),2,8,0);
            namedWindow("shubiao",WINDOW_FREERATIO);
            imshow("shubiao",image);
            //reset
        }
        }
    }
}
void QuickDemo::mouse_drawing_demo(Mat &image)
{
    namedWindow("shubiao",WINDOW_FREERATIO);
    setMouseCallback("shubiao",on_draw,(void*)(&image));
    imshow("shubiao",image);
    image.copyTo(temp);
}