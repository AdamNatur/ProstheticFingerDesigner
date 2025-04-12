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

	

	/*ui.setupUi(this);*/
}

MainForm::~MainForm()
{}
