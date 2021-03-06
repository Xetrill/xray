#ifndef _MPxConstraint
#define _MPxConstraint
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors,  which is protected by U.S. and Canadian federal copyright law 
// and by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be 
// copied or duplicated, in whole or in part, without the prior written 
// consent of Autodesk.
// 
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the following 
// disclaimer, must be included in all copies of the Software, in whole 
// or in part, and all derivative works of the Software, unless such copies 
// or derivative works are solely in the form of machine-executable object 
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, 
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO 
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST 
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR 
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS 
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES. 
// ==========================================================================
//+
//
// CLASS:    MPxConstraint
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxConstraint)
//
//
// *****************************************************************************
//
#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


class TdataGeometry;

#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MPxTransform.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxConstraint)

/// Proxy constraint node (OpenMayaAnim) (OpenMayaMPx.py) 
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MPxConstraint : public MPxNode
{
public:

	///
	enum UpVectorType 
	{
		/// Scene up vector
		kScene = 0,
		/// Origin of object
		kObject,
		/// 
		kObjectRotation,
		///
		kVector,
		///
		kLast
	};

    ///
	MPxConstraint();
    ///
	virtual ~MPxConstraint();


	///
	virtual const MObject weightAttribute() const;
	///
    virtual const MObject targetAttribute() const;
	///
    virtual const MObject constraintRotateOrderAttribute() const;

	// Boolean
	static MObject enableRestPosition;
	// Boolean
	static MObject lockOutput;

protected:
	///
    virtual const MObject passiveOutputAttribute() const;
	///
	virtual void getOutputAttributes(MObjectArray& attributeArray);

	// Utility methods

	///
	MStatus	closestPoint( const MObject& dataGeometryObject,
								const MPoint& toThisPoint,
								MPoint& theClosestPoint ) const;
	///
	MStatus closestNormal( const MObject& dataGeometryObject,
								const MPoint& toThisPoint,
								MVector& theNormal ) const;
	///
	MStatus closestTangent( const MObject& dataGeometryObject,
								 const MPoint& toThisPoint,
								 MVector& theTangent ) const;

	///
	MEulerRotation computeAim( const MMatrix& parentInverseMatrix,
							   const MVector& targetVector,
							   const MVector& aimVector,
							   const MVector& upVector,
							   const MVector& wupVector,
							   MEulerRotation::RotationOrder order = MEulerRotation::kXYZ,
							   const MQuaternion& jointOrient = MQuaternion::identity,
							   MStatus* ResultStatus = NULL ) const;
	///
	MVector worldUpVector( MPxConstraint::UpVectorType upType,
								const MVector& upVector,
								const MMatrix& upMatrix,
								const MPoint& constrPoint,
								MStatus* ResultStatus = NULL ) const;
	///
	MPoint worldConstraintPoint( const MMatrix& parentInverseMatrix,
									const MVector& translate,
									const MVector& rotatePivot,
									const MVector& rotatePivotTranslate,
									MStatus* ResultStatus = NULL ) const;

	void* instance;

private:
	static void				initialSetup();
	static const char*	    className();


	TdataGeometry *convert( const MObject& dataGeometryObject ) const;
	void setData(void *ptr);
	void setConstraintData(void *ptr);
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxConstraint */
