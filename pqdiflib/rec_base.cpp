/*
**  Class:          CPQDIF_R_DataSource
**  Description:    Implements a PQDIF record "wrapper" for the data source record. You can cast a standard record object to this class.
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


CPQDIF_E_Collection * CPQDIFRecord::FindCollectionInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag
            )
    {
    CPQDIF_E_Collection *   pcolReturn = NULL;

    if( pcoll )
        {
        pcolReturn = (CPQDIF_E_Collection *)pcoll->GetElement( tag, ID_ELEMENT_TYPE_COLLECTION );
        }

    return pcolReturn;
    }


CPQDIF_E_Scalar * CPQDIFRecord::FindScalarInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag
            )
    {
    CPQDIF_E_Scalar *   pscReturn = NULL;

    if( pcoll )
        {
        pscReturn = (CPQDIF_E_Scalar *)pcoll->GetElement( tag, ID_ELEMENT_TYPE_SCALAR );
        }

    return pscReturn;
    }


bool CPQDIFRecord::GetScalarValueInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag,
                    UINT4                   typePhysical,
                    PQDIFValue&             value
            )
    {
    bool                status = FALSE;
    long                typePhysicalThisItem;
    CPQDIF_E_Scalar *   pscReturn = NULL;

    if( pcoll )
        {
        pscReturn = (CPQDIF_E_Scalar *)pcoll->GetElement( tag, ID_ELEMENT_TYPE_SCALAR );
        if( pscReturn )
            {
                status = pscReturn->GetValue( typePhysicalThisItem, value )
                    && typePhysicalThisItem == (long)typePhysical;
            }
        }

    return status;
    }


CPQDIF_E_Scalar * CPQDIFRecord::FindOrCreateScalarInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag,
                    UINT4                   typePhysical
            )
    {
    CPQDIF_E_Scalar *       pscReturn = NULL;

    if( pcoll )
        {
        pscReturn = (CPQDIF_E_Scalar *)pcoll->GetElement( tag, ID_ELEMENT_TYPE_SCALAR );
        if( !pscReturn )
            {
            //  If it doesn't exist, create, initialize and add it
            pscReturn = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
            if( pscReturn )
                {
                pscReturn->SetTag( tag );
                pscReturn->SetPhysicalType( typePhysical );
                pcoll->Add( pscReturn );
                }
            }
        }
    return pscReturn;
    }


CPQDIF_E_Vector * CPQDIFRecord::FindVectorInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag
            )
    {
    CPQDIF_E_Vector *   pvectReturn = NULL;

    if( pcoll )
        {
        pvectReturn = (CPQDIF_E_Vector *)pcoll->GetElement( tag, ID_ELEMENT_TYPE_VECTOR );
        }

    return pvectReturn;
    }


CPQDIF_E_Vector * CPQDIFRecord::FindOrCreateVectorInCollection
            ( 
                    CPQDIF_E_Collection *   pcoll,
            const   GUID&                   tag,
                    UINT4                   typePhysical
            )
    {
    CPQDIF_E_Vector *       pvectReturn = NULL;

    if( pcoll )
        {
        pvectReturn = (CPQDIF_E_Vector *)pcoll->GetElement( tag, ID_ELEMENT_TYPE_VECTOR );
        if( !pvectReturn )
            {
            //  If it doesn't exist, create, initialize and add it
            pvectReturn = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
            if( pvectReturn )
                {
                pvectReturn->SetTag( tag );
                pvectReturn->SetPhysicalType( typePhysical );
                pcoll->Add( pvectReturn );
                }
            }
        }

    return pvectReturn;
    }
