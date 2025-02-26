#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mechanismform.h"
#include "ringform.h"
#include "plateform.h"
#include <QFileDialog>
#include <QtGui>


QString fileName;

void replaceTab(QTabWidget * tabs, int index,
                QWidget * replacement, const QString & label = QString())
{
    Q_ASSERT(tabs && tabs->count() > index);
    tabs->removeTab(index);
    if (replacement) tabs->insertTab(index, replacement, label);
}

ProstheticDesigner::ProstheticDesigner(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProstheticDesigner)
{
    ui->setupUi(this);

    replaceTab(ui->tabWidget, 0, new MechanismForm(), "Mechanism");
    replaceTab(ui->tabWidget, 1, new RingForm(), "Ring");
    ui->tabWidget->addTab(new PlateForm(), "Plate");
}

ProstheticDesigner::~ProstheticDesigner()
{
    delete ui;
}

// void ProstheticDesigner::on_openBtn_clicked()
// {
//     fileName = QFileDialog::getOpenFileName(this,
//     QString::fromUtf8("Open File"),
//     QDir::currentPath(),
//     "Images (*.stl *.3dm);;All files (*.*)");
//     ui->Filepath->setText(fileName);
// }


void ProstheticDesigner::on_pushButton_clicked()
{
    close();
}

