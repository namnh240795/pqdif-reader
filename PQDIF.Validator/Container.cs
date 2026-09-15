/*
**  Class:          PQDIF.Validator.Container
**  Description:	Record-level tag which identifies the container record (always the first one in the file, and there must be only one per file).
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

    // Defines IEEE PQDIF data types including PQDIF timestamps
    using PQDIFNet;

    // Defines IEEE PQDIF IDs
    using static PQDIFNet.Constants.Logical;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    /// <summary>
    /// Record-level tag which identifies the container record (always the first one in the file, and there must be only one per file).
    /// </summary>
    public class Container
    {
        /// <summary>
        /// Specifies the format version for read/write compatibility.
        /// </summary>
        /// <remarks>Required</remarks>
        public uint[] VersionInfo = new uint[4];

        /// <summary>
        /// Original name of the file
        /// </summary>
        /// <remarks>Required</remarks>
        public string FileName = "";

        /// <summary>
        /// Date/time when the file was created
        /// </summary>
        /// <remarks>Required</remarks>
        public DateTime Creation = new DateTime();

        /// <summary>
        /// Date/time when the file was last saved
        /// </summary>
        public DateTime LastSaved = new DateTime();

        /// <summary>
        /// The number of times the file has been saved/modified
        /// </summary>
        public uint? TimesSaved;

        /// <summary>
        /// The language (English, etc.) of the file
        /// </summary>
        public string Language = "";

        /// <summary>
        /// Arbitrary title
        /// </summary>
        public string Title = "";

        /// <summary>
        /// Arbitrary subject string
        /// </summary>
        public string Subject = "";

        /// <summary>
        /// Individual/company who caused the file to be written
        /// </summary>
        public string Author = "";

        /// <summary>
        /// Keywords for assisting searches
        /// </summary>
        public string Keywords = "";

        /// <summary>
        /// Arbitrary comments
        /// </summary>
        public string Comments = "";

        /// <summary>
        /// Individual/company who last wrote to file
        /// </summary>
        public string LastSavedBy = "";

        /// <summary>
        /// Creating application
        /// </summary>
        public string Application = "";

        /// <summary>
        /// Security descriptor information
        /// </summary>
        public string Security = "";

        /// <summary>
        /// Owner of file contents (This and some of the following fields are for copyright and trademark information)
        /// </summary>
        public string Owner = "";

        /// <summary>
        /// Copyright notice
        /// </summary>
        public string Copyright = "";

        /// <summary>
        /// Trademark notice
        /// </summary>
        public string Trademarks = "";

        /// <summary>
        /// Notes associated with this file (This corresponds to the IEEE COMTRADE .HDR file, for example)
        /// </summary>
        public string Notes = "";

        /// <summary>
        /// Specifies how the compression is applied to the file
        /// </summary>
        public uint? CompressionStyleID = null;

        /// <summary>
        /// Required if tagCompressionStyleID specifies that compression is turned on
        /// </summary>
        public uint? CompressionAlgorithmID = null;

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
        /// IEEE Std 1159.3 Compliance Logger for this <see cref="Container"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerCompliance = new Logger();

        /// <summary>
        /// Application Logger for this <see cref="Container"/>
        /// </summary>
        /// <remarks>Not an IEEE 1159.3 Tag</remarks>
        public Logger LoggerApplication = new Logger();

        /// <summary>
        /// Initializes a new instance of the <see cref="Container"/> class.
        /// </summary>
        public Container()
        {
            // Default constructor
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Container"/> class.
        /// </summary>
        /// <param name="PqdifFile">PQDIF file from which to read a container record</param>
        /// <param name="PointerCollection">Pointer to the Collection Record in <paramref name="PqdifFile"/></param>
        /// <param name="RecordOK"><c>true</c> if the record was read OK</param>
        public Container(CPQDIFNet PqdifFile, ref IntPtr PointerCollection, out bool RecordOK)
        {
            // Initialize the return value
            RecordOK = true;

            // Set up an error handler
            try
            {                

                // Determine the number of tags associated with the current record
                int CollectionCount = 0;
                bool Result = PqdifFile.CollectionGetCount(PointerCollection, ref CollectionCount);
                if (!Result)
                {
                    LoggerApplication.Log("CollectionGetCount", LogLevels.Error);
                    RecordOK = false;
                    return;
                }

                // Initialize flags that tell us if required tags are missing.
                bool MissingTagVersionInfo = true;
                bool MissingTagFileName = true;
                bool MissingTagCreation = true;

                // For each tag in the current collection...
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

                    // If the current TagID has not been matched, then try to match it as tagVersionInfo
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetVectorArray(TagID, tagVersionInfo, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagVersionInfo", ref VersionInfo);
                    if (TagMatched)
                        MissingTagVersionInfo = false;

                    // If the current TagID has not been matched, then try to match it as tagFileName
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagFileName, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagFileName", ref FileName);
                    if (TagMatched)
                        MissingTagFileName = false;

                    // If the current TagID has not been matched, then try to match it as tagCreation
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCreation, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagCreation", ref Creation);
                    if (TagMatched) 
                        MissingTagCreation = false;

                    // If the current TagID has not been matched, then try to match it as all other known tags of the Container Collection.
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagLastSaved, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagLastSaved", ref LastSaved);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagTimesSaved, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagTimesSaved", ref TimesSaved);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagLanguage, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagLanguage", ref Language);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagTitle, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagTitle", ref Title);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagSubject, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagSubject", ref Subject);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagAuthor, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagAuthor", ref Author);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagKeywords, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagKeywords", ref Keywords);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagComments, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagComments", ref Comments);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagLastSavedBy, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagLastSavedBy", ref LastSavedBy);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagApplication, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagApplication", ref Application);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagSecurity, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagSecurity", ref Security);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagOwner, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagOwner", ref Owner);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagCopyright, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagCopyright", ref Copyright);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagTrademarks, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagTrademarks", ref Trademarks);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagNotes, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagNotes", ref Notes);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCompressionStyleID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagCompressionStyleID", ref CompressionStyleID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetScalarData(TagID, tagCompressionAlgorithmID, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagCompressionAlgorithmID", ref CompressionAlgorithmID);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagAddress1, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagAddress1", ref Address1);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagAddress2, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagAddress2", ref Address2);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagCity, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagCity", ref City);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagState, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagState", ref State);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagPostalCode, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagPostalCode", ref PostalCode);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagCountry, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagCountry", ref Country);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagPhoneVoice, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagPhoneVoice", ref PhoneVoice);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagPhoneFAX, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagPhoneFAX", ref PhoneFAX);
                    if (!TagMatched) TagMatched = PQDIFNetWrapper.ElementGetString(TagID, tagEMail, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, "tagEMail", ref EMail);

                    // If still not matched, try to match the current tag to tagBlank, which can be used to leave a space in a collection.
                    if (!TagMatched)
                        TagMatched = TagID == tagBlank;

                    // If the TagID has not been matched, then log an information message about this unknown tag.
                    if (!TagMatched)
                        LoggerCompliance.Log("Unknown or Unexpected Tag: " + PQDIFNetWrapper.GetName(TagID), LogLevels.Info);

                } // For each tag in the current collection


                // Log warnings if required tags are missing.
                if (MissingTagVersionInfo)
                    LoggerCompliance.Log("Required tagVersionInfo is Missing.", LogLevels.Error);

                if (MissingTagFileName)
                    LoggerCompliance.Log("Required tagFileName is Missing.", LogLevels.Error);

                if (MissingTagCreation)
                    LoggerCompliance.Log("Required tagCreation is Missing.", LogLevels.Error);

            } // try

            // Handle exceptions here.
            catch (Exception ex)
            {
                LoggerApplication.Log(ex.Message, LogLevels.Error);
            }

        } // Container


        /// <summary>
        /// Returns the Container Record Formated in XML
        /// </summary>
        /// <param name="ApplicationLog">If set to <c>true</c>, application log messages will be returned</param>
        /// <param name="ComplianceLog">If set to <c>true</c>, IEEE Std 1159.3 compliance log messages will be returned</param>
        /// <value>Container Record Formated in XML</value>
        public string GetXML(bool ApplicationLog, bool ComplianceLog)
        {            
            // Initialize a string builder
            System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();
            StringBuilder.Append("<tagContainer>\r\n");

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

            // Export the tags of the Container record as XML elements.
            StringBuilder.Append(Utilities.XML.GetElement(tagVersionInfo, VersionInfo, null) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagFileName, FileName) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagCreation, Creation) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagLastSaved, LastSaved) + "\r\n");
            if (TimesSaved != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagTimesSaved, (uint)TimesSaved) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagLanguage, Language) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagTitle, Title) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagSubject, Subject) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagAuthor, Author) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagKeywords, Keywords) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagComments, Comments) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagLastSavedBy, LastSavedBy) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagApplication, Application) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagSecurity, Security) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagOwner, Owner) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagCopyright, Copyright) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagTrademarks, Trademarks) + "\r\n");
            StringBuilder.Append(Utilities.XML.GetElement(tagNotes, Notes) + "\r\n");

            if (CompressionStyleID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCompressionStyleID, (uint)CompressionStyleID) + "\r\n");

            if (CompressionAlgorithmID != null)
                StringBuilder.Append(Utilities.XML.GetElement(tagCompressionAlgorithmID, (uint)CompressionAlgorithmID) + "\r\n");

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

            // Finish the Container record.
            StringBuilder.Append("</tagContainer>\r\n");

            // Return the XML version of the Container
            return StringBuilder.ToString();

        } // GetXML      

    } // class Container

} // namespace PQDifValidator