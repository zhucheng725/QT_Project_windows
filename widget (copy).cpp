#include "widget.h"
#include "ui_widget.h"
#include <QTimer>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/videoio/videoio.hpp>

#include <opencv4/opencv2/imgproc/types_c.h>

#include <iostream>

#include"tinyxml2.h"
using namespace tinyxml2;

using namespace std;
using namespace cv;



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Widget)
{
    ui_->setupUi(this);
    timer1_ = new QTimer(this);
    connect(timer1_,SIGNAL(timeout()),this,SLOT(readFrame())); //timer触发readFrame槽函数


}

// 关闭窗口释放地址空间
Widget::~Widget()
{
    if (timer1_ != NULL) {
        delete timer1_;
    }
    if (ui_ != NULL) {
      delete ui_;
    }

}






//定时将mat格式图像转成QImage格式的图像
void Widget::readFrame()
{

    std::vector<vector<Point>>	contours;
    std::vector<Vec4i>			hierarchy;
    double max_area = 50;
    double min_area = 10;
    double max_length = 20;
    double min_length = 5;
    Scalar color(255, 255, 255);
    Mat srcImageClone, srcGray,dstImage, BinaryImage, contourImage, contourImageGray;

    cap_.read(srcImage_); // 读取摄像头图片
    srcImageClone = srcImage_.clone();
    dstImage = srcImage_.clone();
    cv::cvtColor(srcImage_, srcGray, CV_BGR2GRAY);// 灰度图

    cv::GaussianBlur(srcGray, srcGray, Size(13, 13), 2,2);//高斯滤波
//    cv::medianBlur(srcGray, srcGray, 5);// 中值滤波
    threshold(srcGray, BinaryImage, 128, 255, CV_THRESH_BINARY);// 二值化

    cv::findContours(BinaryImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);// 边缘提取

//    Rect rect = boundingRect(Mat(contours[0]));
//    std::cout << "---rect---" << std::endl;
//    std::cout << rect.tl() << std::endl;
//    std::cout << rect.br() << std::endl;
//    std::cout << rect.width << std::endl;
//    std::cout << rect.height<< std::endl;
//    std::cout << "---rect---" << std::endl;
//    contourImage = srcImageClone(rect);


    // 面积和长度在某个阈值范围内保留，剩余的剔除。保留方式以在新建图里用白色绘制进去
    // m就是保留相关轮廓矩阵
    Mat m = Mat::zeros(dstImage.rows, dstImage.cols, CV_8UC3);
    for(size_t idx= 0;idx <contours.size();idx++){
        if(contourArea(contours[idx]) < max_area && contourArea(contours[idx]) > min_area){
            if(cv::arcLength(contours[idx], true) < max_length && cv::arcLength(contours[idx], true) > min_length){

//                std::vector<Vec3f> circles;
//                Rect rect = boundingRect(Mat(contours[idx]));
//                cv::rectangle(m, rect, color, 1, 8);
                cv::drawContours( m, contours, idx,  color, -1, 8, hierarchy );

//                std::cout << "---rect---" << std::endl;
//                std::cout << rect.tl().x<< std::endl;
//                std::cout <<  rect.tl().y << std::endl;
//                std::cout << rect.br().x << std::endl;
//                std::cout << rect.br().y << std::endl;
//                Rect new_rect(rect.tl().x, rect.tl().y, rect.br().x, rect.br().y);
//                std::cout << "---rect1---" << std::endl;
//                contourImage = srcImageClone(rect);
//                std::cout << "---rect2---" << std::endl;
//                std::cout << contourImage.size << std::endl;
//                std::cout << "---rect2---" << std::endl;
//                cv::cvtColor(contourImage, contourImageGray, CV_BGR2GRAY);// 灰度图


            }
        }
    }

    // 原始图显示
    QImage img;
    img = QImage((const unsigned char*)(srcImageClone.data),srcImageClone.cols,srcImageClone.rows,QImage::Format_RGB888);
    ui_->label->setScaledContents(true);
    ui_->label->resize(ui_->label->width(),ui_->label->height());
    ui_->label->setPixmap(QPixmap::fromImage(img));

    processFrame(m); //处理原始图像，显示在窗口右侧

}




void Widget::processFrame(Mat& srcImage)
{
    //std::cout << "TESTY" << std::endl;
    Mat srcGray;
    std::vector<Vec3f> circles;
    cv::cvtColor(srcImage, srcGray, CV_BGR2GRAY);// 灰度图

//    HoughCircles(srcGray, circles, HOUGH_GRADIENT,1,10,50,200,0,0);
//    image：输入图像：8-bit，灰度图
//    circles：输出圆的结果。
//    method：定义检测图像中圆的方法。目前唯一实现的方法是HOUGH_GRADIENT。
//    dp：寻找圆弧圆心的累计分辨率，这个参数允许创建一个比输入图像分辨率低的累加器。（这样做是因为有理由认为图像中存在的圆会自然降低到与图像宽高相同数量的范畴）。如果dp设置为1，则分辨率是相同的；如果设置为更大的值（比如2），累加器的分辨率受此影响会变小（此情况下为一半）。dp的值不能比1小。
//    minDist：该参数是让算法能明显区分的两个不同圆之间的最小距离。
//    param1 ：用于Canny的边缘阀值上限，下限被置为上限的一半。
//    param2：HOUGH_GRADIENT方法的累加器阈值。阈值越小，检测到的圈子越多。
//    minRadius ：最小圆半径。
//    maxRadius：最大圆半径

    HoughCircles(srcGray, circles, HOUGH_GRADIENT,1,1,100,7,0,30);// 霍夫圆检测
    for (size_t i = 0; i < circles.size(); i++){
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            //检测圆中心
            if(radius <= 3){
                circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
                //检测圆轮廓
                circle(srcImage, center, radius, Scalar(255, 0, 255), 3, 8, 0);
            }

        }


    //img = QImage((const unsigned char*)(srcImage.data),srcImage.cols,srcImage.rows,QImage::Format_Indexed8);//二值化 灰度图 显示方法
    QImage img;
    img = QImage((const unsigned char*)(srcImage.data),srcImage.cols,srcImage.rows,QImage::Format_RGB888); // 彩图显示方法
    ui_->label_2->setScaledContents(true);
    ui_->label_2->resize(ui_->label_2->width(),ui_->label_2->height());
    ui_->label_2->setPixmap(QPixmap::fromImage(img));


}



void Widget::checkCam()
{

    cap_.open("/dev/video0");  // 读取摄像头端口号

    if(!cap_.isOpened())    // 判断是否打开成功
    {
        ui_->textBrowser->append(" can not open usb camera");
    }
    else
    {
        ui_->textBrowser->append(" open usb camera");
        timer1_->start(10); //timer触发,打开定时器，实时读取摄像头
    }

}

//开始按钮触发
void Widget::on_pushButton_1_clicked()
{
    readXml();
    checkCam();
}


void Widget::button2_clicked()
{
    //关掉定时器，释放摄像头，退出程序
    timer1_->stop();
    cap_.release();
    QApplication *app;
    app->quit();
}

// 读取xml文件内容
void Widget::readXml()
{
    XMLDocument doc;
    doc.LoadFile("/media/kirito/1T/procedure/qt/project1/OpenCam/config.xml");
    const char* ROI_Width= doc.FirstChildElement( "ROI_Width" )->GetText();
    const char* ROI_Height= doc.FirstChildElement( "ROI_Height" )->GetText();
    const char* DetectPoint= doc.FirstChildElement( "DetectPoint" )->GetText();
    const char* Distance_Thresh= doc.FirstChildElement( "Distance_Thresh" )->GetText();

}



