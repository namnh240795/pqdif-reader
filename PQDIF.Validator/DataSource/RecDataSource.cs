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

    // Contains interfaces and classes that define generic collections, which allow users to create strongly typed collections that provide better type safety and performance than non-generic strongly typed collections.
    using System.Collections.Generic;

    // Defines IEEE PQDIF data types including PQDIF timestamps
    using PQDIFNet;

    // Defines IEEE PQDIF IDs and data types 
    using static PQDIFNet.Constants.Logical;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    /// <summary>
    /// Record-level tag which identifies a data source (an instrument, etc.)
    /// </summary>
    /// <remarks>Required</remarks>
    public class RecDataSource
    {
        /// <summary>
        /// Standard ID or custom ID to specify the data source of the source PQ monitor
        /// </summary>
        /// <remarks>Required</remarks>
        public Guid DataSourceTypeID;

        /// <summary>
        /// Standard ID or custom ID to specify the vendor which made the PQ monitor or generated the PQDIF file
        /// </summary>
        public Guid VendorID = ID_VENDOR_NONE;

        /// <summary>
        /// Standard ID or custom ID to specify the equipment which made the PQ monitor or generated the PQDIF file
        /// </summary>
        public Guid EquipmentID = ID_EQUIP_NONE;

        /// <summary>
        /// This collection can include the standard name, address, and telephone number tags. These apply to the vendor as well as tags about the instrument itself.
        /// </summary>
        public CustomSourceInfo CustomSourceInfo = new CustomSourceInfo();

        /// <summary>
        /// Arbitrary string to hold data source (instrument) serial number
        /// </summary>
        public string SerialNumberDS = "";

        /// <summary>
        /// Arbitrary string to hold data source (instrument) version number (if applicable)
        /// </summary>
        public string VersionDS = "";

        /// <summary>
        /// Arbitrary string to hold the name of the data source (instrument)
        /// </summary>
        public string NameDS = "";

        /// <summary>
        /// Arbitrary string to hold data source (instrument) owner name
        /// </summary>
        public string OwnerDS = "";

        /// <summary>
        /// Arbitrary string to hold data source (instrument) location information
        /// </summary>
        public string LocationDS = "";

        /// <summary>
        /// Time Zone (EST, CST, UTC, UTC−05:00, etc.)
        /// </summary>
        public string TimeZoneDS = "";

        /// <summary>
        /// Longitude/Latitude
        /// </summary>
        public uint[] CoordinatesDS = new uint[2];

        /// <summary>
        /// The tagChannelDefns collection shall be a collection where the count equals the number of channel definitions. Each entry shall be another collection, each having the tagOneChannelDefn tag.
        /// </summary>
        public List<OneChannelDefn> ChannelDefns = new List<OneChannelDefn>();

        /// <summary>
        /// Time that this data source record became effective
        /// </summary>
        public DateTime Effective = new DateTime();

        /// <summary>
        /// Geographic coordinate that specifies the north/south position of a point on the Earth's surface. Latitude should be stored as angular measurement ranging
        /// from 0° at the Equator to +90° at the North Pole and to -90° at the South Pole.
        /// </summary>
        public double Latitude = 0;

        /// <summary>
        /// Geographic coordinate that specifies the east/west position of a point on the Earth's surface. Longitude should be stored as angular measurement ranging
        /// from 0° at the Prime Meridian to +180° eastward and −180° westward.
        /// </summary>
        public double Longitude = 0;

        /// <summary>
        /// Arbitrary comments
        /// </summary>
        public string Comments = "";

        /// <summary>
        /// Specifies the number of seconds to add to tag values and/or series values that store TIMESTAMPPQDIF types
        /// in the data source record or in settings records and/or observation records associated with this data source record
        /// from Coordinated Universal Time (UTC)
        /// </summary>
        public double? UTCtoLST = null;

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="RecDataSource"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="RecDataSource"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="RecDataSource"/> class.
        /// </summary>
        public RecDataSource()
        {
            // Default constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RecDataSource"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a container record</param>
        /// <param name="PointerCollection">Pointer to the Collection that Contains a Data Source Record in <paramref name="PqdifFile"/></param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public RecDataSource(CPQDIFNet PqdifFile, ref IntPtr PointerCollection, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
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
                bool MissingChannelDefns = true;


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

                    // If the current TagID has not been matched, then try to match it as known tags of the DataSource Collection.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetVectorArray(TagID, tagCoordinatesDS, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref CoordinatesDS);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagDataSourceTypeID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref DataSourceTypeID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagVendorID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref VendorID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagEquipmentID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref EquipmentID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagSerialNumberDS, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref SerialNumberDS);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagVersionDS, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref VersionDS);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagNameDS, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref NameDS);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagOwnerDS, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref OwnerDS);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagLocationDS, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref LocationDS);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagTimeZoneDS, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref TimeZoneDS);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagEffective, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref Effective);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagLatitude, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref Latitude);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagLongitude, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref Longitude);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagComments, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref Comments);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagUTCtoLST, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, CollectionLogMessagePrefix, ref UTCtoLST);


                    // If this tag is for a Channel Definitions collection...
                    if (TagID == tagChannelDefns)
                    {

                        // We matched the tag.
                        TagMatched = true;

                        // We have found the required channel definitions collection
                        MissingChannelDefns = false;

                        // Copy the channel definitions pointer
                        IntPtr ChannelDefnsPointer = PointerCollectionTag;

                        // Determine if ChannelDefnsPointer points to a collection and the count of elements associated with that collection.
                        int ChannelDefnCount = PQDIFNetWrapper.CollectionGetCount(PqdifFile, ChannelDefnsPointer, LoggerApplication, LoggerCompliance);

                        // Log a a compliance error if the channel definition collection does not have at least one channel definition.
                        if (ChannelDefnCount == 0)
                            LoggerCompliance.Log(CollectionLogMessagePrefix + ": tagChannelDefns must have at least one tagOneChannelDefn record.", LogLevels.Error);

                        // For each channel definition...
                        for (int ChannelDefnIndex = 0; ChannelDefnIndex < ChannelDefnCount; ChannelDefnIndex++)
                        {
                            // Create a log message for this channel definition.
                            string ChannelDefnLogMessagePrefix = CollectionLogMessagePrefix + ": ChannelDefn Index " + ChannelDefnIndex.ToString();

                            // Get the pointer to the current channel definition
                            IntPtr ChannelDefnPointer = new IntPtr();
                            Result = PqdifFile.CollectionGetEntry(ChannelDefnsPointer, ChannelDefnIndex, ref ChannelDefnPointer);

                            // If we could get a pointer to the current channel definition...
                            if (Result)
                            {
                                // Create the current channel definition record.
                                OneChannelDefn ThisChannelDefn = new OneChannelDefn(PqdifFile, ChannelDefnPointer, LogLevelApplication, LogLevelCompliance, out RecordOK);

                                // If we created the channel definition, then add it to the list of channel definitions.
                                if (RecordOK)
                                    ChannelDefns.Add(ThisChannelDefn);
                            }
                            else
                            {
                                LoggerApplication.Log(ChannelDefnLogMessagePrefix + ": CollectionGetEntry Error", LogLevels.Error);
                            }

                        } // For each channel definition

                    } // If this tag is for a Channel Definitions collection

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(CollectionLogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the data source


                // Log warnings if required tags are missing.
                if (MissingChannelDefns)
                    LoggerCompliance.Log("Required tagChannelDefns is Missing.", LogLevels.Error);


            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // RecDataSource


        /// <summary>
        /// Returns the <see cref="RecDataSource"/> Record Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Data Source Record Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagRecDataSource>\r\n");

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

            // Export the tags of the Data Source Record collection as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagDataSourceTypeID, DataSourceTypeID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagVendorID, VendorID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagEquipmentID, EquipmentID) + "\r\n");
            StringBuilder.Append(CustomSourceInfo.GetXML(ApplicationLog, ComplianceLog));
            StringBuilder.Append(Utilities.XML.GetElement(tagSerialNumberDS, SerialNumberDS) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagVersionDS, VersionDS) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagNameDS, NameDS) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagOwnerDS, OwnerDS) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagLocationDS, LocationDS) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagTimeZoneDS, TimeZoneDS) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagCoordinatesDS, CoordinatesDS, null) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagTimeZoneDS, TimeZoneDS) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagComments, Comments) + "\r\n");

            if (UTCtoLST != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagUTCtoLST, UTCtoLST) + "\r\n");

            // Export the Channel Definitions as XML elements
            StringBuilder.Append("<tagChannelDefns>\r\n");
            foreach (OneChannelDefn ThisChannelDefn in ChannelDefns)
                StringBuilder.Append(ThisChannelDefn.GetXML(ApplicationLog, ComplianceLog));
            StringBuilder.Append("</tagChannelDefns>\r\n");

            StringBuilder.Append(Utilities.XML.GetElement(tagLatitude, Latitude) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagLongitude, Longitude) + "\r\n");

            // Finish the Data Source record.
            StringBuilder.Append("</tagRecDataSource>\r\n");

            // Return the XML version of the Container
            return StringBuilder.ToString();

        } // GetXML


        /// <summary>
        /// Returns the nominal voltage for this data source from the first channel definition with <see cref="OneChannelDefn.QuantityMeasuredID"/> equal to <see cref="ID_QM_VOLTAGE"/>
        /// that has a series definition with <see cref="OneSeriesDefn.QuantityCharacteristicID"/> equal to <see cref="ID_QC_RMS"/> or <see cref="ID_QC_INSTANTANEOUS"/> 
        /// </summary>
        /// <returns>Nominal Voltage for this Data Source</returns>
        public double? GetNominalVoltage()
        {
            // Initialize the return value
            double? NominalVoltage = null;

            // Initalize the flag that indicates that we have found a nominal voltage
            bool NominalVoltageFound = false;

            // 1/√2
            const double Peak2Rms = 0.70710678118654757;

            // For each channel definition of this data source...
            foreach (OneChannelDefn ThisChannelDefn in ChannelDefns)
            {
                // If this is a voltage measurement...
                if (ThisChannelDefn.QuantityMeasuredID == ID_QM_VOLTAGE)
                {
                    // For each series definition of this channel definition...
                    foreach (OneSeriesDefn ThisSeriesDefn in ThisChannelDefn.SeriesDefns)
                    {
                        // If this series definition is for the non-temporal value of an instantaneous voltage or rms voltage measurement...
                        if (((ThisSeriesDefn.QuantityCharacteristicID == ID_QC_INSTANTANEOUS) || (ThisSeriesDefn.QuantityCharacteristicID == ID_QC_RMS)) && (ThisSeriesDefn.ValueTypeID != ID_SERIES_VALUE_TYPE_TIME))
                        {
                            // If we have a valid nominal quantity...
                            if (ThisSeriesDefn.SeriesNominalQuantity != null)
                            {
                                // If this nominal is defined for a peak voltage
                                if (ThisSeriesDefn.QuantityCharacteristicID == ID_QC_INSTANTANEOUS)

                                    // Scale it down by the square-root of two
                                    NominalVoltage = ThisSeriesDefn.SeriesNominalQuantity * Peak2Rms;

                                else

                                    // Use the rms voltage scaled as it is.
                                    NominalVoltage = ThisSeriesDefn.SeriesNominalQuantity;

                                // We found a nominal voltage.
                                NominalVoltageFound = true;

                            } // If we have a valid nominal quantity

                        } // If this series definition is for the non-temporal value of an rms voltage measurement

                        // Stop searching if we found a nominal voltage.
                        if (NominalVoltageFound)
                            break;

                    } // For each series definition of this channel definition

                } // If this is a voltage measurement

                // Stop searching if we found a nominal voltage.
                if (NominalVoltageFound)
                    break;

            } // For each channel definition

            // Return the nominal voltage
            return NominalVoltage;

        } // GetNominalVoltage        

    } // class RecDataSource

} // namespace PQDifValidator