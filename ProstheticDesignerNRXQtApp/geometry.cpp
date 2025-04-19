
#include "stdafx.h"

#include "geometry.h"





Mechanism::Mechanism(AcDbBlockTableRecord* pBlock, double inner_diam)
{
	base_angle = 60;
	initial_angle = 21;
	base_length = 8; // default = 8
	r2_length = 71.56; //default = 71.56;
	r2_hand_length = 8; //default = 8;
	r3_length = 74.12; // default = 74.12;
	middle_hand_length = 7.5; //default = 7.5;
	middle_base_length = 17; //default = 17;
	distal_length = 7; //default = 7
	middle_angle = 32;
	r2_angle = 103.71;


	point_H.set(63, 10.5, 0);
	point_H_upd.set(point_H.x, point_H.y, point_H.z);
	point_A.set(67, 17.428203, 0);
	point_I.set(126.720983, -22.067388, 0);
	point_I_upd.set(point_I.x, point_I.y, point_I.z);
	point_D.set(124.872386, -29.850876, 0);
	point_C.set(124.576954, -29.2544, 0);
	point_G.set(141.229084, -32.675889, 0);
	point_F.set(141.200273, -25.675948, 0);

	main_width = 16;
	main_offset = 4;
	main_thickness = 3;
	cylinder_diam = 6;
	offset = 4.5;
	thickness = 3;
	leftCut_depth = 3.5;
	functional_dist = 10;
	counterbore_diam = 14;
	counterbore_depth = 2.5;
	centralHole_diam = 12;
	functionalHole_diam = 3;

	pBase = rectangleExtrude(pBlock, NcGePoint3d(point_H.x - main_width - main_offset, point_H.y - main_thickness, -main_width / 2), main_thickness, main_width + main_offset, main_width);

	NcDb3dSolid* pStartCylinder = cylinderExtrude(pBlock, NcGePoint3d(point_H.x, point_H.y, -main_width / 2), NcGeVector3d(0, 0, 1), cylinder_diam / 2, main_width);

	pBase->booleanOper(NcDb::kBoolUnite, pStartCylinder);

	NcDb3dSolid* pSlot = slotExtrude(pBlock, NcGePoint3d(point_H.x, point_H.y, -offset), base_length, cylinder_diam / 2, base_angle * PI / 180, NcGeVector3d(0, 0, 1), thickness);

	pBase->booleanOper(NcDb::kBoolUnite, pSlot);

	NcDb3dSolid* pCutLeft = rectangleExtrude(pBlock, NcGePoint3d(point_H.x - cylinder_diam / 2 - 1, point_H.y - main_thickness, main_width / 2), cylinder_diam, main_offset + cylinder_diam, -leftCut_depth);

	pBase->booleanOper(NcDb::kBoolSubtract, pCutLeft);

	NcDb3dSolid* pCutRight = rectangleExtrude(pBlock, NcGePoint3d(point_H.x - cylinder_diam / 2 - 1, point_H.y - main_thickness, -main_width / 2), cylinder_diam, main_offset + cylinder_diam, main_width - leftCut_depth - functional_dist);

	pBase->booleanOper(NcDb::kBoolSubtract, pCutRight);

	NcDb3dSolid* pCounterbore = cylinderExtrude(pBlock, NcGePoint3d(point_H.x - main_width / 2 - main_offset, point_H.y, 0), NcGeVector3d(0, 1, 0), counterbore_diam / 2, -counterbore_depth);
	pBase->booleanOper(NcDb::kBoolSubtract, pCounterbore);

	NcDb3dSolid* pCentralHole = cylinderExtrude(pBlock, NcGePoint3d(point_H.x - main_width / 2 - main_offset, point_H.y, 0), NcGeVector3d(0, 1, 0), centralHole_diam / 2, -thickness);
	pBase->booleanOper(NcDb::kBoolSubtract, pCentralHole);

	NcDb3dSolid* pHoleDown = cylinderExtrude(pBlock, NcGePoint3d(point_H.x, point_H.y, -main_width / 2), NcGeVector3d(0, 0, 1), functionalHole_diam / 2, main_width);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleDown);

	NcDb3dSolid* pHoleUp = cylinderExtrude(pBlock, NcGePoint3d(point_H.x + base_length * cos(base_angle * PI / 180), point_H.y + base_length * sin(base_angle * PI / 180), -main_width / 2), NcGeVector3d(0, 0, 1), functionalHole_diam / 2, main_width);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleUp);

	pBlock->appendNcDbEntity(pBase);

	////////////////////////////////////////////////////////////////////////////////


	//// Creating link

	pLink = createLink(pBlock, NcGePoint3d(point_A.x, point_A.y, -offset - thickness), point_A.distanceTo(point_C), 3, 2, atan((point_C.y - point_A.y) / (point_C.x - point_A.x)), NcGeVector3d(0, 0, 1), thickness);

	pBlock->appendNcDbEntity(pLink);


	//// Creating link with slot

	double radius = 3;
	double hole_diam = 2;

	pLinkSlot = createLink(pBlock, NcGePoint3d(0, 0, -thickness / 2), point_H.distanceTo(point_I), radius, hole_diam, 0, NcGeVector3d(0, 0, 1), thickness);

	NcDb3dSolid* pLinkSlotHand = createLink(pBlock, NcGePoint3d(point_H.distanceTo(point_I), 0, -thickness / 2), point_I.distanceTo(point_D), radius, hole_diam, -atan((point_D.y - point_I.y) / (point_D.x - point_I.x)), NcGeVector3d(0, 0, 1), thickness);

	pLinkSlot->booleanOper(NcDb::kBoolUnite, pLinkSlotHand);

	double rectSlot_length = 15;
	double rectSlot_width = slot_width;

	slot_Length = 10;
	slot_offset = 40;

	NcDb3dSolid* rectSlot = rectangleExtrude(pBlock, NcGePoint3d(slot_offset, -radius, -rectSlot_width / 2), 2 * radius, rectSlot_length, rectSlot_width);

	pLinkSlot->booleanOper(NcDb::kBoolUnite, rectSlot);

	NcDb3dSolid* pSlot1 = slotExtrude(pBlock, NcGePoint3d(0 + slot_offset + rectSlot_length / 2 - slot_Length / 2, radius, 0), slot_Length, 2.0 / 2, 0, NcGeVector3d(0, 0, 1), 2 * radius);

	NcGeMatrix3d* matrixRotateSlot = new NcGeMatrix3d();

	matrixRotateSlot->setToRotation(PI / 2, NcGeVector3d(1, 0, 0), NcGePoint3d(0 + slot_offset + rectSlot_length / 2 - slot_Length / 2, radius, 0));

	pSlot1->transformBy(*matrixRotateSlot);

	pLinkSlot->booleanOper(NcDb::kBoolSubtract, pSlot1);

	NcGeMatrix3d* matrixRotate1 = new NcGeMatrix3d();

	matrixRotate1->setToRotation(atan((point_H.y - point_I.y) / (point_H.x - point_I.x)), NcGeVector3d(0, 0, 1), NcGePoint3d(0, 0, -thickness / 2));

	pLinkSlot->transformBy(*matrixRotate1);

	NcGeMatrix3d* matrixRotate2 = new NcGeMatrix3d();

	matrixRotate2->setToTranslation(NcGeVector3d(point_H.x, point_H.y, thickness / 2 + offset));

	pLinkSlot->transformBy(*matrixRotate2);

	pBlock->appendNcDbEntity(pLinkSlot);

	//// Creating midphalanx base

	pBase_middle = slotExtrude(pBlock, NcGePoint3d(point_I.x, point_I.y, -offset), point_I.distanceTo(point_C), radius, PI + atan((point_C.y - point_I.y) / (point_C.x - point_I.x)), NcGeVector3d(0, 0, 1), thickness);

	NcDb3dSolid* pBaseHorizontal = slotExtrude(pBlock, NcGePoint3d(point_C.x, point_C.y, -offset), point_C.distanceTo(point_G), radius, atan((point_G.y - point_C.y) / (point_G.x - point_C.x)), NcGeVector3d(0, 0, 1), thickness);

	pBase_middle->booleanOper(NcDb::kBoolUnite, pBaseHorizontal);

	NcDb3dSolid* pCircle = cylinderExtrude(pBlock, NcGePoint3d(point_I.x, point_I.y, -offset), NcGeVector3d(0, 0, 1), radius, 9.0);

	pBase_middle ->booleanOper(NcDb::kBoolUnite, pCircle);

	NcDb3dSolid* pHoleDown1 = cylinderExtrude(pBlock, NcGePoint3d(point_C.x, point_C.y, -offset), NcGeVector3d(0, 0, 1), 1.5, thickness);
	pBase_middle->booleanOper(NcDb::kBoolSubtract, pHoleDown1);

	NcDb3dSolid* pHoleHorizontal = cylinderExtrude(pBlock, NcGePoint3d(point_G.x, point_G.y, -offset), NcGeVector3d(0, 0, 1), 1.5, thickness);
	pBase_middle->booleanOper(NcDb::kBoolSubtract, pHoleHorizontal);

	NcDb3dSolid* pHoleUp1 = cylinderExtrude(pBlock, NcGePoint3d(point_I.x, point_I.y, -offset), NcGeVector3d(0, 0, 1), 1.5, 9.0);
	pBase_middle->booleanOper(NcDb::kBoolSubtract, pHoleUp1);

	pBlock->appendNcDbEntity(pBase_middle);

	//// Create distal part

	pDistal = slotExtrude(pBlock, NcGePoint3d(point_G.x, point_G.y, -offset + thickness), point_G.distanceTo(point_F), 3, PI / 2, NcGeVector3d(0, 0, 1), thickness);

	double offset_y = 3;
	double plate_length = 3;
	double plate_width = 2;
	double hole_diam2 = 2;

	NcDb3dSolid* pRect = rectangleExtrude(pBlock, NcGePoint3d(point_G.x + radius, point_G.y + offset_y, -offset + thickness), plate_width, plate_length, thickness);

	pDistal->booleanOper(NcDb::kBoolUnite, pRect);

	NcDb3dSolid* pHoleDown2 = cylinderExtrude(pBlock, NcGePoint3d(point_G.x, point_G.y, -offset + thickness), NcGeVector3d(0, 0, 1), hole_diam2 / 2, thickness);

	pDistal->booleanOper(NcDb::kBoolSubtract, pHoleDown2);

	NcDb3dSolid* pHoleUp2 = cylinderExtrude(pBlock, NcGePoint3d(point_G.x, point_G.y + point_G.distanceTo(point_F), -offset + thickness), NcGeVector3d(0, 0, 1), hole_diam2 / 2, thickness);

	pDistal->booleanOper(NcDb::kBoolSubtract, pHoleUp2);

	pBlock->appendNcDbEntity(pDistal);


	/// Create link 

	pMiddle_link = createLink(pBlock, NcGePoint3d(point_D.x, point_D.y, main_width / 2 - leftCut_depth - thickness), point_D.distanceTo(point_F), 3, 1.5, atan((point_F.y - point_D.y) / (point_F.x - point_D.x)), NcGeVector3d(0, 0, 1), thickness);

	pBlock->appendNcDbEntity(pMiddle_link);


	this->pRing = this->createRing(pBlock, inner_diam);

	NcGeMatrix3d* mat = new NcGeMatrix3d();

	mat->setTranslation(NcGeVector3d(-point_H.x + main_width / 2 + main_offset, -point_H.y, -point_H.z));

	pBase->transformBy(*mat);
	pLink->transformBy(*mat);
	pLinkSlot->transformBy(*mat);
	pBase_middle->transformBy(*mat);
	pDistal->transformBy(*mat);
	pMiddle_link->transformBy(*mat);

	point_H_upd.transformBy(*mat);
	point_I_upd.transformBy(*mat);


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
}

Mechanism::~Mechanism()
{
	pBase->close();
	pLink->close();
	pLinkSlot->close();
	pBase_middle->close();
	pDistal->close();
	pMiddle_link->close();
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

	NcDb3dSolid* pSlot = cylinderExtrude(pBlock, NcGePoint3d(0, 0, 0), vector, radius, height);

	NcDb3dSolid* pRect = rectangleExtrude(pBlock, NcGePoint3d(0, -radius, 0), 2 * radius, centerDist, height);

	pSlot->booleanOper(NcDb::kBoolUnite, pRect);

	NcDb3dSolid* pCylinder = cylinderExtrude(pBlock, NcGePoint3d(centerDist, 0, 0), vector, radius, height);

	pSlot->booleanOper(NcDb::kBoolUnite, pCylinder);

	NcGeMatrix3d* rotateMatrixRotate = new NcGeMatrix3d();

	rotateMatrixRotate->setToRotation(orientAngle, vector, NcGePoint3d(0, 0, 0));

	pSlot->transformBy(*rotateMatrixRotate);

	NcGeMatrix3d* rotateMatrixMove = new NcGeMatrix3d();

	rotateMatrixMove->setToTranslation(NcGeVector3d(startPoint.x, startPoint.y, startPoint.z));

	pSlot->transformBy(*rotateMatrixMove);

	pBlock->appendNcDbEntity(pSlot);

	return pSlot;
}

void midPhalanx_base(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDistVert, double centerDistHoriz, double radius, double thickness) {

	NcDb3dSolid* pBase = slotExtrude(pBlock, startPoint, centerDistVert, radius, -PI / 2, NcGeVector3d(0, 0, 1), thickness);

	NcDb3dSolid* pBaseHorizontal = slotExtrude(pBlock, NcGePoint3d(startPoint.x, startPoint.y - centerDistVert, -thickness / 2), centerDistHoriz, radius, 0, NcGeVector3d(0, 0, 1), thickness);

	pBase->booleanOper(NcDb::kBoolUnite, pBaseHorizontal);

	NcDb3dSolid* pCircle = cylinderExtrude(pBlock, startPoint, NcGeVector3d(0, 0, 1), radius, 8.0);

	pBase->booleanOper(NcDb::kBoolUnite, pCircle);


	NcDb3dSolid* pHoleDown = cylinderExtrude(pBlock, NcGePoint3d(startPoint.x, startPoint.y - centerDistVert, -thickness / 2), NcGeVector3d(0, 0, 1), 1.0, thickness);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleDown);

	NcDb3dSolid* pHoleHorizontal = cylinderExtrude(pBlock, NcGePoint3d(startPoint.x + centerDistHoriz, startPoint.y - centerDistVert, -thickness / 2), NcGeVector3d(0, 0, 1), 1.0, thickness);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleHorizontal);

	NcDb3dSolid* pHoleUp = cylinderExtrude(pBlock, startPoint, NcGeVector3d(0, 0, 1), 1.0, 8.0);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleUp);


	pBlock->appendNcDbEntity(pBase);
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

	NcDb3dSolid* pStartCylinder = cylinderExtrude(pBlock, NcGePoint3d(main_width / 2 + main_offset, cylinder_diam / 2, -main_width / 2), NcGeVector3d(0, 0, 1), cylinder_diam / 2, main_width);

	pBase->booleanOper(NcDb::kBoolUnite, pStartCylinder);

	NcDb3dSolid* pSlot = slotExtrude(pBlock, NcGePoint3d(main_width / 2 + main_offset, cylinder_diam / 2, main_width / 2 - offset), base_length, cylinder_diam / 2, base_angle, NcGeVector3d(0, 0, 1), -thickness);

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

	NcDb3dSolid* pHoleUp = cylinderExtrude(pBlock, NcGePoint3d(main_width / 2 + main_offset + base_length * cos(base_angle), cylinder_diam / 2 + base_length * sin(base_angle), -main_width / 2), NcGeVector3d(0, 0, 1), functionalHole_diam / 2, main_width);
	pBase->booleanOper(NcDb::kBoolSubtract, pHoleUp);

	pBlock->appendNcDbEntity(pBase);
}

NcDb3dSolid* createLink(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDist, double radius, double hole_radius, double orientAngle, NcGeVector3d vector, double height) {

	NcDb3dSolid* pSlot = cylinderExtrude(pBlock, NcGePoint3d(0, 0, 0), vector, radius, height);

	NcDb3dSolid* pRect = rectangleExtrude(pBlock, NcGePoint3d(0, -radius, 0), 2 * radius, centerDist, height);

	pSlot->booleanOper(NcDb::kBoolUnite, pRect);

	NcDb3dSolid* pHole1 = cylinderExtrude(pBlock, NcGePoint3d(0, 0, 0), vector, hole_radius, height);

	pSlot->booleanOper(NcDb::kBoolSubtract, pHole1);

	NcDb3dSolid* pCylinder = cylinderExtrude(pBlock, NcGePoint3d(centerDist, 0, 0), vector, radius, height);

	pSlot->booleanOper(NcDb::kBoolUnite, pCylinder);

	NcDb3dSolid* pHole2 = cylinderExtrude(pBlock, NcGePoint3d(centerDist, 0, 0), vector, hole_radius, height);

	pSlot->booleanOper(NcDb::kBoolSubtract, pHole2);

	NcGeMatrix3d* rotateMatrixRotate = new NcGeMatrix3d();

	rotateMatrixRotate->setToRotation(orientAngle, vector, NcGePoint3d(0, 0, 0));

	pSlot->transformBy(*rotateMatrixRotate);

	NcGeMatrix3d* rotateMatrixMove = new NcGeMatrix3d();

	rotateMatrixMove->setToTranslation(NcGeVector3d(startPoint.x, startPoint.y, startPoint.z));

	pSlot->transformBy(*rotateMatrixMove);

	pBlock->appendNcDbEntity(pSlot);

	return pSlot;
}


void Mechanism::setPos(NcGePoint3d point_base, NcGePoint3d point_finger, NcDb3dSolid* pSolid)
{
	NcGeMatrix3d* matTrans = new NcGeMatrix3d();
	matTrans->setTranslation(NcGeVector3d(point_finger.x - point_base.x, point_finger.y - point_base.y, point_finger.z - point_base.z + 7));

	NcGeMatrix3d* matRotateX = new NcGeMatrix3d();
	matRotateX->setToRotation(PI / 2, NcGeVector3d(1, 0, 0));

	NcGeMatrix3d* matRotateZ = new NcGeMatrix3d();
	matRotateZ->setToRotation(PI / 2, NcGeVector3d(0, 0, 1), point_base);

	pSolid->transformBy(*matRotateX);
	pSolid->transformBy(*matRotateZ);
	pSolid->transformBy(*matTrans);
}

void Mechanism::setPosPoint(NcGePoint3d point_base, NcGePoint3d point_finger, NcGePoint3d& point)
{
	NcGeMatrix3d* matTrans = new NcGeMatrix3d();
	matTrans->setTranslation(NcGeVector3d(point_finger.x - point_base.x, point_finger.y - point_base.y, point_finger.z - point_base.z));

	NcGeMatrix3d* matRotateX = new NcGeMatrix3d();
	matRotateX->setToRotation(PI / 2, NcGeVector3d(1, 0, 0));

	NcGeMatrix3d* matRotateZ = new NcGeMatrix3d();
	matRotateZ->setToRotation(PI / 2, NcGeVector3d(0, 0, 1), point_base);

	point.transformBy(*matRotateX);
	point.transformBy(*matRotateZ);
	point.transformBy(*matTrans);
}

void Mechanism::setPosMech(NcGePoint3d point_base, NcGePoint3d point_finger)
{
	this->setPos(point_base, point_finger, this->pBase);
	this->setPos(point_base, point_finger, this->pLink);
	this->setPos(point_base, point_finger, this->pLinkSlot);
	this->setPos(point_base, point_finger, this->pBase_middle);
	this->setPos(point_base, point_finger, this->pDistal);
	this->setPos(point_base, point_finger, this->pMiddle_link);

	setPosPoint(point_base, point_finger, this->point_H_upd);
	setPosPoint(point_base, point_finger, this->point_I_upd);
}

void Mechanism::setPosRing(Mechanism mech, int a, double offset)
{
	NcGeMatrix3d matRotateX;
	double angle = atan((point_H.y - point_I.y) / (point_H.x - point_I.x));

	matRotateX.setToRotation(PI / 2 + angle, NcGeVector3d(1, 0, 0), NcGePoint3d(0, 0, 0));

	NcGeMatrix3d matRotateY;

	matRotateY.setToRotation(-PI, NcGeVector3d(0, 1, 0), NcGePoint3d(0, 0, 0));

	NcGeMatrix3d matMove;

	matMove.setToTranslation(NcGeVector3d((point_I_upd.x - point_H_upd.x) + offset, point_H_upd.y, point_H_upd.z + 7));

	NcGeMatrix3d matMove2;

	double absfs = (slot_Length / 2 + slot_offset)* sin(angle);

	matMove2.setToTranslation(NcGeVector3d(0, (slot_Length / 2 + slot_offset) * cos(angle), (slot_Length / 2 + slot_offset) * sin(angle) - (inner_diameter / 2 + ring_thickness + slot_depth)));

	NcGeMatrix3d matMove3;

	matMove3.setToTranslation(NcGeVector3d(0, -slot_Length / 2 *cos(-angle), slot_Length / 2 * sin(-angle)));


	mech.pRing->transformBy(matRotateY);
	mech.pRing->transformBy(matRotateX);
	mech.pRing->transformBy(matMove);
	mech.pRing->transformBy(matMove2);
	mech.pRing->transformBy(matMove3);

}

NcDb3dSolid* Mechanism::createRing(AcDbBlockTableRecord* pBlock, double inner_diam)
{
	this->inner_diameter = inner_diam;

	///// Creating main ring contour
	pRing = cylinderExtrude(pBlock, NcGePoint3d(0.0, 0.0, -height / 2), NcGeVector3d(0, 0, 1), inner_diameter / 2 + ring_thickness, height);


	///// Creating fixation part
	NcDb3dSolid* pFixation = rectangleExtrude(pBlock, NcGePoint3d(-fixation_translateX - fixation_width / 2, 0, -height / 2), inner_diameter / 2 + ring_thickness + fixation_height, fixation_width, height);
	pRing->booleanOper(NcDb::kBoolUnite, pFixation);


	///// Creating inner main hole
	NcDb3dSolid* pHole = cylinderExtrude(pBlock, NcGePoint3d(0.0, 0.0, -height / 2), NcGeVector3d(0, 0, 1), inner_diameter / 2, height);
	pRing->booleanOper(NcDb::kBoolSubtract, pHole);

	///// Creating slot in fixation

	NcDb3dSolid* pSlot = rectangleExtrude(pBlock, NcGePoint3d(-fixation_translateX - fixation_width / 2 + fixation_walls_thickness, inner_diameter / 2 + ring_thickness + fixation_height - slot_depth, -height / 2), slot_depth, slot_width, height);
	pRing->booleanOper(NcDb::kBoolSubtract, pSlot);

	///// Creating revolve for nut and screw

	NcDb3dSolid* pThreadedHole = cylinderExtrude(pBlock, NcGePoint3d(-fixation_translateX, inner_diameter / 2 + ring_thickness + fixation_height - slot_depth, 0), NcGeVector3d(0, 1, 0), diam_HoleForThread / 2, -(inner_diameter / 2 + ring_thickness + fixation_height - slot_depth));
	pRing->booleanOper(NcDb::kBoolSubtract, pThreadedHole);

	NcDb3dSolid* pNutForHole = cylinderExtrude(pBlock, NcGePoint3d(-fixation_translateX, inner_diameter / 2 + ring_thickness + fixation_height - slot_depth - hole_depth, 0), NcGeVector3d(0, 1, 0), diam_HoleForNut / 2, -(inner_diameter / 2 + ring_thickness + fixation_height - slot_depth - hole_depth));
	pRing->booleanOper(NcDb::kBoolSubtract, pNutForHole);

	//////////

	pBlock->appendNcDbEntity(pRing);

	return pRing;
}







