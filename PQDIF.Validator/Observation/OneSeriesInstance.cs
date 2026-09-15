/*
**  Class:          PQDIF.Validator.OneSeriesInstance
**  Description:	One of these collections per series instance
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
    /// One of these collections per series instance
    /// </summary>
     public partial class OneSeriesInstance
     {
        /// <summary>
        /// Contains the nominal base voltage, or any other necessary normalizing quantity
        /// </summary>
        public double? SeriesBaseQuantity = null;

        /// <summary>
        /// If not present, assumed to be 1. The physical type may not match that of <see cref="SeriesValues"/>. 
        /// <see cref="SeriesScale"/>, along with <see cref="SeriesOffset"/>, provides a method to translate 
        /// the sample values stored in <see cref="SeriesValues"/> to real values as follows: 
        /// real value = (<see cref="SeriesValues"/> * <see cref="SeriesScale"/>) + <see cref="SeriesOffset"/>.
        /// </summary>
        public object SeriesScale = null;

        /// <summary>
        /// If not present, assumed to be 0. Should be used as a starting point when the <see cref="ID_SERIES_METHOD_INCREMENT"/> storage method is used. 
        /// The physical type may not match that of <see cref="SeriesValues"/>. 
        /// <see cref="SeriesOffset"/>, along with <see cref="SeriesScale"/>, provides a method to translate 
        /// the samples stored in the series to real values as follows: 
        /// real value = (<see cref="SeriesValues"/> * <see cref="SeriesScale"/>) + <see cref="SeriesOffset"/>.
        /// </summary>
        public object SeriesOffset = null;

        /// <summary>
        /// Identifies the channel which owns the series to be shared. An index into the tagChannelInstances collection.
        /// </summary>
        public uint? SeriesShareChannelIdx = null;

        /// <summary>
        /// Identifies the series to be shared. An index into the tagSeriesInstances collection. The <see cref="SeriesValues"/> vector 
        /// from this series is used. This shall be present if <see cref="SeriesShareChannelIdx"/> is used.
        /// </summary>
        public uint? SeriesShareSeriesIdx;

        /// <summary>
        /// Contains the actual data points of the series. Required unless the data series is shared, 
        /// in which case both <see cref="SeriesShareChannelIdx"/> and <see cref="SeriesShareSeriesIdx"/> should be present.
        /// </summary>
        /// <remarks>Required.</remarks>
        public Array SeriesValues = null;

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="OneSeriesInstance"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="OneSeriesInstance"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="OneSeriesInstance"/> class.
        /// </summary>
        public OneSeriesInstance()
        {
            // Default Constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="OneSeriesInstance"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a series instance</param>
        /// <param name="SeriesInstancePointer">Series Instance Pointer.</param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public OneSeriesInstance(CPQDIFNet PqdifFile, IntPtr SeriesInstancePointer, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
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
                bool MissingSeriesValues = true;


                // Determine if SeriesInstancePointer points to a collection and the count of elements associated with that collection.
                int SeriesInstanceCollectionCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, SeriesInstancePointer, LoggerApplication, LoggerCompliance);

                // For each tag in the SeriesInstance collection...
                for (int SeriesCollectionIndex = 0; SeriesCollectionIndex < SeriesInstanceCollectionCount; SeriesCollectionIndex++)
                {
                    // Create a log message for this Series Instance index.
                    string SeriesInstanceLogMessagePrefix = "Collection Index " + SeriesCollectionIndex.ToString();

                    // Get the pointer to the current tag
                    IntPtr TagPointer = new IntPtr();
                    bool Result = PqdifFile.CollectionGetEntry(SeriesInstancePointer, SeriesCollectionIndex, ref TagPointer);
                    if (!Result)
                    {
                        LoggerApplication.Log(SeriesInstanceLogMessagePrefix + ": CollectionGetEntry Error", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Get the tag ID and tag name
                    Guid TagID = new Guid();
                    string TagName = "";
                    Result = PqdifFile.ElementGetTag(TagPointer, ref TagID, ref TagName);
                    if (!Result)
                    {
                        LoggerApplication.Log(SeriesInstanceLogMessagePrefix + ": ElementGetTag Error", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Initialize a flag that tells us that the current TagID was not matched to a known TagID.
                    bool TagMatched = false;

                    // Try to match the ID to the known tags of a series instance record and change the flag for required flag when we find them.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagSeriesBaseQuantity, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, SeriesInstanceLogMessagePrefix, ref SeriesBaseQuantity);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagSeriesScale, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, SeriesInstanceLogMessagePrefix, ref SeriesScale);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagSeriesOffset, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, SeriesInstanceLogMessagePrefix, ref SeriesOffset);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagSeriesShareChannelIdx, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, SeriesInstanceLogMessagePrefix, ref SeriesShareChannelIdx);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagSeriesShareSeriesIdx, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, SeriesInstanceLogMessagePrefix, ref SeriesShareSeriesIdx);

                    // If the tag has not yet been matched...
                    if (!TagMatched)
                    {
                        // If the tag is a match for tagSeriesValues...
                        if (TagID == tagSeriesValues)
                        {
                            // We have found the required tagSeriesValues element.
                            MissingSeriesValues = false;
                            TagMatched = true;

                            // Try to get the series values as an array.
                            Result = PQDIFNetWrapper.ElementGetVectorArray(TagID, tagSeriesValues, PqdifFile, TagPointer, LoggerApplication, LoggerCompliance, SeriesInstanceLogMessagePrefix, ref SeriesValues);
                            if (!Result)
                            {
                                LoggerApplication.Log("ElementGetVectorArray Error", LogLevels.Error);
                                RecordOK = false;
                                return;
                            }

                        } // If the tag is a match for tagSeriesValues

                    } // If the tag has not yet been matched

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(SeriesInstanceLogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the CustomSourceInfo collection


                // Log warnings if required tags are missing.
                if (MissingSeriesValues && (SeriesShareChannelIdx == null) && (SeriesShareSeriesIdx == null))
                    LoggerCompliance.Log("tagSeriesValues, tagSeriesShareChannelIdx, and tagSeriesShareSeriesIdx are missing. tagSeriesValues is required unless the data series is shared, in which case both tagSeriesShareChannelIdx and tagSeriesShareSeriesIdx should be present.", LogLevels.Error);

                else if (MissingSeriesValues && (SeriesShareChannelIdx == null))
                    LoggerCompliance.Log("tagSeriesValues and tagSeriesShareChannelIdx are missing. tagSeriesValues is required unless the data series is shared, in which case both tagSeriesShareChannelIdx and tagSeriesShareSeriesIdx should be present.", LogLevels.Error);

                else if (MissingSeriesValues && (SeriesShareSeriesIdx == null))
                    LoggerCompliance.Log("tagSeriesValues and tagSeriesShareSeriesIdx are missing. tagSeriesValues is required unless the data series is shared, in which case both tagSeriesShareChannelIdx and tagSeriesShareSeriesIdx should be present.", LogLevels.Error);

                // Log messages about physical type discrepancies of tagSeriesScale and/or tagSeriesOffset.
                // This is added as a warning rather than an error since the pqdiflib supports scale, offset, and series values being different types.
                // ToDo: Add a version check here for the version of IEEE 1159.3 that follows IEEE Std 1159.3-2019, since the requirement will change.
                Type SeriesValuesType = SeriesValuesBaseType();
                if ((SeriesValues != null) && (SeriesScale != null) && (SeriesValuesType != null) && (SeriesValuesType != SeriesScale.GetType()))
                    LoggerCompliance.Log("IEEE Std 1159.3-2019 states that the physical type of tagSeriesScale should match that of tagSeriesValues.", LogLevels.Warning);

                if ((SeriesValues != null) && (SeriesOffset != null) && (SeriesValuesType != null) && (SeriesValuesType != SeriesOffset.GetType()))
                    LoggerCompliance.Log("IEEE Std 1159.3-2019 states that the physical type of tagSeriesOffset should match that of tagSeriesValues.", LogLevels.Warning);

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // OneSeriesInstance


        /// <summary>
        /// Returns the <see cref="OneSeriesInstance" /> Collection Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>Series Instance Collection Formated in XML</returns>
        public string GetXML(bool ApplicationLog, bool ComplianceLog, uint? MaxSeriesValues)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagOneSeriesInstance>\r\n");

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

            // Export the tags of the Series Instance collection as XML elements.
            if (SeriesBaseQuantity != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagSeriesBaseQuantity, (double)SeriesBaseQuantity) + "\r\n");

            if (SeriesScale != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagSeriesScale, SeriesScale) + "\r\n");

            if (SeriesOffset != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagSeriesOffset, SeriesOffset) + "\r\n");

            if (SeriesShareChannelIdx != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagSeriesShareChannelIdx, (uint)SeriesShareChannelIdx) + "\r\n");

            if (SeriesShareSeriesIdx != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagSeriesShareSeriesIdx, (uint)SeriesShareSeriesIdx) + "\r\n");

            if (SeriesValues != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagSeriesValues, SeriesValues, MaxSeriesValues) + "\r\n");

            // Finish the Series Instance record.
            StringBuilder.Append("</tagOneSeriesInstance>\r\n");

            // Return the XML version of the Series Instance
            return StringBuilder.ToString();

        } // GetXML
           

        /// <summary>
        /// Returns the base type of the <see cref="SeriesValues"/> array.
        /// </summary>
        /// <returns>Type</returns>
        private Type SeriesValuesBaseType()
        {
            if (SeriesValues == null)
                return null;

            if (SeriesValues.GetType() == typeof(bool[]))
                return typeof(bool);

            if (SeriesValues.GetType() == typeof(uint[]))
                return typeof(uint);

            else if (SeriesValues.GetType() == typeof(int[]))
                return typeof(int);

            else if (SeriesValues.GetType() == typeof(ushort[]))
                return typeof(ushort);

            else if (SeriesValues.GetType() == typeof(short[]))
                return typeof(short);

            else if (SeriesValues.GetType() == typeof(float[]))
                return typeof(float);

            else if (SeriesValues.GetType() == typeof(double[]))
                return typeof(double);

            else
                return null;

        } // SeriesValuesBaseType
          

    } // class OneSeriesInstance

} // namespace PQDIF.Validator