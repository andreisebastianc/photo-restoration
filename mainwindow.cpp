#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->imagesCollection = new ProcessedImagesCollection();
    this->graphicsViews = new QList<ImageDisplayer*>();
    this->openImage("/home/andrei/Documents/work/photo-restoration/test-2.jpg");
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
    this->clearLayout();
    this->initLayoutWidgets();
    this->renderProcessedPixmaps();
    this->renderConfigWidgets();
    //this->initGraphicsScene();

    return true;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/andrei/Documents/work/photo-restoration", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {

        if (!this->openImage(fileName)) {
            QMessageBox::warning(this, tr("Open Image"),
                                 tr("The image file could not be loaded."),
                                 QMessageBox::Cancel);
            return;
        }
    }
}

void MainWindow::onProcess(QVector<int> params)
{
    Ramponi::Processor* proc = new Ramponi::Processor(params);
    QSharedPointer<Ramponi::Processor> ycrcbProcessor(proc);
    QVector<QPair<QPixmap, QString>> pixmaps = this->imagesCollection->addProcessor(ycrcbProcessor);

    this->display(pixmaps);
    this->renderConfigWidgets();
}

void MainWindow::clearLayout() {
    delete this->ui->centralWidget;
    this->configWidget = NULL;

    QWidget* widget = new QWidget(this);
    this->setCentralWidget(widget);
    this->ui->centralWidget->setLayout(new QGridLayout(widget));
}

void MainWindow::initLayoutWidgets() {
    this->splitter = new QSplitter(this);
    this->splitter->setOrientation(Qt::Vertical);
    this->ui->centralWidget->layout()->addWidget(this->splitter);
}

void MainWindow::renderProcessedPixmaps() {
    for (int i = 0; i < this->imagesCollection->length(); ++i) {
        QGraphicsView *view = this->createGraphicsScene(this->imagesCollection->at(i).first);
        this->splitter->addWidget(view);
        view->show();
    }
}

void MainWindow::renderConfigWidgets() {
    if(this->configWidget){
        disconnect(this->configWidget);
    }
    delete this->configWidget;

    this->configWidget = new ProcessorConfigGenerator(this, Ramponi::Processor::config);
    connect(this->configWidget, &ProcessorConfigGenerator::process, this, &MainWindow::onProcess);
    splitter->addWidget(this->configWidget);
}

void MainWindow::display(const QVector<QPair<QPixmap, QString>> &pairs)
{
    QPair<QPixmap, QString> pair;
    foreach (pair, pairs) {
        QGraphicsScene *scene = new QGraphicsScene(this);
        ImageDisplayer *view = new ImageDisplayer(scene, this);

        scene->addPixmap(pair.first);
        scene->addText(pair.second)->setDefaultTextColor(Qt::red);

        this->graphicsViews->append(view);
        splitter->addWidget(view);
        view->show();
    }
}

QGraphicsView* MainWindow::createGraphicsScene(const QPixmap &pixmap)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    ImageDisplayer *view = new ImageDisplayer(scene, this);

    scene->addPixmap(pixmap);
    scene->addText("ORIGINAL")->setDefaultTextColor(Qt::red);

    this->graphicsViews->append(view);

    return view;
}
