/*
**  Class:          CPQDIF_E_Vector
**  Description:    Implements the PQDIF vector element
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


const int defaultGrowBy = 4;

//  Construction
//  ============

CPQDIF_E_Vector::CPQDIF_E_Vector()
{
    m_array.SetSize( 0, defaultGrowBy );
}


CPQDIF_E_Vector::~CPQDIF_E_Vector()
{
}


void CPQDIF_E_Vector::SetPhysicalType( long type )
{
    long    count = m_array.GetSize() / getNumBytesOfType();

    if( m_typePhysical != type )
    {
        //  Yup, it's changed!
        m_typePhysical = type; 

        //  ... and realloc space for the new type.
        SetCount( count );
    
        //  Signal that the record changed.
        if( m_pRecord )
            m_pRecord->SetChanged( true );
    }
}

        
#ifdef PQDIF_USE_COM
bool CPQDIF_E_Vector::SetValue( long index, VARIANT & value )
    {
    bool        status = false;
    PQDIFValue  valPQ;

    //  Copy over the information
    status = convertFromVariant( value, valPQ );
    if( status )
    {
        SetValue( index, valPQ );
    }

    return status;
    }

bool CPQDIF_E_Vector::GetValue( long index, VARIANT & value ) const
    {
    bool        status = false;
    PQDIFValue  valPQ;
    long    nItem;
    GetCount(nItem);

    if( index >= 0 && index < nItem )
    {
        status = GetValue( index, valPQ );
        if( status )
        {
            status = convertToVariant( valPQ, value );
        }
    }

    return status;
    }
#endif

bool CPQDIF_E_Vector::SetValue( long index, const PQDIFValue& value )
{
    bool    status = false;
    BYTE *  pvalSource = (BYTE *) getPointerToValue( value );
    BYTE *  pvalTarget = (BYTE *) getPointer( index );

    if( pvalSource && pvalTarget )
    {
        memcpy( pvalTarget, pvalSource, getNumBytesOfType() );
        status = true;
    }

    return status;
}


bool CPQDIF_E_Vector::GetValue( long index, PQDIFValue& value ) const
{
    bool    status = false;
    BYTE *  pvalTarget = (BYTE *) getPointerToValue( value );
    BYTE *  pvalSource = (BYTE *) getPointer( index );

    if( pvalSource && pvalTarget )
    {
        memcpy( pvalTarget, pvalSource, getNumBytesOfType() );
        status = true;
    }

    return status;
}


bool CPQDIF_E_Vector::SetValue( long index, double value )
{
    bool        status;
    PQDIFValue  valuePQ;

    status = convertFromDouble( value, valuePQ );
    if( status )
    {
        status = SetValue( index, valuePQ );
    }

    return status;
}


bool CPQDIF_E_Vector::GetValue( long index, double& value ) const
{
    bool        status;
    PQDIFValue  valuePQ;

    status = GetValue( index, valuePQ );
    if( status )
    {
        status = convertToDouble( valuePQ, value );
    }

    return status;
}



bool CPQDIF_E_Vector::SetValues( const char * text )
{
    bool    status = false;
    long    idxItem;
    long    countItems;
    BYTE *  data = NULL;

    //  Init
    countItems = static_cast<long>(strlen( text )) + 1;

    switch( GetPhysicalType() )
    {
        case ID_PHYS_TYPE_CHAR1:
            SetCount( countItems );
            data = GetRawData();
            if( data )
            {
                //  Copy the string across directly
                memcpy( data, text, countItems - 1 );

                //  Make sure it's null-terminated
                data[ countItems - 1 ] = (BYTE) 0;
                status = true;
            }
            break;

        case ID_PHYS_TYPE_CHAR2:
            SetCount( countItems );
            data = GetRawData();
            if( data )
            {
                CHAR2 * temp = (CHAR2 *) data;

                //  Copy the data across by physical item size
                //  Don't copy the last character -- should be a NULL
                for( idxItem = 0; idxItem < ( countItems - 1); idxItem++ )
                {
                    //  Copy each value
                    *temp = (CHAR2) text[ (int) idxItem ];
                    temp++;
                }

                //  NULL-terminate it
                *temp = (CHAR2) 0;
                status = true;
            }
            break;

        default:
            status = false;
            break;
    }

    return status;
}


bool CPQDIF_E_Vector::GetValues( string& strOutput ) const
{
            bool    status = false;
            long    idxItem;
            long    countItems;
    const   BYTE *  data = NULL;

    //  Init
    GetCount( countItems );
    data = GetRawData();

    switch( GetPhysicalType() )
    {
        case ID_PHYS_TYPE_CHAR1:
            if( data )
            {
                //  Copy the data across by physical item size
                //  Don't copy the last character -- should be a NULL
                strOutput = (const char *) data;
                status = TRUE;
            }
            break;

        case ID_PHYS_TYPE_CHAR2:
            if( data )
            {
                CHAR2 * temp = (CHAR2 *) data;

                //  Copy the data across by physical item size
                //  Don't copy the last character -- should be a NULL
                for( idxItem = 0; idxItem < ( countItems - 1); idxItem++ )
                {
                    //  Copy across a single value
                    strOutput += (char) ( *temp );

                    //  Next!
                    temp++;
                }

                status = TRUE;
            }
            break;

        default:
            status = false;
            break;
    }

    return status;
}


BYTE * CPQDIF_E_Vector::GetRawData( void )
{
    return m_array.GetData();
}


const BYTE * CPQDIF_E_Vector::GetRawData( void ) const
{
    return m_array.GetData();
}


bool CPQDIF_E_Vector::GetCount( long& count ) const
{
    bool    status = false;

    count = m_array.GetSize() / getNumBytesOfType();
    status = true;

    return status;
}


bool CPQDIF_E_Vector::SetCount( long count )
{
    bool    status = false;

    //  Resize the array.
	int size = count * getNumBytesOfType();
    m_array.SetSize( size, defaultGrowBy );
    status = m_array.GetSize() == size;

    return status;
}


long CPQDIF_E_Vector::GetSizeBytes( void )
{
    return m_array.GetSize();
}


BYTE * CPQDIF_E_Vector::getPointer( int idx )
{
    BYTE *  ptr = NULL;
    long    count = m_array.GetSize() / getNumBytesOfType();

    ptr = m_array.GetData();
    if( ptr && idx >= 0 && idx < count )
    {
        ptr += ( idx * getNumBytesOfType() );
    }

    return ptr;
}


const BYTE * CPQDIF_E_Vector::getPointer( int idx ) const
{
    const   BYTE *  ptr = NULL;
            long    count = m_array.GetSize() / getNumBytesOfType();

    ptr = m_array.GetData();
    if( ptr && idx >= 0 && idx < count )
    {
        ptr += ( idx * getNumBytesOfType() );
    }

    return ptr;
}


//  Type-specific member functions
//  ------------------------------

#define ELVECT_SETVALUE( nametype, type, idtype ) \
void CPQDIF_E_Vector::SetValue##nametype( long index, type value ) \
{ \
    bool    status = false; \
    type *  pval = NULL; \
    SetPhysicalType( idtype ); \
    pval = (type *) getPointer( index ); \
    if( pval ) \
    { \
        *pval = value; \
        status = true; \
    } \
}


#define ELVECT_GETVALUE( nametype, type, idtype ) \
bool CPQDIF_E_Vector::GetValue##nametype( long index, type& value ) const \
{   bool    status = false; \
    type *  pval = NULL; \
    if( GetPhysicalType() == idtype ) \
    { \
        pval = (type *) getPointer( index ); \
        if( pval ) \
        { \
            value = *pval; \
            status = true; \
        } \
    } \
    return status; }


#define ELVECT_SETVALUES_ARRAY( nametype, type, idtype ) \
void CPQDIF_E_Vector::SetValues##nametype( const type * values, long count ) \
{   bool    status = false; \
    type *  pval = NULL; \
    SetPhysicalType( idtype ); \
    SetCount( count ); \
    pval = (type *) getPointer( 0 ); \
    if( pval ) \
    { \
        memcpy( pval, values, count * sizeof( type ) ); \
        status = true; \
    } \
} 


#define ELVECT_GETVALUES_ARRAY( nametype, type, idtype ) \
long CPQDIF_E_Vector::GetValues##nametype( type * values, long max ) const \
{   long    countActual = 0; \
    type *  pval = NULL; \
    long    sizeType = getNumBytesOfType(); \
    long    count = m_array.GetSize() / sizeType; \
    if( GetPhysicalType() == idtype ) \
    { \
        pval = (type *) getPointer( 0 ); \
        if( pval ) \
        { \
            countActual  = min( max, count ); \
            memcpy( values, pval, countActual * sizeType ); \
        } \
    } \
    return countActual; }


ELVECT_SETVALUE       ( INT1, INT1, ID_PHYS_TYPE_INTEGER1 )
ELVECT_SETVALUES_ARRAY( INT1, INT1, ID_PHYS_TYPE_INTEGER1 )
ELVECT_GETVALUE       ( INT1, INT1, ID_PHYS_TYPE_INTEGER1 )
ELVECT_GETVALUES_ARRAY( INT1, INT1, ID_PHYS_TYPE_INTEGER1 )

ELVECT_SETVALUE       ( INT2, INT2, ID_PHYS_TYPE_INTEGER2 )
ELVECT_SETVALUES_ARRAY( INT2, INT2, ID_PHYS_TYPE_INTEGER2 )
ELVECT_GETVALUE       ( INT2, INT2, ID_PHYS_TYPE_INTEGER2 )
ELVECT_GETVALUES_ARRAY( INT2, INT2, ID_PHYS_TYPE_INTEGER2 )

ELVECT_SETVALUE       ( INT4, INT4, ID_PHYS_TYPE_INTEGER4 )
ELVECT_SETVALUES_ARRAY( INT4, INT4, ID_PHYS_TYPE_INTEGER4 )
ELVECT_GETVALUE       ( INT4, INT4, ID_PHYS_TYPE_INTEGER4 )
ELVECT_GETVALUES_ARRAY( INT4, INT4, ID_PHYS_TYPE_INTEGER4 )

ELVECT_SETVALUE       ( UINT4, UINT4, ID_PHYS_TYPE_UNS_INTEGER4 )
ELVECT_SETVALUES_ARRAY( UINT4, UINT4, ID_PHYS_TYPE_UNS_INTEGER4 )
ELVECT_GETVALUE       ( UINT4, UINT4, ID_PHYS_TYPE_UNS_INTEGER4 )
ELVECT_GETVALUES_ARRAY( UINT4, UINT4, ID_PHYS_TYPE_UNS_INTEGER4 )

ELVECT_SETVALUE       ( REAL4, REAL4, ID_PHYS_TYPE_REAL4 )
ELVECT_SETVALUES_ARRAY( REAL4, REAL4, ID_PHYS_TYPE_REAL4 )
ELVECT_GETVALUE       ( REAL4, REAL4, ID_PHYS_TYPE_REAL4 )
ELVECT_GETVALUES_ARRAY( REAL4, REAL4, ID_PHYS_TYPE_REAL4 )

ELVECT_SETVALUE       ( REAL8, REAL8, ID_PHYS_TYPE_REAL8 )
ELVECT_SETVALUES_ARRAY( REAL8, REAL8, ID_PHYS_TYPE_REAL8 )
ELVECT_GETVALUE       ( REAL8, REAL8, ID_PHYS_TYPE_REAL8 )
ELVECT_GETVALUES_ARRAY( REAL8, REAL8, ID_PHYS_TYPE_REAL8 )

ELVECT_SETVALUE       ( TimeStamp, TIMESTAMPPQDIF, ID_PHYS_TYPE_TIMESTAMPPQDIF )
ELVECT_SETVALUES_ARRAY( TimeStamp, TIMESTAMPPQDIF, ID_PHYS_TYPE_TIMESTAMPPQDIF )
ELVECT_GETVALUE       ( TimeStamp, TIMESTAMPPQDIF, ID_PHYS_TYPE_TIMESTAMPPQDIF )
ELVECT_GETVALUES_ARRAY( TimeStamp, TIMESTAMPPQDIF, ID_PHYS_TYPE_TIMESTAMPPQDIF )

