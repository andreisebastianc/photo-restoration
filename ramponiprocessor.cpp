#include "ramponiprocessor.h"

RamponiProcessor::RamponiProcessor()
{

}

RamponiProcessor::~RamponiProcessor()
{

}

cv::Mat RamponiProcessor::produceSmoothMat(const cv::Mat mat, const int k, const int A, int smoothSteps) const
{
    cv::Mat res = cv::Mat(mat.rows, mat.cols, CV_32F);
    int i;
    int j;
    int dividend1;
    int divisor1;
    int dividend2;
    int divisor2;

    while (smoothSteps--) {
        for (i = 1; i < mat.rows - 1; ++i) {
            for (j = 1; j < mat.cols - 1; ++j) {
                dividend1 = mat.at<uchar>(i-1,j) + mat.at<uchar>(i+1,j) - 2 * mat.at<uchar>(i,j);
                divisor1 = k * qPow( (mat.at<uchar>(i-1,j) + mat.at<uchar>(i+1,j)), 2 ) + A;

                dividend2 = mat.at<uchar>(i,j-1) + mat.at<uchar>(i,j+1) - 2 * mat.at<uchar>(i,j);
                divisor2 = k * qPow( (mat.at<uchar>(i,j-1) + mat.at<uchar>(i,j+1)), 2 ) + A;

                res.at<uchar>(i,j) = dividend1/divisor1 + dividend2/divisor2;
            }
        }
    }

    return res;
}

// @todo  N. Otsu “A Threshold selection method from gray-scale histogram” is t
int RamponiProcessor::calculateDetailImageCoeficient(const cv::Mat luminanceMat, const cv::Mat foxedMat, const int threshold) const
{
    int i;
    int j;
    int counter = 0;
    int res = 0;

    for (i = 1; i < luminanceMat.rows - 1; ++i) {
        for (j = 1; j < luminanceMat.cols - 1; ++j) {
            if ( foxedMat.at<uchar>(i,j) == 0 && luminanceMat.at<uchar>(i,j) > threshold ) {
                res += luminanceMat.at<uchar>(i,j);
                counter++;
            }
        }
    }

    if ( counter ) {
        return res / counter;
    }

    return 0;
}

cv::Mat RamponiProcessor::produceDetailsMat(const cv::Mat luminanceMat, const cv::Mat smoothedImage, const int coeficient) const
{
    cv::Mat res = cv::Mat(luminanceMat.rows, luminanceMat.cols, CV_32F);

    int i;
    int j;

    for (i = 1; i < res.rows - 1; ++i) {
        for (j = 1; j < res.cols - 1; ++j) {
            res.at<uchar>(i,j) = luminanceMat.at<uchar>(i,j) - smoothedImage.at<uchar>(i,j) + coeficient;
        }
    }

    return res;
}

QPixmap RamponiProcessor::process(const QPixmap &pixmap) const
{
    cv::Mat img = ProcessorUtils::QPixmap2Mat(pixmap);

    cv::vector<cv::Mat> channels;

    // Foxing
    // foxing detection
    cv::vector<cv::Mat> yCrCbMatChannels = ProcessorUtils::ExtractYCrCb(img);
    cv::Mat foxingMat = ProcessorUtils::ExtractFoxingMat(yCrCbMatChannels[0], 30);
    // detais image extraction
    cv::Mat luminanceMat;
    cv::cvtColor(img, luminanceMat, CV_BGR2GRAY);

    int coeficient = calculateDetailImageCoeficient(luminanceMat,foxingMat);

    cv::Mat smoothedMat = this->produceSmoothMat(luminanceMat);

    cv::Mat detailImage = this->produceDetailsMat(luminanceMat,smoothedMat, coeficient);

    // processing of foxed areas
    // foxed map with smooth transitions
    cv::Mat smoothFoxingMat = this->produceSmoothMat(foxingMat);

    // merging
    int i;
    int j;
    cv::Mat yn = cv::Mat(smoothFoxingMat.rows, smoothFoxingMat.cols, CV_32F);

    for (i = 1; i < yn.rows - 1; ++i) {
        for (j = 1; j < yn.cols - 1; ++j) {
            yn.at<uchar>(i,j) = detailImage.at<uchar>(i,j) * smoothFoxingMat.at<uchar>(i,j) +
                    luminanceMat.at<uchar>(i,j)*(1-smoothFoxingMat.at<uchar>(i,j));
        }
    }

    // color filtering

    // temp for outputing a picture
    channels.push_back(yn);
    channels.push_back(yn);
    channels.push_back(yn);

    cv::merge(channels, yn);

    return ProcessorUtils::Mat2QPixmap(yn);
}

