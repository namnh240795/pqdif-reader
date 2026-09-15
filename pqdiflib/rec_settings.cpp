/*
**  Class:          CPQDIF_R_Settings
**  Description:    Implements a PQDIF record "wrapper" for the settings record. You can cast a standard record object to this class.
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

CPQDIF_R_Settings::~CPQDIF_R_Settings()
{

	return;

}

long CPQDIF_R_Settings::GetCountChannels( void )
{
    long                    count = 0;
    CPQDIF_E_Collection *   pcolCI;

    pcolCI = GetChannelSettings();
    if( pcolCI )
    {
        count = pcolCI->GetCount(); 
    }

    return count;
}


bool CPQDIF_R_Settings::GetConnectionInfo
        (
        UINT4&  connectionType
        )
{
    bool    status = false;

    CPQDIF_E_Scalar *       psc;

    //  Init
	connectionType = 0;

    psc = FindScalarInCollection( m_pcollMain, tagSettingPhysicalConnection );
    if( psc )
		status = psc->GetValueUINT4( connectionType );

	return status;
}


bool CPQDIF_R_Settings::GetInfo
        (
        TIMESTAMPPQDIF&     timeEffective,
        TIMESTAMPPQDIF&     timeInstalled,
        TIMESTAMPPQDIF&     timeRemoved,  
        bool&               useCal,       
        bool&               useTrans      
        )
{
    bool    status = false;
    bool    foundItem = false;

    CPQDIF_E_Scalar *       psc;

    //  Init
    memset( &timeEffective, 0, sizeof( timeEffective ) );
    memset( &timeInstalled, 0, sizeof( timeEffective ) );
    memset( &timeRemoved, 0, sizeof( timeEffective ) );
    useCal = false;
    useTrans = false;

    //  Find effective time (required)
    status = true;
    if( status )
    {
        status = false;
        psc = FindScalarInCollection( m_pcollMain, tagEffective );
        if( psc )
        {
            foundItem = psc->GetValueTimeStamp( timeEffective );
            if( foundItem )
            {
                status = true;
            }
        }
    }

    //  Find installed time (optional)
    if( status )
    {
        psc = FindScalarInCollection( m_pcollMain, tagTimeInstalled );
        if( psc )
        {
            foundItem = psc->GetValueTimeStamp( timeInstalled );
        }
    }

    //  Find removed time (optional)
    if( status )
    {
        psc = FindScalarInCollection( m_pcollMain, tagTimeRemoved );
        if( psc )
        {
            foundItem = psc->GetValueTimeStamp( timeRemoved );
        }
    }

    //  Find useCal (required)
    if( status )
    {
        status = false;
        psc = FindScalarInCollection( m_pcollMain, tagUseCalibration );
        if( psc )
        {
            foundItem = psc->GetValueBOOL4( useCal );
            if( foundItem )
            {
                status = true;
            }
        }
    }

    //  Find useTrans (required)
    if( status )
    {
        status = false;
        psc = FindScalarInCollection( m_pcollMain, tagUseTransducer );
        if( psc )
        {
            foundItem = psc->GetValueBOOL4( useTrans );
            if( foundItem )
            {
                status = true;
            }
        }
    }

    return status;
}

bool CPQDIF_R_Settings::GetChannelInfo (long idxChannel, UINT4 & idxChannelDefn) 
    {
    //
    //
    //  Initialize
    //
    bool    foundItem = false;

    idxChannelDefn = 999999999;

    //  Find the appropriate channel collection
    CPQDIF_E_Collection * pcollMain = GetOneChannelSetting( idxChannel );
    if( pcollMain )
        {
        PQDIFValue              value;
        //  Find channel defn (required)
        foundItem = GetScalarValueInCollection( pcollMain, tagChannelDefnIdx, ID_PHYS_TYPE_UNS_INTEGER4, value );
        if( foundItem )
            {
            idxChannelDefn = value.uint4;
            }
        }

    return foundItem;
    }

#ifdef zap
//
//
//  Most of the returned items are optional.  This function
//  returns 0 for those that it can't find.  It returns all of
//  the values it can find.  It should probably return false
//  if it can't find the channel definition index.  I didn't
//  change that for fear of side effects.
// 
//  This function should probably not be used.  Instead,
//  individual queries on each element would be more appropriate
//  so that proper action can be taken when one is not available.
//  Using a value of 0 to indicate non-use has many potential
//  problems - EWG.
//
bool CPQDIF_R_Settings::GetChannelInfo
        (
        long            idxChannel,
        UINT4&          idxChannelDefn,
        UINT4&          triggerTypeID,
        REAL8&          fullScale,  
        REAL8&          noiseFloor, 
        REAL8&          triggerLow, 
        REAL8&          triggerHigh,
        REAL8&          triggerRate,
        CPQDIF_E_Vector&    triggerShapeParam   //  Array of [3] ... or NULL
        //REAL8 *         triggerShapeParam   //  Array of [3] ... or NULL
        ) 
{
    bool    status = false;
    bool    foundItem;

    CPQDIF_E_Collection *   pcollMain;
    CPQDIF_E_Collection *   pcollChannel = NULL;
    CPQDIF_E_Vector *       pvect;
    PQDIFValue              value;

            long        count;
            long        idx;
            double      valueREAL8;
    const   long        countParams = 3;

    //  Find the appropriate channel collection
    pcollMain = GetOneChannelSetting( idxChannel );
    if( pcollMain )
    {
        status = true;
    }

    //  Find channel defn (required)
    if( status )
    {
        foundItem = GetScalarValueInCollection( pcollMain, tagChannelDefnIdx, ID_PHYS_TYPE_UNS_INTEGER4, value );
        if( foundItem )
        {
            idxChannelDefn = value.uint4;
            status = true;
        }
    }

    //  Find trigger type (optional)
    if( status )
    {
        foundItem = GetScalarValueInCollection( pcollMain, tagTriggerTypeID, ID_PHYS_TYPE_UNS_INTEGER4, value );
        if( foundItem )
        {
            triggerTypeID = value.uint4;
            status = true;
        }
    }

    //  Find full scale (optional)
    if( status )
    {
//        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagFullScale, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            fullScale = value.real8;
            status = true;
        }
    }

    //  Find noise floor (optional)
    if( status )
    {
//        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagNoiseFloor, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            noiseFloor = value.real8;
            status = true;
        }
    }

    //  Find trigger low (optional)
    if( status )
    {
//        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagTriggerLow, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            triggerLow = value.real8;
            status = true;
        }
    }

    //  Find trigger high (optional)
    if( status )
    {
//        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagTriggerHigh, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            triggerHigh = value.real8;
            status = true;
        }
    }


    //  Find trigger rate (optional)
    if( status )
    {
//        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagTriggerRate, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            triggerRate = value.real8;
            status = true;
        }
    }

    //  Find trigger shape (optional)
    if( status )
    {
        //status = false;

        pvect = FindVectorInCollection( pcollMain, tagTriggerShapeParam );
        if( pvect )
        {
            //  Check size of vector
            pvect->GetCount( count );
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_REAL8
                && count == countParams )
            {
                //  Copy them over (pvect --> triggerShapeParam)
                triggerShapeParam.SetCount( count );
                for( idx = 0; idx < count; idx++ )
                {
                    pvect->GetValueREAL8( idx, valueREAL8 );
                    triggerShapeParam.SetValueREAL8( idx, valueREAL8 );
                }
                status = true;

            }   //  Got the right physical type and array size
        }   //  Found vector
    }

    return status;
}
#endif


bool CPQDIF_R_Settings::GetChanTrans
        (
        long                idxChannel,
        UINT4&              xdTransformerTypeID,
        REAL8&              xdSystemSideRatio,  
        REAL8&              xdMonitorSideRatio,
        CPQDIF_E_Vector&    xdFrequencyResponse //  Array [n]... or NULL
        )
{
    bool    status = false;
    bool    foundItem;

    CPQDIF_E_Collection *   pcollMain;
    CPQDIF_E_Vector *       pvect;
    PQDIFValue              value;

    long        count;
    long        idx;
    double      valueREAL8;

    //  Find the appropriate channel collection
    pcollMain = GetOneChannelSetting( idxChannel );
    if( pcollMain )
    {
        status = true;
    }

    //  Find xdTransformerTypeID (required)
    if( status )
    {
        foundItem = GetScalarValueInCollection( pcollMain, tagXDTransformerTypeID, ID_PHYS_TYPE_UNS_INTEGER4, value );
        if( foundItem )
        {
            xdTransformerTypeID = value.uint4;
            status = true;
        }
    }

    //  Find xdSystemSideRatio (required)
    if( status )
    {
        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagXDSystemSideRatio, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            xdSystemSideRatio = value.real8;
            status = true;
        }
    }

    //  Find xdMonitorSideRatio (required)
    if( status )
    {
        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagXDMonitorSideRatio, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            xdMonitorSideRatio = value.real8;
            status = true;
        }
    }

    //  Find xdFrequencyResponse (required)
    if( status )
    {
        status = false;

        pvect = FindVectorInCollection( pcollMain, tagXDFrequencyResponse );
        if( pvect )
        {
            //  Check size of vector
            pvect->GetCount( count );
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_REAL8 )
            {
                //  Copy them over (pvect --> triggerShapeParam)
                xdFrequencyResponse.SetCount( count );
                for( idx = 0; idx < count; idx++ )
                {
                    pvect->GetValueREAL8( idx, valueREAL8 );
                    xdFrequencyResponse.SetValueREAL8( idx, valueREAL8 );
                }
                status = true;

            }   //  Got the right physical type and array size
        }   //  Found vector
    }

    return status;
}


bool CPQDIF_R_Settings::GetChanCal
        (
        long                idxChannel,
        REAL8&              calTimeSkew,  
        REAL8&              calOffset,  
        REAL8&              calRatio,  
        bool&               calMustUseARCal,
        CPQDIF_E_Vector&    calApplied,     //  Array [n]
        CPQDIF_E_Vector&    calRecorded     //  Array [n]
        )
{
    bool    status = false;
    bool    foundItem;

    CPQDIF_E_Collection *   pcollMain;
    CPQDIF_E_Vector *       pvect;
    PQDIFValue              value;

    long        count;
    long        idx;
    double      valueREAL8;

    //  Find the appropriate channel collection
    pcollMain = GetOneChannelSetting( idxChannel );
    if( pcollMain )
    {
        status = true;
    }

    //  Find calTimeSkew (required)
    if( status )
    {
        status = false; 
        foundItem = GetScalarValueInCollection( pcollMain, tagCalTimeSkew, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            calTimeSkew = value.real8;
            status = true;
        }
    }

    //  Find calOffset (required)
    if( status )
    {
        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagCalOffset, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            calOffset = value.real8;
            status = true;
        }
    }

    //  Find calRatio (required)
    if( status )
    {
        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagCalRatio, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
        {
            calRatio = value.real8;
            status = true;
        }
    }

    //  Find calMustUseARCal (required)
    if( status )
    {
        status = false;
        foundItem = GetScalarValueInCollection( pcollMain, tagCalMustUseARCal, ID_PHYS_TYPE_BOOLEAN4, value );
        if( foundItem )
        {
            calMustUseARCal = value.bool4 ? true : false;
            status = true;
        }
    }

    //  Find calApplied (required)
    if( status )
    {
        status = false;

        pvect = FindVectorInCollection( pcollMain, tagCalApplied );
        if( pvect )
        {
            //  Check size of vector
            pvect->GetCount( count );
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_REAL8 )
            {
                //  Copy them over (pvect --> triggerShapeParam)
                calApplied.SetCount( count );
                for( idx = 0; idx < count; idx++ )
                {
                    pvect->GetValueREAL8( idx, valueREAL8 );
                    calApplied.SetValueREAL8( idx, valueREAL8 );
                }
                status = true;

            }   //  Got the right physical type and array size
        }   //  Found vector
    }

    //  Find calRecorded (required)
    if( status )
    {
        status = false;

        pvect = FindVectorInCollection( pcollMain, tagCalRecorded );
        if( pvect )
        {
            //  Check size of vector
            pvect->GetCount( count );
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_REAL8 )
            {
                //  Copy them over (pvect --> triggerShapeParam)
                calRecorded.SetCount( count );
                for( idx = 0; idx < count; idx++ )
                {
                    pvect->GetValueREAL8( idx, valueREAL8 );
                    calRecorded.SetValueREAL8( idx, valueREAL8 );
                }
                status = true;

            }   //  Got the right physical type and array size
        }   //  Found vector
    }

    return status;
}


//  Set functions
//  -------------
bool CPQDIF_R_Settings::SetConnectionInfo
        (
        const   UINT4 connectionType
        )
{
    bool        status = true;

    CPQDIF_E_Scalar *       psc;
    PQDIFValue              value;

    //  Set the connection type
    if( status )
    {
        status = false;
        psc = FindOrCreateScalarInCollection( m_pcollMain,
            tagSettingPhysicalConnection, ID_PHYS_TYPE_UNS_INTEGER4 );
        //  Set value
        if( psc )
        {
            value.uint4 = connectionType;
            status = psc->SetValue( ID_PHYS_TYPE_UNS_INTEGER4, value );
        }
    }
	return status;
}


bool CPQDIF_R_Settings::SetInfo
        (
        const   TIMESTAMPPQDIF&     timeEffective,
        const   TIMESTAMPPQDIF&     timeInstalled,
        const   TIMESTAMPPQDIF&     timeRemoved,  
                bool                useCal,       
                bool                useTrans      
        )
{
    bool        status = true;

    CPQDIF_E_Scalar *       psc;
    PQDIFValue              value;
    CPQDIF_E_Collection *   pcolInstances;

    //  Set times ... effective
    if( status )
    {
        status = false;
        psc = FindOrCreateScalarInCollection( m_pcollMain,
            tagEffective, ID_PHYS_TYPE_TIMESTAMPPQDIF );
        //  Set value
        if( psc )
        {
            value.ts = timeEffective;
            status = psc->SetValue( ID_PHYS_TYPE_TIMESTAMPPQDIF, value );
        }
    }
    //  ... installed
    if( status )
    {
        status = false;
        psc = FindOrCreateScalarInCollection( m_pcollMain,
            tagTimeInstalled, ID_PHYS_TYPE_TIMESTAMPPQDIF );
        //  Set value
        if( psc )
        {
            value.ts = timeInstalled;
            status = psc->SetValue( ID_PHYS_TYPE_TIMESTAMPPQDIF, value );
        }
    }
    //  ... removed
    if( status )
    {
        status = false;
        psc = FindOrCreateScalarInCollection( m_pcollMain,
            tagTimeRemoved, ID_PHYS_TYPE_TIMESTAMPPQDIF );
        //  Set value
        if( psc )
        {
            value.ts = timeRemoved;
            status = psc->SetValue( ID_PHYS_TYPE_TIMESTAMPPQDIF, value );
        }
    }

    //  Flags ... useCal
    if( status )
    {
        status = false;
        psc = FindOrCreateScalarInCollection( m_pcollMain,
            tagUseCalibration, ID_PHYS_TYPE_BOOLEAN4 );
        //  Set value
        if( psc )
        {
            value.bool4 = useCal;
            status = psc->SetValue( ID_PHYS_TYPE_BOOLEAN4, value );
        }
    }
    //  ... useTrans
    if( status )
    {
        status = false;
        psc = FindOrCreateScalarInCollection( m_pcollMain,
            tagUseTransducer, ID_PHYS_TYPE_BOOLEAN4 );
        //  Set value
        if( psc )
        {
            value.bool4 = useTrans;
            status = psc->SetValue( ID_PHYS_TYPE_BOOLEAN4, value );
        }
    }

    //  Is there a collection?
    pcolInstances = GetChannelSettings(); 
    if( !pcolInstances )
    {
        //  Nope, insert an empty collection ...
        pcolInstances = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcolInstances->SetTag( tagChannelSettingsArray );
        m_pcollMain->Add( pcolInstances );
    }

    return status;
}

long CPQDIF_R_Settings::AddChannel
        (
        UINT4               idxChannelDefn
        ) 
    {
    long        idxNew = -1;
    //
    //
    //  Is there a collection?
    //
    CPQDIF_E_Collection *   pcolInstances = GetChannelSettings(); 
    if( !pcolInstances )
        {
        //  Nope, insert an empty collection ...
        pcolInstances = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcolInstances->SetTag( tagChannelSettingsArray );
        m_pcollMain->Add( pcolInstances );
        }


    if( pcolInstances )
        {
        //  Get the new index
        idxNew = GetCountChannels();

        //  Create the new channel instance
        CPQDIF_E_Collection *   pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcollOne->SetTag( tagOneChannelSetting );

        //  Stuff in the information...
        pcollOne->SetScalarUINT4( tagChannelDefnIdx, idxChannelDefn );

        //  We're done filling the collection ... add it!
        pcolInstances->Add( pcollOne );
        }

    return idxNew;
    }

long CPQDIF_R_Settings::AddChannel
        (
        UINT4               idxChannelDefn,
        UINT4               idTriggerType
        ) 
    {
    long        idxNew = -1;
    //
    //
    //  Is there a collection?
    //
    CPQDIF_E_Collection *   pcolInstances = GetChannelSettings(); 
    if( !pcolInstances )
        {
        //  Nope, insert an empty collection ...
        pcolInstances = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcolInstances->SetTag( tagChannelSettingsArray );
        m_pcollMain->Add( pcolInstances );
        }


    if( pcolInstances )
        {
        //  Get the new index
        idxNew = GetCountChannels();

        //  Create the new channel instance
        CPQDIF_E_Collection *   pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcollOne->SetTag( tagOneChannelSetting );

        //  Stuff in the information...
        pcollOne->SetScalarUINT4( tagChannelDefnIdx, idxChannelDefn );
        pcollOne->SetScalarUINT4( tagTriggerTypeID, idTriggerType );

        //  We're done filling the collection ... add it!
        pcolInstances->Add( pcollOne );
        }

    return idxNew;
    }



#ifdef zap
//
//
//  This should only be used if all passed in parameters are used
//
long CPQDIF_R_Settings::AddChannel
        (
                UINT4               idxChannelDefn,
                UINT4               triggerTypeID,
                REAL8               fullScale,  
                REAL8               noiseFloor, 
                REAL8               triggerLow, 
                REAL8               triggerHigh,
                REAL8               triggerRate,
                //REAL8 *         triggerShapeParam   //  Array of [3] ... or NULL
        const   CPQDIF_E_Vector *   triggerShapeParam   //  Array of [3] ... or NULL
        ) 
    {
    long        idxNew = -1;
    //
    //
    //  Is there a collection?
    //
    CPQDIF_E_Collection *   pcolInstances = GetChannelSettings(); 
    if( !pcolInstances )
        {
        //  Nope, insert an empty collection ...
        pcolInstances = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcolInstances->SetTag( tagChannelSettingsArray );
        m_pcollMain->Add( pcolInstances );
        }

    CPQDIF_E_Collection *   pcollOne;

    CPQDIF_E_Vector *       pvect;

    long                    idx;
    long                    count;
    double                  valueParam;
    const long              countParams = 3;

    if( pcolInstances )
        {
        //  Get the new index
        idxNew = GetCountChannels();

        //  Create the new channel instance
        pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcollOne->SetTag( tagOneChannelSetting );

        //  Stuff in the information...
        pcollOne->SetScalarUINT4( tagChannelDefnIdx, idxChannelDefn );
        pcollOne->SetScalarUINT4( tagTriggerTypeID, triggerTypeID );

        //  Thresholds
        pcollOne->SetScalarREAL8( tagFullScale, fullScale );
        pcollOne->SetScalarREAL8( tagNoiseFloor, noiseFloor );
        pcollOne->SetScalarREAL8( tagTriggerLow, triggerLow );
        pcollOne->SetScalarREAL8( tagTriggerHigh, triggerHigh );
        pcollOne->SetScalarREAL8( tagTriggerRate, triggerRate );

        //  Shape param
        pvect = FindOrCreateVectorInCollection( pcollOne, 
                    tagTriggerShapeParam, ID_PHYS_TYPE_REAL8 );
        //  Set its value(s)
        if( pvect && triggerShapeParam )
            {
            //  Validate physical type & size
            pvect->GetCount( count );
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_REAL8 
                && count == countParams )
                {
                pvect->SetCount( countParams );
                for( idx = 0; idx < countParams; idx++ )
                    {
                    triggerShapeParam->GetValueREAL8( idx, valueParam );
                    pvect->SetValueREAL8( idx, valueParam );
                    }
                }
            }

        //  We're done filling the collection ... add it!
        pcolInstances->Add( pcollOne );
        }

    return idxNew;
    }
#endif

bool CPQDIF_R_Settings::SetTriggerShapeParam
        (
        UINT4                     idxChannel,
        const CPQDIF_E_Vector *   triggerShapeParam   //  Array of [3] ... or NULL
        ) 
    {
    bool    status = false;

    CPQDIF_E_Collection *   pcollOne = GetOneChannelSetting( idxChannel );

    if( pcollOne )
        {
        CPQDIF_E_Vector *  pvect = FindOrCreateVectorInCollection( pcollOne, 
                    tagTriggerShapeParam, ID_PHYS_TYPE_REAL8 );
        //
        //  Set its value(s)
        //
        if( pvect && triggerShapeParam )
            {
            const long              countParams = 3;
            long                    count;
            //
            //
            //  Validate physical type & size
            //
            pvect->GetCount( count );
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_REAL8 
                && count == countParams )
                {
                double              valueParam;

                pvect->SetCount( countParams );

                for(long idx = 0; idx < countParams; idx++ )
                    {
                    triggerShapeParam->GetValueREAL8( idx, valueParam );
                    pvect->SetValueREAL8( idx, valueParam );
                    }
                status = true;
                }
            }
        }
    return status;
    }


bool CPQDIF_R_Settings::GetChannelReal8(UINT4 idxChannel, GUID tag, REAL8 &dVal)
    {
    bool foundItem = false;

    CPQDIF_E_Collection *   pcollOne = GetOneChannelSetting( idxChannel );

    if( pcollOne )
        {
        PQDIFValue  value;

        foundItem = GetScalarValueInCollection( pcollOne, tag, ID_PHYS_TYPE_REAL8, value );
        if( foundItem )
            {
            dVal = value.real8;
            }
        }
    return foundItem;
    }

bool CPQDIF_R_Settings::SetChannelReal8(UINT4 idxChannel, GUID tag, REAL8 dVal)
    {
    bool    status = false;

    CPQDIF_E_Collection *   pcollOne = GetOneChannelSetting( idxChannel );

    if( pcollOne )
        {
        CPQDIF_E_Scalar * psc = FindOrCreateScalarInCollection( pcollOne,
            tag, ID_PHYS_TYPE_REAL8 );

        if( psc )
            {
            psc->SetValueREAL8(dVal);

            status = true;
            }
        }
    return status;
    }


bool CPQDIF_R_Settings::SetChanTrans
        (
                UINT4               idxChannel,
                UINT4               xdTransformerTypeID,
                REAL8               xdSystemSideRatio,  
                REAL8               xdMonitorSideRatio,
        const   CPQDIF_E_Vector *   xdFrequencyResponse   //  Array [n]... or NULL
        )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcollOne = GetOneChannelSetting( idxChannel );
    CPQDIF_E_Scalar *       psc;
    CPQDIF_E_Vector *       pvect;

    long        idx;
    long        count;
    double      value;

    if( pcollOne )
    {
        //  Scalars
        psc = FindOrCreateScalarInCollection( pcollOne,
            tagXDTransformerTypeID, ID_PHYS_TYPE_UNS_INTEGER4 );
        if( psc )
        {
            psc->SetValueUINT4( xdTransformerTypeID );
        }
        psc = FindOrCreateScalarInCollection( pcollOne,
            tagXDSystemSideRatio, ID_PHYS_TYPE_REAL8 );
        if( psc )
        {
            psc->SetValueREAL8( xdSystemSideRatio );
        }
        psc = FindOrCreateScalarInCollection( pcollOne,
            tagXDMonitorSideRatio, ID_PHYS_TYPE_REAL8 );
        if( psc )
        {
            psc->SetValueREAL8( xdMonitorSideRatio );
        }

        //  Vector
        if( xdFrequencyResponse )
        {
            pvect = FindOrCreateVectorInCollection( pcollOne, 
                    tagXDFrequencyResponse, ID_PHYS_TYPE_REAL8 );
            if( pvect && xdFrequencyResponse->GetPhysicalType() == ID_PHYS_TYPE_REAL8 )
            {
                //  Transfer the data over
                xdFrequencyResponse->GetCount( count );
                pvect->SetCount( count );
                for( idx = 0; idx < count; idx++)
                {
                    xdFrequencyResponse->GetValueREAL8( idx, value );
                    pvect->SetValueREAL8( idx, value );
                }
            }
        }

        status = true;
    }

    return status;
}


bool CPQDIF_R_Settings::SetChanCal
        (
                UINT4               idxChannel,
                REAL8               calTimeSkew,  
                REAL8               calOffset,  
                REAL8               calRatio,  
                bool                calMustUseARCal,
        const   CPQDIF_E_Vector *   calApplied,     //  Array [n]... or NULL
        const   CPQDIF_E_Vector *   calRecorded     //  Array [n]... or NULL
        )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcollOne = GetOneChannelSetting( idxChannel );
    CPQDIF_E_Scalar *       psc;
    CPQDIF_E_Vector *       pvect;

    long        idx;
    long        count;
    double      value;

    if( pcollOne )
    {
        //  Scalars
        psc = FindOrCreateScalarInCollection( pcollOne,
                tagCalTimeSkew, ID_PHYS_TYPE_REAL8 );
        if( psc )
        {
            psc->SetValueREAL8( calTimeSkew );
        }
        psc = FindOrCreateScalarInCollection( pcollOne,
                tagCalOffset, ID_PHYS_TYPE_REAL8 );
        if( psc )
        {
            psc->SetValueREAL8( calOffset );
        }
        psc = FindOrCreateScalarInCollection( pcollOne,
                tagCalRatio, ID_PHYS_TYPE_REAL8 );
        if( psc )
        {
            psc->SetValueREAL8( calRatio );
        }
        psc = FindOrCreateScalarInCollection( pcollOne,
                tagCalMustUseARCal, ID_PHYS_TYPE_BOOLEAN4 );
        if( psc )
        {
            psc->SetValueBOOL4( (BOOL4) calMustUseARCal );
        }

        //  Vectors
        if( calApplied && calRecorded )
        {
            //  Applied
            pvect = FindOrCreateVectorInCollection( pcollOne, 
                        tagCalApplied, ID_PHYS_TYPE_REAL8 );
            if( pvect && calApplied->GetPhysicalType() == ID_PHYS_TYPE_REAL8 )
            {
                //  Transfer the data over
                calApplied->GetCount( count );
                pvect->SetCount( count );
                for( idx = 0; idx < count; idx++)
                {
                    calApplied->GetValueREAL8( idx, value );
                    pvect->SetValueREAL8( idx, value );
                }
            }

            //  Recorded
            pvect = FindOrCreateVectorInCollection( pcollOne, 
                        tagCalRecorded, ID_PHYS_TYPE_REAL8 );
            if( pvect && calRecorded->GetPhysicalType() == ID_PHYS_TYPE_REAL8 )
            {
                //  Transfer the data over
                calRecorded->GetCount( count );
                pvect->SetCount( count );
                for( idx = 0; idx < count; idx++)
                {
                    calRecorded->GetValueREAL8( idx, value );
                    pvect->SetValueREAL8( idx, value );
                }
            }
        }

        status = true;
    }

    return status;
}

    
//  Protected stuff

CPQDIF_E_Collection * CPQDIF_R_Settings::GetChannelSettings( void )
{
    CPQDIF_E_Collection *   pcollSettings;

    //  Create the collection if it does not yet exist.
    pcollSettings = FindCollectionInCollection( m_pcollMain, tagChannelSettingsArray );
    if( !pcollSettings )
    {
        pcollSettings = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollSettings )
        {
            pcollSettings->SetTag( tagChannelSettingsArray );
            m_pcollMain->Add( pcollSettings );
        }
    }

    return pcollSettings;
}


CPQDIF_E_Collection * CPQDIF_R_Settings::GetOneChannelSetting
        ( 
        long idxChannel
        )
{
    CPQDIF_E_Collection *   pcolReturn = NULL;
    CPQDIF_E_Collection *   pcolInstances = GetChannelSettings();
    CPQDIF_Element *        pel;

    if( pcolInstances )
    {
        pel = pcolInstances->GetElement( idxChannel );
        if( pel )
        {
            if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION
                && PQDIF_IsEqualGUID( pel->GetTag(), tagOneChannelSetting ) )
            {
                pcolReturn = (CPQDIF_E_Collection *) pel;
            }
        }
    }

    return pcolReturn;
}
