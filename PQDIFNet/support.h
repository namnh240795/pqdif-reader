/*
**  Class:			
**  Description:    Definitions of some helper methods for CPQDIFNet.
**
** --------------------------------------------------------------------------
**
** Copyright 2021 PQDIF Authors
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

#pragma once
#include <vcclr.h>
#include "utils.h"

#include "pqdif_classes_wrapper.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PQDIFNet {

    /// <summary>Copy a System::Guid to a PQD GUID.</summary>
	/// <param name="dst">Holds the converted GUID.</param>
	/// <param name="src">Guid to convert.</param>
    void GUIDFromGuid(GUID &dst, System::Guid% src);

    /// <summary>Copy a PQD GUID to a System::Guid.</summary>
	/// <param name="psrc">Pointer to GUID to convert.</param>
	/// <returns>Converted Guid.</returns>
    System::Guid^ GuidFromGUID(GUID *psrc);

    /// <summary>Map a System::Type to an equivalent physical type.</summary>
    /// <param name="type">System::Type to map.</param>
    /// <returns>Physical type defined in pqdif_ph.h.</returns>
    long PhysicalTypeFromNetType(System::Type^ type);

    /// <summary>Convert the specified scalar value to the corresponding
    /// value type.  For example, System::Int32.
	/// </summary>
	/// <param name="typePhysical">Physical type of value.</param>
	/// <param name="value">Scalar to convert.</param>
	/// <returns>value converted to equivalent .Net type.</returns>
    System::Object^ ConvertScalar(long typePhysical, PQDIFValue& value);

    /// <summary>
    /// Set the value of the passed scalar from the managed object value.
	/// </summary>
    /// <param name="typePhysical">The physical type of value.</param>
	/// <param name="theValue">The unmanaged scalar value to set.</param>
    /// <param name="value">New value for scalar, as a System::Object.</param>
    /// <returns>True if successful; false if we don't support the type of value.</returns>
    bool SetScalar(long& typePhysical, PQDIFValue& theValue, System::Object^ value);

    template<typename T> bool SetVectorFromType(CPQDIF_E_Vector& vect, System::Array^ arValues);

    /// <summary>
    /// Set the values in the vector from the managed object value.
	/// </summary>
    /// <param name="vect">The vector whose values are to be set.</param>
    /// <param name="arValues">The values.</param>
    /// <returns>True if successful; false if we don't support the type of value.</returns>
    bool SetVector(CPQDIF_E_Vector& vect, System::Array^ arValues);


    template<typename PQDType, typename NetType> System::Array^ SetArrayFromVector(CPQDIF_E_Vector& vect);

    /// <summary>Copy the specified vector to an array of the correct
    /// value type.  For example, System::Array&gt;System::Int32&lt;.
    /// </summary>
    System::Array^ NewArrayFromVector(CPQDIF_E_Vector& vect);

	/// <summary>Set a vector to a string value.</summary>
	/// <param name="vect">The vector to set.</param>
	/// <param name="str">The string.</param>
	/// <returns>True.</returns>
    bool SetVectorArrayFromString( CPQDIF_E_Vector& vect, System::String^ str );

    /// <summary>Copy the managed array of unsigned long values to an unmanaged array.</summary>
	/// <param name="ar">Managed array to copy.</param>
	/// <returns>Unmanaged array of values.</returns>
    unsigned long* NewArrayFromNetLong(cli::array<unsigned long>^ ar);
}

