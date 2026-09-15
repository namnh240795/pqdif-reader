/*
**  Class:          PQDIF.Validator.CustomSourceInfo
**  Description:	One of these collections per channel
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


// Contains fundamental classes and base classes that define commonly-used value and reference data types, events and event handlers, interfaces, attributes, and processing exceptions.
using System;

// Defines IEEE PQDIF data types including PQDIF timestamps
using PQDIFNet;

// Defines IEEE PQDIF IDs and data types 
using static PQDIFNet.Constants.Logical;

// For logging errors, warnings, other messages.
using PQDIF.Log;


namespace PQDIF.Validator
{

    /// <summary>
    /// This collection can include the standard name, address and telephone number tags. These apply to the vendor as well as tags about the instrument itself
    /// </summary>
    public class CustomSourceInfo
    {
        /// <summary>
        /// The type of instrument
        /// </summary>
        public Guid InstrumentTypeID = ID_INSTR_TYPE_PQM;

        /// <summary>
        /// Arbitrary string
        /// </summary>
        public string InstrumentModelName = "";

        /// <summary>
        /// Arbitrary string
        /// </summary>
        public string InstrumentModelNumber = "";

        /// <summary>
        /// Optional Contact Name
        /// </summary>
        public string Name = "";

        /// <summary>
        /// Optional Line 1 of a Mailing Address
        /// </summary>
        public string Address1 = "";

        /// <summary>
        /// Optional Line 2 of a Mailing Address
        /// </summary>
        public string Address2 = "";

        /// <summary>
        /// Optional City of a Mailing Address
        /// </summary>
        public string City = "";

        /// <summary>
        /// Optional State or Province
        /// </summary>
        public string State = "";

        /// <summary>
        /// Optional Postal Code
        /// </summary>
        public string PostalCode = "";

        /// <summary>
        /// Optional Country of a Mailing Address
        /// </summary>
        public string Country = "";

        /// <summary>
        /// Optional Telephone Number
        /// </summary>
        public string PhoneVoice = "";

        /// <summary>
        /// Optional Fax Number
        /// </summary>
        public string PhoneFAX = "";

        /// <summary>
        /// Optional Email Address
        /// </summary>
        public string EMail = "";

        /// <summary>
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="CustomSourceInfo"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="CustomSourceInfo"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="CustomSourceInfo"/> class.
        /// </summary>
        public CustomSourceInfo()
        {
            // Default Constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="CustomSourceInfo"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a container record</param>
        /// <param name="PointerCollection">Pointer to the Collection that Contains a Data Source Record in <paramref name="PqdifFile"/></param>
        /// <param name="LogLevelApplication">Specifies the Log Level for Application Errors</param>
        /// <param name="LogLevelCompliance">Specifies the Log Level for Compliance Errors</param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public CustomSourceInfo(CPQDIFNet PqdifFile, ref IntPtr PointerCollection, LogLevels LogLevelApplication, LogLevels LogLevelCompliance, out bool RecordOK)
        {
            // Initialize the return value
            RecordOK = true;

            // Set up an error handler
            try
            {
                // Set the logger level
                LoggerApplication.LogLevel = LogLevelApplication;
                LoggerCompliance.LogLevel = LogLevelCompliance;

                // Determine the number of tags associated with the current record
                int CollectionCount = 0;
                bool Result = PqdifFile.CollectionGetCount(PointerCollection, ref CollectionCount);
                if (!Result)
                {
                    LoggerApplication.Log("CollectionGetCount Error", LogLevels.Error);
                    RecordOK = false;
                    return;
                }

                // For each tag in the CustomSourceInfo collectione...
                for (int CollectionIndex = 0; CollectionIndex < CollectionCount; CollectionIndex++)
                {
                    // Get the pointer to the current tag
                    IntPtr PointerCollectionTag = new IntPtr();
                    Result = PqdifFile.CollectionGetEntry(PointerCollection, CollectionIndex, ref PointerCollectionTag);
                    if (!Result)
                    {
                        LoggerApplication.Log("CollectionGetEntry", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Get the tag ID and tag name
                    Guid TagID = new Guid();
                    string TagName = "";
                    Result = PqdifFile.ElementGetTag(PointerCollectionTag, ref TagID, ref TagName);
                    if (!Result)
                    {
                        LoggerApplication.Log("ElementGetTag", LogLevels.Error);
                        RecordOK = false;
                        return;
                    }

                    // Initialize a flag that tells us that the current TagID was not matched to a known TagID.
                    bool TagMatched = false;

                    // If the current TagID has not been matched, then try to match it as all other known tags of the Container Collection.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagInstrumentTypeID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref InstrumentTypeID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagInstrumentModelName, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref InstrumentModelName);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagInstrumentModelNumber, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref InstrumentModelNumber);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagName, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref Name);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagAddress1, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref Address1);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagAddress2, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref Address2);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagCity, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref City);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagState, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref State);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagPostalCode, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref PostalCode);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagCountry, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref Country);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagPhoneVoice, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref PhoneVoice);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagPhoneFAX, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref PhoneFAX);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagEMail, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "", ref EMail);

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log(": Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the CustomSourceInfo collection

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // CustomSourceInfo


        /// <summary>
        /// Returns the <see cref="CustomSourceInfo"/> Collection Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Custom Source Info Collection Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagCustomSourceInfo>\r\n");

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

            // Export the tags of the Custom Source Info collection as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagInstrumentTypeID, InstrumentTypeID) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagInstrumentModelName, InstrumentModelName) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagInstrumentModelNumber, InstrumentModelNumber) + "\r\n");

            StringBuilder.Append(Utilities.XML.GetElement(tagName, Name) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagAddress1, Address1) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagAddress2, Address2) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagCity, City) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagState, State) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagPostalCode, PostalCode) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagCountry, Country) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagPhoneVoice, PhoneVoice) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagPhoneFAX, PhoneFAX) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagEMail, EMail) + "\r\n");
                        
            // Finish the Custom Source Info record.
            StringBuilder.Append("</tagCustomSourceInfo>\r\n");

            // Return the XML version of the Custom Source Info
            return StringBuilder.ToString();

        } // GetXML

    } // class CustomSourceInfo

} // namespace PQDIF.Validator