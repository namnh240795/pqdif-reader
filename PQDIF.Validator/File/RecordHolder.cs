/*
**  Class:          PQDIF.Validator.RecordHolder
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
    /// <summary>
    /// PQDIF Top-Level Record Types
    /// </summary>
    public enum RecType
    {
        /// <summary>
        /// Unknown Record or Blank Record
        /// </summary>
        Blank = 0,

        /// <summary>
        /// Container Record
        /// </summary>
        Container = 1,

        /// <summary>
        /// Data Source Record
        /// </summary>
        RecDataSource = 2,

        /// <summary>
        /// Monitor Settings Record
        /// </summary>
        RecMonitorSettings = 3,

        /// <summary>
        /// Observation Record
        /// </summary>
        RecObservation = 4

    } // enum RecType


    /// <summary>
    /// Specifies a Class for Stroring PQDIF Records and Related Metadata
    /// </summary>
    public class RecordHolder
    {
        /// <summary>
        /// Index of the Current Record in a PQDIF File
        /// </summary>
        public int RecIndex = -1;

        /// <summary>
        /// Record Type of <see cref="Record"/>
        /// </summary>
        public RecType RecType = RecType.Blank;

        /// <summary>
        /// Current PQDIF Record. See <see cref="Container"/>, <see cref="RecDataSource"/>, <see cref="RecMonitorSettings"/>, and <see cref="RecObservation"/>.
        /// </summary>
        public object Record = null;

        /// <summary>
        /// If <see cref="Record"/> is a <see cref="RecObservation"/> or <see cref="RecMonitorSettings"/> record, then this will be a reference to the record with the <see cref="RecDataSource"/> with <see cref="RecDataSource.ChannelDefns"/> that should be used with this <see cref="RecObservation"/>.
        /// </summary>
        public RecDataSource RelatedRecDataSource = null;

        /// <summary>
        /// If this record is a <see cref="RecObservation"/>, then this will be a reference to the <see cref="RecMonitorSettings"/> with a <see cref="RecMonitorSettings.ChannelSettingsArray"/> that should be used with this <see cref="RecObservation"/>.
        /// </summary>
        public RecMonitorSettings RelatedRecMonitorSettings = null;

    } // RecordHolder

} // namespace PQDIF.Validator