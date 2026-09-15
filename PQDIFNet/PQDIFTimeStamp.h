/*
**  Class:
**  Description:	Provides a .NET managed equivalent to TIMESTAMPPQDIF.
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

#include "support.h"
#include "utils.h"
#include "pqdif_classes_wrapper.h"


using namespace System;
using namespace System::Runtime::InteropServices;

// Day, min, sec conversion.
const int secPerDay = (24 * 60 * 60);

// Bias for FILETIME (1/1/1601) to DT (12/30/1899).
const System::UInt64 ui64Bias = 0x014f35a9a90cc000;

// Multiplier for FILETIME conversion.
const System::UInt64 ui64100nsPerSec = 10000000;

namespace PQDIFNet {

	/// <summary>
	/// Provides the .NET equivalent of the API defined in PQDcom4.
	/// </summary>
	public value struct CPQDIFTimeStamp : IDisposable
	{
	public:
#pragma region Properties
        /// <summary>
        /// Days since January 1, 1900 UCT
        /// </summary>
        property int day;

        /// <summary>
        /// Fractional seconds since midnight of day.
        /// E.g.  seconds / total seconds per day, 86400
        /// </summary>
        property double sec;
#pragma endregion

		/// <summary>
		/// Return this date and time value in the form of an OLE Automation Date type.
		/// This is the Date representation used by Visual Basic 6.
		/// </summary>
		double ToOADate()
		{
		    return (double) day	+  ( (double) sec / (double) SECONDS_PER_DAY );

		}

		/// <summary>
		/// Create with an OLE Automation Date type.
		/// </summary>
		void FromOADate(System::UInt64 oadt)
		{
			//  Initialize the result
			day = 0;
			sec = 0.0;

			System::UInt64 & ui64Value = /* *(UINT64 *)& */ oadt;


			// Bias value from FILETIME base (1/1/1601) to
			// DT base (12/30/1899).
			ui64Value -= ui64Bias;

			// Extract the day.
			day = (System::UInt32)(ui64Value / (secPerDay*ui64100nsPerSec));

			// Extract the seconds.
			sec = (double)(ui64Value % (secPerDay*ui64100nsPerSec)) / 10000000.0;
		}
	};
}