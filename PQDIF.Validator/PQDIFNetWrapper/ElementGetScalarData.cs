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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref int? TagValue)
        {
            // Initialize the non-nullable value.
            int NonNullableValue = 0;

            // Determine if the tag can be found and get the non-nullable value.
            bool TagFound = ElementGetScalarData(ThisTagID, TagToMatch, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, LogMessagePrefix, ref NonNullableValue);

            // If we found the desired tag...
            if (TagFound)

                // Return the value.
                TagValue = NonNullableValue;

            // Return true if we found the desired tag.
            return TagFound;

        } // ElementGetScalarData


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref int TagValue)
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

                    // Initialize the tag's value
                     object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {
                        // If the tag's value is numeric...
                        if (Utilities.Information.IsNumeric(Value))

                            // Return the tag's value as an unsigned integer
                            if (Value.GetType() == typeof(short))                                                          
                                TagValue = (short)Value;
                            else if (Value.GetType() == typeof(int))
                                TagValue = (int)Value;

                        else

                            // Log a compliance problem.
                            LoggerCompliance.Log(LogMessagePrefix + ": Tag Value is Not Numeric.", LogLevels.Error);
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref uint? TagValue)
        {
            // Initialize the non-nullable value.
            uint NonNullableValue = 0;

            // Determine if the tag can be found and get the non-nullable value.
            bool TagFound = ElementGetScalarData(ThisTagID, TagToMatch, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, LogMessagePrefix, ref NonNullableValue);

            // If we found the desired tag...
            if (TagFound)

                // Return the value.
                TagValue = NonNullableValue;

            // Return true if we found the desired tag.
            return TagFound;

        } // ElementGetScalarData


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref uint TagValue)
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

                    // Initialize the tag's value
                    object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {
                        // If the tag's value is numeric...
                        if (Utilities.Information.IsNumeric(Value))

                            // Return the tag's value as an unsigned integer
                            TagValue = (uint)Value;

                        else

                            // Log a compliance problem.
                            LoggerCompliance.Log(LogMessagePrefix + ": Tag Value is Not Numeric.", LogLevels.Error);
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData             


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref Guid TagValue)
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

                    // Initialize the tag's value
                    object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {
                        // If the tag's value is a GUID...
                        if (Value.GetType() == typeof(Guid))

                            // Return the tag's value as an GUID.
                            TagValue = (Guid)Value;

                        else

                            // Log a compliance problem.
                            LoggerCompliance.Log(LogMessagePrefix + ": Tag Value is Not a GUID.", LogLevels.Error);
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref double? TagValue)
        {

            // Initialize the non-nullable value.
            double NonNullableValue = 0;

            // Determine if the tag can be found and get the non-nullable value.
            bool TagFound = ElementGetScalarData(ThisTagID, TagToMatch, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, LogMessagePrefix, ref NonNullableValue);

            // If we found the desired tag...
            if (TagFound)

                // Return the value.
                TagValue = NonNullableValue;

            // Return true if we found the desired tag.
            return TagFound;

        }


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref double TagValue)
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

                    // Initialize the tag's value
                    object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {
                        // If the tag's value is numeric...
                        if (Utilities.Information.IsNumeric(Value))

                            // Return the tag's value as a double
                            TagValue = Convert.ToDouble(Value);

                        else

                            // Log a compliance problem.
                            LoggerCompliance.Log(LogMessagePrefix + ": Tag Value is Not Numeric.", LogLevels.Error);
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref CPQDIFTimeStamp TagValue)
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

                    // Initialize the tag's value
                    object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {

                        // If the tag's value is a date...
                        if (Utilities.Information.IsDate(Value))

                            // Return the tag's value as a PQDIF TimeStamp
                            TagValue.FromOADate((ulong)((DateTime)Value).ToFileTimeUtc());

                        else

                            // Log a compliance problem.
                            LoggerCompliance.Log(LogMessagePrefix + ": Tag Value is Not a Date.", LogLevels.Error);
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData



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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref DateTime? TagValue)
        {

            // Initialize the non-nullable value.
            DateTime NonNullableValue = new DateTime();

            // Determine if the tag can be found and get the non-nullable value.
            bool TagFound = ElementGetScalarData(ThisTagID, TagToMatch, PqdifFile, PointerCollectionTag, LoggerApplication, LoggerCompliance, LogMessagePrefix, ref NonNullableValue);

            // If we found the desired tag...
            if (TagFound)

                // Return the value.
                TagValue = NonNullableValue;

            // Return true if we found the desired tag.
            return TagFound;

        }



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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref DateTime TagValue)
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

                    // Initialize the tag's value
                    object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {

                        // If the tag's value is a date...
                        if (Utilities.Information.IsDate(Value))

                            // Return the tag's value as a PQDIF TimeStamp
                            TagValue = (DateTime)Value;

                        else

                            // Log a compliance problem.
                            LoggerCompliance.Log(LogMessagePrefix + ": Tag Value is Not a Date.", LogLevels.Error);
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData



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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref bool TagValue)
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

                    // Initialize the tag's value
                    object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {
                        // If the tag's value is numeric...
                        if (Utilities.Information.IsNumeric(Value))
                        {

                            // Return the tag's value as a boolean
                            TagValue = Convert.ToBoolean(Value);
                        }
                        else

                            // Log a compliance problem.
                            LoggerCompliance.Log(LogMessagePrefix + ": Tag Value is Not Numeric.", LogLevels.Error);
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData


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
        public static bool ElementGetScalarData(Guid ThisTagID, Guid TagToMatch, CPQDIFNet PqdifFile, IntPtr PointerCollectionTag, Logger LoggerApplication, Logger LoggerCompliance, string LogMessagePrefix, ref object TagValue)
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

                    // Initialize the tag's value
                    object Value = null;

                    // If the element is not a scalar, then exit.
                    if (!IsElementScalar(PqdifFile, PointerCollectionTag, LoggerApplication, LogMessagePrefix))
                    {
                        LoggerCompliance.Log(LogMessagePrefix + ": should be a scalar.", LogLevels.Error);
                        return TagMatched;
                    }

                    // Get the tag's value as a scalar
                    bool Result = PqdifFile.ElementGetScalarData(PointerCollectionTag, ref Value);

                    // If we could get a value from the tag...
                    if (Result)
                    {
                        // Return the tag's value as an unsigned integer
                        TagValue = Value;
                    }
                    else
                    {
                        // Log an error
                        LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData", LogLevels.Error);
                    }

                } // If the current tag is the tag for which we are searching

            } // try

            catch (Exception ex)
            {
                LoggerApplication.Log(LogMessagePrefix + ": ElementGetScalarData: " + ex.Message, LogLevels.Error);
            }

            // Return the a flag that indicates if we matched the tag.
            return TagMatched;

        } // ElementGetScalarData

        /// <summary>
        /// Returns a PQDIF Time Stamp from a <see cref="DateTime"/>
        /// </summary>
        /// <param name="TimeStamp">Time Stamp</param>
        /// <returns>PQDIF TimeStamp.</returns>
        public static CPQDIFTimeStamp GetPqdifTimeStamp(DateTime TimeStamp)
        {
            CPQDIFTimeStamp PQDIFTimeStamp = new CPQDIFTimeStamp();
            PQDIFTimeStamp.FromOADate((ulong)TimeStamp.ToFileTimeUtc());
            return PQDIFTimeStamp;
        }

    } // class TagReader

} // namespace PQDIF.Validator