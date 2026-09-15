/*
**  Class:          PQController
**  Description:    This class is used to control the reconstitution of PQDIF element objects from a buffer where they have been archived.
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

PQController::PQController( void )
    {
    }

PQController::~PQController( void )
    {
    }


void PQController::ParseRecord
        ( 
        BYTE *                  buffer, 
        SIZE4                   size, 
        CPQDIF_E_Collection *   pcollMain
        )
    {
    
	PQDIFIterator piter( this, buffer, size, 0, pcollMain );
    piter.ParseCollection();

    return;
    }


CPQDIF_E_Collection * PQController::acceptCollection
        ( 
        CPQDIF_E_Collection *   pcoll,
        int                     /*index*/,
        const GUID&             tag
        )
    {
    CPQDIF_E_Collection *   pcollNew;

    //  Create the new collection object
    pcollNew = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
    if( pcollNew )
        {

        //  Add it to the previous collection
        pcollNew->SetTag( tag );
        pcoll->Add( pcollNew );

        }

    return pcollNew;
    }

            
CPQDIF_E_Scalar * PQController::acceptScalar
        ( 
        CPQDIF_E_Collection *   pcoll,
        int                     /*index*/,
        const GUID&             tag, 
        long                    typePhysical,
        void *                  pdata
        )
    {
    CPQDIF_E_Scalar *   pel;

    ASSERT( pdata );

    //  Create new element
    pel = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
    ASSERT( pel );
    if( pel )
        {
        //  Initialize it. If successful then ...
        pel->SetTag( tag );
        if( pel->SetValue( typePhysical, *(PQDIFValue *)pdata ) )
            {

            //  Add it to the current collection
            ASSERT( pcoll );
            pcoll->Add( pel );

            }
        else
            {
            delete pel;
            pel = NULL;
            }

        }

    return pel;
    }


CPQDIF_E_Vector * PQController::acceptVector
        ( 
        CPQDIF_E_Collection *   pcoll,
        int                     /*index*/,
        const GUID&             tag, 
        long                    typePhysical,
        c_vector *              pvector,
        void *                  pdata
        )
    {
    SIZE4               sizeValue;
    CPQDIF_E_Vector *   pel = NULL;

    //  Validate parameters
    ASSERT( pvector );
    ASSERT( pdata );

    //  Init
    sizeValue = theInfo.GetNumBytesOfType( typePhysical );

	SIZE4 TotalSize = sizeValue * pvector->count;
	if ((TotalSize > 0) && (pvector->count > 0) && (pvector->count < 16*1024*1024))
	    {

		//  Create new element
		pel = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
		ASSERT( pel );
		if( pel )
			{

			//  Initialize it
			pel->SetTag( tag );
			pel->SetPhysicalType( typePhysical );
			if( pel->SetCount( pvector->count ) )
				{
				memcpy( pel->GetRawData(), pdata, pvector->count * sizeValue );

				//  Add the element to the current collection
				ASSERT( pcoll );
				pcoll->Add( pel );

				}
			else
				{
				delete pel;
				pel = NULL;
				}

			}
	    }

    return pel;
    }


bool PQController::decodeValue
        ( 
        long        typePhysical,
        void *      pdata,
        PQDIFValue& value
        )
    {
    bool    status = false;
    int     iSize = theInfo.GetNumBytesOfType( typePhysical );
    if( iSize > 0 )
        {
        memcpy( (void *)&value, pdata, iSize );
        status = true;
        }
    return status;
    }

