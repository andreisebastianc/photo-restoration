#include "processorutils.h"

ProcessorUtils::ProcessorUtils()
{

}

// from
// https://bitbucket.org/bastian_weber/hibo/src/8eb02029bb54d6e14b7038d522a3ed9489617ae7/BorderExtractor.cpp?at=default
QPixmap ProcessorUtils::Mat2QPixmap(const cv::Mat &src)
{
    QImage image;

    if (src.type() == CV_8UC4){
        image = QImage(src.data, src.cols, src.rows, src.step, QImage::Format_ARGB32);
    } else if (src.type() == CV_8UC3){
        image = QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888).rgbSwapped();
    } else if (src.type() == CV_8UC1){
        QVector<QRgb> sColorTable;
        for (int i = 0; i < 256; ++i){
            sColorTable.push_back(qRgb(i, i, i));
        }
        image = QImage(src.data, src.cols, src.rows, src.step, QImage::Format_Indexed8);
        image.setColorTable(sColorTable);
    } else{
        qDebug() << "ERROR: Conversion from cv::Mat to QPixmap unsuccessfull because type is unknown.";
    }

    return QPixmap::fromImage(image);
}

// from
// https://bitbucket.org/bastian_weber/hibo/src/8eb02029bb54d6e14b7038d522a3ed9489617ae7/BorderExtractor.cpp?at=default
cv::Mat ProcessorUtils::QPixmap2Mat(const QPixmap &src)
{
    QImage image = src.toImage();
    cv::Mat mat;

    if (image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_ARGB32_Premultiplied){
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    } else if (image.format() == QImage::Format_RGB32){
        QImage converted = image.convertToFormat(QImage::Format_RGB888).rgbSwapped();
        mat = cv::Mat(converted.height(), converted.width(), CV_8UC3, const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
        //alternative: don't convert and use CV_8UC4 - but then you have a white alpha channel
    } else if (image.format() == QImage::Format_RGB888){
        QImage swapped = image.rgbSwapped();
        mat = cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
    } else if (image.format() == QImage::Format_Indexed8){
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    } else{
        qDebug() << "ERROR: Conversion from QPixmap to cv::Mat unsuccessfull because type is unknown.";
    }

    return mat;
}

cv::vector<cv::Mat> ProcessorUtils::ExtractYCrCb(const cv::Mat &src) {
    cv::Mat imgYCrCb;
    cv::vector<cv::Mat> channels;
    cv::cvtColor(src, imgYCrCb, cv::COLOR_RGB2YCrCb);
    cv::split(imgYCrCb, channels);
    return channels;
}

QVector<int> ProcessorUtils::extractHistogram(const cv::Mat grayscaleImg)
{
    QVector<int> hist;
    int i = 0;
    for(i = 0; i < 256; i++) {
        hist << 0;
    }

    cv::vector<cv::Mat> channels;

    cv::split(grayscaleImg, channels);
    int val;
    cv::Mat channel = channels[0];

    for (int i = 0; i < channel.cols; ++i) {
        for (int j = 0; j < channel.rows; ++j) {
            val = channel.at<uchar>(i,j);
            hist[val] += 1;
        }
    }

    return hist;
}

cv::Mat ProcessorUtils::ExtractFoxingMat(const cv::Mat crMat, int threshold)
{
    int i;
    int j;

    cv::Mat foxingDetected = cv::Mat::zeros(crMat.rows, crMat.cols, CV_8UC1);

    int maxCr = 0;
    for (i = 0; i < crMat.rows; ++i) {
        for (j = 0; j < crMat.cols; ++j) {
            if (maxCr < crMat.at<uchar>(i,j) ) {
                maxCr = crMat.at<uchar>(i,j);
            }
        }
    }

    for (i = 0; i < crMat.rows; ++i) {
        for (j = 0; j < crMat.cols; ++j) {
            if(crMat.at<uchar>(i,j) > ( maxCr - threshold)){
                foxingDetected.at<uchar>(i,j) = 1;
            }
        }
    }

    return foxingDetected;
}

