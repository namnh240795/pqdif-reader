/*
**  Class:              CPQDIF_R_Settings
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

class CPQDIF_R_Settings : public CPQDIF_R_General
    {
    public:
        CPQDIF_R_Settings() {}
        virtual ~CPQDIF_R_Settings() ;

    // Operations
    public:

    // Attributes
    public:

    //  Read functions
    bool GetInfo
        (
        TIMESTAMPPQDIF&     timeEffective,
        TIMESTAMPPQDIF&     timeInstalled,
        TIMESTAMPPQDIF&     timeRemoved,  
        bool&               useCal,       
        bool&               useTrans      
        );

	bool CPQDIF_R_Settings::GetConnectionInfo
        (
        UINT4&  connectionType
        );

    long GetCountChannels( void );

    bool GetChannelInfo (long idxChannel, UINT4 & idxChannelDefn);

#ifdef zap
    //
    //
    //  Depricated function
    //
    bool GetChannelInfo
        (
        long                idxChannel,
        UINT4&              idxChannelDefn,
        UINT4&              triggerTypeID,
        REAL8&              fullScale,  
        REAL8&              noiseFloor, 
        REAL8&              triggerLow, 
        REAL8&              triggerHigh,
        REAL8&              triggerRate,
        CPQDIF_E_Vector&    triggerShapeParam   //  Array of [3]
        );
#endif

    bool GetChanTrans
        (
        long                idxChannel,
        UINT4&              xdTransformerTypeID,
        REAL8&              xdSystemSideRatio,  
        REAL8&              xdMonitorSideRatio,
        CPQDIF_E_Vector&    xdFrequencyResponse //  Array [n]
        );
    bool GetChanCal
        (
        long                idxChannel,
        REAL8&              calTimeSkew,  
        REAL8&              calOffset,  
        REAL8&              calRatio,  
        bool&               calMustUseARCal,
        CPQDIF_E_Vector&    calApplied,     //  Array [n]
        CPQDIF_E_Vector&    calRecorded     //  Array [n]
        );

    //  Write functions
    bool SetInfo
        (
        const   TIMESTAMPPQDIF&     timeEffective,
        const   TIMESTAMPPQDIF&     timeInstalled,
        const   TIMESTAMPPQDIF&     timeRemoved,  
                bool                useCal,       
                bool                useTrans      
        );


	bool CPQDIF_R_Settings::SetConnectionInfo
        (
        const   UINT4 connectionType
        );

#ifdef zap
    //
    //
    //  Depricated function
    //
    long AddChannel
        (
                UINT4               idxChannelDefn,
                UINT4               triggerTypeID,
                REAL8               fullScale,  
                REAL8               noiseFloor, 
                REAL8               triggerLow, 
                REAL8               triggerHigh,
                REAL8               triggerRate,
        const   CPQDIF_E_Vector *   triggerShapeParam   //  Array of [3] ... or NULL
        );
#endif
    long AddChannel
        (
        UINT4               idxChannel
        );
    long AddChannel
        (
        UINT4               idxChannel,
        UINT4               idTriggerType
        );



    bool SetChanTrans
        (
                UINT4               idxChannel,
                UINT4               xdTransformerTypeID,
                REAL8               xdSystemSideRatio,  
                REAL8               xdMonitorSideRatio,
        const   CPQDIF_E_Vector *   xdFrequencyResponse //  Array [n]... or NULL
        );
    bool SetChanCal
        (
                UINT4               idxChannel,
                REAL8               calTimeSkew,  
                REAL8               calOffset,  
                REAL8               calRatio,  
                bool                calMustUseARCal,
        const   CPQDIF_E_Vector *   calApplied,     //  Array [n]... or NULL
        const   CPQDIF_E_Vector *   calRecorded     //  Array [n]... or NULL
        );

    bool SetTriggerShapeParam
        (
        UINT4                     idxChannelDefn,
        const CPQDIF_E_Vector *   triggerShapeParam   //  Array of [3] ... or NULL
        ); 


    bool SetChannelReal8(UINT4 idxChannel, GUID tag, REAL8 dVal);

    bool SetTriggerLow(const UINT4 idxChannel, const REAL8 dVal)
        {
        return SetChannelReal8(idxChannel, tagTriggerLow, dVal);
        }

    bool SetTriggerHigh(const UINT4 idxChannel, const REAL8 dVal)
        {
        return SetChannelReal8(idxChannel, tagTriggerHigh, dVal);
        }

    bool SetTriggerRate(const UINT4 idxChannel, const REAL8 dVal)
        {
        return SetChannelReal8(idxChannel, tagTriggerRate, dVal);
        }

    bool SetFullScale(const UINT4 idxChannel, const REAL8 dVal)
        {
        return SetChannelReal8(idxChannel, tagFullScale, dVal);
        }

    bool SetNoiseFloor(const UINT4 idxChannel, const REAL8 dVal)
        {
        return SetChannelReal8(idxChannel, tagNoiseFloor, dVal);
        }
    
    bool SetEffective (const TIMESTAMPPQDIF& timeEffective)
        {
        return SetTimeInMainCollection(tagEffective, timeEffective);
        }

    bool SetInstalled (const TIMESTAMPPQDIF& timeInstalled)
        {
        return SetTimeInMainCollection(tagTimeInstalled, timeInstalled);
        }

    bool SetRemoved (const TIMESTAMPPQDIF& timeRemoved)
        {
        return SetTimeInMainCollection(tagTimeRemoved, timeRemoved);
        }

    bool SetNominalFrequency(const REAL8 dVal)
        {
        return SetREAL8InMainCollection(tagNominalFrequency, dVal);
        }

    bool SetUseCalibration(const BOOL4 bVal)
        {
        return SetBOOL4InMainCollection(tagUseCalibration, bVal);
        }

    bool SetUseTransducer(const BOOL4 bVal)
        {
        return SetBOOL4InMainCollection(tagUseTransducer, bVal);
        }


    bool GetChannelReal8(UINT4 idxChannel, GUID tag, REAL8 &dVal);

    bool GetTriggerLow(const UINT4 idxChannel, REAL8 &dVal)
        {
        return GetChannelReal8(idxChannel, tagTriggerLow, dVal);
        }

    bool GetTriggerHigh(const UINT4 idxChannel, REAL8 &dVal)
        {
        return GetChannelReal8(idxChannel, tagTriggerHigh, dVal);
        }

    bool GetTriggerRate(const UINT4 idxChannel, REAL8 &dVal)
        {
        return GetChannelReal8(idxChannel, tagTriggerRate, dVal);
        }

    bool GetFullScale(const UINT4 idxChannel, REAL8 &dVal)
        {
        return GetChannelReal8(idxChannel, tagFullScale, dVal);
        }

    bool GetNoiseFloor(const UINT4 idxChannel, REAL8 &dVal)
        {
        return GetChannelReal8(idxChannel, tagNoiseFloor, dVal);
        }

    bool GetEffective (TIMESTAMPPQDIF& timeEffective)
        {
        return GetTimeInMainCollection(tagEffective, timeEffective);
        }

    bool GetInstalled (TIMESTAMPPQDIF& timeInstalled)
        {
        return GetTimeInMainCollection(tagTimeInstalled, timeInstalled);
        }

    bool GetRemoved (TIMESTAMPPQDIF& timeRemoved)
        {
        return GetTimeInMainCollection(tagTimeRemoved, timeRemoved);
        }

    bool GetNominalFrequency(REAL8 &dVal)
        {
        return GetREAL8InMainCollection(tagNominalFrequency, dVal);
        }

    bool GetUseCalibration(BOOL4 &bVal)
        {
        return GetBOOL4InMainCollection(tagUseCalibration, bVal);
        }

    bool GetUseTransducer(BOOL4 &bVal)
        {
        return GetBOOL4InMainCollection(tagUseTransducer, bVal);
        }


    //  Internal functions
    public:
        CPQDIF_E_Collection * GetChannelSettings( void );
        CPQDIF_E_Collection * GetOneChannelSetting
            ( 
            long    idxChannelDefn
            );

    //  Local data
    private:
        //  None
    };

