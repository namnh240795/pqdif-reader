/*
**  Class:              CPQDIF_StreamIO
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

//  External interfaces
class CPQDIF_StreamProcessor;

class CPQDIF_StreamIO
    {
    public:
	    CPQDIF_StreamIO();
        virtual ~CPQDIF_StreamIO();

    // Attributes
    public:

    // Operations
    public:
        virtual bool SeekPos( long pos ) = 0;
        virtual bool GetPos( long& pos ) = 0;
        virtual bool SeekEnd( void ) = 0;

        virtual BYTE * ReadBlock( long size, long& actualSize ) = 0;

		virtual bool CanWriteFull( void ) const { return m_canWriteFull; }
        virtual bool CanWriteIncremental( void ) const { return m_canWriteInc; }

        virtual bool BeginBlock( void );
        virtual bool AppendBlock( BYTE * buffer, long size );
        virtual bool WriteBlock( long &sizeActual ) = 0;

        virtual void Flush( void ) {}   //  Optional (default behavior: nothing)

        virtual bool ConnectProcessor( CPQDIF_StreamProcessor * proc );
        virtual long GetChecksum( void );
        virtual void ResetChecksum( void );

        //  Called by the processor
        //  =======================
        //  Dereferences data buffered in the stream for the processor to read.
        //  The number of bytes available is returned in sizeActual. If max is 0,
        //  the total remaining length of the input buffer is returned.
        //  Returns FALSE when at end of buffer, TRUE otherwise.
        virtual bool ProcessRead ( const BYTE * &buffer, long max, long& sizeActual );

        //  Reserves storage in the stream for the processor to write
        //  data into. 
        virtual bool ProcessWriteReserve( BYTE * &buffer, long size );

        //  This function is set the actual size of the buffer reserved
        //  by preceeding ProceWriteReserve call.
        virtual bool ProcessWriteRelease( long sizeActual );

    // Implementation
    protected:
        virtual bool ExecuteProcessorEncode( void );
        virtual bool ExecuteProcessorDecode( void );

    //  Member data
    protected:
        //  This is connected to the object, but not owned by it.
	    CPQDIF_StreamProcessor * m_processor;

        //  These are owned by the stream object
		bool		    m_canWriteFull;
		bool		    m_canWriteInc;

        CPQByteArray    m_buffRead;
        long            m_sizeRead;
        long            m_posRead;

        CPQByteArray    m_buffWrite;
        long            m_sizeWrite;
        long            m_posWrite;
    };

