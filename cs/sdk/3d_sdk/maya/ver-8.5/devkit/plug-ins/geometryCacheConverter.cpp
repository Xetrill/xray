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
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////////////////
//
// File Name: geometryCacheConverter.cpp
//
// Description : 
//		Converts the specified geometry	cache files to a specified format.
//
//	Usage :
//		convertGeometryCache -toAscii -file fileName1 -file fileName2 ..
//		
//		Where "fileName1" and "fileName2" are the string paths to the geometry 
//		cache files.
//
///////////////////////////////////////////////////////////////////////////////

// Project includes
//
#include <geometryCacheFile.h>

// Maya includes
//
#include <maya/MSyntax.h>
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnPlugin.h>

#define LFLAG_TOASCII "-toAscii"
#define SFLAG_TOASCII "-ta"

#define LFLAG_FILE "-file"
#define SFLAG_FILE "-f"

class convertGeometryCache : public MPxCommand
{
public:
	// Constructor / Destructor methods
	//
					convertGeometryCache();
	virtual			~convertGeometryCache(); 

	// Creator method
	//
	static void*	creator();

	// Syntax methods
	//
	virtual bool		hasSyntax();
	static MSyntax		cmdSyntax();

	// Do It method
	//
	virtual MStatus		doIt( const MArgList& args );
};

///////////////////////////////////////////////////////////////////////////////
//
// Methods
//
///////////////////////////////////////////////////////////////////////////////

convertGeometryCache::convertGeometryCache() 
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Constructor
//
///////////////////////////////////////////////////////////////////////////////
{
}

convertGeometryCache::~convertGeometryCache() 
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Destructor
//
///////////////////////////////////////////////////////////////////////////////
{
}

void* convertGeometryCache::creator()
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Creates and returns an instance of this class
//
///////////////////////////////////////////////////////////////////////////////
{
	return new convertGeometryCache();
}

bool convertGeometryCache::hasSyntax()
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Specifies whether or not the command has a syntax object
//
///////////////////////////////////////////////////////////////////////////////
{
	return true;
}

MSyntax convertGeometryCache::cmdSyntax()
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Creates a syntax object
//
///////////////////////////////////////////////////////////////////////////////
{
	MSyntax syntax;
	syntax.addFlag( SFLAG_TOASCII, LFLAG_TOASCII, MSyntax::kNoArg );
	syntax.addFlag( SFLAG_FILE, LFLAG_FILE, MSyntax::kString );
	syntax.makeFlagMultiUse( SFLAG_FILE );	
	syntax.enableQuery( false );
	syntax.enableEdit( false );

	return syntax;
}

MStatus convertGeometryCache::doIt( const MArgList& args )
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Converts the specified files to the specified conversion format
//
///////////////////////////////////////////////////////////////////////////////
{
	MStatus status = MS::kSuccess;

	MArgDatabase argDb( syntax(), args, &status );
	if( !status ) return status;

	bool isToAscii = argDb.isFlagSet( SFLAG_TOASCII );
	bool hasFile = argDb.isFlagSet( SFLAG_FILE );
	if( !isToAscii || !hasFile )
	{
		MGlobal::displayError( 
			"Specify at least one file and format to convert to." );
		return status;
	}

	// Create an MIffFile to read our cache files
    //
	MIffFile iffFilePtr;

	// Iterate through all the files specified
	//
	uint numUses = argDb.numberOfFlagUses( SFLAG_FILE );
	for( uint i = 0; i < numUses; i++ )
	{
		MArgList argList;
		status = argDb.getFlagArgumentList( SFLAG_FILE, i, argList ); 
		if( !status ) return status;

		MString name = argList.asString( 0, &status );
		if( !status ) return status;

		// Create a geometryCacheFile object from the current file path
		//
		geometryCacheFile cacheFile( name, &iffFilePtr);

		// Read the geometry cache file
		//
		bool readStatus = cacheFile.readCacheFiles();

		if( !readStatus ) {
			// If the read failed, report the file name that failed
			//
			MGlobal::displayError( "Failed in reading file \"" + name + "\"" );

			// Skip the conversion process
			//
			continue;
		} 
		
		// Convert the geometry cache file to the specified format
		//
        if( isToAscii ) {
			// Convert to Ascii
			//
			bool convertStatus = cacheFile.convertToAscii();

			if( !convertStatus )
			{
				// If the convert failed, report the file name that failed
				//
				MGlobal::displayError( "Failed in converting file \"" + 
										name +
										"\" to ASCII");
			}
		}

		// Insert other file format conversions here
		//
	}

	return status;
}

MStatus initializePlugin( MObject obj )
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Initializes the plugin
//
///////////////////////////////////////////////////////////////////////////////
{
	MStatus   status;
	MFnPlugin plugin( obj, "Autodesk", "8.0", "Any");

	status = plugin.registerCommand( "convertGeometryCache", 
										convertGeometryCache::creator, 
										convertGeometryCache::cmdSyntax );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Un-initializes the plugin
//
///////////////////////////////////////////////////////////////////////////////
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterCommand( "convertGeometryCache" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
