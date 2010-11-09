//-*****************************************************************************
//
// Copyright (c) 2009-2010,
//  Sony Pictures Imageworks, Inc. and
//  Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Sony Pictures Imageworks, nor
// Industrial Light & Magic nor the names of their contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-*****************************************************************************

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// We include some global mesh data to test with from an external source
// to keep this example code clean.
#include <Alembic/AbcGeom/Tests/MeshData.h>

//-*****************************************************************************
using namespace Alembic::AbcGeom; // Contains Abc, AbcCoreAbstract

void Example1_MeshOut()
{
    OArchive archive( Alembic::AbcCoreHDF5::WriteArchive(), "subD1.abc" );

    // Create a SubD class.
    OSubD meshyObj( OObject( archive, kTop ), "subd" );
    OSubDSchema &mesh = meshyObj.getSchema();

    // Set a mesh sample.
    // We're creating the sample inline here,
    // but we could create a static sample and leave it around,
    // only modifying the parts that have changed.
    OSubDSchema::Sample mesh_samp(
        V3fArraySample( ( const V3f * )g_verts, g_numVerts ),
        Int32ArraySample( g_indices, g_numIndices ),
        Int32ArraySample( g_counts, g_numCounts ) );

    // Set the sample.
    mesh.set( mesh_samp );

    std::cout << "Writing: " << archive.getName() << std::endl;
}

//-*****************************************************************************
void Example1_MeshIn()
{
    IArchive archive( Alembic::AbcCoreHDF5::ReadArchive(), "subD1.abc" );
    std::cout << "Reading: " << archive.getName() << std::endl;

    ISubD meshyObj( IObject( archive, kTop ), "subd" );
    ISubDSchema &mesh = meshyObj.getSchema();

    ISubDSchema::Sample mesh_samp;
    mesh.get( mesh_samp );

    std::cout << "Mesh num vertices: "
              << mesh_samp.getPositions()->size() << std::endl;

    std::cout << "0th vertex from the mesh sample: "
              << (*(mesh_samp.getPositions()))[0] << std::endl;

    std::cout << "0th vertex from the mesh sample with get method: "
              << mesh_samp.getPositions()->get()[0] << std::endl;
}

//-*****************************************************************************
int main( int argc, char *argv[] )
{
    Example1_MeshOut();
    Example1_MeshIn();

    return 0;
}
