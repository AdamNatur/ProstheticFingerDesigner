#include "stdafx.h"

#include "Ring.h"

#include "geometry.h"

Ring::Ring(AcDbBlockTableRecord* pBlock, int innerDiam)
{
	inner_diameter = innerDiam;
	ring_thickness = 2;
	fixation_width = 9;
	fixation_height = 10;
	fixation_translateX = 6.5;
	fixation_walls_thickness = 1;
	slot_width = fixation_width - fixation_walls_thickness * 2;
	slot_depth = 6;
	diam_HoleForThread = 3;
	hole_depth = 2;
	diam_HoleForNut = 6;
	height = 10;

	///// Creating main ring contour
	pRing = cylinderExtrude(pBlock, NcGePoint3d(0.0, 0.0, -height / 2), NcGeVector3d(0, 0, 1), inner_diameter / 2 + ring_thickness, height);


	///// Creating fixation part
	NcDb3dSolid* pFixation = rectangleExtrude(pBlock, NcGePoint3d(-fixation_translateX - fixation_width / 2, 0, -height / 2), inner_diameter / 2 + fixation_height, fixation_width, height);
	pRing->booleanOper(NcDb::kBoolUnite, pFixation);


	///// Creating inner main hole
	NcDb3dSolid* pHole = cylinderExtrude(pBlock, NcGePoint3d(0.0, 0.0, -height / 2), NcGeVector3d(0, 0, 1), inner_diameter / 2, height);
	pRing->booleanOper(NcDb::kBoolSubtract, pHole);

	///// Creating slot in fixation

	NcDb3dSolid* pSlot = rectangleExtrude(pBlock, NcGePoint3d(-fixation_translateX - fixation_width / 2 + fixation_walls_thickness, inner_diameter / 2 + fixation_height - slot_depth, -height / 2), slot_depth, slot_width, height);
	pRing->booleanOper(NcDb::kBoolSubtract, pSlot);

	///// Creating revolve for nut and screw

	NcDb3dSolid* pThreadedHole = cylinderExtrude(pBlock, NcGePoint3d(-fixation_translateX, inner_diameter / 2 + fixation_height - slot_depth, 0), NcGeVector3d(0, 1, 0), diam_HoleForThread / 2, -(inner_diameter / 2 + fixation_height - slot_depth));
	pRing->booleanOper(NcDb::kBoolSubtract, pThreadedHole);

	NcDb3dSolid* pNutForHole = cylinderExtrude(pBlock, NcGePoint3d(-fixation_translateX, inner_diameter / 2 + fixation_height - slot_depth - hole_depth, 0), NcGeVector3d(0, 1, 0), diam_HoleForNut / 2, -(inner_diameter / 2 + fixation_height - slot_depth - hole_depth));
	pRing->booleanOper(NcDb::kBoolSubtract, pNutForHole);

	//////////

	pBlock->appendNcDbEntity(pRing);
}

Ring::~Ring()
{

};