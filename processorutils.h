#ifndef PROCESSORUTILS_H
#define PROCESSORUTILS_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPixmap>
#include <QImage>

class ProcessorUtils
{
public:
    ProcessorUtils();
    static QPixmap Mat2QPixmap(cv::Mat const& src);
    static cv::Mat QPixmap2Mat(QPixmap const& src);
};

#endif // PROCESSORUTILS_H
