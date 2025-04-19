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
	changeState(pLineEditMainStatus, default);

	pLabelMechanism = new QLabel("Mechanism", this);
	pPushBtnMechanismCreate = new QPushButton("Set", this);
	pLineEditMechanismStatus = new QLineEdit(this);
	pLineEditMechanismStatus->setReadOnly(true);
	changeState(pLineEditMechanismStatus, default);

	pLabelRing = new QLabel("Ring", this);
	pComboBoxRingDiam = new QComboBox(this);
	configureComboBox(pComboBoxRingDiam);
	pLineEditRingStatus = new QLineEdit(this);
	pLineEditRingStatus->setText("Not specified");
	pLineEditRingStatus->setReadOnly(true);
	changeState(pLineEditRingStatus, default);

	pLabelJointDist = new QLabel("Joint Distance");
	pLineEditSetJointDist = new QLineEdit(this);
	pLineEditJointDistStatus = new QLineEdit(this);
	pLineEditJointDistStatus->setReadOnly(true);
	changeState(pLineEditJointDistStatus, default);

	QRegExpValidator* validator = new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this);
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

	pHLayoutRing->addWidget(pLabelRing, 2);
	pHLayoutRing->addWidget(pComboBoxRingDiam, 2);
	pHLayoutRing->addWidget(pLineEditRingStatus, 4);
	layout->addLayout(pHLayoutRing);

	pHLayoutJointDist->addWidget(pLabelJointDist);
	pHLayoutJointDist->addWidget(pLineEditSetJointDist);
	pHLayoutJointDist->addWidget(pLineEditJointDistStatus);
	layout->addLayout(pHLayoutJointDist);

	this->setLayout(layout);

	changeState(pLineEditMainStatus, default);
	changeState(pLineEditRingStatus, default);
	changeState(pLineEditMechanismStatus, default);
	changeState(pLineEditJointDistStatus, default);
	pPushBtnMechanismCreate->setDisabled(true);
	pComboBoxRingDiam->setDisabled(true);
	pLineEditSetJointDist->setReadOnly(true);

	QObject::connect(pComboBoxRingDiam, &QComboBox::currentTextChanged, this, &CustomQGroupBox::ringDiamChanged);
	QObject::connect(pLineEditRingStatus, &QLineEdit::textChanged, this, &CustomQGroupBox::changeStyleCorrect);
	QObject::connect(pLineEditSetJointDist, &QLineEdit::textEdited, this, &CustomQGroupBox::validateJointDist);
	QObject::connect(pCheckBoxFinger, &QCheckBox::stateChanged, this, &CustomQGroupBox::checkBoxFingerChanged);
	QObject::connect(pPushBtnMechanismCreate, &QPushButton::clicked, this, &CustomQGroupBox::setMechanism);

}

CustomQGroupBox::~CustomQGroupBox()
{}

void CustomQGroupBox::setMechanism()
{
	this->pMechanismForm->exec();
}


void CustomQGroupBox::configureComboBox(QComboBox* box)
{
	unsigned int init_size = 12;
	unsigned int max_size = 25;

	for (unsigned int i = init_size; i <= max_size; i++)
	{
		box->addItem(QString::number(i));
	}
}

void changeStyle(QLineEdit* plineEdit, enum errorCode i)
{
	if (i == noError)
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
	if (i == tooLowValue || i == tooBigValue || i == negativeValue)
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
	if (i == default )
	{
		plineEdit->setStyleSheet("QLineEdit { "
			"background-color: black; "
			/*		"border: 2px solid gray; "
					"border-radius: 5px; "
					"padding: 0 8px; "*/
			"selection-background-color: darkgray; "
			/*"font-size: 16px; "
			*/"}");
	}
	if (i == active)
	{
		plineEdit->setStyleSheet("QLineEdit { "
			"background-color: grey; "
			/*		"border: 2px solid gray; "
					"border-radius: 5px; "
					"padding: 0 8px; "*/
			"selection-background-color: darkgray; "
			/*"font-size: 16px; "*/
			"}");
	}


	
}

void CustomQGroupBox::ringDiamChanged()
{
	QString str = "Specified finger size - " + this->pComboBoxRingDiam->currentText();

	this->pLineEditRingStatus->setText(str);

	changeStyle(pLineEditRingStatus, noError);
}

void CustomQGroupBox::changeStyleCorrect()
{
	changeStyle(pLineEditRingStatus, default);
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

	return totalCheck;
}

void CustomQGroupBox::changeState(QLineEdit* plineEdit, enum errorCode state)
{
	if (state == active) 
		plineEdit->setText("Ready to work!");
	if (state == default)
		plineEdit->setText("Not Active!");

	changeStyle(plineEdit, state);
}

void CustomQGroupBox::checkBoxFingerChanged()
{
	if (!this->pCheckBoxFinger->isChecked())
	{
		changeState(pLineEditMainStatus, default);
		changeState(pLineEditRingStatus, default);
		changeState(pLineEditMechanismStatus, default);
		changeState(pLineEditJointDistStatus, default);
		pPushBtnMechanismCreate->setDisabled(true);
		pComboBoxRingDiam->setDisabled(true);
		pLineEditSetJointDist->setReadOnly(true);
	}
	else
	{
		changeState(pLineEditMainStatus, active);
		changeState(pLineEditRingStatus, active);
		changeState(pLineEditMechanismStatus, active);
		changeState(pLineEditJointDistStatus, active);
		pPushBtnMechanismCreate->setDisabled(false);
		pComboBoxRingDiam->setDisabled(false);
		pLineEditSetJointDist->setEnabled(true);
		pLineEditSetJointDist->setReadOnly(false);
	}
}

