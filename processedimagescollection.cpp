#include "processedimagescollection.h"

ProcessedImagesCollection::ProcessedImagesCollection()
{

}

ProcessedImagesCollection::~ProcessedImagesCollection()
{

}

void ProcessedImagesCollection::setPixmap(const QPixmap pixmap)
{
    this->pixmap = pixmap;
    this->processedPixmaps.clear();
    QPair<QPixmap, QString> orig = QPair<QPixmap, QString>(pixmap, "original");
    this->processedPixmaps.append(orig);

    for (int i = 0; i < this->processors.length(); ++i) {
        this->processPixmap(this->processors.at(i));
    }
}

QVector<QPair<QPixmap, QString> > ProcessedImagesCollection::processPixmap(const QSharedPointer<IProcessor> &processor) {
    QVector<QPair<QPixmap, QString> > processed = processor.data()->process(this->pixmap);
    QPair<QPixmap, QString> proc;
    foreach (proc, processed) {
        this->processedPixmaps.append(proc);
    }
    return processed;
}

QVector<QPair<QPixmap, QString> > ProcessedImagesCollection::addProcessor(const QSharedPointer<IProcessor> &processor)
{
    this->processors.append(processor);
    return this->processPixmap(processor);
}

int ProcessedImagesCollection::length()
{
    return this->processedPixmaps.length();
}

QPair<QPixmap, QString> ProcessedImagesCollection::at(int index)
{
    return this->processedPixmaps.at(index);
}

QList<QPair<QPixmap, QString>> ProcessedImagesCollection::getProcessedImages()
{
    return this->processedPixmaps;
}
