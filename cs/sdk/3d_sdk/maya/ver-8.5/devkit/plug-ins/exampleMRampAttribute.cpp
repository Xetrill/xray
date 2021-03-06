//
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors, which is protected by U.S. and Canadian federal copyright 
// law and by international treaties.
//
// The Data is provided for use exclusively by You. You have the right 
// to use, modify, and incorporate this Data into other products for 
// purposes authorized by the Autodesk software license agreement, 
// without fee.
//
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the 
// following disclaimer, must be included in all copies of the 
// Software, in whole or in part, and all derivative works of 
// the Software, unless such copies or derivative works are solely 
// in the form of machine-executable object code generated by a 
// source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
// PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
// TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
// BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
// DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
// AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
// OR PROBABILITY OF SUCH DAMAGES.
//
// =========================================================================
//+
//

#include <maya/MIOStream.h>

#include <maya/MPxNode.h> 
#include <maya/MFnPlugin.h>
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MRampAttribute.h>


class exampleRampAttrNode : public MPxNode
{
public:
	exampleRampAttrNode() {};
	virtual        ~exampleRampAttrNode() {};
	static  void   *creator();
	static  MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock  &dataBlock);

	static MObject input1;
	static MObject input2;
	static MTypeId  id;

};

MTypeId exampleRampAttrNode::id( 0x81027 );

MObject exampleRampAttrNode::input1;
MObject exampleRampAttrNode::input2;

void *exampleRampAttrNode::creator()
{
   return((void *) new exampleRampAttrNode);
}

MStatus exampleRampAttrNode::initialize()
{
	MStatus stat;

	MString curveRamp("curveRamp");
	MString cvr("cvr");
	MString colorRamp("colorRamp");
	MString clr("clr");

	input1 = MRampAttribute::createCurveRamp(curveRamp, cvr);
	input2 = MRampAttribute::createColorRamp(colorRamp, clr);

	stat = addAttribute(input1);
	if(!stat)
	{
		cout << "ERROR in adding curveRamp Attribute!\n";
	}
	stat = addAttribute(input2);
	if(!stat)
	{
		cout << "ERROR in adding colorRamp Attribute!\n";
	}

	return stat;
}

MStatus exampleRampAttrNode::compute( const MPlug &plug, MDataBlock  &dataBlock ) 
{
	return(MStatus::kSuccess);
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "7.0", "Any");

	status = plugin.registerNode("exampleRampAttrNode", exampleRampAttrNode::id,
		   	exampleRampAttrNode::creator, exampleRampAttrNode::initialize);
	if (!status) 
	{
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(exampleRampAttrNode::id);
	if (!status) 
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
