/*
**  Class:          PQDIF.Validator.File
**  Description:	Reads PQDIF files from a File to a List of PQDIF Objects
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

    // Contains classes that represent ASCII and Unicode character encodings; abstract base classes for converting blocks of characters to and from blocks of bytes;
    // and a helper class that manipulates and formats String objects without creating intermediate instances of String.
    using System.Text;

    // Contains interfaces and classes that define generic collections, which allow users to create strongly typed collections that provide better type safety and performance than non-generic strongly typed collections.
    using System.Collections.Generic;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    // Defines IEEE Std 1159.3 PQDIF IDs
    using static PQDIFNet.Constants.Logical;

    /// <summary>
    /// For Reading and Writing PQDIF Objects in PQDIF Files
    /// </summary>
    public partial class File
    {
        /// <summary>
        /// File Name of the PQDIF file
        /// </summary>
        public string FileName = "";

        /// <summary>
        /// <c>true</c> if the PQDIF file has been read
        /// </summary>
        public bool FileRead = false;

        /// <summary>
        /// <c>true</c> if the PQDIF file has been read with success
        /// </summary>
        public bool FileOK = false;

        /// <summary>
        /// Application Logger
        /// </summary>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// PQDIF Compliance Logger
        /// </summary>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Maximum Number of Series Values to Export to XML Files
        /// </summary>
        public uint? MaxSeriesValues = null;

        /// <summary>
        /// List of PQDIF Records Holder Objects.
        /// </summary>
        public List<RecordHolder> RecordHolders = new List<RecordHolder>();


        /// <summary>
        /// Returns a sorted list of Data Source names (<see cref="RecDataSource.NameDS"/>) stored in the PQDIF file
        /// </summary>
        /// <returns>Sorted List <see cref="RecDataSource.NameDS"/></returns>
        public List<string> GetDataSourceNames()
        {
            // Initialize the return value
            List<string> SourceNames = new List<string>();

            // For each record in the PQDIF file...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)

                // If this is a data source record...
                if (ThisRecordHolder.RecType == RecType.RecDataSource)
                {
                    RecDataSource ThisRecDataSource = (RecDataSource)ThisRecordHolder.Record;

                    // If the list of data sources does not contain the current data source name...
                    if (!SourceNames.Contains(ThisRecDataSource.NameDS))

                        // Add the current data source name.
                        SourceNames.Add(ThisRecDataSource.NameDS);

                } // If this is a data source record

            // Sort the data source list
            if (SourceNames.Count > 0)
                SourceNames.Sort();

            // Return the list of data source names
            return SourceNames;

        } // GetDataSourceNames


        /// <summary>
        /// Returns the Earliest <see cref="RecObservation.TimeStart"/> in the the PQDIF File
        /// </summary>
        /// <value>Earliest <see cref="RecObservation.TimeStart"/> in the the PQDIF File, or <c>null</c> if PQDIF file has no <see cref="RecObservation"/> record</value>
        public DateTime? GetTimeStartMin()
        {
            // Initialize the earliest date in the PQDIF file.
            bool RecObservationFound = false;
            DateTime WhenStartMin = DateTime.MaxValue;

            // For each record in the PQDIF file...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)

                // If this is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    RecObservationFound = true;
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;
                    if (WhenStartMin > ThisRecObservation.TimeStart )
                        WhenStartMin = ThisRecObservation.TimeStart;
                }

            // If we found no observation records...
            if (!RecObservationFound)

                // Return null.
                return null;

            else

                // Return the earliest RecObservation.TimeStart found.
                return WhenStartMin;

        } // GetTimeStartMin


        /// <summary>
        /// Returns the Latest <see cref="RecObservation.TimeStart"/> in the the PQDIF File
        /// </summary>
        /// <value>Latest <see cref="RecObservation.TimeStart"/> in the the PQDIF File, or <c>null</c> if PQDIF file has no <see cref="RecObservation"/> record</value>
        public DateTime? GetTimeStartMax()
        {
            // Initialize the latest date in the PQDIF file.
            bool RecObservationFound = false;
            DateTime WhenStartMax = DateTime.MinValue;

            // For each record in the PQDIF file...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)

                // If this is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    RecObservationFound = true;
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;
                    if (WhenStartMax < ThisRecObservation.TimeStart)
                        WhenStartMax = ThisRecObservation.TimeStart;
                }

            // If we found no observation records...
            if (!RecObservationFound)

                // Return null.
                return null;

            else

                // Return the latest RecObservation.TimeStart found.
                return WhenStartMax;

        } // GetTimeStartMax


        /// <summary>
        /// Initializes a new instance of the <see cref="File"/> class.
        /// </summary>
        public File()
        {
            // Default Constructor

        } // File

        /// <summary>
        /// Returns the PQDIF File Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Container Record Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {            
            // Initialize a new string builder class and an XML file.
            StringBuilder StringBuilder = new StringBuilder();
            StringBuilder.Append(@"<?xml version=""1.0""?>");
            StringBuilder.Append(@"<records>");

            // For each object...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                if (ThisRecordHolder.RecType == RecType.Container)
                {
                    Container ThisContainer = (Container)ThisRecordHolder.Record;
                    StringBuilder.Append(ThisContainer.GetXML(ApplicationLog, ComplianceLog));
                }

                if (ThisRecordHolder.RecType == RecType.RecDataSource)
                {
                    RecDataSource ThisDataSource = (RecDataSource)ThisRecordHolder.Record;
                    StringBuilder.Append(ThisDataSource.GetXML(ApplicationLog, ComplianceLog));
                }

                if (ThisRecordHolder.RecType == RecType.RecMonitorSettings)
                {
                    RecMonitorSettings TheseMonitorSettings = (RecMonitorSettings)ThisRecordHolder.Record;
                    StringBuilder.Append(TheseMonitorSettings.GetXML(ApplicationLog, ComplianceLog));
                }

                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    RecObservation ThisObservationDataSource = (RecObservation)ThisRecordHolder.Record;
                    StringBuilder.Append(ThisObservationDataSource.GetXML(ApplicationLog, ComplianceLog, MaxSeriesValues));
                }

            } // For each object

            // Finish the XML file
            StringBuilder.Append(@"</records>");

            // Return the PQDIF file formatted as XML.
            return StringBuilder.ToString();

        } // GetXML

        /// <summary>
        /// Validates the Count of Channel Instances to the Count of Related Channel Definitions.
        /// </summary>
        private void ValidateChannelInstanceCount()
        {
            // For each record in the PQDIF file...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                // If this is an observation record and we have a valid corresponding data source record index...
                if ((ThisRecordHolder.RecType == RecType.RecObservation) && (ThisRecordHolder.RelatedRecDataSource != null))
                {
                    // Get a reference to the observation record.
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;

                    // If the related data source record is known...
                    if (ThisRecordHolder.RelatedRecDataSource != null)
                    {
                        // For each channel instance in this observation record...
                        foreach (OneChannelInst ThisChannelInst in ThisRecObservation.ChannelInstances)
                        {
                            // Determine the channel definition index for this channel instance
                            int ChannelDefnIdx = (int)ThisChannelInst.ChannelDefnIdx;

                            // If the channel definition index is valid...
                            if (ChannelDefnIdx < ThisRecordHolder.RelatedRecDataSource.ChannelDefns.Count)
                            {
                                // Get a reference to this channel definition.
                                OneChannelDefn ThisChannelDefn = ThisRecordHolder.RelatedRecDataSource.ChannelDefns[ChannelDefnIdx];

                                // If the count of series instances and series definitions do not match, then log a compliance error.
                                if (ThisChannelInst.SeriesInstances.Count != ThisChannelDefn.SeriesDefns.Count)
                                {
                                    // Log a compliance error.
                                    ThisChannelInst.LoggerCompliance.Log("The count of series instances, which is " + ThisChannelInst.SeriesInstances.Count.ToString() +
                                                                         ", should be equal to the count of series definitions in the related data source record. The count of channel definitions is " + 
                                                                         ThisChannelDefn.SeriesDefns.Count.ToString() + ".", LogLevels.Error);
                                }

                            } // If the data source record is valid

                        } // For each channel instance in this observation record

                    } // If the data source record is valid

                } // If this is an observation record and we have a valid corresponding data source record index

            } // For each record in the PQDIF file

        } // ValidateChannelInstanceCount


        /// <summary>
        /// Adds <see cref="LogEvent"/> records from the <see cref="Container"/>, <see cref="RecDataSource"/>, <see cref="RecMonitorSettings"/>, 
        /// and <see cref="RecObservation"/> records, their collections, and subcollections into <see cref="LoggerApplication"/> and <see cref="LoggerCompliance"/>
        /// </summary>
        private void AddLogsFromRecords()
        {
            // Initialize a record index
            int RecIndex = 0;

            // Initialize a prefix for log messages.
            string LogMessagePrefix;

            // For each object...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                
                // If this record a Container...
                if (ThisRecordHolder.RecType == RecType.Container)
                {
                    Container ThisContainer = (Container)ThisRecordHolder.Record;
                    LogMessagePrefix = "Container (RecIndex = " + RecIndex.ToString() + "): ";

                    // Copy the application and compliance logs from the data source record
                    CopyLogs(ThisContainer.LoggerApplication, ThisContainer.LoggerCompliance, LogMessagePrefix);
                  
                } // If this record a Container


                // Otherwise, if this record is a data source...
                else if (ThisRecordHolder.RecType == RecType.RecDataSource)
                {
                    RecDataSource ThisDataSource = (RecDataSource)ThisRecordHolder.Record;
                    LogMessagePrefix = "Data Source (RecIndex = " + RecIndex.ToString() + "): ";

                    // Copy the application and compliance logs from the data source record
                    CopyLogs(ThisDataSource.LoggerApplication, ThisDataSource.LoggerCompliance, LogMessagePrefix);

                    // Initialize a channel definition index
                    int ChannelDefnIdx = 0;

                    // For each channel definition...
                    foreach (OneChannelDefn ThisChannelDefn in ThisDataSource.ChannelDefns)
                    {
                        // Copy the application logs from the data source record
                        string LogChannelMessagePrefix = LogMessagePrefix + "Channel Definition " + ChannelDefnIdx.ToString() + ": ";
                        CopyLogs(ThisChannelDefn.LoggerApplication, ThisChannelDefn.LoggerCompliance, LogChannelMessagePrefix);

                        // Initialize a series definition index
                        int SeriesDefnIdx = 0;

                        // For each series definition...
                        foreach (OneSeriesDefn ThisSeriesDefn in ThisChannelDefn.SeriesDefns)
                        {
                            // Copy the application logs and compliance from the series definition.
                            string LogSeriesMessagePrefix = LogChannelMessagePrefix + "Series Definition " + SeriesDefnIdx.ToString() + ": ";
                            CopyLogs(ThisSeriesDefn.LoggerApplication, ThisSeriesDefn.LoggerCompliance, LogSeriesMessagePrefix);

                            // Increment the series definition index.
                            SeriesDefnIdx++;

                        } // For each channel definition

                        // Increment the channel definition index.
                        ChannelDefnIdx++;

                    } // For each channel definition

                } // Otherwise, if this record is a data source

                // Otherwise, if this record is a monitor setting...
                if (ThisRecordHolder.RecType == RecType.RecMonitorSettings)
                {
                    RecMonitorSettings TheseMonitorSettings = (RecMonitorSettings)ThisRecordHolder.Record;
                    LogMessagePrefix = "Monitor Settings Record (RecIndex = " + RecIndex.ToString() + "): ";

                    // Copy the application and compliance logs from the monitor settings record
                    CopyLogs(TheseMonitorSettings.LoggerApplication, TheseMonitorSettings.LoggerCompliance, LogMessagePrefix);

                    // Initialize a channel setting index
                    int ChannelSettingIdx = 0;

                    // For each channel setting...
                    foreach (OneChannelSetting ThisChannelSetting in TheseMonitorSettings.ChannelSettingsArray)
                    {
                        // Copy the application logs and compliance from the channel setting instance.
                        string LogChannelMessagePrefix = LogMessagePrefix + "Channel Setting " + ChannelSettingIdx.ToString() + ": ";
                        CopyLogs(ThisChannelSetting.LoggerApplication, ThisChannelSetting.LoggerCompliance, LogChannelMessagePrefix);

                        // Increment the channel setting index.
                        ChannelSettingIdx++;

                    } // For each channel setting

                } // Otherwise, if this record is a monitor setting


                // Otherwise, if this record is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    RecObservation ThisObservationDataSource = (RecObservation)ThisRecordHolder.Record;
                    LogMessagePrefix = "Observation Record (RecIndex = " + RecIndex.ToString() + "): ";

                    // Copy the application and compliance logs from the data source record
                    CopyLogs(ThisObservationDataSource.LoggerApplication, ThisObservationDataSource.LoggerCompliance, LogMessagePrefix);

                    // Initialize a channel instance index
                    int ChannelInstanceIdx = 0;

                    // For each channel instance...
                    foreach (OneChannelInst ThisChannelInst in ThisObservationDataSource.ChannelInstances)
                    {
                        // Copy the application logs and compliance from the series instance.
                        string LogChannelMessagePrefix = LogMessagePrefix + "Channel Instance " + ChannelInstanceIdx.ToString() + ": ";
                        CopyLogs(ThisChannelInst.LoggerApplication, ThisChannelInst.LoggerCompliance, LogChannelMessagePrefix);

                        // Initialize a series instance index
                        int SeriesInstanceIdx = 0;

                        // For each series instance...
                        foreach (OneSeriesInstance ThisSeriesInstance in ThisChannelInst.SeriesInstances)
                        {
                            // Copy the application logs and compliance from the series instance.
                            string LogSeriesMessagePrefix = LogChannelMessagePrefix + "Series Instance " + SeriesInstanceIdx.ToString() + ": ";
                            CopyLogs(ThisSeriesInstance.LoggerApplication, ThisSeriesInstance.LoggerCompliance, LogSeriesMessagePrefix);

                            // Increment the series instance index.
                            SeriesInstanceIdx++;

                        } // For each series instance

                        // Increment the channel instance index.
                        ChannelInstanceIdx++;

                    } // For each channel instance

                } // Otherwise, if this record is an observation record

                // Increment the record index 
                RecIndex++;

            } // For each object

        } // AddLogsFromRecords


        /// <summary>
        /// Performs a deep copy of logs from <paramref name="SubLoggerApplication"/> to <see cref="LoggerApplication"/> 
        /// and from <paramref name="SubLoggerCompliance"/> to <see cref="LoggerCompliance"/>
        /// </summary>
        /// <param name="SubLoggerApplication">Application Logger from which to copy log events.</param>
        /// <param name="SubLoggerCompliance">PQDIF Compliance Logger from which to copy log events</param>
        /// <param name="LogSeriesMessagePrefix">String to Prepend to each copied <see cref="LogEvent.Message"/></param>
        private void CopyLogs(Logger SubLoggerApplication, Logger SubLoggerCompliance, string LogSeriesMessagePrefix)
        {
            // Peform a deep copy the application logs by creating a new object with the same field values as the sublogger except for a prefix
            foreach (LogEvent ThisLogEvent in SubLoggerApplication.GetLogEventList())
            {
                LoggerApplication.Log(new LogEvent
                {
                    WhenLogged = ThisLogEvent.WhenLogged,
                    Message = LogSeriesMessagePrefix + ThisLogEvent.Message,
                    Exception = ThisLogEvent.Exception,
                    Level = ThisLogEvent.Level
                });
            }

            // Peform a deep copy the compliance logs by creating a new object with the same field values as the sublogger except for a prefix
            foreach (LogEvent ThisLogEvent in SubLoggerCompliance.GetLogEventList())
            {
                LoggerCompliance.Log(new LogEvent
                {
                    WhenLogged = ThisLogEvent.WhenLogged,
                    Message = LogSeriesMessagePrefix + ThisLogEvent.Message,
                    Exception = ThisLogEvent.Exception,
                    Level = ThisLogEvent.Level
                });
            }

        } // AddLogs


        /// <summary>
        /// Gets the earliest and latest time stamps in <see cref="OneChannelInst.SeriesInstances"/> of every <see cref="RecObservation"/> in <see cref="RecordHolders"/>
        /// </summary>
        /// <param name="TimeStampMin">Earliest Time Stamp Found in the <see cref="OneChannelInst.SeriesInstances"/> of the <see cref="RecObservation"/></param>
        /// <param name="TimeStampMax">Latest Time Stamp Found in the <see cref="OneChannelInst.SeriesInstances"/> of the <see cref="RecObservation"/></param>
        /// <returns><c>true</c> if <see cref="RecordHolders"/> contains a <see cref="RecObservation"/></returns>
        public bool GetTimeStampMinMax(ref DateTime TimeStampMin, ref DateTime TimeStampMax)
        {
            // Initialize return values.
            bool Result = false;
            TimeStampMin = DateTime.MaxValue;
            TimeStampMax = DateTime.MinValue;
            
            // For each record in the PQDIF file...
            foreach (RecordHolder RecordHolder in RecordHolders)
            {
                // If this is an observation record...
                if (RecordHolder.RecType == RecType.RecObservation)
                {
                    // Get the min and max time stamps for this oberservation record.                    
                    DateTime TimeStampMinRecObservation = DateTime.MaxValue;
                    DateTime TimeStampMaxRecObservation = DateTime.MinValue;

                    // If we can get min/max timestamps for this observation record...
                    if (GetRecObservationTimeStampMinMax(RecordHolder, ref TimeStampMinRecObservation, ref TimeStampMaxRecObservation))
                    {
                        // We found timestamp min/max values.
                        Result = true;

                        // Update the min/max timestamps for this entire PQDIF file
                        if (TimeStampMin > TimeStampMinRecObservation)
                            TimeStampMin = TimeStampMinRecObservation;

                        if (TimeStampMax < TimeStampMaxRecObservation)
                            TimeStampMax = TimeStampMaxRecObservation;

                    } // If we can get min/max timestamps for this observation record

                } // If this is an observation record

            } // For each record in the PQDIF file

            // Return true if at least one observation record was found.
            return Result;

        } // GetTimeStampMinMax


        /// <summary>
        /// Gets the earliest and latest time stamps in <see cref="OneChannelInst.SeriesInstances"/> if <paramref name="RecordHolder"/> contains a <see cref="RecObservation"/>
        /// </summary>
        /// <param name="RecordHolder">Record holder that contains a <see cref="RecObservation"/> object for which we want earliest and latest time stamps</param>
        /// <param name="TimeStampMin">Earliest Time Stamp Found in the <see cref="OneChannelInst.SeriesInstances"/> of the <see cref="RecObservation"/></param>
        /// <param name="TimeStampMax">Latest Time Stamp Found in the <see cref="OneChannelInst.SeriesInstances"/> of the <see cref="RecObservation"/></param>
        /// <returns><c>true</c> if <paramref name="RecordHolder"/> contains a <see cref="RecObservation"/></returns>
        public bool GetRecObservationTimeStampMinMax(RecordHolder RecordHolder, ref DateTime TimeStampMin, ref DateTime TimeStampMax)
        {
            // Initialize return values.
            bool Result = false;
            TimeStampMin = DateTime.MaxValue;
            TimeStampMax = DateTime.MinValue;

            // If this is an observation record...
            if (RecordHolder.RecType == RecType.RecObservation) 
            {
                // This record contains an observation record
                Result = true;

                // Get a reference to the observation record
                RecObservation RecObservation = (RecObservation)RecordHolder.Record;

                // Initialize the time stamps to the observation event's time stamp.
                TimeStampMin = RecObservation.TimeStart;
                TimeStampMax = RecObservation.TimeStart;

                // Get the data source record that precedes the observation record.
                RecDataSource RecDataSource = RecordHolder.RelatedRecDataSource;
                
                // For each channel instance...
                foreach (OneChannelInst ThisChannelInst in RecObservation.ChannelInstances)
                {
                    DateTime ThisTimeStampMin = DateTime.MaxValue;
                    DateTime ThisTimeStampMax = DateTime.MinValue;

                    // Determine the channel definition for this channel instance
                    OneChannelDefn ThisChannelDefn = ThisChannelInst.GetChannelDefn(RecDataSource);

                    // If the channel definition is valid...
                    if (ThisChannelDefn != null)
                    {
                        // If we found time stamps within the series instances...
                        if (ThisChannelInst.GetTimeStartMinMax(RecObservation, ThisChannelDefn, ref ThisTimeStampMin, ref ThisTimeStampMax))
                        {
                            // Update the min/max timestamps for this RecObservation
                            if (TimeStampMin > ThisTimeStampMin)
                                TimeStampMin = ThisTimeStampMin;

                            if (TimeStampMax < ThisTimeStampMax)
                                TimeStampMax = ThisTimeStampMax;

                        } // If we found time stamps within the series instances

                    } // If the channel definition is valid

                } // For each channel instance

            } // If this is an observation record

            // Return true if RecordHolder contains a RecObservation
            return Result;

        } // GetRecObservationTimeStampMinMax


        /// <summary>
        /// Gets the list of <see cref="tagQuantityTypeID"/> values stored in all of the <see cref="RecDataSource.ChannelDefns"/> objects associated
        /// with all <see cref="RecObservation"/> objects of the PQDIF file stored in <see cref="RecordHolders"/>
        /// </summary>
        /// <returns>List of all <see cref="tagQuantityTypeID"/> values in the PQDIF file stored in <see cref="RecordHolders"/></returns>
        public List<Guid> GetQuantityTypeIDs()
        {
            // Initialize the return value
            List<Guid> ListQuantityTypeID = new List<Guid>();

            // For each record holder...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                // If this is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    // Get the list of QuantityTypeID values stored in this record.
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;
                    List<Guid> TheseQuantityTypeIDs = GetQuantityTypeIDs(ThisRecordHolder, ThisRecObservation);

                    // IF there are new QuantityTypeID values, add them to list for all observation records.
                    foreach (Guid QuantityTypeID in TheseQuantityTypeIDs)
                        if (!ListQuantityTypeID.Contains(QuantityTypeID))
                            ListQuantityTypeID.Add(QuantityTypeID);

                } // If this is an observation record

            } // For each record holder

            // Return the list of Quantity Type IDs for all observation record.
            return ListQuantityTypeID;

        } // GetQuantityTypeIDs


        /// <summary>
        /// Gets the list of <see cref="tagQuantityTypeID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/>
        /// </summary>
        /// <param name="RecordHolder">Record holder that contains a <see cref="RecObservation"/> object for which we want a list of <see cref="tagQuantityTypeID"/> values</param>
        /// <param name="RecObservation">Observation record for which for which we want a list of <see cref="tagQuantityTypeID"/> values</param>
        /// <returns>List of <see cref="tagQuantityTypeID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/></returns>
        public List<Guid> GetQuantityTypeIDs(RecordHolder RecordHolder, RecObservation RecObservation)
        {
            // Initialize the return value
            List<Guid> ListQuantityTypeID = new List<Guid>();

            // If this is an observation record and we have a related data source record...
            if ((RecordHolder.RecType == RecType.RecObservation) && (RecordHolder.RelatedRecDataSource != null))
            {
                // For each channel instance...
                foreach (OneChannelInst ThisChannelInst in RecObservation.ChannelInstances)
                { 
                    // Determine the channel definition for this channel instance
                    OneChannelDefn ThisChannelDefn = ThisChannelInst.GetChannelDefn(RecordHolder.RelatedRecDataSource);

                    // If the channel definition is valid and the quantity type is not already in the list of quantity type IDs...
                    if ((ThisChannelDefn != null) && !ListQuantityTypeID.Contains(ThisChannelDefn.QuantityTypeID))
                    
                        // Add the quantity type ID to the list of quanity type IDs.
                            ListQuantityTypeID.Add(ThisChannelDefn.QuantityTypeID);                    

                } // For each channel instance

            } // If this is an observation record and we have a related data source record...

            // Return the list of quantity type IDs for this observation record.
            return ListQuantityTypeID;

        } // GetQuantityTypeIDs


        /// <summary>
        /// Gets the list of <see cref="tagPhaseID"/> values stored in all of the <see cref="RecDataSource.ChannelDefns"/> objects associated
        /// with all <see cref="RecObservation"/> objects of the PQDIF file stored in <see cref="RecordHolders"/>
        /// </summary>
        /// <returns>List of all <see cref="tagPhaseID"/> values in the PQDIF file stored in <see cref="RecordHolders"/></returns>
        public List<uint> GetPhaseIDs()
        {
            // Initialize the return value
            List<uint> ListPhaseID = new List<uint>();

            // For each record holder...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                // If this is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    // Get the list of PhaseID values stored in this record.
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;
                    List<uint> ThesePhaseIDs = GetPhaseIDs(ThisRecordHolder, ThisRecObservation);

                    // IF there are new PhaseID values, add them to list for all observation records.
                    foreach (uint PhaseID in ThesePhaseIDs)
                        if (!ListPhaseID.Contains(PhaseID))
                            ListPhaseID.Add(PhaseID);

                } // If this is an observation record

            } // For each record holder

            // Return the list of PhaseID IDs for all observation record.
            return ListPhaseID;

        } // GetPhaseIDs



        /// <summary>
        /// Gets the list of <see cref="tagPhaseID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/>
        /// </summary>
        /// <param name="RecordHolder">Record holder that contains a <see cref="RecObservation"/> object for which we want a list of <see cref="tagPhaseID"/> values</param>
        /// <param name="RecObservation">Observation record for which for which we want a list of <see cref="tagPhaseID"/> values</param>
        /// <returns>List of <see cref="tagPhaseID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/></returns>
        public List<uint> GetPhaseIDs(RecordHolder RecordHolder, RecObservation RecObservation)
        {
            // Initialize the return value
            List<uint> ListPhaseID = new List<uint>();

            // If this is an observation record and we have a related data source record...
            if ((RecordHolder.RecType == RecType.RecObservation) && (RecordHolder.RelatedRecDataSource != null))
            {
                // For each channel instance...
                foreach (OneChannelInst ThisChannelInst in RecObservation.ChannelInstances)
                {
                    // Determine the channel definition for this channel instance
                    OneChannelDefn ThisChannelDefn = ThisChannelInst.GetChannelDefn(RecordHolder.RelatedRecDataSource);

                    // If the channel definition is valid and the phase ID is not already in the list of phase IDs...
                    if ((ThisChannelDefn != null) && ! ListPhaseID.Contains(ThisChannelDefn.PhaseID))

                        // Add the phase ID to the list of phase IDs.
                        ListPhaseID.Add(ThisChannelDefn.PhaseID);

                } // For each channel instance

            } // If this is an observation record and we have a related data source record...

            // Return the list of phase IDs for this observation record.
            return ListPhaseID;

        } // GetPhaseIDs


        /// <summary>
        /// Gets the list of <see cref="tagQuantityMeasuredID"/> values stored in all of the <see cref="RecDataSource.ChannelDefns"/> objects associated
        /// with all <see cref="RecObservation"/> objects of the PQDIF file stored in <see cref="RecordHolders"/>
        /// </summary>
        /// <returns>List of all <see cref="tagQuantityMeasuredID"/> values in the PQDIF file stored in <see cref="RecordHolders"/></returns>
        public List<uint> GetQuantityMeasuredIDs()
        {
            // Initialize the return value
            List<uint> ListQuantityMeasuredID = new List<uint>();

            // For each record holder...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                // If this is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    // Get the list of QuantityMeasuredID values stored in this record.
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;
                    List<uint> TheseQuantityMeasuredIDs = GetQuantityMeasuredIDs(ThisRecordHolder, ThisRecObservation);

                    // IF there are new QuantityMeasuredID values, add them to list for all observation records.
                    foreach (uint QuantityMeasuredID in TheseQuantityMeasuredIDs)
                        if (! ListQuantityMeasuredID.Contains(QuantityMeasuredID))
                            ListQuantityMeasuredID.Add(QuantityMeasuredID);

                } // If this is an observation record

            } // For each record holder

            // Return the list of quantity measured IDs for all observation records.
            return ListQuantityMeasuredID;

        } // GetQuantityMeasuredIDs


        /// <summary>
        /// Gets the list of <see cref="tagQuantityMeasuredID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/>
        /// </summary>
        /// <param name="RecordHolder">Record holder that contains a <see cref="RecObservation"/> object for which we want a list of <see cref="tagQuantityMeasuredID"/> values</param>
        /// <param name="RecObservation">Observation record for which for which we want a list of <see cref="tagQuantityMeasuredID"/> values</param>
        /// <returns>List of <see cref="tagQuantityMeasuredID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/></returns>
        public List<uint> GetQuantityMeasuredIDs(RecordHolder RecordHolder, RecObservation RecObservation)
        {
            // Initialize the return value
            List<uint> ListQuantityMeasuredID = new List<uint>();

            // If this is an observation record and we have a related data source record...
            if ((RecordHolder.RecType == RecType.RecObservation) && (RecordHolder.RelatedRecDataSource != null))
            {
                // For each channel instance...
                foreach (OneChannelInst ThisChannelInst in RecObservation.ChannelInstances)
                {
                    // Determine the channel definition for this channel instance
                    OneChannelDefn ThisChannelDefn = ThisChannelInst.GetChannelDefn(RecordHolder.RelatedRecDataSource);

                    // If the channel definition is valid and the quantity measured ID is not already in the list of quantity measured IDs...
                    if ((ThisChannelDefn != null) && !ListQuantityMeasuredID.Contains(ThisChannelDefn.QuantityMeasuredID))

                        // Add the quantity type ID to the list of quanity measured IDs.
                        ListQuantityMeasuredID.Add(ThisChannelDefn.QuantityMeasuredID);

                } // For each channel instance

            } // If this is an observation record and we have a related data source record...

            // Return the list of quantity measured IDs for this observation record.
            return ListQuantityMeasuredID;

        } // GetQuantityMeasuredIDs


        /// <summary>
        /// Gets the list of <see cref="tagQuantityCharacteristicID"/> values stored in all of the <see cref="RecDataSource.ChannelDefns"/> objects associated
        /// with all <see cref="RecObservation"/> objects of the PQDIF file stored in <see cref="RecordHolders"/>
        /// </summary>
        /// <returns>List of all <see cref="tagQuantityCharacteristicID"/> values in the PQDIF file stored in <see cref="RecordHolders"/></returns>
        public List<Guid> GetQuantityCharacteristicIDs()
        {
            // Initialize the return value
            List<Guid> ListQuantityCharacteristicID = new List<Guid>();

            // For each record holder...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                // If this is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    // Get the list of QuantityCharacteristicID values stored in this record.
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;
                    List<Guid> TheseQuantityCharacteristicIDs = GetQuantityCharacteristicIDs(ThisRecordHolder, ThisRecObservation);

                    // IF there are new QuantityCharacteristicID values, add them to list for all observation records.
                    foreach (Guid QuantityCharacteristicID in TheseQuantityCharacteristicIDs)
                        if (! ListQuantityCharacteristicID.Contains(QuantityCharacteristicID))
                            ListQuantityCharacteristicID.Add(QuantityCharacteristicID);

                } // If this is an observation record

            } // For each record holder

            // Return the list of Quantity Characteristic IDs for all observation records.
            return ListQuantityCharacteristicID;

        } // GetQuantityCharacteristicIDs


        /// <summary>
        /// Gets the list of <see cref="tagQuantityCharacteristicID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/>
        /// </summary>
        /// <param name="RecordHolder">Record holder that contains a <see cref="RecObservation"/> object for which we want a list of <see cref="tagQuantityCharacteristicID"/> values</param>
        /// <param name="RecObservation">Observation record for which for which we want a list of <see cref="tagQuantityCharacteristicID"/> values</param>
        /// <returns>List of <see cref="tagQuantityCharacteristicID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/></returns>
        public List<Guid> GetQuantityCharacteristicIDs(RecordHolder RecordHolder, RecObservation RecObservation)
        {
            // Initialize the return value
            List<Guid> ListQuantityCharacteristicID = new List<Guid>();

            // If this is an observation record and we have a related data source record...
            if ((RecordHolder.RecType == RecType.RecObservation) && (RecordHolder.RelatedRecDataSource != null))
            {
                // For each channel instance...
                foreach (OneChannelInst ThisChannelInst in RecObservation.ChannelInstances)
                {
                    // Determine the channel definition for this channel instance
                    OneChannelDefn ThisChannelDefn = ThisChannelInst.GetChannelDefn(RecordHolder.RelatedRecDataSource);

                    // If the channel definition is valid...
                    if (ThisChannelDefn != null)
                    {
                        // For each series definition in this channel definition...
                        foreach (OneSeriesDefn ThisSeriesDefn in ThisChannelDefn.SeriesDefns)
                        {
                            // If the Quantity Characteristic ID is not already in the list of quantity types...
                            if (! ListQuantityCharacteristicID.Contains(ThisSeriesDefn.QuantityCharacteristicID))

                                // Add this Quantity Characteristic ID to the list of Quantity Characteristic IDs.
                                ListQuantityCharacteristicID.Add(ThisSeriesDefn.QuantityCharacteristicID);

                        } // For each series instance in this channel instance                    

                    } // If the channel definition is valid                    

                } // For each channel instance

            } // If this is an observation record and we have a related data source record...

            // Return the list of quantity types for this observation record.
            return ListQuantityCharacteristicID;

        } // GetQuantityCharacteristicIDs


        /// <summary>
        /// Gets the list of <see cref="tagValueTypeID"/> values stored in all of the <see cref="RecDataSource.ChannelDefns"/> objects associated
        /// with all <see cref="RecObservation"/> objects of the PQDIF file stored in <see cref="RecordHolders"/>
        /// </summary>
        /// <returns>List of all <see cref="tagValueTypeID"/> values in the PQDIF file stored in <see cref="RecordHolders"/></returns>
        public List<Guid> GetValueTypeIDs()
        {
            // Initialize the return value
            List<Guid> ListValueTypeIDs = new List<Guid>();

            // For each record holder...
            foreach (RecordHolder ThisRecordHolder in RecordHolders)
            {
                // If this is an observation record...
                if (ThisRecordHolder.RecType == RecType.RecObservation)
                {
                    // Get the list of ValueTypeID values stored in this record.
                    RecObservation ThisRecObservation = (RecObservation)ThisRecordHolder.Record;
                    List<Guid> TheseValueTypeIDs = GetValueTypeIDs(ThisRecordHolder, ThisRecObservation);

                    // IF there are new ValueTypeID values, add them to list for all observation records.
                    foreach (Guid ValueTypeID in TheseValueTypeIDs)
                        if (! ListValueTypeIDs.Contains(ValueTypeID))
                            ListValueTypeIDs.Add(ValueTypeID);

                } // If this is an observation record

            } // For each record holder

            // Return the list of Value Type IDs for all observation records.
            return ListValueTypeIDs;

        } // GetValueTypeIDs


        /// <summary>
        /// Gets the list of <see cref="tagValueTypeID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/>
        /// </summary>
        /// <param name="RecordHolder">Record holder that contains a <see cref="RecObservation"/> object for which we want a list of <see cref="tagQuantityCharacteristicID"/> values</param>
        /// <param name="RecObservation">Observation record for which we want a list of <see cref="tagValueTypeID"/> values</param>
        /// <returns>List of <see cref="tagValueTypeID"/> values stored in the <see cref="RecDataSource.ChannelDefns"/> associated with <paramref name="RecObservation"/></returns>
        public List<Guid> GetValueTypeIDs(RecordHolder RecordHolder, RecObservation RecObservation)
        {
            // Initialize the return value
            List<Guid> ListValueTypeIDs = new List<Guid>();

            // If this is an observation record and we have a related data source record...
            if ((RecordHolder.RecType == RecType.RecObservation) && (RecordHolder.RelatedRecDataSource != null))
            {
                // For each channel instance...
                foreach (OneChannelInst ThisChannelInst in RecObservation.ChannelInstances)
                {
                    // Determine the channel definition for this channel instance
                    OneChannelDefn ThisChannelDefn = ThisChannelInst.GetChannelDefn(RecordHolder.RelatedRecDataSource);

                    // If the channel definition is valid...
                    if (ThisChannelDefn != null)
                    {
                        // For each series definition in this channel definition...
                        foreach (OneSeriesDefn ThisSeriesDefn in ThisChannelDefn.SeriesDefns)
                        {
                            // If the Value Type ID is not already in the list of Value Type IDs...
                            if (! ListValueTypeIDs.Contains(ThisSeriesDefn.ValueTypeID))

                                // Add this Value Type ID to the list of Value Type IDs
                                ListValueTypeIDs.Add(ThisSeriesDefn.ValueTypeID);

                        } // For each series instance in this channel instance                    

                    } // If the channel definition is valid                    

                } // For each channel instance

            } // If this is an observation record and we have a related data source record...

            // Return the list of value type IDs for this observation record.
            return ListValueTypeIDs;

        } // GetQuantityCharacteristicIDs


        /// <summary>
        /// Returns the nominal voltage for this data source from the first channel definition with <see cref="OneChannelDefn.QuantityMeasuredID"/> equal to <see cref="ID_QM_VOLTAGE"/>
        /// that has a series definition with <see cref="OneSeriesDefn.QuantityCharacteristicID"/> equal to <see cref="ID_QC_RMS"/> or <see cref="ID_QC_INSTANTANEOUS"/> 
        /// </summary>
        /// <param name="RecordHolder">PQDIF Record Holder</param>
        /// <param name="RecObservation">PQDIF Observation Record for which we want a Nominal Voltage</param>
        /// <returns>Nominal Voltage for this Data Source</returns>
        public double? GetNominalVoltage(RecordHolder RecordHolder, RecObservation RecObservation)
        {
            // Initialize the return value
            double? NominalVoltage = null;

            // Initalize the flag that indicates that we have found a nominal voltage
            bool NominalVoltageFound = false;

            // 1/√2
            const double Peak2Rms = 0.70710678118654757;

            // If we have a valid data source for this observation record...
            if (RecordHolder.RelatedRecDataSource != null)
            {
                // Get a reference to this data source record.
                RecDataSource ThisRecDataSource = RecordHolder.RelatedRecDataSource;

                // For each channel instance of this observation record...
                foreach (OneChannelInst ThisChannelInst in RecObservation.ChannelInstances)
                {
                    // Get a reference to the channel definition that corresponds to this channel instance.
                    OneChannelDefn ThisChannelDefn = ThisChannelInst.GetChannelDefn(ThisRecDataSource);

                    // If this is a voltage measurement...
                    if (ThisChannelDefn.QuantityMeasuredID == ID_QM_VOLTAGE)
                    {
                        // If we have the same number of channel definitions and series definitions...
                        if (ThisChannelDefn.SeriesDefns.Count == ThisChannelInst.SeriesInstances.Count)
                        {
                            // For each series instance of this channel instance...
                            for (int SeriesIndex = 0; SeriesIndex < ThisChannelInst.SeriesInstances.Count; SeriesIndex++)
                            {
                                // Get a rerence to the current series instance and related series definition.
                                OneSeriesDefn ThisSeriesDefn = ThisChannelDefn.SeriesDefns[SeriesIndex];
                                OneSeriesInstance ThisSeriesInstance = ThisChannelInst.SeriesInstances[SeriesIndex];

                                // If this series definition is for the non-temporal value of an instantaneous voltage or rms voltage measurement...
                                if (((ThisSeriesDefn.QuantityCharacteristicID == ID_QC_INSTANTANEOUS) || (ThisSeriesDefn.QuantityCharacteristicID == ID_QC_RMS)) && (ThisSeriesDefn.ValueTypeID != ID_SERIES_VALUE_TYPE_TIME))
                                {
                                    // If we have a valid nominal quantity...
                                    if (ThisSeriesInstance.SeriesBaseQuantity != null)
                                    {
                                        // If this nominal is defined for a peak voltage
                                        if (ThisSeriesDefn.QuantityCharacteristicID == ID_QC_INSTANTANEOUS)

                                            // Scale it down by the square-root of two.
                                            NominalVoltage = ThisSeriesInstance.SeriesBaseQuantity * Peak2Rms;

                                        else

                                            // Use the rms voltage scaled as it is.
                                            NominalVoltage = ThisSeriesInstance.SeriesBaseQuantity;

                                        // We found a nominal voltage.
                                        NominalVoltageFound = true;

                                    } // If we have a valid nominal quantity

                                } // If this series definition is for the non-temporal value of an rms voltage measurement

                                // Stop searching if we found a nominal voltage.
                                if (NominalVoltageFound)
                                    break;

                            } // For each series definition of this channel definition

                        } // If we have the same number of channel definitions and series definitions
                        
                    } // If this is a voltage measurement

                    // Stop searching if we found a nominal voltage.
                    if (NominalVoltageFound)
                        break;

                } // For each channel instance of this observation record


                // If we did not find a nominal voltage in a series instance of the observation record, then look in its data source record.
                if (!NominalVoltageFound)                
                    NominalVoltage = ThisRecDataSource.GetNominalVoltage();
                

            } // If we have a valid data source for this observation record

            // Return the nominal voltage
            return NominalVoltage;

        } // GetNominalVoltage


        /// <summary>
        /// Returns Local Standard Time converted from Coordinated Universal Time (UTC)
        /// </summary>
        /// <param name="RecordHolder">PQDIF Record Holder</param>
        /// <param name="TimeStampUTC">Coordinated Universal Time (UTC)</param>
        /// <returns>Local Standard Time</returns>
        /// <remarks>This method uses tagUTCtoLST to convert from UTC to LTC stored in the data source record or related data source record.</remarks>
        public DateTime GetTimeStampLST(RecordHolder RecordHolder, DateTime TimeStampUTC)
        {
            // Initialize the UTC to LST offset.
            double UTCtoLST = 0;

            // If the record is a data source record...
            if (RecordHolder.RecType == RecType.RecDataSource)
            {
                // Get the UTC-to-LST offset for this data sourc record.
                RecDataSource ThisRecDataSource = (RecDataSource)RecordHolder.Record;

                // If the UTC-to-LST offset is valid...
                if (ThisRecDataSource.UTCtoLST != null)

                    // Store the UTC-to-LST offset from this data source record.
                    UTCtoLST = (double)ThisRecDataSource.UTCtoLST;
            }

            // Othewise, if we have a valid data source for this observation record...
            else if (RecordHolder.RelatedRecDataSource != null)
            {
                // If the UTC-to-LST offset from the related data source is valid...
                if (RecordHolder.RelatedRecDataSource.UTCtoLST != null)

                    // Store the UTC-to-LST offset from this related data source record.
                    UTCtoLST = (double)RecordHolder.RelatedRecDataSource.UTCtoLST;

            } // If the record is a data source record


            // Return the Local Standard Time
            return TimeStampUTC.AddSeconds(UTCtoLST);


        } // GetTimeStampLST



        /// <summary>
        /// Returns Local Time converted from Coordinated Universal Time (UTC)
        /// </summary>
        /// <param name="RecordHolder">PQDIF Record Holder</param>
        /// <param name="TimeStampUTC">Coordinated Universal Time (UTC)</param>
        /// <returns>Local Time</returns>
        /// <remarks>To convert from from UTC to Local Time, this method uses the tagUTCtoLST value stored in the data source record 
        /// or related data source record. It also uses tagLSTtoLocal, which is stored in the observation record 
        /// or related monitor settings record.</remarks>
        public DateTime GetTimeStampLocal(RecordHolder RecordHolder, DateTime TimeStampUTC)
        {

            // Initialize the UTC-to-LST and LST-to-Local offsets
            double UTCtoLST = 0;
            double LSTtoLocal = 0;

            // If the record is a data source record...
            if (RecordHolder.RecType == RecType.RecDataSource)
            {
                // Get a reference to this data source record.
                RecDataSource ThisRecDataSource = (RecDataSource)RecordHolder.Record;

                // If the UTC-to-LST offset for this data source record is valid...
                if (ThisRecDataSource.UTCtoLST != null)

                    // Store the UTC-to-LST offset for this data source record.
                    UTCtoLST = (double)ThisRecDataSource.UTCtoLST;

            } // If the record is a data source record

            // Otherwise, if the record is a monitor settings record...
            else if (RecordHolder.RecType == RecType.RecMonitorSettings)
            {
                // Get a reference to this monitor settings record.
                RecMonitorSettings ThisRecMonitorSettings = (RecMonitorSettings)RecordHolder.Record;

                // If the LST-to-Local offset for this monitor settings record is valid...
                if (ThisRecMonitorSettings.LSTtoLocal != null)
                
                    // Store the LST-to-Local offset for this monitor settings record.
                    LSTtoLocal = (double)ThisRecMonitorSettings.LSTtoLocal;
                

                // If there is a related data source record...
                if (RecordHolder.RelatedRecDataSource != null)

                    // If the UTC-to-LTC offset for the data source record is valid...
                    if (RecordHolder.RelatedRecDataSource.UTCtoLST != null)

                        // Store the UTC-to-LST offset.
                        UTCtoLST = (double)RecordHolder.RelatedRecDataSource.UTCtoLST;

            } // Otherwise, if the record is a monitor settings record


            // Otherwise, if the record is an observation record...
            else if (RecordHolder.RecType == RecType.RecObservation)
            {
                // Get a reference to this observation record.
                RecObservation ThisRecObservation = (RecObservation)RecordHolder.Record;

                // If the LST-to-Local offset for this observation record is valid...
                if (ThisRecObservation.LSTtoLocal != null)
                    LSTtoLocal = (double)ThisRecObservation.LSTtoLocal;

                // Otherwise, if there is a related monitor settings record...
                else if (RecordHolder.RelatedRecMonitorSettings != null)
                {
                 
                    // Store the LST-to-Local offset for this monitor settings record.
                    LSTtoLocal = (double)RecordHolder.RelatedRecMonitorSettings.LSTtoLocal;

                } // Otherwise, if there is a related monitor settings record

                // Get the UTC-to-LST offset from the related data source record.
                if (RecordHolder.RelatedRecDataSource != null)
                    if (RecordHolder.RelatedRecDataSource.UTCtoLST != null)
                        UTCtoLST = (double)RecordHolder.RelatedRecDataSource.UTCtoLST;

            } // Otherwise, if the record is an observation record


            // Return Local Time
            return TimeStampUTC.AddSeconds(UTCtoLST + LSTtoLocal);

        } // GetTimeStampLocal


    } // Class File

} // namespace PQDIF.Validator