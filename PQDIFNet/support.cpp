/*
**  Class:			
**  Description:	Definitions of some helper methods for CPQDIFNet.
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



#include "stdafx.h"

#include "support.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PQDIFNet {

    /// <summary>Copy a System::Guid to a PQD GUID.
	/// <param name="dst">Holds the converted GUID.</param>
	/// <param name="src">Guid to convert.</param>
    /// </summary>
    void GUIDFromGuid(GUID &dst, System::Guid% src)
    {
		try
		{
			// Copy src to dst.
			Marshal::StructureToPtr(src, (IntPtr)&dst, false);
		}
		catch ( ... )
		{
			;
		}
    }

    /// <summary>Copy a PQD GUID to a System::Guid.
	/// <param name="psrc">Pointer to GUID to convert.</param>
	/// <returns>Converted Guid.</returns>
    /// </summary>
    System::Guid^ GuidFromGUID(GUID *psrc)
    {
        System::Guid^   guid = nullptr;
			
		try
		{
			guid = (System::Guid)Marshal::PtrToStructure((System::IntPtr)psrc, System::Guid::typeid);
		}
		catch ( ... )
		{
			;
		}

        return guid;
    }

    /// <summary>Map a System::Type to an equivalent physical type.
    /// <param name="type">System::Type to map.</param>
    /// <returns>Physical type defined in pqdif_ph.h.</returns>
    /// </summary>
    long PhysicalTypeFromNetType(System::Type^ type)
    {
        long    typePhysical = -1;

        if (type == System::Boolean::typeid)
        {
            typePhysical = ID_PHYS_TYPE_BOOLEAN2;
        }
        else if (type == System::Char::typeid || type == System::Byte::typeid)
        {
            typePhysical = ID_PHYS_TYPE_INTEGER1;
        }
        else if (type == System::Int16::typeid)
        {
            typePhysical = ID_PHYS_TYPE_INTEGER2;
        }
        else if (type == System::Int32::typeid)
        {
            typePhysical = ID_PHYS_TYPE_INTEGER4;
        }
        else if (type == System::Single::typeid)
        {
            typePhysical = ID_PHYS_TYPE_REAL4;
        }
        else if (type == System::Double::typeid)
        {
            typePhysical = ID_PHYS_TYPE_REAL8;
        }
		else if (type == System::Numerics::Complex::typeid)
		{
			typePhysical = ID_PHYS_TYPE_COMPLEX16;
		}

        return typePhysical;
    }

    /// <summary>Convert the specified scalar value to the corresponding
    /// value type.  For example, System::Int32.
	/// <param name="typePhysical">Physical type of value.</param>
	/// <param name="value">Scalar to convert.</param>
	/// <returns>value converted to equivalent .Net type.</returns>
    /// </summary>
    System::Object^ ConvertScalar(long typePhysical, PQDIFValue& value)
    {
        System::Object^ result;
        DATE            tm;

        try
        {
            switch (typePhysical)
            {
                case ID_PHYS_TYPE_BOOLEAN1           :
                    result = gcnew System::Byte(value.bool1);
                    break;
                case ID_PHYS_TYPE_BOOLEAN2           :
                    result = gcnew System::Int16(value.bool2);
                    break;
                case ID_PHYS_TYPE_BOOLEAN4           :
                    result = gcnew System::Int32(value.bool4);
                    break;

                case ID_PHYS_TYPE_CHAR1              :
                    result = gcnew System::Byte(value.char1);
                    break;
                case ID_PHYS_TYPE_INTEGER1           :
                    result = gcnew System::Byte(value.int1);
                    break;
                case ID_PHYS_TYPE_UNS_INTEGER1       :
                    result = gcnew System::Byte(value.uint1);
                    break;

                case ID_PHYS_TYPE_CHAR2              :
                    result = gcnew System::Char(value.char2);
                    break;
                case ID_PHYS_TYPE_INTEGER2           :
                    result = gcnew System::Int16(value.int2);
                    break;
                case ID_PHYS_TYPE_UNS_INTEGER2       :
                    result = gcnew System::UInt16(value.uint2);
                    break;

                case ID_PHYS_TYPE_INTEGER4           :
                    result = gcnew System::Int32(value.int4);
                    break;
                case ID_PHYS_TYPE_UNS_INTEGER4       :
                    result = gcnew System::UInt32(value.uint4);
                    break;

                //  Real/complex
                case ID_PHYS_TYPE_REAL4              :
                    result = gcnew System::Single(value.real4);
                    break;
                case ID_PHYS_TYPE_REAL8              :
                    result = gcnew System::Double(value.real8);
                    break;

                case ID_PHYS_TYPE_COMPLEX8           :
                    result = gcnew System::Numerics::Complex(value.complex8.real, value.complex8.image);
                    break;

                case ID_PHYS_TYPE_COMPLEX16          :
                    result = gcnew System::Numerics::Complex(value.complex16.real, value.complex16.image);
                    break;

                //  Date/time
                case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                    theSupport.SetDateFromTimeStamp( tm, (TIMESTAMPPQDIF)value.ts );
                    result = System::DateTime::FromOADate(tm);
                    break;

                //  GUID
                case ID_PHYS_TYPE_GUID               :
                    result = GuidFromGUID(&value.guid);
                    break;
            }
        }
        catch (System::Exception^ )
        {
        }
        return result;
    }

    /// <summary>
    /// Set the value of the passed scalar from the managed object value.
    /// <param name="typePhysical">The physical type of value.</param>
	/// <param name="theValue">The unmanaged scalar value to set.</param>
    /// <param name="value">New value for scalar, as a System::Object.</param>
    /// <returns>True if successful; false if we don't support the type of value.</returns>
	/// </summary>
    bool SetScalar(long& typePhysical, PQDIFValue& theValue, System::Object^ value)
    {
        bool        status = true;
        long        typeFromNet = PhysicalTypeFromNetType(value->GetType());

        if (typePhysical != typeFromNet)
        {
            typePhysical = typeFromNet;
        }

        try
        {
            switch( typePhysical )
            {
                case ID_PHYS_TYPE_BOOLEAN1           :
                    theValue.bool1 = (BOOL1) value;
                case ID_PHYS_TYPE_BOOLEAN2           :
                    theValue.bool2 = (BOOL2) value;
                case ID_PHYS_TYPE_BOOLEAN4           :
                    theValue.bool4 = (BOOL4) value;
                    break;

                case ID_PHYS_TYPE_CHAR1              :
                    theValue.char1 = (CHAR1) value;
                    break;
                case ID_PHYS_TYPE_INTEGER1           :
                    theValue.int1 = (INT1) value;
                    break;
                case ID_PHYS_TYPE_UNS_INTEGER1       :
                    theValue.uint1 = (UINT1) value;
                    break;

                case ID_PHYS_TYPE_CHAR2              :
                    theValue.char2 = (CHAR2) value;
                    break;
                case ID_PHYS_TYPE_INTEGER2           :
                    theValue.int2 = (INT2) value;
                    break;
                case ID_PHYS_TYPE_UNS_INTEGER2       :
                    theValue.uint2 = (UINT2) value;
                    break;

                case ID_PHYS_TYPE_INTEGER4           :
                    theValue.int4 = (INT4) value;
                    break;
                case ID_PHYS_TYPE_UNS_INTEGER4       :
                    theValue.uint4 = (UINT4) value;
                    break;

                //  Real/complex
                case ID_PHYS_TYPE_REAL4              :
                    theValue.real4 = (REAL4) value;
                    break;
                case ID_PHYS_TYPE_REAL8              :
                    theValue.real8 = (REAL8) value;
                    break;

                case ID_PHYS_TYPE_COMPLEX8           :
					// This shouldn't happen.  We should always
					// map System::Numerics::Complex to COMPLEX16.
					break;

                case ID_PHYS_TYPE_COMPLEX16          :
					theValue.complex16.real = ((System::Numerics::Complex)value).Real;
					theValue.complex16.image = ((System::Numerics::Complex)value).Imaginary;
                    break;

                //  Date/time
                case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                    // TODO - Fix date.
                    //  From the documentation, it appears that the
                    //  whole number portion of the DATE is compatible
                    //  with our .day member. (Although it is possible that
                    //  they will be off by 1 day...)
                    //theValue.ts.day = (UINT4) ( value.date );

                    //  Convert the fractional day to fractional seconds.
                    //theValue.ts.sec = (REAL8) ( value.date - (DATE) theValue.ts.day );
                    //theValue.ts.sec *= (REAL8) SECONDS_PER_DAY;
                    ;
                    break;

                //  GUID
                case ID_PHYS_TYPE_GUID               :
					GUIDFromGuid(theValue.guid, (System::Guid)value);
                    break;

                default:
                    status = false;
                    break;
            }
        }
        catch (System::Exception^)
        {
        }
        return status;

    }

    template<typename T> bool SetVectorFromType(CPQDIF_E_Vector& vect, System::Array^ arValues)
    {
        int         numValues = arValues->Length;
        T*          dst = reinterpret_cast<T*>(vect.GetRawData());

        for (int i=0; i<numValues; i++)
        {
            dst[i] = (T)arValues->GetValue(i);
        }
        return true;
    }

    /// <summary>
    /// Set the values in the vector from the managed object value.
    /// <param name="vect">The vector whose values are to be set.</param>
    /// <param name="arValues">The values.</param>
    /// <returns>True if successful; false if we don't support the type of value.</returns>
    /// </summary>
    bool SetVector(CPQDIF_E_Vector& vect, System::Array^ arValues)
    {
        bool        status = false;
        long        typePhysical = -1;
        int         countItems = 0;
		COMPLEX16	*dst = NULL;
		GUID		*pGuid = NULL;

        try
        {
            // Get the type of data in the array.
            if ((countItems = arValues->Length) > 0)
            {
                if ((typePhysical = PhysicalTypeFromNetType(arValues->GetValue(0)->GetType())) > 0)
                {
                    vect.SetPhysicalType(typePhysical);
                    vect.SetCount(arValues->Length);

                    switch( typePhysical )
                    {
                        case ID_PHYS_TYPE_BOOLEAN1           :
                            status = SetVectorFromType<BOOL1>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_BOOLEAN2           :
                            status = SetVectorFromType<BOOL2>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_BOOLEAN4           :
                            status = SetVectorFromType<BOOL4>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_CHAR1              :
                            status = SetVectorFromType<CHAR1>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_INTEGER1           :
                            status = SetVectorFromType<INT1>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_UNS_INTEGER1       :
                            status = SetVectorFromType<UINT1>(vect, arValues);
                            break;

                        case ID_PHYS_TYPE_CHAR2              :
                            status = SetVectorFromType<CHAR2>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_INTEGER2           :
                            status = SetVectorFromType<INT2>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_UNS_INTEGER2       :
                            status = SetVectorFromType<UINT2>(vect, arValues);
                            break;

                        case ID_PHYS_TYPE_INTEGER4           :
                            status = SetVectorFromType<INT4>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_UNS_INTEGER4       :
                            status = SetVectorFromType<UINT4>(vect, arValues);
                            break;

                        //  Real/complex
                        case ID_PHYS_TYPE_REAL4              :
                            status = SetVectorFromType<REAL4>(vect, arValues);
                            break;
                        case ID_PHYS_TYPE_REAL8              :
                            status = SetVectorFromType<REAL8>(vect, arValues);
                            break;

                        case ID_PHYS_TYPE_COMPLEX8           :
							// This shouldn't happen.  We should always
							// map System::Numerics::Complex to COMPLEX16.
						break;
	
                        case ID_PHYS_TYPE_COMPLEX16          :
							dst = reinterpret_cast<COMPLEX16*>(vect.GetRawData());

							for (int i=0; i<countItems; i++)
							{

								dst[i].real = ((System::Numerics::Complex)arValues->GetValue(i)).Real;
								dst[i].image = ((System::Numerics::Complex)arValues->GetValue(i)).Imaginary;
							}
                            break;

                        //  Date/time
                        case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                            // TODO - Fix date.
                            //  From the documentation, it appears that the
                            //  whole number portion of the DATE is compatible
                            //  with our .day member. (Although it is possible that
                            //  they will be off by 1 day...)
                            //theValue.ts.day = (UINT4) ( value.date );

                            //  Convert the fractional day to fractional seconds.
                            //theValue.ts.sec = (REAL8) ( value.date - (DATE) theValue.ts.day );
                            //theValue.ts.sec *= (REAL8) SECONDS_PER_DAY;
                            ;
                            break;

                        //  GUID
                        case ID_PHYS_TYPE_GUID               :
							pGuid = reinterpret_cast<GUID*>(vect.GetRawData());

							for (int i=0; i<countItems; i++)
							{
								GUIDFromGuid(pGuid[i], (System::Guid)arValues->GetValue(i));
							}
                            break;
                    }
                }
            }
        }
        catch (System::Exception^)
        {
        }
        return status;
    }

    template<typename PQDType, typename NetType> System::Array^ SetArrayFromVector(CPQDIF_E_Vector& vect)
    {
        System::Array^  result=nullptr;
        long            numValues = 0;
        NetType         value;
        PQDType*        src = reinterpret_cast<PQDType*>(vect.GetRawData());

        vect.GetCount(numValues);
        result = gcnew cli::array<NetType>(numValues);

        for (int i=0; i<numValues; i++)
        {
            value = src[i];
            result->SetValue(value, i);
        }
        return result;
    }

    /// <summary>Copy the specified vector to an array of the correct
    /// value type.  For example, System::Array&gt;System::Int32&lt;.
    /// </summary>
    System::Array^ NewArrayFromVector(CPQDIF_E_Vector& vect)
    {
        System::Array^ result=nullptr;
        long typePhysical;
		long numValues = 0;
		COMPLEX8* srcC8 = NULL;
		COMPLEX16* srcC16 = NULL;

        typePhysical = vect.GetPhysicalType();

        try
        {
            switch (typePhysical)
            {
                case ID_PHYS_TYPE_BOOLEAN1           :
                    result = SetArrayFromVector<BOOL1, System::Byte>(vect);
                    break;

                case ID_PHYS_TYPE_BOOLEAN2           :
                    result = SetArrayFromVector<BOOL2, System::Int16>(vect);
                    break;

                case ID_PHYS_TYPE_BOOLEAN4           :
                    result = SetArrayFromVector<BOOL4, System::Int32>(vect);
                    break;

                // These are both typedef'd to char.
                case ID_PHYS_TYPE_CHAR1              :
                case ID_PHYS_TYPE_INTEGER1           :
                    result = SetArrayFromVector<CHAR1, System::Byte>(vect);
                    break;

                case ID_PHYS_TYPE_UNS_INTEGER1       :
                    result = SetArrayFromVector<UINT1, System::Byte>(vect);
                    break;

                // These are both typedef'd as short.
                case ID_PHYS_TYPE_CHAR2              :
                case ID_PHYS_TYPE_INTEGER2           :
                    result = SetArrayFromVector<CHAR2, System::Int16>(vect);
                    break;

                case ID_PHYS_TYPE_UNS_INTEGER2       :
                    result = SetArrayFromVector<UINT2, System::UInt16>(vect);
                    break;

                case ID_PHYS_TYPE_INTEGER4           :
                    result = SetArrayFromVector<INT4, System::Int32>(vect);
                    break;
                case ID_PHYS_TYPE_UNS_INTEGER4       :
                    result = SetArrayFromVector<UINT4, System::UInt32>(vect);
                    break;

                //  Real/complex
                case ID_PHYS_TYPE_REAL4              :
                    result = SetArrayFromVector<REAL4, System::Single>(vect);
                    break;
                case ID_PHYS_TYPE_REAL8              :
                    result = SetArrayFromVector<REAL8, System::Double>(vect);
                    break;

                case ID_PHYS_TYPE_COMPLEX8           :
			        srcC8 = reinterpret_cast<COMPLEX8*>(vect.GetRawData());
					vect.GetCount(numValues);
					result = gcnew cli::array<System::Numerics::Complex>(numValues);
					for (int i=0; i<numValues; i++)
					{
						result->SetValue(System::Numerics::Complex(srcC8[i].real, srcC8[i].image), i);
					}
                    break;

                case ID_PHYS_TYPE_COMPLEX16          :
			        srcC16= reinterpret_cast<COMPLEX16*>(vect.GetRawData());
					vect.GetCount(numValues);
					result = gcnew cli::array<System::Numerics::Complex>(numValues);
					for (int i=0; i<numValues; i++)
					{
						result->SetValue(System::Numerics::Complex(srcC16[i].real, srcC16[i].image), i);
					}
                    break;

                //  Date/time
                case ID_PHYS_TYPE_TIMESTAMPPQDIF     :
                    // Not supported.  Need to throw exception.
                    break;

                //  GUID
                case ID_PHYS_TYPE_GUID               :
			        GUID* srcGUID = reinterpret_cast<GUID*>(vect.GetRawData());
					vect.GetCount(numValues);
					result = gcnew cli::array<System::Guid>(numValues);
					for (int i=0; i<numValues; i++)
					{
						result->SetValue(GuidFromGUID(&(srcGUID[i])), i);
					}
                    break;
            }
        }
        catch (System::Exception^)
        {
        }
        return result;
    }

	/// <summary>Set a vector to a string value.</summary>
	/// <param name="vect">The vector to set.</param>
	/// <param name="str">The string.</param>
	/// <returns>True.</returns>
    bool SetVectorArrayFromString( CPQDIF_E_Vector& vect, System::String^ str )
    {
        pin_ptr<const wchar_t> pString = PtrToStringChars(str);
        wstring         wsString = pString;
        string          theString;

        ws2s(wsString, theString);
        vect.SetValues(theString.c_str());

        return true;
    }

    /// <summary>
    /// Copy the managed array of unsigned long values to an unmanaged array.
	/// <param name="ar">Managed array to copy.</param>
	/// <returns>Unmanaged array of values.</returns>
    /// </summary>
    unsigned long* NewArrayFromNetLong(cli::array<unsigned long>^ ar)
    {
        long    countItems;
        unsigned long *arLong = NULL;

        if ((countItems = ar->Length) > 0)
        {
            arLong = new unsigned long[countItems];

            for (int i=0; i<countItems; i++)
            {
                arLong[i] = ar[i];
            }
        }
        return arLong;
    }
}

