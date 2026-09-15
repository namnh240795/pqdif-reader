/*
**  Class:              CPQDIF_R_General
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

//  Forward-declare these classes
class CPQDIF_StreamIO;

class CPQDIF_E_Collection;
class CPQDIF_E_Scalar;
class CPQDIF_E_Vector;

class PQAlloc;

union PQDIFValue;


class CPQDIF_R_General : public CPQDIFRecord
    {
    //  Subclasses are friends - need to fix?
    friend class CPQDIF_R_Observation;
    friend class CPQDIF_R_DataSource;

    public:
        CPQDIF_R_General();
        virtual ~CPQDIF_R_General();

    // Operations
    public:
        virtual bool ReadHeader( CPQDIF_StreamIO * pstream );
        virtual bool ReadBody( CPQDIF_StreamIO * pstream );

        virtual bool WriteHeader( CPQDIF_StreamIO * pstream );
        virtual bool WriteBody( CPQDIF_StreamIO * pstream );

        // Attributes
    public:
        virtual bool HeaderGetPos( LINKABS4& pos ) const
            {
            pos = m_posThisRecord;
            return true;
            }
        virtual bool HeaderSetPos( LINKABS4 pos )
            {
            m_posThisRecord = pos;
            return true;
            }
        virtual bool HeaderGetTag( GUID& tagRecord ) const
            {
            tagRecord = m_headerRecord.tagRecordType;
            return true;
            }
        virtual bool HeaderSetTag( GUID tagRecord )
            {
            m_headerRecord.tagRecordType = tagRecord;
            return true;
            }
        virtual bool HeaderGetSize( SIZE4& sizeHeader, SIZE4& sizeBody ) const
            {
            sizeHeader = m_headerRecord.sizeHeader;
            sizeBody   = m_headerRecord.sizeData  ;
            return true;
            }
        virtual bool HeaderSetSize( SIZE4 sizeHeader, SIZE4 sizeBody )
            {
            m_headerRecord.sizeHeader = sizeHeader;
            m_headerRecord.sizeData   = sizeBody  ;
            return true;
            }
        virtual bool HeaderGetChecksum( UINT& checksum ) const
            {
            checksum = m_headerRecord.checksum;
            return true;
            }
        virtual bool HeaderSetChecksum( UINT checksum )
            {
            m_headerRecord.checksum = checksum;
            return true;
            }
        virtual bool HeaderGetPosNextRecord( LINKABS4& pos ) const
            {
            pos = m_headerRecord.linkNextRecord;
            return true;
            }
        virtual bool HeaderSetPosNextRecord( LINKABS4 pos )
            {
            m_headerRecord.linkNextRecord = pos;
            return true;
            }
        virtual CPQDIF_E_Collection * GetMainCollection( void ) const
            {
            return m_pcollMain;
            }
        virtual bool SetMainCollection( CPQDIF_E_Collection * collMain );
        virtual bool GetChanged( void )
            {
            return m_changed;
            }
        virtual void SetChanged( bool changed )
            {
            m_changed = changed;
            }

        bool SetTimeInMainCollection  (const GUID &tag, const TIMESTAMPPQDIF & timeTime);
        bool SetREAL8InMainCollection (const GUID &tag, const REAL8 dVal);
        bool SetBOOL4InMainCollection (const GUID &tag, const BOOL4 bVal);

        bool GetTimeInMainCollection  (const GUID &tag, TIMESTAMPPQDIF & timeTime);
        bool GetREAL8InMainCollection (const GUID &tag, REAL8 & dVal);
        bool GetBOOL4InMainCollection (const GUID &tag, BOOL4 & bVal);

    // Overrides
    public:

	//  Implementation
	protected:
        bool BufferUpCollection
            ( 
            CPQDIF_E_Collection *   pcoll,
            PQAlloc&                allocator,
            long                    idxRoot = -1
            );

    //  Member data
    protected:
        c_record_mainheader     m_headerRecord;
        LINKABS4                m_posThisRecord;

        CPQDIF_E_Collection *   m_pcollMain;    //  Main collection element
                                                //  (if NULL, record has not been read yet)

        bool                    m_changed;      //  Record changed flag.

    };

