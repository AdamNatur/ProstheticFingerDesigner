#pragma once

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

#include "RingForm.h"


class CustomQGroupBox  : public QGroupBox
{
	Q_OBJECT

public:
	CustomQGroupBox(const QString& title, QWidget* parent);
	~CustomQGroupBox();

public:
	RingForm* pRingForm = new RingForm(this);

public:
	QCheckBox* pCheckBoxFinger;
	QLineEdit* pLineEditMainStatus;
	QHBoxLayout* pHLayout;

	QLabel* pLabelMechanism;
	QPushButton* pPushBtnMechanismCreate;
	QLineEdit* pLineEditMechanismStatus;
	QHBoxLayout* pHLayoutMechanism;

	QLabel* pLabelRing;
	QPushButton* pPushBtnRingCreate;
	QLineEdit* pLineEditRingStatus;
	QHBoxLayout* pHLayoutRing;

	QLabel* pLabelJointDist;
	QLineEdit* pLineEditSetJointDist;
	QLineEdit* pLineEditJointDistStatus;
	QHBoxLayout* pHLayoutJointDist;

public:
	enum errorCode validateJointDistValue(double dist);
	enum totalValidation totalValidation();

private slots:
	void ringBtnClicked();
	void changeStatus();
	void changeStyleCorrect();
	void validateJointDist();
};

enum errorCode {
	noError,
	tooLowValue,
	tooBigValue,
	negativeValue,
};

enum totalValidation {
	correct,
	errorExist,
	notFullyEvaluated
};