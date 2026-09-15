/*
**  Class:          PQDIF.Validator.PQDIFNetWrapper
**  Description:	Contains Helper Methods for Reading PQDIF Tags
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
    // Contains fundamental classes and base classes that define commonly-used value and reference data types, events and event handlers, interfaces, attributes, and processing exceptions
    using System;

    // Defines IEEE PQDIF methods and data types
    using PQDIFNet;

    // Defines IEEE PQDIF IDs
    using static PQDIFNet.Constants.Element;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

    /// <summary>
    /// Contains Helper Methods for Reading, Adding, and Editing PQDIF Tags
    /// </summary>
    public static partial class PQDIFNetWrapper
    {
        /// <summary>
        /// Returns the Value of a PQDIF Tag in <paramref name="TagValue"/>
        /// </summary>
        /// <param name="ThisTagID">Current Tag ID</param>
        /// <param name="TagToMatch">PQDIF Tag to Match</param>
        /// <param name="PqdifFile">PQDIF File that Contains <paramref name="ThisTagID"/></param>
        /// <param name="PointerCollectionTag">Pointerto the Collection  in <paramref name="PqdifFile"/> that Contains <paramref name="ThisTagID"/></param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LoggerCompliance">PQDIF Compliance Logger</param>
        /// <param name="LogMessagePrefix">Prefix to Append in Front of Messages Logged to <paramref name="LoggerApplication"/></param>
        /// <param name="TagValue">Returns the Value of <paramref name="ThisTagID"/> if <paramref name="TagToMatch"/> is a Match</param>
        /// <returns><c>true</c> if <paramref name="ThisTagID"/> Matches <paramref name="TagToMatch"/></returns>
        public static bool ElementGetString(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref string TagValue)
        {
            // Initialize the return value
            bool TagMatched = false;

            // Set up an error handler
            try
            {
                // If the current tag is the tag for which we are searching...
                if (ThisTagID == TagToMatch)
                {
                    // Append the name of the this tag to the log message.
                    LogMessagePrefix += ": " + GetName(ThisTagID);

                    // We matched the tag.
                    TagMatched = true;

                    // If the element is a string...
                    if (IsElementString(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        // Get the value of the tag as a string.
                        bool Result = PqdifFile.ElementGetString(PointerCollectionTag, ref TagValue);

                        // If we could not get the string value, then log an error.
                        if (!Result)
                            LoggerApplication.Log(LogMessagePrefix + ": ElementGetString", LogLevels.Error);
                    }

                    // Otherwise, log a compliance error.
                    else
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": Not a Vector Element of CHAR1 or CHAR2", LogLevels.Error);
                    }


                } // If the current tag is the tag for which we are searching
            }
            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetString: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetString


        /// <summary>
        /// Returns the Count Elements of a Collection Referenced by <paramref name="CollectionPointer"/> in <paramref name="PqdifFile"/>. 
        /// Logs a compliance error if the element associated with <paramref name="CollectionPointer"/> is not a collection.
        /// </summary>
        /// <param name="PqdifFile">PQDIF File that Contains a Collection Referenced by <paramref name="CollectionPointer"/> </param>
        /// <param name="CollectionPointer">Pointer to a Collection  in <paramref name="PqdifFile"/></param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LoggerCompliance">PQDIF Compliance Logger</param>
        /// <returns>Count Elements of a Collection Referenced by <paramref name="CollectionPointer"/> in <paramref name="PqdifFile"/></returns>
        public static int CollectionGetCount(CPQDIFNet PqdifFile, IntPtr CollectionPointer, Logger LoggerApplication, Logger LoggerCompliance)
        {

            // Initialize the return value
            int CollectionCount = 0;

            // Set up an error handler
            try
            {
                // If CollectionPointer is pointing to a collection...
                if (IsElementCollection(PqdifFile, CollectionPointer, LoggerApplication))
                {
                    // Determine the count of elements in this collection.                    
                    bool Result = PqdifFile.CollectionGetCount(CollectionPointer, ref CollectionCount);

                    // If we could not get the collection count then, log an application error.
                    if (!Result)
                        LoggerApplication.Log("CollectionGetCount Error", LogLevels.Error);
                }

                else
                {
                    // Log a compliance error
                    LoggerCompliance.Log("Not a Collection", LogLevels.Error);
                }

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log("CollectionGetCount Error: " + ex.Message, LogLevels.Error);
            }

            // Return the collection count.
            return CollectionCount;

        } // CollectionGetCount


        /// <summary>
        /// Returns the name of a PQDIF tag
        /// </summary>
        /// <param name="Tag">PQDIF Tag</param>
        /// <returns>PQDIF Name.</returns>
        public static string GetName(Guid Tag)
        {
            // Initialize the return value.
            string TagName = "";

            // Try to get the name of the tag.
            object NullableTagName = PQDIFNet.Constants.Logical.GetName(Tag);

            // If the tag name is not null, then set the return value to the tag name.
            if (NullableTagName != null)
                TagName = (string)NullableTagName;

            // If the tag name is zero length, then return the tag's GUID value as a string.
            if (TagName.Length == 0)
                TagName = Tag.ToString();

            // Return the tag name
            return TagName;

        } // GetName

        /// <summary>
        /// Returns the name of a PQDIF ID
        /// </summary>
        /// <param name="Tag">PQDIF Tag</param>
        /// <param name="ID">PQDIF integer ID</param>
        /// <returns>PQDIF Name.</returns>
        public static string GetName(Guid Tag, int ID)
        {
            // Initialize the return value.
            string IdName = "";

            // Try to get the name of the ID.
            object NullableIdName = PQDIFNet.Constants.Logical.GetName(Tag, ID);

            // If the ID name is not null, then set the return value to the ID name.
            if (NullableIdName != null)
                IdName = (string)NullableIdName;

            // If the ID name is zero length, then return the ID's value as a string.
            if (IdName.Length == 0)
                IdName = ID.ToString();

            // Return the ID name
            return IdName;

        } // GetName


        /// <summary>
        /// Returns <c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_SCALAR"/>
        /// </summary>
        /// <param name="PqdifFile">PQDIF File that Contains a Collection Referenced by <paramref name="PointerCollectionTag"/> </param>
        /// <param name="PointerCollectionTag">Pointer to a Collection  in <paramref name="PqdifFile"/></param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LogMessagePrefix">Prefix to Append in Front of Messages Logged to <paramref name="LoggerApplication"/></param>
        /// <returns><c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_SCALAR"/></returns>
        public static bool IsElementScalar(CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, string LogMessagePrefix)
        {
            // Initialize the return value
            bool RetValue = false;

            // Get the types of this element.
            int TypeElement = 0;
            int TypePhysical = 0;
            bool Result = PqdifFile.ElementGetType(PointerCollectionTag, ref TypeElement, ref TypePhysical);

            // If we could get the element type...
            if (Result)

                // Return true if this element is a scalar.
                RetValue = TypeElement == ID_ELEMENT_TYPE_SCALAR;

            else

                // Log an error
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetType Error", LogLevels.Error);

            // Return 
            return RetValue;

        } // IsElementCollection


        /// <summary>
        /// Returns <c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_VECTOR"/>
        /// </summary>
        /// <param name="PqdifFile">PQDIF File that Contains a Collection Referenced by <paramref name="PointerCollectionTag"/> </param>
        /// <param name="PointerCollectionTag">Pointer to a Collection  in <paramref name="PqdifFile"/></param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LogMessagePrefix">Prefix to Append in Front of Messages Logged to <paramref name="LoggerApplication"/></param>
        /// <returns><c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_VECTOR"/></returns>
        public static bool IsElementVector(CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, string LogMessagePrefix)
        {
            // Initialize the return value
            bool RetValue = false;

            // Get the types of this element.
            int TypeElement = 0;
            int TypePhysical = 0;
            bool Result = PqdifFile.ElementGetType(PointerCollectionTag, ref TypeElement, ref TypePhysical);

            // If we could get the element type...
            if (Result)

                // Return true if this element is a vector.
                RetValue = TypeElement == ID_ELEMENT_TYPE_VECTOR;

            else

                // Log an error
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetType Error", LogLevels.Error);

            // Return 
            return RetValue;

        } // IsElementVector


        /// <summary>
        /// Returns <c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_COLLECTION"/>
        /// </summary>
        /// <param name="PqdifFile">PQDIF File that Contains a Collection Referenced by <paramref name="PointerCollectionTag"/> </param>
        /// <param name="PointerCollectionTag">Pointer to a Collection  in <paramref name="PqdifFile"/></param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <returns><c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_COLLECTION"/></returns>
        public static bool IsElementCollection(CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication)
        {
            // Initialize the return value
            bool RetValue = false;

            // Get the types of this element.
            int TypeElement = 0;
            int TypePhysical = 0;
            bool Result = PqdifFile.ElementGetType(PointerCollectionTag, ref TypeElement, ref TypePhysical);

            // If we could get the element type...
            if (Result)

                // Return true if this element is a collection.
                RetValue = TypeElement == ID_ELEMENT_TYPE_COLLECTION;

            else

                // Log an error
                LoggerApplication.Log("ElementGetType Error", LogLevels.Error);

            // Return 
            return RetValue;

        } // IsElementCollection


        /// <summary>
        /// Returns <c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_COLLECTION"/>
        /// </summary>
        /// <param name="PqdifFile">PQDIF File that Contains a Collection Referenced by <paramref name="PointerCollectionTag"/> </param>
        /// <param name="PointerCollectionTag">Pointer to a Collection in <paramref name="PqdifFile"/></param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LogMessagePrefix">Prefix to Append in Front of Messages Logged to <paramref name="LoggerApplication"/></param>
        /// <returns><c>true</c> if the element is a <see cref="ID_ELEMENT_TYPE_COLLECTION"/></returns>
        public static bool IsElementString(CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, string LogMessagePrefix)
        {
            // Initialize the return value
            bool RetValue = false;

            // Get the types of this element.
            int TypeElement = 0;
            int TypePhysical = 0;
            bool Result = PqdifFile.ElementGetType(PointerCollectionTag, ref TypeElement, ref TypePhysical);

            // If we could get the element type...
            if (Result)

                // Return true if this element is a collection and if it is comprised of ASCII or Unicode characters.
                RetValue = (TypeElement == ID_ELEMENT_TYPE_VECTOR) && ((TypePhysical == PQDIFNet.Constants.Physical.ID_PHYS_TYPE_CHAR1) || (TypePhysical == PQDIFNet.Constants.Physical.ID_PHYS_TYPE_CHAR2));

            else

                // Log an error
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetType Error", LogLevels.Error);

            // Return 
            return RetValue;

        } // IsElementVector


        /// <param name="PqdifFile">PQDIF File that Contains a Collection Referenced by <paramref name="PointerCollection"/></param>
        /// <param name="PointerCollection">Pointer to a Collection in <paramref name="PqdifFile"/></param>
        /// <param name="TagToFind">A tag to find in <paramref name="PointerCollection"/></param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LoggerCompliance">PQDIF Compliance Logger</param>
        /// <param name="PointerToTag">Returns a pointer to the <paramref name="TagToFind"/> if found</param>
        /// <returns><c>true</c> if no errors and if the tag specified by <paramref name="TagToFind"/> was found in the collection specified by <paramref name="PointerCollection"/></returns>
        public static bool GetPointerFromCollection(CPQDIFNet PqdifFile, IntPtr PointerCollection, Guid TagToFind, Logger LoggerApplication, Logger LoggerCompliance, ref IntPtr PointerToTag)
        {

            // Initialize the flag that says we did not find the tag we needed to find.
            bool TagFound = false;

            // If the element is a collection...
            if (IsElementCollection(PqdifFile, PointerCollection, LoggerApplication))
            {
                // Determine the count of tags in the collection.
                int CollectionCount = CollectionGetCount(PqdifFile, PointerCollection, LoggerApplication, LoggerCompliance);

                // For each tag in the specified record...
                for (int CollectionIndex = 0; CollectionIndex < CollectionCount; CollectionIndex++)
                {
                    // Get the pointer to the current tag
                    IntPtr PointerElement = new IntPtr();
                    bool Result = PqdifFile.CollectionGetEntry(PointerCollection, CollectionIndex, ref PointerElement);

                    // If we got the pointer to the current tag...
                    if (Result)
                    {

                        // Get the tag ID and tag name
                        Guid TagID = Guid.Empty;
                        string TagName = "";
                        Result = PqdifFile.ElementGetTag(PointerElement, ref TagID, ref TagName);

                        // If we could get the tag name and ID...
                        if (Result)
                        {
                            // If we have found the tag we needed to find...
                            if (TagID == TagToFind)
                            {
                                // We found the tag that we needed to find.
                                TagFound = true;

                                // Return the pointer to the specified element in the specified collection
                                PointerToTag = PointerElement;

                            } // If we have found the tag we needed to find

                        } // If we could get the tag name and ID

                        // Otherwise, we could not get the tag name and ID.
                        else
                        {
                            LoggerApplication.Log("ElementGetTag", LogLevels.Error);
                        }

                    } // If we got the pointer to the current tag

                } // For each tag in the specified record

            } // If the element is a collection            

            // Return the return value
            return TagFound;

        } // GetPointerFromCollection


        /// <summary>
        /// Adds a New Tag to <paramref name="TagCollection"/>
        /// </summary>
        /// <param name="PqdifFile">PQDIF File that Contains a Record Referenced by <paramref name="RecIndex"/></param>
        /// <param name="RecIndex">Record in which to Add a Tag</param>
        /// <param name="TagCollection">Main Collection in the Record Specified by <paramref name="RecIndex"/> on which to add a Tag</param>
        /// <param name="NewTagID">TagID of the New Tag to Create in <paramref name="TagCollection"/></param>
        /// <param name="NewElementTypeID">Element Type of the New Types</param>
        /// <param name="NewTagValue">Value of New Tag to Add</param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LoggerCompliance">PQDIF Compliance Logger</param>
        /// <param name="LogMessagePrefix">Prefix to Append in Front of Messages Logged to <paramref name="LoggerApplication"/></param>
        /// <returns><c>true</c> if the tag was added, <c>false</c> otherwise.</returns>
        public static bool CollectionAddTag(CPQDIFNet PqdifFile, int RecIndex, Guid TagCollection,
                                                     Guid NewTagID, int NewElementTypeID, object NewTagValue,
                                                     Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix)
        {
            return CollectionAddTag(PqdifFile, RecIndex, TagCollection, null,
                                    NewTagID, NewElementTypeID, NewTagValue,
                                    LoggerApplication, LoggerCompliance, LogMessagePrefix);
        }


        /// <summary>
        /// Adds a New Tag to <paramref name="TagSubCollection"/>
        /// </summary>
        /// <param name="PqdifFile">PQDIF File that Contains a Record Referenced by <paramref name="RecIndex"/></param>
        /// <param name="RecIndex">Record in which to Add a Tag</param>
        /// <param name="TagCollection">Main Collection in the Record Specified by <paramref name="RecIndex"/> on which to add a Tag</param>
        /// <param name="TagSubCollection">Collection under <paramref name="TagCollection"/> on which to add a Tag</param>
        /// <param name="NewTagID">TagID of the New Tag to Create in <paramref name="TagSubCollection"/></param>
        /// <param name="NewElementTypeID">Element Type of the New Types</param>
        /// <param name="NewTagValue">Value of New Tag to Add</param>
        /// <param name="LoggerApplication">PQDIF Application Logger</param>
        /// <param name="LoggerCompliance">PQDIF Compliance Logger</param>
        /// <param name="LogMessagePrefix">Prefix to Append in Front of Messages Logged to <paramref name="LoggerApplication"/></param>
        /// <returns><c>true</c> if the tag was added, <c>false</c> otherwise.</returns>
        public static bool CollectionAddTag(CPQDIFNet PqdifFile, int RecIndex, Guid TagCollection, Guid? TagSubCollection,
                                            Guid NewTagID, int NewElementTypeID, object NewTagValue,
                                            Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix)
        {

            try
            {
                // Get a pointer to the current record
                IntPtr PointerRecord = new IntPtr();
                bool Result = PqdifFile.RecordGetCollection(RecIndex, ref PointerRecord);
                if (!Result)
                {
                    LoggerApplication.Log(LogMessagePrefix + ": RecordGetCollection: Could Not Get Pointer to Record " + RecIndex.ToString(), LogLevels.Error);
                    return false;
                }

                // Get a pointer to the collection.
                IntPtr PointerToCollection = new IntPtr();
                Result = GetPointerFromCollection(PqdifFile, PointerRecord, TagCollection, LoggerApplication, LoggerCompliance, ref PointerToCollection);
                if (!Result)
                {
                    LoggerApplication.Log(LogMessagePrefix + ": GetPointerFromCollection: Could Not Get Pointer to the Collection.", LogLevels.Error);
                    return false;
                }

                // If we need to get a pointer to a subcollection of the collection...
                IntPtr PointerToTargetCollection = new IntPtr();
                if (TagSubCollection != null)
                {
                    // Get a pointer to the subcollection of the main collection.                  
                    Result = GetPointerFromCollection(PqdifFile, PointerToCollection, (Guid)TagSubCollection, LoggerApplication, LoggerCompliance, ref PointerToTargetCollection);
                    if (!Result)
                    {
                        LoggerApplication.Log(LogMessagePrefix + ": GetPointerFromCollection: Could Not Get Pointer to the SubCollection.", LogLevels.Error);
                        return false;
                    }
                }
                else
                {
                    // The target collection is the main collection.
                    PointerToTargetCollection = PointerToCollection;
                }


                // Create an element for the new tag.
                IntPtr PointerElement = new IntPtr();
                Result = PqdifFile.ElementCreate(NewElementTypeID, NewTagID, ref PointerElement);
                if (!Result)
                {
                    LoggerApplication.Log(LogMessagePrefix + ": ElementCreate: Could Not Create a New Element.", LogLevels.Error);
                    return false;
                }

                // Add the new tag to the target collection.
                Result = PqdifFile.CollectionAddEntry(PointerToTargetCollection, PointerElement);
                if (!Result)
                {
                    LoggerApplication.Log(LogMessagePrefix + ": CollectionAddEntry: Could Not Add the New Tag to the Subcollection.", LogLevels.Error);
                    return false;
                }

                // If the new element is a scalar...
                if (NewElementTypeID == ID_ELEMENT_TYPE_SCALAR)

                    // Set the scalar value
                    Result = PqdifFile.ScalarSetValue(PointerElement, NewTagValue);
                if (!Result)
                {
                    LoggerApplication.Log(LogMessagePrefix + ": ScalarSetValue: Could Not Set the Scalar Value.", LogLevels.Error);
                    return false;
                }

                // Otherwise, if the new element is a vector...
                else if ((NewElementTypeID == ID_ELEMENT_TYPE_VECTOR) && (NewTagValue.GetType() == typeof(Array)))
                {
                    Result = PqdifFile.VectorSetArray(PointerElement, (Array)NewTagValue);
                    if (!Result)
                    {
                        LoggerApplication.Log(LogMessagePrefix + ": VectorSetArray: Could Not Set the Vector Value.", LogLevels.Error);
                        return false;
                    }
                }


            } // try

            catch (Exception ex)
            {
                // Log an error message
                LoggerApplication.Log(LogMessagePrefix + ": CollectionAddTagForScalar Error: " + ex.Message, LogLevels.Error);
            }

            // Here if no errors
            return true;

        } // CollectionAddTag

    } // class TagReader      

} // namespace PQDIF.Validator