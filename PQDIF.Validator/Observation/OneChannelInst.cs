/*
**  Class:          PQDIF.Validator.OneChannelInst
**  Description:	One of these collections per channel instance
**
** --------------------------------------------------------------------------
**
** Copyright 2022 PQDIF Authors
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

    // Contains interfaces and classes that define generic collections, which allow users to create strongly typed collections that provide better type safety and performance than non-generic strongly typed collections.
    using System.Collections.Generic;

    // Defines IEEE PQDIF data types including PQDIF timestamps
    using PQDIFNet;

    // Defines IEEE PQDIF IDs and data types 
    using static PQDIFNet.Constants.Logical;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    /// <summary>
    /// One of these collections per channel instance
    /// </summary>
    public class OneChannelInst
    {

        /// <summary>
        /// Specifies which of the available channel definitions this instance belongs to.  Index into tagChannelDefns collection
        /// of the matching data source record.
        /// </summary>
        /// <remarks>Required</remarks>
        public uint ChannelDefnIdx = 0;

        /// <summary>
        /// This collection shall contain the exact number of series which were defined for the specified channel definition
        /// </summary>
        /// <remarks>Required</remarks>
        public List<OneSeriesInstance> SeriesInstances = new List<OneSeriesInstance>();

        /// <summary>
        /// Simple characterization value: magnitude of disturbance (percent: 100%=nominal)
        /// </summary>
        public double? CharactMagnitude = null;

        /// <summary>
        /// Simple characterization value: frequency (Hertz)
        /// </summary>
        public double? CharactFrequency = null;

        /// <summary>
        /// Contains a 32-bit integer that represents module specific information related to the trigger reason
        /// </summary>
        public uint? ChanTriggerModuleInfo = null;

        /// <summary>
        /// Contains the name of a device specific code or hardware module, algorithm, or rule not necessarily channel based that caused this channel to be recorded
        /// </summary>
        public string ChanTriggerModuleName = "";

        /// <summary>
        /// Contains the name of the device involved in an external cross-trigger scenario
        /// </summary>
        public string CrossTriggerDeviceName = "";

        /// <summary>
        /// Contains the channel definition index of the channel that triggered in a cross-trigger scenario
        /// </summary>
        public uint? CrossTriggerChanIdx = null;

        /// <summary>
        /// Integer ID representing the trigger type for this channel instance.  Used only with type <see cref="ID_QT_VALUELOG"/> with a trigger method of channel.
        /// </summary>
        public uint? ChanTriggerTypeID =  null;

        /// <summary>
        /// For a channel which contains multiple instances to represent a sparse log of time-stamped frequency-domain information, 
        /// this specifies the frequency for which this channel instance applies (in Hertz). If not present, the channel characteristics
        /// are frequency independent unless further specified by the quantity characteristic
        /// </summary>
        public double? ChannelFrequency = null;

        /// <summary>
        /// For a channel which contains multiple instances to represent a sparse log of time-stamped frequency-domain information, 
        /// this specifies the frequency for which this channel instance applies (in Index).
        /// </summary>
        public int? ChannelGroupID = null;

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="OneChannelInst"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="OneChannelInst"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="OneChannelInst"/> class.
        /// </summary>
        public OneChannelInst()
        {
            // Default constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="OneSeriesInstance"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a container record</param>
        /// <param name="ChannelInstancePointer">Channel Instance Pointer</param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public OneChannelInst(CPQDIFNet PqdifFile, IntPtr ChannelInstancePointer, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
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
                bool MissingSeriesInstances = true;
                bool MissingChannelDefnIdx = true;


                // Determine if ChannelInstancePointer points to a collection and the count of elements associated with that collection.
                int ChannelInstanceCollectionCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, ChannelInstancePointer, LoggerApplication, LoggerCompliance);


                // For each tag in the Channel Instance collection...
                for (int ChannelCollectionIndex = 0; ChannelCollectionIndex < ChannelInstanceCollectionCount; ChannelCollectionIndex++)
                {
                    // Create a log message for this Channel Instance index.
                    string ChannelInstanceLogMessagePrefix = "Collection Index " + ChannelCollectionIndex.ToString();

                    // Get the pointer to the current tag
                    IntPtr TagPointer = new IntPtr();
                    bool Result = PqdifFile.CollectionGetEntry(ChannelInstancePointer, ChannelCollectionIndex, ref TagPointer);
                    if (!Result)
                    {
                        LoggerApplication.Log(ChannelInstanceLogMessagePrefix + ": CollectionGetEntry", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Get the tag ID and tag name
                    Guid TagID = new Guid();
                    string TagName = "";
                    Result = PqdifFile.ElementGetTag(TagPointer, ref TagID, ref TagName);
                    if (!Result)
                    {
                        LoggerApplication.Log(ChannelInstanceLogMessagePrefix + ": ElementGetTag", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Initialize a flag that tells us that the current TagID was not matched to a known TagID.
                    bool TagMatched = false;

                    // Try to match the required tagChannelDefnIdx.
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagChannelDefnIdx, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref ChannelDefnIdx);
                        if (TagMatched)
                            MissingChannelDefnIdx = false;
                    }

                    // Try to match the ID to other known tags of a Channel instance record and change the flag for required flag when we find them.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCharactMagnitude, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref CharactMagnitude);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCharactFrequency, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref CharactFrequency);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagChanTriggerModuleName, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix , ref ChanTriggerModuleName);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagCrossTriggerDeviceName, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref CrossTriggerDeviceName);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCrossTriggerChanIdx, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref CrossTriggerChanIdx);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagChanTriggerTypeID, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref ChanTriggerTypeID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagChannelFrequency, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref ChannelFrequency);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagChannelGroupID, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, ChannelInstanceLogMessagePrefix, ref ChannelGroupID);

                    // If this Tag is for a Series Instance collection...
                    if (TagID == tagSeriesInstances)
                    {
                        // We matched the tag.
                        TagMatched = true;

                        // We have found the required Series Instance collection
                        MissingSeriesInstances = false;

                        // Copy the Series Instances pointer
                        IntPtr SeriesInstancesPointer = TagPointer;

                        // Determine if SeriesInstancesPointer points to a collection and the count of elements associated with that collection.
                        int SeriesInstanceCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, SeriesInstancesPointer, LoggerApplication, LoggerCompliance);

                        // Log a a compliance error if the Series Instance collection does not have at least one Series Instance.
                        if (SeriesInstanceCount == 0)
                            LoggerCompliance.Log(ChannelInstanceLogMessagePrefix + ": tagSeriesInstances must have at least one tagOneSeriesInstance record.", LogLevels.Error);

                        // For each Series Instance...
                        for (int SeriesInstanceIndex = 0; SeriesInstanceIndex < SeriesInstanceCount; SeriesInstanceIndex++)
                        {
                            // Create a log message for this ChannelDefns index.
                            string SeriesDefnLogMessagePrefix = ChannelInstanceLogMessagePrefix + ": Series Instance Index " + SeriesInstanceIndex.ToString();

                            // Get the pointer to the current Series Instance
                            IntPtr SeriesInstancePointer = new IntPtr();
                            Result = PqdifFile.CollectionGetEntry(SeriesInstancesPointer, SeriesInstanceIndex, ref SeriesInstancePointer);

                            // If we could get a pointer to the current Series Instance...
                            if (Result)
                            {
                                // Create the current Series Instance record.
                                OneSeriesInstance ThisSeriesInstance = new OneSeriesInstance(PqdifFile, SeriesInstancePointer, LoggerApplication.LogLevel, LoggerCompliance.LogLevel, out RecordOK);

                                // If we created the Series Instance, then add it to the list of Series Instances.
                                if (RecordOK)
                                    SeriesInstances.Add(ThisSeriesInstance);
                            }
                            else
                            {
                                LoggerApplication.Log(SeriesDefnLogMessagePrefix + ": CollectionGetEntry Error ", LogLevels.Error);
                            }

                        } // For each Series Instance

                    } // If this Tag is for a Series Instance collection

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(ChannelInstanceLogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the Channel Instance collection


                // Log warnings if required tags are missing.
                if (MissingChannelDefnIdx)
                    LoggerCompliance.Log("Required tagChannelDefnIdx is Missing.", LogLevels.Error);
                if (MissingSeriesInstances)
                    LoggerCompliance.Log("Required tagSeriesInstances is Missing.", LogLevels.Error);

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // OneChannelInst


        /// <summary>
        /// Returns the <see cref="OneChannelInst" /> Collection Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>Channel Instance Collection Formated in XML</returns>
        public string GetXML(bool ApplicationLog, bool ComplianceLog, uint? MaxSeriesValues)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagOneChannelInst>\r\n");

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

            // Export the tags of the Channel Instance collection as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagChannelDefnIdx, ChannelDefnIdx) + "\r\n");

            StringBuilder.Append("<tagSeriesInstances>\r\n");
            foreach (OneSeriesInstance ThisSeriesInstance in SeriesInstances)
                StringBuilder.Append(ThisSeriesInstance.GetXML(ApplicationLog, ComplianceLog, MaxSeriesValues));
            StringBuilder.Append("</tagSeriesInstances>\r\n");


            if (CharactMagnitude != null)
            StringBuilder.Append(Utilities.XML.GetElement(tagCharactMagnitude, (double)CharactMagnitude) + "\r\n");

            if (CharactFrequency != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCharactFrequency, (double)CharactFrequency) + "\r\n");

            if (ChanTriggerModuleInfo != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagChanTriggerModuleInfo, (uint)ChanTriggerModuleInfo) + "\r\n");

            StringBuilder.Append(Utilities.XML.GetElement(tagChanTriggerModuleName, ChanTriggerModuleName) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagCrossTriggerDeviceName, CrossTriggerDeviceName) + "\r\n");

            if (CrossTriggerChanIdx != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCrossTriggerChanIdx, (uint)CrossTriggerChanIdx) + "\r\n");

            if (ChanTriggerTypeID !=null)
                StringBuilder.Append(Utilities.XML.GetElement(tagChanTriggerTypeID, (uint)ChanTriggerTypeID) + "\r\n");

            if (ChannelFrequency != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagChannelFrequency, (uint)ChannelFrequency) + "\r\n");

            if (ChannelGroupID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagChannelGroupID, (uint)ChannelGroupID) + "\r\n");

            // Finish the Channel Instance record.
            StringBuilder.Append("</tagOneChannelInst>\r\n");

            // Return the XML version of the Channel Instance
            return StringBuilder.ToString();

        } // GetXML


        /// <summary>
        /// Gets the Earliest and Latest Time Stamps Found in the <see cref="SeriesInstances"/> of this <see cref="OneChannelInst"/>
        /// </summary>
        /// <param name="RecObservation">PQDIF Observation Record for this <see cref="OneChannelInst"/></param>
        /// <param name="ChannelDefn">Channel Definition for this <see cref="OneChannelInst"/></param>
        /// <param name="TimeStampMin">Earliest Time Stamp Found in the <see cref="SeriesInstances"/> of <paramref name="RecObservation"/></param>
        /// <param name="TimeStampMax">Latest Time Stamp Found in the <see cref="SeriesInstances"/> of <paramref name="RecObservation"/></param>
        /// <returns><c>true</c> if min/max timestamps were found</returns>
        public bool GetTimeStartMinMax(RecObservation RecObservation, OneChannelDefn ChannelDefn, ref DateTime TimeStampMin, ref DateTime TimeStampMax)
        {
            // Initialize the return values
            bool Result = false;
            TimeStampMin = DateTime.MaxValue;
            TimeStampMax = DateTime.MinValue;

            // Find the series definition that contains time.
            int SeriesDefnIdxTime = -1;
            uint StorageMethodIdTime = 0;
            for (int SeriesDefnIdx = 0; SeriesDefnIdx < ChannelDefn.SeriesDefns.Count; SeriesDefnIdx++)
                if (ChannelDefn.SeriesDefns[SeriesDefnIdx].ValueTypeID == ID_SERIES_VALUE_TYPE_TIME)
                {
                    // Remember this first series definition index and stop searching.
                    SeriesDefnIdxTime = SeriesDefnIdx;
                    StorageMethodIdTime = ChannelDefn.SeriesDefns[SeriesDefnIdx].StorageMethodID;
                    break;
                }

            // If we found a series definition for time, and the series definition corresponds to a channel instance...
            if ((SeriesDefnIdxTime >= 0) && (SeriesDefnIdxTime < SeriesInstances.Count))
            {
                // If we have a valid number of series values...
                if ((SeriesInstances[SeriesDefnIdxTime].SeriesValues != null) && (SeriesInstances[SeriesDefnIdxTime].SeriesValues.Length > 0))
                {
                    // Get an array with the offset of time in seconds
                    double[] StartTimes = GetSeriesInstance(RecObservation, SeriesInstances[SeriesDefnIdxTime]).GetSeriesValues(StorageMethodIdTime);

                    // Get the current time stamp.
                    DateTime ThisTimeStamp = RecObservation.TimeStart.AddSeconds(StartTimes[0]);

                    // Check for a new timestamp min/max based on the first time stamp of the series instance.
                    if (TimeStampMin > ThisTimeStamp)
                        TimeStampMin = ThisTimeStamp;

                    if (TimeStampMax < ThisTimeStamp)
                        TimeStampMax = ThisTimeStamp;

                    // Check for a new timestamp min/max based on the last time stamp of the series instance.
                    ThisTimeStamp = RecObservation.TimeStart.AddSeconds(StartTimes[StartTimes.Length - 1]);

                    if (TimeStampMin > ThisTimeStamp)
                        TimeStampMin = ThisTimeStamp;

                    if (TimeStampMax < ThisTimeStamp)
                        TimeStampMax = ThisTimeStamp;                    

                    // Here if we found a time stamp series.
                    Result = true;

                } // If we have a valid number of series values               

            } // If we found a series definition for time, and the series definition corresponds to a channel instance

            // Return true if the min/max time stamps were found.
            return Result;

        } // GetTimeStartMinMax


        /// <summary>
        /// Returns a <see cref="OneSeriesInstance"/> that we should use for <see cref="OneSeriesInstance.SeriesValues"/>, checking <see cref="OneSeriesInstance.SeriesShareChannelIdx"/> 
        /// and <see cref="OneSeriesInstance.SeriesShareSeriesIdx"/> for the need to use a shared <see cref="OneSeriesInstance"/> 
        /// </summary>
        /// <param name="RecObservation">Record Observation</param>
        /// <param name="SeriesInstance">Series Instance in <paramref name="RecObservation"/></param>
        /// <returns>Series Instance to use for Series Samples</returns>
        private OneSeriesInstance GetSeriesInstance(RecObservation RecObservation, OneSeriesInstance SeriesInstance)
        {
            // Initialize the return series instance
            OneSeriesInstance RetSeriesInstance = SeriesInstance;

            // If the series values are missing, but we should use a series instance of another channel instance...
            if ((SeriesInstance.SeriesValues == null) && (SeriesInstance.SeriesShareChannelIdx != null) && (SeriesInstance.SeriesShareSeriesIdx != null))
            {
                // From IEEE Std 1159.3-2019, Clause 5.5.3 Shared series
                //
                // For a normal series, a tagSeriesValues vector is required. However, to save space, sharing data from another series is an option.
                // In this case, the tagSeriesValues tag is replaced by two tags: tagSeriesShareChannelIdx and tagSeriesShareSeriesIdx. These tags
                // indicate a “master” normal series (e.g., one that has a tagSeriesValues tag) which has the designated data to be shared.
                // These tags (tagSeriesShareChannelIdx and tagSeriesShareSeriesIdx) are indices into collections within the current observation.
                int SeriesShareChannelIdx = (int)SeriesInstance.SeriesShareChannelIdx;
                int SeriesShareSeriesIdx = (int)SeriesInstance.SeriesShareSeriesIdx;

                // If the shared channel instance is valid...
                if (SeriesShareChannelIdx < RecObservation.ChannelInstances.Count)
                {
                    // If the shared series instance of the shared channel instance is valid...
                    if (SeriesShareSeriesIdx < RecObservation.ChannelInstances[SeriesShareChannelIdx].SeriesInstances.Count)
                    {
                        // Change the reference to the series instance from the other data source.
                        RetSeriesInstance = RecObservation.ChannelInstances[SeriesShareChannelIdx].SeriesInstances[SeriesShareSeriesIdx];
                    }
                }

            } // If the series values are missing, but we should use a series instance of another channel instance

            // Return a series instance.
            return RetSeriesInstance;

        } // GetSeriesInstance

        /// <summary>
        /// Gets the <see cref="OneChannelDefn"/> in <paramref name="RecDataSource"/> that corresponds to this <see cref="OneChannelInst"/>
        /// </summary>
        /// <param name="RecDataSource">Data Source Record Related to the <see cref="RecObservation"/> that contains this <see cref="OneChannelInst"/></param>
        /// <returns><see cref="OneChannelDefn"/> in <paramref name="RecDataSource"/> that corresponds to this <see cref="OneChannelInst"/>; 
        /// <c>null</c> if <see cref="ChannelDefnIdx"/> does not match a <see cref="OneChannelDefn"/> in <paramref name="RecDataSource"/>.</returns>
        public OneChannelDefn GetChannelDefn(RecDataSource RecDataSource)
        {
            // Initialize the return value.
            OneChannelDefn ChannelDefn = null;

            // Determine the channel definition index for this channel instance
            int ThisChannelDefnIdx = (int)ChannelDefnIdx;

            // If the channel definition index is valid...
            if (ThisChannelDefnIdx < RecDataSource.ChannelDefns.Count)

                // Get a reference to this channel definition.
                ChannelDefn = RecDataSource.ChannelDefns[ThisChannelDefnIdx];

            // Return the related channel definition
            return ChannelDefn;

        } // GetChannelDefn

    } // class OneChannelInst

} // namespace PQDIF.Validator