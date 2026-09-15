## PQDIFNet

This project provides class libraries for reading and writing PQDIF files using .NET assemblies.

```c#
/// <summary>
/// Reads the PQDIF file specified by <see cref="FileName"/>        
/// </summary>
/// <param name="FileName">File to Read</param>
/// <returns><c>true</c> if the file was read OK.</returns>
public bool PQDifRead(string FileName)
{
    // Declare a new PQDIF file
    CPQDIFNet PqdifFile = null;
    
    try
    {   

        // Initialize a new PQDIF file
        PqdifFile = new CPQDIFNet
        {
            FlatFileName = FileName
        };                

        // If we could read the PQDIF file...
        if (PqdifFile.Read())
        {   

            // Get the number of records in the PQDIF file.
            int RecCount = PqdifFile.RecordGetCount();                    

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

                    // If we can get the get a pointer to the collection...
                    if (PqdifFile.RecordGetCollection(RecIndex, ref PointerCollection))
                    {

                        // If the top-level record is a collection...
                        if (PQDIFNetWrapper.IsElementCollection(PqdifFile, PointerCollection, LoggerApplication))
                        {

                            // If the record is a container record...
                            if (tagRecordGUID == tagContainer)
                            {
                                // Process the container
                            }

                            // Otherwise, if the record is a data source record...
                            else if (tagRecordGUID == tagRecDataSource)
                            {
                                // Process the Data Source record
                            }

                            // Otherwise, if the record is a monitor settings record...
                            else if (tagRecordGUID == tagRecMonitorSettings)
                            {
                                // Process the Monitor Settings record
                            }

                            // Otherwise, if the record is an observation record...
                            else if (tagRecordGUID == tagRecObservation)
                            {
                                // Process the Observation record

                            } // If the record is a container record

                        } // If the top-level record is a collection

                    } // If we can get the get a pointer to the collection

                } // If we can get the record info

            } // For each record

        } // If we could read the PQDIF file
        

        // The PQDIF file was read.
        FileRead = true;


    } // try

    // Log application errors.
    catch (Exception ex)
    {
        // Log errors
    }

    // Run this code on exit.
    finally
    {
        // Clean Up.
        try
        {
            if (PqdifFile != null)
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

} // PQDifRead
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

