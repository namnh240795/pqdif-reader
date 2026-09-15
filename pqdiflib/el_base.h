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

union PQDIFValue
    {
    BOOL1       bool1;
    BOOL2       bool2;
    BOOL4       bool4;

    CHAR1       char1;  //  ASCII string character
    CHAR2       char2;  //  Unicode string character

    INT1        int1;
    INT2        int2;
    INT4        int4;

    UINT1       uint1;
    UINT2       uint2;
    UINT4       uint4;

    REAL4           real4;
    REAL8           real8;
    COMPLEX8        complex8;
    COMPLEX16       complex16;

    TIMESTAMPPQDIF  ts;
    
    GUID            guid;
    };


class CPQDIF_Element
    {
    //  Construct/destruct
    public:
        CPQDIF_Element();
        virtual ~CPQDIF_Element() {}

    //  Operator(s)
    public:

    // Attributes
    public:
        virtual long GetElementType( void ) const
            {return -1; }
        
        inline CPQDIFRecord * GetRecord( void )
            {
            return m_pRecord;
            }
        virtual void SetRecord( CPQDIFRecord * pRecord )
            {
            m_pRecord = pRecord;
            }

    // Operations
    public:
        inline const GUID& GetTag( void ) const
            { return m_tag; }
        inline void SetTag( const GUID& tag )
            {
            m_tag = tag;
            if( m_pRecord )
                m_pRecord->SetChanged( true ); 
            }
        inline long GetPhysicalType( void ) const
            { return m_typePhysical; }
        virtual void SetPhysicalType( long type )
            {
            m_typePhysical = type; 
            if( m_pRecord )
                m_pRecord->SetChanged( true ); 
            }

    // Implementation
    protected:
#ifdef PQDIF_USE_COM  // if COM support, then VARIANT is available
        bool convertToVariant( const PQDIFValue& valuePQDIF, VARIANT & valueVariant ) const;
        bool convertFromVariant( const VARIANT & valueVariant, PQDIFValue& valuePQDIF ) const;
#endif
        bool convertToDouble( const PQDIFValue& m_value, double& value ) const;
        bool convertFromDouble( double value, PQDIFValue& m_value ) const;
        long getNumBytesOfType( void ) const;
        inline BYTE * getPointerToValue( PQDIFValue& value ) const { return (BYTE *)&value; }
        inline const BYTE * getPointerToValue( const PQDIFValue& value ) const { return (const BYTE *)&value; }
    
    //  Member data
    protected:
        GUID            m_tag;          //  Tag for this element.
        long            m_typePhysical; //  Physical type for this element.
        CPQDIFRecord *  m_pRecord;      //  The record that this element
                                        //  is associated with.
    };
