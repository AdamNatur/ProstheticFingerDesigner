#include "stdafx.h"
#include "MechanismForm.h"

#include "QValidator"

#define GET_VARIABLE_NAME(Variable) (#Variable)

MechanismForm::MechanismForm(QWidget *parent)
	: QDialog(parent)
{
    this->resize(900, 450);
    this->setMinimumWidth(900);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    mainHLayout = new QHBoxLayout(this);
    mainHLayout->setSpacing(6);
    mainHLayout->setContentsMargins(11, 11, 11, 11);
    mainHLayout->setObjectName(QString::fromUtf8("horizontalLayout_8"));

    label_Picture = new QLabel(this);
    label_Picture->setObjectName(QString::fromUtf8("label_Picture"));
    QPixmap* pixmap = new QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_default.jpg"));
    pixmap->scaled(label_Picture->size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    label_Picture->setPixmap(*pixmap);
    label_Picture->setFixedSize(this->width()*0.75, this->height() * 0.9);
    label_Picture->setScaledContents(true);
    mainHLayout->addWidget(label_Picture, 3);

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
    configureHLayout(hLayout4, label_r3Length, lineEdit_r3Length, lineEdit_r3LengthStatus,
        "r3Length");
    configureHLayout(hLayout5, label_middleHandLength, lineEdit_middleHandLength, lineEdit_middleHandLengthStatus,
        "middleLength");
    configureHLayout(hLayout6, label_middleBaseLength, lineEdit_middleBaseLength, lineEdit_middleBaseLengthStatus,
        "middleBaseLength");
    configureHLayout(hLayout7, label_distalLength, lineEdit_distalLength, lineEdit_distalLengthStatus,
        "distalLength");

    spacerV = new QSpacerItem(80, 50, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    parameterVLayout->addSpacerItem(spacerV);

    pushBtn_OK = new QPushButton("Apply", this);

    parameterVLayout->addWidget(pushBtn_OK);

    mainHLayout->addLayout(parameterVLayout, 1);


    QObject::connect(this->lineEdit_r2Length, &QLineEdit::textEdited, this, &MechanismForm::r2LengthPicked);
    QObject::connect(this->lineEdit_r2HandLength, &QLineEdit::textEdited, this, &MechanismForm::r2HandLengthPicked);
    QObject::connect(this->lineEdit_r3Length, &QLineEdit::textEdited, this, &MechanismForm::r3LengthPicked);
    QObject::connect(this->lineEdit_baseLength, &QLineEdit::textEdited, this, &MechanismForm::baseLengthPicked);
    QObject::connect(this->lineEdit_middleBaseLength, &QLineEdit::textEdited, this, &MechanismForm::middleBaseLengthPicked);
    QObject::connect(this->lineEdit_middleHandLength, &QLineEdit::textEdited, this, &MechanismForm::middleHandLengthPicked);
    QObject::connect(this->lineEdit_distalLength, &QLineEdit::textEdited, this, &MechanismForm::distalLengthPicked);
    QObject::connect(this->pushBtn_OK, &QPushButton::clicked, this, &MechanismForm::apply);

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
    QRegExpValidator* validator = new QRegExpValidator(QRegExp("^(?:\[0-9]{1,3}|\[0-9]{1,2}[.]\[0-9]{2})$"), this);
    lineEdit->setValidator(validator);
    lineEditStatus->setReadOnly(true);
    layout->addWidget(label, 1);
    layout->addWidget(lineEdit, 3);
    layout->addWidget(lineEditStatus, 5);
    this->parameterVLayout->addLayout(layout);
    this->parameterVLayout->addStretch(1);
}


void MechanismForm::r2LengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_r2Length.jpg")));
    checkR2Length = validation(lineEdit_r2Length, lineEdit_r2LengthStatus, min[GET_VARIABLE_NAME(r2Length)], max[GET_VARIABLE_NAME(r2Length)]);
}

void MechanismForm::r2HandLengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_r2HandLength.jpg")));
    checkR2HandLength = validation(lineEdit_r2HandLength, lineEdit_r2HandLengthStatus, min[GET_VARIABLE_NAME(r2HandLength)], max[GET_VARIABLE_NAME(r2HandLength)]);
}

void MechanismForm::r3LengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_r3Length.jpg")));
    checkR3Length = validation(lineEdit_r3Length, lineEdit_r3LengthStatus, min[GET_VARIABLE_NAME(r3Length)], max[GET_VARIABLE_NAME(r3Length)]);
}

void MechanismForm::baseLengthPicked()
{
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_baseLength.jpg")));
    checkBaseLength = validation(lineEdit_baseLength, lineEdit_baseLengthStatus, min[GET_VARIABLE_NAME(baseLength)], max[GET_VARIABLE_NAME(baseLength)]);
}

void MechanismForm::middleBaseLengthPicked()
{
    this->label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_middleBaseLength.jpg")));
    checkMiddleBaseLength = validation(lineEdit_middleBaseLength, lineEdit_middleBaseLengthStatus, min[GET_VARIABLE_NAME(middleBaseLength)], max[GET_VARIABLE_NAME(middleBaseLength)]);
}

void MechanismForm::middleHandLengthPicked()
{
    this->label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_middleHandLength.jpg")));
    checkMiddleHandLength = validation(lineEdit_middleHandLength, lineEdit_middleHandLengthStatus, min[GET_VARIABLE_NAME(middleHandLength)], max[GET_VARIABLE_NAME(middleHandLength)]);
}

void MechanismForm::distalLengthPicked()
{
    this->label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Mechanism/Mechanism_distalLength.jpg")));
    checkDistalLength = validation(lineEdit_distalLength, lineEdit_distalLengthStatus, min[GET_VARIABLE_NAME(distalLength)], max[GET_VARIABLE_NAME(distalLength)]);

}


int MechanismForm::validation(QLineEdit* pLineEdit, QLineEdit* pLineEditStatus, double min, double max)
{
    double value = pLineEdit->text().toDouble();
    int res = 0;

    if (value < min)
    {
        pLineEditStatus->setText("Too low value!");
        changeStyle(pLineEditStatus, 0);
        res = 1;
    }
    if (value > max)
    {
        pLineEditStatus->setText("Too big value!");
        changeStyle(pLineEditStatus, 0);
        res = 1;
    }
    if (value >= min && value <= max)
    {
        pLineEditStatus->setText("Correct value!");
        changeStyle(pLineEditStatus, 1);
        res = 0;
    }

    if (pLineEdit->text() == "")
    {
        pLineEditStatus->setText("Not specified!");
        changeStyle(pLineEditStatus, 0);
        res = 2;
    }

    return res;
}

void MechanismForm::changeStyle(QLineEdit* plineEdit, int i)
{
    if (i == 0)
    {
        plineEdit->setStyleSheet("QLineEdit { "
            "background-color: red; "
            /*		"border: 2px solid gray; "
                    "border-radius: 5px; "
                    "padding: 0 8px; "*/
            "selection-background-color: darkgray; "
            /*"font-size: 16px; "
            */"}");
    }
    if (i == 1)
    {
        plineEdit->setStyleSheet("QLineEdit { "
            "background-color: green; "
            /*		"border: 2px solid gray; "
                    "border-radius: 5px; "
                    "padding: 0 8px; "*/
            "selection-background-color: darkgray; "
            /*"font-size: 16px; "
            */"}");
    }
    if (i == -1)
    {
        plineEdit->setStyleSheet("QLineEdit { "
            "background-color: grey; "
            /*		"border: 2px solid gray; "
                    "border-radius: 5px; "
                    "padding: 0 8px; "*/
            "selection-background-color: darkgray; "
            /*"font-size: 16px; "
            */"}");
    }
}

void MechanismForm::apply()
{
    int sum = checkBaseLength + checkDistalLength +
        checkMiddleBaseLength + checkMiddleHandLength +
        checkR2HandLength + checkR2Length + checkR3Length;
    if (sum == 0)
        totalValidation = 0;
    else
        totalValidation = 1;

    this->close();
    this->accepted();
}

