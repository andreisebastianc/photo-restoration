#ifndef IMAGEDISPLAYER_H
#define IMAGEDISPLAYER_H

#include <QGraphicsView>
#include <QTimeLine>
#include <QWheelEvent>

class ImageDisplayer : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageDisplayer(QGraphicsScene* scene, QWidget* parent=0) : QGraphicsView(scene, parent)
    {
    }
    virtual ~ImageDisplayer();

public slots:
    void scalingTime(qreal x);
    void on_scrollAnimation_done();

signals:
    void scroll( QWheelEvent* event );

protected:
    void wheelEvent( QWheelEvent* event );

private:
    int scalings;
};

#endif // IMAGEDISPLAYER_H
