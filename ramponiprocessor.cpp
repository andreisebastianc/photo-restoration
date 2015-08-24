#include "ramponiprocessor.h"

Ramponi::Processor::Processor()
{
}

Ramponi::Processor::~Processor()
{
}

const QVector<QString> Ramponi::Processor::config = {
    "s, smoothing steps",
    "n, number of times rational filter is applied",
    "A, used in detail image extraction",
    "k, used in detail image extraction",
    "t, threshold output but Otsu's method"
};

cv::Mat Ramponi::Processor::produceSmoothMat(const cv::Mat mat, const int k, const int A, int smoothSteps) const
{
    cv::Mat res = cv::Mat(mat.rows, mat.cols, CV_8UC1);
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
int Ramponi::Processor::calculateDetailImageCoeficient(const cv::Mat luminanceMat, const cv::Mat foxedMat, const int threshold) const
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

cv::Mat Ramponi::Processor::produceDetailsMat(const cv::Mat luminanceMat, const cv::Mat smoothedImage, const int coeficient) const
{
    cv::Mat res = cv::Mat(luminanceMat.rows, luminanceMat.cols, CV_8UC1);

    int i;
    int j;

    for (i = 1; i < res.rows - 1; ++i) {
        for (j = 1; j < res.cols - 1; ++j) {
            res.at<uchar>(i,j) = luminanceMat.at<uchar>(i,j) - smoothedImage.at<uchar>(i,j) + coeficient;
        }
    }

    return res;
}

cv::Mat Ramponi::Processor::correctFoxing(const cv::Mat &src, const cv::Mat &smoothFoxing) const
{
    cv::Mat res = cv::Mat(src.rows, src.cols, CV_8UC1);

    int i;
    int j;
    int val;
    int median;
    int size;
    QVector<int> valuesForMedian;
    QVector<std::array<int,2>> temp;

    for (i = 1; i < smoothFoxing.rows - 1; ++i) {
        for (j = 1; j < smoothFoxing.cols - 1; ++j) {
            val = smoothFoxing.at<uchar>(i,j);
            if(val > 0) {
                valuesForMedian.push_back(val);
                temp.push_back({i,j});
            } else {
                res.at<uchar>(i,j) = src.at<uchar>(i,j);
            }
        }
    }

    std::sort(valuesForMedian.begin(), valuesForMedian.end());

    size = valuesForMedian.size();
    if(size % 2 == 0) {
        median = ( valuesForMedian.at(size / 2 - 1) + valuesForMedian.at(size / 2) ) / 2;
    } else {
        median = valuesForMedian.at(size / 2);
    }

    for (i = 0; i < temp.size(); ++i) {
        res.at<uchar>(temp.at(i)[0], temp.at(i)[1]) = median;
    }

    return res;
}

QPixmap Ramponi::Processor::process(const QPixmap &pixmap) const
{
    cv::Mat img = ProcessorUtils::QPixmap2Mat(pixmap);

    // Foxing
    // foxing detection
    cv::vector<cv::Mat> yCrCbMatChannels = ProcessorUtils::ExtractYCrCb(img);

    cv::Mat foxingMat = ProcessorUtils::ExtractFoxingMat(yCrCbMatChannels[2], 30);
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
    cv::Mat yn = cv::Mat(smoothFoxingMat.rows, smoothFoxingMat.cols, CV_8UC1);

    for (i = 1; i < yn.rows - 1; ++i) {
        for (j = 1; j < yn.cols - 1; ++j) {
            yn.at<uchar>(i,j) = detailImage.at<uchar>(i,j) * smoothFoxingMat.at<uchar>(i,j) +
                    luminanceMat.at<uchar>(i,j)*(1-smoothFoxingMat.at<uchar>(i,j));
        }
    }

    // color filtering
    // @todo need to figure out what to do with the resulting values from color filtering
    this->correctFoxing(yCrCbMatChannels[1], smoothFoxingMat);
    this->correctFoxing(yCrCbMatChannels[2], smoothFoxingMat);

    //  tonal adjustment

    return ProcessorUtils::Mat2QPixmap(yn);
}
