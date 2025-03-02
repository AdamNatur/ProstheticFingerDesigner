//
// Копирайт (С) 2019, ООО «Нанософт разработка». Все права защищены.
// 
// Данное программное обеспечение, все исключительные права на него, его
// документация и сопроводительные материалы принадлежат ООО «Нанософт разработка».
// Данное программное обеспечение может использоваться при разработке и входить
// в состав разработанных программных продуктов при соблюдении условий
// использования, оговоренных в «Лицензионном договоре присоединения
// на использование программы для ЭВМ «Платформа nanoCAD»».
// 
// Данное программное обеспечение защищено в соответствии с законодательством
// Российской Федерации об интеллектуальной собственности и международными
// правовыми актами.
// 
// Используя данное программное обеспечение,  его документацию и
// сопроводительные материалы вы соглашаетесь с условиями использования,
// указанными выше. 
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

		acutPrintf(L"\nМеханизм успешно построен!\n");

		pMechanismForm->close(); 

	});

	pMechanismForm->exec();

	
//
//	return LineId;
}

void CreateDistalPhalange() {

	
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


		//Boolean Subtraction

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

		acutPrintf(L"\nКольцо успешно построено!\n");
		pRingForm->close();

		});

	pRingForm->exec();

}


NcDb3dSolid* rectangleExtrude(AcDbBlockTableRecord* pBlock, NcGePoint3d initPoint, double length, double width, double height) {

	AcDb3dPolyline* pPolylineBracer = new AcDb3dPolyline();


	pPolylineBracer->appendVertex(new NcDb3dPolylineVertex(NcGePoint3d(initPoint.x, initPoint.y, initPoint.z)));
	pPolylineBracer->appendVertex(new NcDb3dPolylineVertex(NcGePoint3d(initPoint.x, initPoint.y + length, initPoint.z)));
	pPolylineBracer->appendVertex(new NcDb3dPolylineVertex(NcGePoint3d(initPoint.x + width, initPoint.y + length, initPoint.z)));
	pPolylineBracer->appendVertex(new NcDb3dPolylineVertex(NcGePoint3d(initPoint.x + width, initPoint.y, initPoint.z)));

	pPolylineBracer->makeClosed();

	AcDbVoidPtrArray rectContour;

	rectContour.append(pPolylineBracer);

	AcDbVoidPtrArray regionsRect;

	NcDbRegion::createFromCurves(rectContour, regionsRect);

	AcDbRegion* pRegion_bracer = static_cast<NcDbRegion*>(regionsRect[0]);

	pBlock->appendAcDbEntity(pRegion_bracer);

	NcDb3dSolid* pRectSolid = new NcDb3dSolid();

	pRectSolid->extrude(pRegion_bracer, height, 0.0); 

	pBlock->appendNcDbEntity(pRectSolid);

	pRegion_bracer->setVisibility(pRegion_bracer->visibility(), true);

	return pRectSolid;

}

NcDb3dSolid* cylinderExtrude(AcDbBlockTableRecord* pBlock, NcDbCircle* pCircle, double height) {

	AcDbVoidPtrArray circleContour;

	circleContour.append(pCircle);

	AcDbVoidPtrArray regionsCircle;

	NcDbRegion::createFromCurves(circleContour, regionsCircle);

	AcDbRegion* pRegion_circle = static_cast<NcDbRegion*>(regionsCircle[0]);

	pBlock->appendAcDbEntity(pRegion_circle);

	NcDb3dSolid* pCircleSolid = new NcDb3dSolid();

	pCircleSolid->extrude(pRegion_circle, height, 0.0);

	pBlock->appendNcDbEntity(pCircleSolid);

	pRegion_circle->setVisibility(pRegion_circle->visibility(), true);

	return pCircleSolid;
}

void FingerProsthetic_Plate() {

	PlateForm* pPlateForm = new PlateForm(nullptr);

	QObject::connect(pPlateForm->ui.checkBox_Index, &QCheckBox::stateChanged, pPlateForm, &PlateForm::checkBoxIndexFinger);
	QObject::connect(pPlateForm->ui.checkBox_middle, &QCheckBox::stateChanged, pPlateForm, &PlateForm::checkBoxMiddleFinger);
	QObject::connect(pPlateForm->ui.checkBox_Ring, &QCheckBox::stateChanged, pPlateForm, &PlateForm::checkBoxRingFinger);
	QObject::connect(pPlateForm->ui.checkBox_Pinky, &QCheckBox::stateChanged, pPlateForm, &PlateForm::checkBoxPinky);

	QObject::connect(pPlateForm->ui.pushBtn_Create, &QPushButton::clicked, [&]() {

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* pBlock;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
		pBlockTable->close();


		///Plate parameters

		double bracer_width = 15;	/// Bracer width
		double bracer_length = 100;	/// Bracer length

		double finger_width = 18;	// Finger width for bracer

		double mainPlateFinger_width = 80; // Total width of all fingers
		double distanceBtwJoints = (mainPlateFinger_width - finger_width) / 3; // Distance between each joint (equal)

		double bracer_offset = (bracer_length - mainPlateFinger_width) / 2; // Offset from edge of bracer to first finger

		double plate_thickness = 2; // Thickness plate
		double cylinder_height = 4; // Cylinder height
		double hole = 8; // Hole diameter



		NcDb3dSolid* pPlateSolid = new NcDb3dSolid();


		pPlateSolid = rectangleExtrude(pBlock, NcGePoint3d(0, 0, 0), bracer_width, bracer_length, plate_thickness);


		NcGePoint3d fingerJointCenter(0,0,0);


		///Creating finger plates for each finger

		if (pPlateForm->ui.checkBox_Index->isChecked()) {

			fingerJointCenter.set(bracer_offset + finger_width / 2, bracer_width + pPlateForm->ui.lineEdit_IndexFinger->text().toDouble(), 0);

			NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset, bracer_width, 0), pPlateForm->ui.lineEdit_IndexFinger->text().toDouble(), finger_width, plate_thickness);
			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pFingerSolid);

			NcDbCircle* pCircleToAdd = new NcDbCircle(fingerJointCenter, NcGeVector3d(0,0,1), finger_width / 2);

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, pCircleToAdd, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDbCircle* pCircleToCut = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, pCircleToCut, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToCut);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pCylinderToCut);

			pCylinderToCut->close();
			pCylinderToAdd->close();
		}

		if (pPlateForm->ui.checkBox_middle->isChecked()) {

			fingerJointCenter.set(bracer_offset + finger_width / 2 + distanceBtwJoints, bracer_width + pPlateForm->ui.lineEdit_MiddleFinger->text().toDouble(), 0);

			NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset + distanceBtwJoints, bracer_width, 0), pPlateForm->ui.lineEdit_MiddleFinger->text().toDouble(), finger_width, plate_thickness);
			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pFingerSolid);

			NcDbCircle* pCircleToAdd = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2);

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, pCircleToAdd, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDbCircle* pCircleToCut = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, pCircleToCut, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToCut);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pCylinderToCut);

			pCylinderToCut->close();
			pCylinderToAdd->close();

		}

		if (pPlateForm->ui.checkBox_Ring->isChecked()) {

			fingerJointCenter.set(bracer_offset + finger_width / 2 + 2 * distanceBtwJoints, bracer_width + pPlateForm->ui.lineEdit_RingFinger->text().toDouble(), 0);

			NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset + 2 * distanceBtwJoints, bracer_width, 0), pPlateForm->ui.lineEdit_RingFinger->text().toDouble(), finger_width, plate_thickness);
			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pFingerSolid);

			NcDbCircle* pCircleToAdd = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2);

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, pCircleToAdd, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDbCircle* pCircleToCut = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, pCircleToCut, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToCut);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pCylinderToCut);

			pCylinderToCut->close();
			pCylinderToAdd->close();

		}

		if (pPlateForm->ui.checkBox_Pinky->isChecked()) {

			fingerJointCenter.set(bracer_offset + finger_width / 2 + 3 * distanceBtwJoints, bracer_width + pPlateForm->ui.lineEdit_Pinky->text().toDouble(), 0);

			NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset + 3 * distanceBtwJoints, bracer_width, 0), pPlateForm->ui.lineEdit_Pinky->text().toDouble(), finger_width, plate_thickness);
			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pFingerSolid);

			NcDbCircle* pCircleToAdd = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2);

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, pCircleToAdd, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDbCircle* pCircleToCut = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, pCircleToCut, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToCut);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pCylinderToCut);

			pCylinderToCut->close();
			pCylinderToAdd->close();
		}

		double slotOffset_thickness = 4;
		double slot_width = 3;


		NcDb3dSolid* pSlotLeft = rectangleExtrude(pBlock, NcGePoint3d(slotOffset_thickness, slotOffset_thickness, 0), bracer_width - slotOffset_thickness * 2, slot_width, plate_thickness);

		pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pSlotLeft);


		NcDb3dSolid* pSlotRight = rectangleExtrude(pBlock, NcGePoint3d(bracer_length - slotOffset_thickness - slot_width, slotOffset_thickness, 0), bracer_width - slotOffset_thickness * 2, slot_width, plate_thickness);

		pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pSlotRight);

		//const NcArray< NcDbSubentId* >& edgeSubentIds;
		//const NcGeDoubleArray& radius;
		//const NcGeDoubleArray& startSetback;
		//const NcGeDoubleArray& endSetback;

		pBlock->appendAcDbEntity(pPlateSolid);

		////////

		pPlateSolid->close();
		pBlock->close();

		acutPrintf(L"\nПлатформа успешно построена!\n");

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