#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->imagesCollection = new ProcessedImagesCollection();

    QSharedPointer<YCrCBProcessor> ycrcbProcessor(new YCrCBProcessor());
    this->imagesCollection->addProcessor(ycrcbProcessor);

    QSharedPointer<YCrCBProcessor> ycrcbProcessor2(new YCrCBProcessor());
    this->imagesCollection->addProcessor(ycrcbProcessor2);

    this->graphicsViews = new QList<QGraphicsView*>();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->imagesCollection;
    delete this->graphicsViews;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        QPixmap newImage;

        if (!newImage.load(fileName)) {
            QMessageBox::warning(this, tr("Open Image"),
                                 tr("The image file could not be loaded."),
                                 QMessageBox::Cancel);
            return;
        }

        this->imagesCollection->setPixmap(newImage);
        this->initGraphicsScene();
    }
}

void MainWindow::initGraphicsScene()
{
    for (int i = 0; i < this->imagesCollection->length(); ++i) {
        QGraphicsView *view = this->createGraphicsScene(this->imagesCollection->at(i));
        this->ui->centralWidget->layout()->addWidget(view);
        view->show();
    }
}

QGraphicsView* MainWindow::createGraphicsScene(const QPixmap &pixmap)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene, this);

    scene->addPixmap(pixmap);

    this->graphicsViews->append(view);

    return view;
}
