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

NcDb3dSolid* cylinderExtrude(AcDbBlockTableRecord* pBlock, NcGePoint3d point, NcGeVector3d normal, double radius, double height) {

	NcDbCircle* pCircle = new NcDbCircle(point, normal, radius);
	
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

NcDb3dSolid* slotExtrude(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDist, double radius, double orientAngle, NcGeVector3d vector, double height) {

	NcDb3dSolid* pSlot = cylinderExtrude(pBlock, startPoint, vector, radius, height);

	NcDb3dSolid* pRect = rectangleExtrude(pBlock, NcGePoint3d(startPoint.x, startPoint.y - radius, startPoint.z), 2 * radius, centerDist, height);

	pSlot->booleanOper(NcDb::kBoolUnite, pRect);

	NcDb3dSolid* pCylinder = cylinderExtrude(pBlock, NcGePoint3d(startPoint.x + centerDist, startPoint.y, startPoint.z), vector, radius, height);

	pSlot->booleanOper(NcDb::kBoolUnite, pCylinder);

	NcGeMatrix3d* rotateMatrix = new NcGeMatrix3d();

	rotateMatrix->setToRotation(orientAngle, vector, NcGePoint3d(startPoint));

	pSlot->transformBy(*rotateMatrix);

	pBlock->appendNcDbEntity(pSlot);

	return pSlot;
}

void midPhalanx_base(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDistVert, double centerDistHoriz, double radius, double thickness) {

	NcDb3dSolid* pBase = slotExtrude(pBlock, startPoint, centerDistVert, radius, -PI / 2, NcGeVector3d(0, 0, 1), thickness);

	NcDb3dSolid* pBaseHorizontal = slotExtrude(pBlock, NcGePoint3d(startPoint.x, startPoint.y - centerDistVert, -thickness / 2), centerDistHoriz, radius, 0, NcGeVector3d(0, 0, 1), thickness);

	pBase->booleanOper(NcDb::kBoolUnite, pBaseHorizontal);

	NcDb3dSolid* pCircle = cylinderExtrude(pBlock, startPoint, NcGeVector3d(0, 0, 1), radius, 8.0);

	pBase->booleanOper(NcDb::kBoolUnite, pCircle);


	NcDb3dSolid* pHoleDown = cylinderExtrude(pBlock, NcGePoint3d(startPoint.x, startPoint.y - centerDistVert, -thickness / 2), NcGeVector3d(0,0,1), 1.0, thickness);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleDown);

	NcDb3dSolid* pHoleHorizontal = cylinderExtrude(pBlock, NcGePoint3d(startPoint.x + centerDistHoriz, startPoint.y - centerDistVert, -thickness / 2), NcGeVector3d(0, 0, 1), 1.0, thickness);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleHorizontal);

	NcDb3dSolid* pHoleUp = cylinderExtrude(pBlock, startPoint, NcGeVector3d(0, 0, 1), 1.0, 8.0);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleUp);


	pBlock->appendNcDbEntity(pBase);

	pBase->close();

}

void midPhalanx_distal(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDist, double radius, double thickness) {

	NcDb3dSolid* pDistal = slotExtrude(pBlock, startPoint, 8.0, 1.5, PI / 2, NcGeVector3d(0, 0, 1), thickness);

	double offset_y = 3;
	double plate_length = 3;
	double plate_width = 2;
	double hole_diam = 2;

	NcDb3dSolid* pRect = rectangleExtrude(pBlock, NcGePoint3d(radius, offset_y, -thickness / 2), plate_width, plate_length, thickness);

	pDistal->booleanOper(NcDb::kBoolUnite, pRect);

	NcDb3dSolid* pHoleDown = cylinderExtrude(pBlock, startPoint, NcGeVector3d(0, 0, 1), hole_diam / 2, thickness);

	pDistal->booleanOper(NcDb::kBoolSubtract, pHoleDown);

	NcDb3dSolid* pHoleUp = cylinderExtrude(pBlock, NcGePoint3d(startPoint.x, startPoint.y + centerDist, startPoint.z), NcGeVector3d(0, 0, 1), hole_diam / 2, thickness);

	pDistal->booleanOper(NcDb::kBoolSubtract, pHoleUp);

	pBlock->appendNcDbEntity(pDistal);

	pDistal->close();

}

void createBase(AcDbBlockTableRecord* pBlock, double base_angle, double base_length) {

	double main_width = 16;
	double main_offset = 4;
	double main_thickness = 3;
	double cylinder_diam = 6;
	double offset = 9.5;
	double thickness = 3;
	double leftCut_depth = 3.5;

	double functional_dist = 10;

	double counterbore_diam = 14;
	double counterbore_depth = 2.5;

	double centralHole_diam = 12;
	double functionalHole_diam = 3;


	NcDb3dSolid* pBase = rectangleExtrude(pBlock, NcGePoint3d(-main_width / 2, 0, -main_width / 2), main_thickness, main_width + main_offset, main_width);

	NcDb3dSolid* pStartCylinder = cylinderExtrude(pBlock, NcGePoint3d(main_width / 2 + main_offset, cylinder_diam / 2, -main_width / 2), NcGeVector3d(0,0,1), cylinder_diam / 2, main_width);

	pBase->booleanOper(NcDb::kBoolUnite, pStartCylinder);

	NcDb3dSolid* pSlot = slotExtrude(pBlock, NcGePoint3d(main_width / 2 + main_offset, cylinder_diam / 2, main_width / 2 - offset), base_length, cylinder_diam / 2, base_angle, NcGeVector3d(0,0,1), - thickness);

	pBase->booleanOper(NcDb::kBoolUnite, pSlot);

	NcDb3dSolid* pCutLeft = rectangleExtrude(pBlock, NcGePoint3d(main_width / 2, 0, main_width / 2), cylinder_diam, main_offset + cylinder_diam, -leftCut_depth);

	pBase->booleanOper(NcDb::kBoolSubtract, pCutLeft);

	NcDb3dSolid* pCutRight = rectangleExtrude(pBlock, NcGePoint3d(main_width / 2, 0, -main_width / 2), cylinder_diam, main_offset + cylinder_diam, main_width - leftCut_depth - functional_dist);

	pBase->booleanOper(NcDb::kBoolSubtract, pCutRight);


	NcDb3dSolid* pCounterbore = cylinderExtrude(pBlock, NcGePoint3d(0, thickness, 0), NcGeVector3d(0, 1, 0), counterbore_diam / 2, -counterbore_depth);
	pBase->booleanOper(NcDb::kBoolSubtract, pCounterbore);

	NcDb3dSolid* pCentralHole = cylinderExtrude(pBlock, NcGePoint3d(0, thickness, 0), NcGeVector3d(0, 1, 0), centralHole_diam / 2, -thickness);
	pBase->booleanOper(NcDb::kBoolSubtract, pCentralHole);

	NcDb3dSolid* pHoleDown = cylinderExtrude(pBlock, NcGePoint3d(main_width / 2 + main_offset, cylinder_diam / 2, -main_width / 2), NcGeVector3d(0, 0, 1), functionalHole_diam / 2, main_width);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleDown);

	NcDb3dSolid* pHoleUp = cylinderExtrude(pBlock, NcGePoint3d(main_width / 2 + main_offset + base_length*cos(base_angle), cylinder_diam / 2 + base_length * sin(base_angle), -main_width / 2), NcGeVector3d(0, 0, 1), functionalHole_diam / 2, main_width);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleUp);

	pBlock->appendNcDbEntity(pBase);

	pBase->close();
}

NcDb3dSolid* createLink(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double center_dist, double radius, double angle, double hole_diam, double thickness) {

	NcDb3dSolid* pLink = slotExtrude(pBlock, startPoint, center_dist, radius, angle, NcGeVector3d(0,0,-1), thickness);

	NcDb3dSolid* pHole1 = cylinderExtrude(pBlock, startPoint, NcGeVector3d(0, 0, 1), hole_diam / 2, thickness);

	pLink->booleanOper(NcDb::kBoolSubtract, pHole1);

	NcGePoint3d endPoint(startPoint.x + center_dist * cos(angle), startPoint.x + center_dist * sin(angle), 0);

	NcDb3dSolid* pHole2 = cylinderExtrude(pBlock, endPoint, NcGeVector3d(0, 0, 1), hole_diam / 2, thickness);

	pLink->booleanOper(NcDb::kBoolSubtract, pHole2);

	pBlock->appendNcDbEntity(pLink);

	return pLink;
}

void createLinkWithSlot(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double center_dist, double radius, double angle, double hole_diam, double slot_offset, double thickness) {

	NcDb3dSolid* pLinkSlot = createLink(pBlock, startPoint, center_dist, radius, 0, radius, thickness);

	double rectSlot_length = 15;
	double rectSlot_width = 6;

	double slot_Length = 10;

	NcDb3dSolid* rectSlot = rectangleExtrude(pBlock, NcGePoint3d(slot_offset, -radius, -rectSlot_width / 2), 2 * radius, rectSlot_length, rectSlot_width);

	pLinkSlot->booleanOper(NcDb::kBoolUnite, rectSlot);

	NcDb3dSolid* pSlot = slotExtrude(pBlock, NcGePoint3d(startPoint.x + slot_offset + rectSlot_length / 2 - slot_Length / 2, radius, 0), slot_Length, 3.0 / 2, 0, NcGeVector3d(0,1,0), -rectSlot_width);

	pLinkSlot->booleanOper(NcDb::kBoolSubtract, pSlot);

	pBlock->appendNcDbEntity(pLinkSlot);

	pLinkSlot->close();
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

		//createBase(pBlock, base_angle / 180.0 * PI, 8);

		/*NcDb3dSolid *pTest = */

		midPhalanx_base(pBlock, NcGePoint3d(0, 8, -2), 8, 14, 4, 4);

		//pBlock->appendNcDbEntity(pTest);

		//pTest->close();

		//NcGePoint3d origin(0, 0, 0);
		//NcGePoint3d point_A(base_length * cos(base_angle * PI / 180), base_length * sin(base_angle * PI / 180), 0);
		//NcGePoint3d point_I(r2_length * cos(initial_angle * PI / 180), -r2_length * sin(initial_angle * PI / 180), 0);
		//NcGePoint3d point_D(r2_length * cos(initial_angle * PI / 180) + 0.5, -r2_length * sin(initial_angle * PI / 180) - r2_hand_length, 0);
		//NcGePoint3d point_C(r2_length * cos(initial_angle * PI / 180), -r2_length * sin(initial_angle * PI / 180) - middle_hand_length, 0);
		//NcGePoint3d point_G(r2_length * cos(initial_angle * PI / 180) + middle_base_length, -r2_length * sin(initial_angle * PI / 180) - middle_hand_length, 0);
		//NcGePoint3d point_F(r2_length * cos(initial_angle * PI / 180) + middle_base_length + 0.5, -r2_length * sin(initial_angle * PI / 180) - middle_hand_length + distal_length, 0);



		////NcDbPoint* point = new NcDbPoint(endPt);
		//NcDbLine* pLine_base = new NcDbLine(origin, point_A);
		//NcDbLine* pLine_r2 = new NcDbLine(origin, point_I);
		//NcDbLine* pLine_r2_hand = new NcDbLine(point_I, point_D);
		//NcDbLine* pLine_r3 = new NcDbLine(point_A, point_C);
		//NcDbLine* pLine_driver = new NcDbLine(point_D, point_F);
		//NcDbLine* pLine_distal = new NcDbLine(point_F, point_G);
		//NcDbLine* pLine_middle_base = new NcDbLine(point_C, point_G);
		//NcDbLine* pLine_middle_hand = new NcDbLine(point_I, point_C);


		//AcDbObjectId LineId;

		//pBlock->appendAcDbEntity(LineId, pLine_base);
		//pBlock->appendAcDbEntity(LineId, pLine_r2);
		//pBlock->appendAcDbEntity(LineId, pLine_r2_hand);
		//pBlock->appendAcDbEntity(LineId, pLine_r3);
		//pBlock->appendAcDbEntity(LineId, pLine_driver);
		//pBlock->appendAcDbEntity(LineId, pLine_distal);
		//pBlock->appendAcDbEntity(LineId, pLine_middle_base);
		//pBlock->appendAcDbEntity(LineId, pLine_middle_hand);

		//pLine_base->close();
		//pLine_r2->close();

		//pLine_base->close();
		//pLine_r2->close();
		//pLine_r2_hand->close();
		//pLine_r3->close();
		//pLine_driver->close();
		//pLine_distal->close();
		//pLine_middle_base->close();
		//pLine_middle_hand->close();
		pBlock->close();

		acutPrintf(L"\nМеханизм успешно построен!\n");

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

		double inner_diameter = (pRingForm->ui.lineEdit->text()).toDouble();
		double ring_thickness = 2;
		double height = 10;

		double fixation_width = 9;
		double fixation_height = 6;
		double fixation_translateX = 6.5;
		
		double fixation_walls_thickness = 1;
		double slot_width = fixation_width - fixation_walls_thickness * 2;
		double slot_depth = 4;


		AcGeVector3d normal(0.0, 0.0, 1.0);

		AcGePoint3d center(0.0, 0.0, 0.0);

		///// Creating main ring contour
		NcDb3dSolid* pRing = cylinderExtrude(pBlock, center, normal, inner_diameter / 2 + ring_thickness, height);


		///// Creating fixation part
		NcDb3dSolid* pFixation = rectangleExtrude(pBlock, NcGePoint3d(-fixation_translateX - fixation_width / 2, 0, 0), inner_diameter / 2 + ring_thickness + fixation_height, fixation_width, height);

		pRing->booleanOper(NcDb::kBoolUnite, pFixation);


		///// Creating inner main hole
		NcDb3dSolid* pHole = cylinderExtrude(pBlock, center, normal, inner_diameter / 2, height);
		pRing->booleanOper(NcDb::kBoolSubtract, pHole);

		///// Creating slot in fixation

		NcDb3dSolid* pSlot = rectangleExtrude(pBlock, NcGePoint3d(-fixation_translateX - fixation_width / 2 + fixation_walls_thickness, inner_diameter / 2 + ring_thickness + fixation_height - slot_depth, 0), slot_depth, slot_width, height);
		pRing->booleanOper(NcDb::kBoolSubtract, pSlot);

		///// Creating revolve for nut and screw

		double diam_HoleForThread = 4;
		double hole_depth = 2;
		double diam_HoleForNut = 6;

		NcDb3dSolid* pThreadedHole = cylinderExtrude(pBlock, NcGePoint3d(-fixation_translateX, inner_diameter / 2 + ring_thickness + fixation_height - slot_depth, height / 2), NcGeVector3d(0, 1, 0), diam_HoleForThread / 2,  -(inner_diameter / 2 + ring_thickness + fixation_height - slot_depth));

		pRing->booleanOper(NcDb::kBoolSubtract, pThreadedHole);

		NcDb3dSolid* pNutForHole = cylinderExtrude(pBlock, NcGePoint3d(-fixation_translateX, inner_diameter / 2 + ring_thickness + fixation_height - slot_depth - hole_depth, height / 2), NcGeVector3d(0, 1, 0), diam_HoleForNut / 2, -(inner_diameter / 2 + ring_thickness + fixation_height - slot_depth - hole_depth));

		pRing->booleanOper(NcDb::kBoolSubtract, pNutForHole);

		//////////

		pBlock->appendNcDbEntity(pRing);
		pRing->close();

		pBlock->close();

		acutPrintf(L"\nКольцо успешно построено!\n");
		pRingForm->close();

		});

	pRingForm->exec();

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

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToCut);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pCylinderToCut);

			pCylinderToCut->close();
			pCylinderToAdd->close();
		}

		if (pPlateForm->ui.checkBox_middle->isChecked()) {

			fingerJointCenter.set(bracer_offset + finger_width / 2 + distanceBtwJoints, bracer_width + pPlateForm->ui.lineEdit_MiddleFinger->text().toDouble(), 0);

			NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset + distanceBtwJoints, bracer_width, 0), pPlateForm->ui.lineEdit_MiddleFinger->text().toDouble(), finger_width, plate_thickness);
			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pFingerSolid);

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDbCircle* pCircleToCut = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2, cylinder_height);

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

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2, cylinder_height);

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

			NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2, cylinder_height);

			pBlock->appendNcDbEntity(pCylinderToAdd);

			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

			NcDbCircle* pCircleToCut = new NcDbCircle(fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2);

			NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2, cylinder_height);

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