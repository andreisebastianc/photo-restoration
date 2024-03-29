#include "processorconfiggenerator.h"

ProcessorConfigGenerator::ProcessorConfigGenerator(QWidget *parent, const QVector<QPair<QString, QString> > options) : QWidget(parent)
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

void ProcessorConfigGenerator::addLabelAndInputToLayout(QPair<QString, QString> option, int row)
{
    QLabel *label = new QLabel(this);
    QLineEdit *input = new QLineEdit(this);
    input->setText(option.second);
    label->setText(option.first);
    label->setBuddy(input);

    layout->addWidget(label, row, 0);
    layout->addWidget(input, row, 1);
}

void ProcessorConfigGenerator::handleClick() {
    QVector<int> results;
    QList<QLineEdit*> widgets = this->findChildren<QLineEdit*>();

    // @todo this doesn't work
    for (int i = 0; i < widgets.length(); ++i) {
        int result = widgets.at(i)->text().toFloat();
        results << result;
    }

    emit process(results);
}
