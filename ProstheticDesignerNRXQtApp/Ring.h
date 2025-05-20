#pragma once

#include "resource.h"		// main symbols
#include "arxHeaders.h"
#include "AcExtensionModule.h"


class Ring : public NcDb3dSolid
{
public:
	Ring(AcDbBlockTableRecord* pBlock, int innerDiam);
	~Ring();

public:

	NcDb3dSolid* pRing;

	double inner_diameter;
	double ring_thickness;
	double height;
	double fixation_width;
	double fixation_height;
	double fixation_translateX;
	double fixation_walls_thickness;
	double slot_width;
	double slot_depth;
	double diam_HoleForThread;
	double hole_depth;
	double diam_HoleForNut;


};


