/*
**  Class:          
**  Description:        Base Class for PQDIF Records
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

//  Forward-declare these items
class CPQDIF_StreamIO;

class CPQDIF_E_Collection;
class CPQDIF_E_Scalar;
class CPQDIF_E_Vector;

union PQDIFValue;

class CPQDIFRecord
    {
    public:
        CPQDIFRecord() {}
        virtual ~CPQDIFRecord() {}

    // Operations
    public:
        virtual bool ReadHeader( CPQDIF_StreamIO * pstream ) = 0;
        virtual bool ReadBody( CPQDIF_StreamIO * pstream ) = 0;

        virtual bool WriteHeader( CPQDIF_StreamIO * pstream ) = 0;
        virtual bool WriteBody( CPQDIF_StreamIO * pstream ) = 0;

        // Attributes
    public:
        virtual bool HeaderGetPos( LINKABS4& pos ) const = 0;
        virtual bool HeaderSetPos( LINKABS4 pos ) = 0;
        virtual bool HeaderGetTag( GUID& tagRecord ) const = 0;
        virtual bool HeaderSetTag( GUID tagRecord ) = 0;
        virtual bool HeaderGetSize( SIZE4& sizeHeader, SIZE4& sizeBody ) const = 0;
        virtual bool HeaderSetSize( SIZE4 sizeHeader, SIZE4 sizeBody ) = 0;
        virtual bool HeaderGetChecksum( UINT& checksum ) const = 0;
        virtual bool HeaderSetChecksum( UINT checksum ) = 0;
        virtual bool HeaderGetPosNextRecord( LINKABS4& pos ) const = 0;
        virtual bool HeaderSetPosNextRecord( LINKABS4 pos ) = 0;
        virtual CPQDIF_E_Collection * GetMainCollection( void ) const = 0;
        virtual bool SetMainCollection( CPQDIF_E_Collection * collMain ) = 0;
        virtual bool GetChanged( void ) = 0;
        virtual void SetChanged( bool changed ) = 0;

	//	Static functions (do not affect--or even require--an object)
	public:
        static CPQDIF_E_Collection * FindCollectionInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag
            );
        static CPQDIF_E_Scalar * FindScalarInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag
            );
        static bool GetScalarValueInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag,
                    UINT4                   typePhysical,
                    PQDIFValue&             value
            );
        static CPQDIF_E_Vector * FindVectorInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag
            );

        static CPQDIF_E_Scalar * FindOrCreateScalarInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag,
                    UINT4                   typePhysical
            );
        static CPQDIF_E_Vector * FindOrCreateVectorInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag,
                    UINT4                   typePhysical
            );

    // Implementation
    protected:
		//	None; pure virtual base class.

    //  Member data
    protected:
		//	None; pure virtual base class.
    };

