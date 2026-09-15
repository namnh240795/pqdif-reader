/*
**  Class:          CPQDIF_Element
**  Description:
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

CPQDIF_Element::CPQDIF_Element()
{
    m_pRecord = NULL;
    memset( &m_tag, 0, sizeof( m_tag ) );
    m_typePhysical = 0;
}


#ifdef PQDIF_USE_COM
bool CPQDIF_Element::convertFromVariant( const VARIANT & value, PQDIFValue& m_value ) const
    {
    bool    status = true;
    INT1    val_int1 = 0;
    INT2    val_int2 = 0;
    INT4    val_int4 = 0;

    //  First, verify the variant physical type matches
    //  (and pull out the integer values)
    status = TRUE;
    switch( m_typePhysical )
        {
        case ID_PHYS_TYPE_BOOLEAN1           :
        case ID_PHYS_TYPE_BOOLEAN2           :
        case ID_PHYS_TYPE_BOOLEAN4           :
            if( value.vt != VT_BOOL )
                status = false;
            break;

        case ID_PHYS_TYPE_CHAR1              :
        case ID_PHYS_TYPE_INTEGER1           :
        case ID_PHYS_TYPE_UNS_INTEGER1       :
            switch( value.vt )
                {
                case VT_UI1:
                    val_int1 = (INT1) value.bVal;
                    break;

                case VT_I1:
                    val_int1 = (INT1) value.cVal;
                    break;

                default:
                    status = false;
                }
            break;

        case ID_PHYS_TYPE_CHAR2              :
        case ID_PHYS_TYPE_INTEGER2           :
        case ID_PHYS_TYPE_UNS_INTEGER2       :
            switch( value.vt )
                {
                case VT_UI2:
                    val_int2 = (INT2) value.uiVal;
                    break;

                case VT_I2:
                    val_int2 = (INT2) value.iVal;
                    break;

                default:
                    status = false;
                }
            break;

        case ID_PHYS_TYPE_INTEGER4           :
        case ID_PHYS_TYPE_UNS_INTEGER4       :
            switch( value.vt )
                {
                case VT_UI4:
                    val_int4 = (INT4) value.ulVal;
                    break;

                case VT_I4:
                    val_int4 = (INT4) value.lVal;
                    break;

                default:
                    status = false;
                }
            break;

        //  Real/complex
        case ID_PHYS_TYPE_REAL4              :
            if( value.vt != VT_R4 )
                status = false;
            break;

        case ID_PHYS_TYPE_REAL8              :
            if( value.vt != VT_R4 )
                status = false;
            break;

        case ID_PHYS_TYPE_COMPLEX8           :
        case ID_PHYS_TYPE_COMPLEX16          :
            //if( value.vt == VT_ARRAY ) ???
            break;

        case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
            if( value.vt != VT_DATE )
                status = false;
            break;

        case ID_PHYS_TYPE_GUID               :
            //if( value.vt == VT_ARRAY ) ???
            break;

        default:
            status = false;
            break;
        }

    if( status )
        {
        switch( m_typePhysical )
            {
            case ID_PHYS_TYPE_BOOLEAN1           :
                m_value.bool1 = (BOOL1) value.boolVal;
            case ID_PHYS_TYPE_BOOLEAN2           :
                m_value.bool2 = (BOOL2) value.boolVal;
            case ID_PHYS_TYPE_BOOLEAN4           :
                m_value.bool4 = (BOOL4) value.boolVal;
                break;

            case ID_PHYS_TYPE_CHAR1              :
                m_value.char1 = (CHAR1) val_int1;
                break;
            case ID_PHYS_TYPE_INTEGER1           :
                m_value.int1 = (INT1) val_int1;
                break;
            case ID_PHYS_TYPE_UNS_INTEGER1       :
                m_value.uint1 = (UINT1) val_int1;
                break;

            case ID_PHYS_TYPE_CHAR2              :
                m_value.char2 = (CHAR2) val_int2;
                break;
            case ID_PHYS_TYPE_INTEGER2           :
                m_value.int2 = (INT2) val_int2;
                break;
            case ID_PHYS_TYPE_UNS_INTEGER2       :
                m_value.uint2 = (UINT2) val_int2;
                break;

            case ID_PHYS_TYPE_INTEGER4           :
                m_value.int4 = (INT4) val_int4;
                break;
            case ID_PHYS_TYPE_UNS_INTEGER4       :
                m_value.uint4 = (UINT4) val_int4;
                break;

            //  Real/complex
            case ID_PHYS_TYPE_REAL4              :
                m_value.real4 = (REAL4) value.fltVal;
                break;
            case ID_PHYS_TYPE_REAL8              :
                m_value.real8 = (REAL8) value.dblVal;
                break;

            case ID_PHYS_TYPE_COMPLEX8           :
            case ID_PHYS_TYPE_COMPLEX16          :
                //  need to fix
                //  SAFEARRAY?
                break;

            //  Date/time
            case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                //  From the documentation, it appears that the 
                //  whole number portion of the DATE is compatible
                //  with our .day member. (Although it is possible that
                //  they will be off by 1 day...)
                m_value.ts.day = (UINT4) ( value.date );

                //  Convert the fractional day to fractional seconds.
                m_value.ts.sec = (REAL8) ( value.date - (DATE) m_value.ts.day );
                m_value.ts.sec *= (REAL8) SECONDS_PER_DAY;
                break;

            //  GUID
            case ID_PHYS_TYPE_GUID               :
                //  need to fix
                //  SAFEARRAY?
                break;

            default:
                status = false;
                break;
            }
        }

    return status;
    }


bool CPQDIF_Element::convertToVariant( const PQDIFValue& m_value, VARIANT & value ) const
    {
    bool    status = true;
    string  strRep;

    VariantInit( &value );

    USES_CONVERSION;

    if( status )
        {
        switch( m_typePhysical )
            {
            case ID_PHYS_TYPE_BOOLEAN1           :
                value.vt = VT_BOOL;
                value.boolVal = m_value.bool1;
                break;

            case ID_PHYS_TYPE_CHAR1              :
                value.vt = VT_I1;
                value.cVal = m_value.char1;
                break;

            case ID_PHYS_TYPE_INTEGER1           :
                value.vt = VT_I1;
                value.cVal = m_value.int1;
                break;

            case ID_PHYS_TYPE_UNS_INTEGER1       :
                //  VT_UI1 / value.bVal
                value.vt = VT_I1;
                value.cVal = m_value.uint1;
                break;

            case ID_PHYS_TYPE_BOOLEAN2           :
                value.vt = VT_BOOL;
                value.boolVal = m_value.bool2;
                break;

            case ID_PHYS_TYPE_CHAR2              :
                //  Single-char Unicode string instead?
                value.vt = VT_I2;
                value.iVal = m_value.char2;
                break;
            
            case ID_PHYS_TYPE_INTEGER2           :
                value.vt = VT_I2;
                value.iVal = m_value.int2;
                break;

            case ID_PHYS_TYPE_UNS_INTEGER2       :
                //VT_UI2 / uiVal
                value.vt = VT_I2;
                value.iVal = m_value.uint2 ;
                break;

            case ID_PHYS_TYPE_BOOLEAN4           :
                value.vt = VT_BOOL;
                value.boolVal = (VARIANT_BOOL) m_value.bool4;
                break;

            case ID_PHYS_TYPE_INTEGER4           :
                value.vt = VT_I4;
                value.lVal = m_value.int4;
                break;

            case ID_PHYS_TYPE_UNS_INTEGER4       :
                //  VT_UI4 / ulVal
                value.vt = VT_I4;
                value.lVal = m_value.uint4;
                break;

            //  Real/complex
            case ID_PHYS_TYPE_REAL4              :
                value.vt = VT_R4;
                value.fltVal = m_value.real4;
                break;

            case ID_PHYS_TYPE_REAL8              :
                value.vt = VT_R8;
                value.dblVal = m_value.real8;
                break;

            case ID_PHYS_TYPE_COMPLEX8           :
            case ID_PHYS_TYPE_COMPLEX16          :
                //  need to fix
                //  SAFEARRAY?
                //value.vt = VT_ARRAY;
                //value.parray = NULL;

                //  In string form ("real imag")
//                strRep = _T( "" );
                strRep = "";
                char szBuffer[64];

                if( m_typePhysical == ID_PHYS_TYPE_COMPLEX8 )
                    {
                    sprintf( szBuffer, "%.8g %.8g",
                        m_value.complex8.real,
                        m_value.complex8.image );
                    strRep = szBuffer;
                    }
                else
                    {
                    sprintf( szBuffer, "%.16g %.16g",
                        m_value.complex16.real,
                        m_value.complex16.image );
                    strRep = szBuffer;
                    }

                value.vt = VT_BSTR;
#if _MSC_VER >= 1100
                value.bstrVal = SysAllocString(_bstr_t(strRep.c_str()));
#else
                value.bstrVal = SysAllocString(A2W(strRep.c_str()));
#endif
                break;

            //  Date/time
            case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                value.vt = VT_DATE;
                value.date = (double) m_value.ts.day
                        +  ( (double) m_value.ts.sec / (double) SECONDS_PER_DAY );
                break;

            //  GUID
            case ID_PHYS_TYPE_GUID               :
                status = theSupport.NewVariantArrayFromGUID( value, m_value.guid );
                //  need to fix
                //  SAFEARRAY?
                //value.vt = VT_ARRAY;
                //value.parray = NULL;
                //unsigned char ** pstrRPC;
                //  Generate a GUID string
                //UuidToString( &m_value.guid, pstrRPC );
                //RpcStringFree( pstrRPC );

                //  String in this format:
                //  {89738618-F1C3-11cf-9D89-0080C72E70A3}
                //{
                //char * pstr = strRep.GetBuffer( 64 );
                //sprintf( pstr, "{%08.8lx-%04.4x-%04.4x-%02.2x%02.2x%02.2x%02.2x}",
                //    m_value.guid.Data1,
                //    m_value.guid.Data2,
               //     m_value.guid.Data3,
                //    (short) m_value.guid.Data4[ 0 ],
                //    (short) m_value.guid.Data4[ 1 ],
                //    (short) m_value.guid.Data4[ 2 ],
                //    (short) m_value.guid.Data4[ 3 ]
                //    );
                //strRep.ReleaseBuffer();
                //}
                //  Return it back in the variant...
                //value.vt = VT_BSTR | VT_BYREF;
                //*value.pbstrVal = strRep.AllocSysString();
                //value.vt = VT_BSTR;
                //value.bstrVal = strRep.AllocSysString();
                break;

            default:
                status = false;
                break;
            }
        }

    return status;
    }
#endif

bool CPQDIF_Element::convertFromDouble( double valueSource, PQDIFValue& valueTarget ) const
{
    bool    status = true;

    //  First, verify the variant physical type matches
    //  (and pull out the integer values)
    if( status )
    {
        switch( m_typePhysical )
        {
            case ID_PHYS_TYPE_BOOLEAN1           :
                valueTarget.bool1 = (BOOL1) valueSource;
                break;
            case ID_PHYS_TYPE_BOOLEAN2           :
                valueTarget.bool2 = (BOOL2) valueSource;
                break;
            case ID_PHYS_TYPE_BOOLEAN4           :
                valueTarget.bool4 = (BOOL4) valueSource;
                break;

            case ID_PHYS_TYPE_CHAR1              :
                valueTarget.char1 = (CHAR1) valueSource;
                break;
            case ID_PHYS_TYPE_INTEGER1           :
                valueTarget.int1 = (INT1) valueSource;
                break;
            case ID_PHYS_TYPE_UNS_INTEGER1       :
                valueTarget.uint1 = (UINT1) valueSource;
                break;

            case ID_PHYS_TYPE_CHAR2              :
                valueTarget.char2 = (CHAR2) valueSource;
                break;
            case ID_PHYS_TYPE_INTEGER2           :
                valueTarget.int2 = (INT2) valueSource;
                break;
            case ID_PHYS_TYPE_UNS_INTEGER2       :
                valueTarget.uint2 = (UINT2) valueSource;
                break;

            case ID_PHYS_TYPE_INTEGER4           :
                valueTarget.int4 = (INT4) valueSource;
                break;
            case ID_PHYS_TYPE_UNS_INTEGER4       :
                valueTarget.uint4 = (UINT4) valueSource;
                break;

            //  Real/complex
            case ID_PHYS_TYPE_REAL4              :
                valueTarget.real4 = (REAL4) valueSource;
                break;
            case ID_PHYS_TYPE_REAL8              :
                valueTarget.real8 = (REAL8) valueSource;
                break;

            case ID_PHYS_TYPE_COMPLEX8           :
                valueTarget.complex8.real = (REAL4) valueSource;
                valueTarget.complex8.image = 0.0;
                break;

            case ID_PHYS_TYPE_COMPLEX16          :
                valueTarget.complex16.real = (REAL4) valueSource;
                valueTarget.complex16.image = 0.0;
                break;

            //  Date/time
            case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                //  From the documentation, it appears that the 
                //  whole number portion of the DATE is compatible
                //  with our .day member. (Although it is possible that
                //  they will be off by 1 day...)
                valueTarget.ts.day = (UINT4) ( valueSource );

                //  Convert the fractional day to fractional seconds.
                valueTarget.ts.sec = (REAL8) ( valueSource - (double) valueTarget.ts.day );
                valueTarget.ts.sec *= (REAL8) SECONDS_PER_DAY;
                break;

            //  GUID
            case ID_PHYS_TYPE_GUID               :
                //  Cannot convert from double.
                break;

            default:
                status = false;
                break;
        }
    }

    return status;
}


bool CPQDIF_Element::convertToDouble( const PQDIFValue& valueSource, double& valueTarget ) const
{
    bool    status = true;

    valueTarget = 0.0;

    if( status )
    {
        switch( m_typePhysical )
        {
            case ID_PHYS_TYPE_BOOLEAN1           :
                valueTarget = (double) valueSource.bool1;
                break;

            case ID_PHYS_TYPE_CHAR1              :
                valueTarget = (double) valueSource.char1;
                break;

            case ID_PHYS_TYPE_INTEGER1           :
                valueTarget = (double) valueSource.int1;
                break;

            case ID_PHYS_TYPE_UNS_INTEGER1       :
                valueTarget = (double) valueSource.uint1;
                break;

            case ID_PHYS_TYPE_BOOLEAN2           :
                valueTarget = (double) valueSource.bool2;
                break;

            case ID_PHYS_TYPE_CHAR2              :
                //  Single-char Unicode string instead?
                valueTarget = (double) valueSource.char2;
                break;
            
            case ID_PHYS_TYPE_INTEGER2           :
                valueTarget = (double) valueSource.int2;
                break;

            case ID_PHYS_TYPE_UNS_INTEGER2       :
                valueTarget = (double) valueSource.uint2 ;
                break;

            case ID_PHYS_TYPE_BOOLEAN4           :
                valueTarget = (double) valueSource.bool4;
                break;

            case ID_PHYS_TYPE_INTEGER4           :
                valueTarget = (double) valueSource.int4;
                break;

            case ID_PHYS_TYPE_UNS_INTEGER4       :
                valueTarget = (double) valueSource.uint4;
                break;

            //  Real/complex
            case ID_PHYS_TYPE_REAL4              :
                valueTarget = (double) valueSource.real4;
                break;

            case ID_PHYS_TYPE_REAL8              :
                valueTarget = (double) valueSource.real8;
                break;

            case ID_PHYS_TYPE_COMPLEX8           :
            case ID_PHYS_TYPE_COMPLEX16          :
                //  Cannot convert to double.
                break;

            //  Date/time
            case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                //valueTarget.vt = VT_DATE;
                valueTarget = (double) valueSource.ts.day
                        +  ( (double) valueSource.ts.sec / (double) SECONDS_PER_DAY );
                break;

            //  GUID
            case ID_PHYS_TYPE_GUID               :
                //  Cannot convert to double.
                break;

            default:
                status = false;
                break;
        }
    }

    return status;
}


long CPQDIF_Element::getNumBytesOfType( void ) const 
{ 
    return theInfo.GetNumBytesOfType( m_typePhysical ); 
}

