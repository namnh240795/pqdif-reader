/*
**  Class:          PQDIF.Validator.Utilities.Conversion
**  Description:	Reads PQDIF files from a File to a List of PQDIF Objects
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
namespace PQDIF.Validator.Utilities
{
    /// <summary>
    /// For Converting Values
    /// </summary>
    public static class Conversion
    {
        /// <summary>
        /// Converts a <c>null</c> or zero-length string value to a default value
        /// </summary>
        /// <param name="Value">The value to be checked for a <c>null</c> value</param>
        /// <param name="NullValue">Supplies a value to be returned if <paramref name="Value"/> is <c>null</c></param>
        /// <returns>System.Object.</returns>
        /// <remarks>This function is useful for expressions that may include <c>null</c> values. To force an expression to evaluate to a non-<c>null</c> value 
        /// even when it contains a <c>null</c> value, use this function to return zero, a zero-length string, or a custom return value.</remarks>
        public static object FixNull(object Value, object NullValue)
        {
            // Initialize the return value.
            object ReturnValue;

            // If the value is null, then return the null value.
            if (Value == null)
                ReturnValue = NullValue;

            // Otherwise, if the value is a zero-length string, then return the null value.
            else if (Value.ToString().Length == 0)
                ReturnValue = NullValue;

            // Otherwise, return the null value.
            else
                ReturnValue = Value;

            // Return the return value
            return ReturnValue;

        } // FixNull


        /// <summary>
        /// Returns the integer portion of a number.
        /// </summary>
        /// <param name="Number">Required. A number of type <c>object</c> or any valid numeric expression. If Number contains <c>null</c>, <c>null</c> is returned.</param>
        /// <returns>The integer portion of a number.</returns>
        public static int Int(object Number)
        {
            int result = 0;

            if (Information.IsNumeric(Number))
                if (int.TryParse(Number.ToString(), out int result1))
                    result = result1;

            // Returns the integer portion of the number
            return result;

        } // Int

    } // class Conversion

} // PQDifUtilities