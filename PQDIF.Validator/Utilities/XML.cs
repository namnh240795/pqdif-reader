/*
**  Class:          PQDIF.Validator.Utilities
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

namespace PQDIF.Validator.Utilities
{
    // Contains fundamental classes and base classes that define commonly-used value and reference data types, events and event handlers, interfaces, attributes, and processing exceptions
    using System;

    // Provides methods for encoding and decoding URLs when processing Web requests.
    using static System.Net.WebUtility;

    /// <summary>
    /// Methods for Converting Tags and Tag Values to XML Elements
    /// </summary>
    public static class XML
    {
        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, Guid Value)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Get the tag value
            string IDName = PQDIFNetWrapper.GetName(Value);

            // Return the tag name and value formatted in an XML element.
            return "<" + TagName + @" Type=""GUID"">" + IDName + "</" + TagName + ">";
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, string Value)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Return the tag name and value formatted in an XML element.
            return "<" + TagName + @" Type=""CHAR1"" Size=""" + Value.Length.ToString() + @""">" + HtmlEncode(Value) + "</" + TagName + ">";
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, double Value)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Return the tag name and value formatted in an XML element.
            return "<" + TagName + @" Type=""REAL8"">" + FormatFloat(Value) + "</" + TagName + ">";
        }
               

        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, uint Value)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Get the tag value
            string IDName = PQDIFNetWrapper.GetName(Tag, (int)Value);

            // Return the tag name and value formatted in an XML element.
            return "<" + TagName + @" Type=""UINT"">" + IDName + "</" + TagName + ">";
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, bool Value)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Form the XML with a 1 if true and a 0 if false.
            string XML;
            if (Value)
                XML= "<" + TagName + @" Type=""BOOL4"">1</" + TagName + ">";
            else
                XML= "<" + TagName + @" Type=""BOOL4"">0</" + TagName + ">";

            // Return the tag name and value formatted in an XML element.
            return XML;
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, DateTime Value)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Return the tag name and value formatted in an XML element.
            return "<" + TagName + @" Type=""DT"">" + Value.ToString("yyyy-MM-dd HH:mm:ss.ffffff") + "</" + TagName + ">";
        }             

        

        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, uint[] Value, uint? MaxSeriesValues)
        {
            return GetElement(Tag, (Array)Value, MaxSeriesValues);
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, int[] Value, uint? MaxSeriesValues)
        {
            return GetElement(Tag, (Array)Value, MaxSeriesValues);
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, float[] Value, uint? MaxSeriesValues)
        {
            return GetElement(Tag, (Array)Value, MaxSeriesValues);
        }

        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, double[] Value, uint? MaxSeriesValues)
        {
            return GetElement(Tag, (Array)Value, MaxSeriesValues);
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, bool[] Value, uint? MaxSeriesValues)
        {
            return GetElement(Tag, (Array)Value, MaxSeriesValues);
        }


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <param name="MaxSeriesValues">Maximum Count of Series Samples to Export to XML</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, Array Value, uint? MaxSeriesValues)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Initialize the element with the tag name only.
            string Element = "<" + TagName + "/>";

            // Initialize the element type
            string ElementType = "";

            // If the value is not null...
            if (Value != null)
            {
                // Get the rows and columns.
                int Rows = Value.GetLength(0);

                // Export no more than [MaxSeriesValues] values.
                string Ellipsis = "";

                if (MaxSeriesValues !=null) 
                    if (Rows > MaxSeriesValues)
                    { 
                        Rows = (int)MaxSeriesValues;
                        Ellipsis = "...";
                    }

                // If the array has one or more rows...
                if (Rows > 0)
                {
                    // Initialize the list of values of the array
                    System.Text.StringBuilder StringBuilder = new System.Text.StringBuilder();

                    // Copy the values into a list of values.
                    if (Value.GetType() == typeof(ushort[]))
                    {
                        ElementType = @" Type=""UINT2"" ";
                        for (int i = 0; i < Rows; i++)
                            StringBuilder.Append(((short[])Value)[i].ToString() + ",");
                    }

                    else if (Value.GetType() == typeof(short[]))
                    {
                        ElementType = @" Type=""INT2"" ";
                        for (int i = 0; i < Rows; i++)
                            StringBuilder.Append(((short[])Value)[i].ToString() + ",");
                    }

                    else if (Value.GetType() == typeof(uint[]))
                    { 
                        ElementType = @" Type=""UINT"" ";
                        for (int i = 0; i < Rows; i++)
                            StringBuilder.Append(((uint[])Value)[i].ToString() + ",");
                    }

                    else if (Value.GetType() == typeof(int[]))
                    {
                        ElementType = @" Type = ""INT4"" ";
                        for (int i = 0; i < Rows; i++)
                            StringBuilder.Append(((int[])Value)[i].ToString() + ",");
                    }

                    else if (Value.GetType() == typeof(float[]))
                    {
                        ElementType = @" Type=""REAL4"" ";
                        for (int i = 0; i < Rows; i++)
                            StringBuilder.Append(FormatFloat(((float[])Value)[i]) + ",");
                    }

                    else if (Value.GetType() == typeof(double[]))
                    {
                        ElementType = @" Type=""REAL8"" ";
                        for (int i = 0; i < Rows; i++)
                            StringBuilder.Append(FormatFloat(((double[])Value)[i]) + ",");
                    }

                    else if (Value.GetType() == typeof(bool[]))
                    {
                        ElementType = @" Type=""BOOL4"" ";
                        for (int i = 0; i < Rows; i++)
                        {
                            if (((bool[])Value)[i])
                                StringBuilder.Append("1,");
                            else
                                StringBuilder.Append("0,");
                        }                            
                    }

                    // Trim off the final comma from the value list.
                    if (StringBuilder.Length > 0)
                        StringBuilder.Remove(StringBuilder.Length - 1, 1);

                    // Add the Ellipsis characters if we exceeded the maximum samples to export.
                    StringBuilder.Append(Ellipsis);

                    // Add the elements to the list
                    Element = "<" + TagName + ElementType + @" Size=""" + Rows.ToString() + @""">" + StringBuilder.ToString() + "</" + TagName + ">";

                } // If the array has one or more rows

            } // If the value is not null

            // Return the tag name and value formatted in an XML element.
            return Element;

        } // GetElement


        /// <summary>
        /// Returns an XML Element the Specified <paramref name="Tag"/> and <paramref name="Value"/>
        /// </summary>
        /// <param name="Tag">Tag</param>
        /// <param name="Value">Tag Value</param>
        /// <returns>XML Element</returns>
        public static string GetElement(Guid Tag, object Value)
        {
            // Get the tag name
            string TagName = PQDIFNetWrapper.GetName(Tag);

            // Initialize the element with the tag name only.
            string Element = "<" + TagName + "/>";

            // If the value is not null...
            if (Value !=null)
            {
                // Try to use one of the other methods to get the tag value in addition to the tag name.
                if (Value.GetType() == typeof(Guid))
                    Element = GetElement(Tag, (Guid)Value);

                else if (Value.GetType() == typeof(string))
                    Element = GetElement(Tag, (string)Value);

                else if (Value.GetType() == typeof(double))
                    Element = GetElement(Tag, (double)Value);

                else if (Value.GetType() == typeof(uint))
                    Element = GetElement(Tag, (uint)Value);

                else if (Value.GetType() == typeof(bool))
                    Element = GetElement(Tag, (bool)Value);

                else if (Value.GetType() == typeof(DateTime))
                    Element = GetElement(Tag, (DateTime)Value);

                else if (Value.GetType() == typeof(uint[]))
                    Element = GetElement(Tag, (uint[])Value, null);

                else if (Value.GetType() == typeof(double[]))
                    Element = GetElement(Tag, (double[])Value, null);

            } // If the value is not null            

            // Return the XML element.
            return Element;

        } // GetElement


        /// <summary>
        /// Formats a Floating Point Number with Maximum Precision
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>string.</returns>
        private static string FormatFloat(double Value)
        {
            string ValueFormat = Value.ToString("F32", System.Globalization.CultureInfo.InvariantCulture).TrimEnd('0');
            if (ValueFormat.Substring(ValueFormat.Length - 1, 1) == ".")
                ValueFormat = ValueFormat.Substring(0, ValueFormat.Length - 1);
            return ValueFormat;

        } // FormatFloat


        /// <summary>
        /// Formats a Floating Point Number with Maximum Precision
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>string.</returns>
        private static string FormatFloat(float Value)
        {
            string ValueFormat = Value.ToString("F32", System.Globalization.CultureInfo.InvariantCulture).TrimEnd('0');
            if (ValueFormat.Substring(ValueFormat.Length - 1, 1) == ".")
                ValueFormat = ValueFormat.Substring(0, ValueFormat.Length - 1);
            return ValueFormat;

        } // FormatFloat

    } // XML

} // PQDIF.Validator.Utilities