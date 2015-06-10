#ifndef YCRCBPROCESSOR_H
#define YCRCBPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPixmap>
#include "iprocessor.h"
#include "processorutils.h"

class YCrCBProcessor : public IProcessor
{
public:
    YCrCBProcessor();
    ~YCrCBProcessor();
    QPixmap process(const QPixmap &pixmap) const;
};

#endif // YCRCBPROCESSOR_H
