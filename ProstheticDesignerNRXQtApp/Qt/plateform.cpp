#include "plateform.h"
#include "ui_plateform.h"
#include <QFileDialog>

PlateForm::PlateForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlateForm)
{
    ui->setupUi(this);

    QDir currentDir;

    currentDir.currentPath();

    QString pic_filepath_kinematics;

    pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/Documents/GitHub/ProstheticDesigner/Media/Plates/1-Finger.jpg");

    QPixmap pix (pic_filepath_kinematics);
    ui->label_pic_plate->setPixmap(pix);

    int finger_count = 0;

    if(finger_count == 0){
        ui->checkBox_PinkyFinger->isChecked() ? finger_count++ : finger_count=finger_count+0;
        ui->checkBox_IndexFinger->isChecked() ? finger_count++ : finger_count=finger_count+0;
        ui->checkBox_MiddleFinger->isChecked() ? finger_count++ : finger_count=finger_count+0;
        ui->checkBox_PinkyFinger->isChecked() ? finger_count++ : finger_count=finger_count+0;
    }

    switch(finger_count){
        case 1:
        {
            pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/Documents/GitHub/ProstheticDesigner/Media/Plates/1-Finger.jpg");
            pix.load(pic_filepath_kinematics);
            ui->label_pic_plate->setPixmap(pix);
            break;
        }
        case 2:
        {
            pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/Documents/GitHub/ProstheticDesigner/Media/Plates/2-Finger.jpg");
            pix.load(pic_filepath_kinematics);
            ui->label_pic_plate->setPixmap(pix);
            break;
        }
        case 3:
        {
            pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/Documents/GitHub/ProstheticDesigner/Media/Plates/3-Finger.jpg");
            pix.load(pic_filepath_kinematics);
            ui->label_pic_plate->setPixmap(pix);
            break;
        }
        case 4:
        {
            pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/Documents/GitHub/ProstheticDesigner/Media/Plates/4-Finger.jpg");
            pix.load(pic_filepath_kinematics);
            ui->label_pic_plate->setPixmap(pix);
            break;
        }
        default:
        {
            pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/Documents/GitHub/ProstheticDesigner/Media/Plates/1-Finger.jpg");
            pix.load(pic_filepath_kinematics);
            ui->label_pic_plate->setPixmap(pix);
            break;
        }
    }

        repaint();
}

PlateForm::~PlateForm()
{
    delete ui;
}
