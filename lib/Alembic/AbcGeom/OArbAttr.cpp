//-*****************************************************************************
//
// Copyright (c) 2009-2011,
//  Sony Pictures Imageworks Inc. and
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
// Industrial Light & Magic, nor the names of their contributors may be used
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

#include <Alembic/AbcGeom/OArbAttr.h>

namespace Alembic {
namespace AbcGeom {

//-*****************************************************************************
const AbcA::DataType &OTypedArbAttr::getDataType()
{
    ALEMBIC_ABC_SAFE_CALL_BEGIN( "OTypedArbAttr::getDataType()" );

    return m_valProp.getDataType();

    ALEMBIC_ABC_SAFE_CALL_END();

    static const AbcA::DataType ret;
    return ret;
}

//-*****************************************************************************
void OTypedArbAttr::set( const sample_type &iSamp,
                         const OSampleSelector &iSS )
{
    ALEMBIC_ABC_SAFE_CALL_BEGIN( "OTypedArbAttr::set()" );

    if ( iSS.getIndex() == 0 )
    {
        // First, create the value and index properties, using metadata from
        // *this, with GeometryScope set
        AbcA::MetaData md = this->getMetaData();
        SetGeometryScope( md, m_scope );
        m_valProp = prop_type( *this, ".vals", md, m_timeSamplingType );

        // are we setting things via indices?
        if ( m_isIndexed )
        {
            m_indices = OInt32ArrayProperty( *this, ".indices",
                                             m_timeSamplingType );

            m_indices.set( iSamp.getIndices(), iSS );
            m_valProp.set( iSamp.getIndexedVals(), iSS );
        }
        else
        {
            m_valProp.set( iSamp.getExpandedVals(), iSS );
        }
    }
    else
    {
        if ( m_isIndexed )
        {
            SetPropUsePrevIfNull( m_indices, iSamp.getIndices(), iSS );
            SetPropUsePrevIfNull( m_valProp, iSamp.getIndexedVals(), iSS );
        }
        else
        {
            SetPropUsePrevIfNull( m_valProp, iSamp.getExpandedVals(), iSS );
        }
    }

    ALEMBIC_ABC_SAFE_CALL_END_RESET();
}

//-*****************************************************************************
void OTypedArbAttr::setFromPrevious( const OSampleSelector &iSS )
{
    ALEMBIC_ABC_SAFE_CALL_BEGIN( "OTypedArbAttr::setFromPrevious()" );

    m_valProp.setFromPrevious( iSS );

    if ( m_isIndexed ) { m_indices.setFromPrevious( iSS ); }

    ALEMBIC_ABC_SAFE_CALL_END();
}

//-*****************************************************************************
size_t OTypedArbAttr::getNumSamples()
{
    ALEMBIC_ABC_SAFE_CALL_BEGIN( "OTypedArbAttr::getNumSamples()" );

    if ( m_isIndexed )
    {
        if ( m_indices )
        {
            return std::max( m_indices.getNumSamples(),
                             m_valProp.getNumSamples() );
        }
        else { return 0; }
    }
    else
    {
        if ( m_valProp ) { return m_valProp.getNumSamples(); }
        else { return 0; }
    }

    ALEMBIC_ABC_SAFE_CALL_END();

    return 0;
}


}
}