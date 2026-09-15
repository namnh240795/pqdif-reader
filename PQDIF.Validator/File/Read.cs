/*
**  Class:          PQDIF.Validator.File
**  Description:	Reads PQDIF files from a File to a List of PQDIF Objects
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

    // Defines IEEE PQDIF IDs and data types including PQDIF timestamps
    using PQDIFNet;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    // Defines IEEE PQDIF IDs and data types including PQDIF timestamps
    using static PQDIFNet.Constants.Logical;

    /// <summary>
    /// For Reading and Writing PQDIF Objects in PQDIF Files
    /// </summary>
    public partial class File
    {       

        /// <summary>
        /// Reads the PQDIF file specified by <paramref name="FileName"/>
        /// </summary>
        /// <param name="FileName">PQDIF File to Read</param>
        /// <returns><c>true</c> if the file was read OK.</returns>
        public bool Read(string FileName)
        {
            this.FileName = FileName;
            return Read();
        }

        /// <summary>
        /// Reads the PQDIF file specified by <see cref="FileName"/>
        /// </summary>
        /// <returns><c>true</c> if the file was read OK.</returns>
        public bool Read()
        {
            // Declare a new PQDIF file
            CPQDIFNet PqdifFile = null;

            // Create a string for logging
            string LogMessagePrefix = "Opening PQDIF File";

            try
            {
                // Clear the logs.
                LoggerApplication.LogsClear();
                LoggerCompliance.LogsClear();

                // Initialize a new PQDIF file
                PqdifFile = new CPQDIFNet
                {
                    FlatFileName = FileName
                };

                // Initialize the flag that says if the file was read OK
                FileOK = false;

                // If we could read the PQDIF file...
                if (PqdifFile.Read())
                {
                    // The file was read OK.
                    FileOK = true;

                    // Initialize a flag for reading records.
                    bool RecordOK = false;

                    // Initialize flags that tell us if required PQDIF records were found.
                    bool ContainerFound = false;
                    bool RecDataSourceFound = false;
                    bool RecObservationFound = false;

                    // Get the number of records in the PQDIF file.
                    int RecCount = PqdifFile.RecordGetCount();
                    LoggerApplication.Log("PQDIF File Read OK. Number of Records = " + RecCount.ToString(), LogLevels.Info);

                    // Initialize the last data source record and monitor settings record found.
                    RecDataSource LastRecDataSource = null;
                    RecMonitorSettings LastRecMonitorSettings = null;

                    // For each record...
                    for (int RecIndex = 0; RecIndex < RecCount; RecIndex++)
                    {
                        // Initialize PQDIF record information.
                        Guid tagRecordGUID = new Guid();
                        string RecordTypeName = "";
                        int SizeHeader = 0;
                        int SizeRecord = 0;
                        int posThisRecord = 0;
                        int posNextRecord = 0;

                        // If we can get the record info...
                        if (PqdifFile.RecordGetInfo(RecIndex, ref tagRecordGUID, ref RecordTypeName, ref SizeHeader, ref SizeRecord, ref posThisRecord, ref posNextRecord))
                        {

                            // Initialize a new pointer.
                            IntPtr PointerCollection = new IntPtr();

                            // Specify a log message this record.
                            LogMessagePrefix = "Record " + RecIndex.ToString();

                            // If we can get the get a pointer to the collection...
                            if (PqdifFile.RecordGetCollection(RecIndex, ref PointerCollection))
                            {

                                // If the top-level record is a collection...
                                if (PQDIFNetWrapper.IsElementCollection(PqdifFile, PointerCollection, LoggerApplication))
                                {

                                    // If the record is a container record...
                                    if (tagRecordGUID == tagContainer)
                                    {
                                        // Log a message.
                                        LoggerApplication.Log(LogMessagePrefix + ": Container Found: " + SizeRecord.ToString() + " bytes", LogLevels.Info);
                                        ContainerFound = true;

                                        // Create a new container record from the PQDIF file
                                        Container NewContainer = new Container(PqdifFile, ref PointerCollection, out RecordOK);

                                        // If we read the container...
                                        if (RecordOK)

                                            // Add the container record to list of records.
                                            RecordHolders.Add(new RecordHolder
                                            {
                                                RecIndex = RecIndex,
                                                RecType = RecType.Container,
                                                Record = NewContainer,
                                                RelatedRecDataSource = null,
                                                RelatedRecMonitorSettings = null
                                            });
                                       

                                        // Log a compliance message if this was not the first record
                                        if ((RecIndex != 0) && (RecCount > 1))
                                            LoggerCompliance.Log(LogMessagePrefix + ": tagContainer should be the first top-level record of a PQDIF file.", LogLevels.Error);
                                    }

                                    // Otherwise, if the record is a data source record...
                                    else if (tagRecordGUID == tagRecDataSource)
                                    {
                                        // Log a message.
                                        LoggerApplication.Log(LogMessagePrefix + ": Data Source Record Found: " + SizeRecord.ToString() + " bytes", LogLevels.Info);
                                        RecDataSourceFound = true;

                                        // Create a new data source record from the PQDIF file
                                        RecDataSource NewDataSource = new RecDataSource(PqdifFile, ref PointerCollection, LoggerApplication.LogLevel, LoggerCompliance.LogLevel, out RecordOK);

                                        // If we read the data source record...
                                        if (RecordOK)

                                            // Add the data source record to list of records.
                                            RecordHolders.Add(new RecordHolder
                                            {
                                                RecIndex = RecIndex,
                                                RecType = RecType.RecDataSource,
                                                Record = NewDataSource,
                                                RelatedRecDataSource = null,
                                                RelatedRecMonitorSettings = null
                                            });

                                        // Remember this as the last data source record found.
                                        LastRecDataSource = NewDataSource;
                                    } 


                                    // Otherwise, if the record is a monitor settings record...
                                    else if (tagRecordGUID == tagRecMonitorSettings)
                                    {
                                        // Log a message.
                                        LoggerApplication.Log(LogMessagePrefix + ": Monitor Settings Record Found: " + SizeRecord.ToString() + " bytes", LogLevels.Info);
                                        RecDataSourceFound = true;

                                        // Create a new data source record from the PQDIF file
                                        RecMonitorSettings NewMonitorSettings = new RecMonitorSettings(PqdifFile, ref PointerCollection, LoggerApplication.LogLevel, LoggerCompliance.LogLevel, out RecordOK);

                                        // If we read the monitor settings record...
                                        if (RecordOK)

                                            // Add the monitor settings record to list of records.
                                            RecordHolders.Add(new RecordHolder
                                            {
                                                RecIndex = RecIndex,
                                                RecType = RecType.RecMonitorSettings,
                                                Record = NewMonitorSettings,
                                                RelatedRecDataSource = LastRecDataSource,
                                                RelatedRecMonitorSettings = null
                                            });

                                        // Remember this as the last monitor settings record found.
                                        LastRecMonitorSettings = NewMonitorSettings;
                                    }

                                    // Otherwise, if the record is an observation record...
                                    else if (tagRecordGUID == tagRecObservation)
                                    {
                                        // Log a message.
                                        LoggerApplication.Log(LogMessagePrefix + ": Observation Record Found: " + (SizeRecord / 1024).ToString() + " kbytes", LogLevels.Info);
                                        RecObservationFound = true;

                                        // Create a new observation record from the PQDIF file
                                        RecObservation NewObservation = new RecObservation(PqdifFile, ref PointerCollection, LoggerApplication.LogLevel, LoggerCompliance.LogLevel, out RecordOK);

                                        // If we read the monitor settings record...
                                        if (RecordOK)

                                            // Add the container record to list of records.
                                            RecordHolders.Add(new RecordHolder
                                            {
                                                RecIndex = RecIndex,
                                                RecType = RecType.RecObservation,
                                                Record = NewObservation,
                                                RelatedRecDataSource = LastRecDataSource,
                                                RelatedRecMonitorSettings = LastRecMonitorSettings
                                            });

                                    } // If the record is a container record

                                    // Otherwise, if the record is a blank record...
                                    else if (tagRecordGUID == tagBlank)
                                    {
                                        // Log a message.
                                        LoggerApplication.Log(LogMessagePrefix + ": Blank Record Found: " + SizeRecord.ToString() + " bytes", LogLevels.Info);
                                    }

                                    // Otherwise, if the record is unknown...
                                    else
                                    {
                                        // Log a compliance message.
                                        LoggerCompliance.Log(LogMessagePrefix + ": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(tagRecordGUID), LogLevels.Info);
                                    }


                                } // If the top-level record is a collection

                                // Otherwise, if the top level record is a not a collection...
                                else
                                {
                                    // Log a compliance error
                                    LoggerCompliance.Log(LogMessagePrefix + ": Top-Level Records Should be a Collection", LogLevels.Error);

                                } // Otherwise, if the top level record is a not a collection


                            } // If we can get the get a pointer to the collection

                        } // If we can get the record info

                    } // For each record

                    // If the container was not found, and there was more than one record in the PQDIF file, then it is non-compliant.
                    if ((!ContainerFound) && (RecCount > 1))
                        LoggerCompliance.Log("Required Container Missing.", LogLevels.Error);

                    // If a data source record was not found, and there was more than one record in the PQDIF file, then it is non-compliant.
                    if ((!RecDataSourceFound) && (RecCount > 1))
                        LoggerCompliance.Log("Required Data Source Record Missing.", LogLevels.Error);

                    // If an observation recrd was not found, and there was more than one record in the PQDIF file, then it is non-compliant.
                    if ((!RecObservationFound) && (RecCount > 1))
                        LoggerCompliance.Log("Required Data Source Record Missing.", LogLevels.Error);


                } // If we could read the PQDIF file

                // Validates the Count of Channel Instances to the Count of Related Channel Definitions.
                ValidateChannelInstanceCount();

                // Add log messages from all of the PQDIF records.
                AddLogsFromRecords();


                // The PQDIF file was read.
                FileRead = true;


            } // try

            // Log application errors.
            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": Error: " + ex.Message, LogLevels.Error);
            }

            // Run this code on exit.
            finally
            {
                // Clean Up.
                try
                {
                    if (PqdifFile !=null)
                    {
                        // Close the PQDIF file and dispose of it.
                        PqdifFile.Close();
                        PqdifFile.Dispose();
                    }
                        
                }
                catch
                {
                    // Ignore errors during clean up.
                }

            } // finally

            // Return the status of reading the file
            return FileOK;

        } // Read        

    } // class File

} // namespace PqdifValidator