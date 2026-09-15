/*
**  Class:          PQDIF.Validator.OneSeriesDefn
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

// Defines IEEE PQDIF data types including PQDIF timestamps
using PQDIFNet;

// Defines IEEE PQDIF IDs
using static PQDIFNet.Constants.Logical;

// For logging errors, warnings, other messages.
using PQDIF.Log;


namespace PQDIF.Validator
{
    /// <summary>
    /// One of these collections per series
    /// </summary>
    public class OneSeriesDefn
    {
        /// <summary>
        /// Specifies the meaning of the series data
        /// </summary>
        /// <remarks>Required</remarks>
        public Guid ValueTypeID = ID_SERIES_VALUE_TYPE_VAL; // From IEEE Std 1159.3 Annex B: "ID_SERIES_VALUE_TYPE_VAL...should be the default value type for a measurement"

        /// <summary>
        /// This specifies the units of the data in this series. The expected physical type for the tagSeriesValues vector is REAL4 or REAL8 (except as noted).
        /// </summary>
        /// <remarks>Required</remarks>
        public uint QuantityUnitsID = (uint)ID_QU_NONE;

        /// <summary>
        /// Additional detail about the meaning of the series data
        /// </summary>
        /// <remarks>Required</remarks>
        public Guid QuantityCharacteristicID = ID_QC_NONE;

        /// <summary>
        /// Defines the number of significant digits in the data represented by this series
        /// </summary>
        public uint? QuantitySignificantDigitsID;

        /// <summary>
        /// Contains a double indicating the scaled distance between two values of the quantity represented by this series (e.g., scaled A/D resolution)
        /// </summary>
        public double? QuantityResolutionID = null;

        /// <summary>
        /// The legal values for this entry are masks since they are OR-able.
        /// </summary>
        /// <remarks>Required</remarks>
        public uint StorageMethodID = (uint)ID_SERIES_METHOD_VALUES;

        /// <summary>
        /// Arbitrary string
        /// </summary>
        public string ValueTypeName = "";

        /// <summary> 
        /// Hint about expected Greek prefix
        /// </summary>
        public uint? HintGreekPrefixID = null;

        /// <summary>
        /// Hint about preferred display units
        /// </summary>
        public uint? HintPreferredUnitsID = null;

        /// <summary>
        ///  Hint about preferred default display
        /// </summary>
        public uint? HintDefaultDisplayID = null;

        /// <summary>
        /// For a probability series definition, this specifies its time interval (in seconds; >0)
        /// </summary>
        public double? ProbInterval = null;

        /// <summary>
        /// For a probability series definition, this specifies its probability percentile (in percent; 0-100)
        /// </summary>
        public double? ProbPercentile = null;

        /// <summary>
        /// Contains the default nominal base voltage or any or any other necessary normalizing quantity.  Display programs may use this value
        /// or the tagSeriesBaseQuantity in the series instance for displaying data in percent or per unit.
        /// </summary>
        public double? SeriesNominalQuantity = null;

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="OneSeriesDefn"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="OneSeriesDefn"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();


        /// <summary>
        /// Initializes a new instance of the <see cref="OneSeriesDefn"/> class.
        /// </summary>
        public OneSeriesDefn()
        {
            // Default Constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="OneSeriesDefn"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a series instance record</param>
        /// <param name="SeriesDefinitionPointer">Pointer to the Series Instance Record in <paramref name="PqdifFile"/></param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public OneSeriesDefn(CPQDIFNet PqdifFile, IntPtr SeriesDefinitionPointer, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
        {

            // Set up an error handler
            try
            {
                // Set the logger level
                LoggerApplication.LogLevel = LogLevelApplication;
                LoggerCompliance.LogLevel = LogLevelCompliance;

                // Initialize the return value.
                RecordOK = true;

                // Initialize flags that tell us if required tags are missing.
                bool MissingValueTypeID = true;
                bool MissingQuantityUnitsID = true;

                // Determine if PointerChannelDefns points to a collection and the count of elements associated with that collection.
                int SeriesDefinitionCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, SeriesDefinitionPointer, LoggerApplication, LoggerCompliance);

                // For each tag in the current collection...
                for (int SeriesDefinitionIndex = 0; SeriesDefinitionIndex < SeriesDefinitionCount; SeriesDefinitionIndex++)
                {

                    // Create a log message for this Series Definition index.
                    string SeriesDefinitionIndexLogMessagePrefix = "Collection Index " + SeriesDefinitionIndex.ToString();

                    // Get the pointer to the current tag
                    IntPtr PointerCollectionTag = new IntPtr();
                    bool Result = PqdifFile.CollectionGetEntry(SeriesDefinitionPointer, SeriesDefinitionIndex, ref PointerCollectionTag);
                    if (!Result)
                    {
                        LoggerApplication.Log(SeriesDefinitionIndexLogMessagePrefix + ": CollectionGetEntry", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Get the tag ID and tag name
                    Guid TagID = new Guid();
                    string TagName = "";
                    Result = PqdifFile.ElementGetTag(PointerCollectionTag, ref TagID, ref TagName);
                    if (!Result)
                    {
                        LoggerApplication.Log(SeriesDefinitionIndexLogMessagePrefix + ": ElementGetTag", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Initialize a flag that tells us that the current TagID was not matched to a known TagID.
                    bool TagMatched = false;

                    // If the current TagID has not been matched, then try to match it as tagValueTypeID                    
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagValueTypeID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref ValueTypeID);
                        if (TagMatched)
                            MissingValueTypeID = false;
                    }

                    // If the current TagID has not been matched, then try to match it as tagQuantityUnitsID                    
                    if (!TagMatched)
                    {
                        TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagQuantityUnitsID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref QuantityUnitsID);
                        if (TagMatched)
                            MissingQuantityUnitsID = false;
                    }

                    // If the current TagID has not been matched, then try to match it as all other known tags of the Series Instance Collection.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagQuantityCharacteristicID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref QuantityCharacteristicID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagQuantitySignificantDigitsID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref QuantitySignificantDigitsID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagQuantityResolutionID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref QuantityResolutionID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagStorageMethodID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref StorageMethodID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagValueTypeName, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref ValueTypeName);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagHintGreekPrefixID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref HintGreekPrefixID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagHintPreferredUnitsID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref HintPreferredUnitsID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagHintDefaultDisplayID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref HintDefaultDisplayID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagProbInterval, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref ProbInterval);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagProbPercentile, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref ProbPercentile);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagSeriesNominalQuantity, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, SeriesDefinitionIndexLogMessagePrefix, ref SeriesNominalQuantity);

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(SeriesDefinitionIndexLogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the current collection


                // Log warnings if required tags are missing.
                if (MissingValueTypeID)
                    LoggerCompliance.Log("Required tagValueTypeID is Missing.", LogLevels.Error);
                if (MissingQuantityUnitsID)
                    LoggerCompliance.Log("Required tagQuantityUnitsID is Missing.", LogLevels.Error);
                if (SeriesDefinitionCount == 0)
                    LoggerCompliance.Log("A Series Definition Collection Should have at least One Element.", LogLevels.Error);

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log("Series Definition: " + ex.Message, LogLevels.Error);
                RecordOK = false;
            }

        } // OneSeriesDefn


        /// <summary>
        /// Returns the <see cref="OneSeriesDefn"/> Record Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Series Definition Collection Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagOneSeriesDefn>\r\n");

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

            // Export the tags of the Series Definition collection as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagValueTypeID, ValueTypeID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagQuantityUnitsID, QuantityUnitsID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagQuantityCharacteristicID, QuantityCharacteristicID) + "\r\n");

            if (QuantitySignificantDigitsID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagQuantitySignificantDigitsID, (uint)QuantitySignificantDigitsID) + "\r\n");

            if (QuantityResolutionID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagQuantityResolutionID, (uint)QuantityResolutionID) + "\r\n");

            StringBuilder.Append(Utilities.XML.GetElement(tagStorageMethodID, StorageMethodID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagValueTypeName, ValueTypeName) + "\r\n");

            if (HintGreekPrefixID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagHintGreekPrefixID, (uint)HintGreekPrefixID) + "\r\n");

            if (HintPreferredUnitsID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagHintPreferredUnitsID, (uint)HintPreferredUnitsID) + "\r\n");

            if (HintDefaultDisplayID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagHintDefaultDisplayID, (uint)HintDefaultDisplayID) + "\r\n");

            if (ProbInterval != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagProbInterval, (double)ProbInterval) + "\r\n");

            if (ProbPercentile != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagProbPercentile, (double)ProbPercentile) + "\r\n");

            if (SeriesNominalQuantity != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagProbPercentile, (double)SeriesNominalQuantity) + "\r\n");

            // Finish the Series Definition record.
            StringBuilder.Append("</tagOneSeriesDefn>\r\n");

            // Return the XML version of the Container
            return StringBuilder.ToString();

        } // GetXML

    } // class OneSeriesDefn    

} // namespace PQDIF.Validator