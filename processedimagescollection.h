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

    void setPixmap(const QPixmap pixmap);
    void addProcessor(const QSharedPointer<IProcessor> &processor);
    int length();
    QPixmap at(int index);

    QList<QPixmap> getProcessedImages();

private:
    QPixmap pixmap;

    QList<QSharedPointer<IProcessor> > processors;
    QList<QPixmap> processedPixmaps;
};

#endif // PROCESSEDIMAGESCOLLECTION_H
