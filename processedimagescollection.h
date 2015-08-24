#ifndef PROCESSEDIMAGESCOLLECTION_H
#define PROCESSEDIMAGESCOLLECTION_H

#include <QList>
#include <QPixmap>
#include <QSharedPointer>
#include "iprocessor.h"

class ProcessedImagesCollection
{
public:
    ProcessedImagesCollection();
    ~ProcessedImagesCollection();
    QPixmap addProcessor(const QSharedPointer<IProcessor> &processor);
    QPixmap at(int index);
    QList<QPixmap> getProcessedImages();
    void setPixmap(const QPixmap pixmap);
    int length();

private:
    QPixmap pixmap;
    QPixmap processPixmap(const QSharedPointer<IProcessor> &processor);
    QList<QSharedPointer<IProcessor> > processors;
    QList<QPixmap> processedPixmaps;
};

#endif // PROCESSEDIMAGESCOLLECTION_H
