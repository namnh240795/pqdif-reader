/*
**  Class:          CPQDIF_S_Chunk
**  Description:    A stream I/O implementing which supports "chunks." This will allow a PQDIF file to be embedded in a database binary field, for example.
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

CPQDIF_S_Chunk::CPQDIF_S_Chunk()
    {
    m_chunkRead = NULL;
    m_sizeChunkRead = 0;
    m_chunkWrite.SetSize( 0, 64 );
    m_posChunk = 0;
    }

CPQDIF_S_Chunk::~CPQDIF_S_Chunk()
    {
    //  Nothing to delete
    }


bool CPQDIF_S_Chunk::SetInput( BYTE * chunk, long size )
    {
    bool    status = FALSE;
    
    if( chunk && size > 0 )
        {
        m_chunkRead = chunk;
        m_sizeChunkRead = size;
        status = TRUE;
        }
    else
        {
        m_chunkRead = NULL;
        m_sizeChunkRead = 0;
        }

    return status;
    }


bool CPQDIF_S_Chunk::GetOutputSize( long& size )
    {
    bool    status = FALSE;

    size = m_chunkWrite.GetSize();
    status = TRUE;

    return status;
    }


bool CPQDIF_S_Chunk::GetOutput( BYTE * chunk, long maxSize )
    {
    bool    status = FALSE;
    long    sizeActual;
    
    if( chunk && maxSize > 0 )
        {
        sizeActual = min( maxSize, (long) m_chunkWrite.GetSize() );
        memcpy( chunk, (BYTE *) m_chunkWrite.GetData(), sizeActual );
        status = TRUE;
        }

    return status;
    }


bool CPQDIF_S_Chunk::SeekPos( long pos )
    {
    bool    status = FALSE;
    
    m_posChunk = pos;
    status = TRUE;

    return status;
    }

bool CPQDIF_S_Chunk::SeekEnd( void )
    {
    bool    status = FALSE;
    
    //  Are we in read mode?
    if( m_chunkRead )
        {
        m_posChunk = m_sizeChunkRead;
        status = TRUE;
        }
    else
        {
        //  Nope, write mode.
        m_posChunk = m_chunkWrite.GetSize();
        status = TRUE;
        }

    return status;
    }

bool CPQDIF_S_Chunk::GetPos( long& pos )
    {
    bool    status = false;
    
    pos = m_posChunk;
    status = true;

    return status;
    }

BYTE * CPQDIF_S_Chunk::ReadBlock( long size, long& actualSize )
    {
    BYTE *  buffRet = NULL;
    long    sizeAvailable;
    long    sizeRead;
    bool    status = FALSE;
    
    //  Init
    sizeRead = 0;

    //  Clear buffers
    m_buffRead.SetSize( 0 );
    m_posRead = 0;
    m_buffWrite.SetSize( 0 );
    m_posWrite = 0;

    //  Do we have a valid chunk to read?
    if( m_chunkRead && m_sizeChunkRead > 0 && m_posChunk >= 0 )
        {
        //  Attempt to read the block
        sizeAvailable = m_sizeChunkRead - m_posChunk;
        if( sizeAvailable > size )
            {
            sizeRead = size;
            }
        else
            {
            sizeRead = sizeAvailable;
            }

        //  Did we get anything?
        //  (If not, we are at the end of the chunk and will return NULL.)
        if( sizeRead > 0 )
            {
            //  Size the buffer first & then copy the block
            m_buffRead.SetSize( sizeRead );
            memcpy( m_buffRead.GetData(), m_chunkRead+m_posChunk, sizeRead );

            //  Increment past the part we just read.
            m_posChunk += sizeRead;

            //  We got the block -- now decode it.
            status = ExecuteProcessorDecode();
            if( status )
                {
                //  Pass back the decoded buffer
                buffRet = m_buffWrite.GetData();
                actualSize = (long) m_buffWrite.GetSize();
                }
            }
        }
    return buffRet;
    }


bool CPQDIF_S_Chunk::WriteBlock( long &sizeActual )
    {
    bool    status = FALSE;
    BYTE *  pdataChunk;
    
    //  Init
    m_buffWrite.SetSize( 0 );
    m_posWrite = 0;
    sizeActual = 0;

    //  Encode the block
    status = ExecuteProcessorEncode();
    if( status )
        {
        //  The write buffer should contain the output
        sizeActual = m_buffWrite.GetSize();
        if( sizeActual > 0 )
            {
            //  Resize the write chunk as necessary.
            long sizeNew = m_posChunk + sizeActual;
            if( sizeNew > m_chunkWrite.GetSize() )
            {
                m_chunkWrite.SetSize( sizeNew );
            }

            //  Find where this block goes
            pdataChunk = (BYTE *) m_chunkWrite.GetData();
            pdataChunk += m_posChunk;

            //  Copy over the block
            memcpy( pdataChunk, (BYTE *) m_buffWrite.GetData(), sizeActual );
            m_posChunk += sizeActual;
            status = TRUE;
            }
        }

    return status;
    }
