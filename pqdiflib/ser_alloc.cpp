/*
**  Class:          PQAlloc
**  Description:    A simple allocator class that understands all of the fundamental PQDIF physical types.
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


void PQAlloc::Reinitialize( long offset )
    {
    data.clear();
    data.reserve( 32 * 1024 );
    idxOffset = offset;
    }

PQAlloc::PQAlloc()
    {
    data.reserve( 32 * 1024 );
    idxOffset = 0;
    }

PQAlloc::~PQAlloc()
    {
    }

long PQAlloc::allocate(size_t size)
    {

    //  Get the current size of the buffer.
    long sizeTotal = static_cast<long>(data.size());

    //  Get the offset to the current block.
    long idx = sizeTotal + idxOffset;

    //  Resize the buffer.
    try
    {
        sizeTotal += static_cast<long>(size);
        data.resize( sizeTotal, 0 );
    }
    catch( ... )
    {
        idx = -1;
    }

    return idx;
    }


long PQAlloc::addCollection
            ( 
            UINT4   count,
            SIZE4   &size
            )
    {

    //  Compute the size of the collection.
    size = sizeof( c_collection ) + ( count * sizeof( c_collection_element ) );
    size = theInfo.padSizeTo4Bytes( size );

    long idx = allocate( size );
    if( idx >= 0 )
        {
        
        //  Dereference the collection header.
        struct c_collection * pcoll = (c_collection *)at( idx );

        //  Init the collection header
        pcoll->count = count;
        
        }

    return idx;
    }


long PQAlloc::addCollectionValue
            ( 
            UINT4                   count, 
            const GUID&             tag, 
            long                    idxCE
            )
    {

    SIZE4 size;
    long idx = addCollection( count, size );
    if( idx >= 0 )
        {

        //  Dereference the collection element that refences
        //  the newly added collection.
        struct c_collection_element * pce = ( c_collection_element * )at(idxCE);

        //  Set up the collection element.
        pce->tagElement = tag;
        pce->typeElement = ID_ELEMENT_TYPE_COLLECTION;
        pce->typePhysical = 0;
        pce->isEmbedded = FALSE;
        pce->reserved = 0;
        pce->link.linkElement = idx;
        pce->link.sizeElement = size;

        }

    return idx;
    }


bool PQAlloc::addScalarValue
            (
            long                    typePhysical,
            PQDIFValue              value,
            const GUID&             tag, 
            long                    idxCE 
            )
    {
    bool    status = FALSE;
    BYTE *  pdata = NULL;

    //  Init the element header
    c_collection_element * pce = (c_collection_element *)at( idxCE );
    pce->tagElement = tag;
    pce->typeElement = ID_ELEMENT_TYPE_SCALAR;
    pce->typePhysical = (INT1) typePhysical;
    pce->isEmbedded = FALSE;
    pce->reserved = 0;

    //  Compute the size of the data. If it's small enough to embed
    //  then ...
    SIZE4 size = theInfo.GetNumBytesOfType( typePhysical );
    if( size <= sizeof( pce->valueEmbedded ) )
        {
        //  This is less than 8 bytes -- no allocation necessary
        //  Thereforem, it IS embedded
        pce->isEmbedded = TRUE;

        //  Set up the collection element properly
        pdata = (BYTE *)( pce->valueEmbedded );
        
        }
    
    //  Else if the data can not be embedded then ...
    else
        {

        //  Allocate memory (be sure to pad it to 4 bytes).
        size = theInfo.padSizeTo4Bytes( size );
        long idx = allocate( size );
        if( idx >= 0 )
            {

            // Since we called allocate the pointer to the
            // collection element must be refreshed.
            pce = (c_collection_element *)at( idxCE );

            //  Set up the collection element properly
            pce->link.linkElement = idx;
            pce->link.sizeElement = size;

            //  Dereference the data.
            pdata = (BYTE *)at( idx );

            }
        }

    //  If we have the pointer, convert the value itself
    if( pdata )
        {
        convertValue( typePhysical, value, pdata );
        status = TRUE;
        }

    return status;
    }


bool PQAlloc::addVectorValue
            (
            long                    typePhysical,
            long                    count, 
            BYTE *                  values,
            const GUID &            tag, 
            long                    idxCE 
            )
    {
    bool status = FALSE;

    //  Determine the size of the vector
    SIZE4 sizeValue = theInfo.GetNumBytesOfType( typePhysical );
    SIZE4 size = sizeof( c_vector ) + ( count * sizeValue );
    size = theInfo.padSizeTo4Bytes( size );

    //  Allocate the vector
    long idx = allocate( size );
    if( idx >= 0 )
        {

        //  Init the collection element header
        c_collection_element * pce = (c_collection_element *)at( idxCE );
        pce->tagElement = tag;
        pce->typeElement = ID_ELEMENT_TYPE_VECTOR;
        pce->typePhysical = (INT1) typePhysical;
        pce->isEmbedded = FALSE;
        pce->reserved = 0;
        pce->link.linkElement = idx;
        pce->link.sizeElement  = size;

        //  Dereference the vector.
        struct c_vector * pvector = (c_vector *)at( idx );

        //  Init the vector header
        pvector->count = count;

        //  Copy the vector data.
        BYTE * pdata = (BYTE *) ( ( (BYTE *) pvector ) + sizeof( c_vector ) );
        memcpy( pdata, values, count * sizeValue );

        status = TRUE;

        }

    return status;
    }


bool PQAlloc::convertValue
            (
            long        typePhysical,
            PQDIFValue  value,
            BYTE *      pdata
            )
    {
    bool    status = TRUE;

    switch( typePhysical )
        {
        case ID_PHYS_TYPE_BOOLEAN1:
            *( (BOOL1 *) pdata ) = value.bool1;
            break;

        case ID_PHYS_TYPE_CHAR1:
            *( (CHAR1 *) pdata ) = value.char1;
            break;

        case ID_PHYS_TYPE_INTEGER1:
            *( (INT1 *) pdata ) = value.int1;
            break;

        case ID_PHYS_TYPE_UNS_INTEGER1:
            *( (UINT1 *) pdata ) = value.uint1;
            break;

        case ID_PHYS_TYPE_BOOLEAN2:
            *( (BOOL2 *) pdata ) = value.bool2;
            break;

        case ID_PHYS_TYPE_CHAR2:
            *( (CHAR2 *) pdata ) = value.char2;
            break;

        case ID_PHYS_TYPE_INTEGER2:
            *( (INT2 *) pdata ) = value.int2;
            break;

        case ID_PHYS_TYPE_UNS_INTEGER2:
            *( (UINT2 *) pdata ) = value.uint2;
            break;

        case ID_PHYS_TYPE_BOOLEAN4:
            *( (BOOL4 *) pdata ) = value.bool4;
            break;

        case ID_PHYS_TYPE_INTEGER4:
            *( (INT4 *) pdata ) = value.int4;
            break;

        case ID_PHYS_TYPE_UNS_INTEGER4:
            *( (UINT4 *) pdata ) = value.uint4;
            break;

        case ID_PHYS_TYPE_REAL4:
            *( (REAL4 *) pdata ) = value.real4;
            break;

        case ID_PHYS_TYPE_REAL8:
            *( (REAL8 *) pdata ) = value.real8;
            break;

        case ID_PHYS_TYPE_COMPLEX8:
            *( (COMPLEX8 *) pdata ) = value.complex8;
            break;

        case ID_PHYS_TYPE_COMPLEX16:
            *( (COMPLEX16 *) pdata ) = value.complex16;
            break;

        case ID_PHYS_TYPE_TIMESTAMPPQDIF:
            *( (ts *) pdata ) = value.ts;
            break;

        case ID_PHYS_TYPE_GUID:
            *( (GUID *) pdata ) = value.guid ;
            break;
        
        default:
            status = FALSE;
            break;
        }

    return status;
    }



long PQAlloc::WriteListToFile(FILE *pf)
    {
    fwrite( &data[0], 1, data.size(), pf );
    return idxOffset + static_cast<long>(data.size());
    }


long PQAlloc::WriteListToStream( CPQDIF_StreamIO *pstrm )
    {

    long sizeActualTotal = 0;

    pstrm->BeginBlock();
    pstrm->AppendBlock( &data[0], static_cast<long>(data.size()) );
    pstrm->WriteBlock( sizeActualTotal );

    return sizeActualTotal;
    }


