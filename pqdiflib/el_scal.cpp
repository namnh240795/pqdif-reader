/*
**  Class:          CPQDIF_E_Scalar
**  Description:    Implements the PQDIF scalar element
**  --------------------------------------------------------------------------
**
**  Copyright 2021 PQDIF Authors
**
**  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
**  either express or implied. See the License for the specific language governing permissions and limitations under the License.
**
**  See the LICENSE file distributed with this work for copyright and licensing information, the AUTHORS file for a list of
**  copyright holders, and the CONTRIBUTORS file for the list of contributors.
**
**  SPDX - License - Identifier: Apache - 2.0
*/

#include "PQDIF_classes.h"


//  Construction
//  ============

CPQDIF_E_Scalar::CPQDIF_E_Scalar()
    {
    memset( &m_value, 0, sizeof( m_value ) );
    }

CPQDIF_E_Scalar::~CPQDIF_E_Scalar()
    {
    }

#ifdef PQDIF_USE_COM
bool CPQDIF_E_Scalar::SetValue( long typePhysical, VARIANT & value )
    {
    bool    status = false;

    m_typePhysical = typePhysical;
    if( m_pRecord )
        m_pRecord->SetChanged( true );

    status = convertFromVariant( value, m_value );

    return status;
    }

bool CPQDIF_E_Scalar::GetValue( VARIANT& value )
    {
    bool    status = true;

    status = convertToVariant( m_value, value );

    return status;
    }
#endif

bool CPQDIF_E_Scalar::SetValue( long typePhysical, PQDIFValue& value )
    {
    bool    status = false;

    int     iSize = theInfo.GetNumBytesOfType( typePhysical );
    if( iSize > 0 )
        {
        m_typePhysical = typePhysical;
        memcpy( (void *)&m_value, (void *)&value, iSize );
        status = true;
    
        if( m_pRecord )
            m_pRecord->SetChanged( true );
        }
    
    return status;
    }

bool CPQDIF_E_Scalar::GetValue( long& typePhysical, PQDIFValue& value ) const
    {
    bool    status = false;

    if( m_typePhysical != 0 )
        {
        typePhysical = m_typePhysical;
        value = m_value;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueUINT2( UINT2 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_UNS_INTEGER2;
    m_value.uint2 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueUINT2( UINT2& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_UNS_INTEGER2 )
        {
        value = m_value.uint2;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueINT2( INT2 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_INTEGER2;
    m_value.int2 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueINT2( INT2& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_INTEGER2 )
        {
        value = m_value.int2;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueUINT4( UINT4 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_UNS_INTEGER4;
    m_value.uint4 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueUINT4( UINT4& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_UNS_INTEGER4 )
        {
        value = m_value.uint4;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueINT4( INT4 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_INTEGER4;
    m_value.int4 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueINT4( INT4& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_INTEGER4 )
        {
        value = m_value.int4;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueBOOL4( bool value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_BOOLEAN4;
    m_value.bool4 = value ? (BOOL4) 1 : (BOOL4) 0;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueBOOL4( bool& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_BOOLEAN4 )
        {
        value = ( m_value.bool4 != 0 ) ? true : false;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueREAL4( REAL4 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_REAL4;
    m_value.real4 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueREAL4( REAL4& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_REAL4 )
        {
        value = m_value.real4;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueREAL8( REAL8 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_REAL8;
    m_value.real8 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueREAL8( REAL8& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_REAL8 )
        {
        value = m_value.real8;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueCOMPLEX8( COMPLEX8 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_COMPLEX8;
    m_value.complex8 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueCOMPLEX8( COMPLEX8& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_COMPLEX8 )
        {
        value = m_value.complex8;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueCOMPLEX16( COMPLEX16 value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_COMPLEX16;
    m_value.complex16 = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueCOMPLEX16( COMPLEX16& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_COMPLEX16 )
        {
        value = m_value.complex16;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueGUID( GUID value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_GUID;
    m_value.guid = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueGUID( GUID& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_GUID )
        {
        value = m_value.guid;
        status = true;
        }

    return status;
    }


bool CPQDIF_E_Scalar::SetValueTimeStamp( TIMESTAMPPQDIF value )
    {
    bool    status = false;

    m_typePhysical = ID_PHYS_TYPE_TIMESTAMPPQDIF;
    m_value.ts = value;
    status = true;

    return status;
    }


bool CPQDIF_E_Scalar::GetValueTimeStamp( TIMESTAMPPQDIF& value ) const
    {
    bool    status = false;

    if( m_typePhysical == ID_PHYS_TYPE_TIMESTAMPPQDIF )
        {
        value = m_value.ts;
        status = true;
        }

    return status;
    }
