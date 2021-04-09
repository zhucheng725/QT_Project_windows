#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <stdio.h>
#include "tinyxml2.h"


#include <direct.h>

using namespace tinyxml2;
using namespace std;
using namespace cv;

/*设定一个定时器用于触发readFrame函数，定时将摄像头视频加载到label显示*/
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Widget)
{
    ui_->setupUi(this);
    timer1_ = new QTimer(this);
    connect(timer1_,SIGNAL(timeout()),this,SLOT(readFrame()));
}

/* 关闭窗口释放地址空间 */
Widget::~Widget()
{
    if (timer1_) {
        delete timer1_;
    }
    if (ui_) {
      delete ui_;
    }
}



/* 定时将mat格式图像转成QImage格式的图像 */
void Widget::readFrame()
{
    Mat srcImageClone, srcGray,dstImage, BinaryImage;

    std::vector<vector<Point>>	contours;
    std::vector<vector<Point>>	contoursOutside;
    std::vector<Vec4i>			hierarchy;
    std::vector<Vec4i>			hierarchyOutside;

    int max_area = atoi(circle_max_area_.c_str());
    int min_area = atoi(circle_min_area_.c_str());
    int max_length = atoi(circle_max_length_.c_str());
    int min_length = atoi(circle_min_length_.c_str());

    int ROI_tl_x = atoi(ROI_tl_x_.c_str());
    int ROI_tl_y = atoi(ROI_tl_y_.c_str());
    int ROI_Height = atoi(ROI_Height_.c_str());
    int ROI_Width = atoi(ROI_Width_.c_str());

    int detectRed = 0;
    int detectGreen = 0;
    int DetectPoint = atoi(DetectPoint_.c_str());


//    std::cout << "ROI_tl_x:" <<ROI_tl_x<< std::endl;

    /* 将需要检测的区域的矩阵原位覆盖在已经创建同样大小的0矩阵 */
    /* 目的是将不在检测区域设置为0，感兴趣区域设置为原来的样子 */
    Rect rect(ROI_tl_x, ROI_tl_y, ROI_Width, ROI_Height);
    cap_.read(srcImage_); /* 读取摄像头图片, 640 400 */
    srcImageClone = srcImage_.clone();
    Mat m = Mat::zeros(srcImageClone.rows, srcImageClone.cols,  CV_8UC3);
    srcImageClone(rect).copyTo(m(rect));

    cv::cvtColor(m, srcGray, CV_BGR2GRAY); /* 灰度图 */
    threshold(srcGray, BinaryImage, 80, 255, CV_THRESH_BINARY); /* 二值化 */
    cv::findContours(BinaryImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE); /* 边缘提取  */
    cv::findContours(BinaryImage, contoursOutside, hierarchyOutside, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); /* 边缘提取  */

    /* 筛选在指定阈值范围的轮廓，没有父子关系的则为单独轮廓，有父子关系则为有缺陷轮廓处理 */
    for(size_t idx= 0;idx <contours.size();idx++){
        if(contourArea(contours[idx]) < max_area && contourArea(contours[idx]) > min_area){
            if(cv::arcLength(contours[idx], true) < max_length && cv::arcLength(contours[idx], true) > min_length){

                 if(hierarchy[idx][2] >= 0 || hierarchy[idx][3] >= 0){
                     Scalar color_red(255,0,0);
                     cv::drawContours( srcImageClone, contours, idx,  color_red, 1, 8, hierarchy);
                     detectRed++;
                 }
                 else{
                      Scalar color_green(0,255,64);
                      cv::drawContours( srcImageClone, contours, idx,  color_green, 1, 8, hierarchy);
                      detectGreen++;
                 }
               }
             }
           }

    /* 显示的内容串在一起打印到图片上 */
    String text("Test:");
    stringstream stringDetect;
    stringDetect << DetectPoint;
    text.append(stringDetect.str());
    stringstream stringDetect1;
    stringDetect1 << detectGreen;
    text.append(", Acc:");
    text.append(stringDetect1.str());
    stringstream stringDetect2;
    stringDetect2 << (contoursOutside.size() - detectGreen);
    text.append(", Err:");
    text.append(stringDetect2.str());
    cv::Point origin;
    origin.x = 10;
    origin.y = 100;

    /* 当实际检测数目等于准备检测的数目且错误的检测为0时则判断为成功，否则为失败 */
    if(detectGreen == DetectPoint && contoursOutside.size() - detectGreen == 0){
        text.append(", success");
    }
    else{
           text.append(", failed");
    }
    cv::putText(srcImageClone, text, origin, cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 255), 1, 8, 0);
    cv::rectangle(srcImageClone, Rect(ROI_tl_x, ROI_tl_y, ROI_Width , ROI_Height),cv::Scalar(255, 255, 0), 1, 8, 0);

    /* 转换图像数据放入label显示 */
    QImage img;
    img = QImage((const unsigned char*)(srcImageClone.data),srcImageClone.cols,srcImageClone.rows,QImage::Format_RGB888);
    // ui_->label->setScaledContents(true);
    // ui_->label->resize(ui_->label->width(),ui_->label->height());
    ui_->label->setPixmap(QPixmap::fromImage(img));


}



void Widget::checkCam()
{
    int deviceAddr = atoi(deviceAddr_.c_str());
    cap_.open(deviceAddr); /* 读取摄像头端口号 */
    if(!cap_.isOpened()) /* 判断是否打开成功 */
    {
        ui_->textBrowser->append("不能打开摄摄像头");
        ui_->textBrowser->append("请检查是否插入摄像头");
        ui_->textBrowser->append("或者摄像头参数是否正确");
    }
    else
    {
        ui_->textBrowser->append("打开摄摄像头");
        ui_->pushButton->setEnabled(false);
        timer1_->start(10); /* timer触发,打开定时器，实时读取摄像头 */
    }
}

/* 开始按钮触发 */
void Widget::on_pushButton_1_clicked()
{
    configFind();
    readXml();
    if(checkConfig_ == 0){
        checkCam();
    }

}

/* 关掉定时器，释放摄像头，退出程序 */
void Widget::button2_clicked()
{

    timer1_->stop();
    cap_.release();
    QApplication *app;
    app->quit();
}


void Widget::readXml()
{
    /* 加载xml文件 */
    XMLDocument doc;
    doc.LoadFile(filepath_.toStdString().data());

    /* doc.error为1即为错误加载 */
    if(doc.Error() == 1)
    {
        checkConfig_ = 1;
        ui_->textBrowser->append("配置文件加载错误，请重新配置");
    }
    else {

        checkConfig_ = 0; /* doc.error 为0即为正确加载 */
        ROI_Width_ = doc.FirstChildElement( "ROI_Width" )->GetText(); /* 检测区域宽度 */
        ROI_Height_ = doc.FirstChildElement( "ROI_Height" )->GetText(); /* 检测区域高度 */
        DetectPoint_ = doc.FirstChildElement( "DetectPoint" )->GetText(); /* 检测圆形数量 */
        ROI_tl_x_ = doc.FirstChildElement( "ROI_tl_x" )->GetText(); /* 检测区域坐标左上角x */
        ROI_tl_y_ = doc.FirstChildElement( "ROI_tl_y" )->GetText(); /* 检测区域坐标左上角y */
        circle_max_area_ = doc.FirstChildElement( "circle_max_area" )->GetText(); /* 检测区域圆形面积最大值阈值 */
        circle_min_area_ = doc.FirstChildElement( "circle_min_area" )->GetText(); /* 检测区域圆形面积最小值阈值 */
        circle_max_length_ = doc.FirstChildElement( "circle_max_length" )->GetText(); /* 检测区域圆形长度最大值阈值 */
        circle_min_length_ = doc.FirstChildElement( "circle_min_length" )->GetText(); /* 检测区域圆形长度最小值阈值 */
        deviceAddr_ = doc.FirstChildElement( "deviceAddr" )->GetText(); /* 摄像头地址 */
    }



}


void Widget::configFind()
{
    /* QString dirpath = QFileDialog::getExistingDirectory(this,"选择目录","./",QFileDialog::ShowDirsOnly); */ /* 选择文件夹路径 */
    filepath_ = QFileDialog::getOpenFileName(this,"选择单个文件","./"); /* 选择文件路径 */
    // ui_->textBrowser->append(filepath_);

}
