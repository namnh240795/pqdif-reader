/*
**  Class:          CPQDIF_SP_Nothing
**  Description:    Implements a "do nothing" processor. Used when the PQDIF file is not compressed -- or for certain records  which are never compressed.
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
#include "zlib.h"


//  Construction
//  ============

CPQDIF_SP_Nothing::CPQDIF_SP_Nothing()
    {
    }

CPQDIF_SP_Nothing::~CPQDIF_SP_Nothing()
    {
    }

bool CPQDIF_SP_Nothing::StreamEncode( void )
    {
    bool            status = FALSE;
    long            sizeActual = 0;
    const BYTE *    bufferInput;
    BYTE *          bufferOutput;

    status = m_pstrm->ProcessRead( bufferInput, 0, sizeActual );
    if( status )
        {
        status = m_pstrm->ProcessWriteReserve( bufferOutput, sizeActual );
        if( status )
            {
            memcpy( bufferOutput, bufferInput, sizeActual );
            m_checksum = adler32( m_checksum, (const Bytef *)bufferInput, sizeActual );
            m_pstrm->ProcessWriteRelease( sizeActual );
            }
        }

    return status;
    }

bool CPQDIF_SP_Nothing::StreamDecode( void )
    {
    bool    status = FALSE;

    //  Since we're doing nothing, decoding is the same
    //  as encoding!
    status = StreamEncode();

    return status;
    }

