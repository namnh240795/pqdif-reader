## PQDIF.Validator

This project provides a class for reading and validating PQDIF files. Error messages and PQDIF compliance messages are logged using PQDIF.Log objects. The contents of a PQDIF file are stored in a list of PQDIF.Validator.RecordHolder objects, which can be used to represent the Container, DataSource, MonitorSettings, and Observation records of a PQDIF file.

## Example Usage

```c#
/// <summary>
/// Demonstrates using PQDIF.Validator to read and validate the contents of a PQDIF file 
and load its contents into a list of PQDIF.Validator.RecordHolder objects.
/// </summary>
private void DemoPQDifValidatorFileRead()
{
    // Create a new PQDIF Validator
    PQDIF.Validator.File PQDifValidatorFile = new PQDIF.Validator.File();

    // Get XML documents summarizing parsing success.
    string MessagesApplication = PQDifValidatorFile.GetXML(true, false);
    string MessagesCompliance = PQDifValidatorFile.GetXML(false, true);

    // If we can read the PQDIF file...
    if (PQDifValidatorFile.Read(@"c:\files\pqdif\MyFile.pqd"))
    {

        // Compute statistics on the records read.
        int CountContainers = 0;
        int CountDataSources = 0;
        int CountChannelDefinitions = 0;
        int CountSeriesDefinitions = 0;
        int CountMonitorSettings = 0;
        int CountObservationRecords = 0;
        int CountChannelInstances = 0;
        int CountSeriesInstances = 0;
        int CountSeriesValues = 0;

        // For each object...
        foreach (PQDIF.Validator.RecordHolder ThisRecordHolder in m_PQDifValidatorFile.RecordHolders)
        {
            // If this is a container...
            if (ThisRecordHolder.Record.GetType() == typeof(PQDIF.Validator.Container))
                CountContainers++;

            // If this is a data source record...
            if (ThisRecordHolder.Record.GetType() == typeof(PQDIF.Validator.RecDataSource))
            {
                CountDataSources++;
                PQDIF.Validator.RecDataSource ThisDataSource = (PQDIF.Validator.RecDataSource)ThisRecordHolder.Record;
                foreach (PQDIF.Validator.OneChannelDefn ThisChannelDefn in ThisDataSource.ChannelDefns)
                {
                    CountChannelDefinitions++;
                    foreach (PQDIF.Validator.OneSeriesDefn ThisSeriesDefn in ThisChannelDefn.SeriesDefns)
                        CountSeriesDefinitions++;
                }
            } // If this is a data source record

            // If this is a Monitor Settings Record...
            if (ThisRecordHolder.Record.GetType() == typeof(PQDIF.Validator.RecMonitorSettings))
                CountMonitorSettings++;

            // If this is an observation record...
            if (ThisRecordHolder.Record.GetType() == typeof(PQDIF.Validator.RecObservation))
            {
                CountObservationRecords++;
                PQDIF.Validator.RecObservation ThisObservation = (PQDIF.Validator.RecObservation)ThisRecordHolder.Record;
                foreach (PQDIF.Validator.OneChannelInst ThisChannelInst in ThisObservation.ChannelInstances)
                {
                    CountChannelInstances++;
                    foreach (PQDIF.Validator.OneSeriesInstance ThisSeriesInstance in ThisChannelInst.SeriesInstances)
                    {
                        CountSeriesInstances++;
                        if (ThisSeriesInstance.SeriesValues != null)
                            CountSeriesValues += ThisSeriesInstance.SeriesValues.Length;
                    }
                }
                
            } // If this is an observation record

        } // For each object

    } // If we can read the PQDIF file

} // DemoPQDifValidatorFileRead
```

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
purposes. See the [LICENSE](../LICENSE) file distributed with this work
for copyright and licensing information.


## License

Copyright 2023 PQDIF Authors

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

