#ifndef YCRCBPROCESSOR_H
#define YCRCBPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPixmap>
#include <QtMath>
#include <array>
#include "iprocessor.h"
#include "processorutils.h"

class RamponiProcessor : public IProcessor
{
public:
    RamponiProcessor();
    ~RamponiProcessor();

    QPixmap process(const QPixmap &pixmap) const;
    cv::Mat produceSmoothMat(const cv::Mat mat, const int k = 1, const int A = 1, int smoothSteps = 1) const;
    cv::Mat produceDetailsMat(const cv::Mat luminanceMatrix, const cv::Mat smoothedImage, const int coeficient) const;
    cv::Mat correctFoxing(const cv::Mat &src, const cv::Mat &smoothFoxing) const;
    int calculateDetailImageCoeficient(const cv::Mat luminanceMatrix, const cv::Mat foxedMatrix, const int threshold = 1) const;
};

#endif // YCRCBPROCESSOR_H
