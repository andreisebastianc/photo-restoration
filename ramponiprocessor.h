#ifndef YCRCBPROCESSOR_H
#define YCRCBPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPixmap>
#include <QtMath>
#include <QDebug>
#include <array>
#include "iprocessor.h"
#include "processorutils.h"

namespace Ramponi {

    class Processor : public IProcessor
    {
        public:
            Processor(QVector<int> params);
            ~Processor();
            QVector<QPair<QPixmap, QString> > process(const QPixmap &pixmap) const;

            static const QVector<QPair<QString, QString>> config;

        private:
            // @note use config object to describe these params
            float s;
            float n;
            float A;
            float k;

            int calculateOtsuThreshold(const QVector<int> histogram, int total) const;
            cv::Mat produceSmoothMat(const cv::Mat numeratorMat, const cv::Mat denominator, const int flag = CV_8UC1) const;
            cv::Mat produceDetailsMat(const cv::Mat luminanceMatrix, const cv::Mat smoothedImage, const int coeficient) const;
            cv::Mat correctFoxing(const cv::Mat &src, const cv::Mat &smoothFoxing) const;
            int calculateDetailImageCoeficient(const cv::Mat luminanceMat, const cv::Mat foxedMat, int threshold) const;
    };

}

#endif // YCRCBPROCESSOR_H

