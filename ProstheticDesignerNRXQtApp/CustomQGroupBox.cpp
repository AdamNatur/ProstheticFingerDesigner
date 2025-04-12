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

}

CustomQGroupBox::~CustomQGroupBox()
{}

void CustomQGroupBox::ringBtnClicked() 
{
	this->pRingForm->exec();
}