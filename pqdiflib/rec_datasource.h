/*
**  Class:              CPQDIF_R_DataSource
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

class CPQDIF_R_DataSource : public CPQDIF_R_General
    {
    public:
        CPQDIF_R_DataSource();
        //CPQDIF_R_DataSource( CPQDIFRecord& record );
        virtual ~CPQDIF_R_DataSource();

    // Operations
    public:

        // Attributes
    public:
        //  Read functions
        bool GetInfo
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
            );
        long GetCountChannelDefns( void );
        long GetCountSeriesDefns( int idxChannelDefn );
        bool GetChannelDefnInfo
            (
            long        idxChannelDefn,
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
        bool GetSeriesDefnInfo
            (
            long     idxChannelDefn,
            long     idxSeriesDefn,
            UINT4&   idQuantityUnits,
            GUID&    idValueType,
            GUID&    idQuantityCharacteristic,
            UINT4&   idStorageMethod
            );

        //  Write functions
        long AddChannelDefn
            (
            const   char *  name,
                    UINT4   idPhase,
            const   GUID&   idQuantityType
            );
        long AddChannelDefn2
            (
            const   char *  name,
                    UINT4   idPhase,
                    UINT4   idQM,
            const   GUID&   idQuantityType
            );
        long AddSeriesDefn
            (
                    long     idxChannelDefn,
                    UINT4    idQuantityUnits,
            const   GUID     idValueType,
                    UINT4    idStorageMethod
            );

        long AddSeriesDefn2
            (
                    long     idxChannelDefn,
                    UINT4    idQuantityUnits,
            const   GUID     idValueType,
            const   GUID     idCharacteristicType,
                    UINT4    idStorageMethod
            );

        bool SetSeriesDefnNominal(long idxChannelDefn, long idxSeriesDefn, double   dNominal);
        bool GetSeriesDefnNominal(long idxChannelDefn, long idxSeriesDefn, double & dNominal);
		bool GetSeriesDefnPrecisionAndResolution
            (
            long        idxChannel,
            long        idxSeries,
            UINT4     & uPrecision,
            double    & dResolution
            );

        bool SetSeriesDefnPrefix(long idxChannelDefn, long idxSeriesDefn, long idPrefix);
        bool SetSeriesDefnDisplay(long idxChannelDefn, long idxSeriesDefn, long idDisplay);
        bool SetSeriesDefnUnits(long idxChannelDefn, long idxSeriesDefn, long idUnits);
        bool SetSeriesDefnResolution(long idxChannelDefn, long idxSeriesDefn, double dRes);
        bool SetSeriesDefnDigits(long idxChannelDefn, long idxSeriesDefn, long idDigits);

        bool SetEffective (const TIMESTAMPPQDIF& timeEffective)
            {
            return SetTimeInMainCollection(tagEffective, timeEffective);
            }
        bool GetEffective (TIMESTAMPPQDIF& timeEffective)
            {
            return GetTimeInMainCollection(tagEffective, timeEffective);
            }



    //  Internal functions
    public:
        CPQDIF_E_Collection * GetChannelDefns( void );
        CPQDIF_E_Collection * GetOneChannelDefn
            ( 
            long    idxChannelDefn
            );
        CPQDIF_E_Collection * GetSeriesDefns
            ( 
            CPQDIF_E_Collection * pcolChannelDefns
            );
        CPQDIF_E_Collection * GetOneSeriesDefn
            ( 
            CPQDIF_E_Collection *   pcolChannelSeriesDefns,
            long                    idxSeriesDefn
            );
        CPQDIF_E_Collection * GetOneSeriesDefn
            ( 
            long    idxChannelDefn,
            long    idxSeriesDefn
            );

    //  Local data
    private:
        
    };

