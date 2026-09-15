/*
**  Class:          PQDIF.Validator.OneSeriesInstance
**  Description:	One of these collections per series instance
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

    // Contains interfaces and classes that define generic collections
    using System.Collections.Generic;

    public partial class OneSeriesInstance
    {

        /// <summary>
        /// Gets the Series Values
        /// </summary>
        /// <param name="TheSeriesValues">Returns the Series Values</param>
        /// <returns><c>true</c> if <see cref="SeriesValues"/> is not <c>null</c> and is composed of an array of <see cref="bool"/> values</returns>
        public bool GetSeriesValues(ref bool[] TheSeriesValues)
        {
            // Initialize the return values
            bool SeriesOK = false;
            TheSeriesValues = null;

            // If the series values are not null...
            if (SeriesValues != null)
            {
                // If the SeriesValue match the requested type...
                if (SeriesValues.GetType() == typeof(short[]))
                {
                    // The series is not null.
                    SeriesOK = true;

                    // Copy the series values into the array of the data type that we will return.
                    TheSeriesValues = (bool[])SeriesValues;                    

                } // If the SeriesValue match the requested type           

            } // If the series values are not null

            // Return the series values
            return SeriesOK;

        } // GetSeriesValues

        /// <summary>
        /// Gets the Series Values
        /// </summary>
        /// <param name="TheSeriesValues">Returns Unscaled Series Values</param>
        /// <returns><c>true</c> if <see cref="SeriesValues"/> is not <c>null</c> and is composed of an array of <see cref="ushort"/> values</returns>
        public bool GetSeriesValues(ref ushort[] TheSeriesValues)
        {
            // Initialize the return values
            bool SeriesOK = false;
            TheSeriesValues = null;

            // If the series values are not null...
            if (SeriesValues != null)
            {
                // If the SeriesValue match the requested type...
                if (SeriesValues.GetType() == typeof(short[]))
                {
                    // The series is not null.
                    SeriesOK = true;

                    // Copy the series values into the array of the data type that we will return.
                    TheSeriesValues = (ushort[])SeriesValues;                   

                } // If the SeriesValue match the requested type           

            } // If the series values are not null

            // Return the series values
            return SeriesOK;

        } // GetSeriesValues


        /// <summary>
        /// Gets the Series Values
        /// </summary>
        /// <param name="TheSeriesValues">Returns Unscaled Series Values</param>
        /// <returns><c>true</c> if <see cref="SeriesValues"/> is not <c>null</c> and is composed of an array of <see cref="short"/> values</returns>
        public bool GetSeriesValues(ref short[] TheSeriesValues)
        {
            // Initialize the return values
            bool SeriesOK = false;
            TheSeriesValues = null;

            // If the series values are not null...
            if (SeriesValues != null)
            {
                // If the SeriesValue match the requested type...
                if (SeriesValues.GetType() == typeof(short[]))
                {
                    // The series is not null.
                    SeriesOK = true;

                    // Copy the series values into the array of the data type that we will return.
                    TheSeriesValues = (short[])SeriesValues;                  

                } // If the SeriesValue match the requested type           

            } // If the series values are not null

            // Return the series values
            return SeriesOK;

        } // GetSeriesValues


        /// <summary>
        /// Gets the Series Values
        /// </summary>
        /// <param name="TheSeriesValues">Returns Unscaled Series Values</param>
        /// <returns><c>true</c> if <see cref="SeriesValues"/> is not <c>null</c> and is composed of an array of <see cref="uint"/> values</returns>
        public bool GetSeriesValues(ref uint[] TheSeriesValues)
        {
            // Initialize the return values
            bool SeriesOK = false;
            TheSeriesValues = null;

            // If the series values are not null...
            if (SeriesValues != null)
            {
                // If the SeriesValue match the requested type...
                if (SeriesValues.GetType() == typeof(uint[]))
                {
                    // The series is not null.
                    SeriesOK = true;

                    // Copy the series values into the array of the data type that we will return.
                    TheSeriesValues = (uint[])SeriesValues;                    

                } // If the SeriesValue match the requested type           

            } // If the series values are not null

            // Return the series values
            return SeriesOK;

        } // GetSeriesValues


        /// <summary>
        /// Gets the Series Values
        /// </summary>
        /// <param name="TheSeriesValues">Returns Unscaled Series Values</param>
        /// <returns><c>true</c> if <see cref="SeriesValues"/> is not <c>null</c> and is composed of an array of <see cref="int"/> values</returns>
        public bool GetSeriesValues(ref int[] TheSeriesValues)
        {
            // Initialize the return values
            bool SeriesOK = false;
            TheSeriesValues = null;

            // If the series values are not null...
            if (SeriesValues != null)
            {
                // If the SeriesValue match the requested type...
                if (SeriesValues.GetType() == typeof(int[]))
                {
                    // The series is not null.
                    SeriesOK = true;

                    // Copy the series values into the array of the data type that we will return.
                    TheSeriesValues = (int[])SeriesValues;

                } // If the SeriesValue match the requested type           

            } // If the series values are not null

            // Return the series values
            return SeriesOK;

        } // GetSeriesValues


        /// <summary>
        /// Gets the Series Values
        /// </summary>
        /// <param name="TheSeriesValues">Returns Unscaled Series Values</param>
        /// <returns><c>true</c> if <see cref="SeriesValues"/> is not <c>null</c> and is composed of an array of <see cref="float"/> values</returns>
        public bool GetSeriesValues(ref float[] TheSeriesValues)
        {
            // Initialize the return values
            bool SeriesOK = false;
            TheSeriesValues = null;

            // If the series values are not null...
            if (SeriesValues != null)
            {
                // If the SeriesValue match the requested type...
                if (SeriesValues.GetType() == typeof(float[]))
                {
                    // The series is not null.
                    SeriesOK = true;

                    // Copy the series values into the array of the data type that we will return.
                    TheSeriesValues = (float[])SeriesValues;

                } // If the SeriesValue match the requested type           

            } // If the series values are not null

            // Return the series values
            return SeriesOK;

        } // GetSeriesValues


        /// <summary>
        /// Gets the Series Values
        /// </summary>
        /// <param name="TheSeriesValues">Returns Unscaled Series Values</param>
        /// <returns><c>true</c> if <see cref="SeriesValues"/> is not <c>null</c> and is composed of an array of <see cref="double"/> values</returns>
        public bool GetSeriesValues(ref double[] TheSeriesValues)
        {
            // Initialize the return values
            bool SeriesOK = false;
            TheSeriesValues = null;

            // If the series values are not null...
            if (SeriesValues != null)
            {
                // If the SeriesValue match the requested type...
                if (SeriesValues.GetType() == typeof(double[]))
                {
                    // The series is not null.
                    SeriesOK = true;

                    // Copy the series values into the array of the data type that we will return.
                    TheSeriesValues = (double[])SeriesValues;                  

                } // If the SeriesValue match the requested type           

            } // If the series values are not null

            // Return the series values
            return SeriesOK;

        } // GetSeriesValues


        /// <summary>
        /// Gets the Series Values as an array of <see cref="double"/> values, regardless of the data type of <see cref="SeriesValues"/>
        /// </summary>
        /// <param name="StorageMethodID">The <see cref="OneSeriesDefn.StorageMethodID"/> for this <see cref="OneSeriesDefn"/></param>
        /// <returns>Series Values</returns>
        public double[] GetSeriesValues(uint StorageMethodID)
        {           

            // Initialize the return values
            double[] RetValues = null;
            bool ZeroValues = false;
            

            // If the sample values are valid...
            if (SeriesValues != null)
            {
                // If the series values are an array of boolean values...
                if (SeriesValues.GetType() == typeof(bool[]))
                {
                    // Get the unscaled values.
                    bool[] TheSeriesValues = null;
                    GetSeriesValues(ref TheSeriesValues);
                    if (TheSeriesValues != null)
                    {
                        RetValues = new double[TheSeriesValues.Length];
                        for (int i = 0; i < TheSeriesValues.Length; i++)
                            if (TheSeriesValues[i])
                                RetValues[i] = 1;
                            else
                                RetValues[i] = 0;
                    }
                }

                // Otherwise, if the series values are an array of unsigned short integers...
                else if (SeriesValues.GetType() == typeof(ushort[]))
                {
                    // Get the unscaled values.
                    ushort[] TheSeriesValues = null;
                    GetSeriesValues(ref TheSeriesValues);
                    if (TheSeriesValues != null)
                    {
                        RetValues = new double[TheSeriesValues.Length];
                        for (int i = 0; i < TheSeriesValues.Length; i++)
                            RetValues[i] = TheSeriesValues[i];
                    }
                }

                // Otherwise, if the series values are an array of short integers...
                if (SeriesValues.GetType() == typeof(short[]))
                {
                    // Get the unscaled values.
                    short[] TheSeriesValues = null;
                    GetSeriesValues( ref TheSeriesValues);
                    if (TheSeriesValues != null)
                    {
                        RetValues = new double[TheSeriesValues.Length];
                        for (int i = 0; i < TheSeriesValues.Length; i++)
                            RetValues[i] = TheSeriesValues[i];
                    }
                }

                // If the series values are an array of unsigned integers...
                else if (SeriesValues.GetType() == typeof(uint[]))
                {
                    // Get the unscaled values.
                    uint[] TheSeriesValues = null;
                    GetSeriesValues( ref TheSeriesValues);
                    if (TheSeriesValues != null)
                    {
                        RetValues = new double[TheSeriesValues.Length];
                        for (int i = 0; i < TheSeriesValues.Length; i++)
                            RetValues[i] = TheSeriesValues[i];
                    }
                }

                // If the series values are an array of integers...
                else if (SeriesValues.GetType() == typeof(int[]))
                {
                    // Get the unscaled values.
                    int[] TheSeriesValues = null;
                    GetSeriesValues(ref TheSeriesValues);
                    if (TheSeriesValues != null)
                    {
                        RetValues = new double[TheSeriesValues.Length];
                        for (int i = 0; i < TheSeriesValues.Length; i++)
                            RetValues[i] = TheSeriesValues[i];
                    }
                }

                // Otherwise, if the series values are an array of single-precision floating point numbers...
                else if (SeriesValues.GetType() == typeof(float[]))
                {
                    // Get the unscaled values.
                    float[] TheSeriesValues = null;
                    GetSeriesValues( ref TheSeriesValues);
                    if (TheSeriesValues != null)
                    {
                        RetValues = new double[TheSeriesValues.Length];
                        for (int i = 0; i < TheSeriesValues.Length; i++)
                            RetValues[i] = TheSeriesValues[i];
                    }
                }

                // Otherwise, if the series values are an array of double-precision floating point numbers...
                else if (SeriesValues.GetType() == typeof(double[]))
                {
                    // Get the unscaled values.
                    GetSeriesValues( ref RetValues);
                }

                // Otherwise, if the series values are of any other data type...
                else 
                {
                    // Return an array of zeroes.
                    ZeroValues = true;
                    RetValues = new double[SeriesValues.Length];
                }

                // If the series is stored using the increment method and we have at least one sampling rate defined...
                if (((StorageMethodID & (uint)PQDIFNet.Constants.Logical.ID_SERIES_METHOD_INCREMENT) == (uint)PQDIFNet.Constants.Logical.ID_SERIES_METHOD_INCREMENT) && (RetValues.Length > 0))
                {
                    // Initialize the list of incremented samples.
                    List<double> IncrementedSamples = new List<double>();

                    // Determine how many rate ranges there will be.
                    int RateCount = (int)RetValues[0];                   

                    // For each sample rate change...
                    for (int RateIndex = 0; RateIndex < RateCount; RateIndex++)
                    {
                        // If we have a valid sampling rate definition...
                        if (RetValues.Length > RateIndex * 2 + 1)
                        {
                            // Determine the number of points for this rate.
                            int PointCount = (int)RetValues[RateIndex * 2 + 1];

                            // Determine the sample period for this rate.
                            double Rate = RetValues[RateIndex * 2 + 2];

                            // Create samples for each sample of this sampling rate
                            for (int PointIndex = 0; PointIndex < PointCount; PointIndex++)
                            {
                                if (IncrementedSamples.Count > 0)
                                    IncrementedSamples.Add(IncrementedSamples[IncrementedSamples.Count - 1] + Rate);
                                else
                                    IncrementedSamples.Add(0);
                            }                                
                            
                        } // If we have a valid sampling rate definition

                    } // For each sample rate change

                    // If we have incremented samples, replace the sample values with them
                    if (IncrementedSamples.Count > 0)
                        RetValues = IncrementedSamples.ToArray();

                } // If the series is stored using the increment method and we have at least one sampling rate defined


                // Determine if we need to scale the data.
                bool Scaled = (StorageMethodID & (uint)PQDIFNet.Constants.Logical.ID_SERIES_METHOD_SCALED) == (uint)PQDIFNet.Constants.Logical.ID_SERIES_METHOD_SCALED;


                // If we need to apply a scale and offset, and we do not have an array of zeroes, and the series values are not an array of boolean values...
                if (Scaled && !ZeroValues && (SeriesValues.GetType() != typeof(bool[])))
                {
                    // Get the scale and offset.
                    double Scale = Convert.ToDouble(Utilities.Conversion.FixNull(SeriesScale, 1));
                    double Offset = Convert.ToDouble(Utilities.Conversion.FixNull(SeriesOffset, 0));

                    // Scale and offset each sample.
                    for (int i = 0; i < RetValues.Length; i++)
                        RetValues[i] = (RetValues[i] * Scale) + Offset;

                } // If we need to apply a scale and offset, and we do not have an array of zeroes, and the series values are not an array of boolean values


            } // If the sample values are valid

            // Return the series values
            return RetValues;

        } // GetSeriesValues

    } // class OneSeriesInstance

} // namespace PQDIF.Validator