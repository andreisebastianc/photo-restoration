#ifndef IPROCESSOR_H
#define IPROCESSOR_H
#include <opencv2/core/core.hpp>
#include <QPixmap>

class IProcessor
{
public:
    virtual ~IProcessor(){}
    virtual QVector<QPair<QPixmap, QString>> process(const QPixmap &pixmap) const = 0;
};

#endif // IPROCESSOR_H
