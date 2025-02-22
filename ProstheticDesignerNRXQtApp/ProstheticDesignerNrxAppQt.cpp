//
// �������� (�) 2019, ��� ��������� ����������. ��� ����� ��������.
// 
// ������ ����������� �����������, ��� �������������� ����� �� ����, ���
// ������������ � ���������������� ��������� ����������� ��� ��������� ����������.
// ������ ����������� ����������� ����� �������������� ��� ���������� � �������
// � ������ ������������� ����������� ��������� ��� ���������� �������
// �������������, ����������� � ������������� �������� �������������
// �� ������������� ��������� ��� ��� ���������� nanoCAD��.
// 
// ������ ����������� ����������� �������� � ������������ � �����������������
// ���������� ��������� �� ���������������� ������������� � ��������������
// ��������� ������.
// 
// ��������� ������ ����������� �����������,  ��� ������������ �
// ���������������� ��������� �� ������������ � ��������� �������������,
// ���������� ����. 
//

#include "stdafx.h"

#include "hostQtUi.h"
#include "qwinwidget.h"
#include "RingForm.h"
#include "MechanismForm.h"
#include "PlateForm.h"
#include "resource.h"		// main symbols
#include "arxHeaders.h"
#include "AcExtensionModule.h"
#include <math.h>
#include <QEventLoop>

extern "C" __declspec(dllexport) void showDialog(HWND parent)
{
  QWinWidget win(parent);
  win.showCentered();

  QMessageBox::about(&win, "ProstheticDesigner.dll", "Hello, ProstheticDesigner is added in nanoCAD!");
}

void addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity)
{
	AcDbBlockTable* pBlockTable;
	AcDbBlockTableRecord* pBlock;

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);

	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
	pBlockTable->close();

	pBlock->appendAcDbEntity(objId, pEntity);
	pBlock->close();
}

void FingerProsthetic_Mechanism() {

	MechanismForm* pMechanismForm = new MechanismForm(nullptr);

	QObject::connect(pMechanismForm->ui.pushBtn_Create, &QPushButton::clicked, [&]() {

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* pBlock;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
		pBlockTable->close();


		double base_angle = 60;
		double initial_angle = 5;
		double base_length = (pMechanismForm->ui.lineEdit_baseLength->text()).toDouble(); // default = 8
		double r2_length = (pMechanismForm->ui.lineEdit_r2Length->text()).toDouble(); //default = 71.56;
		double r2_hand_length = (pMechanismForm->ui.lineEdit_r2HandLength->text()).toDouble(); //default = 8;
		double r3_length = (pMechanismForm->ui.lineEdit_r3Length->text()).toDouble(); // default = 74.12;
		double middle_hand_length = (pMechanismForm->ui.lineEdit_middleHandLength->text()).toDouble(); //default = 7.5;
		double middle_base_length = (pMechanismForm->ui.lineEdit_middleBaseLength->text()).toDouble(); //default = 17;
		double distal_length = (pMechanismForm->ui.lineEdit_distalLength->text()).toDouble(); //default = 7
		double middle_angle = 85;

		NcGePoint3d origin(0, 0, 0);
		NcGePoint3d point_A(base_length * cos(base_angle * PI / 180), base_length * sin(base_angle * PI / 180), 0);
		NcGePoint3d point_I(r2_length * cos(initial_angle * PI / 180), -r2_length * sin(initial_angle * PI / 180), 0);
		NcGePoint3d point_D(r2_length * cos(initial_angle * PI / 180) + 0.5, -r2_length * sin(initial_angle * PI / 180) - r2_hand_length, 0);
		NcGePoint3d point_C(r2_length * cos(initial_angle * PI / 180), -r2_length * sin(initial_angle * PI / 180) - middle_hand_length, 0);
		NcGePoint3d point_G(r2_length * cos(initial_angle * PI / 180) + middle_base_length, -r2_length * sin(initial_angle * PI / 180) - middle_hand_length, 0);
		NcGePoint3d point_F(r2_length * cos(initial_angle * PI / 180) + middle_base_length + 0.5, -r2_length * sin(initial_angle * PI / 180) - middle_hand_length + distal_length, 0);



		//NcDbPoint* point = new NcDbPoint(endPt);
		NcDbLine* pLine_base = new NcDbLine(origin, point_A);
		NcDbLine* pLine_r2 = new NcDbLine(origin, point_I);
		NcDbLine* pLine_r2_hand = new NcDbLine(point_I, point_D);
		NcDbLine* pLine_r3 = new NcDbLine(point_A, point_C);
		NcDbLine* pLine_driver = new NcDbLine(point_D, point_F);
		NcDbLine* pLine_distal = new NcDbLine(point_F, point_G);
		NcDbLine* pLine_middle_base = new NcDbLine(point_C, point_G);
		NcDbLine* pLine_middle_hand = new NcDbLine(point_I, point_C);


		AcDbObjectId LineId;

		pBlock->appendAcDbEntity(LineId, pLine_base);
		pBlock->appendAcDbEntity(LineId, pLine_r2);
		pBlock->appendAcDbEntity(LineId, pLine_r2_hand);
		pBlock->appendAcDbEntity(LineId, pLine_r3);
		pBlock->appendAcDbEntity(LineId, pLine_driver);
		pBlock->appendAcDbEntity(LineId, pLine_distal);
		pBlock->appendAcDbEntity(LineId, pLine_middle_base);
		pBlock->appendAcDbEntity(LineId, pLine_middle_hand);

		pLine_base->close();
		pLine_r2->close();

		pLine_base->close();
		pLine_r2->close();
		pLine_r2_hand->close();
		pLine_r3->close();
		pLine_driver->close();
		pLine_distal->close();
		pLine_middle_base->close();
		pLine_middle_hand->close();
		pBlock->close();

		acutPrintf(L"\n�������� ������� ��������!\n");

		pMechanismForm->close(); 

	});

	pMechanismForm->exec();

	
//
//	return LineId;
}

void FingerProsthetic_Ring() {

	RingForm* pRingForm = new RingForm(nullptr);

	QObject::connect(pRingForm->ui.Create_pushBtn, &QPushButton::clicked, [&]() {

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* pBlock;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
		pBlockTable->close();

		nds_real inner_diameter = (pRingForm->ui.lineEdit->text()).toDouble();
		nds_real ring_thickness = 3;
		nds_real offset = 1;
		nds_real fixation_width = inner_diameter / 2;

		nds_real outer_diameter = inner_diameter + 2 * ring_thickness;

		AcGeVector3d normal(0.0, 0.0, 1.0);

		AcGePoint3d center(0.0, 0.0, 0.0);

		AcDbCircle* pCirc_inner = new AcDbCircle(center, normal, inner_diameter / 2);


		////Intersection outer_diam

		double intersect_point1_x = offset;
		double intersect_point1_y = sqrt(pow(outer_diameter / 2, 2) - pow(intersect_point1_x, 2));
		double intersect_point2_x = offset - fixation_width;
		double intersect_point2_y = sqrt(pow(outer_diameter / 2, 2) - pow(intersect_point2_x, 2));

		/////////

		double outer_arc_startAngle = atan(intersect_point1_y / intersect_point1_x);
		double outer_arc_endAngle = PI - abs(atan(intersect_point2_y / intersect_point2_x));

		//for adding arc type we need - start angle has to be bigger than end angle

		if (offset < 0) {
			AcDbArc* pArc = new AcDbArc(center, outer_diameter / 2, outer_arc_endAngle, outer_arc_startAngle);
		}

		AcDbArc* pArc = new AcDbArc(center, outer_diameter / 2, outer_arc_endAngle, outer_arc_startAngle);


		AcDb3dPolyline* pPolyline = new AcDb3dPolyline();


		double fixation_height = outer_diameter / 2 + 6;
		double slot_width = 4;
		double main_thickness = (fixation_width - slot_width) / 2;

		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point1_x, intersect_point1_y, 0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point1_x, fixation_height, 0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point1_x - main_thickness, fixation_height, 0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point1_x - main_thickness, fixation_height - main_thickness, 0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point1_x - main_thickness - slot_width, fixation_height - main_thickness, 0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point1_x - main_thickness - slot_width, fixation_height, 0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point2_x, fixation_height, 0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(intersect_point2_x, intersect_point2_y, 0))));

		AcDbObjectId circleId;
		AcDbObjectId arcId;
		AcDbObjectId polylineId;

		pBlock->appendAcDbEntity(circleId, pCirc_inner);
		pBlock->appendAcDbEntity(arcId, pArc);
		pBlock->appendAcDbEntity(polylineId, pPolyline);


		//Adding lines to common curve array

		NcDbVoidPtrArray curves;
		curves.append(pPolyline);
		curves.append(pArc);

		///////////////////////


		//Create main sketch to extrude

		NcDbVoidPtrArray Regions;

		NcDbRegion::createFromCurves(curves, Regions);

		NcDbObject* region = new NcDbRegion();

		AcDbRegion* pRegion1 = static_cast<AcDbRegion*>(Regions[0]);

		NRX::Boolean subents1 = true;

		pRegion1->setVisibility(pRegion1->visibility(), subents1);

		////////////////////////////////////////////


		//Hole to do

		AcDbVoidPtrArray curves1;

		curves1.append(pCirc_inner);

		AcDbVoidPtrArray Regions1;

		NcDbRegion::createFromCurves(curves1, Regions1);

		NcDbObject* region2 = new NcDbRegion();

		AcDbRegion* pRegion2 = static_cast<AcDbRegion*>(Regions1[0]);

		NRX::Boolean subents2 = true;

		pRegion2->setVisibility(pRegion2->visibility(), subents2);


		//////////////


		//Create solid 

		NcDb3dSolid* pSolid1 = new NcDb3dSolid();

		double height = 10;
		double taperAngle = 0;

		pSolid1->extrude(pRegion1, height, taperAngle);

		AcDbObjectId SolidId;


		NcDb3dSolid* pSolid2 = new NcDb3dSolid();

		//Create hole

		pSolid2->extrude(pRegion2, height, taperAngle);

		AcDbObjectId HoleId;

		pBlock->appendAcDbEntity(HoleId, pSolid2);

		/////////


		//Boolean Substraction

		pSolid1->booleanOper(NcDb::BoolOperType::kBoolSubtract, pSolid2);

		pBlock->appendAcDbEntity(SolidId, pSolid1);

		/////////////////

		pBlock->close();
		pCirc_inner->close();
		pArc->close();
		pPolyline->close();
		pRegion1->close();
		pSolid1->close();
		pRegion2->close();
		pSolid2->close();

		acutPrintf(L"\n������ ������� ���������!\n");
		pRingForm->close();

		});

	pRingForm->exec();

}

void FingerProsthetic_Plate() {

	PlateForm* pPlateForm = new PlateForm(nullptr);

	QObject::connect(pPlateForm->ui.checkBox_Index, &QCheckBox::stateChanged, pPlateForm, &PlateForm::func);
	QObject::connect(pPlateForm->ui.checkBox_middle, &QCheckBox::stateChanged, pPlateForm, &PlateForm::func);
	QObject::connect(pPlateForm->ui.checkBox_Ring, &QCheckBox::stateChanged, pPlateForm, &PlateForm::func);
	QObject::connect(pPlateForm->ui.checkBox_Pinky, &QCheckBox::stateChanged, pPlateForm, &PlateForm::func);

	QObject::connect(pPlateForm->ui.pushBtn_Create, &QPushButton::clicked, [&]() {


		if (pPlateForm->finger_amount == 4) {
			pPlateForm->ui.label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Plates/4-Finger.jpg")));
			pPlateForm->update();
		}

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* pBlock;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
		pBlockTable->close();


		///Plate parameters

		double bracer_width = 15;
		double bracer_offset = 10;
		double finger_width = 16;
		double space_btw_fingers = 16;


		bool index_finger = 0;
		bool middle_finger = 0;
		bool ring_finger = 0;
		bool pinky = 0;

		double finger_length = 45;

		//////////////////////

		AcDb3dPolyline* pPolyline = new AcDb3dPolyline();

		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(0.0, 0.0, 0.0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(0, bracer_width, 0))));

		double current_x = bracer_offset;
		double current_y = bracer_width;

		///Creating finger plates for each finger

		for (int i = 1; i <= pPlateForm->finger_amount; i++) {

			pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(current_x, current_y, 0.0))));
			pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(current_x, current_y + finger_length, 0.0))));

			current_x += finger_width;

			pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(current_x, current_y + finger_length, 0.0))));
			pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(current_x, current_y, 0.0))));

			if (i != pPlateForm->finger_amount) {
				current_x += space_btw_fingers;
			}
		}

		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(current_x + bracer_offset, bracer_width, 0.0))));
		pPolyline->appendVertex(new AcDb3dPolylineVertex(*(new AcGePoint3d(current_x + bracer_offset, 0, 0))));

		pPolyline->makeClosed();

		AcDbObjectId polylineId;

		pBlock->appendAcDbEntity(polylineId, pPolyline);

		//////////////////////////////

		AcDbVoidPtrArray curves_plate;

		curves_plate.append(pPolyline);

		AcDbVoidPtrArray Regions;

		NcDbRegion::createFromCurves(curves_plate, Regions);


		AcDbRegion* pRegion_plate = static_cast<NcDbRegion*>(Regions[0]);

		AcDbObjectId RegionId;

		pBlock->appendAcDbEntity(RegionId, pRegion_plate);

		NRX::Boolean subents = true;

		pRegion_plate->setVisibility(pRegion_plate->visibility(), subents);


		//Create solid 

		double height = 5;
		double taperAngle = 0.0;

		NcDb3dSolid* pSolid_plate = new NcDb3dSolid();

		pSolid_plate->extrude(pRegion_plate, height, taperAngle);

		//AcDbVoidPtrArray entitySet;

		//pSolid_plate->explode(entitySet);

		//AcDbEntity* pEntuty;

		//pEntuty = static_cast <AcDbEntity*> (entitySet[0]);

		////pEntuty = AcDbEntity::cast(entitySet[0]);

		AcDbObjectId Solid_PlateId;

		pBlock->appendAcDbEntity(Solid_PlateId, pSolid_plate);


		////////

		pPolyline->close();
		pRegion_plate->close();
		pSolid_plate->close();
		pBlock->close();

		acutPrintf(L"\n��������� ������� ���������!\n");

		pPlateForm->close();

		});

		pPlateForm->exec();

	
}

void initApp() {
	ncedRegCmds->addCommand(L"ProstheticDesigner_GROUP",
		L"_FingerProsthetic_Mechanism",
		L"FingerProsthetic_Mechanism",
		ACRX_CMD_TRANSPARENT,
		FingerProsthetic_Mechanism);
	ncedRegCmds->addCommand(L"ProstheticDesigner_GROUP",
		L"_FingerProsthetic_Ring",
		L"FingerProsthetic_Ring",
		ACRX_CMD_TRANSPARENT,
		FingerProsthetic_Ring);
	ncedRegCmds->addCommand(L"ProstheticDesinger_GROUP",
		L"_FingerProsthetic_Plate",
		L"FingerProsthetic_Plate",
		ACRX_CMD_TRANSPARENT,
		FingerProsthetic_Plate);
}

void uninitApp()
{
  ncedRegCmds->removeGroup(L"ProstheticDesigner_GROUP");
}

extern "C" __declspec(dllexport) NcRx::AppRetCode
ncrxEntryPoint(NcRx::AppMsgCode msg, void* appId) 
{
  switch (msg) 
  {
  case NcRx::kInitAppMsg:
    ncrxDynamicLinker->unlockApplication(appId);
    ncrxDynamicLinker->registerAppMDIAware(appId);
    initApp();
    break;

  case NcRx::kUnloadAppMsg:
    uninitApp();
    break;
  }

  return NcRx::kRetOK;
}