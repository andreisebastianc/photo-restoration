#include "processorconfiggenerator.h"

ProcessorConfigGenerator::ProcessorConfigGenerator(QWidget *parent, const QVector<QString> options) : QWidget(parent)
{
    this->options = options;
    setLayout(this->layout);
    generateWidgets();
}

ProcessorConfigGenerator::~ProcessorConfigGenerator()
{
    delete layout;
}

void ProcessorConfigGenerator::generateWidgets()
{
    for (int i = 0; i < options.length(); ++i) {
        addLabelAndInputToLayout(options.at(i), i);
    }

    btn_Process = new QPushButton("&Process", this);
    connect(btn_Process, SIGNAL(released()), this, SLOT(handleClick()));
    layout->addWidget(btn_Process);
}

void ProcessorConfigGenerator::addLabelAndInputToLayout(QString text, int row)
{
    QLabel *label = new QLabel(this);
    QLineEdit *input = new QLineEdit(this);
    input->setText("1");
    label->setText(text);
    label->setBuddy(input);

    layout->addWidget(label, row, 0);
    layout->addWidget(input, row, 1);
}

void ProcessorConfigGenerator::handleClick() {
    QVector<QPair<QString, int> > results;
    QList<QLineEdit*> widgets = this->layout->findChildren<QLineEdit*>();

    // @todo this doesn't work
    for (int i = 0; i < widgets.length(); ++i) {
        QPair<QString, int> result;
        result.first = options.at(i);
        result.second = widgets.at(i)->text().toInt();
        results << result;
    }

    emit process(results);
}
