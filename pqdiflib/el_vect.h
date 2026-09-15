/*
**  Class:          CPQDIF_E_Vector
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

class CPQDIF_E_Vector : public CPQDIF_Element
    {
    public:
        CPQDIF_E_Vector();
        virtual ~CPQDIF_E_Vector();

    //  Operator(s)
    public:

    // Attributes
    public:
        virtual long GetElementType( void ) const
            { return ID_ELEMENT_TYPE_VECTOR; }
        virtual void SetPhysicalType( long type );
        bool GetCount( long& count ) const;
        bool SetCount( long count );
        long GetSizeBytes( void );

    // Operations
    public:
#ifdef PQDIF_USE_COM
        bool SetValue( long index, VARIANT & value );
        bool GetValue( long index, VARIANT & value ) const;
#endif
        bool SetValue( long index, const PQDIFValue& value );
        bool GetValue( long index, PQDIFValue& value ) const;
        bool SetValue( long index, double value );
        bool GetValue( long index, double& value ) const;

    //  Specific types
        void SetValueINT1 ( long idx, INT1 value );
        void SetValuesINT1 ( const INT1 * array, long count );
        bool GetValueINT1 ( long idx, INT1& value ) const;
        long GetValuesINT1 ( INT1 * array, long max ) const;

        void SetValueINT2 ( long idx, INT2 value );
        void SetValuesINT2 ( const INT2 * array, long count );
        bool GetValueINT2 ( long idx, INT2& value ) const;
        long GetValuesINT2 ( INT2 * array, long max ) const;

        void SetValueINT4 ( long idx, INT4 value );
        void SetValuesINT4 ( const INT4 * array, long count );
        bool GetValueINT4 ( long idx, INT4& value ) const;
        long GetValuesINT4 ( INT4 * array, long max ) const;

        void SetValueUINT4( long index, UINT4 value );
        void SetValuesUINT4 ( const UINT4 * array, long count );
        bool GetValueUINT4( long index, UINT4& value ) const;
        long GetValuesUINT4 ( UINT4 * array, long max ) const;

        void SetValueREAL4 ( long idx, REAL4 value );
        void SetValuesREAL4 ( const REAL4 * array, long count );
        bool GetValueREAL4 ( long idx, REAL4& value ) const;
        long GetValuesREAL4 ( REAL4 * array, long max ) const;

        void SetValueREAL8 ( long idx, REAL8 value );
        void SetValuesREAL8 ( const REAL8 * array, long count );
        bool GetValueREAL8 ( long idx, REAL8& value ) const;
        long GetValuesREAL8 ( REAL8 * array, long max ) const;

        void SetValueTimeStamp ( long idx, TIMESTAMPPQDIF value );
        void SetValuesTimeStamp ( const TIMESTAMPPQDIF * array, long count );
        bool GetValueTimeStamp ( long idx, TIMESTAMPPQDIF& value ) const;
        long GetValuesTimeStamp ( TIMESTAMPPQDIF * array, long max ) const;

        bool SetValues( const char * text );
        bool GetValues( string& text ) const;

        BYTE * GetRawData( void );
        const BYTE * GetRawData( void ) const;

    // Implementation
    protected:
        BYTE * getPointer( int idx );
        const BYTE * getPointer( int idx ) const;

    //  Member data
    private:
        CPQByteArray    m_array;
    };
