/*
**  Class:          CPQDIF_PC_FlatFile class. 
**  Description:    Implements a persistence controller for a flat file on disk.
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

CPQDIF_PC_FlatFile::CPQDIF_PC_FlatFile()
    {
    m_chunk = NULL;
    m_sizeChunk = 0;

    m_fname = _T( "" );

    m_pstream = NULL;
    m_pprocHeader = NULL;
    m_pprocBody = NULL;
    }


CPQDIF_PC_FlatFile::~CPQDIF_PC_FlatFile()
    {
    if( m_pstream )
        delete m_pstream;
    if( m_pprocHeader )
        delete m_pprocHeader;
    if( m_pprocBody )
        delete m_pprocBody;
    }

bool CPQDIF_PC_FlatFile::ImportContainerAndRecordsFromChunk( BYTE * chunk, long size, bool &bCompressed )
    {
    bool        status = false;
    //
    //
    //  Initialize compression flag
    //
    bCompressed = false;

    //  Get rid of any old stream and processors hanging around
    if( m_pstream )
        {
        delete m_pstream;
        m_pstream = NULL;
        }
    if( m_pprocHeader )
        {
        delete m_pprocHeader;
        m_pprocHeader = NULL;
        }
    if( m_pprocBody )
        {
        delete m_pprocBody;
        m_pprocBody = NULL;
        }

    if( chunk == NULL || size <= 0)
        return false;

    //  Create streams and default processors
    m_pstream     = theFactory.NewStreamIO( PSIO_Chunk );
    m_pprocHeader = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );
    m_pprocBody   = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );

    //  Open the stream
    ((CPQDIF_S_Chunk *) m_pstream)->SetInput( chunk, size );

    //  Read first header w/NOTHING processors
    m_pstream->ConnectProcessor( m_pprocHeader );

    LINKABS4    posNext = 0;

    CPQDIFRecord * precord = theFactory.NewRecord( PFR_Record );
    if( precord )
        {
        status = precord->ReadHeader( m_pstream );
        if( status )
            {
            //  Go ahead and read the first record body
            status = precord->ReadBody( m_pstream );
            if( status )
                {
                bool    foundCompInfo = false;
                UINT4   styleComp;
                UINT4   algComp;

				CPQDIF_R_Container *	pcont;

				//	Assume it's a container and try to get compression info
				pcont = (CPQDIF_R_Container *) precord;
				foundCompInfo = pcont->GetCompressionInfo( styleComp, algComp );

                //  Do we need to change processors?
                if( foundCompInfo )
                    {
                    switch( styleComp )
                        {
                        case ID_COMP_STYLE_TOTALFILE:
                            //  Unsupported
                            status = FALSE;
                            break;

                        case ID_COMP_STYLE_RECORDLEVEL:
                            //  Yup! Check what algorithm we should use...
                            delete m_pprocBody;
                            m_pprocBody = theFactory.NewStreamProcessor( algComp );
                            if (m_pprocBody)
                                {
                                bCompressed = true;
                                }
                            else
                                {
                                //  Can we support it?
                                status = false;
                                }
                            break;

                        case ID_COMP_STYLE_NONE:
                        default:
                            //  Do nothing
                            break;
                        }
                    }

                if( status )
                    {
                    //  Done -- add the first record to the array
                    m_arrayRecords.Add( precord );
                    }
                
                }   //  Read body OK
            }   //  Read header OK
        if (!status)
            delete precord;
        }	//	Record object created OK

    //  Read through record headers and build list
    while( status )
        {
        //
        //
        //  This function always returns true so we
        //  must check the position.  If the link recod
        //  is zero, then we are done.  If we get a chunk
        //  that doesn't set this to zero, then we will
        //  stop when the position is checked against the
        //  size in the statement that follows.
        //
        precord->HeaderGetPosNextRecord( posNext );
        //
        //
        //  Are we at the end?
        //
        if( posNext > 0 && posNext < size)
            {
            m_pstream->SeekPos( posNext );

            precord = theFactory.NewRecord( PFR_Record );
            if( precord )
                {
                status = precord->ReadHeader( m_pstream );
                if( status )
                    {
                    m_arrayRecords.Add( precord );
                    }
                }
            }
        else
            {
            //
            //
            // yep, we are at the end
            //  use a break so the status flag is true indicating
            //  no failure so far.
            //
            break;
            }

        }   //  while()

    if( status )
        {
        for( long idxRec = 0; idxRec < GetRecordCount(); idxRec++ )
            {
            GetRecordFull( idxRec );
            }
        }

    m_state = pqpc_Modified;

    return status;
    }

//
//
//  This function imports record from a chunk and appends them
//  to the internal array of records.
//
bool CPQDIF_PC_FlatFile::ImportRecordsFromChunk( BYTE * chunk, long size, bool bCompressed )
    {
    bool        status = false;

    //  Get rid of any old stream and processors hanging around
    if( m_pstream )
        {
        delete m_pstream;
        m_pstream = NULL;
        }
    if( m_pprocHeader )
        {
        delete m_pprocHeader;
        m_pprocHeader = NULL;
        }
    if( m_pprocBody )
        {
        delete m_pprocBody;
        m_pprocBody = NULL;
        }

    if( chunk == NULL || size <= 0)
        return status;
    //
    //
    //  Create streams and default processors
    //
    m_pstream     = theFactory.NewStreamIO( PSIO_Chunk );
    m_pprocHeader = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );

    if (bCompressed)
        m_pprocBody   = theFactory.NewStreamProcessor( ID_COMP_ALG_ZLIB );
    else
        m_pprocBody   = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );

    //  Open the stream
    ((CPQDIF_S_Chunk *) m_pstream)->SetInput( chunk, size );

    LINKABS4        posNext = 0;
    CPQDIFRecord *  precord;

    status = true;
    //  Read through record headers and build list
    while( status && posNext < size)
        {
        //
        //
        //  Cue up to next record in stream
        //
        m_pstream->SeekPos( posNext );
        //
        //
        //  Make a new record object
        //
        precord = theFactory.NewRecord( PFR_Record );
        if( precord )
            {
            //
            //
            //  Read the record header with the appropriate
            //  stream processor (no compression)
            //
            m_pstream->ConnectProcessor( m_pprocHeader );
            status = precord->ReadHeader( m_pstream );
            if( status )
                {
                //
                //
                //  Read the body with the appropriate stream
                //  processor (may or may not do compression)
                //
                m_pstream->ConnectProcessor( m_pprocBody );
                status = precord->ReadBody( m_pstream );
                if (status)
                    {
                    //
                    //
                    //  Add the record to our array
                    //
                    m_arrayRecords.Add( precord );
                    //
                    //
                    //  This function always returns true so we
                    //  must check the position.  If the link recod
                    //  is zero, then we are done.  If we get a chunk
                    //  that doesn't set this to zero, then we will
                    //  die when the position is checked against the
                    //  size in the while statement
                    //
                    precord->HeaderGetPosNextRecord( posNext );
                    if (posNext == 0)
                        break;
                    }
                }
            else
                {
                //
                //
                //  if we got here, we probably tried to read past the
                //  end of the stream so we clean up.
                //
                delete precord;
                }
            }
        }   //  while()

//    m_state = pqpc_Empty;
    m_state = pqpc_Modified;

    return status;
    }

bool CPQDIF_PC_FlatFile::ReadHeaders( void )
    {
    bool        status = FALSE;
    LINKABS4    posNext = 0;

    //  Get rid of any old stream hanging around
    if( m_pstream )
        {
        delete m_pstream;
        m_pstream = NULL;
        }

    //  Are we dealing with a chunk or a file?
    if( m_chunk )
        m_whichStream = PSIO_Chunk;
    else
        m_whichStream = PSIO_FlatFile;

    //  Create streams and default processors
    m_pstream = theFactory.NewStreamIO( m_whichStream );
    m_pprocHeader = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );
    m_pprocBody   = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );

    //  Open the stream
    if( m_whichStream == PSIO_Chunk )
        {
        ((CPQDIF_S_Chunk *) m_pstream)->SetInput( m_chunk, m_sizeChunk );
        }
    else
        {
        ((CPQDIF_S_FlatFile *) m_pstream)->Open( m_fname.c_str(), true );
        }

    //  Read first header w/NOTHING processors
    m_pstream->ConnectProcessor( m_pprocHeader );

    CPQDIFRecord * precord = theFactory.NewRecord( PFR_Record );
    if( precord )
        {
        status = precord->ReadHeader( m_pstream );
        if( status )
            {
            //  Go ahead and read the first record body
            status = precord->ReadBody( m_pstream );
            if( status )
                {
                bool    foundCompInfo = FALSE;
                UINT4   styleComp;
                UINT4   algComp;

				CPQDIF_R_Container *	pcont;

				//	Assume it's a container and try to get compression info
				pcont = (CPQDIF_R_Container *) precord;
				foundCompInfo = pcont->GetCompressionInfo( styleComp, algComp );

                //  Do we need to change processors?
                if( foundCompInfo )
                    {
                    switch( styleComp )
                        {
                        case ID_COMP_STYLE_TOTALFILE:
                            //  Unsupported
                            status = FALSE;
                            break;

                        case ID_COMP_STYLE_RECORDLEVEL:
                            //  Yup! Check what algorithm we should use...
                            delete m_pprocBody;
                            m_pprocBody = theFactory.NewStreamProcessor( algComp );

                            //  Can we support it?
                            if( !m_pprocBody )
                                status = FALSE;
                            break;

                        case ID_COMP_STYLE_NONE:
                        default:
                            //  Do nothing
                            break;
                        }
                    }

                if( status )
                    {
                    //  Done -- add the first record to the array
                    m_arrayRecords.Add( precord );
                    }
                
                }   //  Read body OK
            }   //  Read header OK
        }	//	Record object created OK


    //  Read through record headers and build list
    while( status )
        {
        status = precord->HeaderGetPosNextRecord( posNext );

        //  Are we at the end?
        if( status && posNext == 0 )
            break;

        if( status )
            {
            m_pstream->SeekPos( posNext );

            precord = theFactory.NewRecord( PFR_Record );
            if( precord )
                {
                status = precord->ReadHeader( m_pstream );
                if( status )
                    {
                    m_arrayRecords.Add( precord );
                    }
                }
            }

        }   //  while()

    //  If this is a chunk, we need to force it to read in ALL records
    //  immediately.
    if( status && m_whichStream == PSIO_Chunk )
        {
        for( long idxRec = 0; idxRec < GetRecordCount(); idxRec++ )
            {
            GetRecordFull( idxRec );
            }
        }

    m_state = pqpc_Empty;

    return status;
    }


CPQDIFRecord *  CPQDIF_PC_FlatFile::GetRecordFull( long index )
    {
    bool            status = TRUE;
    CPQDIFRecord *  prec = NULL;

    prec = GetRecord( index );
    ASSERT_VALID( prec );
    if( prec && m_pstream && m_pprocBody )
        {
        //  Connect the appropriate processor
        m_pstream->ConnectProcessor( m_pprocBody );

        //  Read the body
        status = prec->ReadBody( m_pstream );
        }

    //  If it didn't work, don't return the record pointer
    if( !status )
        prec = NULL;

	return prec;
    }


bool CPQDIF_PC_FlatFile::WriteRecordsToFile( void )
    {
    bool        status = false;

    CPQDIF_StreamIO *           pstrmWrite;
    CPQDIF_StreamProcessor *    pprocWriteHeader;
    CPQDIF_StreamProcessor *    pprocWriteBody;
    CPQDIF_StreamProcessor *    pprocWriteBodyFirstRecord;
    //
    //
    //  Clear out the old stream and processor stuff
    //
    if( m_pstream )
        delete m_pstream;
    if( m_pprocHeader )
        delete m_pprocHeader;
    if( m_pprocBody )
        delete m_pprocBody;
    //
    //
    //  This function does file output only
    //
    m_whichStream = PSIO_FlatFile;
    //
    //
    //  Init the basic stuff
    //
    pstrmWrite                = theFactory.NewStreamIO( m_whichStream );
    pprocWriteHeader          = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );
    pprocWriteBodyFirstRecord = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );

    //  Init the streams
    ((CPQDIF_S_FlatFile *) pstrmWrite)->New( m_fname.c_str() );

    //  Should we use a different body processor?
    switch( GetCompressionStyle() )
        {
        case ID_COMP_STYLE_TOTALFILE:
            //  We don't support this yet
            pprocWriteBody = NULL;
            break;

        case ID_COMP_STYLE_RECORDLEVEL:
            pprocWriteBody   = theFactory.NewStreamProcessor( GetCompressionAlgorithm() );
            break;

        case ID_COMP_STYLE_NONE:
        default:
            pprocWriteBody   = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );
            break;
        }

    //  Check all the objects
    if( pstrmWrite && pprocWriteHeader && pprocWriteBody && pprocWriteBodyFirstRecord )
        {
        status = true;
        }
    else
        {
        status = false;
        }

    //  Write the file out
    if( status )
        {
        long        countRecords;
        long        idxRecord;
        LINKABS4    posCurrentRecord;
        LINKABS4    posCurrentRecordBody;
        LINKABS4    posNextRecord;
        SIZE4       sizeHeader;
        SIZE4       sizeBody;
        CPQDIFRecord *  precord;

        //  Init
        countRecords = m_arrayRecords.GetSize();
        posCurrentRecord = 0;
        posNextRecord = 0;

        for( idxRecord = 0; idxRecord < countRecords; idxRecord++ )
            {
            precord = (CPQDIFRecord *) m_arrayRecords[ idxRecord ];
            if( !precord )
                break;

            //  GET HEADER INFO (& set initial header info)
            posCurrentRecord = posNextRecord;
            precord->HeaderSetPos( posCurrentRecord );
            precord->HeaderGetSize( sizeHeader, sizeBody );
            posCurrentRecordBody = posCurrentRecord + sizeHeader;

            //  WRITE THE BODY
            //  Connect the appropriate body processor
            //  (The first record is a little different)
            if( idxRecord == 0 )
                pstrmWrite->ConnectProcessor( pprocWriteBodyFirstRecord );
            else
                pstrmWrite->ConnectProcessor( pprocWriteBody );
            status = precord->WriteBody( pstrmWrite );

            //  This determines where the next record will go
            //  (a new posNextRecord).
            pstrmWrite->GetPos( posNextRecord );

            //  UPDATE THE HEADER INFO (w/next record pos and body size)
            sizeBody = (SIZE4) ( posNextRecord - posCurrentRecordBody );
            precord->HeaderSetSize( sizeHeader, sizeBody );
            if( idxRecord == (countRecords - 1) )
                {
                //  Last record!
                posNextRecord = 0;
                }
            precord->HeaderSetPosNextRecord( posNextRecord );

            //  WRITE THE HEADER
            pstrmWrite->ConnectProcessor( pprocWriteHeader );
            status = precord->WriteHeader( pstrmWrite );
            }

        pstrmWrite->Flush();
        }

    m_pstream     = pstrmWrite;
    m_pprocHeader = pprocWriteHeader;
    m_pprocBody   = pprocWriteBody;

    //  Don't keep this one around
    if( pprocWriteBodyFirstRecord )
        delete pprocWriteBodyFirstRecord;

	return status;
    }


bool CPQDIF_PC_FlatFile::WriteNew( void )
    {
    bool        status = FALSE;

    CPQDIF_StreamIO *           pstrmWrite;
    CPQDIF_StreamProcessor *    pprocWriteHeader;
    CPQDIF_StreamProcessor *    pprocWriteBody;
    CPQDIF_StreamProcessor *    pprocWriteBodyFirstRecord;

    //  We should have a new file name, but
    //  the original streams and processors should still be around.
    //  We will use these if we need to read the full records
    //  into memory.

    //  Are we dealing with a chunk or a file?
    if( m_chunk )
        m_whichStream = PSIO_Chunk;
    else
        m_whichStream = PSIO_FlatFile;

    //  Init the basic stuff
    pstrmWrite = theFactory.NewStreamIO( m_whichStream );
    pprocWriteHeader = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );
    pprocWriteBodyFirstRecord = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );

    //  Init the streams
    if( m_whichStream == PSIO_Chunk )
        {
        //  need to fix
        //((CPQDIF_S_Chunk *) pstrmWrite)->SetInput( m_chunk, m_sizeChunk );
        }
    else
        {
        ((CPQDIF_S_FlatFile *) pstrmWrite)->New( m_fname.c_str() );
        }

    //  Should we use a different body processor?
    switch( GetCompressionStyle() )
        {
        case ID_COMP_STYLE_TOTALFILE:
            //  We don't support this yet
            pprocWriteBody = NULL;
            break;

        case ID_COMP_STYLE_RECORDLEVEL:
            pprocWriteBody   = theFactory.NewStreamProcessor( GetCompressionAlgorithm() );
            break;

        case ID_COMP_STYLE_NONE:
        default:
            pprocWriteBody   = theFactory.NewStreamProcessor( ID_COMP_ALG_NONE );
            break;
        }

    //  Check all the objects
    if( pstrmWrite && pprocWriteHeader && pprocWriteBody && pprocWriteBodyFirstRecord )
        {
        status = TRUE;
        }
    else
        {
        status = FALSE;
        }

    //  Write the file out
    if( status )
        {
        long        countRecords;
        long        idxRecord;
        LINKABS4    posCurrentRecord;
        LINKABS4    posCurrentRecordBody;
        LINKABS4    posNextRecord;
        SIZE4       sizeHeader;
        SIZE4       sizeBody;
        CPQDIFRecord *  precord;

        //  Init
        countRecords = m_arrayRecords.GetSize();
        posCurrentRecord = 0;
        posNextRecord = 0;

        for( idxRecord = 0; idxRecord < countRecords; idxRecord++ )
            {
            precord = (CPQDIFRecord *) m_arrayRecords[ idxRecord ];
            if( !precord )
                break;

            //  MAKE SURE THE BODY IS UP-TO-DATE
            //  -- This must be done before we start making any changes
            //  to the record; otherwise, it will read using information
            //  from the new file rather than the old one.
            if( m_pstream && m_pprocBody )
                {
                m_pstream->ConnectProcessor( m_pprocBody );
                precord->ReadBody( m_pstream );
                }

            //  GET HEADER INFO (& set initial header info)
            posCurrentRecord = posNextRecord;
            precord->HeaderSetPos( posCurrentRecord );
            precord->HeaderGetSize( sizeHeader, sizeBody );
            posCurrentRecordBody = posCurrentRecord + sizeHeader;

            //  WRITE THE BODY
            //  Connect the appropriate body processor
            //  (The first record is a little different)
            if( idxRecord == 0 )
                pstrmWrite->ConnectProcessor( pprocWriteBodyFirstRecord );
            else
                pstrmWrite->ConnectProcessor( pprocWriteBody );
            status = precord->WriteBody( pstrmWrite );

            //  This determines where the next record will go
            //  (a new posNextRecord).
            pstrmWrite->GetPos( posNextRecord );

            //  UPDATE THE HEADER INFO (w/next record pos and body size)
            sizeBody = (SIZE4) ( posNextRecord - posCurrentRecordBody );
            precord->HeaderSetSize( sizeHeader, sizeBody );
            if( idxRecord == (countRecords - 1) )
                {
                //  Last record!
                posNextRecord = 0;
                }
            precord->HeaderSetPosNextRecord( posNextRecord );

            //  WRITE THE HEADER
            pstrmWrite->ConnectProcessor( pprocWriteHeader );
            status = precord->WriteHeader( pstrmWrite );
            }

        pstrmWrite->Flush();
        }

    //  Replace the old stream & processors
    if( m_pstream )
        delete m_pstream;
    if( m_pprocHeader )
        delete m_pprocHeader;
    if( m_pprocBody )
        delete m_pprocBody;
    m_pstream     = pstrmWrite;
    m_pprocHeader = pprocWriteHeader;
    m_pprocBody   = pprocWriteBody;

    //  Don't keep this one around
    if( pprocWriteBodyFirstRecord )
        delete pprocWriteBodyFirstRecord;

	return status;
    }


bool CPQDIF_PC_FlatFile::WriteIncremental( void )
    {
    bool        status = TRUE;

    if( !m_pstream->CanWriteIncremental() )
        status = FALSE;

    //  Write the file out
    if( status )
        {
        long            countRecords;
        long            idxRecord;
        LINKABS4        posCurrentRecord;
        LINKABS4        posCurrentRecordBody;
        LINKABS4        posCurrentRecordEnd;
        LINKABS4        posNextNewRecord;
        SIZE4           sizeHeader;
        SIZE4           sizeBody;
        CPQDIFRecord *  precord;
        CPQDIFRecord *  precordPrevious;

        //  Init
        countRecords = m_arrayRecords.GetSize();
        posCurrentRecord = 0;

        //  Find the end of the file
        posNextNewRecord = 0;
        for( idxRecord = 1; idxRecord < countRecords; idxRecord++ )
            {
            precord         = (CPQDIFRecord *) m_arrayRecords[ idxRecord     ];
            if( precord )
                {
                //  Find the end of this record
                precord->HeaderGetPos( posCurrentRecord );
                precord->HeaderGetSize( sizeHeader, sizeBody );
                posCurrentRecordEnd = posCurrentRecord + sizeHeader + sizeBody;
                
                //  Is this the max so far?
                if( posNextNewRecord < posCurrentRecordEnd )
                    {
                    posNextNewRecord = posCurrentRecordEnd;
                    }
                }
            }

        //  Don't even bother with record 0
        for( idxRecord = 1; idxRecord < countRecords; idxRecord++ )
            {
            precord         = (CPQDIFRecord *) m_arrayRecords[ idxRecord     ];
            precordPrevious = (CPQDIFRecord *) m_arrayRecords[ idxRecord - 1 ];
            if( !precord || !precordPrevious )
                break;

            //  Has this record changed?
            //  If not, don't do anything.
            if( !precord->GetChanged() )
                continue;

            //  This record now requires a new location --
            //  we'll position it at posNextNewRecord.

            //  GET HEADER INFO
            //  The sizeBody is NOT necessarily correct.
            posCurrentRecord = posNextNewRecord;
            precord->HeaderGetSize( sizeHeader, sizeBody );
            posCurrentRecordBody = posCurrentRecord + sizeHeader;

            //  WRITE THE BODY
            //  Connect the appropriate body processor
            m_pstream->ConnectProcessor( m_pprocBody );
            status = precord->WriteBody( m_pstream );
            //  This determines where any more new records go
            //  (a new posNextNewRecord).
            m_pstream->GetPos( posNextNewRecord );

            //  UPDATE THE HEADER INFO
            if( idxRecord == (countRecords - 1) )
                {
                //  Last record!
                precord->HeaderSetPosNextRecord( 0 );
                }
            precord->HeaderSetPos( posCurrentRecord );
            sizeBody = (SIZE4) ( posNextNewRecord - posCurrentRecordBody );
            precord->HeaderSetSize( sizeHeader, sizeBody );

            //  We don't need to change this, since the next record
            //  has probably not changed. If it has, this header will be modified
            //  again anyway.
            //      precord->HeaderSetPosNextRecord( posNextRecord );

            //  WRITE THE HEADER
            m_pstream->ConnectProcessor( m_pprocHeader );
            status = precord->WriteHeader( m_pstream );

            //  UPDATE & WRITE THE HEADER FOR THE PREVIOUS RECORD
            precordPrevious->HeaderSetPosNextRecord( posCurrentRecord );
            status = precordPrevious->WriteHeader( m_pstream );
            }
        }

	return status;
    }


bool CPQDIF_PC_FlatFile::WriteToStream(CPQDIF_StreamIO *pstrm)
	{
		bool        status = FALSE;

		CPQDIF_StreamProcessor *    pprocWriteHeader;
		CPQDIF_StreamProcessor *    pprocWriteBody;
		CPQDIF_StreamProcessor *    pprocWriteBodyFirstRecord;

		//  Init the basic stuff
		pprocWriteHeader = theFactory.NewStreamProcessor(ID_COMP_ALG_NONE);
		pprocWriteBodyFirstRecord = theFactory.NewStreamProcessor(ID_COMP_ALG_NONE);

		//  Should we use a different body processor?
		switch (GetCompressionStyle())
		{
		case ID_COMP_STYLE_TOTALFILE:
			//  We don't support this yet
			pprocWriteBody = NULL;
			break;

		case ID_COMP_STYLE_RECORDLEVEL:
			pprocWriteBody = theFactory.NewStreamProcessor(GetCompressionAlgorithm());
			break;

		case ID_COMP_STYLE_NONE:
		default:
			pprocWriteBody = theFactory.NewStreamProcessor(ID_COMP_ALG_NONE);
			break;
		}

		//  Check all the objects
		if (pstrm && pprocWriteHeader && pprocWriteBody && pprocWriteBodyFirstRecord)
		{
			status = TRUE;
		}
		else
		{
			status = FALSE;
		}

		//  Write the file out
		if (status)
		{
			long        countRecords;
			long        idxRecord;
			LINKABS4    posCurrentRecord;
			LINKABS4    posCurrentRecordBody;
			LINKABS4    posNextRecord;
			SIZE4       sizeHeader;
			SIZE4       sizeBody;
			CPQDIFRecord *  precord;

			//  Init
			countRecords = m_arrayRecords.GetSize();
			posCurrentRecord = 0;
			posNextRecord = 0;

			for (idxRecord = 0; idxRecord < countRecords; idxRecord++)
			{
				precord = (CPQDIFRecord *)m_arrayRecords[idxRecord];
				if (!precord)
					break;

				//  MAKE SURE THE BODY IS UP-TO-DATE
				//  -- This must be done before we start making any changes
				//  to the record; otherwise, it will read using information
				//  from the new file rather than the old one.
				if (m_pstream && m_pprocBody)
				{
					m_pstream->ConnectProcessor(m_pprocBody);
					precord->ReadBody(m_pstream);
				}

				//  GET HEADER INFO (& set initial header info)
				posCurrentRecord = posNextRecord;
				precord->HeaderSetPos(posCurrentRecord);
				precord->HeaderGetSize(sizeHeader, sizeBody);
				posCurrentRecordBody = posCurrentRecord + sizeHeader;

				//  WRITE THE BODY
				//  Connect the appropriate body processor
				//  (The first record is a little different)
				if (idxRecord == 0)
					pstrm->ConnectProcessor(pprocWriteBodyFirstRecord);
				else
					pstrm->ConnectProcessor(pprocWriteBody);
				status = precord->WriteBody(pstrm);

				//  This determines where the next record will go
				//  (a new posNextRecord).
				pstrm->GetPos(posNextRecord);

				//  UPDATE THE HEADER INFO (w/next record pos and body size)
				sizeBody = (SIZE4)(posNextRecord - posCurrentRecordBody);
				precord->HeaderSetSize(sizeHeader, sizeBody);
				if (idxRecord == (countRecords - 1))
				{
					//  Last record!
					posNextRecord = 0;
				}
				precord->HeaderSetPosNextRecord(posNextRecord);

				//  WRITE THE HEADER
				pstrm->ConnectProcessor(pprocWriteHeader);
				status = precord->WriteHeader(pstrm);
			}

			pstrm->Flush();
		}

		// Clean up.
		if (pprocWriteHeader)
		{
			delete pprocWriteHeader;
			pprocWriteHeader = NULL;
		}
		if (pprocWriteBodyFirstRecord)
		{
			delete pprocWriteBodyFirstRecord;
			pprocWriteBodyFirstRecord = NULL;
		}
		if (pprocWriteBody)
		{
			delete pprocWriteBody;
			pprocWriteBody = NULL;
		}

		return status;
	}



long CPQDIF_PC_FlatFile::GetCompressionAlgorithm() 
    {
    long                algReturn = 0;
    CPQDIF_Element *    pel;
    long                typePhysical;
    PQDIFValue          value;
    CPQDIFRecord *      precord;
    CPQDIF_E_Collection * pcollFirst;

    if( GetRecordCount() > 0 )
        {
        //  Get the main collection of the first record
        precord = GetRecord( 0 );
        ASSERT( precord );
        pcollFirst = precord->GetMainCollection();

        //  Add it to this collection
        if( pcollFirst )
            {
            pel = pcollFirst->GetElement( tagCompressionAlgorithmID );
            if( pel )
                {
                if( pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR )
                    {
                    CPQDIF_E_Scalar *   psc = (CPQDIF_E_Scalar *) pel;
                    ASSERT_VALID( psc );
                
                    psc->GetValue( typePhysical, value );
                    if( typePhysical == ID_PHYS_TYPE_UNS_INTEGER4 )
                        {
                        algReturn = value.uint4;
                        }
                    }
                }
            }
        }   //  Enough records

	return algReturn;
    }


void CPQDIF_PC_FlatFile::SetCompressionAlgorithm( long algNew ) 
    {
	CPQDIF_E_Scalar *   psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
    if( psc )
        {
        PQDIFValue  value;
        CPQDIFRecord *      precord;
        CPQDIF_E_Collection * pcollFirst;

        //  Specify it...
        value.uint4 = algNew;
        psc->SetValue( ID_PHYS_TYPE_UNS_INTEGER4, value );
        psc->SetTag( tagCompressionAlgorithmID );

        //  Get the main collection of the first record
        precord = GetRecord( 0 );
        pcollFirst = precord->GetMainCollection();

        //  Add it to this collection
        if( pcollFirst )
            {
            pcollFirst->AddOrReplace( psc );
            //m_canWriteIncremental = FALSE;    //  need to fix
            }
        }
    }


long CPQDIF_PC_FlatFile::GetCompressionStyle() 
    {
    long                styleReturn = 0;
    CPQDIF_Element *    pel;
    long                typePhysical;
    PQDIFValue          value;
    CPQDIFRecord *      precord;
    CPQDIF_E_Collection * pcollFirst;

    if( GetRecordCount() > 0 )
        {
        //  Get the main collection of the first record
        precord = GetRecord( 0 );
        ASSERT( precord );
        pcollFirst = precord->GetMainCollection();

        //  Add it to this collection
        if( pcollFirst )
            {
            pel = pcollFirst->GetElement( tagCompressionStyleID );
            if( pel )
                {
                if( pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR )
                    {
                    CPQDIF_E_Scalar *   psc = (CPQDIF_E_Scalar *) pel;
                    ASSERT_VALID( psc );
                
                    psc->GetValue( typePhysical, value );
                    if( typePhysical == ID_PHYS_TYPE_UNS_INTEGER4 )
                        {
                        styleReturn = value.uint4;
                        }
                    }
                }
            }
        }

	return styleReturn;
    }


void CPQDIF_PC_FlatFile::SetCompressionStyle( long styleNew ) 
    {
	CPQDIF_E_Scalar *   psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
    if( psc )
        {
        PQDIFValue  value;
        CPQDIFRecord *      precord;
        CPQDIF_E_Collection * pcollFirst;

        //  Specify it...
        value.uint4 = styleNew;
        psc->SetValue( ID_PHYS_TYPE_UNS_INTEGER4, value );
        psc->SetTag( tagCompressionStyleID );

        //  Get the main collection of the first record
        precord = GetRecord( 0 );
        pcollFirst = precord->GetMainCollection();

        //  Add it to this collection
        if( pcollFirst )
            {
            pcollFirst->AddOrReplace( psc );
            //m_canWriteIncremental = FALSE;    //  need to fix
            }
        }
    }


void CPQDIF_PC_FlatFile::UpdateInformation( void )
    {
    CPQDIFRecord *      precord;
    bool    m_canWriteIncremental = false;  //  need to fix

    //  See if this flag needs to be deactivated...
    if( m_canWriteIncremental )
        {
        precord = GetRecord( 0 );
        if( !precord )
            {
            m_canWriteIncremental = FALSE;
            }
        else
            {
            //  The first record must not be changed.
            //  Otherwise, we cannot write incremental.
            if( precord->GetChanged() )
                {
                m_canWriteIncremental = FALSE;
                }
            }
        }
    }


void CPQDIF_PC_FlatFile::SetChunkInput( BYTE * chunk, long size )
    {
    if( chunk && size > 0 )
        {
        m_chunk = chunk;
        m_sizeChunk = size;
        }
    else
        {
        m_chunk = NULL;
        m_sizeChunk = 0;
        }
    }


bool CPQDIF_PC_FlatFile::GetChunkOutputSize( long& size )
    {
    bool    status = FALSE;

    //  If it's a chunk, then pull out all the data
    if( m_whichStream == PSIO_Chunk )
        {
        status = ((CPQDIF_S_Chunk *) m_pstream)->GetOutputSize( size );
        }

    return status;
    }


bool CPQDIF_PC_FlatFile::GetChunkOutput( BYTE * chunk, long size )
    {
    bool    status = FALSE;

    //  If it's a chunk, then pull out all the data
    if( m_whichStream == PSIO_Chunk )
        {
        status = ((CPQDIF_S_Chunk *) m_pstream)->GetOutput( chunk, size );
        }

    return status;
    }
