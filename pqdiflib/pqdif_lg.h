/*
** File Description: PQDIF Logical Format Definitions
**
** This file contains the complete specifications for the logical  format of a PQDIF file. It is based
** on the physical structure of the file, which is defined in PQDIF_PH.H.
**
** --------------------------------------------------------------------------
**
** Copyright 2024 PQDIF Authors
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
**
** --------------------------------------------------------------------------
**
**  ======================================================================
**  LOGICAL HIERARCHY OF RECORDS
**  ======================================================================
**  There are four types of records that may be stored in a PQDIF file:
**  Container, Data Source, Monitor Settings, and Observation.
**
**  There are absolute links from one record to another. These are
**  different from links within a record, which are relative within the
**  record. When these links are followed, the records form a logical
**  hierarchy:
**
**    +-----------+
**    | Container |
**    +-----------+
**        |
**        |   +---------------+
**        +---| Data Source 1 |
**        |   +---------------+
**        |       |
**        |       |   +--------------------+
**        |       +---| Monitor Settings 1 |
**        |       |   +--------------------+
**        |       |       |
**        |       |       |   +---------------+
**        |       |       +---| Observation 1 |
**        |       |       |   +---------------+
**        |       |       |   +---------------+
**        |       |       +---| Observation 2 |
**        |       |       |   +---------------+
**        |       |      ...
**        |       |       |   +---------------+
**        |       |       +---| Observation n |
**        |       |           +---------------+
**        |       |
**        |       |   +--------------------+
**        |       +---| Monitor Settings 2 |
**        |       |   +--------------------+
**        |      ...
**        |       |   +--------------------+
**        |       +---| Monitor Settings n |
**        |       |   +--------------------+
**        |      ...
**        |   +---------------+
**        +---| Data Source 2 |
**        |   +---------------+
**       ...
**        |   +---------------+
**        +---| Data Source n |
**        |   +---------------+
**       ...
**
**  The first record in a PQDIF file shall be of the Container type.
**  The data in this record should describe attributes of the items
**  contained within the PQDIF file.
**
**  The container record should be followed by a Data Source
**  record which may be followed by one or more Data Source
**  records or Observation records. A Data Source record should
**  describe the source of the data that is contained in the
**  Observation records that follow it.
**
**  Note that Monitor Settings records are optional; in their absence,
**  Observation records should fall directly under the appropriate
**  Data Source record.
*/
#ifndef PQDIF_LG_H
#define PQDIF_LG_H

/*
**  RECORD HEADER
**  ======================================================================
**  The first item in a PQDIF File (and in each record) is a 128 bit
**  GUID which serves as a unique signature for both the file as a whole
**  and each record.
**
**  Every record shall have this GUID.
*/
const GUID guidRecordSignaturePQDIF = { /* 4a111440-e49f-11cf-9900-505144494600 */
    0x4a111440,
    0xe49f,
    0x11cf,
    {0x99, 0x00, 0x50, 0x51, 0x44, 0x49, 0x46, 0x00}
};


/*
**  TAG FOR ANY RECORD
**  ======================================================================
**  The following tag can be used to leave space in a collection. To leave
**  a collection item blank, specify it as a scalar of less than 8 bytes
**  (such as a UINT4), and specify it as embedded.
*/
const GUID tagBlank =  //  
{ 0x89738618, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };
// {89738618-F1C3-11cf-9D89-0080C72E70A3}


//  ======================================================================
//  Do not modify anything after the following comment:
//  {{{{ AUTO-GENERATED CONSTANTS }}}}

//  Description:   Record-level tag that identifies the container record. It is always the first record in the file and there shall be only one per file.
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagContainer = { 0x89738606, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Record-level tag which identifies a data source (an instrument, etc.)
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagRecDataSource = { 0x89738619, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Record-level tag which identifies a set of configuration parameters
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagRecMonitorSettings = { 0xb48d858c, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Record-level tag which identifies an observation -- an event, measurement, etc.
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagRecObservation = { 0x8973861a, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Specifies the format version for read/write compatibility. The four required numbers in the vector are described below.
//  Element type:  Vector [ 4 ]
//  Physical type: UINT4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagVersionInfo = { 0x89738607, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Original name of the file
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Required
//  Version:       1.0
const GUID tagFileName = { 0x89738608, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Date/time when the file was created
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Required
//  Version:       1.0
const GUID tagCreation = { 0x89738609, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Date/time when the file was last saved
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagLastSaved = { 0x8973860a, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The number of times the file has been saved/modified
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTimesSaved = { 0x8973860b, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The language (English, etc.) of the file
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagLanguage = { 0x8973860c, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary title
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTitle = { 0x8973860d, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary subject string
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSubject = { 0x8973860e, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Individual/company who caused the file to be written
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagAuthor = { 0x8973860f, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Keywords for assisting searches
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagKeywords = { 0x89738610, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary comments
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagComments = { 0x89738611, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Individual/company who last wrote to file
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagLastSavedBy = { 0x89738612, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Creating application
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagApplication = { 0x89738623, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Security descriptor information
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSecurity = { 0x89738613, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Owner of file contents (This and some of the following fields are for copyright and trademark information)
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagOwner = { 0x89738614, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Copyright notice
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCopyright = { 0x89738615, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Trademark notice
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTrademarks = { 0x89738616, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Notes associated with this file
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagNotes = { 0x89738617, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Specifies how the compression is applied to the file
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCompressionStyleID = { 0x8973861b, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Required if tagCompressionStyleID specifies that compression is applied
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCompressionAlgorithmID = { 0x8973861c, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   If compression style is _TOTALFILE, this is the checksum for the entire file. This feature has been deprecated under 1.5.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5 Deprecated
const GUID tagCompressionChecksum = { 0x8973861d, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Contact Name
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagName = { 0xb48d85a2, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Line 1 of a Mailing Address
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagAddress1 = { 0xb48d85a3, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Line 2 of a Mailing Address
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagAddress2 = { 0xb48d85a4, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional City of a Mailing Address
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCity = { 0xb48d85a5, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional State or Province
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagState = { 0xb48d85a6, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Postal Code
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagPostalCode = { 0xb48d85a7, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Country of a Mailing Address
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCountry = { 0xb48d85a8, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Telephone Number
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagPhoneVoice = { 0xb48d85a9, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Fax Number
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagPhoneFAX = { 0x3d786f80, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Optional Email Address
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagEMail = { 0x3d786f81, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Standard ID or custom ID to specify the data source of the source PQ monitor
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Required
//  Version:       1.0
const GUID tagDataSourceTypeID = { 0xb48d8581, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Standard ID or custom ID to specify the vendor which made the PQ monitor or generated the PQDIF file
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagVendorID = { 0xb48d8582, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Standard ID or custom ID to specify the equipment which made the PQ monitor or generated the PQDIF file
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagEquipmentID = { 0xb48d8583, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   This collection can include the standard name, address, and telephone number tags. These apply to the vendor as well as tags about the instrument itself.
//  Element type:  Collection
//  Physical type: (n/a)
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCustomSourceInfo = { 0xb48d8584, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string to hold data source (instrument) serial number
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSerialNumberDS = { 0xb48d8585, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string to hold data source (instrument) version number (if applicable)
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagVersionDS = { 0xb48d8586, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string to hold the name of the data source (instrument)
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Required
//  Version:       1.0
const GUID tagNameDS = { 0xb48d8587, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string to hold data source (instrument) owner name
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagOwnerDS = { 0xb48d8588, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string to hold data source (instrument) location information
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagLocationDS = { 0xb48d8589, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Time Zone (EST, CST, UTC, UTC-05:00, etc.)
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTimeZoneDS = { 0xb48d858a, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Longitude/Latitude
//  Element type:  Vector [ 2 ]
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCoordinatesDS = { 0xb48d858b, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The tagChannelDefns collection shall be a collection where the count equals the number of channel definitions. Each entry shall be another collection, each having the tagOneChannelDefn tag.
//  Element type:  Collection [ # defs ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagChannelDefns = { 0xb48d858d, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Geographic coordinate that specifies the north/south position of a point on the Earth's surface. Latitude should be stored as angular measurement ranging from 0  at the Equator to +90  at the North Pole and to -90  at the South Pole.
//  Element type:  Vector [ * ]
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.6
const GUID tagLatitude = { 0x5217B688, 0x0F0B, 0x44B6, { 0xAA, 0x27, 0xEB, 0x87, 0x41, 0x68, 0xCD, 0xB6 } };

//  Description:   Geographic coordinate that specifies the east/west position of a point on the Earth's surface. Longitude should be stored as angular measurement ranging from 0  at the Prime Meridian to +180  eastward and -180  westward.
//  Element type:  Vector [ * ]
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.6
const GUID tagLongitude = { 0xA9307899, 0x5A94, 0x465C, { 0x8B, 0xEA, 0xCE, 0xA0, 0x7F, 0xE4, 0xEA, 0x65 } };

//  Description:   Specifies the number of seconds to add to tag values and/or series values that store TIMESTAMPPQDIF types in the data source record or in settings records and/or observation records associated with this data source record from Coordinated Universal Time (UTC) to Local Standard Time (LST)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagUTCtoLST = { 0x6ACEC12E, 0x43B1, 0x4336, { 0xAF, 0x8F, 0x1D, 0xD2, 0x89, 0xE4, 0xE1, 0x68 } };

//  Description:   The type of instrument
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagInstrumentTypeID = { 0x3d786f82, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagInstrumentModelName = { 0x3d786f83, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagInstrumentModelNumber = { 0x3d786f84, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Shall have one or more channel definitions
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagOneChannelDefn = { 0xb48d858e, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string to hold a name for the channel
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagChannelName = { 0xb48d8590, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Phase identifier
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagPhaseID = { 0xb48d8591, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagOtherChannelIdentifier = { 0xb48d8593, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   This tag can be repeated if there are multiple groupings. The first one should be the highest-level group (example: a bus), and the next one should be a lower group (example: a feeder).
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagGroupName = { 0xb48d8594, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The high-level description of the type of quantity which is being captured by this channel. The following series value types (in order) should be used (ID_SERIES_VALUE_TYPE_VAL, etc.).
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Required
//  Version:       1.0
const GUID tagQuantityTypeID = { 0xb48d8592, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Identifies the physical quantity under measurement  (For example, Voltage, Current, Power, etc.).  In general, there is a one-to-one correspondence between this and the units of the series, but not always.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Required
//  Version:       1.5
const GUID tagQuantityMeasuredID = { 0xc690e872, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The instrument physical channel with which this channel definition is associated
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagPhysicalChannel = { 0x89738622, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Additional quantity information
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagQuantityName = { 0xb48d8595, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Identifies the series which will be the primary. Index into the tagSeriesDefns collection
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagPrimarySeriesIdx = { 0xb48d8596, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The tagSeriesDefns collection shall be a collection where the count equals the count of series definitions. Each entry shall be another collection, each having the tagOneSeriesDefn tag.
//  Element type:  Collection [ # sers ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagSeriesDefns = { 0xb48d8598, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   One of these collections per series
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagOneSeriesDefn = { 0xb48d859a, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Specifies the meaning of the series data
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Required
//  Version:       1.0
const GUID tagValueTypeID = { 0xb48d859c, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   This specifies the units of the data in this series. The expected physical type for the tagSeriesValues vector is REAL4 or REAL8 (except as noted).
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagQuantityUnitsID = { 0xb48d859b, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Additional detail about the meaning of the series data
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Required
//  Version:       1.5
const GUID tagQuantityCharacteristicID = { 0x3d786f9e, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Defines the number of significant digits in the data represented by this series
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagQuantitySignificantDigitsID = { 0xa112f421, 0xb111, 0x11d2, { 0x9b, 0x37, 0x0, 0x40, 0x5, 0x2c, 0x2d, 0x28 } };

//  Description:   Contains a double indicating the scaled distance between two values of the quantity represented by this series (e.g., scaled A/D resolution)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagQuantityResolutionID = { 0xfb228ee0, 0xfc8d, 0x11d2, { 0xb4, 0x9a, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   The legal values for this entry are masks since they are OR-able.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagStorageMethodID = { 0xb48d85a1, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Arbitrary string
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagValueTypeName = { 0xb48d859d, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Hint about expected Greek prefix
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagHintGreekPrefixID = { 0xb48d859e, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Hint about preferred display units
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagHintPreferredUnitsID = { 0xb48d859f, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Hint about preferred default display
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagHintDefaultDisplayID = { 0xb48d85a0, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   For a probability series definition, this specifies its time interval (in seconds; >0)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagProbInterval = { 0x2747d441, 0x2bd0, 0x11d2, { 0xae, 0x42, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   For a probability series definition, this specifies its probability percentile (in percent; 0-100)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagProbPercentile = { 0x2747d440, 0x2bd0, 0x11d2, { 0xae, 0x42, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Contains the default nominal base voltage or any or any other necessary normalizing quantity.  Display programs may use this value or the tagSeriesBaseQuantity in the series instance for displaying data in percent or per unit.
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagSeriesNominalQuantity = { 0xfa118c8, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   The time that these settings become effective
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Required
//  Version:       1.0
const GUID tagEffective = { 0x62f28183, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The time when the monitor was installed
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Required
//  Version:       1.0
const GUID tagTimeInstalled = { 0x3d786f85, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The time when the monitor was removed
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTimeRemoved = { 0x3d786f86, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   If True, the calibration adjustments should be applied to the series data before using. Otherwise, the data is for informative use only.
//  Element type:  Scalar
//  Physical type: BOOL4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagUseCalibration = { 0x62f28180, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   If True, the transducer adjustments should be applied to the series data before using. Otherwise, the data is for informative use only.
//  Element type:  Scalar
//  Physical type: BOOL4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagUseTransducer = { 0x62f28181, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Channel specific monitor settings stuff
//  Element type:  Collection [ # chan ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagChannelSettingsArray = { 0x62f28182, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Nominal power system frequency for this instrument in Hertz
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagNominalFrequency = { 0xfa118c3, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   Identifies the physical connection of the instrumentation or instrument transducers
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagSettingPhysicalConnection = { 0x9f256ee0, 0x803b, 0x11d3, { 0xb9, 0x2f, 0x0, 0x50, 0xda, 0x2b, 0x1f, 0x4d } };

//  Description:   Specifies the number of seconds to add to tag values and/or series values that store TIMESTAMP types in a Data Source, Monitor Settings, or Observation record from Local Standard Time (LST) to Local Time. If tagLSTtoLocal is used without tagUTCtoLST, then tagUTCtoLST shall be 0.
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagLSTtoLocal = { 0x00B9EC4C, 0x2912, 0x4D9A, { 0xB3, 0xB5, 0xB2, 0xEF, 0xDB, 0x73, 0x6E, 0x28 } };

//  Description:   One of these collections per channel
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagOneChannelSetting = { 0x3d786f9a, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The channel definition which these settings apply to. Index into tagChannelDefns collection of the matching data source record.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagChannelDefnIdx = { 0xb48d858f, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Integer ID representing which trigger fields are used
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTriggerTypeID = { 0x62f28184, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Full scale range for this instrument channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagFullScale = { 0x3d786f87, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Noise floor for this instrument channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagNoiseFloor = { 0x3d786f89, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Parameters for shape based triggering algorithms for this channel
//  Element type:  Vector [ 3 ]
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTriggerShapeParam = { 0x62f28188, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   PT or CT
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagXDTransformerTypeID = { 0x62f28189, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   System side part of ratio
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagXDSystemSideRatio = { 0x62f2818a, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Monitor side part of ratio
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagXDMonitorSideRatio = { 0x62f2818b, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Transducer frequency response
//  Element type:  Vector [ # freq ]
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagXDFrequencyResponse = { 0x62f2818c, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Chanel time skew
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCalTimeSkew = { 0x62f2818d, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Channel DC offset error
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCalOffset = { 0x62f2818e, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Channel ratio error
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCalRatio = { 0x62f2818f, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Flag indicating that the applied/recorded calibration arrays shall be used to correct data
//  Element type:  Scalar
//  Physical type: BOOL4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCalMustUseARCal = { 0x62f28190, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Array of applied signals for this channel
//  Element type:  Vector [ # cal ]
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCalApplied = { 0x62f28191, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Array of recorded actual values for the applied signal
//  Element type:  Vector [ # cal ]
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagCalRecorded = { 0x62f28192, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   High-High trigger for this channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagTriggerHighHigh = { 0x5b12f431, 0xff54, 0x11d3, { 0xb9, 0x68, 0x0, 0x50, 0xda, 0x2b, 0x1f, 0x4d } };

//  Description:   High trigger for this channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTriggerHigh = { 0x62f28186, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Low trigger for this channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTriggerLow = { 0x62f28185, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Low-Low trigger for this channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagTriggerLowLow = { 0x5b12f430, 0xff54, 0x11d3, { 0xb9, 0x68, 0x0, 0x50, 0xda, 0x2b, 0x1f, 0x4d } };

//  Description:   Deadband trigger for this channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagTriggerDeadBand = { 0x5b12f432, 0xff54, 0x11d3, { 0xb9, 0x68, 0x0, 0x50, 0xda, 0x2b, 0x1f, 0x4d } };

//  Description:   Rate of change trigger for this channel
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTriggerRate = { 0x62f28187, 0xf9c4, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Name of the Observation
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Required
//  Version:       1.0
const GUID tagObservationName = { 0x3d786f8a, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Time this observation was created
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Required
//  Version:       1.0
const GUID tagTimeCreate = { 0x3d786f8b, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The start time of the observation
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Required
//  Version:       1.0
const GUID tagTimeStart = { 0x3d786f8c, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Type of trigger which caused the observation
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Required
//  Version:       1.0
const GUID tagTriggerMethodID = { 0x3d786f8d, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Time this observation was triggered if appropriate
//  Element type:  Scalar
//  Physical type: TIMESTAMP
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagTimeTriggered = { 0x3d786f8e, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Index into tagChannelInstances collection within this record. This specifies which channel(s) initiated the observation.
//  Element type:  Vector
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagChannelTriggerIdx = { 0x3d786f8f, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The serial number of the observation (if generated by an instrument, for example)
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagObservationSerial = { 0x3d786f90, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Serial number (for example, a specific cycle), which can be used to correlate observations
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagObservationAggregationSerial = { 0x89738621, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   IEEE 1159 Disturbance Categories
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagDisturbanceCategoryID = { 0xb48d8597, 0xf5f5, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   This collection contains a set of channel instances. It is not required to contain the same number of channels as there are channel instances. This can be determined on an observation-by-observation basis.
//  Element type:  Collection [ # chan ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagChannelInstances = { 0x3d786f91, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Direction of disturbance represented by the data in this observation.  Value of 0 means unknown, 1 means originated from load side, 2 means originated from source side.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagCharactDisturbDirection = { 0xfa118c0, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   Quality of the direction result given in another tag.  Range is from 0 (no confidence), to 100 (full confidence)
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagCharactDisturbDirectionQuality = { 0xfa118c1, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   Minimum RMS Voltage Magnitude (per unit)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagCharactMinVoltagePU = { 0xFF9525C1, 0x62D7, 0x4C9D, { 0xAF, 0x92, 0x4D, 0xEC, 0x21, 0xBE, 0x55, 0x43 } };

//  Description:   Maximum RMS Voltage Magnitude (per unit)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagCharactMaxVoltagePU = { 0xF3EACFF9, 0xDE6E, 0x4542, { 0x82, 0xFB, 0x48, 0x0E, 0xB7, 0xFE, 0x00, 0x12 } };

//  Description:   Minimum RMS Voltage Magnitude (volts)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagCharactMinVoltage = { 0x95C6E3E0, 0x89E0, 0x415F, { 0xB1, 0xC5, 0x32, 0xB3, 0xEA, 0xD2, 0xFB, 0xC8 } };

//  Description:   Maximum RMS Voltage Magnitude (volts)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagCharactMaxVoltage = { 0x5265F355, 0x0CCF, 0x4ACD, { 0x88, 0xCC, 0xC7, 0x9A, 0xB1, 0xF9, 0xDB, 0xFF } };

//  Description:   Minimum RMS Current Magnitude (amps)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagCharactMinCurrent = { 0x5EBFE15F, 0xE6FA, 0x43DD, { 0xA7, 0x2D, 0xF9, 0x77, 0xBC, 0xA8, 0x4F, 0x2F } };

//  Description:   Maximum RMS Current Magnitude (amps)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagCharactMaxCurrent = { 0xE0B1F568, 0xA016, 0x4B58, { 0x81, 0x29, 0x7C, 0x61, 0xFC, 0xB5, 0x1E, 0x18 } };

//  Description:   An ID used to classify a voltage sag by the impact on voltage magnitude and phase angle
//  Element type:  Scalar
//  Physical type: GUID
//  Required/opt:  Optional
//  Version:       1.7
const GUID tagSagTypeID = { 0X714C2C6D, 0X3D6D, 0X41D0, { 0X9B, 0X8C, 0X6E, 0XAE, 0XAF, 0X1F, 0X82, 0X77 } };

//  Description:   One of these collections per channel instance
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagOneChannelInst = { 0x3d786f92, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Simple characterization value: duration of disturbance (seconds)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagCharactDuration = { 0x2747d444, 0x2bd0, 0x11d2, { 0xae, 0x42, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   This collection shall contain the exact number of series which were defined for the specified channel definition
//  Element type:  Collection [ # sers ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagSeriesInstances = { 0x3d786f93, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Simple characterization value: magnitude of disturbance (percent: 100%=nominal)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagCharactMagnitude = { 0x2747d443, 0x2bd0, 0x11d2, { 0xae, 0x42, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Simple characterization value: frequency (Hertz)
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagCharactFrequency = { 0x2747d445, 0x2bd0, 0x11d2, { 0xae, 0x42, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Contains a 32-bit integer that represents module specific information related to the trigger reason
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagChanTriggerModuleInfo = { 0xfa118c7, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   Contains the name of a device specific code or hardware module, algorithm, or rule not necessarily channel based that caused this channel to be recorded
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagChanTriggerModuleName = { 0xfa118c6, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   Contains the name of the device involved in an external cross-trigger scenario
//  Element type:  Vector [ * ]
//  Physical type: CHAR1
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagCrossTriggerDeviceName = { 0xfa118c5, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   Contains the channel definition index of the channel that triggered in a cross-trigger scenario
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagCrossTriggerChanIdx = { 0xfa118c4, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   Integer ID representing the trigger type for this channel instance.  Used only with type ID_QT_VALUELOG with a trigger method of channel.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagChanTriggerTypeID = { 0xfa118c2, 0xcb4a, 0x11d2, { 0xb3, 0xb, 0xfe, 0x25, 0xcb, 0x9a, 0x17, 0x60 } };

//  Description:   For a channel which contains multiple instances to represent a sparse log of time-stamped frequency-domain information, this specifies the frequency for which this channel instance applies (in Hertz).  If not present, the channel characteristics are frequency independent unless further specified by the quantity characteristic.
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagChannelFrequency = { 0x2747d442, 0x2bd0, 0x11d2, { 0xae, 0x42, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   For a channel which contains multiple instances to represent a sparse log of time-stamped frequency-domain information, this specifies the frequency for which this channel instance applies (in Index).  The index refers to a harmonic or interharmonic group index.
//  Element type:  Scalar
//  Physical type: INT2
//  Required/opt:  Optional
//  Version:       1.5
const GUID tagChannelGroupID = { 0xf90de218, 0xe67b, 0x4cf1, { 0xa2, 0x95, 0xb0, 0x21, 0xa2, 0xd4, 0x67, 0x67 } };

//  Description:   One of these collections per series instance
//  Element type:  Collection [ * ]
//  Physical type: (n/a)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagOneSeriesInstance = { 0x3d786f94, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Contains the nominal base voltage, or any other necessary normalizing quantity
//  Element type:  Scalar
//  Physical type: REAL8
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSeriesBaseQuantity = { 0x3d786f95, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   If not present, assumed to be 1. The physical type may not match that of tagSeriesValues. tagSeriesScale, along with tagSeriesOffset, provides a method to translate the sample values stored in tagSeriesValues to real values as follows: real value = (tagSeriesValues * tagSeriesScale) + tagSeriesOffset.
//  Element type:  Scalar
//  Physical type: (any type)
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSeriesScale = { 0x3d786f96, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   If not present, assumed to be 0. Should be used as a starting point when the ID_SERIES_METHOD_INCREMENT storage method is used. The physical type may not match that of tagSeriesValues. tagSeriesOffset, along with tagSeriesScale, provides a method to translate the samples stored in the series to real values as follows: real value = (tagSeriesValues * tagSeriesScale )+ tagSeriesOffset.
//  Element type:  Scalar
//  Physical type: (any type)
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSeriesOffset = { 0x3d786f97, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Identifies the channel which owns the series to be shared. An index into the tagChannelInstances collection.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSeriesShareChannelIdx = { 0x8973861f, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Identifies the series to be shared. An index into the tagSeriesInstances collection. The tagSeriesValues vector from this series is used. This shall be present if tagSeriesShareChannelIdx is used.
//  Element type:  Scalar
//  Physical type: UINT4
//  Required/opt:  Optional
//  Version:       1.0
const GUID tagSeriesShareSeriesIdx = { 0x89738620, 0xf1c3, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Contains the actual data points of the series. Required unless the data series is shared, in which case both tagSeriesShareChannelIdx and tagSeriesShareSeriesIdx should be present.
//  Element type:  Vector [ * ]
//  Physical type: (any type)
//  Required/opt:  Required
//  Version:       1.0
const GUID tagSeriesValues = { 0x3d786f99, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };



#endif  //  PQDIF_LG_H


