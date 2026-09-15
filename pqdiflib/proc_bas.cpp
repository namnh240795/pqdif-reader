/*
**  Class:          CPQDIF_StreamProcessor
**  Description:    Base class for PQDIF stream processors generally for compression/decompression.
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

CPQDIF_StreamProcessor::CPQDIF_StreamProcessor()
    {
    m_pstrm = NULL;
    m_checksum = adler32( 0L, Z_NULL, 0 );
    }

CPQDIF_StreamProcessor::~CPQDIF_StreamProcessor()
    {
    }

bool CPQDIF_StreamProcessor::ConnectStream( CPQDIF_StreamIO * pstrm )
    {
    m_pstrm = pstrm;
    return true;
    }


void CPQDIF_StreamProcessor::ResetChecksum( void )
{
    m_checksum = adler32( 0L, Z_NULL, 0 );
}


