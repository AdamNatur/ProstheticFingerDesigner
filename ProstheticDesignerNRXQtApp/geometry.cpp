
#include "stdafx.h"

#include "geometry.h"



Mechanism::Mechanism(AcDbBlockTableRecord* pBlock, double inner_diam, MechanismForm* form)
{
	base_angle = 60;
	initial_angle = 27.07;
	base_length = form->lineEdit_baseLength->text().toDouble(); // default = 8
	r2_length = form->lineEdit_r2Length->text().toDouble(); //default = 71.56;
	r2_hand_length = form->lineEdit_r2HandLength->text().toDouble(); //default = 8;
	r3_length = form->lineEdit_r3Length->text().toDouble(); // default = 74.12;
	middle_hand_length = form->lineEdit_middleHandLength->text().toDouble(); //default = 7.5;
	middle_base_length = form->lineEdit_middleBaseLength->text().toDouble(); //default = 17;
	distal_length = form->lineEdit_distalLength->text().toDouble();; //default = 7
	r2_angle = 103.71;

	double middle_base = 85;

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


	point_H.set(0, 0, 0);
	point_H_upd.set(point_H.x, point_H.y, point_H.z);
	point_A.set(point_H.x + base_length * cos(base_angle / 360 * 2 * PI), 
		point_H.y + base_length * sin(base_angle / 360 * 2 * PI), 0);
	point_I.set(point_H.x + r2_length * cos(-initial_angle / 360 * 2 * PI), 
		point_H.y + r2_length * sin(-initial_angle / 360 * 2 * PI), 0);
	point_I_upd.set(point_I.x, point_I.y, point_I.z);

	double beta = 180 - initial_angle + r2_angle;

	point_D.set(point_I.x + r2_hand_length * cos(beta / 360 * 2 * PI), point_I.y + r2_hand_length * sin(beta / 360 * 2 * PI), 0);


	NcGeCircArc2d circle1(NcGePoint2d(point_A.x,point_A.y), r3_length);
	NcGeCircArc2d circle2(NcGePoint2d(point_I.x, point_I.y), middle_hand_length); 
	NcGePoint2d point1(0,0);
	NcGePoint2d point2(0,0);
	int i = 2;
	circle1.intersectWith(circle2, i, point1, point2, NcGeContext::gTol);
	NcGePoint2d point_res = point1.y > point2.y ? point2 : point1;
	point_C.set(point_res.x, point_res.y, 0);

	NcGeLineSeg2d segCI(NcGePoint2d(point_C.x , point_C.y), NcGePoint2d(point_I.x, point_I.y));
	NcGeLine2d* normal = new NcGeLine2d();
	segCI.getPerpLine(NcGePoint2d(point_C.x, point_C.y), *normal);
	NcGePoint2d endPoint(0, 0);
	NcGeCircArc2d circleNormal(NcGePoint2d(point_C.x, point_C.y), middle_base_length);
	NcGePoint2d pointNormal(0, 0);
	NcGePoint2d pointNormal2(0, 0);
	int k = 1;
	circleNormal.intersectWith(*normal, k, pointNormal, pointNormal2, NcGeContext::gTol);
	endPoint = pointNormal2;
	NcGeMatrix2d matPnt;
	matPnt.setToRotation(5 / 360 * 2 * PI, NcGePoint2d(point_C.x, point_C.y));
	endPoint.transformBy(matPnt);
	point_G.set(endPoint.x, endPoint.y, 0);
	

	NcGeLineSeg2d segCG(NcGePoint2d(point_C.x, point_C.y), NcGePoint2d(point_G.x, point_G.y));
	NcGeLine2d* normalCG = new NcGeLine2d();
	segCG.getPerpLine(NcGePoint2d(point_G.x, point_G.y), *normalCG);
	NcGePoint2d endPoint2(0, 0);
	NcGeCircArc2d circleNormal2(NcGePoint2d(point_G.x, point_G.y), distal_length);
	NcGePoint2d pointNormal12(0, 0);
	NcGePoint2d pointNormal22(0, 0);
	int t = 2;
	circleNormal2.intersectWith(*normalCG, t, pointNormal12, pointNormal22, NcGeContext::gTol);
	endPoint2 = pointNormal22;
	NcGeMatrix2d matPnt2;
	matPnt2.setToRotation(PI + 5 / 360 * 2 * PI, NcGePoint2d(point_G.x, point_G.y));
	endPoint2.transformBy(matPnt2);
	point_F.set(endPoint2.x, endPoint2.y, 0);




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


	///Creating base Fixation

	pBaseFixation = cylinderExtrude(pBlock, NcGePoint3d(point_H.x - main_width / 2 - main_offset, point_H.y, 0), NcGeVector3d(0, 1, 0), counterbore_diam / 2, -counterbore_depth);

	double thicknessBaseSlot = 1;
	double lengthBaseSlot = 4;

	/*NcDb3dSolid* pBaseFixationSlotBig = slotExtrude(pBlock, NcGePoint3d(point_H.x - main_width / 2 - main_offset, point_H.y - thicknessBaseSlot, 0), 5, 3, 0, NcGeVector3d(0, 1, 0), -counterbore_depth + thickness);
	pBaseFixation->booleanOper(NcDb::kBoolSubtract, pBaseFixationSlotBig);*/
	//NcDb3dSolid* pBaseFixationSlotSmall = slotExtrude(pBlock, NcGePoint3d(point_H.x - main_width / 2 - main_offset, point_H.y, 0), 5, 1.5, 0, NcGeVector3d(0, 1, 0), -counterbore_depth);
	//pBaseFixation->booleanOper(NcDb::kBoolSubtract, pBaseFixationSlotSmall);

	pBlock->appendNcDbEntity(pBaseFixation);
	////////////////////////////////////////////////////////////////////////////////

	////Creating Ring
	pRing = new Ring(pBlock, inner_diam);

	//// Creating link

	pLink = createLink(pBlock, NcGePoint3d(point_A.x, point_A.y, -offset - thickness), point_A.distanceTo(point_C), 3, 2, atan((point_C.y - point_A.y) / (point_C.x - point_A.x)), NcGeVector3d(0, 0, 1), thickness);

	pBlock->appendNcDbEntity(pLink);


	//// Creating link with slot

	radius = 3;
	double hole_diam = 2;

	pLinkSlot = createLink(pBlock, NcGePoint3d(0, 0, -thickness / 2), point_H.distanceTo(point_I), radius, hole_diam, 0, NcGeVector3d(0, 0, 1), thickness);

	NcDb3dSolid* pLinkSlotHand = createLink(pBlock, NcGePoint3d(point_H.distanceTo(point_I), 0, -thickness / 2), point_I.distanceTo(point_D), radius, hole_diam, -atan((point_D.y - point_I.y) / (point_D.x - point_I.x)), NcGeVector3d(0, 0, 1), thickness);

	pLinkSlot->booleanOper(NcDb::kBoolUnite, pLinkSlotHand);

	double rectSlot_length = 15;
	double rectSlot_width = pRing->slot_width;

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

	pDistal = createLink(pBlock, NcGePoint3d(point_G.x, point_G.y, -offset + thickness), 
		point_G.distanceTo(point_F), 3, 1.5, PI / 2, NcGeVector3d(0, 0, 1), thickness);

	double offset_y = 3;
	double plate_length = 3;
	double plate_width = 2;
	double hole_diam2 = 2;

	NcDb3dSolid* pRect = rectangleExtrude(pBlock, NcGePoint3d(point_G.x + radius, point_G.y + offset_y, -offset + thickness), 
		plate_width, plate_length, thickness);
	pDistal->booleanOper(NcDb::kBoolUnite, pRect);
	NcGeMatrix3d* mat228 = new NcGeMatrix3d();

	mat228->setToRotation(PI + PI / 2 + atan((point_G.y - point_F.y) / (point_G.x - point_F.x)), NcGeVector3d(0, 0, 1), NcGePoint3d(point_G.x, point_G.y, -offset + thickness));
	pDistal->transformBy(*mat228);

	pBlock->appendNcDbEntity(pDistal);



	/// Create link 

	pMiddle_link = createLink(pBlock, NcGePoint3d(point_F.x, point_F.y, main_width / 2 - leftCut_depth - thickness), 
		point_D.distanceTo(point_F), 3, 1.5, PI + atan((point_D.y - point_F.y) / (point_D.x - point_F.x)), NcGeVector3d(0, 0, 1), thickness);
	pBlock->appendNcDbEntity(pMiddle_link);
 
	NcGeMatrix3d* mat = new NcGeMatrix3d();
	mat->setTranslation(NcGeVector3d(-point_H.x + main_width / 2 + main_offset, -point_H.y, -point_H.z));

	pBase->transformBy(*mat);
	pLink->transformBy(*mat);
	pLinkSlot->transformBy(*mat);
	pBase_middle->transformBy(*mat);
	pDistal->transformBy(*mat);
	pMiddle_link->transformBy(*mat);
	pBaseFixation->transformBy(*mat);

	point_H_upd.transformBy(*mat);
	point_I_upd.transformBy(*mat);
}

Mechanism::~Mechanism()
{
	pBase->close();
	pLink->close();
	pLinkSlot->close();
	pBase_middle->close();
	pDistal->close();
	pMiddle_link->close();
	pBaseFixation->close();
	pRing->pRing->close();
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

NcDb3dSolid* cylinderExtrude(AcDbBlockTableRecord* pBlock, NcGePoint3d point, NcGeVector3d normal,
	double radius, double height) {

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
	matTrans->setTranslation(NcGeVector3d(point_finger.x - point_base.x, point_finger.y - point_base.y, point_finger.z - point_base.z + 7));

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
	this->setPos(point_base, point_finger, this->pBaseFixation);
	
	setPosPoint(point_base, point_finger, this->point_H_upd);
	setPosPoint(point_base, point_finger, this->point_I_upd);
}

void Mechanism::setPosRing(int a, double offset)
{
	NcGeMatrix3d matRotateX;
	double angle = atan((point_H.y - point_I.y) / (point_H.x - point_I.x));

	matRotateX.setToRotation(PI / 2, NcGeVector3d(1, 0, 0), NcGePoint3d(0, 0, 0));

	NcGeMatrix3d matRotateY;

	matRotateY.setToRotation(-PI, NcGeVector3d(0, 1, 0), NcGePoint3d(0, 0, 0));

	NcGeMatrix3d matMove;

	matMove.setToTranslation(NcGeVector3d((point_I_upd.x - point_H_upd.x) + offset, point_H_upd.y, point_H_upd.z + this->radius ));

	NcGeMatrix3d matMove2;

	matMove2.setToTranslation(NcGeVector3d(0, slot_Length / 2 + slot_offset, -(pRing->inner_diameter / 2 + pRing->fixation_height)));

	NcGeMatrix3d matRotate;

	matRotate.setToRotation(angle, NcGeVector3d(1, 0, 0), point_H_upd);

	pRing->pRing->transformBy(matRotateY);
	pRing->pRing->transformBy(matRotateX);
	pRing->pRing->transformBy(matMove);
	pRing->pRing->transformBy(matMove2);
	pRing->pRing->transformBy(matRotate);
}

void Mechanism::mirror()
{
	//this->pBase->;
	//this->pLink = NULL;
	//this->pLinkSlot = NULL;
	//this->pBase_middle = NULL;
	//this->pDistal = NULL;
	//this->pMiddle_link = NULL;
}






