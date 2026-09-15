/*
**  Class:          CPQDIF_PersistController
**  Description:    The base class for persistence controller classes.These classes are used to control how the PQDIF data is stored
**                  and retrieved from a particular medium.
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

CPQDIF_PersistController::CPQDIF_PersistController()
    {
    m_arrayRecords.SetSize( 0, 8 );
    m_state = pqpc_Empty;
    }


CPQDIF_PersistController::~CPQDIF_PersistController()
    {
    CPQDIFRecord *  prec = NULL;
    //  Delete record objects in array
    for( int idx = 0; idx < m_arrayRecords.GetSize(); idx++ )
        {
        prec = (CPQDIFRecord *)(m_arrayRecords[ idx ]);
        delete prec;
        }
    }


CPQDIFRecord *  CPQDIF_PersistController::GetRecord( long index )
    {
    CPQDIFRecord *  prec = NULL;

    if( index >= 0 && index < m_arrayRecords.GetSize() )
        {
        prec = (CPQDIFRecord *) m_arrayRecords[ index ];
        }

    return prec;
    }


long CPQDIF_PersistController::GetRecordCount( void ) const
    {
    return m_arrayRecords.GetSize();
    }


bool CPQDIF_PersistController::InsertRecord( CPQDIFRecord * prec, long indexToInsert )
    {
    bool            status = FALSE;

    if( indexToInsert >= 0 && indexToInsert <= m_arrayRecords.GetSize() )
        {
        m_arrayRecords.InsertAt( indexToInsert, prec );
        status = TRUE;
        }

    return status;
    }


bool CPQDIF_PersistController::RemoveRecord( long index )
    {
    bool            status = FALSE;

    if( index >= 0 && index < m_arrayRecords.GetSize() )
        {
        m_arrayRecords.RemoveAt( index );
        status = TRUE;
        }

    return status;
    }


long CPQDIF_PersistController::CreateContainerRecord
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
            )
    {
    CPQDIFRecord *          prec;
    long                    idxNewRecord = 0;
    CPQDIF_E_Collection *   pcollMain;

    prec = theFactory.NewRecord( PFR_Container );
    if( prec )
        {
        //  Create main collection for new record
        pcollMain = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollMain )
            {
            //  Add string entries
            pcollMain->SetVectorString( tagLanguage, language );
            pcollMain->SetVectorString( tagTitle, title );
            pcollMain->SetVectorString( tagSubject, subject );
            pcollMain->SetVectorString( tagAuthor, author );
            pcollMain->SetVectorString( tagKeywords, keywords );
            pcollMain->SetVectorString( tagComments, comments );
            pcollMain->SetVectorString( tagLastSavedBy, lastSavedBy );
            pcollMain->SetVectorString( tagApplication, application );
            pcollMain->SetVectorString( tagSecurity, security );

            pcollMain->SetVectorString( tagOwner, owner );
            pcollMain->SetVectorString( tagCopyright, copyright );
            pcollMain->SetVectorString( tagTrademarks, trademarks );

            pcollMain->SetVectorString( tagNotes, notes );

            //  Hand it over to the record object
            prec->SetMainCollection( pcollMain );

            //  Stuff the record into the first slot
            m_arrayRecords.SetAtGrow( 0, prec );
            }
        }

    return idxNewRecord;
    }

long CPQDIF_PersistController::CreateContainerRecord
    (
    const   char *  szFileName,
    const   TIMESTAMPPQDIF * timeCreate,
    const   long    lMajor,
    const   long    lMinor,
    const   long    lCompatMajor,
    const   long    lCompatMinor
    )
    {
    CPQDIFRecord *          prec;
    long                    idxNewRecord = 0;
    CPQDIF_E_Collection *   pcollMain;

    prec = theFactory.NewRecord( PFR_Container );
    if( prec )
        {
        //  Create main collection for new record
        pcollMain = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollMain )
            {
            //  Add string entries
            pcollMain->SetVectorString( tagFileName, szFileName );

            pcollMain->SetScalarTimeStamp( tagCreation, *timeCreate );

            UINT4 aVer[4];

            aVer[0] = lMajor;
            aVer[1] = lMinor;
            aVer[2] = lCompatMajor;
            aVer[3] = lCompatMinor;

            pcollMain->SetVectorUINT4( tagVersionInfo, aVer, 4 );

            //  Hand it over to the record object
            prec->SetMainCollection( pcollMain );

            //  Stuff the record into the first slot
            m_arrayRecords.SetAtGrow( 0, prec );
            }
        }

    return idxNewRecord;
    }



long CPQDIF_PersistController::CreateDataSourceRecord
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
            )
    {
    CPQDIFRecord *          prec;
    long                    idxNewRecord = indexInsert; //  Is this right? need to fix
    CPQDIF_E_Collection *   pcollMain;
    CPQDIF_E_Collection *   pcollDefinitions;

    prec = theFactory.NewRecord( PFR_DataSource );
    if( prec )
        {
        //  Create main collection for new record
        pcollMain = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollMain )
            {
            //  Add GUID & string entries
            pcollMain->SetScalarGUID( tagDataSourceTypeID, idDataSourceType );
            pcollMain->SetScalarGUID( tagVendorID, idVendor );
            pcollMain->SetScalarGUID( tagEquipmentID, idEquipment );

            pcollMain->SetVectorString( tagSerialNumberDS, serialNumberDS );
            pcollMain->SetVectorString( tagVersionDS, versionDS );
            pcollMain->SetVectorString( tagNameDS, nameDS );
            pcollMain->SetVectorString( tagOwnerDS, ownerDS );
            pcollMain->SetVectorString( tagLocationDS, locationDS );
            pcollMain->SetVectorString( tagTimeZoneDS, timeZoneDS );

            //  Insert an empty collection ...
            pcollDefinitions = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            pcollDefinitions->SetTag( tagChannelDefns );
            pcollMain->Add( pcollDefinitions );

            //  Hand it over to the record object
            prec->SetMainCollection( pcollMain );

            //  Add the record
            InsertRecord( prec, indexInsert );
            }
        }

    return idxNewRecord;
    }

long CPQDIF_PersistController::CreateMonitorSettingsRecord
            (
                    long                indexInsert,
            const   TIMESTAMPPQDIF *    timeEffective,
            const   TIMESTAMPPQDIF *    timeInstalled,
            const   TIMESTAMPPQDIF *    timeRemoved,  
                    bool                useCal,       
                    bool                useTrans      
            )
    {
    long                    idxNewRecord = indexInsert; //  Is this right? need to fix
    CPQDIF_R_Settings *     prec;
    CPQDIF_E_Collection *   pcollMain;

    prec = (CPQDIF_R_Settings *) theFactory.NewRecord( PFR_MonitorSettings );
    if( prec )
        {
        //  Create main collection for new record
        pcollMain = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollMain )
            {
            prec->SetMainCollection( pcollMain );

            prec->SetInfo( *timeEffective,
                           *timeInstalled,
                           *timeRemoved,  
                            useCal,       
                            useTrans );

            //  Add the record
            InsertRecord( prec, indexInsert );
            }
        }

    return idxNewRecord;
    }

long CPQDIF_PersistController::CreateMonitorSettingsRecord
            (
            long indexInsert
            )
    {
    long                    idxNewRecord = indexInsert; //  Is this right? need to fix
    CPQDIF_R_Settings *     prec;
    CPQDIF_E_Collection *   pcollMain;

    prec = (CPQDIF_R_Settings *) theFactory.NewRecord( PFR_MonitorSettings );
    if( prec )
        {
        //  Create main collection for new record
        pcollMain = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollMain )
            {
            prec->SetMainCollection( pcollMain );

            //  Add the record
            InsertRecord( prec, indexInsert );
            }
        }

    return idxNewRecord;
    }

long CPQDIF_PersistController::CreateObservationRecord
            (
                    long                indexInsert,
            const   char *              name,
            const   TIMESTAMPPQDIF *    timeCreate,
            const   TIMESTAMPPQDIF *    timeStart,
                    UINT4               idTriggerMethod,
            const   TIMESTAMPPQDIF *    timeTriggered,      //  Optional (can be NULL)
                    long                countTriggers,
                    UINT4 *             aidxChannelTrigger  //  Array of channel indices[ countTriggers ]
                                                            //  Optional (can be NULL)
            )
    {
    CPQDIFRecord *          prec;
    long                    idxNewRecord = indexInsert; //  Is this right? need to fix
    CPQDIF_E_Collection *   pcollMain;
    CPQDIF_E_Collection *   pcollInstances;

    //  Triggers
    long                    idxTrigger;
    CPQDIF_E_Vector *       pvectChannelIdx;
    PQDIFValue              valuePQDIF;

	//	Create and populate the new record
    prec = theFactory.NewRecord( PFR_Observation );
    if( prec )
        {
        //  Create main collection for new record
        pcollMain = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollMain )
            {
            //  Add entries
            pcollMain->SetVectorString( tagObservationName, name );
            pcollMain->SetScalarTimeStamp( tagTimeCreate, *timeCreate );
            pcollMain->SetScalarTimeStamp( tagTimeStart, *timeStart );
            pcollMain->SetScalarUINT4( tagTriggerMethodID, idTriggerMethod );

            //  Optional time triggered
            if( timeTriggered )
                {
                pcollMain->SetScalarTimeStamp( tagTimeTriggered, *timeTriggered );
                }

            //  Optional triggers
            if( countTriggers > 0 && aidxChannelTrigger )
                {
                pvectChannelIdx = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
                if( pvectChannelIdx )
                    {
                    pvectChannelIdx->SetTag( tagChannelTriggerIdx );
                    pvectChannelIdx->SetPhysicalType( ID_PHYS_TYPE_UNS_INTEGER4 );
                    pvectChannelIdx->SetCount( countTriggers );

                    for( idxTrigger = 0; idxTrigger < countTriggers; idxTrigger++ )
                        {
                        valuePQDIF.uint4 = aidxChannelTrigger[ idxTrigger ];
                        pvectChannelIdx->SetValue( idxTrigger, valuePQDIF );
                        }

                    pcollMain->Add( pvectChannelIdx );
                    }
                }

            //  Insert an empty collection ...
            pcollInstances = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            pcollInstances->SetTag( tagChannelInstances );
            pcollMain->Add( pcollInstances );

            //  Hand it over to the record object
            prec->SetMainCollection( pcollMain );

            //  Add the record
            InsertRecord( prec, indexInsert );
            }
        }

    return idxNewRecord;
    }
