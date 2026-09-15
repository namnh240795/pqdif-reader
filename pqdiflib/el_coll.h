/*
**  Class:          
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


typedef vector<CPQDIF_Element *> CArrayElements;


class CPQDIF_E_Collection : public CPQDIF_Element
    {
    public:
        CPQDIF_E_Collection();
        virtual ~CPQDIF_E_Collection();

    //  Operator(s)
    public:

    // Attributes
    public:
        virtual long GetElementType( void ) const
            { return ID_ELEMENT_TYPE_COLLECTION; }

        virtual void SetRecord( CPQDIFRecord * pRecord );

    // Operations
    public:
        //  Read
        long GetCount( void ) const;
        CPQDIF_Element * GetElement( long index ) const;
        CPQDIF_Element * GetElement( const GUID& tag, long elementType = -1 ) const;

        //  Modify
        void Add( CPQDIF_Element * pel );
        void RemoveAt( long index );
        void AddOrReplace( CPQDIF_Element * pel );

        //  Set vectors
        void SetVectorString
            ( 
            const   GUID&   tagElement,
            const   char *  text,
                    bool    allowReplace = true
            );
        void SetVectorINT1
            ( 
            const   GUID&   tagElement,
            const   INT1 *  values,
                    long    count,
                    bool    allowReplace = true
            );
        void SetVectorINT2
            ( 
            const   GUID&   tagElement,
            const   INT2 *  values,
                    long    count,
                    bool    allowReplace = true
            );
        void SetVectorINT4
            ( 
            const   GUID&   tagElement,
            const   INT4 *  values,
                    long    count,
                    bool    allowReplace = true
            );
        void SetVectorUINT4
            ( 
            const   GUID&   tagElement,
            const   UINT4 * values,
                    long    count,
                    bool    allowReplace = true
            );
        void SetVectorREAL4
            ( 
            const   GUID&   tagElement,
            const   REAL4 * values,
                    long    count,
                    bool    allowReplace = true
            );
        void SetVectorREAL8
            ( 
            const   GUID&   tagElement,
            const   REAL8 * values,
                    long    count,
                    bool    allowReplace = true
            );
        void SetVectorTimeStamp
            ( 
            const   GUID&               tagElement,
            const   TIMESTAMPPQDIF *    values,
                    long                count,
                    bool                allowReplace = true
            );

        //  Get vectors
        bool GetVectorString
            ( 
            const   GUID&   tagElement,
                    char *  text,
                    long    max
            ) const;
        long GetVectorINT1
            ( 
            const   GUID&   tagElement,
                    INT1 *  values,
                    long    max
            ) const;
        long GetVectorINT2
            ( 
            const   GUID&   tagElement,
                    INT2 *  values,
                    long    max
            ) const;
        long GetVectorINT4
            ( 
            const   GUID&   tagElement,
                    INT4 *  values,
                    long    max
            ) const;
        long GetVectorUINT4
            ( 
            const   GUID&   tagElement,
                    UINT4 * values,
                    long    max
            ) const;
        long GetVectorREAL4
            ( 
            const   GUID&   tagElement,
                    REAL4 * values,
                    long    max
            ) const;
        long GetVectorREAL8
            ( 
            const   GUID&   tagElement,
                    REAL8 * values,
                    long    max
            ) const;
        long GetVectorTimeStamp
            ( 
            const   GUID&               tagElement,
                    TIMESTAMPPQDIF *    values,
                    long                max
            ) const;

        //  Set scalars
        void SetScalarGUID
            ( 
            const   GUID&   tagElement,
                    GUID    value,
                    bool    allowReplace = true
            );
        void SetScalarTimeStamp
            ( 
            const   GUID&               tagElement,
                    TIMESTAMPPQDIF      value,
                    bool                allowReplace = true
            );
        void SetScalarUINT2
            ( 
            const   GUID&   tagElement,
                    UINT2   value,
                    bool    allowReplace = true
            );
        void SetScalarINT2
            ( 
            const   GUID&   tagElement,
                    INT2    value,
                    bool    allowReplace = true
            );
        void SetScalarUINT4
            ( 
            const   GUID&   tagElement,
                    UINT4   value,
                    bool    allowReplace = true
            );
        void SetScalarINT4
            ( 
            const   GUID&   tagElement,
                    INT4    value,
                    bool    allowReplace = true
            );
        void SetScalarBOOL4
            ( 
            const   GUID&   tagElement,
                    bool    value,
                    bool    allowReplace = true
            );
        void SetScalarREAL4
            ( 
            const   GUID&   tagElement,
                    REAL4   value,
                    bool    allowReplace = true
            );
        void SetScalarREAL8
            ( 
            const   GUID&   tagElement,
                    REAL8   value,
                    bool    allowReplace = true
            );
        void SetScalarCOMPLEX8
            ( 
            const   GUID&       tagElement,
                    COMPLEX8    value,
                    bool        allowReplace = true
            );
        void SetScalarCOMPLEX16
            ( 
            const   GUID&       tagElement,
                    COMPLEX16   value,
                    bool        allowReplace = true
            );

        //  Get scalars
        bool GetScalarGUID
            ( 
            const   GUID&   tagElement,
                    GUID&   value
            ) const;
        bool GetScalarTimeStamp
            ( 
            const   GUID&               tagElement,
                    TIMESTAMPPQDIF&     value
            ) const;
        bool GetScalarUINT2
            ( 
            const   GUID&   tagElement,
                    UINT2&  value
            ) const;
        bool GetScalarINT2
            ( 
            const   GUID&   tagElement,
                    INT2&   value
            ) const;
        bool GetScalarUINT4
            ( 
            const   GUID&   tagElement,
                    UINT4&  value
            ) const;
        bool GetScalarINT4
            ( 
            const   GUID&   tagElement,
                    INT4&   value
            ) const;
        bool GetScalarBOOL4
            ( 
            const   GUID&   tagElement,
                    bool&   value
            ) const;
        bool GetScalarREAL4
            ( 
            const   GUID&   tagElement,
                    REAL4&  value
            ) const;
        bool GetScalarREAL8
            ( 
            const   GUID&   tagElement,
                    REAL8&  value
            ) const;
        bool GetScalarCOMPLEX8
            ( 
            const   GUID&       tagElement,
                    COMPLEX8&   value
            ) const;
        bool GetScalarCOMPLEX16
            ( 
            const   GUID&       tagElement,
                    COMPLEX16&  value
            ) const;

    //  Implementation
    private:

    //  Member data
    private:
        CArrayElements  m_array;
    };
