#include "ramponiprocessor.h"

Ramponi::Processor::Processor(QVector<int> params)
{
    this->s = params.at(0);
    this->n = params.at(1);
    this->A = params.at(2);
    this->k = params.at(3);
}

Ramponi::Processor::~Processor()
{
}

const QVector<QPair<QString, QString>> Ramponi::Processor::config = {
    QPair<QString, QString>("s, threshold for foxing", "80"),
    QPair<QString, QString>("n, number of times rational filter is applied", "10"),
    QPair<QString, QString>("A, used in detail image extraction", "10"),
    QPair<QString, QString>("k, used in detail image extraction", "10")
};

cv::Mat Ramponi::Processor::produceSmoothMat(const cv::Mat numeratorMat, const cv::Mat denominator) const
{
    cv::Mat res = cv::Mat(numeratorMat.rows, numeratorMat.cols, CV_8UC1);
    int steps = this->n;
    int i;
    int j;
    int dividend1;
    int divisor1;
    int dividend2;
    int divisor2;

    //@todo add assert here

    while (steps--) {
        for (i = 1; i < numeratorMat.rows - 1; ++i) {
            for (j = 1; j < numeratorMat.cols - 1; ++j) {
                dividend1 = numeratorMat.at<uchar>(i-1,j) + numeratorMat.at<uchar>(i+1,j) - 2 * numeratorMat.at<uchar>(i,j);
                divisor1 = this->k * qPow( (denominator.at<uchar>(i-1,j) - denominator.at<uchar>(i+1,j)), 2 ) + this->A;

                dividend2 = numeratorMat.at<uchar>(i,j-1) + numeratorMat.at<uchar>(i,j+1) - 2 * numeratorMat.at<uchar>(i,j);
                divisor2 = this->k * qPow( (denominator.at<uchar>(i,j-1) - denominator.at<uchar>(i,j+1)), 2 ) + this->A;

                res.at<uchar>(i,j) = dividend1/divisor1 + dividend2/divisor2;
            }
        }
    }

    return res;
}

// @todo  N. Otsu “A Threshold selection method from gray-scale histogram” is t
int Ramponi::Processor::calculateDetailImageCoeficient(const cv::Mat luminanceMat, const cv::Mat foxedMat, int threshold) const
{
    int i;
    int j;
    int counter = 0;
    int res = 0; // M

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

    // @note assignment
    if( (size = valuesForMedian.size() ) ) {

        if(size % 2 == 0) {
            median = ( valuesForMedian.at(size / 2 - 1) + valuesForMedian.at(size / 2) ) / 2;
        } else {
            median = valuesForMedian.at(size / 2);
        }

        for (i = 0; i < temp.size(); ++i) {
            res.at<uchar>(temp.at(i)[0], temp.at(i)[1]) = median;
        }
    }

    return res;
}

QPixmap Ramponi::Processor::process(const QPixmap &pixmap) const
{
    cv::Mat img = ProcessorUtils::QPixmap2Mat(pixmap);

    int otsuThreshold = this->calculateOtsuThreshold(img);
    qDebug() << otsuThreshold;

    // Foxing

    cv::vector<cv::Mat> yCrCbMatChannels = ProcessorUtils::ExtractYCrCb(img);

    // foxing detection
    // Cr is pos 1 from yCrCb
    cv::Mat fox = ProcessorUtils::ExtractFoxingMat(yCrCbMatChannels[1], this->s);

    // detais image extraction
    //cv::Mat luminanceMat;
    cv::Mat luminanceMat = yCrCbMatChannels[0];

    cv::Mat smoothedMat = this->produceSmoothMat(luminanceMat, luminanceMat); // Ylp

    // why is K a shifting param?
    int coeficient = calculateDetailImageCoeficient(luminanceMat, fox, otsuThreshold); // K

    cv::Mat detailImage = this->produceDetailsMat(luminanceMat, smoothedMat, coeficient); // Yhp

    // processing of foxed areas
    // foxed map with smooth transitions
    cv::Mat smoothFoxingMat = this->produceSmoothMat(fox, luminanceMat); //FFox

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
    yCrCbMatChannels[1] = this->correctFoxing(yCrCbMatChannels[1], smoothFoxingMat);
    yCrCbMatChannels[2] = this->correctFoxing(yCrCbMatChannels[2], smoothFoxingMat);

    //  tonal adjustment
    cv::Mat res;
    cv::Mat output;
    yCrCbMatChannels[0] = yn;
    cv::merge(yCrCbMatChannels, res);
    cv::cvtColor(res, output, cv::COLOR_YCrCb2RGB);

    return ProcessorUtils::Mat2QPixmap(output);
}

int Ramponi::Processor::calculateOtsuThreshold(const cv::Mat img) const
{

    cv::MatND histogram;
    cv::Mat gray;
    cv::cvtColor(img, gray, CV_RGB2GRAY);
    // Initialize parameters
    int histSize = 256;    // bin size
    float range[] = { 0, 255 };
    const float *ranges[] = { range };

    // Calculate histogram
    calcHist( &gray, 1, 0, cv::Mat(), histogram, 1, &histSize, ranges, true, false );

    // copied from http://www.dandiggins.co.uk/arlib-9.html
    int i;
    float w = 0; // first order cumulative
    float u = 0; // second order cumulative
    float uT = 0; // total mean level

    int k = 255; // maximum histogram index

    float histNormalized[256];  // normalized histogram values

    float work1, work2; // working variables
    double work3 = 0.0;


    // Create normalised histogram values
    // (size=image width * image height)
    for (i=1; i<=k; i++) {
        histNormalized[i] = histogram.at<uchar>(i) / (float)histSize;
    }


    // Calculate total mean level
    for (i=1; i<=k; i++) {
        uT+=(i*histNormalized[i]);
    }


    // Find optimal threshold value
    for (i=1; i<k; i++) {
        w+=histNormalized[i-1];
        u+=(i*histNormalized[i-1]);
        work1 = (uT * w - u);
        work2 = (work1 * work1) / ( w * (1.0f-w) );
        if (work2>work3) work3=work2;
    }

    // Convert the final value to an integer
    int threshold = (int)sqrt(work3);
    return threshold;
}
