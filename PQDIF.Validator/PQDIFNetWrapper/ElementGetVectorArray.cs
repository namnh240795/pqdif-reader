/*
**  Class:          PQDIF.Validator.PQDIFNetWrapper
**  Description:	
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

    // Defines IEEE PQDIF element types
    using static PQDIFNet.Constants.Element;

    // For logging errors, warnings, other messages.
    using PQDIF.Log;

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
        public static bool ElementGetVectorArray(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref uint[] TagValue)
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

                    // We matched the tag
                    TagMatched = true;

                    // If this element is a vector...
                    if (IsElementVector(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {

                        // Get the value of the tag as a vector
                        int VectorCount = 0;
                        bool Result = PqdifFile.ElementGetVectorCount(PointerCollectionTag, ref VectorCount);

                        // If could get the value of the tag as a vector...
                        if (Result)
                        {
                            // If possibe, determine the size the array into which we will copy values.
                            if (TagValue != null)
                            {
                                // If the size of the actual array and the expected array do not match...
                                if (VectorCount != TagValue.Length)

                                    // Log an compliance error
                                    LoggerCompliance.Log(LogMessagePrefix + ": ElementGetVectorCount: Vector Length should be " + TagValue.Length.ToString() + ".", LogLevels.Error);
                            }
                            else
                            {
                                // Allocate memory for the array 
                                TagValue = new uint[VectorCount];
                            }

                            // Create a buffer to hold the values.
                            Array NewValues = new Array[VectorCount];

                            // Copy the new values into the buffer.
                            Result = PqdifFile.ElementGetVectorArray(PointerCollectionTag, ref NewValues);

                            // If we could copy the new values into the buffer...
                            if (Result)
                            {
                                // Copy each new value into the return array.
                                for (int i = 0; i < VectorCount; i++)
                                    TagValue[i] = ((uint[])NewValues)[i];
                            }

                            // Otherwise, log an application error.
                            else
                            {
                                LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorArray", LogLevels.Error);
                            }

                        } // If could not get the value of the tag as a vector

                        else
                        {
                            // Log an application error.
                            LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorCount", LogLevels.Error);

                        } // If could not get the value of the tag as a vector

                    } // If this element is a vector

                    else
                    {
                        // Log an compliance error
                        LoggerCompliance.Log(LogMessagePrefix + " should be a vector element.", LogLevels.Error);
                    }


                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorArray: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetVectorArray


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
        public static bool ElementGetVectorArray(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref double[] TagValue)
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

                    // We matched the tag
                    TagMatched = true;

                    // If this element is a vector...
                    if (IsElementVector(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {

                        // Get the value of the tag as a vector
                        int VectorCount = 0;
                        bool Result = PqdifFile.ElementGetVectorCount(PointerCollectionTag, ref VectorCount);

                        // If could get the value of the tag as a vector...
                        if (Result)
                        {
                            // If possibe, determine the size the array into which we will copy values.
                            if (TagValue != null)
                            {
                                // If the size of the actual array and the expected array do not match...
                                if (VectorCount != TagValue.Length)

                                    // Log an compliance error
                                    LoggerCompliance.Log(LogMessagePrefix + ": ElementGetVectorCount: Vector Length should be " + TagValue.Length.ToString() + ".", LogLevels.Error);
                            }
                            else
                            {
                                // Allocate memory for the array 
                                TagValue = new double[VectorCount];
                            }

                            // Create a buffer to hold the values.
                            Array NewValues = new Array[VectorCount];

                            // Copy the new values into the buffer.
                            Result = PqdifFile.ElementGetVectorArray(PointerCollectionTag, ref NewValues);

                            // If we could copy the new values into the buffer...
                            if (Result)
                            {
                                // Copy each new value into the return array.
                                for (int i = 0; i < VectorCount; i++)
                                    TagValue[i] = ((double[])NewValues)[i];
                            }

                            // Otherwise, log an application error.
                            else
                            {
                                LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorArray", LogLevels.Error);
                            }

                        } // If could not get the value of the tag as a vector

                        else
                        {
                            // Log an application error.
                            LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorCount", LogLevels.Error);

                        } // If could not get the value of the tag as a vector

                    } // If this element is a vector                        

                    else
                    {
                        // Log an compliance error
                        LoggerCompliance.Log(LogMessagePrefix + " should be a vector element.", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorArray: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetVectorArray


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
        public static bool ElementGetVectorArray(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref Array TagValue)
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

                    // We matched the tag
                    TagMatched = true;

                    // If this element is a vector...
                    if (IsElementVector(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        // Get the value of the tag as a vector
                        int VectorCount = 0;
                        bool Result = PqdifFile.ElementGetVectorCount(PointerCollectionTag, ref VectorCount);

                        // If could not get the value of the tag as a vector...
                        if (Result)
                        {
                            // Copy the new values into the buffer.
                            Result = PqdifFile.ElementGetVectorArray(PointerCollectionTag, ref TagValue);

                            // If we could copy the new values into the buffer...
                            if (!Result)

                                // Log an application error
                                LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorArray", LogLevels.Error);
                        }

                        else
                        {
                            // Log an application error.
                            LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorCount", LogLevels.Error);

                        } // If could not get the value of the tag as a vector 

                    } // If this element is a vector

                    // Otherwise, log a compliance error
                    else
                    {
                        LoggerCompliance.Log(LogMessagePrefix + " should be a vector", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetVectorArray: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetVectorArray

    } // class TagReader

} // namespace PQDIF.Validator