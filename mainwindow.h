#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include "processedimagescollection.h"
#include "ramponiprocessor.h"
#include "imagedisplayer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool openImage(QString fileName);
    ~MainWindow();

private slots:
    void on_actionOpen_Image_triggered();

private:

    void initGraphicsScene();
    QGraphicsView* createGraphicsScene(const QPixmap &pixmap);
    Ui::MainWindow *ui;
    ProcessedImagesCollection *imagesCollection;
    QList<ImageDisplayer*> *graphicsViews;
};

#endif // MAINWINDOW_H
