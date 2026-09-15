/*
**  File Description: PQDIF Standard ID Definitions
**
**  This file contains the current list of standard IDs for the various tags in the PQDIF standard.
**  The IDs consist of two types: GUIDs and integers (UINT4). The IDs that are defined with
**  the integer type use a #define directive. The IDs that are defined with the GUID type
**  use a const keyword declaration with a GUID structure.
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

#ifndef PQDIF_ID_H
#define PQDIF_ID_H

//  ======================================================================
//  Do not modify anything after the following comment:
//  {{{{ AUTO-GENERATED CONSTANTS }}}}

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagCompressionStyleID
//  ==========================================================

//  Description:   No compression is used.
//  Version:       1.0
const int ID_COMP_STYLE_NONE = 0;

//  Description:   The body of each record is compressed individually. The checksums will be found in the header of each record.
//  Version:       1.0
const int ID_COMP_STYLE_RECORDLEVEL = 2;

//  Description:   Everything after the container record is compressed as a single block. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const int ID_COMP_STYLE_TOTALFILE = 1;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagCompressionAlgorithmID
//  ==========================================================

//  Description:   No compression algorithm is used.
//  Version:       1.0
const int ID_COMP_ALG_NONE = 0;

//  Description:   A standard compression algorithm -- ZLIB -- standardized by the IETF (Internet Engineering Task Force). See http://quest.jpl.nasa.gov/zlib/ for details.
//  Version:       1.0
const int ID_COMP_ALG_ZLIB = 1;

//  Description:   A commercial package, the PKZIP data compression library, was used to compress the data. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const int ID_COMP_ALG_PKZIPCL = 64;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagDataSourceTypeID
//  ==========================================================

//  Description:   Actual Measurements
//  Version:       1.0
const GUID ID_DS_TYPE_MEASURE = { 0xe6b51730, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Manual Measurements
//  Version:       1.0
const GUID ID_DS_TYPE_MANUAL = { 0xe6b51731, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Simulated Measurements
//  Version:       1.0
const GUID ID_DS_TYPE_SIMULATE = { 0xe6b51732, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Benchmark Measurements
//  Version:       1.0
const GUID ID_DS_TYPE_BENCHMARK = { 0xe6b51733, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Debug Measurements
//  Version:       1.0
const GUID ID_DS_TYPE_DEBUG = { 0xe6b51734, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagVendorID
//  ==========================================================

//  Description:   Default Vendor
const GUID ID_VENDOR_NONE = { 0xe6b51701, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   ASEA Brown Boveri
//  Version:       1.6
const GUID ID_VENDOR_ABB = { 0xE5815120, 0xF105, 0x4683, { 0xA4, 0x14, 0x33, 0xD6, 0x6B, 0x33, 0xCE, 0xF8 } };

//  Description:   A. Eberle
//  Version:       1.6
const GUID ID_VENDOR_A_EBERLE = { 0xfa2847f5, 0x27dc, 0x4658, { 0xb1, 0x2e, 0x80, 0xb4, 0xc0, 0xdf, 0xef, 0x3d } };

//  Description:   ACTL
//  Version:       1.5
const GUID ID_VENDOR_ACTL = { 0x80c4a761, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Advantech
//  Version:       1.5
const GUID ID_VENDOR_ADVANTECH = { 0x650f988f, 0x378c, 0x47b8, { 0xba, 0xed, 0xcc, 0xcb, 0x3f, 0x95, 0x9a, 0xd7 } };

//  Description:   Alpes Technologies
//  Version:       1.6
const GUID ID_VENDOR_ALPESTECHNOLOGIES = { 0x067f350b, 0x5c9e, 0x4537, { 0xa0, 0x51, 0xb9, 0x52, 0xce, 0x43, 0xd7, 0x6f } };

//  Description:   Ametek Power Instruments
//  Version:       1.6
const GUID ID_VENDOR_AMETEK = { 0x8d57862e, 0x4a7a, 0x4b1e, { 0xb7, 0xe8, 0xf7, 0xc9, 0x00, 0x11, 0xef, 0x3f } };

//  Description:   Arbiter Systems
//  Version:       1.6
const GUID ID_VENDOR_ARBITER = { 0x81bbfe63, 0xd3cd, 0x40fa, { 0xb2, 0x4b, 0x33, 0x2b, 0xb2, 0xa2, 0x5b, 0xd4 } };

//  Description:   Dranetz Technologies (Basic Measuring Instruments)
const GUID ID_VENDOR_BMI = { 0xe6b51702, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Bonneville Power Authority
const GUID ID_VENDOR_BPA = { 0xe6b51703, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Camille Bauer Metrawatt
//  Version:       1.6
const GUID ID_VENDOR_CAMILLEBAUER = { 0x87a75537, 0xdf9f, 0x45fe, { 0x80, 0x96, 0x79, 0x78, 0x0a, 0x0c, 0x9c, 0xe1 } };

//  Description:   Centro Elettrotecnico Sperimentale Italiano
const GUID ID_VENDOR_CESI = { 0xe6b51704, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   CESINEL
//  Version:       1.6
const GUID ID_VENDOR_CESINEL = { 0xb12b64c6, 0x1e0e, 0x4440, { 0xb1, 0x1, 0xd5, 0xf5, 0xf, 0xb9, 0x17, 0xcd } };

//  Description:   Eaton (Cooper Power Systems)
const GUID ID_VENDOR_COOPER = { 0xe6b51705, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   DCG
const GUID ID_VENDOR_DCG = { 0xe6b51706, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Dranetz Technologies
const GUID ID_VENDOR_DRANETZ = { 0xe6b51707, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Eaton
//  Version:       1.6
const GUID ID_VENDOR_EATON = { 0x9732DC7F, 0x449B, 0x4928, { 0xAE, 0x82, 0x41, 0xD0, 0x3B, 0x5B, 0x4A, 0x8D } };

//  Description:   Ecamec Tecnologia
//  Version:       1.6
const GUID ID_VENDOR_ECAMEC = { 0xF52E7577, 0xF281, 0x4D18, { 0xB4, 0x0C, 0xE5, 0x16, 0xBA, 0xA2, 0x33, 0x62 } };

//  Description:   Électricité de France
const GUID ID_VENDOR_EDF = { 0xe6b51708, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   EDMI
//  Version:       1.6
const GUID ID_VENDOR_EDMI = { 0x5AFAD18D, 0xE143, 0x4032, { 0x8A, 0x50, 0x33, 0x32, 0x41, 0x75, 0x4A, 0x84 } };

//  Description:   Elcom
//  Version:       1.5
const GUID ID_VENDOR_ELCOM = { 0xf7e9eb70, 0x6f1d, 0x11d6, {0x9c, 0xb3, 0x00, 0x20, 0xe0, 0x10, 0x45, 0x3b } };

//  Description:   Electro Industries
//  Version:       1.6
const GUID ID_VENDOR_ELECTRO_INDUSTRIES = { 0x3f39084a, 0xa2be, 0x4aa0, { 0x9b, 0x06, 0x08, 0xcd, 0x27, 0x3d, 0x2b, 0x55 } };

//  Description:   Electrotek Concepts
const GUID ID_VENDOR_ELECTROTEK = { 0xe6b5170a, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Elspec
//  Version:       1.6
const GUID ID_VENDOR_ELSPEC = { 0x3d01d792, 0x3944, 0x45d0, { 0xac, 0x02, 0x12, 0x28, 0x6c, 0x31, 0x3b, 0x5a } };

//  Description:   E-Max Instruments
//  Version:       1.6
const GUID ID_VENDOR_EMAX = { 0x24b36513, 0x90b8, 0x4340, { 0x92, 0x8a, 0x10, 0xf4, 0x90, 0x29, 0x01, 0xb9 } };

//  Description:   EnerNex Corporation
//  Version:       1.6
const GUID ID_VENDOR_ENERNEX = { 0x549f6a7f, 0x5334, 0x4c90, { 0x9f, 0x77, 0xa6, 0xce, 0xa7, 0xf3, 0x04, 0x65 } };

//  Description:   European Network of Transmission System Operators for Electricity 
//  Version:       1.6
const GUID ID_VENDOR_ENTSO_E = { 0x601954B3, 0xB517, 0x4641, { 0x91, 0x54, 0x52, 0xE8, 0xD6, 0x8E, 0xC0, 0x5D } };

//  Description:   Electric Power Research Institute
const GUID ID_VENDOR_EPRI = { 0xe6b51709, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Fluke Instruments
const GUID ID_VENDOR_FLUKE = { 0xe6b5170b, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   General Electric
//  Version:       1.5
const GUID ID_VENDOR_GE = { 0x5202bd00, 0x245c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Gossen Metrawatt
//  Version:       1.6
const GUID ID_VENDOR_GMC = { 0xCD0FD6E6, 0xBCB9, 0x47AC, { 0xBF, 0x65, 0x9F, 0x2E, 0x26, 0x1C, 0xA1, 0x05 } };

//  Description:   HIOKI Corporation 
//  Version:       1.6
const GUID ID_VENDOR_HIOKI = { 0x816c20da, 0xed92, 0x4602, { 0x9d, 0x4a, 0xcd, 0x71, 0x86, 0x64, 0x43, 0x51 } };

//  Description:   Hydro-Québec
const GUID ID_VENDOR_HYDROQUEBEC = { 0xe6b5170c, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Institute of Electrical and Electronics Engineers
const GUID ID_VENDOR_IEEE = { 0xe6b5170d, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Kreiss-Johnson Technologies
const GUID ID_VENDOR_KREISSJOHNSON = { 0xe6b5170e, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Landis+Gyr
//  Version:       1.6
const GUID ID_VENDOR_LANDIS_GYR = { 0x129c0835, 0x6870, 0x4f2c, { 0x9a, 0x66, 0x07, 0x91, 0xd1, 0x31, 0xa1, 0x84 } };

//  Description:   Qualitrol (LEM Instruments)
//  Version:       1.5
const GUID ID_VENDOR_LEM = { 0x80c4a722, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Megger
//  Version:       1.6
const GUID ID_VENDOR_MEGGER = { 0x86B2A599, 0x1CB3, 0x4707, { 0xBF, 0xE3, 0xC8, 0x6A, 0xF4, 0xE3, 0x1E, 0xF7 } };

//  Description:   MetOne
//  Version:       1.5
const GUID ID_VENDOR_METONE = { 0xb5b5da61, 0xe2e1, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   Metrosonic
const GUID ID_VENDOR_METROSONIC = { 0xe6b5170f, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Metrum
//  Version:       1.6
const GUID ID_VENDOR_METRUM = { 0xc527d1a4, 0xa227, 0x4af4, { 0x99, 0x61, 0x63, 0x9b, 0x5e, 0x0e, 0x67, 0xe9 } };

//  Description:   Nexant Inc
//  Version:       1.6
const GUID ID_VENDOR_NEXANT = { 0x84c6047f, 0xf929, 0x4f43, { 0xb6, 0x4e, 0x40, 0xd4, 0x06, 0xbf, 0x0d, 0xb9 } };

//  Description:   Outram Research
//  Version:       1.6
const GUID ID_VENDOR_ORL = { 0xf6cae1ef, 0xc947, 0x4f11,{  0xa3, 0x89, 0xf7, 0x5c, 0x87, 0x09, 0x05, 0xbd } };

//  Description:   Power Monitors
//  Version:       1.5
const GUID ID_VENDOR_PMI = { 0x609acec0, 0x993d, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Schneider Electric (Power Measurement)
const GUID ID_VENDOR_PML = { 0xe6b51710, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   PSI Solutions
const GUID ID_VENDOR_PSI = { 0xe6b51711, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Powerside (Power Standards Labs)
//  Version:       1.6
const GUID ID_VENDOR_PSL = { 0x4e354a9b, 0x2ad0, 0x463f, { 0x9c, 0x08, 0x52, 0xa8, 0xff, 0x9d, 0x51, 0x49 } };

//  Description:   Power Technologies
const GUID ID_VENDOR_PTI = { 0xe6b51712, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Open Source or Public Domain
const GUID ID_VENDOR_PUBLICDOMAIN = { 0xe6b51713, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Qualitrol
//  Version:       1.6
const GUID ID_VENDOR_QUALITROL = { 0x92B87689, 0xF1AD, 0x4088, { 0xB8, 0x5B, 0x15, 0xDA, 0x81, 0x96, 0x4B, 0x6F } };

//  Description:   Fluke (Reliable Power Meters)
const GUID ID_VENDOR_RPM = { 0xe6b51714, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   SATEC
const GUID ID_VENDOR_SATEC = { 0xe2da5081, 0x7fdb, 0x11d3, { 0x9b, 0x39, 0x0, 0x40, 0x5, 0x2c, 0x2d, 0x28 } };

//  Description:   Schneider Electric
//  Version:       1.6
const GUID ID_VENDOR_SCHNEIDER_ELECTRIC = { 0x2A8B3149, 0xEF5C, 0x480E, { 0xA9, 0x13, 0xBF, 0x30, 0x25, 0x57, 0x2A, 0xAA } };

//  Description:   Schweitzer Engineering Labs
//  Version:       1.6
const GUID ID_VENDOR_SEL = { 0x902096B3, 0x57E8, 0x4D57, { 0x90, 0xBA, 0xED, 0x4B, 0x63, 0xBD, 0x8A, 0x6D } };

//  Description:   Siemens
//  Version:       1.6
const GUID ID_VENDOR_SIEMENS = { 0x19E5093C, 0x0A4B, 0x4BE4, { 0xBF, 0xE7, 0xF1, 0xFC, 0x98, 0xBE, 0x13, 0xBB } };

//  Description:   Socomec
//  Version:       1.6
const GUID ID_VENDOR_SOCOMEC = { 0xBABAF201, 0x02EA, 0x4E11, { 0x80, 0xA9, 0x45, 0x55, 0xEB, 0xF8, 0x41, 0x09 } };

//  Description:   Schneider Electric (Square D)
const GUID ID_VENDOR_SQUAREDPOWERLOGIC = { 0xe6b51715, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Rockwell Automation (SoftSwitching Technologies)
//  Version:       1.6
const GUID ID_VENDOR_SST = { 0x987a7255, 0xcb3a, 0x418a, { 0x84, 0xf8, 0x58, 0x84, 0x93, 0xc5, 0x5f, 0x23 } };

//  Description:   Telog Instruments
const GUID ID_VENDOR_TELOG = { 0xe6b51716, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Trinergi (Beving Electronics)
//  Version:       1.5
const GUID ID_VENDOR_TRINERGI = { 0xfd5a3a8, 0xd73a, 0x11d2, { 0xac, 0x3e, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Unipower
//  Version:       1.6
const GUID ID_VENDOR_UNIPOWER = { 0xbea1af55, 0x4786, 0x73de, { 0xd0, 0x41, 0x73, 0xbf, 0xc3, 0xf5, 0xa0, 0x3a } };

//  Description:   Global Power Technology (World Power Technologies)
const GUID ID_VENDOR_WPT = { 0xe2da5082, 0x7fdb, 0x11d3, { 0x9b, 0x39, 0x0, 0x40, 0x5, 0x2c, 0x2d, 0x28 } };

//  Description:   Algodue Elettronica
//  Version:       1.7
const GUID ID_VENDOR_ALGODUE = { 0x53DB509D, 0x00A5, 0x453B, { 0xAD, 0x17, 0xE0, 0x8E, 0x67, 0x61, 0xE5, 0x6D } };

//  Description:   KMB Systems
//  Version:       1.7
const GUID ID_VENDOR_KMB = { 0x67993EB8, 0x99FA, 0x4733, { 0x9E, 0x0E, 0x86, 0xA8, 0xCE, 0x8A, 0x8B, 0x3E } };

//  Description:   MIKRONIKA
//  Version:       1.6
const GUID ID_VENDOR_MIKRONIKA = { 0x44DB8965, 0xF196, 0x4414, { 0xA3, 0x71, 0x15, 0x83, 0x99, 0xC1, 0x00, 0x69 } };

//  Description:   BMR
//  Version:       1.6
const GUID ID_VENDOR_BMR = { 0x42e6bd66, 0xdcf7, 0x4f8b, { 0xbc, 0xe8, 0xa5, 0xf9, 0x47, 0x8e, 0x48, 0x7c } };

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagEquipmentID
//  ==========================================================

//  Description:   Default Equipment
//  Version:       1.6
const GUID ID_EQUIP_NONE = { 0xA169B672, 0x6A75, 0x4189, { 0xA7, 0x8E, 0x74, 0x9F, 0xF9, 0x85, 0x4E, 0x0F } };

//  Description:   Acumentrics Control Sensor
//  Version:       1.5
const GUID ID_EQUIP_ACUMENTRICS_CONTROL = { 0x5202bd04, 0x245c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Advantech ADAM 4017
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM4017 = { 0x92b7977b, 0xc02, 0x4766, { 0x95, 0xcf, 0xdd, 0x37, 0x9c, 0xae, 0xb4, 0x17 } };

//  Description:   Advantech ADAM 4018
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM4018 = { 0x3008151e, 0x2317, 0x4405, { 0xa5, 0x9e, 0xe7, 0xb3, 0xb2, 0x6, 0x67, 0xa9 } };

//  Description:   Advantech ADAM 4018M
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM4018M = { 0x3a1af807, 0x1347, 0x45f8, { 0x96, 0x6a, 0xf4, 0x81, 0xc6, 0xae, 0x20, 0x8e } };

//  Description:   Advantech ADAM 4050
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM4050 = { 0x9212066d, 0xea65, 0x477e, { 0xbf, 0x95, 0xe4, 0xa0, 0x6, 0x6d, 0x25, 0xce } };

//  Description:   Advantech ADAM 4052
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM4052 = { 0x8bba416b, 0xa7ec, 0x4616, { 0x8b, 0x8f, 0x59, 0xfe, 0xd7, 0x49, 0x32, 0x3d } };

//  Description:   Advantech ADAM 4053
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM4053 = { 0xdc29b83f, 0xbebe, 0x4cf3, { 0xb3, 0xfb, 0x0, 0xdc, 0x63, 0x62, 0x6d, 0xd9 } };

//  Description:   Advantech ADAM 4080
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM4080 = { 0x64fc42c6, 0x3c90, 0x4633, { 0x99, 0xdf, 0x2c, 0x60, 0x58, 0x21, 0x4b, 0x72 } };

//  Description:   Advantech ADAM 5017
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM5017 = { 0x2f46263c, 0x92ac, 0x4717, { 0x8a, 0x8, 0xa6, 0x17, 0x7d, 0xf3, 0xf6, 0x11 } };

//  Description:   Advantech ADAM 5018
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM5018 = { 0xcc2d3247, 0xfe65, 0x4db6, { 0x82, 0x6, 0x50, 0xa, 0x23, 0x15, 0x1b, 0xb2 } };

//  Description:   Advantech ADAM 5050
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM5050 = { 0xc950a2e3, 0x7a35, 0x440c, { 0x86, 0x60, 0x63, 0xf6, 0x11, 0x97, 0x25, 0x19 } };

//  Description:   Advantech ADAM 5051
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM5051 = { 0xc8f92334, 0xa69b, 0x4856, { 0xb2, 0x53, 0xec, 0x24, 0x71, 0xd1, 0x37, 0xd6 } };

//  Description:   Advantech ADAM 5052
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM5052 = { 0xe9261dfe, 0x3d44, 0x47e3, { 0xac, 0x36, 0x3b, 0x9, 0x7f, 0xaa, 0x8c, 0xda } };

//  Description:   Advantech ADAM 5080
//  Version:       1.5
const GUID ID_EQUIP_ADVANTECH_ADAM5080 = { 0x6c37b63c, 0xe770, 0x4b85, { 0xbd, 0x32, 0x47, 0x39, 0xd6, 0xeb, 0x98, 0x46 } };

//  Description:   Arbiter Systems 1133A Power Sentinel
//  Version:       1.6
const GUID ID_EQUIP_ARBITER_1133A = { 0x03b4d505, 0x6e6e, 0x43ff, { 0x8f, 0xa5, 0x7e, 0x40, 0xf9, 0x25, 0xec, 0xc0 } };

//  Description:   Dranetz PQNode/PQPager 3100
const GUID ID_EQUIP_BMI_3100 = { 0xf1c04780, 0x50fb, 0x11d3, { 0xac, 0x3e, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Dranetz PQNode 7100
const GUID ID_EQUIP_BMI_7100 = { 0xe6b51717, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Dranetz PQNode 8010
const GUID ID_EQUIP_BMI_8010 = { 0xe6b51718, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Dranetz PQNode 8020
const GUID ID_EQUIP_BMI_8020 = { 0xe6b51719, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Basic Measuring Instruments 8800
//  Version:       1.5
const GUID ID_EQUIP_BMI_8800 = { 0xe77d1a81, 0x1235, 0x11d5, { 0xa3, 0x90, 0x0, 0x10, 0xa4, 0x92, 0x4e, 0xcc } };

//  Description:   Dranetz PQNode 9010
const GUID ID_EQUIP_BMI_9010 = { 0xe6b5171a, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   CANDURA Instruments EnergyPro
//  Version:       1.6
const GUID ID_EQUIP_CANDURA_EP600 = { 0xE2A5A20E, 0x1E77, 0x4FB8, { 0x9F, 0xFD, 0xEF, 0x4F, 0xC0, 0x0C, 0x29, 0xEA } };

//  Description:   CANDURA Instruments PQPMU
//  Version:       1.6
const GUID ID_EQUIP_CANDURA_PQPMU = { 0x0D9D22A5, 0x2C70, 0x4B16, { 0xA8, 0x10, 0x3E, 0x5E, 0x65, 0xA2, 0x8A, 0xF4 } };

//  Description:   CANDURA Instruments PQPro
//  Version:       1.6
const GUID ID_EQUIP_CANDURA_PQPRO = { 0xB8479B8D, 0xC681, 0x4A5A, { 0xAF, 0xEC, 0xB5, 0xAA, 0x0C, 0x6B, 0xDF, 0x0E } };

//  Description:   CANDURA Instruments PVII
//  Version:       1.6
const GUID ID_EQUIP_CANDURA_PVII = { 0x743B81EE, 0x29C1, 0x44D4, { 0xAD, 0x44, 0x62, 0xE7, 0xC4, 0xC2, 0x3B, 0xE0 } };

//  Description:   Camille Bauer LINAX PQ3000
//  Version:       1.6
const GUID ID_EQUIP_CB_PQ3000 = { 0xd9bab7b7, 0x7a02, 0x4442, { 0x95, 0x25, 0x8b, 0x30, 0x32, 0x2c, 0xf1, 0x27 } };

//  Description:   Camille Bauer LINAX PQ5000
//  Version:       1.6
const GUID ID_EQUIP_CB_PQ5000 = { 0x3e828400, 0x82d0, 0x4b55, { 0xba, 0x9f, 0x57, 0x57, 0x8f, 0x9a, 0x05, 0x62 } };

//  Description:   Eaton V-Flicker Software
const GUID ID_EQUIP_COOPER_VFLICKER = { 0xe6b5171c, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Eaton V-Harm Software
const GUID ID_EQUIP_COOPER_VHARM = { 0xe6b5171b, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   DCG Electromagnetic Transient Program
const GUID ID_EQUIP_DCG_EMTP = { 0xe6b5171d, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Dranetz Disturbance Waveform Analyzer 656
const GUID ID_EQUIP_DRANETZ_656 = { 0xe6b5171e, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Dranetz Disturbance Waveform Analyzer 658
const GUID ID_EQUIP_DRANETZ_658 = { 0xe6b5171f, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Dranetz Energy Analyzer 8000
const GUID ID_EQUIP_DRANETZ_8000 = { 0xe6b51720, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   ELCOM BK550
//  Version:       1.5
const GUID ID_EQUIP_ELCOM_BK550 = { 0xf4380a60, 0x6f1d, 0x11d6, {0x9c, 0xb3, 0x00, 0x20, 0xe0, 0x10, 0x45, 0x3b } };

//  Description:   Elspec Export from PQSCADA software
//  Version:       1.6
const GUID ID_EQUIP_ELSPEC_PQSCADA = { 0x17a53bb1, 0xd98b, 0x48b2, { 0x95, 0x74, 0x6f, 0xb3, 0xfb, 0xbe, 0x81, 0xe2 } };

//  Description:   Exported from E-Max Director Software
//  Version:       1.6
const GUID ID_EQUIP_EMAX_DIRECTOR = { 0xe6b611a9, 0x166c, 0x45ba, { 0x80, 0x31, 0x04, 0x2e, 0xe5, 0xc8, 0x99, 0x4e } };

//  Description:   EPRI Open Distribution System Simulator (OpenDSS)
//  Version:       1.5
const GUID ID_EQUIP_ETK_DSS = { 0xd347ba66, 0xe34c, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   Electrotek Harmoni Software
const GUID ID_EQUIP_ETK_HARMONI = { 0xe6b51728, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Electrotek NodeCenter Software
//  Version:       1.5
const GUID ID_EQUIP_ETK_NODECENTER = { 0xc52e8460, 0x58b4, 0x4f1a, { 0x84, 0x69, 0x69, 0xca, 0x3f, 0xef, 0x9f, 0xf1 } };

//  Description:   Electrotek PASS/PNDS Software
const GUID ID_EQUIP_ETK_PASS = { 0xe6b51723, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Electrotek PQDIF Editor Software
const GUID ID_EQUIP_ETK_PQDIFEDITOR = { 0xe6b51722, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Electrotek PQDiffractor Software
//  Version:       1.6
const GUID ID_EQUIP_ETK_PQDIFFRACTOR = { 0xd95cf777, 0x8405, 0x4f65, { 0xa5, 0xa8, 0x04, 0x16, 0x83, 0xf3, 0x3d, 0x10 } };

//  Description:   Electrotek PQView Software
const GUID ID_EQUIP_ETK_PQVIEW = { 0xe6b51727, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Electrotek PQWeb Software
//  Version:       1.5
const GUID ID_EQUIP_ETK_PQWEB = { 0x5202bd06, 0x245c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Electrotek PQWeb SuperHarm Software
const GUID ID_EQUIP_ETK_SUPERHARM = { 0xe6b51724, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Electrotek PQWeb SuperTran Software
const GUID ID_EQUIP_ETK_SUPERTRAN = { 0xe6b51725, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Electrotek Test Program Software
const GUID ID_EQUIP_ETK_TESTPROGRAM = { 0xe6b51721, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Electrotek Text-to-PQDIF Software
//  Version:       1.5
const GUID ID_EQUIP_ETK_TEXTPQDIF = { 0x5202bd05, 0x245c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Electrotek TOP Software
const GUID ID_EQUIP_ETK_TOP = { 0xe6b51726, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Qualitrol Ewon 4000
//  Version:       1.5
const GUID ID_EQUIP_EWON_4000 = { 0x80c4a762, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Fluke Cur Device
const GUID ID_EQUIP_FLUKE_CUR = { 0xe6b51729, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Fluke F41
const GUID ID_EQUIP_FLUKE_F41 = { 0xe6b5172a, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   GE kV Energy Meter
//  Version:       1.5
const GUID ID_EQUIP_GE_KV = { 0x5202bd01, 0x245c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   GE kV2 Energy Meter
//  Version:       1.5
const GUID ID_EQUIP_GE_KV2 = { 0x5202bd03, 0x245c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Dranetz DataNode 61000
//  Version:       1.6
const GUID ID_EQUIP_GPT_61000 = { 0x7e8607f0, 0xbfbc, 0x44d7, { 0x8e, 0x0b, 0x44, 0xda, 0xdb, 0x68, 0x28, 0x71 } };

//  Description:   Dranetz DataNode Energy Series 210
//  Version:       1.6
const GUID ID_EQUIP_GPT_ES210 = { 0x0cd7fbdb, 0xaae6, 0x4cf5, { 0xb1, 0x7a, 0x5a, 0xfd, 0xc1, 0x1a, 0x49, 0x20 } };

//  Description:   Dranetz DataNode Energy Series 230
//  Version:       1.6
const GUID ID_EQUIP_GPT_ES230 = { 0xd8774821, 0xad15, 0x41a7, { 0xb1, 0xfa, 0x5e, 0x02, 0x57, 0xfb, 0xcb, 0x8a } };

//  Description:   Converted from IEEE COMTRADE File
const GUID ID_EQUIP_IEEE_COMTRADE = { 0xe6b5172b, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Export from Landis+Gyr Maxcom Software
//  Version:       1.6
const GUID ID_EQUIP_LANDIS_GYR_MAXCOM = { 0x713ccdf8, 0x0fd7, 0x4411, { 0xb9, 0x9f, 0x4c, 0x19, 0xe0, 0x5f, 0xae, 0x29 } };

//  Description:   LEM Instruments Analyst 1Q
//  Version:       1.5
const GUID ID_EQUIP_LEM_ANALYST1Q = { 0x477ecb3b, 0x917f, 0x4915, { 0xaf, 0x99, 0xa6, 0xc2, 0x9a, 0xc1, 0x87, 0x64 } };

//  Description:   LEM Instruments Analyst 2050
//  Version:       1.5
const GUID ID_EQUIP_LEM_ANALYST2050 = { 0x9878ccab, 0xa842, 0x4cac, { 0x95, 0xf, 0x6d, 0x47, 0x21, 0x5b, 0xff, 0xdf } };

//  Description:   LEM Instruments Analyst 2060
//  Version:       1.5
const GUID ID_EQUIP_LEM_ANALYST2060 = { 0x312471a2, 0xb586, 0x491c, { 0x85, 0x5a, 0xca, 0x5, 0x45, 0x9a, 0x7e, 0x20 } };

//  Description:   LEM Instruments Analyst 3Q
//  Version:       1.5
const GUID ID_EQUIP_LEM_ANALYST3Q = { 0xd567cb71, 0xbcc0, 0x41ee, { 0x8e, 0x8c, 0x35, 0x85, 0x15, 0x53, 0xf6, 0x55 } };

//  Description:   Qualitrol BEN 5000 Digital Fault Recorder
//  Version:       1.5
const GUID ID_EQUIP_LEM_BEN5000 = { 0xa70e32b1, 0x2f1a, 0x4543, { 0xa6, 0x84, 0x78, 0xa4, 0xb5, 0xbe, 0x34, 0xbb } };

//  Description:   Qualitrol BEN 6000 Digital Fault Recorder
//  Version:       1.5
const GUID ID_EQUIP_LEM_BEN6000 = { 0x5a4c1b5, 0x6681, 0x47e6, { 0x9f, 0x64, 0x8d, 0xa1, 0x25, 0xdb, 0xec, 0x32 } };

//  Description:   Qualitrol Ewave
//  Version:       1.5
const GUID ID_EQUIP_LEM_EWAVE = { 0xe46981d5, 0x708d, 0x4822, { 0x97, 0xaa, 0xfd, 0xb6, 0xf7, 0x3b, 0x3a, 0xf2 } };

//  Description:   Qualitrol Ewon 4000
//  Version:       1.5
const GUID ID_EQUIP_LEM_EWON4000 = { 0xd4c0895c, 0xfd48, 0x4981, { 0x99, 0x7c, 0x9e, 0x70, 0xd8, 0xe, 0xfb, 0x6 } };

//  Description:   LEM Instruments MBX 300
//  Version:       1.5
const GUID ID_EQUIP_LEM_MBX300 = { 0xd4578d61, 0xdf2b, 0x4218, { 0xa7, 0xb1, 0x5e, 0xf1, 0xa9, 0xbb, 0x85, 0xfa } };

//  Description:   LEM Instruments MBX 601
//  Version:       1.5
const GUID ID_EQUIP_LEM_MBX601 = { 0x1f3cda7b, 0x2ce1, 0x4030, { 0xa3, 0x90, 0xe3, 0xd4, 0x9c, 0x56, 0x15, 0xd2 } };

//  Description:   LEM Instruments MBX 602
//  Version:       1.5
const GUID ID_EQUIP_LEM_MBX602 = { 0x4a157756, 0x414a, 0x427b, { 0x99, 0x32, 0x55, 0x76, 0xe, 0xd5, 0xf7, 0x7 } };

//  Description:   LEM Instruments MBX 603
//  Version:       1.5
const GUID ID_EQUIP_LEM_MBX603 = { 0xf7b4677b, 0xb277, 0x45b5, { 0xaa, 0xae, 0x5f, 0xb3, 0x93, 0x41, 0xb3, 0x90 } };

//  Description:   LEM Instruments MBX 686
//  Version:       1.5
const GUID ID_EQUIP_LEM_MBX686 = { 0x40004266, 0xa978, 0x4991, { 0x9e, 0xd6, 0xc1, 0xcd, 0x73, 0xf, 0x5b, 0xf5 } };

//  Description:   LEM Instruments MBX 800
//  Version:       1.5
const GUID ID_EQUIP_LEM_MBX800 = { 0x1c14b57a, 0xba25, 0x47fb, { 0x88, 0xfa, 0x5f, 0xe5, 0xce, 0xc9, 0x9e, 0x6a } };

//  Description:   LEM Instruments 200
//  Version:       1.5
const GUID ID_EQUIP_LEM_200 = { 0x8449f6b9, 0x10f4, 0x40a7, { 0xa1, 0xc3, 0xbe, 0x33, 0x8e, 0xb9, 0x74, 0x22 } };

//  Description:   LEM Instruments Perma 701
//  Version:       1.5
const GUID ID_EQUIP_LEM_PERMA701 = { 0x9b0dfd9d, 0xd4e9, 0x419d, { 0xba, 0x10, 0xc1, 0xce, 0xe6, 0xcf, 0x8f, 0x93 } };

//  Description:   LEM Instruments Perma 702
//  Version:       1.5
const GUID ID_EQUIP_LEM_PERMA702 = { 0x7f5d62ac, 0x9fab, 0x400f, { 0xb5, 0x1a, 0xf0, 0xf3, 0x94, 0x1f, 0xb5, 0xaa } };

//  Description:   LEM Instruments Perma 705
//  Version:       1.5
const GUID ID_EQUIP_LEM_PERMA705 = { 0xd85fea9c, 0x14d5, 0x45eb, { 0x83, 0x1f, 0xe0, 0x39, 0x73, 0x9, 0x2b, 0xd8 } };

//  Description:   LEM Instruments Perma 706
//  Version:       1.5
const GUID ID_EQUIP_LEM_PERMA706 = { 0x16d6bbfc, 0xb5a, 0x4cf0, { 0x81, 0xcf, 0x48, 0xa3, 0x10, 0x5e, 0xff, 0x4f } };

//  Description:   Qualitrol QWave Light
//  Version:       1.5
const GUID ID_EQUIP_LEM_QWAVELIGHT = { 0x5198ceb9, 0x4b4e, 0x439c, { 0xa1, 0xc0, 0x21, 0x8c, 0x96, 0x3d, 0x6a, 0x9c } };

//  Description:   Qualitrol QWave Micro
//  Version:       1.5
const GUID ID_EQUIP_LEM_QWAVEMICRO = { 0xe0380e52, 0xc205, 0x43a0, { 0x9f, 0xf4, 0x76, 0xfb, 0xd6, 0x76, 0x5f, 0x37 } };

//  Description:   Qualitrol QWave Nomad
//  Version:       1.5
const GUID ID_EQUIP_LEM_QWAVENOMAD = { 0x165f145d, 0x90c3, 0x4591, { 0x95, 0x9a, 0x33, 0xb1, 0x1, 0xd4, 0xbf, 0x8b } };

//  Description:   Qualitrol QWave Power Distribution
//  Version:       1.5
const GUID ID_EQUIP_LEM_QWAVEPOWER_DISTRIBUTION = { 0x2401bf48, 0x9db2, 0x46ec, { 0xac, 0xde, 0x5d, 0xed, 0xde, 0x25, 0xe5, 0x4e } };

//  Description:   Qualitrol QWave Power Transport
//  Version:       1.5
const GUID ID_EQUIP_LEM_QWAVEPOWER_TRANSPORT = { 0xd4422eeb, 0xb1cd, 0x4ba9, { 0xa7, 0xc8, 0x5d, 0x14, 0x1d, 0xf4, 0x5, 0x18 } };

//  Description:   Qualitrol QWave Premium
//  Version:       1.5
const GUID ID_EQUIP_LEM_QWAVEPREMIUM = { 0x6b609a29, 0x4a64, 0x4d1c, { 0xa6, 0xe3, 0xca, 0xef, 0x94, 0xfa, 0x56, 0xa0 } };

//  Description:   Qualitrol QWave Win
//  Version:       1.5
const GUID ID_EQUIP_LEM_QWAVETWIN = { 0x67a42a2d, 0xb831, 0x4222, { 0x80, 0x5e, 0xd5, 0xfd, 0xeb, 0xdd, 0x3a, 0x46 } };

//  Description:   Qualitrol Topas 1000
//  Version:       1.5
const GUID ID_EQUIP_LEM_TOPAS1000 = { 0x459b8614, 0x6724, 0x48fb, { 0xb5, 0xd4, 0xf1, 0x49, 0xed, 0xc, 0x62, 0xf5 } };

//  Description:   Qualitrol Topas 1019
//  Version:       1.5
const GUID ID_EQUIP_LEM_TOPAS1019 = { 0x7b11408b, 0x9d2c, 0x407c, { 0x84, 0xa5, 0x89, 0x44, 0x2, 0x18, 0xdc, 0xf8 } };

//  Description:   Qualitrol Topas 1020
//  Version:       1.5
const GUID ID_EQUIP_LEM_TOPAS1020 = { 0xd1def77d, 0x990f, 0x484e, { 0xa1, 0x66, 0xf7, 0x92, 0x11, 0x70, 0xa6, 0x4b } };

//  Description:   Qualitrol Topas 1040
//  Version:       1.5
const GUID ID_EQUIP_LEM_TOPAS1040 = { 0xd3cc1de2, 0x6e6b, 0x4b6e, { 0xad, 0x90, 0x10, 0xd6, 0x58, 0x5f, 0x8f, 0xa2 } };

//  Description:   Qualitrol Topas SLT
//  Version:       1.5
const GUID ID_EQUIP_LEM_TOPASLT = { 0x9c46483a, 0x541e, 0x4d66, { 0x9c, 0x10, 0xf9, 0x43, 0xab, 0xfc, 0x34, 0x8a } };

//  Description:   Medcal
//  Version:       1.5
const GUID ID_EQUIP_MEDCAL = { 0xf3bfa0a1, 0xeb87, 0x11d2, { 0xac, 0x3e, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   MetOne ELT15
//  Version:       1.5
const GUID ID_EQUIP_METONE_ELT15 = { 0xb5b5da62, 0xe2e1, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   Metrosonic M1
const GUID ID_EQUIP_METROSONIC_M1 = { 0xe6b5172d, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Ranger Hybrid Power And Process Logger
//  Version:       1.6
const GUID ID_EQUIP_ORL_AP300 = { 0x624b1d45, 0xa606, 0x4af6, { 0xa6, 0xe2, 0xc5, 0x0b, 0x60, 0x5f, 0xf7, 0xee } };

//  Description:   Ranger Data Logger
//  Version:       1.6
const GUID ID_EQUIP_ORL_OTHER = { 0xbf20b0a3, 0x00b0, 0x4f30, { 0x8b, 0x40, 0xb6, 0xa4, 0xce, 0x50, 0xa9, 0x3d } };

//  Description:   Ranger Power Master 1000, Single Phase Power Quality Monitor
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM1000 = { 0x01b1f4bd, 0x8532, 0x4a93, { 0x96, 0x99, 0x8b, 0x09, 0xa7, 0x05, 0x01, 0xcf } };

//  Description:   Ranger Power Master 1200, Single Phase Power Quality Monitor
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM1200 = { 0x22ecfe8c, 0xc127, 0x435b, { 0x99, 0x43, 0xde, 0xd5, 0x11, 0xa4, 0x66, 0x8d } };

//  Description:   Ranger Power Master 2000, 0-200A Meter Socket Logger
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM2000 = { 0x629d75b1, 0x1638, 0x4a9c, { 0x9d, 0xdb, 0xf1, 0xd6, 0xc2, 0x4c, 0x33, 0xcb } };

//  Description:   Ranger Power Master 2200, Split Phase Power Quality Monitor
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM2200 = { 0xcdfd3a33, 0xd8b5, 0x42d8, { 0xa8, 0x4f, 0x19, 0x3e, 0x6d, 0xbb, 0x55, 0x45 } };

//  Description:   Ranger Power Master 3000, 3-Phase Power Quality Monitor
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM3000 = { 0xb86d35f2, 0x6d7b, 0x4a3e, { 0xb0, 0x77, 0x10, 0x86, 0xb8, 0xe0, 0xae, 0x19 } };

//  Description:   Ranger Power Master 3006, 6-Channel Current Monitor
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM3006 = { 0x571a0cc0, 0x799f, 0x4d4b, { 0xb6, 0x10, 0x2e, 0xd7, 0x80, 0x34, 0xd9, 0xc2 } };

//  Description:   Ranger Power Master 4000, 3-Phase Power Quality Analyser
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM4000 = { 0x99c08428, 0x54c0, 0x4979, { 0x8a, 0x0c, 0x2a, 0x27, 0x34, 0xe9, 0x89, 0xea } };

//  Description:   Ranger Power Master 6000, 3-Phase Power Quality Analyser
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM6000 = { 0x367f5c25, 0x9f92, 0x4ef0, { 0x92, 0xe8, 0x88, 0xad, 0xa2, 0xcd, 0x44, 0xd2 } };

//  Description:   Ranger Power Master 7000, 3-Phase, Sub-cycle Power Quality Analyser
//  Version:       1.6
const GUID ID_EQUIP_ORL_PM7000 = { 0x8404c5b0, 0x34cb, 0x4748, { 0xac, 0xc2, 0x80, 0x5a, 0x49, 0x01, 0xcf, 0xc1 } };

//  Description:   Ranger II
//  Version:       1.6
const GUID ID_EQUIP_ORL_RANGER_II = { 0xd03a67c1, 0xb030, 0x4b17, { 0xb5, 0xfa, 0x21, 0xef, 0xfa, 0x9b, 0x8f, 0x43 } };

//  Description:   Ranger III
//  Version:       1.6
const GUID ID_EQUIP_ORL_RANGER_III = { 0x7fea241d, 0x9fc0, 0x4df3, { 0xa2, 0xae, 0x23, 0x5b, 0xf1, 0x11, 0x65, 0x06 } };

//  Description:   Ranger IV
//  Version:       1.6
const GUID ID_EQUIP_ORL_RANGER_IV = { 0xb77a58da, 0x5e6e, 0x40eb, { 0xb3, 0x24, 0xc5, 0xcf, 0xb7, 0xe8, 0xfb, 0xa0 } };

//  Description:   Ranger Harmonics Analyser
//  Version:       1.6
const GUID ID_EQUIP_ORL_RANGERHA5000 = { 0xaba3c0b7, 0xb343, 0x4185, { 0xa0, 0xca, 0xd6, 0x5c, 0xcf, 0xd2, 0x30, 0x11 } };

//  Description:   Ranger Meter Socket Logger
//  Version:       1.6
const GUID ID_EQUIP_ORL_RANGERMETERSOCKET = { 0x923c5048, 0xef6f, 0x424e, { 0xba, 0xe6, 0x3f, 0x0b, 0x6d, 0xa2, 0xa8, 0xb9 } };

//  Description:   Ranger Harmonics Power Logger
//  Version:       1.6
const GUID ID_EQUIP_ORL_RANGERRR1250 = { 0x4743fd54, 0x2a6d, 0x4f37, { 0xae, 0xd2, 0x5e, 0x3a, 0x60, 0xe5, 0xdd, 0x99 } };

//  Description:   Ranger Scout
//  Version:       1.6
const GUID ID_EQUIP_ORL_RANGERSCOUT = { 0xb09ddc01, 0x4493, 0x433d, { 0x95, 0x0a, 0x50, 0xab, 0xb9, 0x7a, 0x48, 0x7f } };

//  Description:   PMI Scanner
//  Version:       1.5
const GUID ID_EQUIP_PMI_SCANNER = { 0x609acec1, 0x993d, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Schneider Electric ACM 3710
const GUID ID_EQUIP_PML_3710 = { 0x85726d0, 0x1dc0, 0x11d0, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Schneider Electric ACM 3720
const GUID ID_EQUIP_PML_3720 = { 0x85726d1, 0x1dc0, 0x11d0, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Schneider Electric ACM 3800
const GUID ID_EQUIP_PML_3800 = { 0x85726d2, 0x1dc0, 0x11d0, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Schneider Electric PowerLogic ION 7300
const GUID ID_EQUIP_PML_7300 = { 0x85726d3, 0x1dc0, 0x11d0, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Schneider Electric ION 7700
const GUID ID_EQUIP_PML_7700 = { 0x85726d4, 0x1dc0, 0x11d0, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Schneider Electric PME Log Server
const GUID ID_EQUIP_PML_LOGSERVER = { 0x85726d6, 0x1dc0, 0x11d0, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Schneider Electric Virtual ION Processor
const GUID ID_EQUIP_PML_VIP = { 0x85726d5, 0x1dc0, 0x11d0, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   PSL Power Standards Labs PQube
//  Version:       1.6
const GUID ID_EQUIP_PQUBE = { 0x06afbbc3, 0x35a9, 0x4840, { 0x96, 0xf9, 0x3b, 0x64, 0x38, 0x1b, 0xde, 0x51 } };

//  Description:   Alternate Transients Program
const GUID ID_EQUIP_PUBLIC_ATP = { 0xe6b5172c, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Qualimetre
//  Version:       1.5
const GUID ID_EQUIP_QUALIMETRE = { 0x80c4a764, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Qualitrol QWave Light
//  Version:       1.5
const GUID ID_EQUIP_QWAVE_LIGHT = { 0x80c4a72a, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Qualitrol QWave Micro
//  Version:       1.5
const GUID ID_EQUIP_QWAVE_MICRO = { 0x80c4a727, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Qualitrol QWave Nomad
//  Version:       1.5
const GUID ID_EQUIP_QWAVE_NOMAD = { 0x80c4a72b, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Qualitrol QWave Power Distribution
//  Version:       1.5
const GUID ID_EQUIP_QWAVE_POWER_DISTRIBUTION = { 0x80c4a723, 0x2816, 0x11d4,{ 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Qualitrol QWave Power Transmission
//  Version:       1.5
const GUID ID_EQUIP_QWAVE_POWER_TRANSMISSION = { 0x80c4a725, 0x2816, 0x11d4,{ 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Qualitrol QWave Premium
//  Version:       1.5
const GUID ID_EQUIP_QWAVE_PREMIUM = { 0x80c4a729, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Qualitrol QWave Twin
//  Version:       1.5
const GUID ID_EQUIP_QWAVE_TWIN = { 0x80c4a728, 0x2816, 0x11d4, { 0x8a, 0xb4, 0x0, 0x40, 0x5, 0x69, 0x8d, 0x26 } };

//  Description:   Schneider Electric PowerLogic  System Manager Software
const GUID ID_EQUIP_SQD_SMS = { 0xe6b5172e, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Rockwell Automation I-Grid
//  Version:       1.6
const GUID ID_EQUIP_SST_IGRID = { 0x91a20336, 0x0eba, 0x4785, { 0x99, 0xfb, 0xd6, 0xd4, 0x54, 0x09, 0x9c, 0x8e } };

//  Description:   Telog M1
const GUID ID_EQUIP_TELOG_M1 = { 0xe6b5172f, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Trinergi PQM
//  Version:       1.5
const GUID ID_EQUIP_TRINERGI_PQM = { 0xfd5a3aa, 0xd73a, 0x11d2, { 0xac, 0x3e, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Dranetz DataNode 5510
//  Version:       1.5
const GUID ID_EQUIP_WPT_5510 = { 0x752871de, 0x583, 0x4d44, { 0xa9, 0xae, 0xc5, 0xfa, 0xdc, 0x1, 0x44, 0xac } };

//  Description:   Dranetz DataNode 5520
//  Version:       1.5
const GUID ID_EQUIP_WPT_5520 = { 0xb72d289, 0x7645, 0x40b8, { 0x94, 0x6e, 0xc3, 0xce, 0x4f, 0x1b, 0xcd, 0x37 } };

//  Description:   Dranetz DataNode 5530
const GUID ID_EQUIP_WPT_5530 = { 0xe2da5083, 0x7fdb, 0x11d3, { 0x9b, 0x39, 0x0, 0x40, 0x5, 0x2c, 0x2d, 0x28 } };

//  Description:   Dranetz DataNode 5530T
//  Version:       1.5
const GUID ID_EQUIP_WPT_5530T = { 0x8f88ea9e, 0x1007, 0x4569, { 0xab, 0x47, 0x75, 0x6f, 0x29, 0x2a, 0x23, 0xed } };

//  Description:   Dranetz DataNode 5540
const GUID ID_EQUIP_WPT_5540 = { 0xe2da5084, 0x7fdb, 0x11d3, { 0x9b, 0x39, 0x0, 0x40, 0x5, 0x2c, 0x2d, 0x28 } };

//  Description:   Dranetz DataNode 5560
//  Version:       1.5
const GUID ID_EQUIP_WPT_5560 = { 0x5fd9c0ff, 0x4432, 0x41b5, { 0x9a, 0x9e, 0x9a, 0x32, 0xba, 0x2c, 0xf0, 0x5 } };

//  Description:   Dranetz DataNode 5590
//  Version:       1.5
const GUID ID_EQUIP_WPT_5590 = { 0x2861d5ca, 0x23ac, 0x4a51, { 0xa5, 0xa0, 0x49, 0x8d, 0xa6, 0x1d, 0x26, 0xdd } };

//  Description:   Dranetz Dran-View Software
//  Version:       1.5
const GUID ID_EQUIP_WPT_DRANVIEW = { 0x8d97aa1, 0x1719, 0x11d6, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   BMR PLA44 Power Quality Analyzer
//  Version:       1.6
const GUID ID_EQUIP_BMR_PLA44 = { 0x37996685, 0x8014, 0x4f39, { 0x8b, 0xc1, 0x22, 0xe2, 0x5f, 0x41, 0x23, 0x0e } };

//  Description:   BMR PLA34 Power Quality Analyzer
//  Version:       1.6
const GUID ID_EQUIP_BMR_PLA34 = { 0xe254507f, 0xc7a3, 0x4fea, { 0x9a, 0xa8, 0x1b, 0xa0, 0x0e, 0x05, 0x91, 0x8f } };

//  Description:   BMR PLA404
//  Version:       1.6
const GUID ID_EQUIP_BMR_PLA404 = { 0x9dba84e9, 0xf687, 0x4a1e, { 0x84, 0xc4, 0x1f, 0x15, 0x3f, 0xb0, 0x3a, 0x76 } };

//  Description:   BMR PLA33 Measuring Instrument
//  Version:       1.6
const GUID ID_EQUIP_BMR_PLA33 = { 0x580a644f, 0x1e1f, 0x496b, { 0x8a, 0x12, 0x83, 0x5d, 0xcd, 0x93, 0x29, 0x56 } };

//  Description:   Algodue 4000
//  Version:       1.6
const GUID ID_EQUIP_ALGODUE_PQM_4000 = { 0xD08033AC, 0xF13F, 0x4CC5, { 0x8B, 0xD5, 0x0B, 0xFB, 0x28, 0x26, 0x7D, 0x5F } };

//  Description:   Algodue 4000 RGW
//  Version:       1.6
const GUID ID_EQUIP_ALGODUE_PQM_4000_RGW = { 0x5395C35C, 0x39D2, 0x4BEA, { 0xB1, 0x93, 0x2F, 0x84, 0xEF, 0x45, 0x9D, 0xA4 } };

//  Description:   Algodue 3000
//  Version:       1.6
const GUID ID_EQUIP_ALGODUE_PQM_3000 = { 0xE7D98337, 0x48BF, 0x447D, { 0xA6, 0xDA, 0x43, 0x49, 0xD5, 0xFD, 0xBD, 0x28 } };

//  Description:   Algodue 3000 RGW
//  Version:       1.6
const GUID ID_EQUIP_ALGODUE_PQM_3000_RGW = { 0x2448F42A, 0x63BA, 0x4607, { 0xAE, 0x81, 0x21, 0x42, 0x81, 0xF5, 0x13, 0x71 } };

//  Description:   AMETEK JEMSTAR II Meter
//  Version:       1.6
const GUID ID_EQUIP_AMETEK_JEMSTAR_II = { 0xFC1111B2, 0xD5AA, 0x42E3, { 0xB1, 0x77, 0xDA, 0x87, 0xE8, 0x67, 0xFE, 0x05 } };

//  Description:   AMETEK TR-3000 Digital Fault Recorder
//  Version:       1.6
const GUID ID_EQUIP_AMETEK_TR_3000 = { 0xB1311E36, 0x35DA, 0x489D, { 0x9D, 0x97, 0x66, 0x48, 0x75, 0x2A, 0xB6, 0x00 } };

//  Description:   EDMI MK6E Power Quality Revenue Meter
//  Version:       1.6
const GUID ID_EQUIP_EDMI_Mk6E = { 0x10c9d9c8, 0xa232, 0x48c5, {0xb0, 0x70, 0xa5, 0x8e, 0x5e, 0x5f, 0x8f, 0xed} };

//  Description:   EDMI MK11 Power Quality Revenue Meter
//  Version:       1.6
const GUID ID_EQUIP_EDMI_Mk11 = { 0x86aeb1f6, 0xbc8e, 0x4711, {0x81, 0x98, 0xa9, 0x05, 0x0a, 0x9e, 0x78, 0x66} };

//  Description:   SOCOMEC DIRIS Q800 Network Analyser
//  Version:       1.6
const GUID ID_EQUIP_SOCOMEC_DIRIS_Q800 = { 0x659DE369, 0xEE68, 0x43C1, { 0xBD, 0xEE, 0xB5, 0x9A, 0xE0, 0x4D, 0x7E, 0xD6 } };

//  Description:   Mikronika SO52
//  Version:       1.6
const GUID ID_EQUIP_MIKRONIKA_SO52 = { 0x102A11FB, 0xB773, 0x47E2, { 0xB3, 0x6B, 0xD6, 0xC7, 0xD9, 0x34, 0x86, 0x79 } };

//  Description:   Unipower UP2210
//  Version:       1.6
const GUID ID_EQUIP_UNIPOWER_UP2210 = { 0xfdee90bc, 0x4f2d, 0x67c9, { 0x62, 0x6a, 0x66, 0x80, 0x7d, 0xdd, 0xd7, 0xc4 } };

//  Description:   Unipower Unilyzer 901/902
//  Version:       1.6
const GUID ID_EQUIP_UNIPOWER_UNILYZER901902 = { 0x85967218, 0x58e2, 0x487b, { 0x92, 0xc0, 0x6f, 0xbb, 0x3d, 0x9d, 0x5b, 0xde } };

//  Description:   Unipower Unilyzer 900
//  Version:       1.6
const GUID ID_EQUIP_UNIPOWER_UNILYZER900 = { 0x900ce8c9, 0xbf29, 0x48d2, { 0xb0, 0x6b, 0xe1, 0xb8, 0x47, 0x9e, 0x6a, 0x02 } };

//  Description:   Unilyzer 900C
//  Version:       1.6
const GUID ID_EQUIP_UNIPOWER_UNILYZER900C = { 0x71158963, 0xb71d, 0x4cf8, { 0x88, 0xfe, 0xb5, 0x58, 0xf8, 0xd1, 0x71, 0x6e } };

//  Description:   A. Eberle PQI-DA smart
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQIDASMART = { 0xb933f76a, 0xf44f, 0x4a9a, { 0xb5, 0x1a, 0xbd, 0xb5, 0xd8, 0xb0, 0x8c, 0xa7 } };

//  Description:   A. Eberle PQI-DE
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQIDE = { 0x5bdda2f5, 0x3dd4, 0x43a6, { 0xb4, 0x1c, 0x77, 0x1e, 0x87, 0xdb, 0x0c, 0x24 } };

//  Description:   A. Eberle PQI-D
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQID = { 0xb723891c, 0x6ca2, 0x45a9, { 0xbf, 0x47, 0x06, 0x9f, 0x64, 0x32, 0xf0, 0x65 } };

//  Description:   A. Eberle PQI-DA
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQIDA = { 0x4448a597, 0x5b13, 0x4c21, { 0x8a, 0xf8, 0xa7, 0xc5, 0x21, 0xde, 0x23, 0xa5 } };

//  Description:   A. Eberle PQ-Box 50
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQBOX50 = { 0xff0f970d, 0x8bc0, 0x4ac4, { 0x9a, 0x2c, 0xd8, 0xb4, 0xfd, 0x69, 0xb5, 0xcb } };

//  Description:   A. Eberle PQ-Box 100
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQBOX100 = { 0xe2213646, 0x75d0, 0x42e8, { 0x88, 0x59, 0x16, 0xc3, 0x49, 0xd4, 0x75, 0x88 } };

//  Description:   A. Eberle PQ-Box 150
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQBOX150 = { 0x5574f874, 0x377c, 0x4b71, { 0xa3, 0x1d, 0x06, 0x76, 0xf7, 0x97, 0x59, 0xda } };

//  Description:   A. Eberle PQ-Box 200
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQBOX200 = { 0x7764c2ec, 0x8f14, 0x4e19, { 0xa5, 0xbb, 0x5a, 0x2a, 0xdf, 0xbb, 0xd4, 0x88 } };

//  Description:   A. Eberle PQ-Box 300
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQBOX300 = { 0x3022f6e2, 0x2899, 0x4005, { 0xa8, 0xd0, 0x27, 0x3f, 0x1f, 0x95, 0xf6, 0x3d } };

//  Description:   A. Eberle PQ-Box WinPQ
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_WINPQ = { 0xb48e3cdc, 0xebdf, 0x4456, { 0xac, 0x7f, 0xdd, 0x83, 0x76, 0x81, 0x6d, 0x7c } };

//  Description:   A. Eberle PQ-Box WebPQ
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_WEBPQ = { 0xd2b4398b, 0x7df7, 0x4b78, { 0xaa, 0x81, 0x43, 0x36, 0x4c, 0x16, 0x74, 0xdb } };

//  Description:   Schneider Electric PowerLogic PM 2000
//  Version:       1.7
const GUID ID_EQUIP_SE_PM_2000 = { 0x43FC90A3, 0xD066, 0x4724, { 0x97, 0x3E, 0x8B, 0x14, 0xA0, 0x36, 0x71, 0x42 } };

//  Description:   Schneider Electric PowerLogic PM 3000
//  Version:       1.7
const GUID ID_EQUIP_SE_PM_3000 = { 0xE6D3D337, 0x1C9A, 0x4F4D, { 0xB4, 0x75, 0x78, 0x4C, 0xFD, 0x11, 0xB3, 0x49 } };

//  Description:   Schneider Electric PowerLogic EM 4200
//  Version:       1.7
const GUID ID_EQUIP_SE_EM_4200 = { 0xE6DA3C6A, 0x73A5, 0x4821, { 0x85, 0x49, 0x01, 0x91, 0xD6, 0x9F, 0xB3, 0x5A } };

//  Description:   Schneider Electric PowerLogic Branch Circuit Power Meter
//  Version:       1.7
const GUID ID_EQUIP_SE_BCPM = { 0x260D77FB, 0x64AC, 0x4189, { 0x9C, 0xF2, 0x0E, 0x53, 0xB8, 0x9B, 0xF3, 0x80 } };

//  Description:   Schneider Electric PowerLogic EM 3500
//  Version:       1.7
const GUID ID_EQUIP_SE_EM_3500 = { 0x6BCE86B0, 0x709B, 0x4487, { 0x93, 0xCF, 0x60, 0x9D, 0xAD, 0x28, 0x5C, 0x63 } };

//  Description:   Schneider Electric PowerLogic EM 4800
//  Version:       1.7
const GUID ID_EQUIP_SE_EM_4800 = { 0x1108919D, 0x764B, 0x47F0, { 0x9C, 0xA9, 0xB0, 0x33, 0x62, 0xFC, 0xBB, 0x6F } };

//  Description:   Schneider Electric EM 4900
//  Version:       1.7
const GUID ID_EQUIP_SE_EM_4900 = { 0xDA97D7E9, 0x9EAB, 0x4777, { 0x9C, 0x8F, 0xA8, 0x6B, 0x66, 0x87, 0xF7, 0x4E } };

//  Description:   Schneider Electric PowerLogic HDPM 6000
//  Version:       1.7
const GUID ID_EQUIP_SE_PM_HDPM_6000 = { 0x88F05EEF, 0xA828, 0x4800, { 0xA2, 0x8E, 0x49, 0x6A, 0xBB, 0x8A, 0xAB, 0xF5 } };

//  Description:   Schneider Electric PowerLogic PM 5000
//  Version:       1.7
const GUID ID_EQUIP_SE_PM_5000 = { 0x7CACAC26, 0xF08C, 0x41C7, { 0x82, 0x78, 0x30, 0x91, 0x50, 0x54, 0x4E, 0x5F } };

//  Description:   Schneider Electric PowerLogic ION 7330
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_7330 = { 0xE8AD05C3, 0x4DE9, 0x49E8, { 0x81, 0x11, 0xFA, 0xC7, 0x91, 0x87, 0x29, 0xF7 } };

//  Description:   Schneider Electric PowerLogic ION 7350
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_7350 = { 0x3BC4D079, 0x9A7A, 0x47C5, { 0x8B, 0xFA, 0xF9, 0x2F, 0xE6, 0x6A, 0xD2, 0xF3 } };

//  Description:   Schneider Electric PowerLogic ION 7550
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_7550 = { 0x1C84B796, 0xEA3E, 0x4A40, { 0x9C, 0x69, 0x01, 0x95, 0x49, 0x46, 0x55, 0xFF } };

//  Description:   Schneider Electric PowerLogic ION 7650
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_7650 = { 0x5C1DE84E, 0xD23E, 0x4711, { 0xBE, 0x8B, 0x6F, 0x81, 0x65, 0x74, 0x80, 0xA4 } };

//  Description:   Schneider Electric PowerLogic PM 8000
//  Version:       1.7
const GUID ID_EQUIP_SE_PM_8000 = { 0xC6641E95, 0x8787, 0x4CCB, { 0xB7, 0x7E, 0xA6, 0x71, 0x9C, 0x07, 0x57, 0xF3 } };

//  Description:   Schneider Electric PowerLogic ION 9000
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_9000 = { 0xF01D730C, 0x11C2, 0x4A1C, { 0x93, 0xA5, 0xA6, 0x85, 0xCE, 0xF9, 0x32, 0x95 } };

//  Description:   Schneider Electric PowerLogic ION 9000T
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_9000T = { 0x7AFA54C0, 0xA63E, 0x4C79, { 0x9F, 0x49, 0xDC, 0x6D, 0x11, 0x1B, 0x2F, 0xB9 } };

//  Description:   Schneider Electric PowerLogic ION 8650
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_8650 = { 0xA4CFA0E4, 0x02B1, 0x40E4, { 0xB2, 0xC8, 0xDF, 0x3B, 0xF4, 0x32, 0x68, 0x79 } };

//  Description:   Schneider Electric PowerLogic ION 7400
//  Version:       1.7
const GUID ID_EQUIP_SE_ION_7400 = { 0x7874D2EB, 0xA8C7, 0x4995, { 0xA9, 0x85, 0x51, 0x9D, 0xE3, 0xB5, 0xBE, 0xD1 } };

//  Description:   Schneider Electric PowerLogic P5
//  Version:       1.7
const GUID ID_EQUIP_SE_P5 = { 0x1A483E62, 0xA43F, 0x4B5D, { 0x92, 0x21, 0x61, 0xB1, 0x6D, 0x63, 0x99, 0x35 } };

//  Description:   Schneider Electric EcoStruxure Power Monitoring Expert
//  Version:       1.7
const GUID ID_EQUIP_SE_PME = { 0x18CBC8E2, 0x58C1, 0x4BB4, { 0xA2, 0x05, 0x81, 0x8B, 0x9B, 0xBA, 0x27, 0x40 } };

//  Description:   Schneider Electric EcoStruxure Power Operation
//  Version:       1.7
const GUID ID_EQUIP_SE_PO = { 0x16E82944, 0x00F0, 0x41EC, { 0x99, 0x36, 0xE7, 0x3D, 0x90, 0xD4, 0xCC, 0x8E } };

//  Description:   Schneider Electric EcoStruxure Power Advisor
//  Version:       1.7
const GUID ID_EQUIP_SE_PA = { 0x02FD6520, 0x0977, 0x4995, { 0x9B, 0xF3, 0x76, 0x89, 0xAC, 0x5E, 0xD1, 0x2B } };

//  Description:   Schneider Electric StruxureWare Power Monitoring PQDIF Exporter
//  Version:       1.7
const GUID ID_EQUIP_SE_PQDIF_EXPORTER = { 0xE8CAE48A, 0xEDFE, 0x47E3, { 0x9F, 0x2B, 0x0B, 0x13, 0xED, 0xE4, 0xB0, 0xFA } };

//  Description:   A. Eberle PQI-LV
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQILV = { 0xAB9BB8EF, 0xBFB3, 0x4028, { 0x86, 0x0C, 0xF9, 0x0B, 0x29, 0x1F, 0x54, 0x79 } };

//  Description:   A. Eberle PQ-Box 400
//  Version:       1.7
const GUID ID_EQUIP_AEBERLE_PQBOX400 = { 0x869E1CE4, 0xEFBD, 0x4233, { 0xAB, 0x46, 0x6E, 0xF1, 0x73, 0xC9, 0x2B, 0xAF } };

//  Description:   ABB M4M 40
//  Version:       1.7
const GUID ID_EQUIP_ABB_M4M40 = { 0x72B87E94, 0xE2AE, 0x4034, { 0x84, 0x20, 0xEB, 0x38, 0xDF, 0xB7, 0x7F, 0xEE } };

//  Description:   ABB M4M 50
//  Version:       1.7
const GUID ID_EQUIP_ABB_M4M50 = { 0xB22741CF, 0x6872, 0x41B9, { 0x9B, 0xC5, 0xB6, 0xFE, 0x8B, 0xE7, 0x78, 0x0A } };

//  Description:   Camille Bauer LINAX PQ1000
//  Version:       1.7
const GUID ID_EQUIP_CB_PQ1000 = { 0x1f757b23, 0x87cb, 0x437d, { 0x81, 0xb4, 0xe2, 0x92, 0xb5, 0x05, 0xef, 0xca } };;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagInstrumentTypeID
//  ==========================================================

//  Description:   Scope Recorder
//  Version:       1.0
const GUID ID_INSTR_TYPE_SCOPE = { 0xe6b51735, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Frequency Recorder
//  Version:       1.0
const GUID ID_INSTR_TYPE_FR = { 0xe6b51736, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Power Quality Monitor
//  Version:       1.0
const GUID ID_INSTR_TYPE_PQM = { 0xe6b51737, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Voltage Recorder
//  Version:       1.0
const GUID ID_INSTR_TYPE_VR = { 0xe6b51738, 0xf747, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Spectrum Analyzer
//  Version:       1.0
const GUID ID_INSTR_TYPE_SA = { 0xc690e871, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagPhaseID
//  ==========================================================

//  Description:   Phase is not applicable
//  Version:       1.0
const int ID_PHASE_NONE = 0;

//  Description:   A-to-neutral
//  Version:       1.0
const int ID_PHASE_AN = 1;

//  Description:   B-to-neutral
//  Version:       1.0
const int ID_PHASE_BN = 2;

//  Description:   C-to-neutral
//  Version:       1.0
const int ID_PHASE_CN = 3;

//  Description:   Neutral-to-ground
//  Version:       1.0
const int ID_PHASE_NG = 4;

//  Description:   A-to-B
//  Version:       1.0
const int ID_PHASE_AB = 5;

//  Description:   B-to-C
//  Version:       1.0
const int ID_PHASE_BC = 6;

//  Description:   C-to-A
//  Version:       1.0
const int ID_PHASE_CA = 7;

//  Description:   Residual Phase - the vector or point-on-wave sum of Phases A, B, and C.  Should be zero in a perfectly balanced system.
//  Version:       1.0
const int ID_PHASE_RES = 8;

//  Description:   Net Phase - the vector or point-on-wave sum of Phases A, B, C, and the Neutral phase.  Should be zero in a 4-wire system with no earth return path.
//  Version:       1.0
const int ID_PHASE_NET = 9;

//  Description:   The value representing a total or other summarizing value in a multi-phase system
//  Version:       1.5
const int ID_PHASE_TOTAL = 13;

//  Description:   The value representing average of 3 line-neutral values
//  Version:       1.5
const int ID_PHASE_LN_AVE = 14;

//  Description:   The value representing average of 3 line-line values
//  Version:       1.5
const int ID_PHASE_LL_AVE = 15;

//  Description:   The value representing the "worst" of the 3 phases
//  Version:       1.5
const int ID_PHASE_WORST = 16;

//  Description:   DC Positive
//  Version:       1.5
const int ID_PHASE_PLUS = 17;

//  Description:   DC Negative
//  Version:       1.5
const int ID_PHASE_MINUS = 18;

//  Description:   Generic Phase 1
//  Version:       1.5
const int ID_PHASE_GENERAL_1 = 19;

//  Description:   Generic Phase 2
//  Version:       1.5
const int ID_PHASE_GENERAL_2 = 20;

//  Description:   Generic Phase 3
//  Version:       1.5
const int ID_PHASE_GENERAL_3 = 21;

//  Description:   Generic Phase 4
//  Version:       1.5
const int ID_PHASE_GENERAL_4 = 22;

//  Description:   Generic Phase 5
//  Version:       1.5
const int ID_PHASE_GENERAL_5 = 23;

//  Description:   Generic Phase 6
//  Version:       1.5
const int ID_PHASE_GENERAL_6 = 24;

//  Description:   Generic Phase 7
//  Version:       1.5
const int ID_PHASE_GENERAL_7 = 25;

//  Description:   Generic Phase 8
//  Version:       1.5
const int ID_PHASE_GENERAL_8 = 26;

//  Description:   Generic Phase 9
//  Version:       1.5
const int ID_PHASE_GENERAL_9 = 27;

//  Description:   Generic Phase 10
//  Version:       1.5
const int ID_PHASE_GENERAL_10 = 28;

//  Description:   Generic Phase 11
//  Version:       1.5
const int ID_PHASE_GENERAL_11 = 29;

//  Description:   Generic Phase 12
//  Version:       1.5
const int ID_PHASE_GENERAL_12 = 30;

//  Description:   Generic Phase 13
//  Version:       1.5
const int ID_PHASE_GENERAL_13 = 31;

//  Description:   Generic Phase 14
//  Version:       1.5
const int ID_PHASE_GENERAL_14 = 32;

//  Description:   Generic Phase 15
//  Version:       1.5
const int ID_PHASE_GENERAL_15 = 33;

//  Description:   Generic Phase 16
//  Version:       1.5
const int ID_PHASE_GENERAL_16 = 34;

//  Description:   The value representing maximum of 3 line-neutral values
//  Version:       1.6
const int ID_PHASE_LN_MAX = 36;

//  Description:   The value representing minimum of 3 line-neutral values
//  Version:       1.6
const int ID_PHASE_LN_MIN = 37;

//  Description:   The value representing maximum of 3 line-line values
//  Version:       1.6
const int ID_PHASE_LL_MAX = 38;

//  Description:   The value representing minimum of 3 line-line values
//  Version:       1.6
const int ID_PHASE_LL_MIN = 39;

//  Description:   Ground
//  Version:       1.7
const int ID_PHASE_GROUND = 40;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagQuantityTypeID
//  ==========================================================

//  Description:   TIME, VAL. For point-on-wave measurements
//  Version:       1.0
const GUID ID_QT_WAVEFORM = { 0x67f6af80, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   TIME, MIN, MAX, AVG, INST, VAL … / For time-based logged entries
//  Version:       1.5
const GUID ID_QT_VALUELOG = { 0x67f6af82, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   TIME, MIN, MAX, AVG, INST, VAL, PHASEANGLE … / For time-domain  measurements including magnitudes and (optionally) phase angle
//  Version:       1.5
const GUID ID_QT_PHASOR = { 0x67f6af81, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   VAL (FREQUENCY), VAL, PHASEANGLE / For frequency-domain measurements including magnitude and (optionally) phase angle.
//  Version:       1.0
const GUID ID_QT_RESPONSE = { 0x67f6af85, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   TIME, LAT, LON, VAL, POLARITY, ELLIPSE
//  Version:       1.0
const GUID ID_QT_FLASH = { 0x67f6af83, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   BINLOW, BINHIGH, BINID, COUNT
//  Version:       1.0
const GUID ID_QT_HISTOGRAM = { 0x67f6af87, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   XBINLOW, XBINHIGH, YBINLOW, YBINHIGH, BINID, COUNT
//  Version:       1.0
const GUID ID_QT_HISTOGRAM3D = { 0x67f6af88, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   PROB, VAL. (Note that the specific P1 value types have been deprecated.)
//  Version:       1.0
const GUID ID_QT_CPF = { 0x67f6af89, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   VAL, VAL
//  Version:       1.0
const GUID ID_QT_XY = { 0x67f6af8a, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   VAL, DURATION
//  Version:       1.0
const GUID ID_QT_MAGDUR = { 0x67f6af8b, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   VAL, VAL, VAL
//  Version:       1.0
const GUID ID_QT_XYZ = { 0x67f6af8c, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   TIME, VAL, DURATION
//  Version:       1.0
const GUID ID_QT_MAGDURTIME = { 0x67f6af8d, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   TIME, VAL, DURATION, COUNT
//  Version:       1.0
const GUID ID_QT_MAGDURCOUNT = { 0x67f6af8e, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagDisturbanceCategoryID
//  ==========================================================

//  Description:   No IEEE 1159 definition applicable or desired
//  Version:       1.0
const GUID ID_DISTURB_1159_NONE = { 0x67f6af8f, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   IEEE 1159 Transient
//  Version:       1.0
const GUID ID_DISTURB_1159_TRANSIENT = { 0x67f6af90, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   IEEE 1159 Impulsive Transient
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_IMPULSIVE = { 0xdd56ef60, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Impulsive Transient - nanosecond duration
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_IMPULSIVE_NANO = { 0xdd56ef61, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Impulsive Transient - microsecond duration
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_IMPULSIVE_MICRO = { 0xdd56ef63, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Impulsive Transient - millisecond duration
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_IMPULSIVE_MILLI = { 0xdd56ef64, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Oscillatory Transient
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_OSCILLATORY = { 0xdd56ef65, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Oscillatory Transient - Low Frequency
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_OSCILLATORY_LOWFREQ = { 0xdd56ef66, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Oscillatory Transient - Medium Frequency
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_OSCILLATORY_MEDFREQ = { 0xdd56ef67, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Oscillatory Transient - High Frequency
//  Version:       1.5
const GUID ID_DISTURB_1159_TRANSIENT_OSCILLATORY_HIGHFREQ = { 0xdd56ef68, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation
//  Version:       1.0
const GUID ID_DISTURB_1159_SHORTDUR = { 0x67f6af91, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Instantaneous duration
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_INSTANT = { 0xdd56ef69, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Instantaneous Sag
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_INSTANT_SAG = { 0xdd56ef6a, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Instantaneous Swell
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_INSTANT_SWELL = { 0xdd56ef6b, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Momentary Duration
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_MOMENT = { 0xdd56ef6c, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Momentary Interruption
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_MOMENT_INTERRUPT = { 0xdd56ef6d, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Momentary Sag
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_MOMENT_SAG = { 0xdd56ef6e, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Momentary Swell
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_MOMENT_SWELL = { 0xdd56ef6f, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Temporary Duration
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_TEMP = { 0xdd56ef70, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Temporary Interruption
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_TEMP_INTERRUPT = { 0xdd56ef71, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Temporary Sag
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_TEMP_SAG = { 0xdd56ef72, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Short Duration RMS Variation - Temporary Swell
//  Version:       1.5
const GUID ID_DISTURB_1159_SHORTDUR_TEMP_SWELL = { 0xdd56ef73, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Long Duration RMS Variation
//  Version:       1.0
const GUID ID_DISTURB_1159_LONGDUR = { 0x67f6af92, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   IEEE 1159 Long Duration RMS Variation - Interruption
//  Version:       1.5
const GUID ID_DISTURB_1159_LONGDUR_INTERRUPT = { 0xdd56ef74, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Long Duration RMS Variation - Undervoltage
//  Version:       1.5
const GUID ID_DISTURB_1159_LONGDUR_SAG = { 0xdd56ef75, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Long Duration RMS Variation - Overvoltage
//  Version:       1.5
const GUID ID_DISTURB_1159_LONGDUR_SWELL = { 0xdd56ef76, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Imbalance
//  Version:       1.5
const GUID ID_DISTURB_1159_IMBALANCE = { 0xdd56ef77, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Power Frequency Variation
//  Version:       1.5
const GUID ID_DISTURB_1159_POWERFREQVARIATION = { 0xdd56ef7e, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Voltage Fluctuation (causes light flicker)
//  Version:       1.5
const GUID ID_DISTURB_1159_VOLTAGEFLUCTUATION = { 0x67f6af93, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   IEEE 1159 Waveform Distortion
//  Version:       1.5
const GUID ID_DISTURB_1159_WAVEDISTORT = { 0x67f6af94, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   DC offset of voltage or current waveform
//  Version:       1.5
const GUID ID_DISTURB_1159_WAVEDISTORT_DCOFFSET = { 0xdd56ef78, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Waveform Harmonics Present
//  Version:       1.5
const GUID ID_DISTURB_1159_WAVEDISTORT_HARMONIC = { 0xdd56ef79, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Waveform Interharmonics Present
//  Version:       1.5
const GUID ID_DISTURB_1159_WAVEDISTORT_INTERHARMONIC = { 0xdd56ef7a, 0x7edd, 0x11d2, { 0xb3, 0xa, 0x0, 0x60, 0x97, 0x89, 0xd1, 0x93 } };

//  Description:   IEEE 1159 Waveform Notching Present
//  Version:       1.5
const GUID ID_DISTURB_1159_WAVEDISTORT_NOTCHING = { 0x67f6af95, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   IEEE 1159 Waveform Noise Present
//  Version:       1.5
const GUID ID_DISTURB_1159_WAVEDISTORT_NOISE = { 0x67f6af96, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagQuantityUnitsID
//  ==========================================================

//  Description:   Unitless
//  Version:       1.0
const int ID_QU_NONE = 0;

//  Description:   Seconds -- relative from the beginning time of the observation (using tagTimeStart as the beginning time).
//  Version:       1.0
const int ID_QU_SECONDS = 2;

//  Description:   Absolute time. Each timestamp in the series shall be in absolute time using the TIMESTAMPPQDIF physical type. This is generally *not* recommended but is acceptable when _VALUELOG is used.
//  Version:       1.0
const int ID_QU_TIMESTAMP = 1;

//  Description:   The timestamps are in cycles, relative to tagTimeStart
//  Version:       1.0
const int ID_QU_CYCLES = 3;

//  Description:   Volts
//  Version:       1.0
const int ID_QU_VOLTS = 6;

//  Description:   Amperes
//  Version:       1.0
const int ID_QU_AMPS = 7;

//  Description:   Volt-Amperes
//  Version:       1.0
const int ID_QU_VA = 8;

//  Description:   Watts
//  Version:       1.0
const int ID_QU_WATTS = 9;

//  Description:   Volt-amperes reactive
//  Version:       1.0
const int ID_QU_VARS = 10;

//  Description:   Ohms
//  Version:       1.0
const int ID_QU_OHMS = 11;

//  Description:   Siemens
//  Version:       1.0
const int ID_QU_SIEMENS = 12;

//  Description:   Volts per amp
//  Version:       1.0
const int ID_QU_VOLTSPERAMP = 13;

//  Description:   Joules
//  Version:       1.0
const int ID_QU_JOULES = 14;

//  Description:   Hertz
//  Version:       1.0
const int ID_QU_HERTZ = 15;

//  Description:   Celsius
//  Version:       1.0
const int ID_QU_CELSIUS = 16;

//  Description:   Degrees of Arc
//  Version:       1.0
const int ID_QU_DEGREES = 17;

//  Description:   Decibels
//  Version:       1.0
const int ID_QU_DB = 18;

//  Description:   Percent
//  Version:       1.0
const int ID_QU_PERCENT = 19;

//  Description:   Per Unit
//  Version:       1.0
const int ID_QU_PERUNIT = 20;

//  Description:   Number of counts or samples
//  Version:       1.0
const int ID_QU_SAMPLES = 21;

//  Description:   Energy - var-hours
//  Version:       1.5
const int ID_QU_VARHOURS = 22;

//  Description:   Energy - Watt-hours
//  Version:       1.5
const int ID_QU_WATTHOURS = 23;

//  Description:   Energy - VA-hours
//  Version:       1.5
const int ID_QU_VAHOURS = 24;

//  Description:   Meters/Second
//  Version:       1.5
const int ID_QU_MPS = 25;

//  Description:   Miles/Hr
//  Version:       1.5
const int ID_QU_MPH = 26;

//  Description:   Pressure, Bars
//  Version:       1.5
const int ID_QU_BARS = 27;

//  Description:   Pressure, Pascal
//  Version:       1.5
const int ID_QU_PASCALS = 28;

//  Description:   Force, Newtons
//  Version:       1.5
const int ID_QU_NEWTONS = 29;

//  Description:   Torque, Newton-Meters
//  Version:       1.5
const int ID_QU_NEWTONMETERS = 30;

//  Description:   Revolutions/Minute
//  Version:       1.5
const int ID_QU_RPM = 31;

//  Description:   Radians/Second
//  Version:       1.5
const int ID_QU_RADPERSEC = 32;

//  Description:   Meters
//  Version:       1.5
const int ID_QU_METERS = 33;

//  Description:   Flux Linkage - Weber Turns
//  Version:       1.5
const int ID_QU_WEBERTURNS = 34;

//  Description:   Flux Density - Teslas
//  Version:       1.5
const int ID_QU_TESLAS = 35;

//  Description:   Magnetic Field - Webers
//  Version:       1.5
const int ID_QU_WEBERS = 36;

//  Description:   Volts/Volts transfer function
//  Version:       1.5
const int ID_QU_VOLTSPERVOLT = 37;

//  Description:   Amps/Amps transfer function
//  Version:       1.5
const int ID_QU_AMPSPERAMP = 38;

//  Description:   Impedance Transfer Function
//  Version:       1.5
const int ID_QU_AMPSPERVOLT = 39;

//  Description:   Acceleration in Meters/Second²
//  Version:       1.6
const int ID_QU_MPS2 = 40;

//  Description:   Jerk in Meters/Second³
//  Version:       1.6
const int ID_QU_MPS3 = 41;

//  Description:   Radians/Second²
//  Version:       1.6
const int ID_QU_RADPERSEC2 = 42;

//  Description:   Radians/Second³
//  Version:       1.6
const int ID_QU_RADPERSEC3 = 43;

//  Description:   Sound Absorption in Metric Sabins
//  Version:       1.6
const int ID_QU_SABINS = 44;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagValueTypeID
//  ==========================================================

//  Description:   This should be the default value type for a measurement -- a value
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_VAL = { 0x67f6af97, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Time
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_TIME = { 0xc690e862, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Minimum
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_MIN = { 0x67f6af98, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Maximum
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_MAX = { 0x67f6af99, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Average
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_AVG = { 0x67f6af9a, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Instantaneous. This was deprecated in version 1.5 and should not be used. Use ID_SERIES_VALUE_TYPE_VAL instead.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_INST = { 0x67f6af9b, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Phase angle (used for a _VAL series or when it applies to all).
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_PHASEANGLE = { 0x3d786f9d, 0xf76e, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Phase angle that corresponds to a _MIN series (completing a complex pair)
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_PHASEANGLE_MIN = { 0xdc762340, 0x3c56, 0x11d2, { 0xae, 0x44, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Phase angle that corresponds to a _MAX series
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_PHASEANGLE_MAX = { 0xdc762341, 0x3c56, 0x11d2, { 0xae, 0x44, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Phase angle that corresponds to an _AVG series
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_PHASEANGLE_AVG = { 0xdc762342, 0x3c56, 0x11d2, { 0xae, 0x44, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Area under the signal, usually an rms voltage, current or other quantity
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_AREA = { 0xc7825ce0, 0x8ace, 0x11d3, { 0xb9, 0x2f, 0x0, 0x50, 0xda, 0x2b, 0x1f, 0x4d } };

//  Description:   Latitude
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_LATITUDE = { 0xc690e864, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Duration
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_DURATION = { 0xc690e863, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Longitude
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_LONGITUDE = { 0xc690e865, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Polarity
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_POLARITY = { 0xc690e866, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Ellipse (for lightning flash density).
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_ELLIPSE = { 0xc690e867, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Histogram Bin ID
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_BINID = { 0xc690e869, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   High Bin
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_BINHIGH = { 0xc690e86a, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Low Bin
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_BINLOW = { 0xc690e86b, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   X-Axis High Bin
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_XBINHIGH = { 0xc690e86c, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   X-Axis Low Bin
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_XBINLOW = { 0xc690e86d, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Y-Axis High Bin
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_YBINHIGH = { 0xc690e86e, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Y-Axis Low Bin
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_YBINLOW = { 0xc690e86f, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Count
//  Version:       1.0
const GUID ID_SERIES_VALUE_TYPE_COUNT = { 0xc690e870, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Transition event code series.  This series contains codes corresponding to values in a value series that indicates what kind of transition caused the event to be recorded.  Used only with VALUELOG data.
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_TRANSITION = { 0x5369c260, 0xc347, 0x11d2, { 0x92, 0x3f, 0x0, 0x10, 0x4b, 0x2b, 0x84, 0xb1 } };

//  Description:   Cumulative probability in percent
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_PROB = { 0x6763cc71, 0x17d6, 0x11d4, { 0x9f, 0x1c, 0x0, 0x20, 0x78, 0xe0, 0xb7, 0x23 } };

//  Description:   Interval data
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_INTERVAL = { 0x72e82a40, 0x336c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Status Data
//  Version:       1.5
const GUID ID_SERIES_VALUE_TYPE_STATUS = { 0xb82b5c82, 0x55c7, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Probability: 1%. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_P1 = { 0x67f6af9c, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Probability: 5%. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_P5 = { 0x67f6af9d, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Probability: 10%. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_P10 = { 0x67f6af9e, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Probability: 90%. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_P90 = { 0x67f6af9f, 0xf753, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Probability: 95%. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_P95 = { 0xc690e860, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Probability: 99%. This was deprecated in version 1.5 and should not be used.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_P99 = { 0xc690e861, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   Frequency. This was deprecated in version 1.5 and should not be used. It is now a quantity characteristic instead of a value type.
//  Version:       1.5 Deprecated
const GUID ID_SERIES_VALUE_TYPE_FREQUENCY = { 0xc690e868, 0xf755, 0x11cf, { 0x9d, 0x89, 0x0, 0x80, 0xc7, 0x2e, 0x70, 0xa3 } };

//  Description:   The rms average of a series of values
//  Version:       1.6
const GUID ID_SERIES_VALUE_TYPE_RMS = { 0xabaaaa34, 0x6e08, 0x49ae,{ 0xb5, 0x22, 0x18, 0x8c, 0xdf, 0x6a, 0x59, 0x4c } };

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagStorageMethodID
//  ==========================================================

//  Description:   The data in tagSeriesValues are a straight array of data points
//  Version:       1.0
const int ID_SERIES_METHOD_VALUES = 0x01;

//  Description:   All values in tagSeriesValues will be multiplied by tagSeriesScale
//  Version:       1.0
const int ID_SERIES_METHOD_SCALED = 0x02;

//  Description:   The data in tagSeriesValues consists of a special sequence to indicate the contents of a regular rate series (see main documentation for details). The vector contains: #rates, number of points 1, rate 1 ... number of points N, rate N.
//  Version:       1.0
const int ID_SERIES_METHOD_INCREMENT = 0x04;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagHintGreekPrefixID
//  ==========================================================

//  Description:   Value by preference
//  Version:       1.0
const int ID_GREEK_DONTCARE = 0;

//  Description:   By default, displayed values should be multiplied by 1E+15 and units should use prefix symbol f
//  Version:       1.0
const int ID_GREEK_FEMTO = 1;

//  Description:   By default, displayed values should be multiplied by 1E+12 and units should use prefix symbol p
//  Version:       1.0
const int ID_GREEK_PICO = 2;

//  Description:   By default, displayed values should be multiplied by 1E+9 and units should use prefix symbol n
//  Version:       1.0
const int ID_GREEK_NANO = 3;

//  Description:   By default, displayed values should be multiplied by 1E+6 and units should use prefix symbol µ
//  Version:       1.0
const int ID_GREEK_MICRO = 4;

//  Description:   By default, displayed values should be multiplied by 1E+3 and units should use prefix symbol m
//  Version:       1.0
const int ID_GREEK_MILLI = 5;

//  Description:   By default, displayed values should displayed with no rescaling and displayed with no prefix symbol
//  Version:       1.0
const int ID_GREEK_NONE = 6;

//  Description:   By default, displayed values should be multiplied by 1E-3 and units should use prefix symbol k
//  Version:       1.0
const int ID_GREEK_KILO = 7;

//  Description:   By default, displayed values should be multiplied by 1E-6 and units should use prefix symbol M
//  Version:       1.0
const int ID_GREEK_MEGA = 8;

//  Description:   By default, displayed values should be multiplied by 1E-12 and units should use prefix symbol T
//  Version:       1.0
const int ID_GREEK_TERA = 10;

//  Description:   By default, displayed values should be multiplied by 1E-9 and units should use prefix symbol G
//  Version:       1.0
const int ID_GREEK_GIGA = 9;

//  Description:   By default, displayed values should be multiplied by 1E-15 and units should use prefix symbol P
//  Version:       1.6 Deprecated
const int ID_GREEK_PETA = 11;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagHintPreferredUnitsID
//  ==========================================================

//  Description:   By default, displayed values should be displayed in engineering units (that is, not in per unit or percent)
//  Version:       1.0
const int ID_PREFER_ENG = 1;

//  Description:   By default, displayed values should be displayed in percent
//  Version:       1.0
const int ID_PREFER_PCT = 2;

//  Description:   By default, displayed values should be displayed in per unit
//  Version:       1.0
const int ID_PREFER_PU = 3;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagHintDefaultDisplayID
//  ==========================================================

//  Description:   No default is specified for display of phasors
//  Version:       1.0
const int ID_DEFAULT_DONTCARE = 0;

//  Description:   By default, the magnitude component of phasors should be displayed
//  Version:       1.0
const int ID_DEFAULT_MAG = 1;

//  Description:   By default, the phase angle component of phasors should be displayed
//  Version:       1.0
const int ID_DEFAULT_ANG = 2;

//  Description:   By default, the real magnitude component of phasors should be displayed
//  Version:       1.0
const int ID_DEFAULT_REAL = 3;

//  Description:   By default, the imaginary magnitude component of phasors should be displayed
//  Version:       1.0
const int ID_DEFAULT_IMAG = 4;

//  Description:   By default, the real and imaginary magnitude component of phasors should be displayed together
//  Version:       1.0
const int ID_DEFAULT_RX = 5;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagTriggerTypeID
//  ==========================================================

//  Description:   No Trigger
//  Version:       1.0
const int ID_TRIG_NONE = 0x00;

//  Description:   Low Threshold Trigger
//  Version:       1.0
const int ID_TRIG_LOW = 0x01;

//  Description:   High Threshold Trigger
//  Version:       1.0
const int ID_TRIG_HIGH = 0x02;

//  Description:   Rate of Change Trigger
//  Version:       1.0
const int ID_TRIG_RATE = 0x04;

//  Description:   Measurement Shape Trigger
//  Version:       1.0
const int ID_TRIG_SHAPE = 0x08;

//  Description:   Other Trigger
//  Version:       1.0
const int ID_TRIG_OTHER = 0x10;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagXDTransformerTypeID
//  ==========================================================

//  Description:   Current Transformer
//  Version:       1.0
const int ID_XFORMER_TYPE_CT = 2;

//  Description:   Potential Transformer
//  Version:       1.0
const int ID_XFORMER_TYPE_PT = 1;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagTriggerMethodID
//  ==========================================================

//  Description:   No Trigger Method Defined
//  Version:       1.0
const int ID_TRIGGER_METH_NONE = 0;

//  Description:   A specific channel (or channels) caused the trigger; should be used with tagChannelTriggerIdx to specify which channels.
//  Version:       1.0
const int ID_TRIGGER_METH_CHANNEL = 1;

//  Description:   Periodic Sample
//  Version:       1.0
const int ID_TRIGGER_METH_PERIODIC = 2;

//  Description:   Externally Triggered Measurement
//  Version:       1.0
const int ID_TRIGGER_METH_EXTERNAL = 3;

//  Description:   Periodic Statistical Data
//  Version:       1.5
const int ID_TRIGGER_METH_PERIODIC_STATS = 4;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagQuantityCharacteristicID
//  ==========================================================

//  Description:   Default
//  Version:       1.5
const GUID ID_QC_NONE = { 0xa6b31adf, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Instantaneous f(t)
//  Version:       1.5
const GUID ID_QC_INSTANTANEOUS = { 0xa6b31add, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Spectra F(F)
//  Version:       1.5
const GUID ID_QC_SPECTRA = { 0xa6b31ae9, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Peak value
//  Version:       1.5
const GUID ID_QC_PEAK = { 0xa6b31ae2, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   RMS value
//  Version:       1.5
const GUID ID_QC_RMS = { 0xa6b31ae5, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Harmonic RMS
//  Version:       1.5
const GUID ID_QC_HRMS = { 0xa6b31adc, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Frequency
//  Version:       1.5
const GUID ID_QC_FREQUENCY = { 0x7ef68af, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Positive Peak Value
//  Version:       1.7
const GUID ID_QC_PEAK_POS = { 0xDF3ACF16, 0x1BAF, 0x44D4, { 0x8A, 0xE6, 0x97, 0x06, 0x9D, 0x63, 0x4A, 0x1C } };

//  Description:   Negative Peak Value
//  Version:       1.7
const GUID ID_QC_PEAK_NEG = { 0x74DFB3D2, 0xF7EF, 0x428D, { 0xB9, 0x4C, 0xD5, 0x62, 0x8F, 0x3B, 0x3C, 0xE5 } };

//  Description:   Total harmonic distortion (%)
//  Version:       1.5
const GUID ID_QC_TOTAL_THD = { 0xa6b31aec, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Even harmonic distortion (%)
//  Version:       1.5
const GUID ID_QC_EVEN_THD = { 0xa6b31ad4, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Odd harmonic distortion (%)
//  Version:       1.5
const GUID ID_QC_ODD_THD = { 0xa6b31ae0, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Crest factor
//  Version:       1.5
const GUID ID_QC_CREST_FACTOR = { 0xa6b31ad2, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Form factor
//  Version:       1.5
const GUID ID_QC_FORM_FACTOR = { 0xa6b31adb, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Arithmetic sum
//  Version:       1.5
const GUID ID_QC_ARITH_SUM = { 0xa6b31ad0, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Zero sequence component unbalance (%)
//  Version:       1.5
const GUID ID_QC_S0S1 = { 0xa6b31ae7, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Negative sequence component unbalance (%)
//  Version:       1.5
const GUID ID_QC_S2S1 = { 0xa6b31ae8, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Positive sequence component
//  Version:       1.5
const GUID ID_QC_SPOS = { 0xa6b31aea, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Negative sequence component
//  Version:       1.5
const GUID ID_QC_SNEG = { 0xd71a4b91, 0x3c92, 0x11d4, { 0x9f, 0x2c, 0x0, 0x20, 0x78, 0xe0, 0xb7, 0x23 } };

//  Description:   Zero sequence component
//  Version:       1.5
const GUID ID_QC_SZERO = { 0xd71a4b92, 0x3c92, 0x11d4, { 0x9f, 0x2c, 0x0, 0x20, 0x78, 0xe0, 0xb7, 0x23 } };

//  Description:   Imbalance by max deviation from average
//  Version:       1.5
const GUID ID_QC_AVG_IMBAL = { 0xa6b31ad1, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Total THD normalized to RMS
//  Version:       1.5
const GUID ID_QC_TOTAL_THD_RMS = { 0xf3d216e0, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Odd THD normalized to RMS
//  Version:       1.5
const GUID ID_QC_ODD_THD_RMS = { 0xf3d216e1, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Even THD normalized to RMS
//  Version:       1.5
const GUID ID_QC_EVEN_THD_RMS = { 0xf3d216e2, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Total Interharmonic Distortion
//  Version:       1.5
const GUID ID_QC_TID = { 0xf3d216e3, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Total Interharmonic Distortion Normalized to RMS
//  Version:       1.5
const GUID ID_QC_TID_RMS = { 0xf3d216e4, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Interharmonic RMS
//  Version:       1.5
const GUID ID_QC_IHRMS = { 0xf3d216e5, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Spectra by Harmonic Subgroup Index
//  Version:       1.5
const GUID ID_QC_SPECTRA_HGROUP = { 0x53be6ba8, 0x789, 0x455b, { 0x9a, 0x95, 0xda, 0x12, 0x86, 0x83, 0xdd, 0xa7 } };

//  Description:   Spectra by Interharmonic Subgroup Index
//  Version:       1.5
const GUID ID_QC_SPECTRA_IGROUP = { 0x5e51e006, 0x9c95, 0x4c5e, { 0x87, 0x8f, 0x7c, 0xa8, 0x7c, 0xd, 0x2a, 0xe } };

//  Description:   Duration
//  Version:       1.6
const GUID ID_QC_DURATION = { 0xB3729014, 0x9797, 0x460E, { 0xA0, 0xB5, 0xD8, 0x2D, 0xDB, 0xFB, 0xC2, 0x69 } };

//  Description:   Time
//  Version:       1.6
const GUID ID_QC_TIME_OFFSET = { 0xFD434920, 0x23EC, 0x4948, { 0x8C, 0xB8, 0x50, 0x53, 0xD8, 0x6D, 0x05, 0x06 } };

//  Description:   RMS of Odd Harmonics
//  Version:       1.6
const GUID ID_QC_HRMS_ODD = { 0xEE9A8BAC, 0x0234, 0x4B18, { 0xA7, 0x94, 0x9F, 0xFD, 0x78, 0x18, 0xC6, 0xD3 } };

//  Description:   RMS of Even Harmonics
//  Version:       1.6
const GUID ID_QC_HRMS_EVEN = { 0xE4D86BC6, 0xEF02, 0x491C, { 0xBD, 0xCA, 0x36, 0x05, 0x90, 0x64, 0x24, 0x88 } };

//  Description:   RMS of Triplen Harmonics
//  Version:       1.6
const GUID ID_QC_HRMS_TRIPLEN = { 0xC7E9F4D1, 0x212B, 0x4BB7, { 0x9F, 0x57, 0x4C, 0x7B, 0xF4, 0xF3, 0xCD, 0x30 } };

//  Description:   Harmonic distortion for harmonics that are a multiple of three (%)
//  Version:       1.6
const GUID ID_QC_THD_TRIPLEN = { 0x1ba59bbc, 0xd7ff, 0x4ab3,{ 0xa1, 0x52, 0x44, 0xc1, 0x62, 0xd1, 0xce, 0x07 } };

//  Description:   Q Axis Components
//  Version:       1.5
const GUID ID_QC_QAXIS = { 0xd347ba64, 0xe34c, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   D Axis Components
//  Version:       1.5
const GUID ID_QC_DAXIS = { 0xd347ba63, 0xe34c, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   D Axis Components
//  Version:       1.5
const GUID ID_QC_DAXISFIELD = { 0xd347ba65, 0xe34c, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   Transfer function
//  Version:       1.5
const GUID ID_QC_TRANSFERFUNC = { 0x5202bd07, 0x245c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Status Data
//  Version:       1.5
const GUID ID_QC_STATUS = { 0xb82b5c83, 0x55c7, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Harmonic distortion for harmonics that are a multiple of three (Normalized to RMS)
//  Version:       1.7
const GUID ID_QC_THD_TRIPLEN_RMS = { 0x81AE18F3, 0xFFF2, 0x4BAA, { 0xAC, 0xDB, 0x34, 0x21, 0x36, 0x1F, 0xA2, 0x0C } };

//  Description:   Telephone Interference Factor
//  Version:       1.5
const GUID ID_QC_TIF = { 0xa6b31aeb, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Flicker average RMS value
//  Version:       1.5
const GUID ID_QC_FLKR_MAG_AVG = { 0xa6b31ad6, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   dV/V base
//  Version:       1.5
const GUID ID_QC_FLKR_MAX_DVV = { 0xa6b31ad8, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Frequency of maximum flicker harmonic
//  Version:       1.5
const GUID ID_QC_FLKR_FREQ_MAX = { 0xa6b31ad5, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Magnitude of maximum flicker harmonic
//  Version:       1.5
const GUID ID_QC_FLKR_MAG_MAX = { 0xa6b31ad7, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Spectrum weighted average
//  Version:       1.5
const GUID ID_QC_FLKR_WGT_AVG = { 0xa6b31ada, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Flicker spectrum VRMS(F)
//  Version:       1.5
const GUID ID_QC_FLKR_SPECTRUM = { 0xa6b31ad9, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Short Term Flicker
//  Version:       1.5
const GUID ID_QC_FLKR_PST = { 0x515bf320, 0x71ca, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Long Term Flicker
//  Version:       1.5
const GUID ID_QC_FLKR_PLT = { 0x515bf321, 0x71ca, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   TIF normalized to RMS
//  Version:       1.5
const GUID ID_QC_TIF_RMS = { 0xf3d216e6, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Sliding PLT
//  Version:       1.5
const GUID ID_QC_FLKR_PLTSLIDE = { 0x2257ec05, 0x6ea, 0x4709, { 0xb4, 0x3a, 0xc, 0x0, 0x53, 0x4d, 0x55, 0x4a } };

//  Description:   Flicker Perception Intermediate Calculation (Low Pass Filter)
//  Version:       1.5
const GUID ID_QC_FLKR_PILPF = { 0x4d693eec, 0x5d1d, 0x4531, { 0x99, 0x3a, 0x79, 0x3b, 0x53, 0x56, 0xc6, 0x3d } };

//  Description:   Flicker Perception Intermediate Calculation (Max)
//  Version:       1.5
const GUID ID_QC_FLKR_PIMAX = { 0x126de61c, 0x6691, 0x4d16, { 0x8f, 0xdf, 0x46, 0x48, 0x2b, 0xca, 0x46, 0x94 } };

//  Description:   Flicker Perception Intermediate Calculation (Square Root)
//  Version:       1.5
const GUID ID_QC_FLKR_PIROOT = { 0xe065b621, 0xffdb, 0x4598, { 0x93, 0x30, 0x4d, 0x9, 0x35, 0x39, 0x88, 0xb6 } };

//  Description:   Flicker Perception Intermediate Calculation (Square Root - Low Pass Filter)
//  Version:       1.6
const GUID ID_QC_FLKR_PIROOTLPF = { 0x7d11f283, 0x1ce7, 0x4e58, { 0x8a, 0xf0, 0x79, 0x4, 0x87, 0x93, 0xb8, 0xa7 } };

//  Description:   Instantaneous Flicker Perception
//  Version:       1.6
const GUID ID_QC_FLKR_PINST = { 0x12DB02CD, 0xE2BB, 0x4BE8, { 0xA6, 0xFB, 0x77, 0xE2, 0xDA, 0xED, 0xF8, 0x65 } };

//  Description:   The maximum absolute difference between any of the Urms(1/2) values during the RVC event and the final arithmetic mean 100/120 Urms(1/2) value just prior to the RVC event
//  Version:       1.6
const GUID ID_QC_RVC_DELTA_UMAX = { 0xc8a86eca, 0xcd40, 0x4bee, { 0x9f, 0x1b, 0x9e, 0x7a, 0x8d, 0x6a, 0xb3, 0x4c } };

//  Description:   The absolute difference between the final arithmetic mean 100/120 Urms(1/2) value just prior to the RVC event and the first arithmetic mean 100/120 Urms(1/2) value after the RVC event
//  Version:       1.6
const GUID ID_QC_RVC_DELTA_USS = { 0x9789ceb3, 0x8314, 0x41ff,{ 0x85, 0x65, 0xb9, 0x6c, 0xec, 0xd6, 0xef, 0x48 } };

//  Description:   Current-Time Product
//  Version:       1.5
const GUID ID_QC_IT = { 0xa6b31ade, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   RMS value of current for a demand interval
//  Version:       1.5
const GUID ID_QC_RMS_DEMAND = { 0x7ef68a0, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Transformer Derating Factor
//  Version:       1.5
const GUID ID_QC_ANSI_TDF = { 0x8786ca10, 0x9113, 0x11d3, { 0xb9, 0x30, 0x0, 0x50, 0xda, 0x2b, 0x1f, 0x4d } };

//  Description:   Transformer K Factor
//  Version:       1.5
const GUID ID_QC_K_FACTOR = { 0x8786ca11, 0x9113, 0x11d3, { 0xb9, 0x30, 0x0, 0x50, 0xda, 0x2b, 0x1f, 0x4d } };

//  Description:   Total Demand Distortion
//  Version:       1.5
const GUID ID_QC_TDD = { 0xf3d216e7, 0x2aa5, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Peak Demand Current
//  Version:       1.5
const GUID ID_QC_RMS_PEAK_DEMAND = { 0x72e82a44, 0x336c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Real power (watts)
//  Version:       1.5
const GUID ID_QC_P = { 0xa6b31ae1, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Reactive power (VAR)
//  Version:       1.5
const GUID ID_QC_Q = { 0xa6b31ae4, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Apparent power (VA)
//  Version:       1.5
const GUID ID_QC_S = { 0xa6b31ae6, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   True Power Factor - (Vrms * Irms) / P
//  Version:       1.5
const GUID ID_QC_PF = { 0xa6b31ae3, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Displacement Factor - Cosine of the phase angle between fundamental frequency voltage and current phasors.
//  Version:       1.5
const GUID ID_QC_DF = { 0xa6b31ad3, 0xb451, 0x11d1, { 0xae, 0x17, 0x0, 0x60, 0x8, 0x3a, 0x26, 0x28 } };

//  Description:   Value of active power for a demand interval
//  Version:       1.5
const GUID ID_QC_P_DEMAND = { 0x7ef68a1, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of reactive power for a demand interval
//  Version:       1.5
const GUID ID_QC_Q_DEMAND = { 0x7ef68a2, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of apparent power for a demand interval
//  Version:       1.5
const GUID ID_QC_S_DEMAND = { 0x7ef68a3, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of displacement power factor for a demand interval
//  Version:       1.5
const GUID ID_QC_DF_DEMAND = { 0x7ef68a4, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of true power factor for a demand interval
//  Version:       1.5
const GUID ID_QC_PF_DEMAND = { 0x7ef68a5, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Predicted value of active power for current demand interval
//  Version:       1.5
const GUID ID_QC_P_PRED_DEMAND = { 0x672d0305, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Predicted value of reactive power for current demand interval
//  Version:       1.5
const GUID ID_QC_Q_PRED_DEMAND = { 0x672d0306, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Predicted value of apparent power for current demand interval
//  Version:       1.5
const GUID ID_QC_S_PRED_DEMAND = { 0x672d0307, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active power coincident with reactive power demand
//  Version:       1.5
const GUID ID_QC_P_CO_Q_DEMAND = { 0x672d030a, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active power coincident with apparent power demand
//  Version:       1.5
const GUID ID_QC_P_CO_S_DEMAND = { 0x672d030b, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of reactive power coincident with active power demand
//  Version:       1.5
const GUID ID_QC_Q_CO_P_DEMAND = { 0x672d030d, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of reactive power coincident with apparent power demand
//  Version:       1.5
const GUID ID_QC_Q_CO_S_DEMAND = { 0x672d030e, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of displacement power factor coincident with apparent power demand
//  Version:       1.5
const GUID ID_QC_DF_CO_S_DEMAND = { 0x7ef68ad, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of true power factor coincident with apparent power demand
//  Version:       1.5
const GUID ID_QC_PF_CO_S_DEMAND = { 0x7ef68ae, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of true power factor coincident with active power demand
//  Version:       1.5
const GUID ID_QC_PF_CO_P_DEMAND = { 0x672d0308, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of true power factor coincident with reactive power demand
//  Version:       1.5
const GUID ID_QC_PF_CO_Q_DEMAND = { 0x672d0309, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of the power angle at fundamental
//  Version:       1.5
const GUID ID_QC_ANGLE_FUND = { 0x672d030f, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of the reactive power at fundamental frequency
//  Version:       1.5
const GUID ID_QC_Q_FUND = { 0x672d0310, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   True Power Factor - IEEE vector calculations
//  Version:       1.5
const GUID ID_QC_PF_VECTOR = { 0x672d0311, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Displacement Factor - IEEE vector calculations
//  Version:       1.5
const GUID ID_QC_DF_VECTOR = { 0x672d0312, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of apparent power - IEEE vector calculations
//  Version:       1.5
const GUID ID_QC_S_VECTOR = { 0x672d0314, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fundamental frequency apparent power - IEEE vector calculations
//  Version:       1.5
const GUID ID_QC_S_VECTOR_FUND = { 0x672d0315, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fundamental frequency apparent power
//  Version:       1.5
const GUID ID_QC_S_FUND = { 0x672d0316, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Apparent power coincident with active power demand
//  Version:       1.5
const GUID ID_QC_S_CO_P_DEMAND = { 0x672d0317, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Apparent power coincident with reactive power demand
//  Version:       1.5
const GUID ID_QC_S_CO_Q_DEMAND = { 0x672d0318, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   True Power Factor - IEEE arithmetic calculations
//  Version:       1.5
const GUID ID_QC_PF_ARITH = { 0x1c39fb00, 0xa6aa, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Displacement Factor - IEEE Arithmetic calculations
//  Version:       1.5
const GUID ID_QC_DF_ARITH = { 0x1c39fb01, 0xa6aa, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of apparent power - IEEE Arithmetic calculations
//  Version:       1.5
const GUID ID_QC_S_ARITH = { 0x1c39fb02, 0xa6aa, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fundamental frequency apparent power - IEEE Arithmetic calculations
//  Version:       1.5
const GUID ID_QC_S_ARITH_FUND = { 0x1c39fb03, 0xa6aa, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Peak Apparent Power Demand
//  Version:       1.5
const GUID ID_QC_S_PEAK_DEMAND = { 0x72e82a43, 0x336c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Peak Reactive Power Demand
//  Version:       1.5
const GUID ID_QC_Q_PEAK_DEMAND = { 0x72e82a42, 0x336c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Peak Active Power Demand
//  Version:       1.5
const GUID ID_QC_P_PEAK_DEMAND = { 0x72e82a41, 0x336c, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Net Harmonic Active Power
//  Version:       1.5
const GUID ID_QC_P_HARMONIC = { 0xb82b5c80, 0x55c7, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Arithmetic sum Harmonic Active Power
//  Version:       1.5
const GUID ID_QC_P_HARMONIC_UNSIGNED = { 0xb82b5c81, 0x55c7, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fundamental frequency real power
//  Version:       1.5
const GUID ID_QC_P_FUND = { 0x1cdda475, 0x1ebb, 0x42d8, { 0x80, 0x87, 0xd0, 0x1b, 0xb, 0x5c, 0xfa, 0x97 } };

//  Description:   Value of active power integrated over time (Watt-hours)
//  Version:       1.5
const GUID ID_QC_P_INTG = { 0x7ef68a6, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of active power integrated over time (watt-hours) in the positive direction (toward load).
//  Version:       1.5
const GUID ID_QC_P_INTG_POS = { 0x7ef68a7, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of active fundamental frequency power integrated over time (Energy - watt-hours) in the positive direction (toward load)
//  Version:       1.5
const GUID ID_QC_P_INTG_POS_FUND = { 0x672d0300, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active power integrated over time (watt-hours) in the negative direction (away from load).
//  Version:       1.5
const GUID ID_QC_P_INTG_NEG = { 0x7ef68a8, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of active fundamental frequency  power integrated over time (Energy - watt-hours) in the negative direction (away from load)
//  Version:       1.5
const GUID ID_QC_P_INTG_NEG_FUND = { 0x672d0301, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of reactive power integrated over time (var-hours)
//  Version:       1.5
const GUID ID_QC_Q_INTG = { 0x7ef68a9, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of reactive power integrated over time (watt-hours) in the positive direction (toward load)
//  Version:       1.5
const GUID ID_QC_Q_INTG_POS = { 0x7ef68aa, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of fundamental frequency reactive power integrated over time (Energy - watt-hours) in the positive direction (toward load)
//  Version:       1.5
const GUID ID_QC_Q_INTG_POS_FUND = { 0x672d0303, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fundamental frequency reactive power integrated over time (Energy - watt-hours) in the negative direction (away from load)
//  Version:       1.5
const GUID ID_QC_Q_INTG_NEG_FUND = { 0x672d0304, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of reactive power integrated over time (watt-hours) in the negative direction (away from load)
//  Version:       1.5
const GUID ID_QC_Q_INTG_NEG = { 0x7ef68ab, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of apparent power integrated over time (VA-hours)
//  Version:       1.5
const GUID ID_QC_S_INTG = { 0x7ef68ac, 0x9ff5, 0x11d2, { 0xb3, 0xb, 0x0, 0x60, 0x8, 0xb3, 0x71, 0x83 } };

//  Description:   Value of fundamental frequency apparent power integrated over time (VA-hours)
//  Version:       1.5
const GUID ID_QC_S_INTG_FUND = { 0x672d0313, 0x7810, 0x11d4, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active power integrated over time (Energy - watt-hours)
//  Version:       1.5
const GUID ID_QC_P_IVL_INTG = { 0xf098a9a0, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active power integrated over time (Energy - watt-hours) in the positive direction (toward load)
//  Version:       1.5
const GUID ID_QC_P_IVL_INTG_POS = { 0xf098a9a1, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active fund. Freq. power integrated over time (Energy - watt-hours) in the positive direction (toward load)
//  Version:       1.5
const GUID ID_QC_P_IVL_INTG_POS_FUND = { 0xf098a9a2, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active power integrated over time (Energy - watt-hours) in the negative direction (away from load)
//  Version:       1.5
const GUID ID_QC_P_IVL_INTG_NEG = { 0xf098a9a3, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of active fund. Freq.  power integrated over time (Energy - watt-hours) in the negative direction (away from load)
//  Version:       1.5
const GUID ID_QC_P_IVL_INTG_NEG_FUND = { 0xf098a9a4, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of reactive power integrated over time (var-hours)
//  Version:       1.5
const GUID ID_QC_Q_IVL_INTG = { 0xf098a9a5, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of reactive power integrated over time (Energy - watt-hours) in the positive direction (toward load)
//  Version:       1.5
const GUID ID_QC_Q_IVL_INTG_POS = { 0xf098a9a6, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fund. Freq. reactive power integrated over time (Energy - watt-hours) in the positive direction (toward load)
//  Version:       1.5
const GUID ID_QC_Q_IVL_INTG_POS_FUND = { 0xf098a9a7, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fund. Freq. reactive power integrated over time (Energy - watt-hours) in the negative direction (away from load)
//  Version:       1.5
const GUID ID_QC_Q_IVL_INTG_NEG_FUND = { 0xf098a9a8, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of reactive power integrated over time (Energy - watt-hours) in the negative direction (away from load)
//  Version:       1.5
const GUID ID_QC_Q_IVL_INTG_NEG = { 0xf098a9a9, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of apparent power integrated over time (VA-hours)
//  Version:       1.5
const GUID ID_QC_S_IVL_INTG = { 0xf098a9aa, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Value of fundamental frequency apparent power integrated over time (VA-hours)
//  Version:       1.5
const GUID ID_QC_S_IVL_INTG_FUND = { 0xf098a9ab, 0x3ee4, 0x11d5, { 0xa4, 0xb3, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

//  Description:   Linear Position
//  Version:       1.5
const GUID ID_QC_LINEAR = { 0xd347ba61, 0xe34c, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   Rate of change of position with respect to a frame of reference
//  Version:       1.6
const GUID ID_QC_VELOCITY = { 0x039A8FDE, 0x9406, 0x4BF5, { 0xB7, 0xB6, 0xAF, 0x11, 0x63, 0x6D, 0x39, 0x9F } };

//  Description:   Rate of change of velocity
//  Version:       1.6
const GUID ID_QC_ACCELERATION = { 0x5000C15A, 0x2E65, 0x4C86, { 0x91, 0x9E, 0x34, 0x3C, 0x5F, 0xAA, 0x06, 0x4C } };

//  Description:   Rate of change of acceleration
//  Version:       1.6
const GUID ID_QC_JERK = { 0x6042D1E3, 0xEAD6, 0x488F, { 0xAC, 0x54, 0xFC, 0x1E, 0x1D, 0x85, 0xFD, 0xF9 } };

//  Description:   Rotational Position
//  Version:       1.5
const GUID ID_QC_ROTATIONAL = { 0xd347ba62, 0xe34c, 0x11d4, { 0x82, 0xd9, 0x0, 0xe0, 0x98, 0x72, 0xa0, 0x94 } };

//  Description:   Temperature
//  Version:       1.6
const GUID ID_QC_TEMPERATURE = { 0x7EE799FC, 0x19E5, 0x4045, { 0x80, 0xBB, 0xA4, 0x49, 0xB6, 0x1D, 0xC9, 0x03 } };

//  Description:   Relative Humidity, or the ratio of the partial pressure of water vapor in the air–water mixture to the saturated vapor pressure of water at those conditions
//  Version:       1.6
const GUID ID_QC_REL_HUMIDITY = { 0x361165bc, 0x9b4c, 0x4e11,{ 0x8e, 0xe8, 0x89, 0x7d, 0xc5, 0x41, 0xe6, 0x6d } };

//  Description:   Electrical Resistance (R)
//  Version:       1.6
const GUID ID_QC_RESISTANCE = { 0x1C255132, 0xFABA, 0x44A7, { 0x9C, 0xAE, 0x8C, 0xE8, 0x58, 0x51, 0xB7, 0x3E } };

//  Description:   Electrical Reactance (X)
//  Version:       1.6
const GUID ID_QC_REACTANCE = { 0x8627805F, 0xCB7B, 0x4841, { 0x94, 0x23, 0x7C, 0x6E, 0x3D, 0xB4, 0xAD, 0x49 } };

//  Description:   Electrical Impedance (Z)
//  Version:       1.6
const GUID ID_QC_IMPEDANCE = { 0x4C19F9F2, 0xE297, 0x4FF5, { 0x82, 0x5F, 0x91, 0xE5, 0x98, 0xF9, 0x28, 0x56 } };

//  Description:   Electrical Conductance (G)
//  Version:       1.6
const GUID ID_QC_CONDUCTANCE = { 0xAEDE9D60, 0x591D, 0x43C6, { 0x9D, 0x18, 0xFA, 0x34, 0xE0, 0x8C, 0xD6, 0x3F } };

//  Description:   Electrical Susceptance (B)
//  Version:       1.6
const GUID ID_QC_SUSCEPTANCE = { 0x9DF9C55A, 0xCC24, 0x4DCE, { 0x8F, 0x9E, 0x9E, 0xF9, 0xE9, 0x1C, 0x95, 0xC4 } };

//  Description:   Electrical Admittance (Y)
//  Version:       1.6
const GUID ID_QC_ADMITTANCE = { 0x74E51E63, 0xAC9B, 0x43F9, { 0x85, 0x99, 0x1E, 0x67, 0x60, 0x35, 0xB1, 0x4B } };

//  Description:   Positive-Sequence Electrical Resistance (R1)
//  Version:       1.6
const GUID ID_QC_RESISTANCE_SPOS = { 0x01923143, 0x3E31, 0x4BE5, { 0x9B, 0x54, 0x92, 0x10, 0x9C, 0x38, 0x8A, 0x9B } };

//  Description:   Positive-Sequence Electrical Reactance (X1)
//  Version:       1.6
const GUID ID_QC_REACTANCE_SPOS = { 0x24A43F5B, 0xA6B6, 0x456E, { 0xBE, 0x47, 0xA7, 0x12, 0x44, 0xBC, 0xD7, 0xBC } };

//  Description:   Positive-Sequence Electrical Impedance (Z1)
//  Version:       1.6
const GUID ID_QC_IMPEDANCE_SPOS = { 0x27233A3B, 0x5A26, 0x4341, { 0xB0, 0x98, 0x15, 0x41, 0x67, 0x54, 0xD1, 0x4E } };

//  Description:   Positive-Sequence Electrical Conductance (G1)
//  Version:       1.6
const GUID ID_QC_CONDUCTANCE_SPOS = { 0x02582B6C, 0xA2BB, 0x432A, { 0x99, 0xEB, 0xC9, 0xBB, 0xA8, 0xED, 0xA5, 0xED } };

//  Description:   Positive-Sequence Electrical Susceptance (B1)
//  Version:       1.6
const GUID ID_QC_SUSCEPTANCE_SPOS = { 0x265EAAB1, 0xBC47, 0x4471, { 0x92, 0x0B, 0x8B, 0x91, 0xE1, 0x93, 0x86, 0xF4 } };

//  Description:   Positive-Sequence Electrical Admittance (Y1)
//  Version:       1.6
const GUID ID_QC_ADMITTANCE_SPOS = { 0xCDBFD610, 0x8B7C, 0x4099, { 0x94, 0x93, 0x96, 0x23, 0x79, 0x3D, 0x0A, 0xE8 } };

//  Description:   Negative-Sequence Electrical Resistance (R2)
//  Version:       1.6
const GUID ID_QC_RESISTANCE_SNEG = { 0x647DBCDC, 0x4881, 0x4539, { 0xA5, 0x6B, 0xFC, 0x3D, 0x00, 0x0E, 0xC6, 0x6C } };

//  Description:   Negative-Sequence Electrical Reactance (X2)
//  Version:       1.6
const GUID ID_QC_REACTANCE_SNEG = { 0x90091312, 0x6E6B, 0x4443, { 0xA2, 0xB6, 0xD4, 0x03, 0x51, 0xB0, 0x0B, 0xA1 } };

//  Description:   Negative-Sequence Electrical Impedance (Z2)
//  Version:       1.6
const GUID ID_QC_IMPEDANCE_SNEG = { 0x30BBE94B, 0xAA7F, 0x4C39, { 0x95, 0x7A, 0xB8, 0x83, 0x77, 0xFE, 0xE3, 0x80 } };

//  Description:   Negative-Sequence Electrical Conductance (G2)
//  Version:       1.6
const GUID ID_QC_CONDUCTANCE_SNEG = { 0xDC9392D3, 0xEDC8, 0x40E1, { 0xA8, 0xE5, 0x1E, 0xB2, 0x69, 0x33, 0x59, 0x45 } };

//  Description:   Negative-Sequence Electrical Susceptance (B2)
//  Version:       1.6
const GUID ID_QC_SUSCEPTANCE_SNEG = { 0x339B8AF9, 0xF120, 0x4331, { 0xB9, 0xD4, 0xAB, 0xCA, 0x9C, 0xBE, 0xAD, 0x87 } };

//  Description:   Negative-Sequence Electrical Admittance (Y2)
//  Version:       1.6
const GUID ID_QC_ADMITTANCE_SNEG = { 0x43A6B1FC, 0x8EF9, 0x482C, { 0xA0, 0xAD, 0x13, 0x1B, 0xB4, 0x80, 0x8F, 0xA2 } };

//  Description:   Zero-Sequence Electrical Resistance (R0)
//  Version:       1.6
const GUID ID_QC_RESISTANCE_SZERO = { 0xE79A2084, 0x5723, 0x4625, { 0xBC, 0x45, 0xC2, 0x22, 0x29, 0xEC, 0x69, 0xEE } };

//  Description:   Zero-Sequence Electrical Reactance (X0)
//  Version:       1.6
const GUID ID_QC_REACTANCE_SZERO = { 0x2EACA452, 0x59E4, 0x43FC, { 0xA2, 0xD8, 0x62, 0x6D, 0x2F, 0x25, 0x0D, 0x55 } };

//  Description:   Zero-Sequence Electrical Impedance (Z0)
//  Version:       1.6
const GUID ID_QC_IMPEDANCE_SZERO = { 0xBA214F62, 0x0390, 0x48C3, { 0x88, 0xEA, 0x44, 0x9E, 0xC4, 0x18, 0x2B, 0xBC } };

//  Description:   Zero-Sequence Electrical Conductance (G0)
//  Version:       1.6
const GUID ID_QC_CONDUCTANCE_SZERO = { 0x0C2A7E66, 0xA4EF, 0x4ECC, { 0x89, 0x95, 0xDC, 0x65, 0x34, 0xC5, 0x68, 0x3D } };

//  Description:   Zero-Sequence Electrical Susceptance (B0)
//  Version:       1.6
const GUID ID_QC_SUSCEPTANCE_SZERO = { 0x52E726E4, 0xE5A7, 0x4B97, { 0x89, 0x77, 0xD1, 0x83, 0xC7, 0x7E, 0x23, 0x75 } };

//  Description:   Zero-Sequence Electrical Admittance (Y0)
//  Version:       1.6
const GUID ID_QC_ADMITTANCE_SZERO = { 0x791B9905, 0xA4F2, 0x482D, { 0xA2, 0xB6, 0x13, 0x6E, 0x21, 0xB7, 0xCA, 0x0E } };

//  Description:   The power per unit area produced by the Sun in the form of electromagnetic radiation
//  Version:       1.6
const GUID ID_QC_SOLAR_IRRADIANCE = { 0x78014E8C, 0xE35D, 0x4E87, { 0xA9, 0x15, 0xF3, 0xFB, 0x68, 0xA4, 0xCF, 0x8E } };

//  Description:   Time series of flagged intervals
//  Version:       1.6
const GUID ID_QC_FLAGGING = { 0x2A310A59, 0x3CF5, 0x4BC6, { 0xB1, 0xB4, 0x26, 0x02, 0x00, 0x5C, 0x00, 0xC2 } };

//  Description:   Time series of intervals flagged by procedures identified in IEC 61000-4-30
//  Version:       1.6
const GUID ID_QC_FLAGGING_IEC_61000_4_30 = { 0x5FD423AA, 0xDE44, 0x4A13, { 0xBF, 0x5B, 0xAF, 0x96, 0x21, 0xBC, 0x6F, 0xFA } };

//  Description:   Sound Absorption (A)
//  Version:       1.6
const GUID ID_QC_SOUND_ABSORPTION = { 0x91600A0D, 0xE2E8, 0x43EA, { 0x9E, 0x90, 0xCA, 0x71, 0x47, 0x56, 0xE1, 0x72 } };

//  Description:   Perpendicular force per unit area
//  Version:       1.7
const GUID ID_QC_PRESSURE = { 0x424D919A, 0xB262, 0x44D6, { 0x82, 0x05, 0x93, 0x6E, 0xAD, 0x3C, 0x05, 0x12 } };

//  Description:   Static pressure exerted by the weight of the atmosphere
//  Version:       1.7
const GUID ID_QC_PRESSURE_ATMOSPHERIC = { 0xEDA84323, 0x835D, 0x4124, { 0xB3, 0xCB, 0x98, 0x74, 0xC8, 0xC6, 0xBF, 0x99 } };

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagQuantityMeasuredID
//  ==========================================================

//  Description:   None or not applicable
//  Version:       1.5
const int ID_QM_NONE = 0;

//  Description:   Humidity, or the amount of water vapor in the air
//  Version:       1.0
const int ID_QM_HUMIDITY = 18;

//  Description:   Voltage
//  Version:       1.5
const int ID_QM_VOLTAGE = 1;

//  Description:   Current
//  Version:       1.5
const int ID_QM_CURRENT = 2;

//  Description:   Power - includes all data for a quantity or characteristic derived from multiplying voltage and current components.
//  Version:       1.5
const int ID_QM_POWER = 3;

//  Description:   Energy - includes all data from an integration of a quantity or characteristic derived from multiplying voltage and current components together
//  Version:       1.5
const int ID_QM_ENERGY = 4;

//  Description:   Temperature
//  Version:       1.5
const int ID_QM_TEMPERATURE = 5;

//  Description:   Pressure
//  Version:       1.5
const int ID_QM_PRESSURE = 6;

//  Description:   Charge
//  Version:       1.5
const int ID_QM_CHARGE = 7;

//  Description:   Electrical Field
//  Version:       1.5
const int ID_QM_EFIELD = 8;

//  Description:   Magnetic Field
//  Version:       1.5
const int ID_QM_MFIELD = 9;

//  Description:   Velocity
//  Version:       1.5
const int ID_QM_VELOCITY = 10;

//  Description:   Compass Bearing
//  Version:       1.5
const int ID_QM_BEARING = 11;

//  Description:   Applied Force, Electrical, Mechanical, etc.
//  Version:       1.5
const int ID_QM_FORCE = 12;

//  Description:   Torque
//  Version:       1.5
const int ID_QM_TORQUE = 13;

//  Description:   Spatial Position
//  Version:       1.5
const int ID_QM_POSITION = 14;

//  Description:   Flux linkage Weber Turns
//  Version:       1.5
const int ID_QM_FLUXLINKAGE = 15;

//  Description:   Magnetic field Density
//  Version:       1.5
const int ID_QM_FLUXDENSITY = 16;

//  Description:   Status Data
//  Version:       1.5
const int ID_QM_STATUS = 17;

//  Description:   Impedance
//  Version:       1.7
const int ID_QM_IMPEDANCE = 19;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagChanTriggerTypeID
//  ==========================================================

//  Description:   No transition - should not happen
//  Version:       1.5
const int ID_CTT_NONE = 0;

//  Description:   Normal to Low transition
//  Version:       1.5
const int ID_CTT_NORMAL_TO_LO = 1;

//  Description:   Normal to Low-Low transition
//  Version:       1.5
const int ID_CTT_NORMAL_TO_LO_LO = 2;

//  Description:   Normal to High transition
//  Version:       1.5
const int ID_CTT_NORMAL_TO_HI = 3;

//  Description:   Normal to High-High transition
//  Version:       1.5
const int ID_CTT_NORMAL_TO_HI_HI = 4;

//  Description:   Low-Low to Low transition
//  Version:       1.5
const int ID_CTT_LO_LO_TO_LO = 5;

//  Description:   Low-Low to Normal transition
//  Version:       1.5
const int ID_CTT_LO_LO_TO_NORMAL = 6;

//  Description:   Low-Low to High transition
//  Version:       1.5
const int ID_CTT_LO_LO_TO_HI = 7;

//  Description:   Low-Low to High-High transition
//  Version:       1.5
const int ID_CTT_LO_LO_TO_HI_HI = 8;

//  Description:   Low to Low-Low transition
//  Version:       1.5
const int ID_CTT_LO_TO_LO_LO = 9;

//  Description:   Low to Normal transition
//  Version:       1.5
const int ID_CTT_LO_TO_NORMAL = 10;

//  Description:   Low to High transition
//  Version:       1.5
const int ID_CTT_LO_TO_HI = 11;

//  Description:   Low to High-High transition
//  Version:       1.5
const int ID_CTT_LO_TO_HI_HI = 12;

//  Description:   High to Low-Low transition
//  Version:       1.5
const int ID_CTT_HI_TO_LO_LO = 13;

//  Description:   High to Low transition
//  Version:       1.5
const int ID_CTT_HI_TO_LO = 14;

//  Description:   High to Normal transition
//  Version:       1.5
const int ID_CTT_HI_TO_NORMAL = 15;

//  Description:   High to High-High transition
//  Version:       1.5
const int ID_CTT_HI_TO_HI_HI = 16;

//  Description:   High-High to Low-Low transition
//  Version:       1.5
const int ID_CTT_HI_HI_TO_LO_LO = 17;

//  Description:   High-High to Low transition
//  Version:       1.5
const int ID_CTT_HI_HI_TO_LO = 18;

//  Description:   High-High to Normal transition
//  Version:       1.5
const int ID_CTT_HI_HI_TO_NORMAL = 19;

//  Description:   High-High to High transition
//  Version:       1.5
const int ID_CTT_HI_HI_TO_HI = 20;

//  Description:   Deadband transition lower
//  Version:       1.5
const int ID_CTT_DB_LO = 21;

//  Description:   Deadband transition higher
//  Version:       1.5
const int ID_CTT_DB_HI = 22;

//  Description:   Hardware initiated trigger based on periodic trigger rule
//  Version:       1.5
const int ID_CTT_PERIODIC = 23;

//  Description:   User commanded sample - button was pushed
//  Version:       1.5
const int ID_CTT_MANUAL = 24;

//  Description:   Channel triggered because of internal cross-trigger rule.  tagCrossTriggerChanIdx is index of channel that triggered.
//  Version:       1.5
const int ID_CTT_INT_CROSS_TRIG = 25;

//  Description:   Channel triggered because of external cross-trigger rule.  tagCrossTriggerChanIdx is index of channel that triggered on external device.  tagCrossTriggerDeviceName is the name of the external device that initiated the cross trigger.
//  Version:       1.5
const int ID_CTT_EXT_CROSS_TRIG = 26;

//  Description:   Channel triggered because of hardware or software module, rule, or algorithm
//  Version:       1.5
const int ID_CTT_MODULE = 27;

//  Description:   Rate of change threshold exceeded (dV/dt or dI/dt)
//  Version:       1.5
const int ID_CTT_RATE = 28;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagSettingPhysicalConnection
//  ==========================================================

//  Description:   Single-Phase Connection with 1 Voltage and 1 Current
//  Version:       1.5
const int ID_SINGLE_PHASE = 1;

//  Description:   Delta Connection with 2 Element Monitoring
//  Version:       1.5
const int ID_2ELEMENT_DELTA = 2;

//  Description:   Wye Connection with 2 Voltages and 3 Currents
//  Version:       1.5
const int ID_2_5ELEMENT_WYE = 3;

//  Description:   Wye Connection with 3 Voltages and 3 Currents
//  Version:       1.5
const int ID_3ELMENT_WYE = 4;

//  Description:   Delta Connection with 3 Voltages and 3 Currents
//  Version:       1.5
const int ID_3ELEMENT_DELTA = 5;

//  Description:   Split-Phase Connection with 2 Voltage and 2 Current
//  Version:       1.5
const int ID_SPLIT_PHASE = 6;

//  Description:   2 Phase, 2 Voltages, 2 Currents
//  Version:       1.5
const int ID_2ELEMENT_2PHASE = 7;

//  ==========================================================
//  The following IDs should be used with the tag named
//  tagSagTypeID
//  ==========================================================

//  Description:   None
//  Version:       1.7
const GUID ID_SAGTYPE_NONE = { 0x3D7B7C11, 0x80F0, 0x47C5, { 0x86, 0x97, 0xC4, 0x87, 0x8B, 0x5F, 0xC9, 0x54 } };

//  Description:   IEEE 1668 Sag Type I
//  Version:       1.7
const GUID ID_SAGTYPE_1668_I = { 0X2E542339, 0X8A4B, 0X4C9A, { 0XAF, 0XA9, 0XCC, 0X60, 0X55, 0X37, 0X95, 0X98 } };

//  Description:   IEEE 1668 Sag Type II
//  Version:       1.7
const GUID ID_SAGTYPE_1668_II = { 0X648C3F37, 0XF46F, 0X4974, { 0XA2, 0XFA, 0XB6, 0X6F, 0X4F, 0X22, 0X3B, 0XF3 } };

//  Description:   IEEE 1668 Sag Type III
//  Version:       1.7
const GUID ID_SAGTYPE_1668_III = { 0XDC1591D9, 0X3975, 0X45BA, { 0X99, 0XE8, 0XDA, 0XC5, 0XC1, 0X00, 0XA3, 0XEB } };

//  Description:   The observation event includes one or more sag types (e.g., a Sag Type I event that evolves into Sag Type II).
//  Version:       1.7
const GUID ID_SAGTYPE_EVOLVING = { 0x9F7261A4, 0x1BBF, 0x4F5F, { 0x92, 0xB5, 0xE7, 0xBF, 0x29, 0x45, 0x0E, 0x06 } };



#endif  //  PQDIF_ID_H


