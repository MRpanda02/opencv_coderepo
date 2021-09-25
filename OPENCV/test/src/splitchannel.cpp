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
    // ��ֵ��
    threshold(GrayImg, BinBrightImg,100, 255, cv::THRESH_BINARY);
    // �ṹ��Ԫ��
    Mat Element = getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(3,3));
    // ����
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

    //cout << "�޸�ǰ��"<< angle << endl;
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
    //cout << "�޸ĺ�"<< angle << endl;
    return rec;
}

bool ArmorDescriptor::isArmorPattern() const
{
	return true;
}

void PickUpLED(vector<vector<Point>>& LightContours, Mat GrayImg,Mat& Light)
{
    // // ��ʼ������ȡ����
    for(const auto& contour : LightContours){
        // �õ����
        float lightContourArea = contourArea(contour);
        // �����Ҫ̫С
        if((contour.size() <= 5)) continue;                  // lightContourArea < param.light_min_area ???
        // ��Բ�������õ���Ӿ���
        RotatedRect lightRec = fitEllipse(contour);
        // ��������
        adjustRec(lightRec, ANGLE_TO_UP);
        // ��߱ȣ�͹��ɸѡ����
        if(lightRec.size.width / lightRec.size.height > 1.0 || lightContourArea / lightRec.size.area() < 0.5) continue;     // ????
        // �Ե����ķ�Χ�ʵ�����
        lightRec.size.width *= 1.1;
        lightRec.size.height *= 1.1;
        Rect lightRect = lightRec.boundingRect();           // �õ�����������С������   minAreaRect()�õ�������������Сб����
        const Rect scrBound(Point(0,0), GrayImg.size());
        lightRect &= scrBound;
        // ��Ϊ��ɫͨ������󼺷�����ֱ�ӹ��ˣ�����Ҫ�ж���ɫ�ˣ���ֱ�ӽ���������
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
    // ������
    vector<vector<Point>> LightContours;
    vector<Vec4i> hierarchy;
    findContours(BinBrightImg, LightContours, hierarchy, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    imshow("binbright",BinBrightImg);
    PickUpLED(LightContours,GrayImg,Light);
    // // �Ե�������ƥ��ɸѡ
    // // ����������x��С��������
    // sort(lightInfos.begin(),lightInfos.end(),[](const LightDescriptor& ld1,const LightDescriptor& ld2)
    // {
    //     return ld1.center.x < ld2.center.x;
    // });
    // cv::RotatedRect left,right;
    // for(size_t i = 0;i < lightInfos.size();i++){
    //     //�������е�������ƥ��
    //     for(size_t j = i + 1;(j < lightInfos.size());j++)
    //     {
    //         const LightDescriptor& leftLight = lightInfos[i];
    //         const LightDescriptor& rightLight = lightInfos[j];
            
    //         // �ǲ�
    //         float angleDiff_ = abs(leftLight.angle - rightLight.angle);

    //         // ���Ȳ����
    //         float LenDiff_ratio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);

    //         // ɸѡ
    //         if(angleDiff_ > _param.light_max_angle_diff_ || LenDiff_ratio > _param.light_max_height_diff_ratio_){
    //             continue;
    //         }

    //         // ���ҵ���������
    //         float dis = cvex::distance(leftLight.center,rightLight.center);
    //         // ���ҵ������ȵ�ƽ��ֵ
    //         float meanLen = (leftLight.length + rightLight.length) / 2;
    //         // ���ҵ������ĵ�y�Ĳ�ֵ
    //         float yDiff = abs(leftLight.center.y - rightLight.center.y);
    //         // y�����
    //         float yDiff_ratio = yDiff / meanLen;
    //         // ���ҵ������ĵ�x�Ĳ�ֵ
    //         float XDiff = abs(leftLight.center.x - rightLight.center.x);
    //         // ��������������ȱ�ֵ
    //         float xDiff_ratio = XDiff / meanLen;
    //         // ��������������ȱ�ֵ
    //         float ratio = dis / meanLen;
    //         // ɸѡ
    //         if(yDiff_ratio > _param.light_max_y_diff_ratio_ || 
    //         xDiff_ratio < _param.light_min_x_diff_ratio_ || 
    //         ratio > _param.armor_max_aspect_ratio_ || 
    //         ratio < _param.armor_min_aspect_ratio_)
    //         {
    //             continue;
    //         }

    //         // ����ֵ��ȷ����Сװ��
    //         int armorType = ratio > _param.armor_big_armor_ratio ? BIG_ARMOR : SMALL_ARMOR;
    //         float ratiOff = (armorType == BIG_ARMOR) ? max( _param.armor_big_armor_ratio - ratio, float(0)) : 
    //         max(_param.armor_small_armor_ratio - ratio, float(0));
    //         float yOff = yDiff / meanLen;
    //         float rotationScore = -(ratiOff * ratiOff + yOff * yOff);
    //         // �õ�ƥ���װ�װ�
    //         rm::ArmorDescriptor armor(leftLight, rightLight, armorType, GrayImg, rotationScore, _param);
    //         // if(_armor.empty()){
    //         //     cout << " û��ƥ�䵽װ�װ�" << endl;
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

        // // ���ҵ���װ�װ����ɸѡ
        // _armor.erase(remove_if(_armor.begin(),_armor.end(),[](rm::ArmorDescriptor& i)
        // {
        //     return !(i.isArmorPattern());
        // }),_armor.end());
        // // ȫ���жϲ���װ�װ�
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

        /*************************************ģ��ƥ��*****************************************/
        // ģ��ƥ���ʺϴ�ʶ��ͼƬ����仯�ĳ�����ѡ����ʵ�ģ����Եõ��ܸߵ�׼ȷ�ʲ��һ���ʱ��ԶС��svm�Ȼ���ѧϰ����
        // ģ��ƥ�� ����װ�װ����ĵ�ͼ���ж��ǲ���װ�װ�

    //waitKey(10);
    }
    // // if()
    if(cv::waitKey(0)>= 0)
        return 0;
}

