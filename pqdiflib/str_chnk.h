/*
**  Class:              CPQDIF_S_Chunk
**  Description:
**
** --------------------------------------------------------------------------
**
** Copyright 2021 PQDIF Authors
**
** Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
** either express or implied. See the License for the specific language governing permissions and limitations under the License.
**
** See the LICENSE file distributed with this work for copyright and licensing information, the AUTHORS file for a list of
** copyright holders, and the CONTRIBUTORS file for the list of contributors.
**
** SPDX - License - Identifier: Apache - 2.0
*/

class CPQDIF_S_Chunk : public CPQDIF_StreamIO
    {
    public:
	    CPQDIF_S_Chunk();
        virtual ~CPQDIF_S_Chunk();

    //  Attributes
    public:

    //  Operations
    public:
        bool SetInput( BYTE * chunk, long size );

        bool GetOutputSize( long& size );
        bool GetOutput( BYTE * chunk, long maxSize );

    //  Overridables
    public:
        virtual bool SeekPos( long pos );
        virtual bool GetPos( long& pos );
        virtual bool SeekEnd( void );

        virtual BYTE * ReadBlock( long size, long& actualSize );
        virtual bool WriteBlock( long &sizeActual );

    //  Implementation
    protected:
        BYTE *      m_chunkRead;
        long        m_sizeChunkRead;

        CPQByteArray    m_chunkWrite;

        long            m_posChunk;
    };

