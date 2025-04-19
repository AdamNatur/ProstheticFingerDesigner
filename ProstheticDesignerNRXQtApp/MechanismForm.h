#pragma once

#include <QDialog>

class MechanismForm : public QDialog
{
public:
    QHBoxLayout* mainHLayout;
    QLabel* label_Picture;
    QVBoxLayout* parameterVLayout;

    QHBoxLayout* hLayout1;
    QLabel* label_baseLength;
    QLineEdit* lineEdit_baseLength;
    QLineEdit* lineEdit_baseLengthStatus;

    QHBoxLayout* hLayout2;
    QLabel* label_r2Length;
    QLineEdit* lineEdit_r2Length;
    QLineEdit* lineEdit_r2LengthStatus;

    QHBoxLayout* hLayout3;
    QLabel* label_r2HandLength;
    QLineEdit* lineEdit_r2HandLength;
    QLineEdit* lineEdit_r2HandLengthStatus;

    QHBoxLayout* hLayout4;
    QLabel* label_r3Length;
    QLineEdit* lineEdit_r3Length;
    QLineEdit* lineEdit_r3LengthStatus;

    QHBoxLayout* hLayout5;
    QLabel* label_middleHandLength;
    QLineEdit* lineEdit_middleHandLength;
    QLineEdit* lineEdit_middleHandLengthStatus;

    QHBoxLayout* hLayout6;
    QLabel* label_middleBaseLength;
    QLineEdit* lineEdit_middleBaseLength;
    QLineEdit* lineEdit_middleBaseLengthStatus;

    QHBoxLayout* hLayout7;
    QLabel* label_distalLength;
    QLineEdit* lineEdit_distalLength;
    QLineEdit* lineEdit_distalLengthStatus;

    
    //QPushButton* pushBtn_Create;


public:
	MechanismForm(QWidget *parent = nullptr);
	~MechanismForm();

    void MechanismForm::configureHLayout(QHBoxLayout* layout, QLabel* label, QLineEdit* lineEdit,
        QLineEdit* lineEditStatus, QString title);

    void MechanismForm::retranslateUi();

private slots:
    void r2LengthPicked();
    void distalLengthPicked();
    void r2HandLengthPicked();
    void r3LengthPicked();
    void baseLengthPicked();
    void middleBaseLengthPicked();
    void middleHandLengthPicked();

};
