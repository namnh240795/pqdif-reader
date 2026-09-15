/*
**  Class:          CPQDIF_R_DataSource
**  Description:    Implements a PQDIF record "wrapper" for the data source record. You can cast a standard record object to this class.
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


//  Operations

bool CPQDIF_R_DataSource::GetInfo
            (
            GUID&       idDataSourceType,
            GUID&       idVendor,
            GUID&       idEquipment,
            string&     serialNumber,
            string&     version,
            string&     name,
            string&     owner,
            string&     location,
            string&     timeZone
            )
{
    bool    status = FALSE;

    CPQDIF_E_Collection *   pcollMain = m_pcollMain;
    CPQDIF_E_Scalar *       psc;
    CPQDIF_E_Vector *       pvect;
    
    if( pcollMain )
    {
        //  Find GUIDs
        psc = FindScalarInCollection( pcollMain, tagDataSourceTypeID );
        if( psc ) psc->GetValueGUID( idDataSourceType );
        psc = FindScalarInCollection( pcollMain, tagVendorID );
        if( psc ) psc->GetValueGUID( idVendor );
        psc = FindScalarInCollection( pcollMain, tagEquipmentID );
        if( psc ) psc->GetValueGUID( idEquipment );

        //  Find strings
        pvect = FindVectorInCollection( pcollMain, tagSerialNumberDS );
        if( pvect ) pvect->GetValues( serialNumber );
        pvect = FindVectorInCollection( pcollMain, tagVersionDS );
        if( pvect ) pvect->GetValues( version );
        pvect = FindVectorInCollection( pcollMain, tagNameDS );
        if( pvect ) pvect->GetValues( name );
        pvect = FindVectorInCollection( pcollMain, tagOwnerDS );
        if( pvect ) pvect->GetValues( owner );
        pvect = FindVectorInCollection( pcollMain, tagLocationDS );
        if( pvect ) pvect->GetValues( location );
        pvect = FindVectorInCollection( pcollMain, tagTimeZoneDS );
        if( pvect ) pvect->GetValues( timeZone );

        status = TRUE;
    }

    return status;
}


long CPQDIF_R_DataSource::GetCountChannelDefns( void )
{
    long                    count = 0;
    CPQDIF_E_Collection *   pcollDefns;
    
    pcollDefns = GetChannelDefns();
    if( pcollDefns )
    {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcollDefns->GetTag() );
#endif
        count = pcollDefns->GetCount();
    }

    return count;
}
        
long CPQDIF_R_DataSource::GetCountSeriesDefns( int idxChannelDefn )
{
    long                    count = 0;
    CPQDIF_E_Collection *   pcolOneChannelDefn;
    CPQDIF_E_Collection *   pcollSeriesDefns;
    
    //  Find the one channel defn we're looking for
    pcolOneChannelDefn = GetOneChannelDefn( idxChannelDefn );
    if( pcolOneChannelDefn )
    {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcolOneChannelDefn->GetTag() );
#endif
        //  Find the series defns for this channel defn
        pcollSeriesDefns = GetSeriesDefns( pcolOneChannelDefn );
        if( pcollSeriesDefns )
        {
            //  Get the count!
            count = pcollSeriesDefns->GetCount();
        }
    }

    return count;
}


bool CPQDIF_R_DataSource::GetChannelDefnInfo
            (
            long        idxChannelDefn,
            string&     name,
            UINT4&      idPhase,
            GUID&       idQuantityType,
            UINT4&      idQuantityMeasured
            )
{
    bool    status = FALSE;

    CPQDIF_E_Collection *   pcolOneChannelDefn;

    //  Elements we need
    CPQDIF_E_Vector *       pvecName;
    CPQDIF_E_Scalar *       pscPhaseID;
    CPQDIF_E_Scalar *       pscQuantityID;

    //  Init
    name = "";
    idPhase = ID_PHASE_NONE;            //  default
    idQuantityType = ID_QT_WAVEFORM;    //  default
    idQuantityMeasured = ID_QM_NONE;    //  default
    
    //  Find the one channel defn we're looking for
    pcolOneChannelDefn = GetOneChannelDefn( idxChannelDefn );
    if( pcolOneChannelDefn )
    {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcolOneChannelDefn->GetTag() );
#endif
        pvecName = FindVectorInCollection( pcolOneChannelDefn, tagChannelName );  //  optional
        if( pvecName )
        {
            pvecName->GetValues( name );
        }
        pscPhaseID = FindScalarInCollection( pcolOneChannelDefn, tagPhaseID );
        if( pscPhaseID )
        {
            //  Required; check the return value
            status = pscPhaseID->GetValueUINT4( idPhase );
        }
        pscQuantityID = FindScalarInCollection( pcolOneChannelDefn, tagQuantityTypeID );
        if( status && pscQuantityID )
        {
            //  Required; check the return value
            status = pscQuantityID->GetValueGUID( idQuantityType );
        }
        pscQuantityID = FindScalarInCollection( pcolOneChannelDefn, tagQuantityMeasuredID );
        if( status && pscQuantityID )
        {
            //  Required; check the return value
            status = pscQuantityID->GetValueUINT4( idQuantityMeasured );
        }
    }

    return status;
}


bool CPQDIF_R_DataSource::GetChannelPrimarySeries
            (
            long        idxChannelDefn,
            long&       idxPrimarySeries
            )
{
    bool    status = FALSE;

    CPQDIF_E_Collection *   pcolOneChannelDefn;

    //  Elements we need
    CPQDIF_E_Scalar *       psc;
    UINT4                   value;

    //  Find the one channel defn we're looking for
    pcolOneChannelDefn = GetOneChannelDefn( idxChannelDefn );
    if( pcolOneChannelDefn )
    {
        psc = FindScalarInCollection( pcolOneChannelDefn, tagPrimarySeriesIdx );
        if( psc )
        {
            //  Required; check the return value
            status = psc->GetValueUINT4( value );
            if( status )
                idxPrimarySeries = (long) value;
        }
    }

    return status;
}


bool CPQDIF_R_DataSource::GetSeriesDefnInfo
            (
            long     idxChannelDefn,
            long     idxSeriesDefn,
            UINT4&   idQuantityUnits,
            GUID&    idValueType,
            GUID&    idQuantityCharacteristic,
            UINT4&   idStorageMethod
            )
{
    bool    status = FALSE;

    CPQDIF_E_Collection *   pcolOneSeriesDefn;
    CPQDIF_E_Scalar *       pscalar;

    //  Init
    idQuantityUnits = ID_QU_NONE;
    idValueType = ID_SERIES_VALUE_TYPE_VAL;
    idStorageMethod = ID_SERIES_METHOD_VALUES;
    
    //  Find the one channel defn we're looking for
    pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
    {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcolOneSeriesDefn->GetTag() );
#endif
        //  These are all required; check the return values
        status = TRUE;
        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagQuantityUnitsID );
        if( status && pscalar )
        {
            status = pscalar->GetValueUINT4( idQuantityUnits );
        }
        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagValueTypeID );
        if( status && pscalar )
        {
            status = pscalar->GetValueGUID( idValueType );
        }
        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagQuantityCharacteristicID );
        if( status && pscalar )
        {
            status = pscalar->GetValueGUID( idQuantityCharacteristic );
        }
        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagStorageMethodID );
        if( status && pscalar )
        {
            status = pscalar->GetValueUINT4( idStorageMethod );
        }
    }

    return status;
}


//  Protected stuff
        
CPQDIF_E_Collection * CPQDIF_R_DataSource::GetChannelDefns( void )
{
    CPQDIF_E_Collection *   pcollDefns = NULL;

    //  Create the collection if it does not yet exist.
    if( m_pcollMain )
    {
        pcollDefns = FindCollectionInCollection( m_pcollMain, tagChannelDefns );
        if( !pcollDefns )
        {
            pcollDefns = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            if( pcollDefns )
            {
                pcollDefns->SetTag( tagChannelDefns );
                m_pcollMain->Add( pcollDefns );
            }
        }
    }

    return pcollDefns;
}


CPQDIF_E_Collection * CPQDIF_R_DataSource::GetOneChannelDefn
        ( 
        long idxChannelDefn
        )
{
    CPQDIF_E_Collection *   pcolReturn = NULL;
    CPQDIF_E_Collection *   pcolInstances = GetChannelDefns();
    CPQDIF_Element *        pel;

    if( pcolInstances )
    {
        pel = pcolInstances->GetElement( idxChannelDefn );
        if( pel )
        {
            if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION
                && PQDIF_IsEqualGUID( pel->GetTag(), tagOneChannelDefn ) )
            {
                pcolReturn = (CPQDIF_E_Collection *) pel;
            }
        }
    }

    return pcolReturn;
}

    
CPQDIF_E_Collection * CPQDIF_R_DataSource::GetSeriesDefns
            ( 
            CPQDIF_E_Collection * pcolOneChannelDefn
            )
{
    return FindCollectionInCollection( pcolOneChannelDefn, tagSeriesDefns );
}


CPQDIF_E_Collection * CPQDIF_R_DataSource::GetOneSeriesDefn
            ( 
            CPQDIF_E_Collection *   pcolOneChannelDefn,
            long                    idxSeriesDefn
            )
{
    CPQDIF_E_Collection *   pcolReturn = NULL;
    CPQDIF_E_Collection *   pcolSeriesDefns = NULL;
    CPQDIF_Element *        pel;

    if( pcolOneChannelDefn )
    {
        //  Grab the series defns collection for this channels defn.
        pcolSeriesDefns = GetSeriesDefns( pcolOneChannelDefn );
        if( pcolSeriesDefns )
        {
#ifdef _DEBUG
            //  Is this the right tag?
            const   char *  pname;
            pname = theInfo.GetNameOfTag( pcolSeriesDefns->GetTag() );
#endif

            //  Find the specific series defn
            pel = pcolSeriesDefns->GetElement( idxSeriesDefn );
            if( pel )
            {
#ifdef _DEBUG
                pname = theInfo.GetNameOfTag( pel->GetTag() );
#endif
                if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION
                    && PQDIF_IsEqualGUID( pel->GetTag(), tagOneSeriesDefn ) )
                {
                    //  Return it!
                    pcolReturn = (CPQDIF_E_Collection *) pel;
                }
            }
        }
    }

    return pcolReturn;
}


CPQDIF_E_Collection * CPQDIF_R_DataSource::GetOneSeriesDefn
            ( 
            long    idxChannelDefn,
            long    idxSeriesDefn
            )
{
    CPQDIF_E_Collection *   pcolReturn = NULL;
    CPQDIF_E_Collection *   pcolOneChannel = NULL;

    pcolOneChannel = GetOneChannelDefn( idxChannelDefn );
    if( pcolOneChannel )
    {
        pcolReturn = GetOneSeriesDefn( pcolOneChannel, idxSeriesDefn );
    }

    return pcolReturn;
}


long CPQDIF_R_DataSource::AddChannelDefn
            (
            const   char *  name,
                    UINT4   idPhase,
            const   GUID&   idQuantityType
            )
{
    long        idxNew = -1;

    CPQDIF_E_Collection *   pcollDefns;
    CPQDIF_E_Collection *   pcollOne;
    CPQDIF_E_Collection *   pcollSeriesDefns;
    
    pcollDefns = GetChannelDefns();
    if( pcollDefns )
    {
        //  Get the new index
        idxNew = GetCountChannelDefns();

        //  Create the new channel def'n
        pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollOne )
        {
            pcollOne->SetTag( tagOneChannelDefn );

            //  Stuff in the information...
            pcollOne->SetVectorString( tagChannelName, name );
            pcollOne->SetScalarUINT4( tagPhaseID, idPhase );
            pcollOne->SetScalarGUID( tagQuantityTypeID, idQuantityType );

            //  Also add an empty collection for the series definitions
            pcollSeriesDefns = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            if( pcollSeriesDefns )
            {
                pcollSeriesDefns->SetTag( tagSeriesDefns );
                pcollOne->Add( pcollSeriesDefns );
            }
        }

        //  Add it!
        pcollDefns->Add( pcollOne );
    }

    return idxNew;
}

long CPQDIF_R_DataSource::AddChannelDefn2
            (
            const   char *  name,
                    UINT4   idPhase,
                    UINT4   idQM,
            const   GUID&   idQuantityType
            )
{
    long        idxNew = -1;

    CPQDIF_E_Collection *   pcollDefns;
    CPQDIF_E_Collection *   pcollOne;
    CPQDIF_E_Collection *   pcollSeriesDefns;
    
    pcollDefns = GetChannelDefns();
    if( pcollDefns )
    {
        //  Get the new index
        idxNew = GetCountChannelDefns();

        //  Create the new channel def'n
        pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
        if( pcollOne )
        {
            pcollOne->SetTag( tagOneChannelDefn );

            //  Stuff in the information...
            pcollOne->SetVectorString( tagChannelName, name );
            pcollOne->SetScalarUINT4( tagPhaseID, idPhase );
            pcollOne->SetScalarUINT4( tagQuantityMeasuredID, idQM );
            pcollOne->SetScalarGUID( tagQuantityTypeID, idQuantityType );

            //  Also add an empty collection for the series definitions
            pcollSeriesDefns = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            if( pcollSeriesDefns )
            {
                pcollSeriesDefns->SetTag( tagSeriesDefns );
                pcollOne->Add( pcollSeriesDefns );
            }
        }

        //  Add it!
        pcollDefns->Add( pcollOne );
    }

    return idxNew;
}

long CPQDIF_R_DataSource::AddSeriesDefn
            (
                    long     idxChannelDefn,
                    UINT4    idQuantityUnits,
            const   GUID     idValueType,
                    UINT4    idStorageMethod
            )
{
    long        idxNew = -1;

    CPQDIF_E_Collection *   pcolOneChannelDefn;
    CPQDIF_E_Collection *   pcollSeriesDefns;
    CPQDIF_E_Collection *   pcollOne;
    
    //  Find the one channel defn we're looking for
    pcolOneChannelDefn = GetOneChannelDefn( idxChannelDefn );
    if( pcolOneChannelDefn )
    {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcolOneChannelDefn->GetTag() );
#endif

        //  Find the series defns for this channel defn
        pcollSeriesDefns = GetSeriesDefns( pcolOneChannelDefn );
        if( pcollSeriesDefns )
        {
            //  Get the new index
            idxNew = GetCountSeriesDefns( idxChannelDefn );

            //  Create the new series def'n
            pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            pcollOne->SetTag( tagOneSeriesDefn );

            //  Stuff in the information...
            pcollOne->SetScalarUINT4( tagQuantityUnitsID, idQuantityUnits );
            pcollOne->SetScalarGUID( tagValueTypeID, idValueType );
            pcollOne->SetScalarUINT4( tagStorageMethodID, idStorageMethod );

            //  Add it!
            pcollSeriesDefns->Add( pcollOne );
        }
    }

    return idxNew;
}


long CPQDIF_R_DataSource::AddSeriesDefn2
            (
                    long     idxChannelDefn,
                    UINT4    idQuantityUnits,
            const   GUID     idValueType,
            const   GUID     idCharacteristicType,
                    UINT4    idStorageMethod
            )
{
    long        idxNew = -1;

    CPQDIF_E_Collection *   pcolOneChannelDefn;
    CPQDIF_E_Collection *   pcollSeriesDefns;
    CPQDIF_E_Collection *   pcollOne;
    
    //  Find the one channel defn we're looking for
    pcolOneChannelDefn = GetOneChannelDefn( idxChannelDefn );
    if( pcolOneChannelDefn )
    {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcolOneChannelDefn->GetTag() );
#endif
        //  Find the series defns for this channel defn
        pcollSeriesDefns = GetSeriesDefns( pcolOneChannelDefn );
        if( pcollSeriesDefns )
        {
            //  Get the new index
            idxNew = GetCountSeriesDefns( idxChannelDefn );

            //  Create the new series def'n
            pcollOne = (CPQDIF_E_Collection *) theFactory.NewElement( ID_ELEMENT_TYPE_COLLECTION );
            pcollOne->SetTag( tagOneSeriesDefn );

            //  Stuff in the information...
            pcollOne->SetScalarUINT4( tagQuantityUnitsID, idQuantityUnits );
            pcollOne->SetScalarGUID( tagQuantityCharacteristicID, idCharacteristicType );
            pcollOne->SetScalarGUID( tagValueTypeID, idValueType );
            pcollOne->SetScalarUINT4( tagStorageMethodID, idStorageMethod );

            //  Add it!
            pcollSeriesDefns->Add( pcollOne );
        }
    }

    return idxNew;
}

bool CPQDIF_R_DataSource::GetSeriesDefnNominal
            (
            long     idxChannelDefn,
            long     idxSeriesDefn,
            double & dNominal
            )
    {
    bool    status = false;

    CPQDIF_E_Collection *   pcolOneSeriesDefn;
    CPQDIF_E_Scalar *       pscalar;

    //  Initialize
    dNominal = -1.0;
    
    //  Find the one channel defn we're looking for
    pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
        {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcolOneSeriesDefn->GetTag() );
#endif
        //  check the return values
        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagSeriesNominalQuantity );
        if( pscalar )
            {
            status = pscalar->GetValueREAL8( dNominal );
            }
        }

    return status;
    }


bool CPQDIF_R_DataSource::GetSeriesDefnPrecisionAndResolution 
			(
            long        idxChannel,
            long        idxSeries,
            UINT4	  & uPrecision,
            double    & dResolution
            )

{
    CPQDIF_E_Collection *   pcolOneSeriesDefn;
    CPQDIF_E_Scalar *       pscalar;
    bool    status = false;

    //  Initialize
    uPrecision = (UINT4)-1;
	dResolution = 0.0;
    
    //  Find the one channel defn we're looking for
    pcolOneSeriesDefn = GetOneSeriesDefn( idxChannel, idxSeries );
    if( pcolOneSeriesDefn )
        {

        //  check the return values
        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagQuantitySignificantDigitsID );
        if( pscalar )
            {
            status = pscalar->GetValueUINT4( uPrecision );
            }

        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagQuantityResolutionID );
        if( status && pscalar )
            {
            status = pscalar->GetValueREAL8( dResolution );
            }
        }

	return status;
    }



bool CPQDIF_R_DataSource::SetSeriesDefnNominal
            (
            long     idxChannelDefn,
            long     idxSeriesDefn,
            double   dNominal
            )
    {
    bool    status = false;

    CPQDIF_E_Collection *   pcolOneSeriesDefn;
    CPQDIF_E_Scalar *       pscalar;

    //  Find the one channel defn we're looking for
    pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
        {
#ifdef _DEBUG
        //  Is this the right tag?
        const   char *  pname;
        pname = theInfo.GetNameOfTag( pcolOneSeriesDefn->GetTag() );
#endif

        status = true;
        //
        //
        //  See if scalar already present
        //
        pscalar = FindScalarInCollection( pcolOneSeriesDefn, tagSeriesNominalQuantity );
        if( pscalar )
            {
            //
            //
            //  Is there so set new value
            //
            status = pscalar->SetValueREAL8( dNominal );
            }
        else
            {
            //
            //
            //  Is not there so add scalar
            //
            pscalar = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
            if( pscalar )
                {
                pscalar->SetTag( tagSeriesNominalQuantity );
                status = pscalar->SetValueREAL8( dNominal );
                pcolOneSeriesDefn->Add( pscalar );
                }
            }
#ifdef zap
        //
        //
        //  The following line does the same thing as the several
        //  lines above but without error checking.
        //
        //  The third parameter allows replacement of tag
        //  value if tag already present.  If tag not present,
        //  it is added and the value set.
        //  The 
        //
        pcolOneSeriesDefn->SetScalarREAL8( tagSeriesNominalQuantity, dNominal, true);
#endif
        }

    return status;
    }

bool CPQDIF_R_DataSource::SetSeriesDefnPrefix
            (
            long    idxChannelDefn,
            long    idxSeriesDefn,
            long    idPrefix
            )
    {
    bool    status = false;

    //  Find the one channel defn we're looking for
    CPQDIF_E_Collection *   pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
        {
        status = true;
        //
        //
        //  The third parameter allows replacement of tag
        //  value if tag already present.  If tag not present,
        //  it is added and the value set.
        //  The 
        //
        pcolOneSeriesDefn->SetScalarUINT4( tagHintGreekPrefixID, idPrefix, true);
        }

    return status;
    }

bool CPQDIF_R_DataSource::SetSeriesDefnUnits
            (
            long    idxChannelDefn,
            long    idxSeriesDefn,
            long    idUnits
            )
    {
    bool    status = false;

    //  Find the one channel defn we're looking for
    CPQDIF_E_Collection *   pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
        {
        status = true;
        //
        //
        //  The third parameter allows replacement of tag
        //  value if tag already present.  If tag not present,
        //  it is added and the value set.
        //  The 
        //
        pcolOneSeriesDefn->SetScalarUINT4( tagHintPreferredUnitsID, idUnits, true);
        }

    return status;
    }

bool CPQDIF_R_DataSource::SetSeriesDefnDisplay
            (
            long    idxChannelDefn,
            long    idxSeriesDefn,
            long    idDisplay
            )
    {
    bool    status = false;

    //  Find the one channel defn we're looking for
    CPQDIF_E_Collection *   pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
        {
        status = true;
        //
        //
        //  The third parameter allows replacement of tag
        //  value if tag already present.  If tag not present,
        //  it is added and the value set.
        //  The 
        //
        pcolOneSeriesDefn->SetScalarUINT4( tagHintDefaultDisplayID, idDisplay, true);
        }

    return status;
    }

bool CPQDIF_R_DataSource::SetSeriesDefnDigits
            (
            long    idxChannelDefn,
            long    idxSeriesDefn,
            long    idDigits
            )
    {
    bool    status = false;

    //  Find the one channel defn we're looking for
    CPQDIF_E_Collection *   pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
        {
        status = true;
        //
        //
        //  The third parameter allows replacement of tag
        //  value if tag already present.  If tag not present,
        //  it is added and the value set.
        //  The 
        //
        pcolOneSeriesDefn->SetScalarUINT4( tagQuantitySignificantDigitsID, idDigits, true);
        }

    return status;
    }


bool CPQDIF_R_DataSource::SetSeriesDefnResolution
            (
            long    idxChannelDefn,
            long    idxSeriesDefn,
            double  dRes
            )
    {
    bool    status = false;

    //  Find the one channel defn we're looking for
    CPQDIF_E_Collection *   pcolOneSeriesDefn = GetOneSeriesDefn( idxChannelDefn, idxSeriesDefn );
    if( pcolOneSeriesDefn )
        {
        status = true;
        //
        //
        //  The third parameter allows replacement of tag
        //  value if tag already present.  If tag not present,
        //  it is added and the value set.
        //  The 
        //
        pcolOneSeriesDefn->SetScalarREAL8( tagQuantityResolutionID, dRes, true);
        }

    return status;
    }


