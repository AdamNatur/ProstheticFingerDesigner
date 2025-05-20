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

    int checkBaseLength = 1;
    int checkR2Length = 1;
    int checkR2HandLength = 1;
    int checkR3Length = 1;
    int checkMiddleHandLength = 1;
    int checkMiddleBaseLength = 1;
    int checkDistalLength = 1;

    QPushButton* pushBtn_OK;

    QSpacerItem* spacerV;

    int totalValidation = 1;


public:
	MechanismForm(QWidget *parent = nullptr);
	~MechanismForm();

    void MechanismForm::configureHLayout(QHBoxLayout* layout, QLabel* label, QLineEdit* lineEdit,
        QLineEdit* lineEditStatus, QString title);

    void MechanismForm::retranslateUi();
    int validation(QLineEdit* pLineEdit, QLineEdit* lineEditStatus, double min, double max);
    void changeStyle(QLineEdit* pLineEdit, int i);


    std::map<std::string, double> max{ {"baseLength", 8.5}, {"distalLength", 8.0} ,
    {"r2Length", 73.0}, {"r2HandLength", 9.0}, {"r3Length", 76.0}, {"middleHandLength", 8.0},
    {"middleBaseLength", 18} };

    std::map<std::string, double> min{ {"baseLength", 7.0}, {"distalLength", 6.0} ,
    {"r2Length", 70.0}, {"r2HandLength", 7.0}, {"r3Length", 73.0}, {"middleHandLength", 6.0},
    {"middleBaseLength", 16.0} };

private slots:
    void r2LengthPicked();
    void distalLengthPicked();
    void r2HandLengthPicked();
    void r3LengthPicked();
    void baseLengthPicked();
    void middleBaseLengthPicked();
    void middleHandLengthPicked();
    void apply();

};
