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

private:
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

private slots:
	void ringBtnClicked();
};
