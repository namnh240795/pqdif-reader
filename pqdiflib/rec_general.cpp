/*
**  Class:          CPQDIF_R_General
**  Description:    The base class for a PQDIF record.
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

CPQDIF_R_General::CPQDIF_R_General()
{
    //  Init header structure
    memset( &m_headerRecord, 0, sizeof( m_headerRecord ) );
    m_headerRecord.guidRecordSignature = guidRecordSignaturePQDIF;
    m_headerRecord.tagRecordType = tagBlank;
    m_headerRecord.sizeHeader = sizeof( m_headerRecord );
    //  sizeData;
    //  linkNextRecord;
    //  checksum;
    //  auiReserved[ 4 ];

    m_posThisRecord = 0;
    m_pcollMain = NULL; //  Body not read

    m_changed = false;
}


CPQDIF_R_General::~CPQDIF_R_General()
{
    if( m_pcollMain )
        delete m_pcollMain;
}


bool CPQDIF_R_General::ReadHeader( CPQDIF_StreamIO * pstream )
    {
    bool    status = false;
    BYTE *  buffer = NULL;
    long    sizeActual;
    long    pos;

    //  Init header structure
    memset( &m_headerRecord, 0, sizeof( m_headerRecord ) );

    status = pstream->GetPos( pos );
    if( status )
        {
        m_posThisRecord = (LINKABS4) pos;
        buffer = pstream->ReadBlock( sizeof( m_headerRecord ), sizeActual );
        ASSERT( sizeActual == sizeof( m_headerRecord ) );
        }

    if( status && buffer)
        {
        m_headerRecord = *( (c_record_mainheader *) buffer );

        //  Validate the signature
        if( ! PQDIF_IsEqualGUID ( m_headerRecord.guidRecordSignature, guidRecordSignaturePQDIF ) )
            {
            status = FALSE;
            }
        }
    return status;
    }


bool CPQDIF_R_General::ReadBody( CPQDIF_StreamIO * pstream )
{
    bool            status = false;
    PQController    controller;

    //  Have we already read it?
    if( m_pcollMain )
    {
        status = true;
    }
    else
    {
        //  Nope -- create the top-level collection
        m_pcollMain = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        ASSERT( m_pcollMain );
        if( m_pcollMain )
        {
            // Link the collection to the record.
            m_pcollMain->SetRecord( this );

            //  Attach the record tag to the main collection
            m_pcollMain->SetTag( m_headerRecord.tagRecordType );

            //  Position us to the right place
            status = pstream->SeekPos( m_posThisRecord + m_headerRecord.sizeHeader );
            if( status )
            {
                long    sizeActual;
                BYTE *  buffer;

                buffer = pstream->ReadBlock( m_headerRecord.sizeData, sizeActual );
                if( buffer && sizeActual > 0 )
                {
                    //  Do it!
                    controller.ParseRecord( buffer, sizeActual, m_pcollMain );
                    m_changed = false;
                    status = TRUE;
                }
            }
        }
    }

    return status;
}


bool CPQDIF_R_General::WriteHeader( CPQDIF_StreamIO * pstream )
{
    bool    status = FALSE;
    SIZE4   sizeActual;

    //  Just write the dern block out!
    //  Position us to the right place
    status = pstream->SeekPos( m_posThisRecord );
    status = pstream->BeginBlock();
    if( status )
    {
        status = pstream->AppendBlock( 
                    (BYTE *) &m_headerRecord, 
                    sizeof( m_headerRecord ) );
        status = pstream->WriteBlock( sizeActual );
    }

    return status;
}


bool CPQDIF_R_General::WriteBody( CPQDIF_StreamIO * pstream )
{
    bool    status = FALSE;
    PQAlloc allocPQ;
    long    sizeTotal;

    //  Use the allocPQ to write out the main collection
    if( m_pcollMain )
    {

        //  Prepare
        pstream->ResetChecksum();

        //  Position us to the right place
        status = pstream->SeekPos( m_posThisRecord + m_headerRecord.sizeHeader );

        //  Serialize the collection. If successful then ...
        status = BufferUpCollection( m_pcollMain, allocPQ );
        if( status )
        {

            // Write the serialized data to the stream.
            sizeTotal = allocPQ.WriteListToStream( pstream );
            if( sizeTotal == 0 )
            {
                status = false;
            }
            else
            {
                //  Update header with data
                m_headerRecord.sizeData = sizeTotal;
                m_headerRecord.linkNextRecord = m_posThisRecord + m_headerRecord.sizeHeader + m_headerRecord.sizeData;
                m_headerRecord.checksum = pstream->GetChecksum();
            }
        }
    }

    return status;
}


bool CPQDIF_R_General::BufferUpCollection
            ( 
            CPQDIF_E_Collection *   pcoll,
            PQAlloc&                allocPQ,
            long                    idxRoot
            )
{
    bool status = true;
    
    long                    typePhysical;
    PQDIFValue              value;

    //  Get the number of elements in the collection.
    long countElements = pcoll->GetCount();

    //  Allocate storeage for the root collection if required.
    SIZE4 size;
    if( idxRoot < 0 )
    {
        idxRoot = allocPQ.addCollection( countElements, size );
        if( idxRoot < 0 )
        {
            status = false;
        }
    }

    //  Index to the first collection element.
    long idxCE = idxRoot + sizeof( c_collection );

    //  For each elemement in the collection do ...
    for( long idxElement = 0; 
        status && idxElement < countElements;
        idxElement++ )
    {

        //  Get the element.
        CPQDIF_Element * pel = pcoll->GetElement( idxElement );
        if( pel )
        {
            
            //  Process the element according to it's type.
            switch( pel->GetElementType() )
            {

                case ID_ELEMENT_TYPE_COLLECTION:
                {
                    CPQDIF_E_Collection * pcollChild = (CPQDIF_E_Collection *) pel;

                    long idx = allocPQ.addCollectionValue( 
                        pcollChild->GetCount(), 
                        pel->GetTag(),
                        idxCE );
                    if( idx >= 0 )
                    {
                        status = BufferUpCollection( 
                            pcollChild, 
                            allocPQ,
                            idx );
                    }
                    else
                    {
                        status = false;
                    }
                }
                break;
            
                case ID_ELEMENT_TYPE_SCALAR    :
                {
                    CPQDIF_E_Scalar *   pscalar = (CPQDIF_E_Scalar *) pel;

                    pscalar->GetValue( typePhysical, value );

                    status = allocPQ.addScalarValue( 
                        typePhysical,
                        value, 
                        pscalar->GetTag(), 
                        idxCE );
                }
                break;
            
                case ID_ELEMENT_TYPE_VECTOR    :
                {
                    CPQDIF_E_Vector *   pvector = (CPQDIF_E_Vector *) pel;
                    long    count;

                    pvector->GetCount( count );
                    typePhysical = pvector->GetPhysicalType();

                    status = allocPQ.addVectorValue( 
                        typePhysical,
                        count,
                        pvector->GetRawData(),
                        pvector->GetTag(), 
                        idxCE );
                }
                break;

            default:
                break;
            }
        }

        // Index to the next collection element to process.
        idxCE += sizeof c_collection_element;

    }

    return status;
}


bool CPQDIF_R_General::SetMainCollection( CPQDIF_E_Collection * collMain )
{
    bool    status = true;

    //  Clear out old collection?
    if( m_pcollMain )
    {
        delete m_pcollMain;
        m_pcollMain = NULL;
    }

    if( collMain )
    {
        m_pcollMain = collMain;
        collMain->SetRecord( this );
    }

    return status;
}


bool CPQDIF_R_General::GetTimeInMainCollection (const GUID &tag, TIMESTAMPPQDIF& timeTime)
    {
    //  Initialize
    bool foundItem = false;

    memset( &timeTime, 0, sizeof( timeTime ) );
    //
    //
    //  See if we can find the item
    //
    CPQDIF_E_Scalar * psc = FindScalarInCollection( m_pcollMain, tag );
    if( psc )
        {
        foundItem = psc->GetValueTimeStamp( timeTime );
        }
    return foundItem;
    }

bool CPQDIF_R_General::GetREAL8InMainCollection (const GUID &tag, REAL8 & dVal)
    {
    //  Initialize
    bool foundItem = false;

    dVal = 0.0;
    //
    //
    //  See if we can find the item
    //
    CPQDIF_E_Scalar * psc = FindScalarInCollection( m_pcollMain, tag );
    if( psc )
        {
        foundItem = psc->GetValueREAL8( dVal );
        }
    return foundItem;
    }


bool CPQDIF_R_General::GetBOOL4InMainCollection (const GUID &tag, BOOL4 & bVal)
    {
    //  Initialize
    bool foundItem = false;

    bVal = false;
    //
    //
    //  See if we can find the item
    //
    CPQDIF_E_Scalar * psc = FindScalarInCollection( m_pcollMain, tag );
    if( psc )
        {
        bool val;
        foundItem = psc->GetValueBOOL4( val );
        bVal = val;
        }
    return foundItem;
    }

bool CPQDIF_R_General::SetTimeInMainCollection (const GUID &tag, const TIMESTAMPPQDIF& timeTime)
    {
    bool        status = false;

    CPQDIF_E_Scalar * psc = FindOrCreateScalarInCollection( m_pcollMain,
            tag, ID_PHYS_TYPE_TIMESTAMPPQDIF );

    //  Set value
    PQDIFValue              value;

    if( psc )
        {
        value.ts = timeTime;
        status = psc->SetValue( ID_PHYS_TYPE_TIMESTAMPPQDIF, value );
        }

    return status;
    }

bool CPQDIF_R_General::SetREAL8InMainCollection (const GUID &tag, const REAL8 dVal)
    {
    bool        status = false;

    CPQDIF_E_Scalar * psc = FindOrCreateScalarInCollection( m_pcollMain,
            tag, ID_PHYS_TYPE_REAL8 );

    //  Set value
    PQDIFValue              value;

    if( psc )
        {
        value.real8 = dVal;
        status = psc->SetValue( ID_PHYS_TYPE_REAL8, value );
        }

    return status;
    }

bool CPQDIF_R_General::SetBOOL4InMainCollection (const GUID &tag, const BOOL4 bVal)
    {
    bool        status = false;

    CPQDIF_E_Scalar * psc = FindOrCreateScalarInCollection( m_pcollMain,
            tag, ID_PHYS_TYPE_BOOLEAN4 );

    //  Set value
    PQDIFValue              value;

    if( psc )
        {
        value.bool4 = bVal;
        status = psc->SetValue( ID_PHYS_TYPE_BOOLEAN4, value );
        }

    return status;
    }





