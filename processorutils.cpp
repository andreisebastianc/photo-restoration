#include "processorutils.h"

ProcessorUtils::ProcessorUtils()
{

}

QPixmap ProcessorUtils::Mat2QPixmap(const cv::Mat &src)
{
    cv::Mat temp = cv::Mat(src.rows, src.cols, CV_32F); // make the same cv::Mat
    cv::cvtColor(src, temp, cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return QPixmap::fromImage(dest);
}

cv::Mat ProcessorUtils::QPixmap2Mat(const QPixmap &src)
{
    QImage image = src.toImage();
    cv::Mat tmp(image.height(),image.width(),CV_8UC3,(uchar*)image.bits(),image.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cv::cvtColor(tmp, result, cv::COLOR_BGR2RGB);
    return result;
}

cv::vector<cv::Mat> ProcessorUtils::ExtractYCrCb(const cv::Mat &src) {
    cv::Mat imgYCrCb;
    cv::vector<cv::Mat> channels;
    cv::cvtColor(src, imgYCrCb, cv::COLOR_BGR2YCrCb);
    cv::split(imgYCrCb, channels);
    return channels;
}

cv::Mat ProcessorUtils::ExtractFoxingMat(const cv::Mat crMat, int threshold)
{
    int i;
    int j;

    cv::Mat foxingDetected = cv::Mat::zeros(crMat.rows, crMat.cols, CV_32F);


    int maxCr = 0;
    for (i = 0; i < crMat.rows; ++i) {
        for (j = 0; j < crMat.cols; ++j) {
            if (maxCr < crMat.at<uchar>(i,j) ) {
                maxCr = crMat.at<uchar>(i,j);
            }
        }
    }

    for (i = 0; i < crMat.rows; ++i) {
        for (j = 0; j < crMat.cols; ++j) {
            if(crMat.at<uchar>(i,j) > ( maxCr - threshold)){
                foxingDetected.at<uchar>(i,j) = 1;
            }
        }
    }

    return foxingDetected;
}

