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
#include "MainForm.h"
#include "RingForm.h"
#include "MechanismForm.h"
#include "PlateForm.h"
#include "resource.h"		// main symbols
#include "arxHeaders.h"
#include "AcExtensionModule.h"
#include <math.h>
#include <QEventLoop>

#include "geometry.h"

#include "Ring.h"

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

void FingerProsthetic_Ring() {

	RingForm* pRingForm = new RingForm(nullptr);

	QObject::connect(pRingForm->Create_pushBtn, &QPushButton::clicked, [&]() {
		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* pBlock;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
		pBlockTable->close();
		pRingForm->inner_diam = pRingForm-> comboBox_InnerDiam->currentText().toInt();
		Ring ring(pBlock, pRingForm->inner_diam);
		ring.pRing->close();
		pRingForm->close();
		pBlock->close();
	});

	pRingForm->exec();
}

void FingerProsthetic_Plate() {

	PlateForm* pPlateForm = new PlateForm(nullptr);

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

		pBlock->appendAcDbEntity(pPlateSolid);

		pPlateSolid->close();
		pBlock->close();

		acutPrintf(L"\nПлатформа успешно построена!\n");

		pPlateForm->close();

		});

		pPlateForm->exec();
}

void FingerProsthetic_Designer() {

	MainForm* pMainForm = new MainForm(nullptr);

	QObject::connect(pMainForm->pPushBtnCreate, &QPushButton::clicked, [&]() {

		if (pMainForm->totalCheckState == 0)
		{
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
			NcGePoint3d fingerJointCenter(0, 0, 0);


			///Creating finger plates for each finger

			if (pMainForm->pCGBoxIndex->isChecked()) {

				fingerJointCenter.set(bracer_offset + finger_width / 2, bracer_width + pMainForm->pCGBoxIndex->pLineEditSetJointDist->text().toDouble(), 0);

				NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset, bracer_width, 0), pMainForm->pCGBoxIndex->pLineEditSetJointDist->text().toDouble(), finger_width, plate_thickness);
				pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pFingerSolid);

				NcDb3dSolid* pCylinderToAdd = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), finger_width / 2, cylinder_height);
				pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolUnite, pCylinderToAdd);

				NcDb3dSolid* pCylinderToCut = cylinderExtrude(pBlock, fingerJointCenter, NcGeVector3d(0, 0, 1), hole / 2, cylinder_height);
				pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pCylinderToCut);

				pCylinderToCut->close();
				pCylinderToAdd->close();

				Mechanism mech(pBlock, pMainForm->pCGBoxIndex->pComboBoxRingDiam->currentText().toInt(), pMainForm->pCGBoxIndex->pMechanismForm);

				mech.setPosMech(NcGePoint3d(0, 0, 0), fingerJointCenter);

				mech.setPosRing(0, fingerJointCenter.x - 0.5);

			}

			if (pMainForm->pCGBoxMiddle->isChecked()) {

				fingerJointCenter.set(bracer_offset + finger_width / 2 + distanceBtwJoints, bracer_width + pMainForm->pCGBoxMiddle->pLineEditSetJointDist->text().toDouble(), 0);

				NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset + distanceBtwJoints, bracer_width, 0), pMainForm->pCGBoxMiddle->pLineEditSetJointDist->text().toDouble(), finger_width, plate_thickness);
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

				Mechanism mech(pBlock, pMainForm->pCGBoxMiddle->pComboBoxRingDiam->currentText().toInt(), 
					pMainForm->pCGBoxMiddle->pMechanismForm);

				mech.setPosMech(NcGePoint3d(0, 0, 0), fingerJointCenter);

				mech.setPosRing(1, fingerJointCenter.x - 0.5);
			}

			if (pMainForm->pCGBoxRing->isChecked()) {

				fingerJointCenter.set(bracer_offset + finger_width / 2 + 2 * distanceBtwJoints, bracer_width + pMainForm->pCGBoxRing->pLineEditSetJointDist->text().toDouble(), 0);

				NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset + 2 * distanceBtwJoints, bracer_width, 0), pMainForm->pCGBoxRing->pLineEditSetJointDist->text().toDouble(), finger_width, plate_thickness);
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

				Mechanism mech(pBlock, pMainForm->pCGBoxRing->pComboBoxRingDiam->currentText().toInt(), 
					pMainForm->pCGBoxRing->pMechanismForm);

				mech.setPosMech(NcGePoint3d(0, 0, 0), fingerJointCenter);

				mech.setPosRing(2, fingerJointCenter.x - 0.5);
			}

			if (pMainForm->pCGBoxPinky->isChecked()) {

				fingerJointCenter.set(bracer_offset + finger_width / 2 + 3 * distanceBtwJoints, bracer_width + pMainForm->pCGBoxRing->pLineEditSetJointDist->text().toDouble(), 0);

				NcDb3dSolid* pFingerSolid = rectangleExtrude(pBlock, NcGePoint3d(bracer_offset + 3 * distanceBtwJoints, bracer_width, 0), pMainForm->pCGBoxRing->pLineEditSetJointDist->text().toDouble(), finger_width, plate_thickness);
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

				Mechanism mech(pBlock, pMainForm->pCGBoxPinky->pComboBoxRingDiam->currentText().toInt(),
					pMainForm->pCGBoxPinky->pMechanismForm);

				mech.setPosMech(NcGePoint3d(0, 0, 0), fingerJointCenter);

				mech.setPosRing(3, fingerJointCenter.x - 0.5);
			}

			double slotOffset_thickness = 4;
			double slot_width = 3;

			NcDb3dSolid* pSlotLeft = rectangleExtrude(pBlock, NcGePoint3d(slotOffset_thickness, slotOffset_thickness, 0), bracer_width - slotOffset_thickness * 2, slot_width, plate_thickness);
			pBlock->appendNcDbEntity(pSlotLeft);
			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pSlotLeft);

			NcDb3dSolid* pSlotRight = rectangleExtrude(pBlock, NcGePoint3d(bracer_length - slotOffset_thickness - slot_width, slotOffset_thickness, 0), bracer_width - slotOffset_thickness * 2, slot_width, plate_thickness);
			pBlock->appendNcDbEntity(pSlotRight);
			pPlateSolid->booleanOper(NcDb::BoolOperType::kBoolSubtract, pSlotRight);

			pBlock->appendAcDbEntity(pPlateSolid);

			pPlateSolid->close();

			pBlock->close();
		}
	});

	pMainForm->exec();
};

void FingerProsthetic_Mechanism()
{
	//MechanismForm* pMechanismForm = new MechanismForm(nullptr);

	//QObject::connect(pMechanismForm->pushBtn_OK, &QPushButton::clicked, [&](){
	//	AcDbBlockTable* pBlockTable;
	//	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	//	AcDbBlockTableRecord* pBlock;
	//	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
	//	pBlockTable->close();

	//	Mechanism mech(pBlock, pMainForm->pCGBoxIndex->pComboBoxRingDiam->currentText().toInt(), pMainForm->pCGBoxIndex->pMechanismForm);

	//	mech.setPosMech(NcGePoint3d(0, 0, 0), fingerJointCenter);

	//	mech.setPosRing(0, fingerJointCenter.x - 0.5);
	//});

	//pMechanismForm->exec();
}
	

void initApp() {
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
	ncedRegCmds->addCommand(L"ProstheticDesinger_GROUP",
		L"_FingerProsthetic_Designer",
		L"FingerProsthetic_Designer",
		ACRX_CMD_TRANSPARENT,
		FingerProsthetic_Designer);
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