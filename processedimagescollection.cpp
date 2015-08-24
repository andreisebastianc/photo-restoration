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
    this->processedPixmaps.append(pixmap);

    for (int i = 0; i < this->processors.length(); ++i) {
        this->processPixmap(this->processors.at(i));
    }
}

QPixmap ProcessedImagesCollection::processPixmap(const QSharedPointer<IProcessor> &processor) {
    QPixmap processed = processor.data()->process(this->pixmap);
    this->processedPixmaps.append(processed);
    return processed;
}

QPixmap ProcessedImagesCollection::addProcessor(const QSharedPointer<IProcessor> &processor)
{
    this->processors.append(processor);
    return this->processPixmap(processor);
}

int ProcessedImagesCollection::length()
{
    return this->processedPixmaps.length();
}

QPixmap ProcessedImagesCollection::at(int index)
{
    return this->processedPixmaps.at(index);
}

QList<QPixmap> ProcessedImagesCollection::getProcessedImages()
{
    return this->processedPixmaps;
}
