#include "mechanismform.h"
#include "ui_mechanismform.h"

#include <QFileDialog>
#include <QtGui>

MechanismForm::MechanismForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MechanismForm)
{
    ui->setupUi(this);

    QDir currentDir;

    currentDir.currentPath();

    QString pic_filepath_kinematics;

    pic_filepath_kinematics = currentDir.relativeFilePath("C:/Users/adamn/source/repos/ProstheticDesignerNRXQtApp/Qt/Media/Mechanism.jpg");

    QPixmap pix (pic_filepath_kinematics);
    ui->label_pic_kinematic->setPixmap(pix);

    QDoubleValidator *validate_length = new QDoubleValidator(0, 200, 2, this);
    validate_length->setNotation(QDoubleValidator::StandardNotation);
    QDoubleValidator *validate_angle = new QDoubleValidator(0, 90, 2, this);
    validate_angle->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEdit_ActiveAngle->setValidator(validate_angle);
    //ui->lineEdit_ActiveAngle->setInputMask("0 - 90 degrees");

    ui->lineEdit_base_angle->setValidator(validate_angle);
    //ui->lineEdit_base_angle->setInputMask("0 - 90 degrees");

    ui->lineEdit_base_distance->setValidator(validate_length);
    //ui->lineEdit_base_distance->setInputMask("0 - 200 mm");

    ui->lineEdit_r2->setValidator(validate_length);
    //ui->lineEdit_r2->setInputMask("0 - 200 mm");

    ui->lineEdit_r3->setValidator(validate_length);
    //ui->lineEdit_r3->setInputMask("0 - 200 mm");

    ui->lineEdit_r4->setValidator(validate_length);
    //ui->lineEdit_r4->setInputMask("0 - 200 mm");

    ui->lineEdit_r5->setValidator(validate_length);
    //ui->lineEdit_r5->setInputMask("0 - 200 mm");

    ui->lineEdit_r6->setValidator(validate_length);
    //ui->lineEdit_r6->setInputMask("0 - 200 mm");

    ui->lineEdit_mid_angle->setValidator(validate_length);
    //ui->lineEdit_mid_angle->setInputMask("0 - 90 degrees");

    ui->lineEdit_init_mid_dist->setValidator(validate_length);
    //ui->lineEdit_init_mid_dist->setInputMask("0 - 200 mm");

    ui->lineEdit_distal_distance->setValidator(validate_length);
    //ui->lineEdit_distal_distance->setInputMask("0 - 200 mm");


}

MechanismForm::~MechanismForm()
{
    delete ui;
}



void MechanismForm::on_radBtnAutoSet_clicked()
{
    ui->lineEdit_ActiveAngle->setEnabled(true);
    ui->lineEdit_base_angle->setDisabled(true);
    ui->lineEdit_base_distance->setDisabled(true);
    ui->lineEdit_r2->setDisabled(true);
    ui->lineEdit_r3->setDisabled(true);
    ui->lineEdit_r4->setDisabled(true);
    ui->lineEdit_r5->setDisabled(true);
    ui->lineEdit_r6->setDisabled(true);
    ui->lineEdit_mid_angle->setDisabled(true);
    ui->lineEdit_init_mid_dist->setDisabled(true);
    ui->lineEdit_distal_distance->setDisabled(true);


}


void MechanismForm::on_radBtnCustomSet_clicked()
{
    ui->lineEdit_base_angle->setEnabled(true);
    ui->lineEdit_base_distance->setEnabled(true);
    ui->lineEdit_r2->setEnabled(true);
    ui->lineEdit_r3->setEnabled(true);
    ui->lineEdit_r4->setEnabled(true);
    ui->lineEdit_r5->setEnabled(true);
    ui->lineEdit_r6->setEnabled(true);
    ui->lineEdit_mid_angle->setEnabled(true);
    ui->lineEdit_init_mid_dist->setEnabled(true);
    ui->lineEdit_distal_distance->setEnabled(true);
    ui->lineEdit_ActiveAngle->setDisabled(true);
}


void MechanismForm::on_pushBtn_Export_clicked()
{
    QString fileName;
    const QString &filter = "STEP2104(*.step);;Drawings (*.dwg);;All(*.*)";
    fileName = QFileDialog::getOpenFileName(this,
    QString::fromUtf8("Open File"),
    QDir::currentPath(),
    filter);
    ui->lineEdit_Filepath_mechanism->setText(fileName);
}

