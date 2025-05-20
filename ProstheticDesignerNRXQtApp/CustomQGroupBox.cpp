#include "stdafx.h"
#include "CustomQGroupBox.h"

CustomQGroupBox::CustomQGroupBox(const QString& title, QWidget* parent)
	: QGroupBox(title, parent)
{
	this->setTitle(title);
 	this->setWindowTitle(title);
	this->setStyleSheet("QGroupBox::title{color: white;}");

	this->setCheckable(true);
	this->setChecked(false);

	pLabelMechanism = new QLabel("Mechanism", this);
	pPushBtnMechanismCreate = new QPushButton("Set", this);
	pLineEditMechanismStatus = new QLineEdit(this);
	pLineEditMechanismStatus->setMinimumWidth(190);
	pLineEditMechanismStatus->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	pLineEditMechanismStatus->setReadOnly(true);
	changeState(pLineEditMechanismStatus, default);

	pLabelRing = new QLabel("Ring", this);
	pComboBoxRingDiam = new QComboBox(this);
	configureComboBox(pComboBoxRingDiam);
	pLineEditRingStatus = new QLineEdit(this);
	pLineEditRingStatus->setText("Not specified");
	pLineEditRingStatus->setReadOnly(true);
	changeState(pLineEditRingStatus, default);

	QRegExpValidator* validator2 = new QRegExpValidator(QRegExp("^(?:\d{1,3}|\d{1,2}[.,]\d{2})$"), this);
	pLabelJointDist = new QLabel("Joint Distance");
	pLineEditSetJointDist = new QLineEdit(this);
	pLineEditSetJointDist->setValidator(validator2);
	pLineEditJointDistStatus = new QLineEdit(this);
	pLineEditJointDistStatus->setReadOnly(true);
	changeState(pLineEditJointDistStatus, default);

	QRegExpValidator* validator = new QRegExpValidator(QRegExp("^(?:\[0-9]{1,3}|\[0-9]{1,2}[.]\[0-9]{2})$"), this);
	pLineEditSetJointDist->setValidator(validator);
	pLineEditSetJointDist->setDisabled(true);

	QVBoxLayout* layout = new QVBoxLayout(this);

	pHLayoutMechanism = new QHBoxLayout(this);
	pHLayoutRing = new QHBoxLayout(this);
	pHLayoutJointDist = new QHBoxLayout(this);

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

	changeState(pLineEditRingStatus, default);
	changeState(pLineEditMechanismStatus, default);
	changeState(pLineEditJointDistStatus, default);
	pPushBtnMechanismCreate->setDisabled(true);
	pComboBoxRingDiam->setDisabled(true);
	pLineEditSetJointDist->setReadOnly(true);

	QObject::connect(pComboBoxRingDiam, &QComboBox::currentTextChanged, this, &CustomQGroupBox::ringDiamChanged);
	QObject::connect(pLineEditRingStatus, &QLineEdit::textChanged, this, &CustomQGroupBox::changeStyleCorrect);
	QObject::connect(pLineEditSetJointDist, &QLineEdit::textEdited, this, &CustomQGroupBox::validateJointDist);
	QObject::connect(this, &QGroupBox::toggled, this, &CustomQGroupBox::checkBoxFingerChanged);
	QObject::connect(pPushBtnMechanismCreate, &QPushButton::clicked, this, &CustomQGroupBox::setMechanism);
	QObject::connect(pMechanismForm, &QDialog::finished, this, &CustomQGroupBox::checkMechanism);
}

CustomQGroupBox::~CustomQGroupBox()
{}

void CustomQGroupBox::setMechanism()
{
	this->pMechanismForm->exec();
}




void CustomQGroupBox::configureComboBox(QComboBox* box)
{
	unsigned int init_size = 18;
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
			"selection-background-color: darkgray; ""}");
	}
	if (i == tooLowValue || i == tooBigValue || i == negativeValue || i == notFull || i == Error)
	{
		plineEdit->setStyleSheet("QLineEdit { "
			"background-color: red; "
			"selection-background-color: darkgray; ""}");
	}
	if (i == default )
	{
		plineEdit->setStyleSheet("");
	}
	if (i == active)
	{
		plineEdit->setStyleSheet("QLineEdit { "
			"background-color: grey; "
			"selection-background-color: darkgray; "
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
	ringValidator = validateJointDistValue((pLineEditSetJointDist->text()).toDouble());

	changeStyle(pLineEditJointDistStatus, ringValidator);

	if (ringValidator == tooBigValue)
	{
		pLineEditJointDistStatus->setText("Value is too big!");
	}
	if (ringValidator == tooLowValue)
	{
		pLineEditJointDistStatus->setText("Value is too low!");
	}
	if (ringValidator == noError)
	{
		pLineEditJointDistStatus->setText("Value is correct!");
	}
	if (ringValidator == negativeValue)
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

	enum errorCode checkRing = validateJointDistValue((pLineEditSetJointDist->text()).toDouble());

	if (checkRing != noError || this->pMechanismForm->totalValidation == 1)
		totalCheck = errorExist;
	if (pLineEditSetJointDist->text() == "" || this->pMechanismForm->totalValidation == 2)
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
	if (!this->isChecked())
	{
		changeState(pLineEditRingStatus, default);
		changeState(pLineEditMechanismStatus, default);
		changeState(pLineEditJointDistStatus, default);
		changeStyle(pLineEditJointDistStatus, default);
		pLineEditSetJointDist->clear();
		pPushBtnMechanismCreate->setDisabled(true);
		pComboBoxRingDiam->setDisabled(true);
		pLineEditSetJointDist->setDisabled(true);
	}
	else
	{
		changeState(pLineEditRingStatus, active);
		changeState(pLineEditMechanismStatus, active);
		changeState(pLineEditJointDistStatus, active);
		pPushBtnMechanismCreate->setDisabled(false);
		pComboBoxRingDiam->setDisabled(false);
		pLineEditSetJointDist->setEnabled(true);
		pLineEditSetJointDist->setReadOnly(false);
	}
}

void CustomQGroupBox::checkMechanism()
{
	switch (this->pMechanismForm->totalValidation) {
	case(0):
		changeStyle(pLineEditMechanismStatus, noError);
		this->pLineEditMechanismStatus->setText("Correct!");
		break;
	case(1):
		changeStyle(pLineEditMechanismStatus, Error);
		this->pLineEditMechanismStatus->setText("Incorrect data!");
		break;
	case(2):
		changeStyle(pLineEditMechanismStatus, notFull);
		this->pLineEditMechanismStatus->setText("Not fully specified!");
		break;
	}
}
