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
    this->processedPixmaps.append(pixmap);

    for (int i = 0; i < this->processors.length(); ++i) {
        QPixmap processed = this->processors.at(i).data()->process(pixmap);
        this->processedPixmaps.append(processed);
    }
}

void ProcessedImagesCollection::addProcessor(const QSharedPointer<IProcessor> &processor)
{
    this->processors.append(processor);
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
