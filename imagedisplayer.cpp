#include "imagedisplayer.h"

ImageDisplayer::~ImageDisplayer()
{
}

void ImageDisplayer::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15; // see QWheelEvent documentation

    scalings += numSteps;

    // if user moved the wheel in another direction, we reset previously scheduled scalings
    if (scalings * numSteps < 0) {
        scalings = numSteps;
    }

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (on_scrollAnimation_done()));

    anim->start();
}

void ImageDisplayer::scalingTime(qreal x)
{
    qreal factor = 1.0+ qreal(scalings) / 300.0;
    scale(factor, factor);
}

void ImageDisplayer::on_scrollAnimation_done()
{
    if (scalings > 0) {
        scalings--;
    }
    else {
        scalings++;
        sender()->~QObject();
    }
}

