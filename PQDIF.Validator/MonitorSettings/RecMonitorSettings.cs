/*
**  Class:          PQDIF.Validator.RecMonitorSettings
**  Description:	Optional Record-level tag which identifies a set of configuration parameters
**
** --------------------------------------------------------------------------
**
** Copyright 2025 PQDIF Authors
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

// Contains fundamental classes and base classes that define commonly-used value and reference data types, events and event handlers, interfaces, attributes, and processing exceptions.
using System;

// Contains interfaces and classes that define generic collections, which allow users to create strongly typed collections that provide better type safety and performance than non-generic strongly typed collections.
using System.Collections.Generic;

// Defines IEEE PQDIF data types including PQDIF timestamps
using PQDIFNet;

// Defines IEEE PQDIF IDs and data types 
using static PQDIFNet.Constants.Logical;

// For logging errors, warnings, other messages.
using PQDIF.Log;

namespace PQDIF.Validator
{
    /// <summary>
    /// Optional Record-level tag which identifies a set of configuration parameters
    /// </summary>
    public class RecMonitorSettings
    {
        /// <summary>
        /// The time that these settings become effective
        /// </summary>
        /// <remarks>Required</remarks>
        public DateTime Effective = new DateTime();

        /// <summary>
        /// The time when the monitor was installed
        /// </summary>
        /// <remarks>Required</remarks>
        public DateTime TimeInstalled = new DateTime();

        /// <summary>
        /// The time when the monitor was removed
        /// </summary>
        public DateTime? TimeRemoved = null;

        /// <summary>
        /// If <c>true</c>, the calibration adjustments should be applied to the series data before using. Otherwise, the data is for informative use only.
        /// </summary>
        /// <remarks>Required</remarks>
        public bool UseCalibration = false;

        /// <summary>
        /// If <c>true</c>, the transducer adjustments should be applied to the series data before using. Otherwise, the data is for informative use only.
        /// </summary>
        /// <remarks>Required</remarks>
        public bool UseTransducer;

        /// <summary>
        /// Channel specific monitor settings
        /// </summary>
        public List<OneChannelSetting> ChannelSettingsArray = new List<OneChannelSetting>();

        /// <summary>
        /// Nominal power system frequency for this instrument in Hertz
        /// </summary>
        public double? NominalFrequency = null;

        /// <summary>
        /// Identifies the physical connection of the instrumentation or instrument transducers
        /// </summary>
        public uint? SettingPhysicalConnection = null;

        /// <summary>
        /// Arbitrary comments
        /// </summary>
        public string Comments = "";

        /// <summary>
        /// Specifies the number of seconds to add to tag values and/or series values that store TIMESTAMP types in a Data Source, 
        /// Monitor Settings, or Observation record from Local Standard Time (LST) to Local Time. If <see cref="LSTtoLocal"/> is used without tagUTCtoLST, 
        /// then tagUTCtoLST shall be 0.
        /// </summary>
        public double? LSTtoLocal = null;

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="RecMonitorSettings"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="RecMonitorSettings"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="RecMonitorSettings"/> class.
        /// </summary>
        public RecMonitorSettings()
        {
            // Default Constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RecMonitorSettings"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a container record</param>
        /// <param name="PointerCollection">Pointer to the Collection that Contains a Data Source Record in <paramref name="PqdifFile"/></param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public RecMonitorSettings(CPQDIFNet PqdifFile, ref IntPtr PointerCollection, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
        {
            // Initialize the return value
            RecordOK = true;

            // Set up an error handler
            try
            {
                // Set the logger level
                LoggerApplication.LogLevel = LogLevelApplication;
                LoggerCompliance.LogLevel = LogLevelCompliance;

                // Initialize flags that tell us if required tags are missing
                bool MissingEffective = true;
                bool MissingTimeInstalled = true;
                bool MissingUseCalibration = true;
                bool MissingUseTransducer = true;
                bool MissingChannelSettingsArray = true;


                // Determine the number of tags associated with the current record
                int CollectionCount = 0;
                bool Result = PqdifFile.CollectionGetCount(PointerCollection, ref CollectionCount);
                if (!Result)
                {
                    LoggerApplication.Log("CollectionGetCount Error", LogLevels.Error);
                    RecordOK = false;
                    return;
                }

                // For each tag in the data source...
                for (int CollectionIndex = 0; CollectionIndex < CollectionCount; CollectionIndex++)
                {
                    // Create a log message for this collection index.
                    string CollectionLogMessagePrefix = "Collection Index " + CollectionIndex.ToString();

                    // Get the pointer to the current tag
                    IntPtr PointerCollectionTag = new IntPtr();
                    Result = PqdifFile.CollectionGetEntry(PointerCollection, CollectionIndex, ref PointerCollectionTag);
                    if (!Result)
                    {
                        LoggerApplication.Log(CollectionLogMessagePrefix + ": CollectionGetEntry", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Get the tag ID and tag name
                    Guid TagID = new Guid();
                    string TagName = "";
                    Result = PqdifFile.ElementGetTag(PointerCollectionTag, ref TagID, ref TagName);
                    if (!Result)
                    {
                        LoggerApplication.Log(CollectionLogMessagePrefix + ": ElementGetTag", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Initialize a flag that tells us that the current TagID was not matched to a known TagID.
                    bool TagMatched = false;

                    // Try to match the tag to tagEffective
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagEffective, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref Effective);
                        if (TagMatched)
                            MissingEffective = false;
                    }

                    // Try to match the tag to tagTimeInstalled
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTimeInstalled, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref TimeInstalled);
                        if (TagMatched)
                            MissingTimeInstalled = false;
                    }

                    // Try to match the tag to tagTimeRemoved
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTimeRemoved, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref TimeRemoved);

                    // Try to match the tag to tagUseCalibration
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagUseCalibration, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref UseCalibration);
                        if (TagMatched)
                            MissingUseCalibration = false;
                    }

                    // Try to match the tag to tagUseTransducer
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagUseTransducer, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref UseTransducer);
                        if (TagMatched)
                            MissingUseTransducer = false;
                    }

                    // Try to match the tag to other optional tags
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagNominalFrequency, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref NominalFrequency);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagSettingPhysicalConnection, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref SettingPhysicalConnection);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagComments, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref Comments);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagLSTtoLocal, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref LSTtoLocal);


                    // If this tag is for a Channel Settings collection...
                    if (TagID == tagChannelSettingsArray)
                    {

                        // We matched the tag.
                        TagMatched = true;

                        // We have found the required channel settings collection
                        MissingChannelSettingsArray = false;

                        // Copy the channel definitions pointer
                        IntPtr ChannelSettingsPointer = PointerCollectionTag;

                        // Determine if ChannelSettingsPointer points to a collection and the count of elements associated with that collection.
                        int ChannelSettingCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, ChannelSettingsPointer, LoggerApplication, LoggerCompliance);

                        // Log a a compliance error if the series settig collection does not have at least one series definition.
                        if (ChannelSettingCount == 0)
                            LoggerCompliance.Log(CollectionLogMessagePrefix + ": tagChannelSettingsArray Must have at least tagOneChannelSetting Record.", LogLevels.Error);

                        // For each channel setting...
                        for (int ChannelSettingsIndex = 0; ChannelSettingsIndex < ChannelSettingCount; ChannelSettingsIndex++)
                        {
                            // Create a log message for this channel setting.
                            string ChannelSettingLogMessagePrefix = CollectionLogMessagePrefix + ": ChannelSetting Index " + ChannelSettingsIndex.ToString();

                            // Get the pointer to the current channel definition
                            IntPtr ChannelSettingPointer = new IntPtr();
                            Result = PqdifFile.CollectionGetEntry(ChannelSettingsPointer, ChannelSettingsIndex, ref ChannelSettingPointer);

                            // If we could get a pointer to the current channel definition...
                            if (Result)
                            {
                                // Create the current channel setting record.
                                OneChannelSetting ThisChannelSetting = new OneChannelSetting(PqdifFile, ChannelSettingPointer, LogLevelApplication, LogLevelCompliance, out RecordOK);

                                // If we created the channel setting, then add it to the list of channel settings.
                                if (RecordOK)
                                    ChannelSettingsArray.Add(ThisChannelSetting);
                            }
                            else
                            {
                                LoggerApplication.Log(ChannelSettingLogMessagePrefix + ": CollectionGetEntry Error", LogLevels.Error);
                            }

                        } // For each channel setting

                    } // If this tag is for a Channel Settings collection

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(CollectionLogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the data source


                // Log warnings if required tags are missing.
                if (MissingEffective)
                    LoggerCompliance.Log("Required tagTagEffective is Missing.", LogLevels.Error);
                if (MissingTimeInstalled)
                    LoggerCompliance.Log("Required tagTimeInstalled is Missing.", LogLevels.Error);
                if (MissingUseCalibration)
                    LoggerCompliance.Log("Required tagUseCalibration is Missing.", LogLevels.Error);
                if (MissingUseTransducer)
                    LoggerCompliance.Log("Required tagUseTransducer is Missing.", LogLevels.Error);
                if (MissingChannelSettingsArray)
                    LoggerCompliance.Log("Required tagChannelSettingsArray is Missing.", LogLevels.Error);

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // RecMonitorSettings


        /// <summary>
        /// Returns the <see cref="RecMonitorSettings"/> Collection Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Monitor Settings Record Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagRecMonitorSettings>\r\n");

            // If we should return application logs...
            if (ApplicationLog)
            {
                StringBuilder.Append("<ApplicationLog>\r\n");
                StringBuilder.Append(LoggerApplication.GetLogXML());
                StringBuilder.Append("</ApplicationLog>\r\n");
            }

            // If we should return compliance logs...
            if (ComplianceLog)
            {
                StringBuilder.Append("<ComplianceLog>\r\n");
                StringBuilder.Append(LoggerCompliance.GetLogXML());
                StringBuilder.Append("</ComplianceLog>\r\n");
            }

            // Export the tags of the Monitor Settings Record collection as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagEffective, Effective) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagTimeInstalled, TimeInstalled) + "\r\n");

            if (TimeRemoved != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTimeRemoved, (DateTime)TimeRemoved) + "\r\n");

            StringBuilder.Append(Utilities.XML.GetElement(tagUseCalibration, UseCalibration) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagUseTransducer, UseTransducer) + "\r\n");

            StringBuilder.Append("<tagChannelSettingsArray>\r\n");
            foreach (OneChannelSetting ThisChannelSetting in ChannelSettingsArray)
                StringBuilder.Append(ThisChannelSetting.GetXML(ApplicationLog, ComplianceLog));
            StringBuilder.Append("</tagChannelSettingsArray>\r\n");


            if (NominalFrequency != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagNominalFrequency, (double)NominalFrequency) + "\r\n");

            if (SettingPhysicalConnection != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagSettingPhysicalConnection, (uint)SettingPhysicalConnection) + "\r\n");

            StringBuilder.Append(Utilities.XML.GetElement(tagComments, Comments) + "\r\n");

            if (LSTtoLocal != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagLSTtoLocal, LSTtoLocal) + "\r\n");

            // Finish the Monitor Settings record.
            StringBuilder.Append("</tagRecMonitorSettings>\r\n");

            // Return the XML version of the Monitor Settings
            return StringBuilder.ToString();

        } // GetXML

    } // class RecMonitorSettings

} // namespace PQDIF.Validator