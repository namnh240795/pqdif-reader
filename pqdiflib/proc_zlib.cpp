/*
**  Class:          CPQDIF_SP_ZLIB
**  Description:    Implements a compression processor for the ZLIB  compression library
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
    
//  Public-domain ZLIB code
#include "zlib.h"



//  Construction
//  ============

CPQDIF_SP_ZLIB::CPQDIF_SP_ZLIB()
    {
    }


CPQDIF_SP_ZLIB::~CPQDIF_SP_ZLIB()
    {
    }


bool CPQDIF_SP_ZLIB::StreamEncode( void )
    {
    bool    status = TRUE;

    z_stream    c_stream; /* compression stream */
    int         err;

    //  Init ZLIB for best compression ratio
    memset( &c_stream, 0, sizeof( c_stream ) );
    c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;
    err = deflateInit( &c_stream, Z_BEST_COMPRESSION );
    if( err == Z_OK )
        {

        //  Initialize the input buffer.
        const BYTE * bufferInput;
        long sizeInput;
        
        status = m_pstrm->ProcessRead( bufferInput, 0, sizeInput );
        
        c_stream.next_in = (BYTE *)bufferInput;
        c_stream.avail_in = sizeInput;
        
        if( status )
            {

            //  Reserve space in the stream buffer for output.
            //  If successful then ...
            BYTE * bufferOutput;
            int sizeOutput = ( ( sizeInput * 11 )/10 ) + 12;
            
            status = m_pstrm->ProcessWriteReserve( bufferOutput, sizeOutput );
            
            c_stream.next_out = bufferOutput;
            c_stream.avail_out = sizeOutput;
            c_stream.total_out = 0;
            
            if( status )
                {


                //  Compress!
                err = deflate( &c_stream, Z_FINISH );
                if( err != Z_STREAM_END )
                    status = false;

                //  Release the output buffer.
                if( status && c_stream.total_out > 0 )
                    {
                    m_checksum = adler32( m_checksum, (const Bytef *)bufferOutput, c_stream.total_out );
                    status = m_pstrm->ProcessWriteRelease( c_stream.total_out );
                    }
                }
            }
        }
    
    else
        status = false;

    err = deflateEnd(&c_stream);
    if( err != Z_OK )
        status = FALSE;

    return status;
    }


bool CPQDIF_SP_ZLIB::StreamDecode( void )
    {
    bool        status = TRUE;

    z_stream    c_stream; /* decompression stream */
    int         err;

    //  Init
    memset( &c_stream, 0, sizeof( c_stream ) );
    err = inflateInit(&c_stream);
    if( err == Z_OK )
        {

        //  Initialize the input buffer.
        const BYTE * bufferInput;
        long sizeInput;
        
        status = m_pstrm->ProcessRead( bufferInput, 0, sizeInput );
        
        c_stream.next_in = (BYTE *)bufferInput;
        c_stream.avail_in = sizeInput;

        //  Determine the size of the output buffer to use for expanding
        //  the data.
        BYTE * bufferOutput;
        int sizeOutput = max( ( c_stream.avail_in * 4 ), (unsigned) 32*1024 );
        
        while( status && err == Z_OK )
            {

            //  Reserve space in the stream buffer for output.
            //  If successful then ...
            
            status = m_pstrm->ProcessWriteReserve( bufferOutput, sizeOutput );
            
            c_stream.next_out = bufferOutput;
            c_stream.avail_out = sizeOutput;
            c_stream.total_out = 0;
 
            if( status )
                {
            
                //  Compress!
                err = inflate( &c_stream, Z_NO_FLUSH );

                //  Release the output buffer.
                status = m_pstrm->ProcessWriteRelease(  c_stream.total_out );

                }
            }

        }
    else
        status = FALSE;

    err = inflateEnd( &c_stream );
    if( err != Z_OK )
        status = FALSE;

    return status;
    }


