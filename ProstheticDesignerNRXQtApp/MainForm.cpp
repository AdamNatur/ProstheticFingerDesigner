#include "stdafx.h"
#include "MainForm.h"


MainForm::MainForm(QWidget *parent)
	: QDialog(parent)
{
	this->geometry();
	this->setModal(Qt::WindowModal);
	this->resize(400, 400);

	pMainLayout = new QHBoxLayout(this); 
	pDesignLayout = new QVBoxLayout(this);
	pButtonLayout = new QHBoxLayout(this);

	pCGBoxIndex = new CustomQGroupBox("Index finger", this);
	pCGBoxMiddle = new CustomQGroupBox("Middle finger", this);
	pCGBoxRing = new CustomQGroupBox("Ring finger", this);
	pCGBoxPinky = new CustomQGroupBox("Pinky", this);

	plabel_picture = new QLabel("Picture", this);
	plabel_picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Prosthetic Configurations/1-finger.jpg")));
	plabel_picture->setFixedSize(973, 673);
	plabel_picture->setScaledContents(true);

	pVerticalLine->setFrameShape(QFrame::VLine);  
	pVerticalLine->setFrameShadow(QFrame::Sunken); 
	pVerticalLine->setLineWidth(1);               
	pVerticalLine->setMidLineWidth(1);

	pMainLayout->addWidget(plabel_picture, 3);
	pMainLayout->addWidget(pVerticalLine, 1);

	pDesignLayout->addWidget(pCGBoxIndex);
	pDesignLayout->addWidget(pCGBoxMiddle);
	pDesignLayout->addWidget(pCGBoxRing);
	pDesignLayout->addWidget(pCGBoxPinky);

	pButtonLayout->addWidget(pPushBtnCreate);
	pButtonLayout->addWidget(pPushBtnReject);

	pDesignLayout->addLayout(pButtonLayout);

	pMainLayout->addLayout(pDesignLayout, 2);

	this->setLayout(pMainLayout);

	QObject::connect(this->pCGBoxIndex->pCheckBoxFinger, &QCheckBox::stateChanged, this, &MainForm::func);
	QObject::connect(this->pCGBoxMiddle->pCheckBoxFinger, &QCheckBox::stateChanged, this, &MainForm::func);
	QObject::connect(this->pCGBoxRing->pCheckBoxFinger, &QCheckBox::stateChanged, this, &MainForm::func);
	QObject::connect(this->pCGBoxPinky->pCheckBoxFinger, &QCheckBox::stateChanged, this, &MainForm::func);

	/*ui.setupUi(this);*/
}

MainForm::~MainForm()
{}

void MainForm::func() {

	int finger_amount = this->pCGBoxIndex->pCheckBoxFinger->isChecked() + this->pCGBoxMiddle->pCheckBoxFinger->isChecked() 
		+ this->pCGBoxRing->pCheckBoxFinger->isChecked() + this->pCGBoxPinky->pCheckBoxFinger->isChecked();

	switch (finger_amount) {
	case 1: {
		this->plabel_picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Prosthetic Configurations/1-finger.jpg")));
		break;
	}
	case 2: {
		this->plabel_picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Prosthetic Configurations/2-finger.jpg")));
		break;
	}
	case 3: {
		this->plabel_picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Prosthetic Configurations/3-finger.jpg")));
		break;
	}
	case 4: {
		this->plabel_picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Prosthetic Configurations/4-finger.jpg")));
		break;
	}
	default: {
		this->plabel_picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Prosthetic Configurations/1-finger.jpg")));
		break;
	}
	}

	update();
}
