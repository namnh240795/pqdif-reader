/*
**  Class:          PQDIFIterator
**  Description:    Implements iteration through a set of PQDIF elements (stored in a collection) in order to reconstitute them from a buffer. Used in conjunction with the PQController class.
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

PQDIFIterator::PQDIFIterator
        (
        PQController *          pcont,
        BYTE *                  buffer, 
        SIZE4                   size,
        long                    pos,
        CPQDIF_E_Collection *   pcoll
        )
    {
    m_pcont = pcont;

    m_buffer = buffer;
    m_size = size;
    m_pos = pos;

    m_pcoll = pcoll;
    }


PQDIFIterator::~PQDIFIterator()
    {
    }


bool PQDIFIterator::ParseCollection( void )
    {
    INT4                    idx;
    bool                    accepted = false;
    c_collection            collection;
    c_collection_element *  pelement;


    //  Get collection header
    collection = *( (c_collection*) ( m_buffer + m_pos ) );
    m_pos += sizeof( collection );
    ASSERT( m_pos < m_size || ( m_pos == m_size && collection.count == 0 )  );
    ASSERT( m_pos >= 0 );

    //  For each element in the collection do ...
    pelement = (c_collection_element *) ( m_buffer + m_pos );
    for( idx = 0; idx < collection.count; idx++, pelement++ )
        {
        //  See what type it is, and see if it can be accepted
        switch( pelement->typeElement )
            {
            case ID_ELEMENT_TYPE_COLLECTION:
                {
                CPQDIF_E_Collection * pcoll = m_pcont->acceptCollection(
                    m_pcoll,
                    idx,
                    pelement->tagElement );
                accepted = pcoll != NULL;
                if( accepted )
                    {
                    //  Seek to the link (to the container header)
                    m_pos = pelement->link.linkElement;
                    ASSERT( m_pos < m_size );
                    ASSERT( m_pos > 0 );

                    if( m_pos >= m_size || m_pos <= 0 )
                        {
                        goto PQITPC_Abort;
                        }

                    //  Should be at the right file position for this collection
                    PQDIFIterator iter( 
                        m_pcont,
                        m_buffer,
                        m_size,
                        m_pos, 
                        pcoll );

                    //  Recursively parse the new collection
                    accepted = iter.ParseCollection();

                    }
                }
                break;

            case ID_ELEMENT_TYPE_SCALAR:
                { 
                
                //  Alloc & read data
                BYTE * pdata;
                if( pelement->isEmbedded )
                    {
                    //  Pull the data from the embedded value
                    pdata = (BYTE *) pelement->valueEmbedded;
                    }
                else
                    {
                    //  Seek to the link
                    m_pos = pelement->link.linkElement;
                    ASSERT( m_pos < m_size );
                    ASSERT( m_pos > 0 );
                    if( m_pos >= m_size || m_pos < 0 )
                        {
                        goto PQITPC_Abort;
                        }

                    pdata = (BYTE *) ( m_buffer + m_pos );
                    }
                
                //  Accept data?
                accepted = m_pcont->acceptScalar(
                    m_pcoll,
                    idx,
                    pelement->tagElement,
                    pelement->typePhysical,
                    (void *) pdata) != NULL;

                }
                break;

            case ID_ELEMENT_TYPE_VECTOR:
                {

                //  Seek to the link (to the vector header)
                m_pos = pelement->link.linkElement;
                ASSERT( m_pos < m_size );
                ASSERT( m_pos > 0 );
                if( m_pos >= m_size || m_pos < 0 )
                    {
                    goto PQITPC_Abort;
                    }

                //  Grab vector header
                c_vector vector = *( (c_vector *) ( m_buffer + m_pos ) );
                m_pos += sizeof( vector );

                //  It is possible that this points off the end,
                //  if vector.count == 0.
                ASSERT( m_pos < m_size || vector.count == 0 );
                ASSERT( m_pos > 0 );

                //  Grab pointer to data
                BYTE * pdata = (BYTE *) ( m_buffer + m_pos );
                
                //  Accept data? (The acceptVector() function
                //  must be careful not to go off the end if
                //  vector.count == 0.
                accepted = m_pcont->acceptVector( 
                    m_pcoll,
                    idx,
                    pelement->tagElement,
                    pelement->typePhysical,
                    &vector, 
                    (void *) pdata) != NULL;
                
                }
                break;
            }   //  switch()
        }   //  for ()

PQITPC_Abort:

    return accepted;
    }
