#ifndef PROCESSORUTILS_H
#define PROCESSORUTILS_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPixmap>
#include <QImage>
#include <QDebug>

class ProcessorUtils
{
public:
    ProcessorUtils();
    static QPixmap Mat2QPixmap(cv::Mat const& src);
    static cv::Mat QPixmap2Mat(QPixmap const& src);
    static cv::Mat ExtractFoxingMat(cv::Mat const CrMat, int threshold);
    static cv::vector<cv::Mat> ExtractYCrCb(const cv::Mat& src);
};

#endif // PROCESSORUTILS_H
