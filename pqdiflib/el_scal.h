/*
**  Class:          CPQDIF_E_Scalar
**  Description:
**
** --------------------------------------------------------------------------
**
** Copyright 2021 PQDIF Authors
**
** Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
** either express or implied. See the License for the specific language governing permissions and limitations under the License.
**
** See the LICENSE file distributed with this work for copyright and licensing information, the AUTHORS file for a list of
** copyright holders, and the CONTRIBUTORS file for the list of contributors.
**
** SPDX - License - Identifier: Apache - 2.0
*/
class CPQDIF_E_Scalar : public CPQDIF_Element
    {
    public:
        CPQDIF_E_Scalar();
        virtual ~CPQDIF_E_Scalar();

    //  Operator(s)
    public:

    // Attributes
    public:
        virtual long GetElementType( void ) const
            { return ID_ELEMENT_TYPE_SCALAR; }

    // Operations
    public:
#ifdef PQDIF_USE_COM
        bool SetValue( long typePhysical, VARIANT & value );
        bool GetValue( VARIANT& value );
#endif
        bool SetValue( long typePhysical, PQDIFValue& value );
        bool GetValue( long& typePhysical, PQDIFValue& value ) const;

        //  Specific physical types
        bool SetValueUINT2( UINT2 value );
        bool GetValueUINT2( UINT2& value ) const;
        bool SetValueINT2( INT2 value );
        bool GetValueINT2( INT2& value ) const;
        bool SetValueUINT4( UINT4 value );
        bool GetValueUINT4( UINT4& value ) const;
        bool SetValueINT4( INT4 value );
        bool GetValueINT4( INT4& value ) const;
        bool SetValueBOOL4( bool value );
        bool GetValueBOOL4( bool& value ) const;
        bool SetValueREAL4( REAL4 value );
        bool GetValueREAL4( REAL4& value ) const;
        bool SetValueREAL8( REAL8 value );
        bool GetValueREAL8( REAL8& value ) const;
        bool SetValueCOMPLEX8( COMPLEX8 value );
        bool GetValueCOMPLEX8( COMPLEX8& value ) const;
        bool SetValueCOMPLEX16( COMPLEX16 value );
        bool GetValueCOMPLEX16( COMPLEX16& value ) const;
        bool SetValueGUID( GUID value );
        bool GetValueGUID( GUID& value ) const;
        bool SetValueTimeStamp( TIMESTAMPPQDIF value );
        bool GetValueTimeStamp( TIMESTAMPPQDIF& value ) const;

    // Implementation
    private:

    //  Member data
    private:
        PQDIFValue  m_value;
    };
