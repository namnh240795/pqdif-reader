/*
**  Class:          CPQDIFRecord
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

class CPQDIFRecord;

typedef enum
    {
        pqpc_Empty,         //  Newly created
        pqpc_MatchesFile,   //  Contents of memory same as disk
        pqpc_Modified       //  Contents of memory have been modified
    } PQPC_Status;

class CPQDIF_PersistController
    {
    public:
        CPQDIF_PersistController();
        virtual ~CPQDIF_PersistController();

    //  Public member functions
    public:
        virtual long GetRecordCount( void ) const;
        virtual CPQDIFRecord *  GetRecord( long index );
        virtual CPQDIFRecord *  GetRecordFull( long index ) = 0;
        virtual bool InsertRecord( CPQDIFRecord * prec, long indexToInsert );
        virtual bool RemoveRecord( long index );

    //  Other public member functions
    public:
        virtual long CreateContainerRecord
            (
            const   char *  language,
            const   char *  title,
            const   char *  subject,
            const   char *  author,
            const   char *  keywords,
            const   char *  comments,
            const   char *  lastSavedBy,
            const   char *  application,
            const   char *  security,
            const   char *  owner,
            const   char *  copyright,
            const   char *  trademarks,
            const   char *  notes
            );
        virtual long CreateContainerRecord
            (
            const   char *  szFileName,
            const   TIMESTAMPPQDIF * timeCreate,
            const   long    lMajor,
            const   long    lMinor,
            const   long    lCompatMajor,
            const   long    lCompatMinor
            );

    long CreateDataSourceRecord
            (
                    long    indexInsert,
            const   GUID&   idDataSourceType,
            const   GUID&   idVendor,
            const   GUID&   idEquipment,
            const   char *  serialNumberDS, //  Optional (can be NULL)
            const   char *  versionDS,      //  Optional
            const   char *  nameDS,         
            const   char *  ownerDS,        //  Optional
            const   char *  locationDS,     //  Optional
            const   char *  timeZoneDS      //  Optional
            );
    long CreateMonitorSettingsRecord
            (
                    long                indexInsert
            );

    long CreateMonitorSettingsRecord
            (
                    long                indexInsert,
            const   TIMESTAMPPQDIF *    timeEffective,
            const   TIMESTAMPPQDIF *    timeInstalled,
            const   TIMESTAMPPQDIF *    timeRemoved,  
                    bool                useCal,       
                    bool                useTrans      
            );

    long CreateObservationRecord
            (
                    long    indexInsert,
            const   char *  name,
            const   TIMESTAMPPQDIF *    timeCreate,
            const   TIMESTAMPPQDIF *    timeStart,
                    UINT4               idTriggerMethod,
            const   TIMESTAMPPQDIF *    timeTriggered,      //  Optional (can be NULL)
                    long                countTriggers,
                    UINT4 *             aidxChannelTrigger  //  Array of channel indices[ countTriggers ]
                                                            //  Optional (can be NULL)
            );

    //  Member data
    protected:
        CPQPtrArray     m_arrayRecords;
        PQPC_Status     m_state;
    };

