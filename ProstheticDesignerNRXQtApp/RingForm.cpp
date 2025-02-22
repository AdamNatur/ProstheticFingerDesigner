#include "stdafx.h"
#include "RingForm.h"

#include <QFileDialog>

RingForm::RingForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

    QDir currentDir;

    currentDir.currentPath();

    QString pic_filepath_kinematics;

    pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/source/repos/ProstheticDesignerNRXQtApp/Qt/Media/Ring.jpg");

    QPixmap pix(pic_filepath_kinematics); 

    ui.label_Picture->setPixmap(pix);

    inner_diameter = ui.lineEdit->text().toDouble();

    connect(this->ui.Create_pushBtn, &QPushButton::clicked, [&]() {});

}

void RingForm::on_CreateBtn_clicked() {
    inner_diameter = ui.lineEdit->text().toDouble();
    close();
}

RingForm::~RingForm()
{}
