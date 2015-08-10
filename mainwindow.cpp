#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->imagesCollection = new ProcessedImagesCollection();

    QSharedPointer<RamponiProcessor> ycrcbProcessor(new RamponiProcessor());
    this->imagesCollection->addProcessor(ycrcbProcessor);

    this->graphicsViews = new QList<ImageDisplayer*>();

    this->openImage("/home/andrei/Documents/work/photo-restoration/test.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->imagesCollection;
    delete this->graphicsViews;
}

bool MainWindow::openImage(QString fileName)
{
    QPixmap newImage;

    if (!newImage.load(fileName)) {
        return false;
    }

    this->imagesCollection->setPixmap(newImage);
    this->initGraphicsScene();

    return true;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {

        if (!this->openImage(fileName)) {
            QMessageBox::warning(this, tr("Open Image"),
                                 tr("The image file could not be loaded."),
                                 QMessageBox::Cancel);
            return;
        }
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
    ImageDisplayer *view = new ImageDisplayer(scene, this);

    scene->addPixmap(pixmap);

    this->graphicsViews->append(view);

    return view;
}
