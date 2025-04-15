#include "stdafx.h"
#include "CustomQGroupBox.h"

CustomQGroupBox::CustomQGroupBox(const QString& title, QWidget* parent)
	: QGroupBox(title, parent)
{
	this->setTitle(title);
	this->setWindowTitle(title);

	this->setWindowIconText(title);
	this->setStyleSheet("QGroupBox::title{color: white;}");

	pCheckBoxFinger = new QCheckBox(title, this);
	pLineEditMainStatus = new QLineEdit(this);
	pLineEditMainStatus->setReadOnly(true);

	pLabelMechanism = new QLabel("Mechanism", this);
	pPushBtnMechanismCreate = new QPushButton("Set", this);
	pLineEditMechanismStatus = new QLineEdit(this);
	pLineEditMechanismStatus->setReadOnly(true);

	pLabelRing = new QLabel("Ring", this);
	pPushBtnRingCreate = new QPushButton("Set", this);
	pLineEditRingStatus = new QLineEdit(this);
	pLineEditRingStatus->setReadOnly(true);

	pLabelJointDist = new QLabel("Joint Distance");
	pLineEditSetJointDist = new QLineEdit(this);
	pLineEditJointDistStatus = new QLineEdit(this);
	pLineEditJointDistStatus->setReadOnly(true);

	QDoubleValidator* validator = new QDoubleValidator(this);
	validator->setNotation(QDoubleValidator::StandardNotation);
	pLineEditSetJointDist->setValidator(validator);


	QVBoxLayout* layout = new QVBoxLayout(this);

	pHLayout = new QHBoxLayout(this);
	pHLayoutMechanism = new QHBoxLayout(this);
	pHLayoutRing = new QHBoxLayout(this);
	pHLayoutJointDist = new QHBoxLayout(this);

	pHLayout->addWidget(pCheckBoxFinger);
	pHLayout->addWidget(pLineEditMainStatus);
	layout->addLayout(pHLayout);

	pHLayoutMechanism->addWidget(pLabelMechanism);
	pHLayoutMechanism->addWidget(pPushBtnMechanismCreate);
	pHLayoutMechanism->addWidget(pLineEditMechanismStatus);
	layout->addLayout(pHLayoutMechanism);

	pHLayoutRing->addWidget(pLabelRing);
	pHLayoutRing->addWidget(pPushBtnRingCreate);
	pHLayoutRing->addWidget(pLineEditRingStatus);
	layout->addLayout(pHLayoutRing);

	pHLayoutJointDist->addWidget(pLabelJointDist);
	pHLayoutJointDist->addWidget(pLineEditSetJointDist);
	pHLayoutJointDist->addWidget(pLineEditJointDistStatus);
	layout->addLayout(pHLayoutJointDist);

	this->setLayout(layout);

	QObject::connect(pPushBtnRingCreate, &QPushButton::clicked, this, &CustomQGroupBox::ringBtnClicked);
	QObject::connect(pLineEditRingStatus, &QLineEdit::textChanged, this, &CustomQGroupBox::changeStyleCorrect);
	QObject::connect(pLineEditSetJointDist, &QLineEdit::textEdited, this, &CustomQGroupBox::validateJointDist);
	

}

CustomQGroupBox::~CustomQGroupBox()
{}

void changeStyle(QLineEdit* plineEdit, enum errorCode i)
{
	if (i == noError)
	{
		plineEdit->setStyleSheet("QLineEdit { "
			"background-color: green; "
			/*		"border: 2px solid gray; "
					"border-radius: 5px; "
					"padding: 0 8px; "
					"selection-background-color: darkgray; "
					"font-size: 16px; "
					*/"}");
	}
	if (i == tooLowValue || i == tooBigValue || i == negativeValue)
	{
		plineEdit->setStyleSheet("QLineEdit { "
			"background-color: red; "
			/*		"border: 2px solid gray; "
					"border-radius: 5px; "
					"padding: 0 8px; "
					"selection-background-color: darkgray; "
					"font-size: 16px; "
					*/"}");
	}
}

void CustomQGroupBox::ringBtnClicked() 
{
	this->pRingForm->exec();
	this->pLineEditRingStatus->setText(this->pRingForm->comboBox_InnerDiam->currentText());

}

void::CustomQGroupBox::changeStatus()
{
	QString str = "Specified finger size - " + this->pRingForm->comboBox_InnerDiam->currentText();

	this->pLineEditRingStatus->setText(str);
}

void CustomQGroupBox::changeStyleCorrect()
{
	changeStyle(pLineEditRingStatus, noError);
}

void CustomQGroupBox::validateJointDist()
{
	enum errorCode res = validateJointDistValue((pLineEditSetJointDist->text()).toDouble());

	changeStyle(pLineEditSetJointDist, res);

	if (res == tooBigValue)
	{
		pLineEditJointDistStatus->setText("Value is too big!");
	}
	if (res == tooLowValue)
	{
		pLineEditJointDistStatus->setText("Value is too low!");
	}
	if (res == noError)
	{
		pLineEditJointDistStatus->setText("Value is correct!");
	}
	if (res == negativeValue)
	{
		pLineEditJointDistStatus->setText("Value has not be negative!");
	}
}

enum errorCode CustomQGroupBox::validateJointDistValue(double dist)
{
	double minDist = 40.0;
	double maxDist = 90.0;

	if (dist <= minDist && dist >= 0)
		return tooLowValue;
	if (dist >= maxDist)
		return tooBigValue;
	if (dist < 0)
		return negativeValue;
	if (dist >= minDist && dist <= maxDist)
		return noError;
}

enum totalValidation CustomQGroupBox::totalValidation()
{
	enum totalValidation totalCheck = correct;

	enum errorCode res = validateJointDistValue((pLineEditSetJointDist->text()).toDouble());

	if (res != noError)
		totalCheck = errorExist;
	if (this->pLineEditRingStatus->text() == "")
		totalCheck = notFullyEvaluated;
}

