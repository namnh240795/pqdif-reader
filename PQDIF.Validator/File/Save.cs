/*
**  Class:          PQDIF.Validator.File
**  Description:	Reads PQDIF files from a File to a List of PQDIF Objects
**
** --------------------------------------------------------------------------
**
** Copyright 2022-2023 PQDIF Authors
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

namespace PQDIF.Validator
{
    // Contains fundamental classes and base classes that define commonly-used value and reference data types, events and event handlers, interfaces, attributes, and processing exceptions.
    using System;

    // Defines IEEE PQDIF IDs and data types including PQDIF timestamps
    using PQDIFNet;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    // Defines IEEE PQDIF IDs and data types including PQDIF timestamps
    using static PQDIFNet.Constants.Logical;
    using static PQDIFNet.Constants.Element;

    /// <summary>
    /// For Reading and Writing PQDIF Objects in PQDIF Files
    /// </summary>
    public partial class File
    {
        /// <summary>
        /// Saves the the current <see cref="File"/>
        /// </summary>
        /// <param name="NewFileName">Path and File Name to which to Save the PQDIF Records and a New PQDIF File</param>
        /// <returns><c>true</c> if the PQDIF File was Saved Successfully</returns>
        public bool Save(string NewFileName)
        {
            return Save(NewFileName, false, "");
        }

        /// <summary>
        /// Saves the the current <see cref="File"/>
        /// </summary>
        /// <param name="NewFileName">Path and File Name to which to Save the PQDIF Records and a New PQDIF File</param>
        /// <param name="RemovePI">Set to <c>true</c> to remove Personal Information, which will set all strings to zero-length</param>
        /// <param name="NewNameDS">Optional Replacement Data Source Name</param>
        /// <returns><c>true</c> if the PQDIF File was Saved Successfully</returns>
        public bool Save(string NewFileName, bool RemovePI, string NewNameDS)
        {
            // Initialize the return value
            bool SavedOK = true;

            // Create a new PQDIF File
            CPQDIFNet NewPqdifFile = null;

            try
            {

                // The last data source name
                string NameDS = "";

                // Create a new PQDIF file.
                NewPqdifFile = new CPQDIFNet
                {
                    FlatFileName = NewFileName,
                };

                // Initialize a record index.
                int RecIndex = 0;

                // For each object...
                foreach (RecordHolder ThisRecordHolder in RecordHolders)
                {

                    // If this is a container...
                    if (ThisRecordHolder.RecType == RecType.Container)
                    {
                        // Update the user
                        LoggerApplication.Log(RecIndex.ToString() + ": Adding Container", LogLevels.Info);

                        // Get a reference to the container.
                        Container Container = (Container)ThisRecordHolder.Record;

                        // Create a container record                        
                        double TimeStamp = DateTime.Now.ToOADate();
                        NewPqdifFile.RecordCreateContainer3(NewFileName, TimeStamp, (int)Container.VersionInfo[0], (int)Container.VersionInfo[3], (int)Container.VersionInfo[2], (int)Container.VersionInfo[3]);

                        // If we do not need to remove personal information, then set the container info.
                        if (!RemovePI)
                            NewPqdifFile.ContainerSetInfo(Container.Language, Container.Title, Container.Subject, Container.Author, Container.Keywords, Container.Comments, Container.LastSavedBy, Container.Application, Container.Security, Container.Owner, Container.Copyright, Container.Trademarks, Container.Notes);

                        // Set the compression style
                        if (Container.CompressionStyleID != null)
                            NewPqdifFile.CompressionStyle = (int)Container.CompressionStyleID;
                        if (Container.CompressionAlgorithmID != null)
                            NewPqdifFile.CompressionAlgorithm = (int)Container.CompressionAlgorithmID;

                    }

                    // If this is a data source record...
                    if (ThisRecordHolder.RecType == RecType.RecDataSource)
                    {
                        // Update the user
                        LoggerApplication.Log(RecIndex.ToString() + ": Adding Data Source Record", LogLevels.Info);

                        // Get a reference to the current data source record.
                        RecDataSource ThisDataSource = (RecDataSource)ThisRecordHolder.Record;

                        // Create the data source record
                        int DataSourceIdx = 0;

                        // Determine the data source name.
                        NameDS = ThisDataSource.NameDS;

                        // If we need to save the data source with a new name...
                        if (NewNameDS.Length > 0)
                            NameDS = NewNameDS;

                        // Otherwise, if we need to remove personal information...
                        else if (RemovePI)
                            NameDS = "";

                        // If we need to remove personal information...
                        if (RemovePI)
                            DataSourceIdx = NewPqdifFile.RecordCreateDataSource2(RecIndex, ThisDataSource.DataSourceTypeID, ThisDataSource.VendorID, ThisDataSource.EquipmentID, "", "", NameDS, "", "", "");
                        else
                            DataSourceIdx = NewPqdifFile.RecordCreateDataSource2(RecIndex, ThisDataSource.DataSourceTypeID, ThisDataSource.VendorID, ThisDataSource.EquipmentID, ThisDataSource.SerialNumberDS, ThisDataSource.VersionDS, NameDS, ThisDataSource.OwnerDS, ThisDataSource.LocationDS, ThisDataSource.TimeZoneDS);

                        // Get a handle to the new data source record
                        IntPtr DataSourcePtr = new IntPtr();
                        NewPqdifFile.RecordRequestDataSource2(DataSourceIdx, ref DataSourcePtr);

                        // For each channel definition...
                        foreach (OneChannelDefn ThisChannelDefn in ThisDataSource.ChannelDefns)
                        {
                            // Determine the channel name.
                            string ChannelName = ThisChannelDefn.ChannelName;
                            if (RemovePI)
                                ChannelName = "";

                            // Add this channel definition
                            int ChannelDefnIdx = NewPqdifFile.DataSourceAddChannelDefn3(DataSourcePtr, ChannelName, (int)ThisChannelDefn.PhaseID, (int)ThisChannelDefn.QuantityMeasuredID, ThisChannelDefn.QuantityTypeID);

                            // For each series definition...
                            foreach (OneSeriesDefn ThisSeriesDefn in ThisChannelDefn.SeriesDefns)
                            {
                                // Add the series definitions for this channel
                                int SeriesDefnIdx = NewPqdifFile.DataSourceAddSeriesDefn2(DataSourcePtr, ChannelDefnIdx, (int)ThisSeriesDefn.QuantityUnitsID, ThisSeriesDefn.ValueTypeID, ThisSeriesDefn.QuantityCharacteristicID, (int)ThisSeriesDefn.StorageMethodID);

                                // Add optional series definition tags if not null.
                                if (ThisSeriesDefn.SeriesNominalQuantity != null)
                                    NewPqdifFile.DataSourceSetSeriesDefnNominal(DataSourcePtr, ChannelDefnIdx, SeriesDefnIdx, (double)ThisSeriesDefn.SeriesNominalQuantity);

                                if (ThisSeriesDefn.QuantitySignificantDigitsID != null)
                                    NewPqdifFile.DataSourceSetSeriesDefnDigits(DataSourcePtr, ChannelDefnIdx, SeriesDefnIdx, (int)ThisSeriesDefn.QuantitySignificantDigitsID);

                                if (ThisSeriesDefn.HintDefaultDisplayID != null)
                                    NewPqdifFile.DataSourceSetSeriesDefnDisplay(DataSourcePtr, ChannelDefnIdx, SeriesDefnIdx, (int)ThisSeriesDefn.HintDefaultDisplayID);

                                if (ThisSeriesDefn.HintGreekPrefixID != null)
                                    NewPqdifFile.DataSourceSetSeriesDefnPrefix(DataSourcePtr, ChannelDefnIdx, SeriesDefnIdx, (int)ThisSeriesDefn.HintGreekPrefixID);

                                if (ThisSeriesDefn.QuantityResolutionID != null)
                                    NewPqdifFile.DataSourceSetSeriesDefnResolution(DataSourcePtr, ChannelDefnIdx, SeriesDefnIdx, (int)ThisSeriesDefn.QuantityResolutionID);

                            } // For each series definition

                        } // For each channel definition

                        // Release the data source pointer
                        NewPqdifFile.RecordReleaseDataSource2(DataSourcePtr);

                    } // If this is a data source record


                    // If this is a Monitor Settings Record...
                    if (ThisRecordHolder.RecType == RecType.RecMonitorSettings)
                    {
                        // Update the user
                        LoggerApplication.Log(RecIndex.ToString() + ": Adding Monitor Settings Record", LogLevels.Info);

                        // Get a reference to the current monitor settings record.
                        RecMonitorSettings ThisMonitorSettings = (RecMonitorSettings)ThisRecordHolder.Record;

                        // Create a new monitor settings record
                        int SettingsIdx = NewPqdifFile.RecordCreateSettings2(RecIndex);

                        // Get the handle to the new monitor settings record
                        IntPtr SettingsPtr = new IntPtr();
                        NewPqdifFile.RecordRequestSettings2(SettingsIdx, ref SettingsPtr);

                        NewPqdifFile.SettingsSetEffective(SettingsPtr, ThisMonitorSettings.Effective.ToOADate());
                        NewPqdifFile.SettingsSetInstalled(SettingsPtr, ThisMonitorSettings.TimeInstalled.ToOADate());

                        if (ThisMonitorSettings.TimeRemoved != null)
                            NewPqdifFile.SettingsSetRemoved(SettingsPtr, ((DateTime)ThisMonitorSettings.TimeRemoved).ToOADate());

                        NewPqdifFile.SettingsSetUseCalibration(SettingsPtr, ThisMonitorSettings.UseCalibration);
                        NewPqdifFile.SettingsSetUseTransducer(SettingsPtr, ThisMonitorSettings.UseTransducer);

                        // If the optional nominal frequency is numeric, then store it.
                        if (ThisMonitorSettings.NominalFrequency != null)
                            NewPqdifFile.SettingsSetNominalFrequency(SettingsPtr, (double)ThisMonitorSettings.NominalFrequency);

                        // For each channel setting...
                        for (int ChannelSettingIndex = 0; ChannelSettingIndex < ThisMonitorSettings.ChannelSettingsArray.Count; ChannelSettingIndex++)
                        {
                            // If the optional tagTriggerTypeID is not null...
                            if (ThisMonitorSettings.ChannelSettingsArray[ChannelSettingIndex].TriggerTypeID != null)
                                NewPqdifFile.SettingsAddChannel2(SettingsPtr,
                                                                (int)ThisMonitorSettings.ChannelSettingsArray[ChannelSettingIndex].ChannelDefnIdx,
                                                                (int)ThisMonitorSettings.ChannelSettingsArray[ChannelSettingIndex].TriggerTypeID);

                            // Otherwise, add just the required tagChannelDefnIdx
                            else
                                NewPqdifFile.SettingsAddChannel(SettingsPtr,
                                                               (int)ThisMonitorSettings.ChannelSettingsArray[ChannelSettingIndex].ChannelDefnIdx);

                        } // For each channel setting.

                        // Add the required tagEffective timestamp
                        PQDIFNetWrapper.CollectionAddTag(NewPqdifFile, RecIndex, tagRecDataSource, tagEffective, ID_ELEMENT_TYPE_SCALAR, ThisMonitorSettings.Effective, LoggerApplication, LoggerCompliance, RecIndex.ToString() + ": Adding tagEffective");

                    } // If this is a Monitor Settings Record


                    // If this is an observation record...
                    if (ThisRecordHolder.RecType == RecType.RecObservation)
                    {
                        // Update the user
                        LoggerApplication.Log(RecIndex.ToString() + ": Adding Observation Record", LogLevels.Info);

                        // Get a reference to this observation record.
                        RecObservation ThisObservation = (RecObservation)ThisRecordHolder.Record;

                        // Initialize the optional trigger method.
                        int TriggerMethodID = ID_TRIGGER_METH_NONE;
                        if (ThisObservation.TriggerMethodID != 0)
                            TriggerMethodID = (int)ThisObservation.TriggerMethodID;

                        // Initialize the optional triggered time.
                        double TimeTriggered = 0;
                        if (ThisObservation.TimeTriggered != null)
                            TimeTriggered = (double)((DateTime)ThisObservation.TimeTriggered).ToOADate();

                        // Get the observation name
                        string ObservationName = ThisObservation.ObservationName;

                        // If we need to remove personal information, then construct a new observation name using the last data source name saved and the start time.
                        if (RemovePI)
                            ObservationName = NameDS + " " + ThisObservation.TimeStart.ToString();


                        // Create an Observation Record.
                        int ObservationIdx = 0;
                        if (ThisObservation.ChannelTriggerIdx != null)
                            ObservationIdx = NewPqdifFile.RecordCreateObservation2(RecIndex, ObservationName, ThisObservation.TimeCreate.ToOADate(),
                                                                                   ThisObservation.TimeStart.ToOADate(), TriggerMethodID,
                                                                                   TimeTriggered, ThisObservation.ChannelTriggerIdx);

                        else
                            ObservationIdx = NewPqdifFile.RecordCreateObservation3(RecIndex, ObservationName, ThisObservation.TimeCreate.ToOADate(), ThisObservation.TimeStart.ToOADate(), TriggerMethodID);

                        // Get the handle to the new observation record
                        IntPtr ObservationPtr = new IntPtr();
                        NewPqdifFile.RecordRequestObservation2(ObservationIdx, ref ObservationPtr);

                        // For each channel instance...
                        foreach (OneChannelInst ThisChannelInst in ThisObservation.ChannelInstances)
                        {
                            // Add a channel definition.
                            int ChannelInstanceIdx = NewPqdifFile.ObservationAddChannel2(ObservationPtr, (int)ThisChannelInst.ChannelDefnIdx);

                            // If possible, set the channel frequency.
                            if (ThisChannelInst.ChannelFrequency != null)
                                NewPqdifFile.ObservationSetChannelFreq(ObservationPtr, ChannelInstanceIdx, (double)ThisChannelInst.ChannelFrequency);

                            // If possible, set the channel group ID.
                            if (ThisChannelInst.ChannelFrequency != null)
                                PQDIFNetWrapper.CollectionAddTag(NewPqdifFile, RecIndex, tagChannelInstances, tagOneChannelInst,
                                                                 tagChannelGroupID, ID_ELEMENT_TYPE_SCALAR, (double)ThisChannelInst.ChannelFrequency,
                                                                 LoggerApplication, LoggerCompliance, "Adding tagChannelFrequency");

                            // For each series instance...
                            foreach (OneSeriesInstance ThisSeriesInstance in ThisChannelInst.SeriesInstances)
                            {
                                // If the series values are valid...
                                if (ThisSeriesInstance.SeriesValues != null)
                                {
                                    // Add the series data
                                    int SeriesInstanceIdx = NewPqdifFile.ObservationAddSeriesData(ObservationPtr, ChannelInstanceIdx, ThisSeriesInstance.SeriesValues);

                                    // If the scale and offset are numeric...
                                    if (Utilities.Information.IsNumeric(ThisSeriesInstance.SeriesScale) && Utilities.Information.IsNumeric(ThisSeriesInstance.SeriesOffset))

                                        // Set the series scale, offset, and nominal base. Note that these calls must occur *after* the call to ObservationAddSeriesData because the series does not exist until then.
                                        NewPqdifFile.ObservationSetSeriesScale(ObservationPtr, ChannelInstanceIdx, SeriesInstanceIdx, (double)ThisSeriesInstance.SeriesScale, (double)ThisSeriesInstance.SeriesOffset);

                                    // If the optional series base quantity is available, then store it.
                                    if (ThisSeriesInstance.SeriesBaseQuantity != null)
                                        NewPqdifFile.ObservationSetSeriesBaseQty2(ObservationPtr, ChannelInstanceIdx, SeriesInstanceIdx, (double)ThisSeriesInstance.SeriesBaseQuantity);

                                } // If the series values are valid

                            } // For each series instance

                        } // For each channel instance

                        // Release the observation record's pointer
                        if (!NewPqdifFile.RecordReleaseObservation2(ObservationPtr))
                            LoggerApplication.Log("RecordReleaseObservation2 Error", LogLevels.Error);

                    } // If this is an observation record

                    // Increment the record index
                    RecIndex++;

                } // For each object

            } // try

            // Log application errors.
            catch (Exception ex)
            {
                LoggerApplication.Log("Error: " + ex.Message, LogLevels.Error);
            }

            // Run this code on exit.
            finally
            {

                // Clean Up.
                try
                {
                    if (NewPqdifFile != null)
                    {
                        // Close the new PQDIF file and dispose of it.
                        NewPqdifFile.WriteNew();
                        NewPqdifFile.Close();
                        NewPqdifFile.Dispose();
                    }

                }
                catch
                {
                    // Ignore errors during clean up.
                }

            } // finally

            // Return the success flag.
            return SavedOK;

        } // Save

    } // class File

} // namespace PQDIF.Validator