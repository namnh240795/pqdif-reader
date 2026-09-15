/*
**  Class:
**  Description:	Public Constants are defined here.
**
** --------------------------------------------------------------------------
**
** Copyright 2021-2022 PQDIF Authors
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
#include "pqdif_lg.h"
#include "pqdif_id.h"

using namespace System;
using namespace System::Collections::Generic;


#define GuidFromGUID(guid) \
	System::Guid( guid.Data1, guid.Data2, guid.Data3, \
        guid.Data4[0], guid.Data4[1], \
        guid.Data4[2], guid.Data4[3], \
        guid.Data4[4], guid.Data4[5], \
        guid.Data4[6], guid.Data4[7] )


namespace PQDIFNet
{
	namespace Constants
	{

		/// <summary>
		///  Record Element Type IDs
		/// </summary>
		public ref class Element
		{
		public:
			static const int ID_ELEMENT_TYPE_COLLECTION = 1;
			static const int ID_ELEMENT_TYPE_SCALAR = 2;
			static const int ID_ELEMENT_TYPE_VECTOR = 3;
		};
	
		/// <summary>
		/// Definitions of PQDIF Physical Element IDs
		/// </summary>
		public ref class Physical
		{
		public:
			// Booleans
			static const int ID_PHYS_TYPE_BOOLEAN1 = 1;
			static const int ID_PHYS_TYPE_BOOLEAN2 = 2;
			static const int ID_PHYS_TYPE_BOOLEAN4 = 3;

			// Character
			static const int ID_PHYS_TYPE_CHAR1 = 10;              //  ASCII
			static const int ID_PHYS_TYPE_CHAR2 = 11;              //  Unicode

			// Signed integers
			static const int ID_PHYS_TYPE_INTEGER1 = 20;
			static const int ID_PHYS_TYPE_INTEGER2 = 21;
			static const int ID_PHYS_TYPE_INTEGER4 = 22;

			// Unsigned integers
			static const int ID_PHYS_TYPE_UNS_INTEGER1 = 30;
			static const int ID_PHYS_TYPE_UNS_INTEGER2 = 31;
			static const int ID_PHYS_TYPE_UNS_INTEGER4 = 32;

			// Real/complex
			static const int ID_PHYS_TYPE_REAL4 = 40;
			static const int ID_PHYS_TYPE_REAL8 = 41;
			static const int ID_PHYS_TYPE_COMPLEX8 = 42;           //  Two REAL4s: real, imag
			static const int ID_PHYS_TYPE_COMPLEX16 = 43;          //  Two REAL8s: real, imag

			// Date/time variations
			static const int ID_PHYS_TYPE_TIMESTAMPPQDIF = 50;     //  Physical: TIMESTAMPPQDIF (total 12 bytes)

			// GUID
			static const int ID_PHYS_TYPE_GUID = 60;               //  Physical: GUID (total 16 bytes)

		};


		///<summary>
		/// Definitions of PQDIF Logical Tags and IDs and Associated Lookup Functions
		///</summary>
		public ref class Logical
		{

			static Dictionary<System::Guid, String ^> ^ GuidNames;

			static Dictionary< Tuple<Guid, int> ^, String ^> ^ IntegerNames;

		public:

			// Generate the Tag definitions.
			static System::Guid tagBlank = GuidFromGUID(::tagBlank);
			#define ADD_TAG_ENTRY( tag ) \
				static System::Guid tag = GuidFromGUID( ::##tag );
			#include "name_tag.inc"
			#undef ADD_TAG_ENTRY


			// Generate the ID definitions.
			#define ADD_ID_GUID_ENTRY( id, tag ) \
				static System::Guid id = GuidFromGUID( ::##id );

			#define ADD_ID_UINT4_ENTRY( id, tag ) \
				static const int id = ::##id;

			#include "name_id.inc"

			#undef ADD_ID_GUID_ENTRY
			#undef ADD_ID_UINT4_ENTRY


			/// <summary>
			/// Returns the name of a PQDIF tag or ID.
			/// </summary>
			/// <param name="tagOrID">Value of a PQDIF GUID</param>
			/// <returns>Name of a PQDIF GUID</returns>
			static System::String ^ GetName(System::Guid tagOrID)
			{
				System::String ^ value = nullptr;

				if (GuidNames == nullptr)
				{
					GuidNames = gcnew Dictionary<System::Guid, String ^>();

					#define ADD_TAG_ENTRY( tag ) \
						GuidNames->Add( tag, #tag );
					#include "name_tag.inc"
					#undef ADD_TAG_ENTRY

					#define ADD_ID_GUID_ENTRY( id, tag ) \
						GuidNames->Add( id, #id );
					#define ADD_ID_UINT4_ENTRY( id, tag )
					#include "name_id.inc"
					#undef ADD_ID_GUID_ENTRY
					#undef ADD_ID_UINT4_ENTRY
				}
				GuidNames->TryGetValue(tagOrID, value);
				return value;
			}


			/// <summary>
			/// Returns the name of an PQDIF Integer ID for a given PQDIF Tag.
			/// </summary>
			/// <param name="tag">PQDIF Tag</param>
			/// <param name="id">PQDIF Integer ID</param>
			/// <returns>The name of an PQDIF Integer ID for a given PQDIF Tag.</returns>
			static String ^ GetName(Guid tag, int id)
			{
				System::String ^ value = nullptr;

				if (IntegerNames == nullptr)
				{
					IntegerNames = gcnew Dictionary< Tuple<Guid, int> ^, String ^>();

					#define ADD_ID_GUID_ENTRY( id, tag )
					#define ADD_ID_UINT4_ENTRY( id, tag ) \
						IntegerNames->Add( Tuple::Create( tag, ::id ), #id );
					#include "name_id.inc"
					#undef ADD_ID_GUID_ENTRY
					#undef ADD_ID_UINT4_ENTRY
				}

				IntegerNames->TryGetValue(Tuple::Create(tag, id), value);
				return value;
			}
		};

	};

}