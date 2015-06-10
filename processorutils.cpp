#include "processorutils.h"

ProcessorUtils::ProcessorUtils()
{

}

QPixmap ProcessorUtils::Mat2QPixmap(const cv::Mat &src)
{
    cv::Mat temp; // make the same cv::Mat
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

