/*
**  Class:          PQDIF.Validator.OneChannelSetting
**  Description:	One of these collections per channel
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
    /// Must have one or more channel definitions
    /// </summary>
    public class OneChannelDefn
    {
        /// <summary>
        /// Arbitrary string to hold a name for the channel
        /// </summary>
        public string ChannelName = "";

        /// <summary>
        /// Phase identifier
        /// </summary>
        /// <remarks>Required</remarks>
        public uint PhaseID = (uint)ID_PHASE_NONE;

        /// <summary>
        /// Arbitrary string
        /// </summary>
        public string OtherChannelIdentifier = "";

        /// <summary>
        /// This tag can be repeated if there are multiple groupings. The first one should be the highest-level group (example: a bus), 
        /// and the next one should be a lower group (example: a feeder).
        /// </summary>
        public string GroupName = "";

        /// <summary>
        /// The high-level description of the type of quantity which is being captured by this channel. The following series value types (in order) 
        /// should be used (ID_SERIES_VALUE_TYPE_VAL, etc.).
        /// </summary>
        /// <remarks>Required</remarks>
        public Guid QuantityTypeID = ID_QT_VALUELOG; // There is no default value for tagQuantityTypeID defined in IEEE Std 1159.3

        /// <summary>
        /// Identifies the physical quantity under measurement  (For example, Voltage, Current, Power, etc.).  In general, there is a one-to-one
        /// correspondence between this and the units of the series, but not always.
        /// </summary>
        /// <remarks>Required</remarks>
        public uint QuantityMeasuredID = (uint)ID_QM_NONE;

        /// <summary>
        /// The instrument physical channel with which this channel definition is associated
        /// </summary>
        public uint PhysicalChannel = 0;

        /// <summary>
        /// Additional quantity information
        /// </summary>
        public string QuantityName = "";

        /// <summary>
        /// Identifies the series which will be the primary. Index into the tagSeriesDefns collection
        /// </summary>
        public uint PrimarySeriesIdx = 0;

        /// <summary>
        /// The tagSeriesDefns collection shall be a collection where the count equals the count of series definitions. 
        /// Each entry shall be another collection, each having the tagOneSeriesDefn tag.
        /// </summary>
        public List<OneSeriesDefn> SeriesDefns = new List<OneSeriesDefn>();

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="OneChannelDefn"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="OneChannelDefn"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="OneChannelDefn"/> class.
        /// </summary>
        public OneChannelDefn()
        { 
            // Default constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="OneChannelDefn"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a series instance record</param>
        /// <param name="ChannelDefnsPointer">Channel Definitions Pointer</param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public OneChannelDefn(CPQDIFNet PqdifFile, IntPtr ChannelDefnsPointer, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
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
                bool MissingPhaseID = true;
                bool MissingQuantityTypeID = true;
                bool MissingQuantityMeasuredID = true;
                bool MissingSeriesDefns = true;

                // Determine if ChannelDefnsPointer points to a collection and the count of elements associated with that collection.
                int ChannelDefnsCollectionCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, ChannelDefnsPointer, LoggerApplication, LoggerCompliance);

                // For each tag in the CustomSourceInfo collectione...
                for (int ChannelDefnsCollectionIndex = 0; ChannelDefnsCollectionIndex < ChannelDefnsCollectionCount; ChannelDefnsCollectionIndex++)
                {
                    // Create a log message for this ChannelDefns index.
                    string ChannelDefnsLogMessagePrefix = "Collection Index " + ChannelDefnsCollectionIndex.ToString();

                    // Get the pointer to the current tag
                    IntPtr ChannelDefnTagPointer = new IntPtr();
                    bool Result = PqdifFile.CollectionGetEntry(ChannelDefnsPointer, ChannelDefnsCollectionIndex, ref ChannelDefnTagPointer);
                    if (!Result)
                    {
                        LoggerApplication.Log(ChannelDefnsLogMessagePrefix + ": CollectionGetEntry", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Get the tag ID and tag name
                    Guid TagID = new Guid();
                    string TagName = "";
                    Result = PqdifFile.ElementGetTag(ChannelDefnTagPointer, ref TagID, ref TagName);
                    if (!Result)
                    {
                        LoggerApplication.Log(ChannelDefnsLogMessagePrefix + ": ElementGetTag", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Initialize a flag that tells us that the current TagID was not matched to a known TagID.
                    bool TagMatched = false;

                    // Try to match the ID to the known tags of a channel definition record and change the flag for required flag when we find them.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagChannelName, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref ChannelName);

                    // Try to match the tag to tagPhaseID
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagPhaseID, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref PhaseID);
                        if (TagMatched)
                            MissingPhaseID = false;
                    }

                    // Try to match the tag to optional tags.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagOtherChannelIdentifier, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref OtherChannelIdentifier);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagGroupName, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref GroupName);

                    // Try to match the tag to tagQuantityTypeID
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagQuantityTypeID, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref QuantityTypeID);
                        if (TagMatched)
                            MissingQuantityTypeID = false;
                    }

                    // Try to match the tag to tagQuantityMeasuredID
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagQuantityMeasuredID, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref QuantityMeasuredID);
                        if (TagMatched)
                            MissingQuantityMeasuredID = false;
                    }

                    // Try to match the tag to optional tags.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagPhysicalChannel, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref PhysicalChannel);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagQuantityName, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref QuantityName);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagPrimarySeriesIdx, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelDefnsLogMessagePrefix, ref PrimarySeriesIdx);


                    // If this Tag is for a Series Definition collection...
                    if (TagID == tagSeriesDefns)
                    {

                        // We matched the tag.
                        TagMatched = true;

                        // We have found the required series definition collection
                        MissingSeriesDefns = false;

                        // Copy the series definitions pointer
                        IntPtr SeriesDefnsPointer = ChannelDefnTagPointer;

                        // Determine if SeriesDefnsPointer points to a collection and the count of elements associated with that collection.
                        int SeriesDefnCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, SeriesDefnsPointer, LoggerApplication, LoggerCompliance);

                        // Log a a compliance error if the series definition collection does not have at least one series definition.
                        if (SeriesDefnCount == 0)
                            LoggerCompliance.Log(ChannelDefnsLogMessagePrefix + ": tagSeriesDefns must have at least one tagOneSeriesDefn record.", LogLevels.Error);

                        // For each series definition...
                        for (int SeriesDefnIndex = 0; SeriesDefnIndex < SeriesDefnCount; SeriesDefnIndex++)
                        {
                            // Create a log message for this ChannelDefns index.
                            string SeriesDefnLogMessagePrefix = ChannelDefnsLogMessagePrefix + ": Series Definition Index " + SeriesDefnIndex.ToString();

                            // Get the pointer to the current series definition
                            IntPtr SeriesDefnPointer = new IntPtr();
                            Result = PqdifFile.CollectionGetEntry(SeriesDefnsPointer, SeriesDefnIndex, ref SeriesDefnPointer);

                            // If we could get a pointer to the current series definition...
                            if (Result)
                            {
                                // Create the current series definition record.
                                OneSeriesDefn ThisSeriesDefn = new OneSeriesDefn(PqdifFile, SeriesDefnPointer, LogLevelApplication, LogLevelCompliance, out RecordOK);

                                // If we created the series definition, then add it to the list of series definitions.
                                if (RecordOK)
                                    SeriesDefns.Add(ThisSeriesDefn);
                            }
                            else
                            {
                                LoggerApplication.Log(SeriesDefnLogMessagePrefix + ": CollectionGetEntry Error ", LogLevels.Error);
                            }

                        } // For each series definition

                    } // If this Tag is for a Series Definition collection

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(ChannelDefnsLogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the CustomSourceInfo collection


                // Log warnings if required tags are missing.
                if (MissingPhaseID)
                    LoggerCompliance.Log("Required tagPhaseID is Missing.", LogLevels.Error);
                if (MissingQuantityTypeID)
                    LoggerCompliance.Log("Required tagQuantityTypeID is Missing.", LogLevels.Error);
                if (MissingQuantityMeasuredID)
                    LoggerCompliance.Log("Required tagQuantityMeasuredID is Missing.", LogLevels.Error);
                if (MissingSeriesDefns)
                    LoggerCompliance.Log("Required tagSeriesDefns is Missing.", LogLevels.Error);
                if (ChannelDefnsCollectionCount == 0)
                    LoggerCompliance.Log("tagChannelDefns Must have at least tagOneChannelDefn Record.", LogLevels.Error);

                // Validate the series definitions for this quantity type.
                ValidateSeriesDefinitionsForQuantityType(LoggerCompliance);

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // OneChannelDefn


        /// <summary>
        /// Validates the count and order of <see cref="OneSeriesDefn.ValueTypeID"/> values for this <see cref="QuantityTypeID"/>
        /// </summary>
        /// <param name="LoggerCompliance">PQDIF Compliance Logger</param>
        private void ValidateSeriesDefinitionsForQuantityType(Logger LoggerCompliance)
        {

            if (QuantityTypeID == ID_QT_WAVEFORM)
            {
                // TIME, VAL / For point-on-wave measurements
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[2] { ID_SERIES_VALUE_TYPE_TIME, ID_SERIES_VALUE_TYPE_VAL });
            }
            else if (QuantityTypeID == ID_QT_VALUELOG)
            {
                // TIME, MIN, MAX, AVG, INST, VAL … / For time-based logged entries.
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[1] { ID_SERIES_VALUE_TYPE_TIME });
            }
            else if (QuantityTypeID == ID_QT_PHASOR)
            {
                // TIME, MIN, MAX, AVG, INST, VAL, PHASEANGLE … / For time-domain measurements including magnitudes and (optionally) phase angle.
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[1] { ID_SERIES_VALUE_TYPE_TIME });
            }
            else if (QuantityTypeID == ID_QT_RESPONSE)
            {
                // VAL, VAL, PHASEANGLE / For frequency-domain measurements including magnitude and (optionally) phase
                if (SeriesDefns.Count == 2)
                    ValidateSeriesDefinitions(LoggerCompliance, new Guid[2] { ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_VAL });

                else if (SeriesDefns.Count == 3)
                    ValidateSeriesDefinitions(LoggerCompliance, new Guid[3] { ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_PHASEANGLE });
            }
            else if (QuantityTypeID == ID_QT_FLASH)
            {
                // TIME, LAT, LON, VAL, POLARITY, ELLIPSE
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[6] { ID_SERIES_VALUE_TYPE_TIME, ID_SERIES_VALUE_TYPE_LATITUDE, ID_SERIES_VALUE_TYPE_LONGITUDE, 
                                                                          ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_POLARITY, ID_SERIES_VALUE_TYPE_ELLIPSE });
            }

            else if (QuantityTypeID == ID_QT_HISTOGRAM)
            {
                // BINLOW, BINHIGH, BINID, COUNT
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[4] { ID_SERIES_VALUE_TYPE_BINLOW, ID_SERIES_VALUE_TYPE_BINHIGH, ID_SERIES_VALUE_TYPE_BINID, ID_SERIES_VALUE_TYPE_COUNT });
            }

            else if (QuantityTypeID == ID_QT_HISTOGRAM3D)
            {
                // XBINLOW, XBINHIGH, YBINLOW, YBINHIGH, BINID, COUNT
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[6] { ID_SERIES_VALUE_TYPE_XBINLOW, ID_SERIES_VALUE_TYPE_XBINHIGH, ID_SERIES_VALUE_TYPE_YBINLOW, 
                                                                          ID_SERIES_VALUE_TYPE_YBINHIGH, ID_SERIES_VALUE_TYPE_BINID, ID_SERIES_VALUE_TYPE_COUNT });
            }

            else if (QuantityTypeID == ID_QT_CPF)
            {
                // PROB, VAL
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[2] { ID_SERIES_VALUE_TYPE_PROB, ID_SERIES_VALUE_TYPE_VAL });
            }

            else if (QuantityTypeID == ID_QT_XY)
            {
                // VAL, VAL
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[2] { ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_VAL });
            }

            else if (QuantityTypeID == ID_QT_MAGDUR)
            {
                // VAL, DURATION
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[2] { ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_DURATION });
            }
            else if (QuantityTypeID == ID_QT_XYZ)
            {
                // VAL, VAL, VAL
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[3] { ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_VAL });
            }
            else if (QuantityTypeID == ID_QT_MAGDURTIME)
            {
                // TIME, VAL, DURATION
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[3] { ID_SERIES_VALUE_TYPE_TIME, ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_DURATION });
            }

            else if (QuantityTypeID == ID_QT_MAGDURCOUNT)
            {
                // TIME, VAL, DUR, COUNT
                ValidateSeriesDefinitions(LoggerCompliance, new Guid[4] { ID_SERIES_VALUE_TYPE_TIME, ID_SERIES_VALUE_TYPE_VAL, ID_SERIES_VALUE_TYPE_DURATION, ID_SERIES_VALUE_TYPE_COUNT });
            }

        } // ValidateSeriesDefinitionsForQuantityType


        /// <summary>
        /// Validates the list of <see cref="OneSeriesDefn.ValueTypeID"/> values against the actual <see cref="SeriesDefns"/> of this <see cref="OneChannelDefn"/>
        /// </summary>
        /// <param name="LoggerCompliance">PQDIF Compliance Logger</param>
        /// <param name="ValueTypeID">List of <see cref="OneSeriesDefn.ValueTypeID"/> values</param>
        public void ValidateSeriesDefinitions(Logger LoggerCompliance, Guid[] ValueTypeID)
            {
            // If there are enough series definitions...
            if (SeriesDefns.Count >= ValueTypeID.Length)
            { 
                // For each series definition...
                for (int ValidTypeIndex = 0; ValidTypeIndex < ValueTypeID.Length; ValidTypeIndex++)
                {
                    // If the ValueTypeID of the series definition does not match the expected series definition...
                    if (SeriesDefns[ValidTypeIndex].ValueTypeID != ValueTypeID[ValidTypeIndex])

                        // Log a compliance error
                        LoggerCompliance.Log("For a Channel Definition with QuantityTypeID set to " + PQDIFNetWrapper.GetName(QuantityTypeID) + ", Series Definition " + ValidTypeIndex.ToString() + " should be " + PQDIFNetWrapper.GetName(ValueTypeID[ValidTypeIndex]) + ".", LogLevels.Error);
                }
            }

            // Otherwise, log a compliance error.
            else
                LoggerCompliance.Log("For QuantityTypeID set to " + PQDIFNetWrapper.GetName(QuantityTypeID) + ", there should be at least " + ValueTypeID.Length.ToString() + " series definitions.", LogLevels.Error);

        } // ValidateList


        /// <summary>
        /// Returns the <see cref="OneChannelDefn"/> Record Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Channel Definition Collection Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagOneChannelDefn>\r\n");

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

            // Export the tags of the Channel Definition collection as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagChannelName, ChannelName) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagPhaseID, PhaseID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagOtherChannelIdentifier, OtherChannelIdentifier) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagGroupName, GroupName) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagQuantityTypeID, QuantityTypeID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagQuantityMeasuredID, QuantityMeasuredID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagPhysicalChannel, PhysicalChannel) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagQuantityName, QuantityName) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagPrimarySeriesIdx, PrimarySeriesIdx) + "\r\n");

            // Export the elements of the Series Definition collection as XML elements
            StringBuilder.Append("<tagSeriesDefns>\r\n");
            foreach (OneSeriesDefn ThisSeriesDefn in SeriesDefns)
                StringBuilder.Append(ThisSeriesDefn.GetXML(ApplicationLog, ComplianceLog));
            StringBuilder.Append("</tagSeriesDefns>\r\n");                       

            // Finish the Channel Definition record.
            StringBuilder.Append("</tagOneChannelDefn>\r\n");

            // Return the XML version of the Channel Definition Collection
            return StringBuilder.ToString();

        } // GetXML

    } // class OneChannelDefn

} // namespace PQDIF.Validator