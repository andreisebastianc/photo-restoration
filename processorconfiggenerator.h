#ifndef PROCESSORCONFIGGENERATOR_H
#define PROCESSORCONFIGGENERATOR_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class ProcessorConfigGenerator : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessorConfigGenerator(QWidget *parent = 0, const QVector<QPair<QString, QString>> options = {});
    virtual ~ProcessorConfigGenerator();

private:
    void generateWidgets();
    void addLabelAndInputToLayout(QPair<QString, QString> option, int row = 0);
    QGridLayout *layout = new QGridLayout();
    QPushButton *btn_Process = NULL;
    QVector<QPair<QString, QString>> options;

signals:
    void process(QVector<int> params);

public slots:
    void handleClick();
};

#endif // PROCESSORCONFIGGENERATOR_H
