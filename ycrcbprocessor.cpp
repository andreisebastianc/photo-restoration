#include "ycrcbprocessor.h"

YCrCBProcessor::YCrCBProcessor()
{

}

YCrCBProcessor::~YCrCBProcessor()
{

}

QPixmap YCrCBProcessor::process(const QPixmap &pixmap) const
{
    cv::Mat img = ProcessorUtils::QPixmap2Mat(pixmap);
    cv::Mat imgYCrCb;
    cv::Mat chan[3];
    cv::Mat aux[3];
    cv::Mat res;

    cv::cvtColor(img, imgYCrCb, cv::COLOR_BGR2YCrCb);

    cv::split(imgYCrCb, chan);

    double luminanceScalar[3] = {1, 127.5, 127.5};

    for(int i = 0; i < 3; i++){
        aux[i] = cv::Mat::ones(imgYCrCb.rows, imgYCrCb.cols, CV_8UC1) * luminanceScalar[i];
    }

    cv::merge(aux, 3, res);

    cv::cvtColor(res, res, cv::COLOR_YCrCb2BGR);

    return ProcessorUtils::Mat2QPixmap(res);
}

