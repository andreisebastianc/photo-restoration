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
    QVector<QPair<QPixmap, QString> > addProcessor(const QSharedPointer<IProcessor> &processor);
    QPair<QPixmap, QString> at(int index);
    QList<QPair<QPixmap, QString>> getProcessedImages();
    void setPixmap(const QPixmap pixmap);
    int length();

private:
    QPixmap pixmap;
    QVector<QPair<QPixmap, QString> > processPixmap(const QSharedPointer<IProcessor> &processor);
    QList<QSharedPointer<IProcessor> > processors;
    QList<QPair<QPixmap, QString>> processedPixmaps;
};

#endif // PROCESSEDIMAGESCOLLECTION_H
