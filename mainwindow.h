#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QSplitter>
#include "processedimagescollection.h"
#include "ramponiprocessor.h"
#include "imagedisplayer.h"
#include "processorconfiggenerator.h"

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
    void onProcess(QVector<int> params);

private:

    void initGraphicsScene();
    void renderConfigWidgets();
    void renderProcessedPixmaps();
    void display(const QPixmap &pixmap);
    void initLayoutWidgets();
    void clearLayout();
    QGraphicsView* createGraphicsScene(const QPixmap &pixmap);
    Ui::MainWindow *ui;
    ProcessorConfigGenerator *configWidget = NULL;
    ProcessedImagesCollection *imagesCollection;
    QList<ImageDisplayer*> *graphicsViews;
    QSplitter *splitter = NULL;
};

#endif // MAINWINDOW_H
