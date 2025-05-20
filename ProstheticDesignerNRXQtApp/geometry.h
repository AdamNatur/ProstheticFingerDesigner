#pragma once

#include "stdafx.h"

#include "hostQtUi.h"
#include "MechanismForm.h"
#include "resource.h"		// main symbols
#include "arxHeaders.h"
#include "AcExtensionModule.h"
#include <math.h>

#include "Ring.h"



class Mechanism
{
public:
	Mechanism(AcDbBlockTableRecord* pBlock, double inner_diam, MechanismForm* form);
	~Mechanism();

public:
	double base_angle;
	double initial_angle;
	double base_length; // default = 8
	double r2_length; //default = 71.56;
	double r2_hand_length; //default = 8;
	double r3_length; // default = 74.12;
	double middle_hand_length; //default = 7.5;
	double middle_base_length; //default = 17;
	double distal_length; //default = 7
	double r2_angle;
	double radius;

	NcGePoint3d point_H;
	NcGePoint3d point_A;
	NcGePoint3d point_I;
	NcGePoint3d point_D;
	NcGePoint3d point_C;
	NcGePoint3d point_G;
	NcGePoint3d point_F;

	NcGePoint3d point_H_upd;
	NcGePoint3d point_I_upd;

	double main_width;
	double main_offset;
	double main_thickness;
	double cylinder_diam;
	double offset;
	double thickness;
	double leftCut_depth;
	double functional_dist;
	double counterbore_diam;
	double counterbore_depth;
	double centralHole_diam;
	double functionalHole_diam;

	NcDb3dSolid* pBase = NULL;
	NcDb3dSolid* pBaseFixation = NULL;
	NcDb3dSolid* pLink = NULL;
	NcDb3dSolid* pLinkSlot = NULL;
	NcDb3dSolid* pBase_middle = NULL;
	NcDb3dSolid* pDistal = NULL;
	NcDb3dSolid* pMiddle_link = NULL;


	AcGePoint3d* center = NULL;

	Ring* pRing = NULL;

	double slot_Length = 10;
	double slot_offset = 40;

public:
	void setPos(NcGePoint3d point_base, NcGePoint3d point_finger, NcDb3dSolid* pSolid);
	void setPosMech(NcGePoint3d point_base, NcGePoint3d point_finger);
	void setPosRing(int a, double offset);
	void setPosPoint(NcGePoint3d point_base, NcGePoint3d point_finger, NcGePoint3d& point);
	void mirror();
};




NcDb3dSolid* rectangleExtrude(AcDbBlockTableRecord* pBlock, NcGePoint3d initPoint, double length, double width, double height);
NcDb3dSolid* cylinderExtrude(AcDbBlockTableRecord* pBlock, NcGePoint3d point, NcGeVector3d normal, double radius, double height);
NcDb3dSolid* slotExtrude(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDist, double radius, double orientAngle, NcGeVector3d vector, double height);
void midPhalanx_base(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDistVert, double centerDistHoriz, double radius, double thickness);
void midPhalanx_distal(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDist, double radius, double thickness);
void createBase(AcDbBlockTableRecord* pBlock, double base_angle, double base_length);
NcDb3dSolid* createLink(AcDbBlockTableRecord* pBlock, NcGePoint3d startPoint, double centerDist, double radius, double hole_radius, double orientAngle, NcGeVector3d vector, double height);


