#include "ringform.h"
#include "ui_ringform.h"

#include <QFileDialog>
#include <QtGui>

RingForm::RingForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RingForm)
{
    ui->setupUi(this);

    QDir currentDir;

    currentDir.currentPath();

    QString pic_filepath_kinematics;

    pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/Documents/GitHub/ProstheticDesigner/Media/Ring.jpg");

    QPixmap pix (pic_filepath_kinematics);
    ui->label_pic_ring->setPixmap(pix);

}

RingForm::~RingForm()
{
    delete ui;
}
