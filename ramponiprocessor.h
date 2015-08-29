#ifndef YCRCBPROCESSOR_H
#define YCRCBPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPixmap>
#include <QtMath>
#include <array>
#include "iprocessor.h"
#include "processorutils.h"

namespace Ramponi {

    class Processor : public IProcessor
    {
        public:
            Processor(QVector<int> params);
            ~Processor();
            QPixmap process(const QPixmap &pixmap) const;

            static const QVector<QString> config;

        private:
            // @note use config object to describe these params
            int s;
            int n;
            int A;
            int k;
            int t;

            cv::Mat produceSmoothMat(const cv::Mat numeratorMat, const cv::Mat denominator) const;
            cv::Mat produceDetailsMat(const cv::Mat luminanceMatrix, const cv::Mat smoothedImage, const int coeficient) const;
            cv::Mat correctFoxing(const cv::Mat &src, const cv::Mat &smoothFoxing) const;
            int calculateDetailImageCoeficient(const cv::Mat luminanceMat, const cv::Mat foxedMat) const;
    };

}

#endif // YCRCBPROCESSOR_H

