/*
**  Class:              CPQDIF_R_Observation
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

class CPQDIF_R_Settings;

class CPQDIF_R_Observation : public CPQDIF_R_General
    {
    public:
        CPQDIF_R_Observation();
        CPQDIF_R_Observation( CPQDIFRecord& record );
        virtual ~CPQDIF_R_Observation();

    // Operations
    public:
        void SetDataSource
            (
            CPQDIF_R_DataSource *   pds
            ) 
            { m_pds = pds; }
        void SetMonitorSettings
            (
            CPQDIF_R_Settings *     psett
            )
            { m_psett = psett; }
		CPQDIF_R_Settings * GetMonitorSettings()
		{
			return m_psett;
		}

        long     GetCountResolvedSeries
            ( 
            long    idxChannel,
            long    idxSeries,
			bool    noShare = false
            );
        double * NewResolvedSeries
            (
            long    idxChannel,
            long    idxSeries,
            long&   countPoints,
            bool    noShare = false //  Set to true to prevent another level of shared series.
                                    //  (This could prevent infinitely recursive calls.)
            );
        TIMESTAMPPQDIF * NewResolvedSeriesTimeStamp
            (
            long    idxChannel,
            long    idxSeries,
            long&   countPoints,
            bool    noShare = false
            );

        // Attributes
    public:
        //  Read functions
        bool GetInfo
            (
            TIMESTAMPPQDIF&     timeStart,
            TIMESTAMPPQDIF&     timeCreate,
            string&             name
            );
        long GetCountChannels( void );
        long GetCountSeries( int idxChannel );
        bool GetChannelInfo
            (
            long        idxChannel,
            string&     name,
            UINT4&      idPhase,
            GUID&       idQuantityType,
            UINT4&      idQuantityMeasured
            );
        bool GetChannelPrimarySeries
            (
            long        idxChannel,
            long&       idxPrimarySeries
            );
#ifdef zap
        bool GetChannelThresholds
            (
            long                idxChannel,
            UINT4&              triggerTypeID,
            REAL8&              fullScale,  
            REAL8&              noiseFloor, 
            REAL8&              triggerLow, 
            REAL8&              triggerHigh,
            REAL8&              triggerRate,
            CPQDIF_E_Vector&    triggerShapeParam   //  Array of [3]
            );
#endif
        bool GetSeriesInfo
            (
            long        idxChannel,
            long        idxSeries,
            UINT4&      idQuantityUnits,
            GUID&       idQuantityCharacteristic,
            GUID&       idValueType
            );
        bool GetTriggerInfo
            (
            UINT4&              idTriggerMethod,
            CPQDIF_E_Vector **  pvectTriggerChanIdx,
            TIMESTAMPPQDIF&     timeTriggered
            );
        bool GetPqdsInfo
            (
            GUID&       idDataSourceType,       //  GUID
            GUID&       idInstrumentType,       //  GUID
            UINT4&      codeLocation,           
            CPQDIF_E_Vector **  pvectCodesDisturbanceType    //  Array[ 3 ] of UINT4
            );
        bool GetSeriesBaseQuantity
            (
            long        idxChannel,
            long        idxSeries,
            double&     value
            );
        bool GetSeriesScale
            (
            long        idxChannel,
            long        idxSeries,
            double&     scale,
            double&     offset
            );

        bool GetSeriesDefnNominal
            (
            long        idxChannel,
            long        idxSeries,
            double    & dNominal
            );

		bool GetSeriesDefnPrecisionAndResolution
            (
            long        idxChannel,
            long        idxSeries,
            UINT4	  & uPrecision,
            double    & dResolution
            );

        CPQDIF_E_Vector * GetSeriesValueVector
            (
            long        idxChannel,
            long        idxSeries
            );

        //  Write functions
        long AddChannel
            (
            long        idxChannelDefn
            );
        long AddSeriesDouble
            (
            long        idxChannel,
            long        countValues,
            double *    arValues
            );
        bool SetSeriesBaseQuantity
            (
            long        idxChannel,
            long        idxSeries,
            double      value
            );
        bool SetSeriesScale
            (
            long        idxChannel,
            long        idxSeries,
            double      scale,
            double      offset
            );
        long AddSeriesVector
            (
            long            idxChannel,
            CPQDIF_E_Vector *   pvect
            );
        long AddSeriesShared
            (
            long            idxChannel,
            long            idxChannelShared,
            long            idxSeriesShared
            );
        bool SetTriggerInfo
            (
                    UINT4               idTriggerMethod,
                    long                countTriggers,
            const   UINT4 *             aidxTriggerChan,
            const   TIMESTAMPPQDIF&     timeTriggered
            );

#ifdef PQDIF_USE_COM

		bool GetObservationExtendedData
            (
			GUID& gidTag,
            VARIANT&  value
            );

		bool GetChannelExtendedData
            (
            long        idxChannel,
			GUID& gidTag,
            VARIANT&  value
            );

		bool GetSeriesExtendedData
            (
            long        idxChannel,
			long		idxSeries,
			GUID& gidTag,
            VARIANT&  value
            );
#endif

		bool CPQDIF_R_Observation::GetObservationExtendedData
            (
			GUID& gidTag,
            long& typePhysical,
			PQDIFValue& value
            );
		bool GetChannelExtendedData
            (
            long        idxChannel,
			GUID& gidTag,
            long& typePhysical,
			PQDIFValue& value
            );

		bool GetSeriesExtendedData
            (
            long        idxChannel,
			long		idxSeries,
			GUID& gidTag,
            long& typePhysical,
			PQDIFValue& value
            );

    //  Internal functions
    public:
        bool GetChannelDefnIdx
            (
            long    idxChannel,
            long&   idxChannelDefn
            );
        CPQDIF_E_Collection * GetChannelInstances( void );
        CPQDIF_E_Collection * GetOneChannel
            ( 
            long    idxChannel
            );
        CPQDIF_E_Collection * GetSeriesInstances
            ( 
            CPQDIF_E_Collection * pcolChannel
            );
        CPQDIF_E_Collection * GetOneSeries
            ( 
            CPQDIF_E_Collection *   pcolChannel,
            long                    idxSeries
            );
        CPQDIF_E_Collection * GetOneSeries
            ( 
            long    idxChannel,
            long    idxSeries
            );

    //  OVERRIDES
    // Operations
    public:
        virtual bool ReadHeader( CPQDIF_StreamIO * pstream )
            { return m_record->ReadHeader( pstream ); }
        virtual bool ReadBody( CPQDIF_StreamIO * pstream )
            { return m_record->ReadBody( pstream ); }

        virtual bool WriteHeader( CPQDIF_StreamIO * pstream )
            { return m_record->WriteHeader( pstream ); }
        virtual bool WriteBody( CPQDIF_StreamIO * pstream )
            { return m_record->WriteBody( pstream ); }

    //  OVERRIDES
        // Attributes
    public:
        virtual bool HeaderGetPos( LINKABS4& pos ) const
            { return m_record->HeaderGetPos( pos ); }
        virtual bool HeaderSetPos( LINKABS4 pos )
            { return m_record->HeaderSetPos( pos ); }
        virtual bool HeaderGetTag( GUID& tagRecord ) const
            { return m_record->HeaderGetTag( tagRecord ); }
        virtual bool HeaderSetTag( GUID tagRecord )
            { return m_record->HeaderSetTag( tagRecord ); }
        virtual bool HeaderGetSize( SIZE4& sizeHeader, SIZE4& sizeBody ) const
            { return m_record->HeaderGetSize( sizeHeader, sizeBody ); }
        virtual bool HeaderSetSize( SIZE4 sizeHeader, SIZE4 sizeBody )
            { return m_record->HeaderSetSize( sizeHeader, sizeBody ); }
        virtual bool HeaderGetChecksum( UINT& checksum ) const
            { return m_record->HeaderGetChecksum( checksum ); }
        virtual bool HeaderSetChecksum( UINT checksum )
            { return m_record->HeaderSetChecksum( checksum ); }
        virtual bool HeaderGetPosNextRecord( LINKABS4& pos ) const
            { return m_record->HeaderGetPosNextRecord( pos ); }
        virtual bool HeaderSetPosNextRecord( LINKABS4 pos )
            { return m_record->HeaderSetPosNextRecord( pos ); }
        virtual CPQDIF_E_Collection * GetMainCollection( void ) const
            { return m_record->GetMainCollection(); }
        virtual bool SetMainCollection( CPQDIF_E_Collection * collMain )
            { return m_record->SetMainCollection( collMain ); }
        virtual bool GetChanged( void )
            { return m_record->GetChanged(); }
        virtual void SetChanged( bool changed )
            { m_record->SetChanged( changed ); }

	//  Private functions
    private:
        long _GenerateSeriesCount
            ( 
            UINT4               idStorageMethod, 
            CPQDIF_E_Vector *   pvectSeriesArray
            );
        double * _GenerateSeriesData
            ( 
            int                 idxChannelDefn,
            UINT4               idStorageMethod, 
            double              rBaseValue, 
            double              rScale, 
            double              rOffset, 
            CPQDIF_E_Vector *   pvectSeriesArray, 
            long&               countPoints
            );
        bool CPQDIF_R_Observation::_GenerateTimeStampArray
            (
            TIMESTAMPPQDIF *    result, 
            double *            seconds, 
            long                countPoints 
            );
        bool _GetCalibrationRatio
            (
            int     idxChannelDefn,
            double& rScale
            );

    //  Local data
    private:
        //  Reference to the data source which owns this observation.
        CPQDIF_R_DataSource *   m_pds;

        //  Reference to the monitor settings record (can be NULL)
        CPQDIF_R_Settings *     m_psett;

        //  Reference to the record which contains the observation data.
        CPQDIF_R_General *		m_record;	//	need to fix
    };

