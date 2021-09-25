#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "opencv_extended.h"
#include "ArmorDetector.h"

using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace rm;

std::vector <cv::RotatedRect> lightInfos;

std::vector <ArmorDescriptor> _armor;
ArmorParam _param;
ArmorDescriptor _targetArmor;
int flag = 0;
cv::Mat splitimage(Mat image,string enemy_color)
{
    Mat aChannels[3];
    Mat image2 = Mat::zeros(image.size(),CV_8UC1);
    split(image,aChannels);
    // imshow("split",image);
    // imshow("B",aChannels[0]);
    // imshow("G",aChannels[1]);
    // imshow("R",aChannels[2]);
    if(enemy_color == "RED")
        image2 = aChannels[2] - aChannels[0];
    else
        image2 = aChannels[0] - aChannels[2];
    //imshow("aftersplit",image2);
    return image2;
}

cv::Mat thresdilate(Mat GrayImg)
{
    Mat BinBrightImg = Mat::zeros(GrayImg.size(),CV_8UC1);
    // 阈值化
    threshold(GrayImg, BinBrightImg,100, 255, cv::THRESH_BINARY);
    // 结构化元素
    Mat Element = getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(3,3));
    // 膨胀
    dilate(BinBrightImg, BinBrightImg,Element);
    //imshow("binlight",BinBrightImg);
    return BinBrightImg;
}

enum
{
	WIDTH_GREATER_THAN_HEIGHT,
	ANGLE_TO_UP
};

RotatedRect& adjustRec(cv::RotatedRect& rec,const int mode)
{
    float& width = rec.size.width;
    float& height = rec.size.height;
    float& angle = rec.angle;

    //cout << "修改前："<< angle << endl;
	if(mode == WIDTH_GREATER_THAN_HEIGHT)
	{
		if(width < height)
		{
			swap(width, height);
			angle += 90.0;
		}
	}

	while(angle >= 90.0) angle -= 180.0;
	while(angle < -90.0) angle += 180.0;

	if(mode == ANGLE_TO_UP)
	{
        if (angle >= 45.0)
        {
            swap(width, height);
            angle -= 90.0;
        }
        else if(angle < -45.0)
        {
            swap(width, height);
            angle += 90.0;
        }
    }
    //cout << "修改后："<< angle << endl;
    return rec;
}

bool ArmorDescriptor::isArmorPattern() const
{
	return true;
}

void PickUpLED(vector<vector<Point>>& LightContours, Mat GrayImg,Mat& Light)
{
    // // 开始遍历提取灯条
    for(const auto& contour : LightContours){
        // 得到面积
        float lightContourArea = contourArea(contour);
        // 面积不要太小
        if((contour.size() <= 5)) continue;                  // lightContourArea < param.light_min_area ???
        // 椭圆拟合区域得到外接矩形
        RotatedRect lightRec = fitEllipse(contour);
        // 矫正灯条
        adjustRec(lightRec, ANGLE_TO_UP);
        // 宽高比，凸度筛选灯条
        if(lightRec.size.width / lightRec.size.height > 1.0 || lightContourArea / lightRec.size.area() < 0.5) continue;     // ????
        // 对灯条的范围适当扩大
        lightRec.size.width *= 1.1;
        lightRec.size.height *= 1.1;
        Rect lightRect = lightRec.boundingRect();           // 得到此轮廓的最小正矩形   minAreaRect()得到包覆轮廓的最小斜矩形
        const Rect scrBound(Point(0,0), GrayImg.size());
        lightRect &= scrBound;
        // 因为颜色通道相减后己方灯条直接过滤，不需要判断颜色了，可直接将灯条保存
        rectangle(Light, lightRect, Scalar(255),3);
        lightInfos.push_back(lightRec);
        }
        if(!Light.empty())
        {
            imshow("rectangles",Light);
        }
        if(lightInfos.empty()){
            cout << " ARMOR_NO" << endl;
        }
}
int main(int argc,int **argv)
{
    VideoCapture camera(1);
    if(!camera.isOpened()){
        cout <<" camera can't opened! "<< endl;
        return -1;
    }
    string enemy_color;
    cout << "enemy_color: RED or BLUE" << endl;
    cin >> enemy_color;
    // namedWindow("binlight",WINDOW_FREERATIO);
    // namedWindow("split",WINDOW_FREERATIO);
    // namedWindow("B",WINDOW_FREERATIO);
    // namedWindow("G",WINDOW_FREERATIO);
    // namedWindow("R",WINDOW_FREERATIO);
    // namedWindow("aftersplit",WINDOW_FREERATIO);
    namedWindow("normal",WINDOW_FREERATIO);
    namedWindow("binbright",WINDOW_FREERATIO);
    namedWindow("rectangles",WINDOW_FREERATIO);
    //Mat image = imread("E:/moon/opencv_coderepo/OPENCV/source/dengtiao.jpg");
    Mat image;
    Mat GrayImg = Mat::zeros(image.size(),CV_8UC1);
    while(true){
        camera >> image;
        imshow("normal",image);
        Mat Light = Mat::zeros(image.size(),CV_8UC1);
        GrayImg = splitimage(image,enemy_color);
        Mat BinBrightImg = thresdilate(GrayImg);
    // 找轮廓
    vector<vector<Point>> LightContours;
    vector<Vec4i> hierarchy;
    findContours(BinBrightImg, LightContours, hierarchy, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    imshow("binbright",BinBrightImg);
    PickUpLED(LightContours,GrayImg,Light);
    // // 对灯条进行匹配筛选
    // // 按灯条中心x从小到大排序
    // sort(lightInfos.begin(),lightInfos.end(),[](const LightDescriptor& ld1,const LightDescriptor& ld2)
    // {
    //     return ld1.center.x < ld2.center.x;
    // });
    // cv::RotatedRect left,right;
    // for(size_t i = 0;i < lightInfos.size();i++){
    //     //遍历所有灯条进行匹配
    //     for(size_t j = i + 1;(j < lightInfos.size());j++)
    //     {
    //         const LightDescriptor& leftLight = lightInfos[i];
    //         const LightDescriptor& rightLight = lightInfos[j];
            
    //         // 角差
    //         float angleDiff_ = abs(leftLight.angle - rightLight.angle);

    //         // 长度差比率
    //         float LenDiff_ratio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);

    //         // 筛选
    //         if(angleDiff_ > _param.light_max_angle_diff_ || LenDiff_ratio > _param.light_max_height_diff_ratio_){
    //             continue;
    //         }

    //         // 左右灯条相距距离
    //         float dis = cvex::distance(leftLight.center,rightLight.center);
    //         // 左右灯条长度的平均值
    //         float meanLen = (leftLight.length + rightLight.length) / 2;
    //         // 左右灯条中心点y的差值
    //         float yDiff = abs(leftLight.center.y - rightLight.center.y);
    //         // y差比率
    //         float yDiff_ratio = yDiff / meanLen;
    //         // 左右灯条中心点x的差值
    //         float XDiff = abs(leftLight.center.x - rightLight.center.x);
    //         // 相距距离与灯条长度比值
    //         float xDiff_ratio = XDiff / meanLen;
    //         // 相距距离与灯条长度比值
    //         float ratio = dis / meanLen;
    //         // 筛选
    //         if(yDiff_ratio > _param.light_max_y_diff_ratio_ || 
    //         xDiff_ratio < _param.light_min_x_diff_ratio_ || 
    //         ratio > _param.armor_max_aspect_ratio_ || 
    //         ratio < _param.armor_min_aspect_ratio_)
    //         {
    //             continue;
    //         }

    //         // 按比值来确定大小装甲
    //         int armorType = ratio > _param.armor_big_armor_ratio ? BIG_ARMOR : SMALL_ARMOR;
    //         float ratiOff = (armorType == BIG_ARMOR) ? max( _param.armor_big_armor_ratio - ratio, float(0)) : 
    //         max(_param.armor_small_armor_ratio - ratio, float(0));
    //         float yOff = yDiff / meanLen;
    //         float rotationScore = -(ratiOff * ratiOff + yOff * yOff);
    //         // 得到匹配的装甲板
    //         rm::ArmorDescriptor armor(leftLight, rightLight, armorType, GrayImg, rotationScore, _param);
    //         // if(_armor.empty()){
    //         //     cout << " 没有匹配到装甲板" << endl;
    //         // }
    //         left.size.width = leftLight.width;
	// 	    left.size.height = leftLight.length;
	// 	    left.center = leftLight.center;
	// 	    left.angle = leftLight.angle;
    //         right.size.width = rightLight.width;
	// 	    right.size.height = rightLight.length;
	// 	    right.center = rightLight.center;
	// 	    right.angle = rightLight.angle;
    //     }
    // }
            // cv::Mat LED = Mat::zeros(image.size(),CV_8UC1);
            // Rect LightRect = left.boundingRect();
            // Rect RightRect = right.boundingRect();
            // rectangle(LED, LightRect, Scalar(255),3);
            // rectangle(LED, RightRect, Scalar(255),3);
            // //_armor.emplace_back(armor);
            // imshow("rectangles",LED);
            waitKey(10);

        // // 对找到的装甲板进行筛选
        // _armor.erase(remove_if(_armor.begin(),_armor.end(),[](rm::ArmorDescriptor& i)
        // {
        //     return !(i.isArmorPattern());
        // }),_armor.end());
        // // 全部判断不是装甲板
        // if(_armor.empty())
        // {
        //     _targetArmor.clear();
        //     if( flag == 3){
        //         return flag = 1;
        //     }
        //     else{
        //         cout << " No armor pattern detected. " << endl; 
        //     }
        // }

        /*************************************模板匹配*****************************************/
        // 模板匹配适合待识别图片不会变化的场景，选择合适的模板可以得到很高的准确率并且花费时间远小于svm等机器学习方法
        // 模板匹配 根据装甲板中心的图案判断是不是装甲板

    //waitKey(10);
    }
    // // if()
    if(cv::waitKey(0)>= 0)
        return 0;
}

