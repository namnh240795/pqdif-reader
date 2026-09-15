/*
**  Class:          CPQDIF_R_Observation
**  Description:    Implements a PQDIF record "wrapper" for the data source record. You can NOT cast a standard record object to this class, because it has its own member data.
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


//  Local functions
//  ===============

// Function to round second values to the nearest nanosecond.
inline double _FixSeconds(double dTime)
{
	return floor( dTime * 1000000000.0 + 0.5 )/ 1000000000.0;
}

static void _AddSeconds( TIMESTAMPPQDIF &dt, double secondsToAdd )
{
    
    // Record the current day for an overflow check.
    DWORD dwDay = dt.day;

    // Perform the requested operation. 	
	dt.sec += secondsToAdd;

    // If the result is greater than the number of seconds in
    // a day then ...
	if( dt.sec > (double) SECONDS_PER_DAY )
	{

        // Carry resulting number of days.
		long lDays = (long)( dt.sec / (double) SECONDS_PER_DAY );
        dt.day += lDays;
		dt.sec -= ( lDays * (double) SECONDS_PER_DAY );


        // Check for an overflow.
        if( dt.day < dwDay )
        {
            dt.day = 0xffffffff;
            dt.sec = 86399.999999;
        }

    }

    // Else if the result is less than 0.0.
    else if( dt.sec < 0.0 )
    {

        // Borrow the required number of days.
        long lDays = 1 - (long)( dt.sec / (double) SECONDS_PER_DAY );
        dt.day -= lDays;
		dt.sec += ( lDays * (double) SECONDS_PER_DAY );   

        // Check for an underflow.
        if( dt.day > dwDay )
        {
            dt.day = 0;
            dt.sec = 0.0;
        }

	}


    // Adjust the result.
	dt.sec = _FixSeconds( dt.sec );

}


//  Construction
//  ============

CPQDIF_R_Observation::CPQDIF_R_Observation()
{
    m_pds = NULL;
    m_psett = NULL;
    m_record = NULL;
}

CPQDIF_R_Observation::CPQDIF_R_Observation( CPQDIFRecord& record )
{
    m_pds = NULL;
    m_psett = NULL;
    m_record = (CPQDIF_R_General *) &record;
}


CPQDIF_R_Observation::~CPQDIF_R_Observation()
{
    //  Don't destroy the underlying records; these are only by reference, not by value.
}


bool CPQDIF_R_Observation::GetInfo
            (
            TIMESTAMPPQDIF&     timeStart,
            TIMESTAMPPQDIF&     timeCreate,
            string&             name
            )
{
    bool    status;

    CPQDIF_E_Scalar *   psc;
    CPQDIF_E_Vector *   pvect;

    //  Init
    status = TRUE;
    memset( &timeStart, 0, sizeof( timeStart ) );
    memset( &timeCreate, 0, sizeof( timeCreate ) );
    name = "";

    //  First, find the times
    if( status )
    {
        psc = FindScalarInCollection( m_record->m_pcollMain, tagTimeStart );
        if( psc )
        {
            status = psc->GetValueTimeStamp( timeStart );
        }
    }

    if( status )
    {
        psc = FindScalarInCollection( m_record->m_pcollMain, tagTimeCreate );
        if( psc )
        {
            status = psc->GetValueTimeStamp( timeCreate );
        }
    }

    //  Get name
    if( status )
    {
        pvect = FindVectorInCollection( m_record->m_pcollMain, tagObservationName );
        if( pvect )
        {
            status = pvect->GetValues( name );
        }
    }

    return status;
}

            
long CPQDIF_R_Observation::GetCountChannels( void )
{
    long                    count = 0;
    CPQDIF_E_Collection *   pcolCI;

    pcolCI = GetChannelInstances();
    if( pcolCI )
    {
        count = pcolCI->GetCount(); 
    }

    return count;
}


bool CPQDIF_R_Observation::GetTriggerInfo
            (
            UINT4&              idTriggerMethod,
            CPQDIF_E_Vector **  pvectTriggerChanIdx,
            TIMESTAMPPQDIF&     timeTriggered
            )
{
    bool    status = false;

    CPQDIF_E_Scalar *   psc;
    CPQDIF_E_Vector *   pvect;

    //  Init
    idTriggerMethod = ID_TRIGGER_METH_NONE;
    ASSERT( pvectTriggerChanIdx );
    *pvectTriggerChanIdx = NULL;
    memset( &timeTriggered, 0, sizeof( timeTriggered ) );

    //  First, find the trigger method
    psc = FindScalarInCollection( m_record->m_pcollMain, tagTriggerMethodID );
    if( psc )
    {
        status = psc->GetValueUINT4( idTriggerMethod );
    }
    else
    {
        //  Default to channel? NAHHH
        //status = TRUE;
        //idTriggerMethod = ID_TRIGGER_METH_CHANNEL;
    }

    //  Get triggered channels? (NOT required; leave status alone)
    if( status && idTriggerMethod == ID_TRIGGER_METH_CHANNEL )
    {
        pvect = FindVectorInCollection( m_record->m_pcollMain, tagChannelTriggerIdx );
        if( pvect )
        {
            //  Validate physical type
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_UNS_INTEGER4 )
            {
                //  Return a pointer to the vector
                *pvectTriggerChanIdx = pvect;
            }
        }
    }

    //  Get the time it was triggered
    if( status )
    {
        status = false;
        psc = FindScalarInCollection( m_record->m_pcollMain, tagTimeTriggered );
        if( psc )
        {
            status = psc->GetValueTimeStamp( timeTriggered );
        }
    }

    return status;
}


long CPQDIF_R_Observation::GetCountSeries( int idxChannel )
{
    long                    count = 0;
    CPQDIF_E_Collection *   pcolOneChannel;
    CPQDIF_E_Collection *   pcolSI;

    pcolOneChannel = GetOneChannel( idxChannel );
    if( pcolOneChannel )
    {
        pcolSI = GetSeriesInstances( pcolOneChannel );
        if( pcolSI )
        {
            count = pcolSI->GetCount(); 
        }
    }

    return count;
}


bool CPQDIF_R_Observation::GetChannelInfo
            (
            long        idxChannel,
            string&     name,
            UINT4&      idPhase,
            GUID&       idQuantityType,
            UINT4&      idQuantityMeasured
            )
{
    bool        status = true;
    long        idxChannelDefn = 0;

    //  Find the channel defn index
    if( status )
    {
        status = GetChannelDefnIdx( idxChannel, idxChannelDefn );
    }

    //  Gather information from the channel definitions
    if( status && m_pds )
    {
        status = m_pds->GetChannelDefnInfo( idxChannelDefn, name, idPhase, 
            idQuantityType, idQuantityMeasured );
    }
    else
    {
        status = false;
    }

    return status;
}

bool CPQDIF_R_Observation::GetChannelPrimarySeries
            (
            long        idxChannel,
            long&       idxPrimarySeries
            )
{
    bool        status = true;
    long        idxChannelDefn = 0;

    //  Find the channel defn index
    if( status )
    {
        status = GetChannelDefnIdx( idxChannel, idxChannelDefn );
    }

    //  Gather information from the channel definitions
    if( status && m_pds )
    {
        status = m_pds->GetChannelPrimarySeries( idxChannelDefn, idxPrimarySeries );
    }
    else
    {
        status = false;
    }

    return status;
}

#ifdef zap
//
//
//  Depricated function
//
bool CPQDIF_R_Observation::GetChannelThresholds
            (
            long                idxChannel,
            UINT4&              triggerTypeID,
            REAL8&              fullScale,  
            REAL8&              noiseFloor, 
            REAL8&              triggerLow, 
            REAL8&              triggerHigh,
            REAL8&              triggerRate,
            CPQDIF_E_Vector&    triggerShapeParam   //  Array of [3]
            )
{
    bool        status = false;
    long        idxChannelDefn;

    bool        found;
    long        idxChannSett;
    long        countChannSett;
    UINT4       idxChannelDefnSett;

    //  DO we have settings?
    if( m_psett )
    {
        //  Determine which channel def'n is used for this channel
        //  Find the channel defn index
        status = GetChannelDefnIdx( idxChannel, idxChannelDefn );

        //  Check all settings channels to see which matches the def'n
        if( status )
        {
            //  Init
            status = false;

            countChannSett = m_psett->GetCountChannels();
            for( idxChannSett = 0; idxChannSett < countChannSett; idxChannSett++ )
            {
                found = m_psett->GetChannelInfo( idxChannSett, idxChannelDefnSett,
                            triggerTypeID, fullScale,  noiseFloor, 
                            triggerLow, triggerHigh, triggerRate,
                            triggerShapeParam );

                //  Is this the one that corresponds to the same def'n?
                if( found && idxChannelDefn == (long) idxChannelDefnSett )
                {
                    status = TRUE;
                    break;
                }
            }   //  For( channel setting )

        }   //  Found channel def'n        
    }   //  Have settings

    return status;
}
#endif

bool CPQDIF_R_Observation::GetSeriesInfo
            (
            long        idxChannel,
            long        idxSeries,
            UINT4&      idQuantityUnits,
            GUID&       idQuantityCharacteristic,
            GUID&       idValueType
            )
{
    bool        status = true;
    long        idxChannelDefn = 0;
    UINT4       idStorageMethod;

    //  Find the channel defn index
    if( status )
    {
        status = GetChannelDefnIdx( idxChannel, idxChannelDefn );
    }

    //  Gather information from the series definition
    if( status && m_pds )
    {
        status = m_pds->GetSeriesDefnInfo( idxChannelDefn, idxSeries,
                    idQuantityUnits, idValueType, idQuantityCharacteristic,
                    idStorageMethod );
    }
    else
    {
        status = false;
    }

    return status;
}


long CPQDIF_R_Observation::AddChannel
            (
            long        idxChannelDefn
            )
{
    long        idxNew = -1;

    CPQDIF_E_Collection *   pcolInstances = GetChannelInstances();
    CPQDIF_E_Collection *   pcollOne;
    CPQDIF_E_Collection *   pcollSeriesInstances;

    if( pcolInstances )
    {
        //  Get the new index
        idxNew = GetCountChannels();

        //  Create the new channel instance
        pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        pcollOne->SetTag( tagOneChannelInst );

        //  Stuff in the information...
        pcollOne->SetScalarUINT4( tagChannelDefnIdx, idxChannelDefn );

        //  Also need a series instances collection
        pcollSeriesInstances = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollSeriesInstances )
        {
            pcollSeriesInstances->SetTag( tagSeriesInstances );
            pcollOne->Add( pcollSeriesInstances );
        }

        //  Add it!
        pcolInstances->Add( pcollOne );
    }

    return idxNew;
}


long CPQDIF_R_Observation::AddSeriesDouble
            (
            long        idxChannel,
            long        countValues,
            double *    arValues
            )
{
    long        idxNew = -1;

    CPQDIF_E_Vector *       pvectData;
    
    long        idx;
    double *    pValue;

    //  Create vector for the series data
    pvectData = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
    if( pvectData )
    {
        pvectData->SetTag( tagSeriesValues );
        pvectData->SetPhysicalType( ID_PHYS_TYPE_REAL8 );

        //  Copy over series data
        pValue = arValues;
        pvectData->SetCount( countValues );
        for( idx = 0; idx < countValues; idx++, pValue++ )
        {
            pvectData->SetValue( idx, *pValue );
        }

        //  Add it ...
        idxNew = AddSeriesVector( idxChannel, pvectData );

        //  If it was not successful, delete the vector
        if( idxNew < 0 )
        {
            delete pvectData;
        }
    }

    return idxNew;
}


long CPQDIF_R_Observation::AddSeriesVector
            (
            long                idxChannel,
            CPQDIF_E_Vector *   pvectData
            )
{
    long        idxNew = -1;

    CPQDIF_E_Collection *   pcolOneChannel;
    CPQDIF_E_Collection *   pcolSI;
    CPQDIF_E_Collection *   pcollOne;
    
    pcolOneChannel = GetOneChannel( idxChannel );
    if( pcolOneChannel && pvectData )
    {
        pcolSI = GetSeriesInstances( pcolOneChannel );
        if( pcolSI )
        {
            //  New channel index
            idxNew = pcolSI->GetCount(); 

            //  Create the new channel def'n
            pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            if( pcollOne )
            {
                //  Set the tag for this collection ...
                pcollOne->SetTag( tagOneSeriesInstance );

                //  Make sure the vector has the right tag ...
                pvectData->SetTag( tagSeriesValues );

                //  The vector is currently the only member of this collection.
                pcollOne->Add( pvectData );
            }

            //  Add it!
            pcolSI->Add( pcollOne );
        }
    }

    return idxNew;
}

long CPQDIF_R_Observation::AddSeriesShared
            (
            long            idxChannel,
            long            idxChannelShared,
            long            idxSeriesShared
            )
    {
    long        idxNew = -1;

    CPQDIF_E_Collection *   pcolOneChannel;
    CPQDIF_E_Collection *   pcolSI;
    CPQDIF_E_Collection *   pcollOne;

    CPQDIF_E_Scalar * pscSharedChannel;
    CPQDIF_E_Scalar * pscSharedSeries;

    pcolOneChannel = GetOneChannel( idxChannel );
    if( pcolOneChannel )
        {
        pcolSI = GetSeriesInstances( pcolOneChannel );
        if( pcolSI )
            {
            //  New channel index
            idxNew = pcolSI->GetCount(); 

            //  Create the new channel def'n
            pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            if( pcollOne )
                {

                //  Set the tag for this collection ...
                pcollOne->SetTag( tagOneSeriesInstance );

                //  Create scalars which indicate which channel/series is to be shared
                pscSharedChannel = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
                pscSharedSeries  = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
                if( pscSharedChannel && pscSharedSeries )
                    {
                    //  Tags & values
                    pscSharedChannel->SetTag( tagSeriesShareChannelIdx );
                    pscSharedChannel->SetPhysicalType( ID_PHYS_TYPE_UNS_INTEGER4 );
                    pscSharedChannel->SetValueUINT4( idxChannelShared );
                    pscSharedSeries->SetTag( tagSeriesShareSeriesIdx );
                    pscSharedSeries->SetPhysicalType( ID_PHYS_TYPE_UNS_INTEGER4 );
                    pscSharedSeries->SetValueUINT4( idxSeriesShared );

                    //  Stuff 'em
                    pcollOne->Add( pscSharedChannel );
                    pcollOne->Add( pscSharedSeries );
                    }
                }

            //  Add it!
            pcolSI->Add( pcollOne );
            }
        }

    return idxNew;
    }

bool CPQDIF_R_Observation::SetSeriesBaseQuantity
            (
            long        idxChannel,
            long        idxSeries,
            double      value
            )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcol;
    CPQDIF_E_Scalar *       psc;

    //  Find the series
    pcol = GetOneSeries( idxChannel, idxSeries );
    if( pcol )
    {
        psc = FindScalarInCollection( pcol, tagSeriesBaseQuantity );
        if( !psc )
        {
            //  Create it !
            psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
            if( psc )
            {
                pcol->Add( psc );
            }
        }

        //  Init the scalar
        if( psc )
        {
            psc->SetTag( tagSeriesBaseQuantity );
            psc->SetValueREAL8( value );
            status = TRUE;
        }
    }

    return status;
}

bool CPQDIF_R_Observation::SetSeriesScale
            (
            long        idxChannel,
            long        idxSeries,
            double      scale,
            double      offset
            )
    {
    bool    status = false;

    CPQDIF_E_Collection *   pcol;
    CPQDIF_E_Scalar *       psc;
#ifdef zap
    //  Find the series
    pcol = GetOneSeries( idxChannel, idxSeries );
    if( pcol )
        {
        psc = FindOrCreateScalarInCollection( pcol, tagSeriesScale, ID_PHYS_TYPE_REAL8  );
        if( psc )
            {
            status = psc->SetValueREAL8( scale );
            }
        psc = FindOrCreateScalarInCollection( pcol, tagSeriesOffset, ID_PHYS_TYPE_REAL8 );
        if( psc )
            {
            status = psc->SetValueREAL8( offset );
            }
        }
#endif
    //  Find the series
    pcol = GetOneSeries( idxChannel, idxSeries );
    if( pcol )
        {
        psc = FindScalarInCollection( pcol, tagSeriesScale );
        if( !psc )
            {
            //  Create it !
            psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
            if( psc )
                {
	            psc->SetTag( tagSeriesScale );
                pcol->Add( psc );
                }
            }

        //  Init the scalar
        if( psc )
            {
            psc->SetValueREAL8( scale );
            status = TRUE;
            }


        psc = FindScalarInCollection( pcol, tagSeriesOffset );
        if( !psc )
            {
            //  Create it !
            psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
            if( psc )
                {
	            psc->SetTag( tagSeriesOffset );
                pcol->Add( psc );
                }
            }

        //  Init the scalar
        if( psc )
            {
            psc->SetValueREAL8( offset );
            status = TRUE;
            }
        }

    return status;
    }

bool CPQDIF_R_Observation::GetSeriesBaseQuantity
            (
            long        idxChannel,
            long        idxSeries,
            double&     value
            )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcol;
    CPQDIF_E_Scalar *       psc;

    //  Find the series
    pcol = GetOneSeries( idxChannel, idxSeries );
    if( pcol )
    {
        psc = FindScalarInCollection( pcol, tagSeriesBaseQuantity );
        if( psc )
        {
            status = psc->GetValueREAL8( value );
        }
    }

    return status;
}

#ifdef PQDIF_USE_COM

bool CPQDIF_R_Observation::GetObservationExtendedData
            (
			GUID& gidTag,
            VARIANT&  value
            )
{
    bool    status = false;

    CPQDIF_E_Scalar *       psc;

    psc = FindScalarInCollection( m_record->m_pcollMain, gidTag );
    if( psc )
    {
	    status = psc->GetValue( value );
    }

    return status;
}

bool CPQDIF_R_Observation::GetSeriesExtendedData
            (
            long        idxChannel,
			long		idxSeries,
			GUID& gidTag,
            VARIANT&  value
            )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcollOneChannel;
    CPQDIF_E_Scalar *       psc;

    pcollOneChannel = GetOneSeries( idxChannel, idxSeries );

	if (pcollOneChannel)
	{
		psc = FindScalarInCollection( pcollOneChannel, gidTag );
		if( psc )
		{
			status = psc->GetValue( value );
		}
	}
    return status;
}

bool CPQDIF_R_Observation::GetChannelExtendedData
            (
            long        idxChannel,
			GUID& gidTag,
            VARIANT&  value
            )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcollOneChannel;
    CPQDIF_E_Scalar *       psc;

    pcollOneChannel = GetOneChannel( idxChannel );

	if (pcollOneChannel)
	{
		psc = FindScalarInCollection( pcollOneChannel, gidTag );
		if( psc )
		{
			status = psc->GetValue( value );
		}
	}
    return status;
}

#endif

bool CPQDIF_R_Observation::GetObservationExtendedData
            (
			GUID& gidTag,
            long& typePhysical,
			PQDIFValue& value
            )
{
    bool    status = false;

    CPQDIF_E_Scalar *       psc;

    psc = FindScalarInCollection( m_record->m_pcollMain, gidTag );
    if( psc )
    {
	    status = psc->GetValue( typePhysical, value );
    }

    return status;
}

bool CPQDIF_R_Observation::GetChannelExtendedData
            (
            long        idxChannel,
			GUID& gidTag,
            long& typePhysical,
			PQDIFValue& value
            )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcollOneChannel;
    CPQDIF_E_Scalar *       psc;

    pcollOneChannel = GetOneChannel( idxChannel );

	if (pcollOneChannel)
	{
		psc = FindScalarInCollection( pcollOneChannel, gidTag );
		if( psc )
		{
			status = psc->GetValue(typePhysical, value);
		}
	}
    return status;
}


bool CPQDIF_R_Observation::GetSeriesExtendedData
            (
            long        idxChannel,
			long		idxSeries,
			GUID& gidTag,
            long& typePhysical,
			PQDIFValue& value
            )
{
    bool    status = false;

    CPQDIF_E_Collection *   pcollOneChannel;
    CPQDIF_E_Scalar *       psc;

    pcollOneChannel = GetOneSeries( idxChannel, idxSeries );

	if (pcollOneChannel)
	{
		psc = FindScalarInCollection( pcollOneChannel, gidTag );
		if( psc )
		{
			status = psc->GetValue( typePhysical, value );
		}
	}
    return status;
}

bool CPQDIF_R_Observation::GetSeriesDefnNominal
            (
            long        idxChannel,
            long        idxSeries,
            double    & dNominal
            )
    {
    bool    status = false;
	long	idxChannelDefn;

    status = GetChannelDefnIdx( idxChannel, idxChannelDefn );

    if (m_pds)
        {
        status = m_pds->GetSeriesDefnNominal(idxChannelDefn, idxSeries, dNominal);
        }

    return status;
    }

bool CPQDIF_R_Observation::GetSeriesDefnPrecisionAndResolution
            (
            long        idxChannel,
            long        idxSeries,
            UINT4		& uPrecision,
            double		& dResolution
            )
    {
    bool    status = false;
	long	idxChannelDefn;

    status = GetChannelDefnIdx( idxChannel, idxChannelDefn );

    if (m_pds)
        {
        status = m_pds->GetSeriesDefnPrecisionAndResolution(idxChannelDefn, idxSeries, uPrecision, dResolution);
        }

    return status;
    }

bool CPQDIF_R_Observation::GetSeriesScale
            (
            long        idxChannel,
            long        idxSeries,
            double&     scale,
            double&     offset
            )
    {
    bool    status = false;

    CPQDIF_E_Collection *   pcol;
    CPQDIF_E_Scalar *       psc;

    //  Find the series
    pcol = GetOneSeries( idxChannel, idxSeries );
    if( pcol )
        {
        psc = FindScalarInCollection( pcol, tagSeriesScale );
        if( psc )
            {
            status = psc->GetValueREAL8( scale );
            }
        psc = FindScalarInCollection( pcol, tagSeriesOffset );
        if( psc )
            {
            status = psc->GetValueREAL8( offset );
            }
        }

    return status;
    }

CPQDIF_E_Vector * CPQDIF_R_Observation::GetSeriesValueVector
            (
            long        idxChannel,
            long        idxSeries
            )
    {
    CPQDIF_E_Vector *       pvectReturn = NULL;

    CPQDIF_E_Collection *   pcolOneSeries;

    pcolOneSeries = GetOneSeries( idxChannel, idxSeries );
    if( pcolOneSeries )
        {
        pvectReturn = FindVectorInCollection( pcolOneSeries, tagSeriesValues );
        }

    return pvectReturn;
    }

bool CPQDIF_R_Observation::SetTriggerInfo
            (
                    UINT4               idTriggerMethod,
                    long                countTriggers,
            const   UINT4 *             aidxTriggerChan,
            const   TIMESTAMPPQDIF&     timeTriggered
            )
{
    bool    status = false;

    CPQDIF_E_Scalar *   psc;
    CPQDIF_E_Vector *   pvect;

    //  Init
    ASSERT( aidxTriggerChan );

    //  First, find (or create) the trigger method
    psc = FindOrCreateScalarInCollection( m_record->m_pcollMain, 
            tagTriggerMethodID, ID_PHYS_TYPE_UNS_INTEGER4 );
    //  Set its value
    if( psc )
    {
        status = psc->SetValueUINT4( idTriggerMethod );
    }

    //  Get triggered channels?
    if( status && idTriggerMethod == ID_TRIGGER_METH_CHANNEL )
    {
        status = false;
        pvect = FindOrCreateVectorInCollection( m_record->m_pcollMain, 
                    tagChannelTriggerIdx, ID_PHYS_TYPE_UNS_INTEGER4 );
        //  Set its value(s)
        if( pvect )
        {
            //  Validate physical type
            if( pvect->GetPhysicalType() == ID_PHYS_TYPE_UNS_INTEGER4 )
            {
                pvect->SetCount( countTriggers );
                for( long idx = 0; idx < countTriggers; idx++ )
                {
                    pvect->SetValueUINT4( idx, aidxTriggerChan[ idx ] );
                }
                status = TRUE;
            }
        }
    }

    //  Set the time it was triggered
    if( status )
    {
        status = false;
        psc = FindOrCreateScalarInCollection( m_record->m_pcollMain,
            tagTimeTriggered, ID_PHYS_TYPE_TIMESTAMPPQDIF );
        //  Set value
        if( psc )
        {
            status = psc->SetValueTimeStamp( timeTriggered );
        }
    }

    return status;
}


long     CPQDIF_R_Observation::GetCountResolvedSeries
            (
            long    idxChannel,
            long    idxSeries,
			bool    noShare
            ) 
{
	int		idx;
    bool    status = false;
    long    countPoints = 0;
    long    idxChannelDefn = 0;
    UINT4   idQuantityUnits;
    GUID    idQuantityCharacteristic;
    GUID    idValueType;
    UINT4   idStorageMethod = 0;

	bool	gotShareChannel = false;
	bool	gotShareSeries = false;
	UINT4	idxShareChannelIdx = 0;
	UINT4	idxShareSeriesIdx = 0;

    CPQDIF_Element *        pel = NULL;
    CPQDIF_E_Collection *   pcol = NULL;
    CPQDIF_E_Vector *       pvectSeriesArray = NULL;

    //  Verify that we're in an observation record
    //  Requires a data source record
    if( m_record && m_pds && idxChannel >= 0 && idxSeries >= 0)
    {
        //  Find the channel defn index
        status = GetChannelDefnIdx( idxChannel, idxChannelDefn );

        //  Fail if we got a bogus index
        if( status && idxChannelDefn < 0 )
            status = false;
    }

    //  Get the storage method
    if( status )
    {
        status = m_pds->GetSeriesDefnInfo( idxChannelDefn, idxSeries,
                    idQuantityUnits, idValueType, idQuantityCharacteristic, idStorageMethod );
    }

    //  Look for shared channel and series indices, or a vector of values
    if( status )
    {
        //  Find the series
        pcol = GetOneSeries( idxChannel, idxSeries );
        if( pcol )
        {
            status = true;

            for( idx = 0; idx < pcol->GetCount(); idx++ )
            {
                pel = pcol->GetElement( idx );
                if( pel )
                {
                    if( pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR )
                    {
                        CPQDIF_E_Scalar *psc = (CPQDIF_E_Scalar *) pel;
                        if( PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesShareChannelIdx ) )
                        {
                            gotShareChannel = TRUE;
                            psc->GetValueUINT4( idxShareChannelIdx );
							if( gotShareSeries )
								break;
                        }

                        if( PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesShareSeriesIdx ) )
                        {
                            gotShareSeries = TRUE;
                            psc->GetValueUINT4( idxShareSeriesIdx );
							if( gotShareChannel )
								break;
                        }
                    }
					else if( pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR
                        && PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesValues ) )
                    {
                        pvectSeriesArray = (CPQDIF_E_Vector *) pel;
						break;
                    }

                }
            }
        }
    }

    //  Now count the values
    if( status )
    {
        if( gotShareChannel && gotShareSeries )
        {
            //  We're sharing the series from somwhere else --
            //  generate it from the shared indices
            if( !noShare )
            {
                countPoints = GetCountResolvedSeries( 
                    (long) idxShareChannelIdx, 
                    (long) idxShareSeriesIdx, 
                    true );  // recurse only once
            }
        }
        else if( pvectSeriesArray )
        {
            //  Generate the new series array
            countPoints = _GenerateSeriesCount( idStorageMethod, pvectSeriesArray);
        }
    }

    return countPoints;
}

double * CPQDIF_R_Observation::NewResolvedSeries
            (
            long    idxChannel,
            long    idxSeries,
            long&   countPoints,
            bool    noShare
            )
{
    bool        status = false;
    int         idx;
    double *    arValues = NULL;

    CPQDIF_Element *        pel = NULL;
    CPQDIF_E_Collection *   pcol = NULL;
    CPQDIF_E_Scalar *       psc = NULL;
    CPQDIF_E_Vector *       pvectSeriesArray = NULL;

    long        typePhysical;
    PQDIFValue  value;
    double      valueReal;

    double      rBaseValue = 1.0;
    double      rScale = 1.0;
    double      rOffset = 0.0;

    long        idxChannelDefn = 0;

    string      name;
    UINT4       idPhase;
    GUID        idQuantityType;
    UINT4       idQuantityMeasured;
    UINT4       idQuantityUnits;
    GUID        idQuantityCharacteristic;
    GUID        idValueType;
    UINT4       idStorageMethod = 0;

    bool        gotShareChannel = false;
    bool        gotShareSeries  = false;
    UINT4       idxShareChannelIdx = 0;
    UINT4       idxShareSeriesIdx = 0;

    //  Verify that we're in an observation record
    //  Requires a data source record
    if( m_record && m_pds && idxChannel >= 0 && idxSeries >= 0)
    {
        //  Find the channel defn index
        status = GetChannelDefnIdx( idxChannel, idxChannelDefn );

        //  Fail if we got a bogus index
        if( status && idxChannelDefn < 0 )
            status = false;
    }

    //  Gather information from the channel & series definitions
    if( status )
    {
        status = m_pds->GetChannelDefnInfo( idxChannelDefn, name, idPhase, idQuantityType, idQuantityMeasured );
    }
    if( status )
    {
        status = m_pds->GetSeriesDefnInfo( idxChannelDefn, idxSeries,
                    idQuantityUnits, idValueType, idQuantityCharacteristic, idStorageMethod );
    }

    if( status )
    {
        status = false;

        //  Find the series
        pcol = GetOneSeries( idxChannel, idxSeries );
        if( pcol )
        {
            status = TRUE;

            //  Loop through all elements in the series collection
            for( idx = 0; idx < pcol->GetCount(); idx++ )
            {
                pel = pcol->GetElement( idx );
                if( pel )
                {
                    if( pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR )
                    {
                        psc = (CPQDIF_E_Scalar *) pel;

                        //  If it's REAL4 or REAL8, go ahead and pull it out
                        valueReal = 0.0;
                        psc->GetValue( typePhysical, value );
                        if( typePhysical == ID_PHYS_TYPE_REAL4 )
                        {
                            valueReal = value.real4;
                        }
                        if( typePhysical == ID_PHYS_TYPE_REAL8 )
                        {
                            valueReal = value.real8;
                        }

                        if( PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesBaseQuantity ) )
                        {
                            rBaseValue = valueReal;
                        }

                        if( PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesScale ) )
                        {
                            rScale = valueReal;
                        }

                        if( PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesOffset ) )
                        {
                            rOffset = valueReal;
                        }

                        if( PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesShareChannelIdx ) )
                        {
                            gotShareChannel = TRUE;
                            psc->GetValueUINT4( idxShareChannelIdx );
                        }

                        if( PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesShareSeriesIdx ) )
                        {
                            gotShareSeries = TRUE;
                            psc->GetValueUINT4( idxShareSeriesIdx );
                        }
                    }

                    if( pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR
                        && PQDIF_IsEqualGUID( pel->GetTag(), tagSeriesValues ) )
                    {
                        pvectSeriesArray = (CPQDIF_E_Vector *) pel;
                    }

                }   //  if( pel )
            }
        }
    }

    //  Generate the series data
    if( status )
    {
        if( gotShareChannel && gotShareSeries )
        {
            //  We're sharing the series from somwhere else --
            //  generate it from the shared indices
            if( !noShare )
            {
                arValues = NewResolvedSeries( 
                    (long) idxShareChannelIdx, 
                    (long) idxShareSeriesIdx, 
                           countPoints, true );
            }
            else
            {
                //  Can't generate! The file is trying to share a series
                //  from another shared series. This is not legal.
                arValues = NULL;
            }
        }
        else if( pvectSeriesArray )
        {
            //  Generate the new series array
            arValues = _GenerateSeriesData( idxChannelDefn, idStorageMethod, 
                rBaseValue, rScale, rOffset, pvectSeriesArray, countPoints );
        }
        else
        {
            //  Can't generate!
            arValues = NULL;
        }
    }

    return arValues;
}


TIMESTAMPPQDIF * CPQDIF_R_Observation::NewResolvedSeriesTimeStamp
            (
            long    idxChannel,
            long    idxSeries,
            long&   countPoints,
            bool    noShare
            )
{
    TIMESTAMPPQDIF *        result = NULL;

    CPQDIF_E_Collection *   pcol;
    CPQDIF_Element *        pel;
    CPQDIF_E_Scalar *       psc;
    CPQDIF_E_Vector *       pvectSeriesArray;

    //  Shared?
    BOOL    gotShareChannel = FALSE;
    BOOL    gotShareSeries = FALSE;
    UINT4   idxShareChannelIdx = 0;
    UINT4   idxShareSeriesIdx = 0;

    pcol = GetOneSeries( idxChannel, idxSeries );
    if( pcol )
    {
        //  Is this sucker shared?
        pel = pcol->GetElement( tagSeriesShareChannelIdx );
        if( pel && pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR )
        {
            gotShareChannel = TRUE;
            psc = static_cast<CPQDIF_E_Scalar *>( pel );
            psc->GetValueUINT4( idxShareChannelIdx );
        }

        pel = pcol->GetElement( tagSeriesShareSeriesIdx );
        if( pel && pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR )
        {
            gotShareSeries = TRUE;
            psc = static_cast<CPQDIF_E_Scalar *>( pel );
            psc->GetValueUINT4( idxShareSeriesIdx );
        }


        if( gotShareSeries && gotShareSeries && !noShare )
        {
            //  SHARED.
            //  Allow ONE level of recursion -- no more.
            result = NewResolvedSeriesTimeStamp( idxShareChannelIdx, 
                        idxShareSeriesIdx, countPoints, true );
        }
        else
        {
            //  NOT SHARED.
            //  Try to decode it the normal way.
            pel = pcol->GetElement( tagSeriesValues );
            if( pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR )
            {
                pvectSeriesArray = static_cast<CPQDIF_E_Vector *>( pel );

                //  Is it already in timestamp form?
                if( pvectSeriesArray->GetPhysicalType() == ID_PHYS_TYPE_TIMESTAMPPQDIF )
                {
                    
                    //  Get the point count.
                    pvectSeriesArray->GetCount( countPoints );
                    
                    //  Allocate a buffer for the result.
                    if( countPoints > 0 )
                        result = new TIMESTAMPPQDIF[ countPoints ];
                    
                    //  Get the result data.
                    if( result )
                        countPoints = pvectSeriesArray->GetValuesTimeStamp ( result, countPoints );
                
                }
                else
                {
                    //  Fabricate it from starting time stamp and second offsets
                    double * seconds = NewResolvedSeries( idxChannel, idxSeries, countPoints, true );
                    if( seconds )
                    {
                                        
                        //  Allocate a buffer for the result.
                        if( countPoints > 0 )
                            result = new TIMESTAMPPQDIF[ countPoints ];

                        //  Convert the seconds offset data into time stamps.
                        if( result )
                            _GenerateTimeStampArray( result, seconds, countPoints );
                        
                        delete [] seconds;
                    }
                }

            }
        }
    }

    return result;
}


bool CPQDIF_R_Observation::GetChannelDefnIdx
            (
            long    idxChannel,
            long&   idxChannelDefn
            )
{
    bool                    status = false;
    CPQDIF_E_Collection *   pcollOneChannel;
    CPQDIF_E_Scalar *       psc;
    UINT4                   idx;

    //  Init
    status = false;
    idxChannelDefn = 0;

    //  Find the channel
    pcollOneChannel = GetOneChannel( idxChannel );
    if( pcollOneChannel )
    {
        //  This is where the index should be
        psc = m_record->FindScalarInCollection( pcollOneChannel, tagChannelDefnIdx );
        if( psc )
        {
            status = psc->GetValueUINT4( idx );
            idxChannelDefn = (long) idx;
            status = TRUE;
        }
    }

    return status;
}


CPQDIF_E_Collection * CPQDIF_R_Observation::GetChannelInstances( void )
{
    CPQDIF_E_Collection *   pcollChannels;

    //  Create the collection if it does not yet exist.
    pcollChannels = m_record->FindCollectionInCollection( m_record->m_pcollMain, tagChannelInstances );
    if( !pcollChannels )
    {
        pcollChannels = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollChannels )
        {
            pcollChannels->SetTag( tagChannelInstances );
            m_record->m_pcollMain->Add( pcollChannels );
        }
    }

    return pcollChannels;
}


CPQDIF_E_Collection * CPQDIF_R_Observation::GetOneChannel( long idxChannel )
{
    CPQDIF_E_Collection *   pcolReturn = NULL;
    CPQDIF_E_Collection *   pcolInstances = GetChannelInstances();
    CPQDIF_Element *        pel;

    if( pcolInstances )
    {
        pel = pcolInstances->GetElement( idxChannel );
        if( pel )
        {
            if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION
                && PQDIF_IsEqualGUID( pel->GetTag(), tagOneChannelInst ) )
            {
                pcolReturn = (CPQDIF_E_Collection *) pel;
            }
        }
    }

    return pcolReturn;
}

    
CPQDIF_E_Collection * CPQDIF_R_Observation::GetSeriesInstances
            ( 
            CPQDIF_E_Collection * pcolChannel
            )
{
    return m_record->FindCollectionInCollection( pcolChannel, tagSeriesInstances );
}


CPQDIF_E_Collection * CPQDIF_R_Observation::GetOneSeries
            ( 
            CPQDIF_E_Collection *   pcolChannel,
            long                    idxSeries
            )
{
    CPQDIF_E_Collection *   pcolReturn = NULL;
    CPQDIF_E_Collection *   pcolSeriesInstances = NULL;
    CPQDIF_Element *        pel;

    if( pcolChannel )
    {
        //  Get the series instances for this channel instance
        pcolSeriesInstances = GetSeriesInstances( pcolChannel );
        if( pcolSeriesInstances )
        {
            //  Find the specific series instance
            pel = pcolSeriesInstances->GetElement( idxSeries );
            if( pel )
            {
                if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION
                    && PQDIF_IsEqualGUID( pel->GetTag(), tagOneSeriesInstance ) )
                {
                    //  Return it!
                    pcolReturn = (CPQDIF_E_Collection *) pel;
                }
            }
        }
    }

    return pcolReturn;
}


CPQDIF_E_Collection * CPQDIF_R_Observation::GetOneSeries
            ( 
            long    idxChannel,
            long    idxSeries
            )
{
    CPQDIF_E_Collection *   pcolReturn = NULL;
    CPQDIF_E_Collection *   pcolOneChannel = NULL;

    pcolOneChannel = GetOneChannel( idxChannel );
    pcolReturn = GetOneSeries( pcolOneChannel, idxSeries );

    return pcolReturn;
}


long CPQDIF_R_Observation::_GenerateSeriesCount
            ( 
            UINT4               idStorageMethod, 
            CPQDIF_E_Vector *   pvectSeriesArray
            )
{
    long    countPoints = 0;

    if( idStorageMethod & ID_SERIES_METHOD_VALUES )
    {
        //  Straight vector.
        pvectSeriesArray->GetCount( countPoints );
    }
    else if( idStorageMethod & ID_SERIES_METHOD_INCREMENT )
    {
        //  Incremental storage method ... some assembly required.
        long    countValues;
        long    idxValue;
        double  rCount;

        //  Read in the rate change list
        pvectSeriesArray->GetCount( countValues );
        if( countValues > 0 )
        {
            //  Grab only the point counts at indices 1, 3, 5, ...
            for( idxValue = 1; idxValue < countValues; idxValue += 2 )
            {
                pvectSeriesArray->GetValue( idxValue, rCount );
                countPoints += (long) rCount;
            }
        }
    }
    else
    {
        //  No third option!
    }

    return countPoints;
}


double * CPQDIF_R_Observation::_GenerateSeriesData
            ( 
            int                 idxChannelDefn,
            UINT4               idStorageMethod, 
            double              rBaseValue, 
            double              rScale, 
            double              rOffset, 
            CPQDIF_E_Vector *   pvectSeriesArray, 
            long&               countPoints
            )
{
    double *    arValues = NULL;
    double *    prTempValues;
    long        idxPoint;
    long        countValues;
    long        idxValue;
    bool        useScale = false;
    bool        useCal = false;

    //  Unused parameter
    rBaseValue = rBaseValue;

    //  Get raw number of values in the vector. This may be the actual
    //  number of points.
    pvectSeriesArray->GetCount( countValues );

    //  Determine how many total points
    countPoints = _GenerateSeriesCount( idStorageMethod, pvectSeriesArray );

    if( idStorageMethod & ID_SERIES_METHOD_VALUES )
    {
        //  Straight vector. No assembly necessary.
        //  The two counts must be identical.
        if( countPoints > 0 && countPoints == countValues )
        {
            //  We already have the number of points...
            arValues = new double[ countPoints ];
            prTempValues = arValues;

            for( idxPoint = 0; idxPoint < countPoints; idxPoint++, prTempValues++ )
            {
                pvectSeriesArray->GetValue( idxPoint, *prTempValues );
            }
        }
    }
    else if( idStorageMethod & ID_SERIES_METHOD_INCREMENT )
    {
        //  Incremental storage method ... some assembly required.
        double      valueNext;
        double      rCount;
        double      rRate ;
        long        countChanges;
        long        idxChange;
        long        countPointsThisChange;

        //  Go through the rate change list and generate the array.
        if( countPoints > 0 )
        {
            arValues = new double[ countPoints ];
            if( arValues )
            {
                //  Init -- get ready for loops
                prTempValues = arValues;
                valueNext = 0.0;    //  First item will be zero

                //  Determine total number of rate changes [0]
                pvectSeriesArray->GetValue( 0, rCount );
                countChanges = (long) rCount;

                //  Get counts and rate changes ...
                for( idxChange = 0; idxChange < countChanges; idxChange++ )
                {
                    //  Calculate index into value array ...
                    idxValue = ( idxChange * 2 ) + 1;

                    //  Get data ...
                    pvectSeriesArray->GetValue( idxValue + 0, rCount );  //  Count @ rate
                    pvectSeriesArray->GetValue( idxValue + 1, rRate );   //  Rate
                    countPointsThisChange = (long) rCount;

                    //  Generate points from the data ...
                    for( idxPoint = 0; idxPoint < countPointsThisChange; idxPoint++, prTempValues++ )
                    {
                        //  Store current point.
                        *prTempValues = valueNext;

                        //  Calculate next point.
                        valueNext += rRate;
                    }   //  for( points );
                }   //  for( changes )

            }
        }
    }
    else
    {
        //  No third option!
        arValues = NULL;
    }

    //  Scale & offset?
    if( idStorageMethod & ID_SERIES_METHOD_SCALED )
    {
        //  Scale/offset should already be set.
        useScale = true;
    }
    else
    {
        //  Default values for scale/offset
        rScale = 1.0;
        rOffset = 0.0;
        useScale = false;
    }

    //  CT/PT ratios?
    useCal = _GetCalibrationRatio( idxChannelDefn, rScale );
    if( useCal )
    {
        //  The scale has now been adjusted to apply
        //  the calibration ratio.
        useScale = true;
    }

    //  Adjust, if necessary.
    if( useScale )
    {
        if( arValues && countPoints > 0 )
        {
            prTempValues = arValues;
            for( idxPoint = 0; idxPoint < countPoints; idxPoint++, prTempValues++ )
            {
                //  Apply scale first, then offset
                *prTempValues *= rScale;
                *prTempValues += rOffset;
            }   //  for( points );
        }
    }

    return arValues;
}


bool CPQDIF_R_Observation::_GenerateTimeStampArray
            (
            TIMESTAMPPQDIF *    result, 
            double *            seconds, 
            long                countPoints 
            )
{
    bool    status = false;
    TIMESTAMPPQDIF      tsStart;
    TIMESTAMPPQDIF      tsTemp;
    string              name;
    TIMESTAMPPQDIF *    resTemp = result;
    double *            secTemp = seconds; 


    //  Get start time for the entire observation. If successful then ...
    status = GetInfo( tsStart, tsTemp, name );
	if (tsStart.day > 0) status = 1;
    if( status )
    {

        //  For each value do ...
        for( int idx = 0; idx < countPoints; idx++, resTemp++, secTemp++ )
        {

            //  Get starting time
            *resTemp = tsStart;

            //  Add seconds to it
            _AddSeconds( *resTemp, *secTemp );

        }   //  for( stamps )

    }

    return status;
}


bool CPQDIF_R_Observation::_GetCalibrationRatio
            (
            int     idxChannelDefn,
            double& rScale
            )
{
    bool            status = false;

    if( m_psett )
    {
        TIMESTAMPPQDIF  tsTemp;
        bool            useCal = false;
        bool            useTrans = false;   //  Not used

        bool gotInfo = m_psett->GetInfo( tsTemp, tsTemp, tsTemp, useCal, useTrans );
        if( gotInfo && useCal )
        {
            long    countChannels;
            countChannels = m_psett->GetCountChannels();
            for( long idxChannel = 0; idxChannel < countChannels; idxChannel++ )
            {
                UINT4           idxChannelDefnCurrent;
                gotInfo = m_psett->GetChannelInfo( idxChannel, idxChannelDefnCurrent);

                if( gotInfo )
                {
                    //  Is this the information for the correct channel defn?
                    if( idxChannelDefn == (int) idxChannelDefnCurrent )
                    {
                        UINT4           xdTransformerTypeID;
                        REAL8           xdSystemSideRatio;
                        REAL8           xdMonitorSideRatio;
                        CPQDIF_E_Vector xdFrequencyResponse;

                        gotInfo = m_psett->GetChanTrans( idxChannel, 
                                    xdTransformerTypeID,
                                    xdSystemSideRatio, xdMonitorSideRatio,
                                    xdFrequencyResponse );
                        if( gotInfo )
                        {
                            //  Looks like we got it...
                            rScale *= xdMonitorSideRatio;
                            rScale /= xdSystemSideRatio;
                            status = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    return status;
}

