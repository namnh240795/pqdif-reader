This project provides software libraries for reading, writing, and viewing **PQDIF** files. Power Quality Data Interchange Format (PQDIF) is a binary file format specified in [IEEE Std. 1159.3-2019](https://standards.ieee.org/standard/1159_3-2019.html) that is used to exchange voltage, current, power, and energy measurements between software applications with a focus on the power quality domain.

PQDIF can be utilized by power quality meters, electric meters, desktop software applications, server software, and web servers. Users of PQDIF files include meter manufacturers, electric utility company engineers, and commercial/industrial consumers of electric power.

PQDIF is defined in a "recommended practice" standard maintained by the [IEEE PQDIF Task Force](http://grouper.ieee.org/groups/1159/3), which is sponsored by the Transmission & Distribution Committee of the [IEEE Power & Energy Society](https://www.ieee-pes.org/).

The file format is designed to represent all power quality phenomena identified in [IEEE Std 1159](https://standards.ieee.org/standard/1159_3-2019.html), which is a recommended practice on monitoring electric power quality. PQDIF can be used to represent other power related measurement data and is extensible to other data types as well. The recommended file format includes optional compression using [zlib](https://zlib.net/) to help reduce disk space and transmission times. The utilization of Globally Unique Identifiers (GUID) to represent each element in the file permits the format to be extensible without the need for a central registration authority.

PQDIF allows storage of the following types of measurements: waveforms, time series value logs (rms voltage, rms current, real/reactive/apparent power, total harmonic distortion, harmonics, flicker, etc.), phasors, frequency spectrums, lightning strikes, histograms, cross-tabulations, and magnitude-duration summary tables for voltage sags, voltage swells, interruptions, transients and rapid voltage changes. PQDIF allows storage of information related to the sources that recorded the data, including name, description, location, transducer settings, trigger settings, and more.

A single PQDIF file is a collection of PQDIF records consisting of a Container record, Data Source record, an optional Monitor Settings record, and one or more Observation records. In contrast, a PQDIF stream is a collection of PQDIF records streamed to a client using communication media such as a network connection. A stream presents records such that they are downloaded in the same order as they would be if read from a PQDIF file.

The specification for PQDIF was first published in [IEEE Std 1159.3-2003](https://standards.ieee.org/standard/1159_3-2003.html), which was reaffirmed in 2009. A second edition was approved and published by IEEE in 2019. The physical structure of PQDIF remains unchanged between IEEE Std 1159.3-2003 and IEEE Std 1159.3-2019, making PQDIF both backward and forwards compatible.

The 2019 edition of IEEE Std 1159.3 includes an annex explaining the relationship of the PQDIF file format with the [IEEE C37.111](https://standards.ieee.org/standard/C37_111-2013.html) COMTRADE file format. Another annex explains how to represent PQDIF files in XML files. The 2019 edition also explains the relationship between PQDIF and IEC 61850.

## Disclaimer


This open source repository contains material that may be included in,
or referenced by, an unapproved draft of a proposed IEEE Standard.
All material in this repository is subject to change. The material in
this repository is presented "as is" and with all faults. Use of the
material is at the sole risk of the user. IEEE specifically disclaims
all warranties and representations with respect to all material
contained in this repository and shall not be liable, under any
theory, for any use of the material. Unapproved drafts of proposed
IEEE standards must not be utilized for any conformance/compliance
purposes. See the [LICENSE](LICENSE) file distributed with this work
for copyright and licensing information.


## License

Copyright 2021 PQDIF Authors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

<http://www.apache.org/licenses/LICENSE-2.0>

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

See the LICENSE file distributed with this work for copyright and
licensing information, the AUTHORS file for a list of copyright
holders, and the CONTRIBUTORS file for the list of contributors.

SPDX-License-Identifier: Apache-2.0

# pqdif-reader
