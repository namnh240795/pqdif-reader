/*
**  Class:          CPQDIF_PC_FlatFile
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

class CPQDIF_StreamIO;
class CPQDIF_StreamProcessor;
enum PF_StreamIO;


class CPQDIF_PC_FlatFile : public CPQDIF_PersistController
    {
    public:
        CPQDIF_PC_FlatFile();
        ~CPQDIF_PC_FlatFile();

    //  Overridables
    public:
        virtual CPQDIFRecord *  GetRecordFull( long index );

    //  Public interface
    public:
        void SetFileName( const char * fname )
            { m_fname = fname; }
        const char * GetFileName( void )
            { return m_fname.c_str(); }

        void SetChunkInput( BYTE * chunk, long size );
        bool GetChunkOutputSize( long& size );
        bool GetChunkOutput( BYTE * chunk, long size );

        bool GetCanWriteIncremental( void )
            {
            UpdateInformation();    //  need to fix
            return false;
            //return m_pstream->CanWriteIncremental();
            }

        bool ReadHeaders( void );

        bool ImportContainerAndRecordsFromChunk( BYTE * chunk, long size, bool &bCompressed );
        bool ImportRecordsFromChunk( BYTE * chunk, long size, bool bCompressed );

        bool WriteIncremental( void );
        bool WriteNew( void );
		bool WriteToStream(CPQDIF_StreamIO *pstrm);
		bool WriteRecordsToFile( void );

	    long GetCompressionAlgorithm();
	    void SetCompressionAlgorithm( long algNew );
	    long GetCompressionStyle();
	    void SetCompressionStyle( long styleNew );

    //  Implementation
    protected:
        void UpdateInformation( void );

    //  Member data
    private:
        //  Chunk (if NULL, assume physical file)
        BYTE *      m_chunk;
        long        m_sizeChunk;
        
        //  Physical flat file path
        string      m_fname;

        //  Keep track of which stream type
        enum PF_StreamIO            m_whichStream;  

        CPQDIF_StreamIO *           m_pstream;
        CPQDIF_StreamProcessor *    m_pprocHeader;
        CPQDIF_StreamProcessor *    m_pprocBody;
    };
