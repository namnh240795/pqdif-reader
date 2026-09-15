/*
**  Class:          PQDIF.Validator.Utilities.Information
**  Description:	For Returning Information about Data Types
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
    // Contains fundamental classes and base classes that define commonly-used value and reference data types, events and event handlers, interfaces, attributes, and processing exceptions.
    using System;

    /// <summary>
    /// Methods to Returning Information about Data Types
    /// </summary>
    public static class Information
    {
        /// <summary>
        /// Returns <c>true</c> if the expression represents a valid <see cref="DateTime"/> value
        /// </summary>
        /// <param name="Expression">Value to be Validated as a <see cref="DateTime"/></param>
        /// <returns><c>true</c> if the expression represents a valid <c>DateTime</c> value.</returns>
        public static bool IsDate(object Expression)
        {
            bool result;

            if (Expression == null)
                result = false;

            else if (Expression.GetType() == typeof(DateTime))
                result = true;

            else if (Expression.GetType() == typeof(string))
                result = DateTime.TryParse(Expression.ToString(), out DateTime _1);

            else if (Expression.GetType() == typeof(object))
                result = DateTime.TryParse(Expression.ToString(), out DateTime _1);

            else
                result = false;

            // Returns whether the expression can be evaluated as a number.
            return result;

        } // IsDate


        /// <summary>
        /// Returns <c>true</c> if an expression can be evaluated as a number
        /// </summary>        
        /// <returns><c>true</c> if the expression can be evaluated as a number</returns>
        public static bool IsNumeric(object Expression)
        {
            bool result;

            if (Expression == null)
                result = false;

            else if (Expression.GetType() == typeof(sbyte))
                result = true;

            else if (Expression.GetType() == typeof(byte))
                result = true;

            else if (Expression.GetType() == typeof(char))
                result = double.TryParse(Expression.ToString(), out double _1);

            else if (Expression.GetType() == typeof(short))
                result = true;

            else if (Expression.GetType() == typeof(ushort))
                result = true;

            else if (Expression.GetType() == typeof(int))
                result = true;

            else if (Expression.GetType() == typeof(uint))
                result = true;

            else if (Expression.GetType() == typeof(long))
                result = true;

            else if (Expression.GetType() == typeof(ulong))
                result = true;

            else if (Expression.GetType() == typeof(float))
                result = true;

            else if (Expression.GetType() == typeof(double))
                result = true;

            else if (Expression.GetType() == typeof(bool))
                result = true;

            else if (Expression.GetType() == typeof(string))
                result = double.TryParse(Expression.ToString(), out double _1);

            else if (Expression.GetType() == typeof(object))
                result = double.TryParse(Expression.ToString(), out double _1);

            else
                result = false;

            // Returns whether the expression can be evaluated as a number.
            return result;

        } // IsNumeric

    } // class Information   

} // PQDifUtilities