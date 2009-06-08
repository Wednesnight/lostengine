/*
Bullet Continuous Collision Detection and Physics Library Maya Plugin
Copyright (c) 2008 Walt Disney Studios
 
This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising
from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:
 
1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must
not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 
Written by: Nicola Candussi <nicola@fluidinteractive.com>
*/

//pluginMain.cpp
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MDGMessage.h>

#include "mayaUtils.h"
#include "rigidBodyNode.h"
#include "rigidBodyArrayNode.h"
#include "collisionShapeNode.h"
#include "nailConstraintNode.h"
#include "dSolverNode.h"
#include "dSolverCmd.h"
#include "dRigidBodyCmd.h"
#include "dRigidBodyArrayCmd.h" 
#include "dNailConstraintCmd.h" 
#include "mvl/util.h"
#include "colladaExport.h"

const char *const colladaOptionScript = "colladaExportOptions";
const char *const colladaDefaultOptions =
    "groups=1;"
    "ptgroups=1;"
    "materials=1;"
    "smoothing=1;"
    "normals=1;"
    ;


MStatus initializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj, "Walt Disney Feature Animation", "2.73", "Any");

    solver_t::initialize();


// Register the translator with the system
   status =  plugin.registerFileTranslator( "COLLADA Physics export", "none",
                                          ObjTranslator::creator,
                                          (char *)colladaOptionScript,
                                          (char *)colladaDefaultOptions );

	MCHECKSTATUS(status,"registerFileTranslator COLLADA Physics export")

    
    //
    status = plugin.registerNode( rigidBodyNode::typeName, rigidBodyNode::typeId,
                                  rigidBodyNode::creator,
                                  rigidBodyNode::initialize,
                                  MPxNode::kLocatorNode );
    MCHECKSTATUS(status, "registering rigidBodyNode")
    MDGMessage::addNodeRemovedCallback(rigidBodyNode::nodeRemoved, rigidBodyNode::typeName);

    //
    status = plugin.registerNode( rigidBodyArrayNode::typeName, rigidBodyArrayNode::typeId,
                                  rigidBodyArrayNode::creator,
                                  rigidBodyArrayNode::initialize,
                                  MPxNode::kLocatorNode );
    MCHECKSTATUS(status, "registering rigidBodyArrayNode")
    MDGMessage::addNodeRemovedCallback(rigidBodyArrayNode::nodeRemoved, rigidBodyArrayNode::typeName);


    // 
    status = plugin.registerNode( collisionShapeNode::typeName, collisionShapeNode::typeId,
                                  collisionShapeNode::creator,
                                  collisionShapeNode::initialize,
                                  MPxNode::kDependNode );
    MCHECKSTATUS(status, "registering collisionShapeNode")

    //
    status = plugin.registerNode( nailConstraintNode::typeName, nailConstraintNode::typeId,
                                  nailConstraintNode::creator,
                                  nailConstraintNode::initialize,
                                  MPxNode::kLocatorNode );
    MCHECKSTATUS(status, "registering nailConstraintNode")
    MDGMessage::addNodeRemovedCallback(nailConstraintNode::nodeRemoved, nailConstraintNode::typeName);


    // 
    status = plugin.registerNode( dSolverNode::typeName, dSolverNode::typeId,
                                  dSolverNode::creator, 
                                  dSolverNode::initialize,
                                  MPxNode::kDependNode );
    MCHECKSTATUS(status, "registering dSolverNode")

    status = plugin.registerCommand( dSolverCmd::typeName,
                                     dSolverCmd::creator,
                                     dSolverCmd::syntax);
    MCHECKSTATUS(status, "registering dSolverCmd")

    status = plugin.registerCommand( dRigidBodyCmd::typeName,
                                     dRigidBodyCmd::creator,
                                     dRigidBodyCmd::syntax);
    MCHECKSTATUS(status, "registering dRigidBodyCmd")


    status = plugin.registerCommand( dRigidBodyArrayCmd::typeName,
                                     dRigidBodyArrayCmd::creator,
                                     dRigidBodyArrayCmd::syntax);
    MCHECKSTATUS(status, "registering dRigidBodyArrayCmd")

    status = plugin.registerCommand( dNailConstraintCmd::typeName,
                                     dNailConstraintCmd::creator,
                                     dNailConstraintCmd::syntax);
    MCHECKSTATUS(status, "registering dNailConstraintCmd")

    MGlobal::executeCommand( "source dynamicaUI.mel" );
    MGlobal::executeCommand( "dynamicaUI_initialize" );
    
    return status;
}

MStatus uninitializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterCommand(dNailConstraintCmd::typeName);
    MCHECKSTATUS(status, "deregistering dNailConstraintCmd")

    status = plugin.deregisterCommand(dRigidBodyArrayCmd::typeName);
    MCHECKSTATUS(status, "deregistering dRigidBodyArrayCmd")

    status = plugin.deregisterCommand(dRigidBodyCmd::typeName);
    MCHECKSTATUS(status, "deregistering dRigidBodyCmd")

    status = plugin.deregisterCommand(dSolverCmd::typeName);
    MCHECKSTATUS(status, "deregistering dSolverGlobalsCmd")

    status = plugin.deregisterNode(dSolverNode::typeId);
    MCHECKSTATUS(status, "deregistering dSolverNode")

    status = plugin.deregisterNode(nailConstraintNode::typeId);
    MCHECKSTATUS(status, "deregistering nailConstraintNode")

    status = plugin.deregisterNode(collisionShapeNode::typeId);
    MCHECKSTATUS(status, "deregistering collisionShapeNode")

    status = plugin.deregisterNode(rigidBodyArrayNode::typeId);
    MCHECKSTATUS(status, "deregistering rigidBodyArrayNode")

    status = plugin.deregisterNode(rigidBodyNode::typeId);
    MCHECKSTATUS(status, "deregistering rigidBodyNode")

    status =  plugin.deregisterFileTranslator( "COLLADA Physics export" );
    MCHECKSTATUS(status,"deregistering COLLADA Physics export" )

    solver_t::cleanup();

    return status;
}

