#include "stdafx.h"
#include "MechanismForm.h"

#include "QValidator"

#define GET_VARIABLE_NAME(Variable) (#Variable)

MechanismForm::MechanismForm(QWidget *parent)
	: QDialog(parent)
{
    

    this->resize(1003, 747);
    mainHLayout = new QHBoxLayout(this);
    mainHLayout->setSpacing(6);
    mainHLayout->setContentsMargins(11, 11, 11, 11);
    mainHLayout->setObjectName(QString::fromUtf8("horizontalLayout_8"));

    label_Picture = new QLabel(this);
    label_Picture->setObjectName(QString::fromUtf8("label_Picture"));
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_default.jpg")));
    label_Picture->setFixedSize(972, 529);
    label_Picture->setScaledContents(true);
    mainHLayout->addWidget(label_Picture);

    parameterVLayout = new QVBoxLayout(this);
    parameterVLayout->setSpacing(6);
    parameterVLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    hLayout1 = new QHBoxLayout(this);
    label_baseLength = new QLabel(this);
    lineEdit_baseLength = new QLineEdit(this);
    lineEdit_baseLengthStatus = new QLineEdit(this);

    hLayout2 = new QHBoxLayout(this);
    label_r2Length = new QLabel(this);
    lineEdit_r2Length = new QLineEdit(this);
    lineEdit_r2LengthStatus = new QLineEdit(this);

    hLayout3 = new QHBoxLayout(this);
    label_r2HandLength = new QLabel(this);
    lineEdit_r2HandLength = new QLineEdit(this);
    lineEdit_r2HandLengthStatus = new QLineEdit(this);

    hLayout4 = new QHBoxLayout(this);
    label_r3Length = new QLabel(this);
    lineEdit_r3Length = new QLineEdit(this);
    lineEdit_r3LengthStatus = new QLineEdit(this);

    hLayout5 = new QHBoxLayout(this);
    label_middleHandLength = new QLabel(this);
    lineEdit_middleHandLength = new QLineEdit(this);
    lineEdit_middleHandLengthStatus = new QLineEdit(this);

     hLayout6 = new QHBoxLayout(this);
    label_middleBaseLength = new QLabel(this);
    lineEdit_middleBaseLength = new QLineEdit(this);
    lineEdit_middleBaseLengthStatus = new QLineEdit(this);

    hLayout7 = new QHBoxLayout(this);
    label_distalLength = new QLabel(this);
    lineEdit_distalLength = new QLineEdit(this);
    lineEdit_distalLengthStatus = new QLineEdit(this);
    

    configureHLayout(hLayout1, label_baseLength, lineEdit_baseLength, lineEdit_baseLengthStatus, 
        "baseLength");
    configureHLayout(hLayout2, label_r2Length, lineEdit_r2Length, lineEdit_r2LengthStatus,
        "r2Length");
    configureHLayout(hLayout3, label_r2HandLength, lineEdit_r2HandLength, lineEdit_r2HandLengthStatus,
        "r2HandLength");
    configureHLayout(hLayout4, label_baseLength, lineEdit_baseLength, lineEdit_baseLengthStatus,
        "baseLength");
    configureHLayout(hLayout5, label_middleHandLength, lineEdit_middleHandLength, lineEdit_middleHandLengthStatus,
        "middleLength");
    configureHLayout(hLayout6, label_middleBaseLength, lineEdit_middleBaseLength, lineEdit_middleBaseLengthStatus,
        "middleBaseLength");
    configureHLayout(hLayout7, label_distalLength, lineEdit_distalLength, lineEdit_distalLengthStatus,
        "distalLength");

    mainHLayout->addLayout(parameterVLayout);


    QObject::connect(lineEdit_r2Length, &QLineEdit::textEdited, this, &MechanismForm::r2LengthPicked);
    QObject::connect(this->lineEdit_r2HandLength, &QLineEdit::textEdited, this, &MechanismForm::r2HandLengthPicked);
    QObject::connect(this->lineEdit_r3Length, &QLineEdit::textEdited, this, &MechanismForm::r3LengthPicked);
    QObject::connect(this->lineEdit_baseLength, &QLineEdit::textEdited, this, &MechanismForm::baseLengthPicked);
    QObject::connect(this->lineEdit_middleBaseLength, &QLineEdit::textEdited, this, &MechanismForm::middleBaseLengthPicked);
    QObject::connect(this->lineEdit_middleHandLength, &QLineEdit::textEdited, this, &MechanismForm::middleHandLengthPicked);
    QObject::connect(this->lineEdit_distalLength, &QLineEdit::textEdited, this, &MechanismForm::distalLengthPicked);

} // setupUi

MechanismForm::~MechanismForm()
{}

void MechanismForm::retranslateUi()
{
    this->setWindowTitle(QCoreApplication::translate("this", "MechanismForm", nullptr));
    this->label_Picture->setText(QString());
    this->label_baseLength->setText(QCoreApplication::translate("this", "base_length", nullptr));
    this->label_r2Length->setText(QCoreApplication::translate("this", "r2_length", nullptr));
    this->label_r2HandLength->setText(QCoreApplication::translate("this", "r2_HandLength", nullptr));
    this->label_r3Length->setText(QCoreApplication::translate("this", "r3_length", nullptr));
    this->label_middleHandLength->setText(QCoreApplication::translate("this", "middle_hand_length", nullptr));
    this->label_middleBaseLength->setText(QCoreApplication::translate("this", "middle_base_length", nullptr));
    this->label_distalLength->setText(QCoreApplication::translate("this", "distal_length", nullptr));
   // this->pushBtn_Create->setText(QCoreApplication::translate("this", "\320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321\214", nullptr));
} // retranslateUi


void MechanismForm::configureHLayout(QHBoxLayout* layout, QLabel* label, QLineEdit* lineEdit,
    QLineEdit* lineEditStatus, QString title) 
{   
    label->setText(title); 
    QRegExpValidator* validator = new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this);
    lineEdit->setValidator(validator);
    lineEditStatus->setReadOnly(true);
    layout->addWidget(label, 1);
    layout->addWidget(lineEdit, 2);
    layout->addWidget(lineEditStatus, 2);
    this->parameterVLayout->addLayout(layout);
}


void MechanismForm::r2LengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_r2Length.jpg")));
}

void MechanismForm::r2HandLengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_r2HandLength.jpg")));
}

void MechanismForm::r3LengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_r3Length.jpg")));
}

void MechanismForm::baseLengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_baseLength.jpg")));
}

void MechanismForm::middleBaseLengthPicked()
{
    this->label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_middleBaseLength.jpg")));
}

void MechanismForm::middleHandLengthPicked()
{
    this->label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_middleHandLength.jpg")));
}

void MechanismForm::distalLengthPicked()
{
    this->label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_distalLength.jpg")));
}