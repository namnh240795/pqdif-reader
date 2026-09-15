/*
**  Class:          CPQDIF_StreamIO
**  Description:    The base class for implementing "stream I/O" which works along with a CPQDIF_PersistController class to supports a specific  persistence mechanism.
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


//  Local constants
const   long    sizeDefaultGrowBy = 16*1024;

//  Construction
//  ============

CPQDIF_StreamIO::CPQDIF_StreamIO()
{
    m_processor = NULL;
	
    m_canWriteFull = false;
    m_canWriteInc  = false;

    m_buffRead.SetSize( 0, sizeDefaultGrowBy );
    m_posRead = 0;
    m_buffWrite.SetSize( 0, sizeDefaultGrowBy );
    m_posWrite = 0;
}


CPQDIF_StreamIO::~CPQDIF_StreamIO()
{
}


bool CPQDIF_StreamIO::ConnectProcessor( CPQDIF_StreamProcessor * proc )
{
    m_processor = proc;

    return TRUE;
}


long CPQDIF_StreamIO::GetChecksum( void )
{ 
    return m_processor->GetChecksum(); 
}


void CPQDIF_StreamIO::ResetChecksum( void )
{ 
    m_processor->ResetChecksum(); 
}


bool CPQDIF_StreamIO::ExecuteProcessorEncode( void )
{
    bool    status = false;

    if( m_processor )
    {
        //  Make sure we are connected to it
        m_processor->ConnectStream( this );

        status = m_processor->StreamEncode();
    }

    return status;
}


bool CPQDIF_StreamIO::ExecuteProcessorDecode( void )
{
    bool    status = false;

    if( m_processor )
    {
        //  Make sure we are connected to it
        m_processor->ConnectStream( this );

        status = m_processor->StreamDecode();
    }

    return status;
}


bool CPQDIF_StreamIO::ProcessRead( const BYTE * &buffer, long max, long& sizeActual )
{
    buffer = m_buffRead.GetData() + m_posRead;        //  Adjust to current position
    sizeActual = m_buffRead.GetSize() - m_posRead;    //  Get size minus current position
    if( max > 0 && max > sizeActual )
        sizeActual = max;
    return sizeActual > 0;
}


bool CPQDIF_StreamIO::ProcessWriteReserve( BYTE * & buffer, long size )
{
    buffer = NULL;
    bool status = m_buffWrite.SetSize( m_posWrite + size );
    if( status )
    {
        buffer = m_buffWrite.GetData() + m_posWrite;
    }
    return status;
}


bool CPQDIF_StreamIO::ProcessWriteRelease( long size )
{
    bool status = m_buffWrite.SetSize( m_posWrite + size );
    if( status )
        m_posWrite += size;
    return status;
}


bool CPQDIF_StreamIO::BeginBlock( void )
{
    bool    status = true;

    //  Clear buffers
    m_buffRead.SetSize( 0, sizeDefaultGrowBy );
    m_posRead = 0;

    return status;
}


bool CPQDIF_StreamIO::AppendBlock( BYTE * buffer, long size )
{
    bool    status = true;

    //  Fill the read buffer
    m_buffRead.Append( buffer, size );

    return status;
}

