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

namespace PQDIF.Validator
{

    // Contains fundamental classes and base classes that define commonly-used value and reference data types, events and event handlers, interfaces, attributes, and processing exceptions.
    using System;

    // Defines IEEE PQDIF data types including PQDIF timestamps
    using PQDIFNet;

    // Defines IEEE PQDIF IDs and data types 
    using static PQDIFNet.Constants.Logical;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    /// <summary>
    /// One of these collections per channel
    /// </summary>
    public class OneChannelSetting
    {
        /// <summary>
        /// The channel definition which these settings apply to. Index into tagChannelDefns collection of the matching data source record.
        /// </summary>
        /// <remarks>Required</remarks>
        public uint ChannelDefnIdx = 0;

        /// <summary>
        /// Integer ID representing which trigger fields are used
        /// </summary>
        public uint? TriggerTypeID = null;

        /// <summary>
        /// Full scale range for this instrument channel
        /// </summary>
        public double? FullScale = null;

        /// <summary>
        /// Noise floor for this instrument channel
        /// </summary>
        public double? NoiseFloor = null;

        /// <summary>
        /// Parameters for shape based triggering algorithms for this channel
        /// </summary>
        public double[] TriggerShapeParam = null;

        /// <summary>
        /// PT or CT
        /// </summary>
        public uint? XDTransformerTypeID = null;

        /// <summary>
        /// System side part of ratio
        /// </summary>
        public double? XDSystemSideRatio = null;

        /// <summary>
        /// Monitor side part of ratio
        /// </summary>
        public double? XDMonitorSideRatio = null;

        /// <summary>
        /// Transducer frequency response
        /// </summary>
        public double[] XDFrequencyResponse = null;

        /// <summary>
        /// Channel time skew
        /// </summary>
        public double? CalTimeSkew = null;

        /// <summary>
        /// Channel DC offset error
        /// </summary>
        public double? CalOffset = null;

        /// <summary>
        /// Channel ratio error
        /// </summary>
        public double? CalRatio = null;

        /// <summary>
        /// Flag indicating that the applied/recorded calibration arrays shall be used to correct data
        /// </summary>
        public double? CalMustUseARCal = null;

        /// <summary>
        /// Array of applied signals for this channel
        /// </summary>
        public double[] CalApplied = null;

        /// <summary>
        /// Array of recorded actual values for the applied signal
        /// </summary>
        public double[] CalRecorded = null;

        /// <summary>
        /// High-High trigger for this channel
        /// </summary>
        public double? TriggerHighHigh = null;

        /// <summary>
        /// High trigger for this channel
        /// </summary>
        public double? TriggerHigh = null;

        /// <summary>
        /// Low trigger for this channel
        /// </summary>
        public double? TriggerLow = null;

        /// <summary>
        /// Low-Low trigger for this channel
        /// </summary>
        public double? TriggerLowLow = null;

        /// <summary>
        /// Deadband trigger for this channel
        /// </summary>
        public double? TriggerDeadBand = null;

        /// <summary>
        /// Rate of change trigger for this channel
        /// </summary>
        public double? TriggerRate = null;

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="OneChannelSetting"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="OneChannelSetting"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="OneChannelSetting"/> class.
        /// </summary>
        public OneChannelSetting()
        {
            // Default constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="OneChannelSetting"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a container record</param>
        /// <param name="ChannelSettingsPointer">Channel Settings Pointer</param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public OneChannelSetting(CPQDIFNet PqdifFile, IntPtr ChannelSettingsPointer, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
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
                bool MissingChannelDefnIdx = true;


                // Determine if ChannelSettingsPointer points to a collection and the count of elements associated with that collection.
                int ChannelSettingsCollectionCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, ChannelSettingsPointer, LoggerApplication, LoggerCompliance);

                // For each tag in the ChannelSettings collection...
                for (int ChannelSettingsCollectionIndex = 0; ChannelSettingsCollectionIndex < ChannelSettingsCollectionCount; ChannelSettingsCollectionIndex++)
                {
                    // Create a log message for this ChannelSettings index.
                    string ChannelSettingsLogMessagePrefix = "Collection Index " + ChannelSettingsCollectionIndex.ToString();

                    // Get the pointer to the current tag
                    IntPtr ChannelDefnTagPointer = new IntPtr();
                    bool Result = PqdifFile.CollectionGetEntry(ChannelSettingsPointer, ChannelSettingsCollectionIndex, ref ChannelDefnTagPointer);
                    if (!Result)
                    {
                        LoggerApplication.Log(ChannelSettingsLogMessagePrefix + ": CollectionGetEntry", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Get the tag ID and tag name
                    Guid TagID = new Guid();
                    string TagName = "";
                    Result = PqdifFile.ElementGetTag(ChannelDefnTagPointer, ref TagID, ref TagName);
                    if (!Result)
                    {
                        LoggerApplication.Log(ChannelSettingsLogMessagePrefix + ": ElementGetTag", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Initialize a flag that tells us that the current TagID was not matched to a known TagID.
                    bool TagMatched = false;


                    // Try to match the tag to tagChannelDefnIdx
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagChannelDefnIdx, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref ChannelDefnIdx);
                        if (TagMatched)
                            MissingChannelDefnIdx = false;
                    }

                    // Try to match the tags to other optional tags
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTriggerTypeID, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerTypeID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagFullScale, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref FullScale);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagNoiseFloor, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref NoiseFloor);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetVectorArray(TagID, tagTriggerShapeParam, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerShapeParam);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagXDTransformerTypeID, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref XDTransformerTypeID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagXDSystemSideRatio, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref XDSystemSideRatio);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagXDMonitorSideRatio, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref XDMonitorSideRatio);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetVectorArray(TagID, tagXDFrequencyResponse, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref XDFrequencyResponse);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCalTimeSkew, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref CalTimeSkew);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCalOffset, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref CalOffset);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCalRatio, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref CalRatio);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCalMustUseARCal, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref CalMustUseARCal);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetVectorArray(TagID, tagCalApplied, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref CalApplied);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetVectorArray(TagID, tagCalRecorded, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref CalRecorded);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTriggerHighHigh, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerHighHigh);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTriggerHigh, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerHigh);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTriggerLow, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerLow);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTriggerLowLow, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerLowLow);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTriggerDeadBand, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerDeadBand);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTriggerRate, PqdifFile, ChannelDefnTagPointer, LoggerApplication, LoggerCompliance, ChannelSettingsLogMessagePrefix, ref TriggerRate);

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(ChannelSettingsLogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the CustomSourceInfo collection


                // Log warnings if required tags are missing.
                if (MissingChannelDefnIdx)
                    LoggerCompliance.Log("Required tagChannelDefnIdx is Missing.", LogLevels.Error);

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // OneChannelSetting


        /// <summary>
        /// Returns the <see cref="OneChannelSetting"/> Record Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Channel Setting Collection Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagOneChannelSetting>\r\n");

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

            // Export the tags of the Channel Setting collection as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagChannelDefnIdx, ChannelDefnIdx) + "\r\n");

            if (TriggerTypeID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerTypeID, (uint)TriggerTypeID) + "\r\n");

            if (FullScale != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagFullScale, (double)FullScale) + "\r\n");

            if (NoiseFloor != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagNoiseFloor, (double)NoiseFloor) + "\r\n");

            if (TriggerShapeParam != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerShapeParam, TriggerShapeParam) + "\r\n");

            if (XDTransformerTypeID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagXDTransformerTypeID, (uint)XDTransformerTypeID) + "\r\n");

            if (XDSystemSideRatio != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagXDSystemSideRatio, (double)XDSystemSideRatio) + "\r\n");

            if (XDMonitorSideRatio != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagXDMonitorSideRatio, (double)XDMonitorSideRatio) + "\r\n");

            if (XDFrequencyResponse != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagXDFrequencyResponse, XDFrequencyResponse) + "\r\n");

            if (CalTimeSkew != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCalTimeSkew, (double)CalTimeSkew) + "\r\n");

            if (CalOffset != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCalOffset, (double)CalOffset) + "\r\n");

            if (CalRatio != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCalRatio, (double)CalRatio) + "\r\n");

            if (CalMustUseARCal != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCalMustUseARCal, (double)CalMustUseARCal) + "\r\n");

            if (CalApplied != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCalApplied, CalApplied) + "\r\n");

            if (CalRecorded != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCalRecorded, CalRecorded) + "\r\n");

            if (TriggerHighHigh != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerHighHigh, (double)TriggerHighHigh) + "\r\n");

            if (TriggerHigh != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerHigh, (double)TriggerHigh) + "\r\n");

            if (TriggerLow != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerLow, (double)TriggerLow) + "\r\n");

            if (TriggerLowLow != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerLowLow, (double)TriggerLowLow) + "\r\n");

            if (TriggerDeadBand != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerDeadBand, (double)TriggerDeadBand) + "\r\n");

            if (TriggerRate != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTriggerRate, (double)TriggerRate) + "\r\n");


            // Finish the Channel Setting record.
            StringBuilder.Append("</tagOneChannelSetting>\r\n");

            // Return the XML version of the Channel Setting
            return StringBuilder.ToString();

        } // GetXML

    } // class OneChannelSetting

} // namespace PQDIF.Validator