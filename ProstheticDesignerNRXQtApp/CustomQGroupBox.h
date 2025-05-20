#pragma once

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

#include "MechanismForm.h"

enum errorCode {
	noError,
	Error,
	tooLowValue,
	tooBigValue,
	negativeValue,
	notFull,
	default, /*not active*/
	active

};

enum totalValidation {
	correct,
	errorExist,
	notFullyEvaluated
};

class CustomQGroupBox  : public QGroupBox
{
	Q_OBJECT

public:
	CustomQGroupBox(const QString& title, QWidget* parent);
	~CustomQGroupBox();

public:
	MechanismForm* pMechanismForm = new MechanismForm(this);

public:

	QLabel* pLabelMechanism;
	QPushButton* pPushBtnMechanismCreate;
	QLineEdit* pLineEditMechanismStatus;
	QHBoxLayout* pHLayoutMechanism;

	QLabel* pLabelRing;
	QComboBox* pComboBoxRingDiam; 
	QLineEdit* pLineEditRingStatus;
	QHBoxLayout* pHLayoutRing;

	QLabel* pLabelJointDist;
	QLineEdit* pLineEditSetJointDist;
	QLineEdit* pLineEditJointDistStatus;
	QHBoxLayout* pHLayoutJointDist;

	errorCode ringValidator = noError;

public:
	enum errorCode validateJointDistValue(double dist);
	enum totalValidation totalValidation();
	void changeState(QLineEdit* pLineEdit, enum errorCode i);
	void configureComboBox(QComboBox* box);

private slots:
	void ringDiamChanged();
	void changeStyleCorrect();
	void validateJointDist();
	void checkBoxFingerChanged();
	void setMechanism();
	void checkMechanism();
};




