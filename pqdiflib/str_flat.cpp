/*
**  Class:          CPQDIF_S_FlatFile
**  Description:    A stream I/O implementing which supports the standard flat file. This is the normal persistence mechanism for PQDIF.
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

CPQDIF_S_FlatFile::CPQDIF_S_FlatFile()
    {
    m_pf = NULL;
    }


CPQDIF_S_FlatFile::~CPQDIF_S_FlatFile()
    {
    if( m_pf )
        {
        fclose( m_pf );
        }
    }


bool CPQDIF_S_FlatFile::Open
		( 
		const	char *	fname,
				bool	readOnly
		)
    {
    bool    status = FALSE;
	char *  flagsOpen = "";

	if( readOnly )
		{
        //  Read-only
		flagsOpen = "rb";
        m_canWriteFull = FALSE;
        m_canWriteInc = FALSE;
		}
    else
        {
        //  Read/write
        flagsOpen = "r+b";
        m_canWriteFull = TRUE;
        m_canWriteInc = TRUE;
        }
    
    if( !m_pf )
        {
        m_pf = fopen( fname, flagsOpen );
        if( m_pf )
            status = TRUE;
        }

    return status;
    }


bool CPQDIF_S_FlatFile::New( const char * fname )
    {
    bool    status = FALSE;
    
    //  Init
    m_canWriteFull = FALSE;
    m_canWriteInc = FALSE;

    if( !m_pf )
        {
        m_pf = fopen( fname, "w+b" );
        if( m_pf )
            {
            m_canWriteFull = TRUE;
            m_canWriteInc = TRUE;
            status = TRUE;
            }
        }

    return status;
    }


bool CPQDIF_S_FlatFile::SeekPos( long pos )
    {
    bool    status = FALSE;
    
    if( m_pf )
        {
        int rc = fseek( m_pf, pos, SEEK_SET );
        if( rc == 0 )
            status = TRUE;
        }

    return status;
    }

bool CPQDIF_S_FlatFile::SeekEnd( void )
    {
    bool    status = FALSE;
    
    if( m_pf )
        {
        int rc = fseek( m_pf, 0, SEEK_END );
        if( rc == 0 )
            status = TRUE;
        }

    return status;
    }

bool CPQDIF_S_FlatFile::GetPos( long& pos )
    {
    bool    status = FALSE;
    
    if( m_pf )
        {
        pos = ftell( m_pf );
        status = TRUE;
        }

    return status;
    }

BYTE * CPQDIF_S_FlatFile::ReadBlock( long size, long& actualSize )
    {
    BYTE *  buffRet = NULL;
    long    sizeRead;
    bool    status = FALSE;
    
    //  Init
    sizeRead = 0;

    //  Clear buffers
    m_buffRead.SetSize( 0 );
    m_posRead = 0;
    m_buffWrite.SetSize( 0 );
    m_posWrite = 0;

    if( m_pf )
        {
        //  End of file yet?
        if( ! feof( m_pf ) )
            {
            //  Nope, read a buffer full...
            //  Size the buffer first
            m_buffRead.SetSize( size );

            //  Attempt to read the block
            sizeRead = (long) fread( (BYTE*) m_buffRead.GetData(), 1, size, m_pf );
            if( sizeRead > 0 )
                {
                //  We got the block -- now decode it.
                m_buffRead.SetSize( sizeRead );
                status = ExecuteProcessorDecode();
                if( status )
                    {
                    //  Pass back the decoded buffer
                    buffRet = m_buffWrite.GetData();
                    actualSize = (long) m_buffWrite.GetSize();
                    }
                }
            }
        }

    return buffRet;
    }


bool CPQDIF_S_FlatFile::WriteBlock( long &sizeActual )
    {
    bool    status = FALSE;
    long    sizeWritten;
    
    m_buffWrite.SetSize( 0 );
    m_posWrite = 0;
    sizeActual = 0;

    if( m_pf )
        {
        //  End of file yet?
        if( ! feof( m_pf ) )
            {
            //  Encode the block
            status = ExecuteProcessorEncode();
            if( status )
                {
                //  The write buffer should contain the output
                sizeActual = m_buffWrite.GetSize();

                //  Attempt to read the block
                sizeWritten = (long) fwrite( (BYTE*) m_buffWrite.GetData(), 1, sizeActual, m_pf );
                if( sizeWritten == sizeActual )
                    {
                    status = TRUE;
                    }
                }
            }
        }

    return status;
    }


void CPQDIF_S_FlatFile::Flush( void )
    {
    if( m_pf )
        {
        fflush( m_pf );
        }
    }

