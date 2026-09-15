/*
**  Class:
**  Description:	Provides a .NET managed API to pqdiflib. The implementation is in CPQDiffNet.h.
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

#include "PQDIFNet.h"

namespace PQDIFNet {

	/////////////////////////////////////////////////////////////////////////////
	/// Private methods.

	/// <summary>Do we have a persistence controller?</summary>
	bool CPQDIFNet::IsInitialized()
	{
		return (m_percont == NULL ? false : true);
	}

	/// <summary>
	/// Is this a valid element handle?
	/// <param name="hElement">Handle to test.</param>
	/// <returns>hElement cast to an element pointer.</returns>
	/// </summary>
	CPQDIF_Element* CPQDIFNet::ValidateElement(System::IntPtr hElement)
	{
		CPQDIF_Element	*pel;

		pel = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );

		return pel;
	}

	/// <summary>
	/// Is this a valid collection handle?
	/// <param name="hElement">Handle to test.</param>
	/// <returns>hElement cast to a collection pointer or null if this is not a valid handle.</returns>
	/// </summary>
	CPQDIF_E_Collection* CPQDIFNet::ValidateCollection(System::IntPtr hElement)
	{
		CPQDIF_Element		*pel;
		CPQDIF_E_Collection *pcoll = NULL;

		pel = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION )
			{
				pcoll = (CPQDIF_E_Collection *) pel;
				ASSERT_VALID( pcoll );
			}
		}

		return pcoll;
	}

	/// <summary>
	/// Is this a valid scalar handle?
	/// <param name="hElement">Handle to test.</param>
	/// <returns>hElement cast to a scalar pointer or null if this is not a valid handle.</returns>
	/// </summary>
	CPQDIF_E_Scalar* CPQDIFNet::ValidateScalar(System::IntPtr hElement)
	{
		CPQDIF_Element	*pel;
		CPQDIF_E_Scalar *pscalar = NULL;

		pel = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR )
			{
				pscalar = (CPQDIF_E_Scalar *) pel;
				ASSERT_VALID( pscalar );
			}
		}
		return pscalar;
	}

	/// <summary>
	/// Is this a valid vector handle?
	/// <param name="hElement">Handle to test.</param>
	/// <returns>hElement cast to a vector pointer or null if this is not a valid handle.</returns>
	/// </summary>
	CPQDIF_E_Vector* CPQDIFNet::ValidateVector(System::IntPtr hElement)
	{
		CPQDIF_Element	*pel;
		CPQDIF_E_Vector *pvector = NULL;

		pel = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR )
			{
				pvector = (CPQDIF_E_Vector *) pel;
				ASSERT_VALID( pvector );
			}
		}
		return pvector;
	}

	/// <summary>
	/// Is this a valid observation record handle?
	/// <param name="hRecordObserv">Handle to test.</param>
	/// <returns>hElement cast to an observation record pointer or null if
	/// this is not a valid handle.
	/// </returns>
	/// </summary>
	CPQDIF_R_Observation* CPQDIFNet::ValidateObservation(System::IntPtr hRecordObserv)
	{
		CPQDIFRecord			*prec;
		CPQDIF_R_Observation	*pobs = NULL;
		GUID					tagThisRecord;

		prec = (CPQDIFRecord *) hRecordObserv.ToPointer();
		ASSERT_VALID( prec );
		if( prec )
		{
			prec->HeaderGetTag( tagThisRecord );
			if( PQDIF_IsEqualGUID( tagThisRecord, tagRecObservation ) )
			{
				pobs = (CPQDIF_R_Observation *) prec;
				ASSERT_VALID( pobs );
			}
		}
		return pobs;
	}

	/// <summary>
	/// Is this a valid data source record handle?
	/// <param name="hRecDS">Handle to test.</param>
	/// <returns>hElement cast to a data source record pointer or null if
	/// this is not a valid handle.
	/// </returns>
	/// </summary>
	CPQDIF_R_DataSource* CPQDIFNet::ValidateDataSource(System::IntPtr hRecDS)
	{
		CPQDIFRecord		*prec;
		CPQDIF_R_DataSource *pds = NULL;
		GUID				tagThisRecord;

		prec = (CPQDIFRecord *) hRecDS.ToPointer();
		ASSERT_VALID( prec );
		if( prec )
		{
			prec->HeaderGetTag( tagThisRecord );
			if( PQDIF_IsEqualGUID( tagThisRecord, tagRecDataSource ) )
			{
				pds = (CPQDIF_R_DataSource *) prec;
				ASSERT_VALID( pds );
			}
		}
		return pds;
	}

	/// <summary>
	/// Is this a valid settings record handle?
	/// <param name="hRecSettings">Handle to test.</param>
	/// <returns>hElement cast to a settings record pointer or null if
	/// this is not a valid handle.
	/// </returns>
	/// </summary>
	CPQDIF_R_Settings* CPQDIFNet::ValidateSettings(System::IntPtr hRecSettings)
	{
		CPQDIFRecord		*prec;
		CPQDIF_R_Settings	*pset = NULL;
		GUID				tagThisRecord;

		prec = (CPQDIFRecord *) hRecSettings.ToPointer();
		ASSERT_VALID( prec );
		if( prec )
		{
			prec->HeaderGetTag( tagThisRecord );
			if( PQDIF_IsEqualGUID( tagThisRecord, tagRecMonitorSettings ) )
			{
				pset = (CPQDIF_R_Settings *) prec;
				ASSERT_VALID( pset );
			}
		}
		return pset;
	}

	/////////////////////////////////////////////////////////////////////////////
	/// Constructor and destructor.

	/// <summary>
	/// Default constructor.  Instantiate our one and only instance of
	/// the persistence controller.
	/// </summary>
	CPQDIFNet::CPQDIFNet() : m_percont(NULL)
	{
		// Allocated on the unmanaged heap.
		m_percont = (CPQDIF_PC_FlatFile *) theFactory.NewPersistController( PFPC_FlatFile );
	}

	/// <summary>
	/// Destructor.  Free our instance of the persistence controller.
	/// </summary>
	CPQDIFNet::~CPQDIFNet()
	{
		delete m_percont;
		m_percont = NULL;
	}

	/////////////////////////////////////////////////////////////////////////////
	/// Properties.

	/// <summary>
	/// The pqd file name.
	/// </summary>
	String^ CPQDIFNet::FlatFileName::get()
	{
		String^ strResult=nullptr;
		const char *flatFileName=NULL;

		if (IsInitialized())
		{
			flatFileName = m_percont->GetFileName();
			if (flatFileName != NULL)
			{
				strResult = gcnew String(flatFileName);
			}
		}
		return strResult;
	}
	void CPQDIFNet::FlatFileName::set(String^ newVal)
	{
		size_t	length=0;
		char   *pBuffer=NULL;
		pin_ptr<const wchar_t> wcsFlatFileName = PtrToStringChars(newVal);

		if (IsInitialized() && (length = wcslen(wcsFlatFileName)) > 0)
		{
			if ((pBuffer = new char[length+1]) != NULL)
			{
				wcstombs_s(&length, pBuffer, (length+1)*sizeof(char), wcsFlatFileName, length);
				if (length > 0)
				{
					m_percont->SetFileName(pBuffer);
				}
				delete[] pBuffer;
				pBuffer = NULL;
			}
		}
	}

	/// <summary>
	/// The compression algorithm.
	/// </summary>
	long CPQDIFNet::CompressionAlgorithm::get()
	{
		long	value=0;

		if (IsInitialized())
		{
			value = m_percont->GetCompressionAlgorithm();
		}
		return value;
	}
	void CPQDIFNet::CompressionAlgorithm::set(long newVal)
	{
		if (IsInitialized())
		{
			m_percont->SetCompressionAlgorithm(newVal);
		}
	}

	/// <summary>
	/// </summary>
	bool CPQDIFNet::CanWriteIncremental::get()
	{
		bool	value=false;

		if (IsInitialized())
		{
			value = m_percont->GetCanWriteIncremental();
		}
		return value;
	}

	/// <summary>
	/// The number of tags in the pqd file.
	/// </summary>
	long CPQDIFNet::TagCount::get()
	{
		long	value=0;

		if (IsInitialized())
		{
			// theInfo is a singleton defined in pqdiflib\pqdinfo.cpp.
			value = (long)theInfo.GetTagCount();
		}
		return value;
	}

	/// <summary>
	/// The compression type.
	/// </summary>
	long CPQDIFNet::CompressionStyle::get()
	{
		long	value=0;

		if (IsInitialized())
		{
			value = m_percont->GetCompressionStyle();
		}
		return value;
	}
	void CPQDIFNet::CompressionStyle::set(long newVal)
	{
		if (IsInitialized())
		{
			m_percont->SetCompressionStyle(newVal);
		}
	}

	/// <summary>
	/// The number of records in this file.
	/// </summary>
	long CPQDIFNet::RecordCount::get()
	{
		long	value=0;

		if (IsInitialized())
		{
			value = m_percont->GetRecordCount();
		}

		return value;
	}

	/////////////////////////////////////////////////////////////////////////////
	/// Public methods.

	/// <summary>
	/// For compatibility with PQDCom.	Returns value of RecordCount property.
	/// </summary>
	long CPQDIFNet::RecordGetCount()
	{
		long	value=0;

		if (IsInitialized())
		{
			value = m_percont->GetRecordCount();
		}

		return value;
	}

	/// <summary>
	/// Read the pqd file specified by the FlatFile property.
	/// </summary>
	bool CPQDIFNet::Read()
	{
		bool	ret=false;

		if (IsInitialized())
		{
			ret = m_percont->ReadHeaders();
		}

		return ret;
	}

	/// <summary>
	/// Close the pqd file.
	/// </summary>
	bool CPQDIFNet::Close()
	{
		return New();
	}

	/// <summary>
	/// Get information for the record specified by index.
	/// <param name="index">The index of the record.</param>
	/// <param name="tagRecordGUID">Out param that returns the GUID for this record type.</param>
	/// <param name="sizeHeader">Out param that returns the size of the record header.</param>
	/// <param name="sizeRecord">Out param that returns the size of the record.</param>
	/// <param name="posThisRecord">Out param that returns the offset of this record in the file.</param>
	/// <param name="posNextRecord">Out param that returns the offset of the next record in the file.</param>
	/// <returns>True if successful; false if not or if we have not been initialized.</returns>
	/// </summary>
	bool CPQDIFNet::RecordGetInfo(long index, Guid% tagRecordGUID, String^% nameRecordType, long% sizeHeader, long% sizeRecord, long% posThisRecord, long% posNextRecord)
	{
		bool			status=false;
		CPQDIFRecord	*prec=NULL;
		GUID			tagRecord;
		string			nameTag;
		wstring 		wNameTag;
		SIZE4			s4Header;
		SIZE4			s4Body;
		LINKABS4		linkThis;
		LINKABS4		linkNext;

		if (IsInitialized())
		{
			try
				{
				prec = m_percont->GetRecord( index );
				if( prec )
					{
					ASSERT_VALID( prec );

					//	Get header info
					prec->HeaderGetTag( tagRecord );
					prec->HeaderGetSize( s4Header, s4Body );
					prec->HeaderGetPos( linkThis );
					prec->HeaderGetPosNextRecord( linkNext );

					//	Return the data...
					// Copy the GUID to a System::Guid.
					tagRecordGUID = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&tagRecord, System::Guid::typeid);

					//	The GUID name
					nameTag = theInfo.GetNameOfTag( tagRecord );
					wNameTag = c2ws(nameTag.c_str());
					nameRecordType = gcnew String(wNameTag.c_str());

					//	Size & pos info
					sizeHeader = s4Header;
					sizeRecord = s4Body;
					posThisRecord = linkThis;
					posNextRecord = linkNext;

					status = true;
					}
			}
			catch ( ... /*System::Exception^ e*/)
			{
				//^ msg = e->Message;
				//^ exType = e->GetType()->ToString();
			}
		}
		return status;
	}

	/// <summary>
	/// Get the main collection from the record indicated by index.
	/// <param name="index">Index of the record.</param>
	/// <param name="hCollection">Out param that returns the handle of the collection as an unsigned 64-bit value."
	/// This handle is passed to other APIs to access the contents of the collection.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordGetCollection(long index, System::IntPtr% hCollection)
	{
		bool				status=false;
		CPQDIFRecord		*prec;
		CPQDIF_E_Collection *pcollMain;

		if (IsInitialized())
		{
			try
			{
				prec = m_percont->GetRecordFull( index );
				ASSERT_VALID( prec );
				if( prec )
				{
					pcollMain = prec->GetMainCollection();
					if( pcollMain )
					{
						//	Return this pointer as the element ID
						hCollection = (System::IntPtr)pcollMain;
						status = true;
					}
				}

			}
			catch( ... )
			{
				// e->Delete();
				//status = false;
			}
		}
		return status;
	}

	/// <summary>
	/// Get this element's Guid.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="tagElement">Out param that returns the Guid for this element type.</param>
	/// <param name="nameTagElement">Out param that returns the name for this element type.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetTag(System::IntPtr hElement, Guid% tagElement, String^% nameTagElement)
	{
		bool				status = false;
		CPQDIF_Element		*pel;
		GUID				tagElementLocal;
		string				name;
		wstring 			wsName;

		try
		{
			pel = ValidateElement( hElement );
			if( pel )
			{
				name = theInfo.GetNameOfTag( pel->GetTag() );
				wsName = c2ws(name.c_str());
				nameTagElement = gcnew String(wsName.c_str());

				// Get the GUID and copy it to a System::Guid.
				tagElementLocal = pel->GetTag();
				tagElement = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&tagElementLocal, System::Guid::typeid);

				status = true;
			}
		}
		catch( ... )
		{
			// e->Delete();
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Get the element type and the type of the data it holds.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="typeElement">Out param that takes the element type.</param>
	/// <param name="typePhysical">Out param that takes the physical type of the data in this element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetType( System::IntPtr hElement, long% typeElement, long% typePhysical)
	{
		bool			status = false;
		CPQDIF_Element	*pel;

		try
		{
			pel = (CPQDIF_Element *) hElement.ToPointer();
			ASSERT_VALID( pel );
			if( pel )
			{
				typeElement = pel->GetElementType();
				typePhysical = pel->GetPhysicalType();
				status = true;
			}
		}
		catch( ... )
		{
			// e->Delete();
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Get the scalar data stored in this element.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="value">Out param that returns the scalar data stored in this element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetScalarData( System::IntPtr hElement, System::Object^% value)
	{
		bool			status = false;
		CPQDIF_E_Scalar *pscalar;
		long			typePhysical = 0;
		PQDIFValue		theValue;

		value = nullptr;
		pscalar = ValidateScalar( hElement );
		if( pscalar )
		{
			//	Get the value and convert to a standard type.
			status = pscalar->GetValue(typePhysical, theValue);
			value = ConvertScalar(typePhysical, theValue);
		}
		return status;
	}

	/// <summary>
	/// Get the scalar data at the specified index in this vector.
	/// <param name="hElement">Handle to a vector element.</param>
	/// <param name="index">Index of datum in vector to retrieve.</param>
	/// <param name="value">Out param that returns the scalar data stored in this element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetVectorData( System::IntPtr hElement, long index, System::Object^% value)
	{
		bool			status = false;
		CPQDIF_Element	*pel;
		CPQDIF_E_Vector *pvector = NULL;
		long			typePhysical = 0;
		PQDIFValue		theValue;

		value = nullptr;
		pel = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR )
			{
				pvector = (CPQDIF_E_Vector *) pel;
				typePhysical = pvector->GetPhysicalType();
				if (pvector->GetValue(index, theValue) == true)
				{
					value = ConvertScalar(typePhysical, theValue);
					status = true;
				}
			}
		}
		return status;
	}

	/// <summary>
	/// Get the number of items in the vector contained in this element.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="count">Out param that takes the number of items in the vector contained in this element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetVectorCount( System::IntPtr hElement, long% count)
	{
		bool			status = false;
		CPQDIF_Element	*pel;
		long			lcount = 0;

		pel = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR )
			{
				CPQDIF_E_Vector * pvector = (CPQDIF_E_Vector *) pel;

				//	Get the value!
				status = pvector->GetCount( lcount );
				count = lcount;
			}
		}
		return status;
	}

	/// <summary>
	/// Create a new element of the specified type.
	/// <param name="typeElement">The type of element to create.</param>
	/// <param name="tag">Guid for this element.</param>
	/// <param name="hElement">Out param that returns a handle to the new element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementCreate(long typeElement, System::Guid tag, System::IntPtr% hElement)
	{
		bool			status = false;
		CPQDIF_Element	*pel;
		GUID tagLocal;

		// Convert from System::Guid to our GUID structure.
		GUIDFromGuid(tagLocal, tag);

		pel = theFactory.NewElement( typeElement );
		if( pel )
		{
			pel->SetTag( tagLocal );
			hElement = (System::IntPtr) pel;
			status = true;
		}
		return status;
	}

	/// <summary>
	/// Delete the specified element.
	/// <param name="hElement">The element to delete.</param>
	/// <returns>True if successful; false if hElement is invalid.</returns>
	/// </summary>
	bool CPQDIFNet::ElementDestroy( System::IntPtr hElement)
	{
		bool			status = false;
		CPQDIF_Element	*pel;

		pel = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			delete pel;
			status = true;
		}
		return status;
	}

	/// <summary>
	/// Add an element to a collection.
	/// <param name="hCollection">The collection.</param>
	/// <param name="hElement">The element to add.</param>
	/// <returns>True if successful; false if hCollection and, or hElement is, are invalid.</returns>
	/// </summary>
	bool CPQDIFNet::CollectionAddEntry( System::IntPtr hCollection,  System::IntPtr hElement)
	{
		bool			status = false;
		CPQDIF_Element	*pel;
		CPQDIF_Element	*pelToAdd;

		pel = (CPQDIF_Element *) hCollection.ToPointer();
		pelToAdd = (CPQDIF_Element *) hElement.ToPointer();
		ASSERT_VALID( pel );
		ASSERT_VALID( pelToAdd );
		if( pel && pelToAdd )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION )
			{
				CPQDIF_E_Collection * pcoll = (CPQDIF_E_Collection *) pel;

				//	Add that sucker!
				pcoll->Add( pelToAdd );
				status = true;
			}
		}
		return status;
	}

	/// <summary>Deprecated.</summary>
	bool CPQDIFNet::CollectionInsertEntry( System::IntPtr /*hCollection*/, long /*indexToInsert*/, System::IntPtr /*pElement*/)
	{
		//	This method is deprecated since the collections are sorted
		//	in this implementation.
		return false;
	}

	/// <summary>
	/// Remove an element from a collection.
	/// <param name="hCollection">The collection from which we remove the element.</param>
	/// <param name="index">Index in the collection of the element to remove.</param>
	/// <returns>True if successful; false if hCollection is invalid.</returns>
	/// </summary>
	bool CPQDIFNet::CollectionRemoveEntry( System::IntPtr hCollection, long index)
	{
		bool				status = false;
		CPQDIF_Element		*pel;
		CPQDIF_E_Collection *pcoll;

		pel = (CPQDIF_Element *) hCollection.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION )
			{
				pcoll = (CPQDIF_E_Collection *) pel;
				//	Remove it!
				pcoll->RemoveAt( index );
				status = true;
			}
		}
		return status;
	}

	/// <summary>
	/// Remove and delete an element from a collection.
	/// <param name="hCollection">The collection.</param>
	/// <param name="index">Index of the element in the collection to remove.</param>
	/// <returns>True if successful; false if hCollection is invalid.</returns>
	/// </summary>
	bool CPQDIFNet::CollectionDestroyEntry( System::IntPtr hCollection, long index)
	{
		bool				status = false;
		long				count = 0;
		CPQDIF_Element		*pel;
		CPQDIF_E_Collection *pcoll;

		pel = (CPQDIF_Element *) hCollection.ToPointer();
		ASSERT_VALID( pel );
		if( pel )
		{
			if( pel->GetElementType() == ID_ELEMENT_TYPE_COLLECTION )
			{
				pcoll = (CPQDIF_E_Collection *) pel;
				count = pcoll->GetCount();
				if( index < count )
				{
					pel = pcoll->GetElement( index );
					if( pel )
					{
						//	Remove it!
						pcoll->RemoveAt( index );
						// Delete it!
						delete pel;
						status = true;
					}
				}
			}
		}
		return status;
	}

	/// <summary>
	/// Call WriteNew() on our persistence controller.
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet:: WriteNew()
	{
		bool	status = false;

		if (IsInitialized())
		{
			status = m_percont->WriteNew();
		}
		return status;
	}

	/// <summary>
	/// Call WriteIncremental() on our persistence controller.
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::WriteIncremental()
	{
		bool	status = false;

		if (IsInitialized())
		{
			status = m_percont->WriteIncremental();
		}
		return status;
	}

	/// <summary>
	/// Call WriteToStream() on our persistence controller.
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::WriteToStream(System::IO::MemoryStream^ pstrm)
	{
		bool	status = false;

		if (IsInitialized())
		{
			CPQDIF_MemoryStreamIO *pstrmMemory = new CPQDIF_MemoryStreamIO(pstrm);
			status = m_percont->WriteToStream(pstrmMemory);
			delete pstrmMemory;
			pstrmMemory = NULL;
		}
		return status;
	}

	/// <summary>
	/// Get the vector stored in this element.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="arValues">Out param that returns the vector as an array.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetVectorArray(System::IntPtr hElement, Array^% arValues)
	{
		bool			status = false;
		CPQDIF_E_Vector *pvector = NULL;

		pvector  = ValidateVector( hElement );
		if( pvector )
		{
			arValues = NewArrayFromVector(*pvector);
			status = true;
		}
		return status;
	}

	/// <summary>
	/// Checks if the scalar in this element is a string.
	/// <param name="hElement">Handle to the element.</param>
	/// <returns>True if the data is a string; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementIsString( System::IntPtr hElement)
	{
		bool			status = false;
		CPQDIF_E_Vector *pvector = NULL;

		pvector = ValidateVector( hElement );
		if( pvector )
		{
			if( pvector->GetPhysicalType() == ID_PHYS_TYPE_CHAR1 ||
				pvector->GetPhysicalType() == ID_PHYS_TYPE_CHAR2 )
			{
				status = true;
			}
		}
		return status;
	}

	/// <summary>
	/// Retrieves the value from a string element.
	/// <param name="hElement">Handle to the element.</param>
	/// <param name="sValue">Out param that returns the string.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetString( System::IntPtr hElement, String^% sValue)
	{
		bool			status = false;
		CPQDIF_E_Vector *pvector = NULL;

		pvector = ValidateVector( hElement );
		if( pvector )
		{
			sValue = gcnew System::String(reinterpret_cast<char *>(pvector->GetRawData()));
			status = true;
		}
		return status;
	}

	//	NOT IMPLEMENTED
	/// TODO Not sure what to do with this but made signature .Net compatible.
	bool CPQDIFNet::RecordCreateByNameInsert(String^ nameTagRecordType, long indexToInsert)
	{
		bool			status = false;
		CPQDIFRecord	*prec;

		if (IsInitialized())
		{
			prec = theFactory.NewRecord( PFR_Record );
			if( prec )
			{
				status = m_percont->InsertRecord( prec, indexToInsert );

				//	Not implemented
				nameTagRecordType = nameTagRecordType;
			}
		}
		return status;
	}

	/// <summary>
	/// Remove and delete the record at index.
	/// <param name="index">Index of record.</param>
	/// <returns>True if successful; False if index is invalid or we are not initialized.</returns>
	/// </summary>
	bool CPQDIFNet::RecordDestroy(long index)
	{
		bool			status = false;
		CPQDIFRecord	*prec;

		if (IsInitialized())
		{
			prec = m_percont->GetRecord( index );
			if( prec )
			{
				status = m_percont->RemoveRecord( index );
				if( status )
				{
					delete prec;
					status = true;
				}
			}
		}
		return status;
	}

	/// <summary>
	/// Sets the value of the scalar to value.
	/// <param name="hScalar">Handle to the scalar.</param>
	/// <param name="value">New value as a System::Object.</param>
	/// <returns>True if successful; false if scalar is not valid.</returns>
	/// </summary>
	bool CPQDIFNet::ScalarSetValue( System::IntPtr hScalar, System::Object^ value)
	{
		bool			status = false;
		CPQDIF_E_Scalar *pscalar = NULL;
		PQDIFValue		theValue;
		long			typePhysical;

		pscalar = ValidateScalar( hScalar );
		if( pscalar )
		{
			// Get the current type
			typePhysical = pscalar->GetPhysicalType();

			// Update the value.
			status = SetScalar(typePhysical, theValue, value);

			// Set the new value.
			pscalar->SetValue(typePhysical, theValue);
		}
		return status;
	}

	/// <summary>
	/// Sets the value of the vector to this String value.
	/// <param name="hVector">Handle to the Vector.</param>
	/// <param name="value">New String value.</param>
	/// <returns>True if successful; false if scalar is not valid.</returns>
	/// </summary>
	bool CPQDIFNet::VectorSetString( System::IntPtr hVector, String^ value)
	{
		bool			status = false;
		CPQDIF_E_Vector *pvector = NULL;
		string			stringOutput;

		pvector = ValidateVector( hVector );
		if( pvector )
		{
			status = SetVectorArrayFromString( *pvector, value );
		}

		return status;
	}

	/// <summary>
	/// Sets the values of the vector to the specified array.
	/// <param name="hVector">Handle to the vector.</param>
	/// <param name="arValues">Values.</param>
	/// <returns>True if successful.</returns>
	/// </summary>
	bool CPQDIFNet::VectorSetArray( System::IntPtr hVector, Array^ arValues)
	{
		bool			status = false;
		CPQDIF_E_Vector *pvector = NULL;

		pvector = ValidateVector( hVector );
		if( pvector )
		{
			status = SetVector( *pvector, arValues );
		}
		return status;
	}

	/// <summary>
	/// Read bytes into the specified character array.  The number of bytes to read
	/// is determined by the length of the array.
	/// <param name="ar">Character array to hold the data.</param>
	/// <returns>True if successful.</returns>
	/// </summary>
	bool CPQDIFNet::ReadChunk(cli::array<unsigned char>^% ar)
	{
		bool	status = false;
		int 	countBytes = 0;

		if (IsInitialized())
		{
			GCHandle pinnedArray = GCHandle::Alloc(ar, GCHandleType::Pinned);
			IntPtr pbData = pinnedArray.AddrOfPinnedObject();

			try
			{
				countBytes = ar->Length;

				//	Tell the controller that this is our input
				m_percont->SetChunkInput( (BYTE *)pbData.ToPointer(), countBytes );
				m_percont->ReadHeaders();
			}
			catch( ... )
			{
				// e->Delete();
				status = false;
			}
			finally
			{
				Marshal::FreeHGlobal(pbData);
			}
		}
		return status;
	}

	/// <summary>
	/// Write bytes from the specified character array to the file.  The number of bytes to
	/// write is determined by the length of the array.
	/// <param name="ar">Character array to write.</param>
	/// <returns>True if successful.</returns>
	/// </summary>
	bool CPQDIFNet::WriteChunk(cli::array<unsigned char>^ ar)
	{
		bool	status = false;
		int 	countBytes = 0;

		if (IsInitialized())
		{
			GCHandle pinnedArray = GCHandle::Alloc(ar, GCHandleType::Pinned);
			IntPtr pbData = pinnedArray.AddrOfPinnedObject();

			try
			{
				countBytes = ar->Length;

				//	Copy the contents of the chunk
				m_percont->GetChunkOutput( (BYTE *)pbData.ToPointer(), countBytes );
			}
			catch( ... )
			{
				// e->Delete();
				status = false;
			}
			finally
			{
				Marshal::FreeHGlobal(pbData);
			}
		}
		return status;
	}

	/// <summary>
	/// Creates a new instance of the Persistence Controller and frees any current instance first.
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::New()
	{
		//	Destroy & re-create
		if (m_percont)
		{
			delete m_percont;
			m_percont = NULL;
		}

		m_percont = (CPQDIF_PC_FlatFile *) theFactory.NewPersistController( PFPC_FlatFile );

		return IsInitialized();
	}

	/// <summary>Get the tag name of the Guid specified by index.
	/// <param name="index">Index of Guid.</param>
	/// <returns>The Guid's tag name.</returns>
	/// </summary>
	String^ CPQDIFNet::GetTagName(long index)
	{
		String^ strResult=nullptr;
		std::string strTagName="";

		if (IsInitialized())
		{
			theInfo.GetTagName(index, strTagName);
			if (!strTagName.empty())
			{
				strResult = gcnew String(strTagName.c_str());
			}
		}
		return strResult;
	}

	/// <summary>
	/// Get the data for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="arValues">Out param that returns array of series values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesData( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Array^% arValues)
	{
		bool					result=false;
		CPQDIF_R_Observation	*pobs=NULL;
		CPQDIF_E_Vector 		*pvect=NULL;
		double *				pvalues;
		long					countPoints;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				pvalues = pobs->NewResolvedSeries( idxChannel, idxSeries, countPoints );
				if( pvalues )
				{
					// Put the values in a VECTOR then use NewArrayFromVector to convert
					// to System::Array.

					pvect = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
					if( pvect )
					{
						pvect->SetValuesREAL8(pvalues, countPoints);
						arValues = NewArrayFromVector(*pvect);
						delete pvect;
						pvect = NULL;
						result = true;
					}

					delete [] pvalues;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			result = false;
		}


		//	Clean up
		if( pvect )
		{
			delete pvect;
		}
		return result;
	}

	/// <summary>Get information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="timeStart">Out param that returns the start time.</param>
	/// <param name="name">Out param that returns the name for this record.</param>
	/// <param name="countChannels">Out param that returns the number of channels.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetInfo( System::IntPtr hRecordObserv, CPQDIFTimeStamp% timeStart, String^% name, long% countChannels)
	{
		bool					status = true;
		CPQDIF_R_Observation	*pobs;
		TIMESTAMPPQDIF			timeStartLocal;
		TIMESTAMPPQDIF			timeCreateLocal;
		string					nameLocal;
		wstring 				wName;

		countChannels = 0;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				//	Get channel count
				countChannels = pobs->GetCountChannels();

				//	Get time & name
				status = pobs->GetInfo( timeStartLocal, timeCreateLocal, nameLocal );

				//	Transfer info over
				if( status )
				{
					timeStart.day = timeStartLocal.day;
					timeStart.sec = timeStartLocal.sec;
				}

				// Return the name.
				wName =c2ws(nameLocal.c_str());
				name = gcnew String(wName.c_str());
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>Get information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="timeStart">Out param that returns the start time.</param>
	/// <param name="name">Out param that returns the name for this record.</param>
	/// <param name="countChannels">Out param that returns the number of channels.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetInfo( System::IntPtr hRecordObserv, double% timeStart, String^% name, long% countChannels)
	{
		bool					status = true;
		CPQDIFTimeStamp 		ts;

		try
		{
			if ((status = ObservationGetInfo(hRecordObserv, ts, name, countChannels)) == true)
			{
				timeStart = ts.ToOADate();
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>Get information for this channel in an observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Out param that returns the index of the channel.</param>
	/// <param name="name">Out param that returns the name for this channel.</param>
	/// <param name="idPhase"></param>
	/// <param name="idQuantityType"></param>
	/// <param name="countSeries">Out param that returns the number of series values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetChannelInfo( System::IntPtr hRecordObserv, long idxChannel, String^% name, long% idPhase, Guid %idQuantityType, long% countSeries)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		string					nameLocal;
		UINT4					idPhaseLocal;
		GUID					idQuantityTypeLocal;
		UINT4					idQuantityMeasuredLocal;
		wstring 				wNameLocal;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				//	Make sure we're in the right range.
				if( idxChannel >= 0 && idxChannel < pobs->GetCountChannels() )
				{
					countSeries = pobs->GetCountSeries( static_cast<long>(idxChannel) );

					//	Get full channel info
					status = pobs->GetChannelInfo( static_cast<long>(idxChannel), nameLocal, idPhaseLocal, idQuantityTypeLocal, idQuantityMeasuredLocal );
					if( status )
					{
						wNameLocal = c2ws(nameLocal.c_str());
						name = gcnew String(wNameLocal.c_str());

						// Copy the GUID to a System::Guid.
						idQuantityType = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idQuantityTypeLocal, System::Guid::typeid);

						idPhase = idPhaseLocal;
					}
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}

		return status;
	}

	/// <summary>Get information for the series data in this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="idQuantityUnits"></param>
	/// <param name="idValueType"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesInfo( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, int% idQuantityUnits, Guid %idValueType)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		UINT4		idQuantityUnitsLocal;
		GUID		idValueTypeLocal;
		GUID		idQuantityCharacteristicLocal;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				status = pobs->GetSeriesInfo( idxChannel, idxSeries, idQuantityUnitsLocal, idQuantityCharacteristicLocal, idValueTypeLocal );
				if( status )
				{
					//	Translate information
					idQuantityUnits = idQuantityUnitsLocal;

					// Copy the GUID to a System::Guid.
					idValueType = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idValueTypeLocal, System::Guid::typeid);
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}

		return status;
	}

	/// <summary>Free the specified observation record.
	/// <param name="hRecordObserv">The record to free.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordReleaseObservation( System::IntPtr hRecordObserv)
	{
		bool					result=false;
		CPQDIF_R_Observation	*pobs;

		pobs = ValidateObservation( hRecordObserv );
		if( pobs )
		{
			delete pobs;
			result = true;
		}
		return result;
	}

	/// <summary>Get an observation record specified by index.
	/// <param name="index">Record to get.</param>
	/// <param name="hRecordObserv">Out param that returns a handle to the record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordRequestObservation(long index, System::IntPtr% hRecordObserv)
	{
		bool					status = false;
		CPQDIFRecord			*precBase;
		CPQDIFRecord			*precCurrent = NULL;
		CPQDIFRecord			*precDS = NULL;
		CPQDIFRecord			*precSett = NULL;
		CPQDIF_R_Observation	*pobs = NULL;

		GUID	tagRecord;
		long	idxRec;

		if (IsInitialized())
		{
			try
			{
				//	Find the observation and associated data source
				precBase = m_percont->GetRecordFull( index );
				if( precBase )
				{
					//	Is this an observation?
					precBase->HeaderGetTag( tagRecord );
					if( PQDIF_IsEqualGUID( tagRecord, tagRecObservation ) )
					{
						//	Find its data source...
						//	Search backward through the record list.
						for( idxRec = index - 1; idxRec >= 0; idxRec-- )
						{

							//	Are we still looking for the DS?
							if( !precDS )
							{
								precCurrent = m_percont->GetRecord( idxRec );
								precCurrent->HeaderGetTag( tagRecord );
								if( PQDIF_IsEqualGUID( tagRecord, tagRecDataSource ) )
								{
									//	Found it!
									precDS = m_percont->GetRecordFull( idxRec );
									status = true;
									//	We're cool even if we just find this one. We should break at this point,
									//	because we won't find any valid settings records *before* it!
									break;
								}
							}

							//	Are we still looking for the settings?
							if( !precSett )
							{
								precCurrent = m_percont->GetRecord( idxRec );
								precCurrent->HeaderGetTag( tagRecord );
								if( PQDIF_IsEqualGUID( tagRecord, tagRecMonitorSettings ) )
								{
									//	Found it!
									precSett = m_percont->GetRecordFull( idxRec );
									//	precSett is not required, so we don't change
									//	the value of status.
								}
							}
						}	//	for( records )
					}
				}	//	if( found obs record )

				//	Create an observation wrapper for the record
				//	(NOTE: precSett is not required)
				if( status && precBase && precDS )
				{
					pobs = theFactory.NewObservationWrapper2( precBase, precDS, precSett );
					if( pobs )
					{
						hRecordObserv = (System::IntPtr) pobs;
						status = true;
					}
				}
			}
			catch( ... )
			{
				// e->Delete();
				//status = false;
			}
		}
		return status;
	}

	/// <summary>
	/// Get the settings record for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="hRecordSettings">Out param that returns the handle of the settings record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSettings( System::IntPtr hRecordObserv, System::IntPtr% hRecordSettings)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;

		hRecordSettings = System::IntPtr::Zero;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				CPQDIF_R_Settings * pSet;
				pSet = pobs->GetMonitorSettings();
				if (pSet)
				{
					hRecordSettings = (System::IntPtr) pSet;
					status = true;
				}
			}
		}
		catch (...)
		{
	//		result = false;
		}

		return status;
	}

	/// <summary>
	/// Get trigger information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idTriggerMethod">The trigger method.</param>
	/// <param name="arTriggerChannels">The trigger channels.</param>
	/// <param name="timeTriggered">The trigger time.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetTriggerInfo( System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, CPQDIFTimeStamp% timeTriggered)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		UINT4					idTriggerMethodLocal;
		CPQDIF_E_Vector *		pvectTriggerChanIdx = NULL;
		TIMESTAMPPQDIF			timeTriggeredLocal;


		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				status = pobs->GetTriggerInfo( idTriggerMethodLocal, &pvectTriggerChanIdx, timeTriggeredLocal );
				if( status )
				{
					//	Transfer the information
					if( pvectTriggerChanIdx )
					{
						arTriggerChannels = NewArrayFromVector(*pvectTriggerChanIdx);
					}
					timeTriggered.day = timeTriggeredLocal.day;
					timeTriggered.sec = timeTriggeredLocal.sec;
				}

				idTriggerMethod = (long) idTriggerMethodLocal;
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get trigger information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idTriggerMethod">The trigger method.</param>
	/// <param name="arTriggerChannels">The trigger channel.</param>
	/// <param name="timeTriggered">The trigger time.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetTriggerInfo( System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, double% timeTriggered)
	{
		bool					status = false;
		CPQDIFTimeStamp 		ts;

		try
		{
			if ((status = ObservationGetTriggerInfo(hRecordObserv, idTriggerMethod, arTriggerChannels, ts)) == true)
			{
				timeTriggered = ts.ToOADate();
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesBaseQty( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		double					theValue = 0;

		pobs = ValidateObservation( hRecordObserv );
		if( pobs )
		{
			status = pobs->GetSeriesBaseQuantity( idxChannel, idxSeries, theValue );
			value = theValue;
		}
		return status;
	}

	/// <summary>
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesNominalQty( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		double					theValue = 0;

		pobs = ValidateObservation( hRecordObserv );
		if( pobs )
		{
			status = pobs->GetSeriesDefnNominal( idxChannel, idxSeries, theValue );
			value = theValue;
		}
		return status;
	}

	/// <summary>
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationSetSeriesBaseQty( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value)
	{
		bool	status = false;
		CPQDIF_R_Observation *	pobs;
		double	theValue = 0;

		pobs = ValidateObservation( hRecordObserv );
		if( pobs )
		{
			status = pobs->SetSeriesBaseQuantity( idxChannel, idxSeries, theValue );
			value = theValue;
		}
		return status;
	}

	/// <summary>Create a new container record and return its index.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateContainer(String^ language, String^ title, String^ subject, String^ author, String^ keywords, String^ comments,
		String^ lastSavedBy, String^ application, String^ security, String^ owner, String^ copyright, String^ trademark, String^ notes)
	{
		long		idxRecord = -1;

		if (IsInitialized())
		{
			pin_ptr<const wchar_t> wcsLanguage = PtrToStringChars(language);
			pin_ptr<const wchar_t> wcsTitle = PtrToStringChars(title);
			pin_ptr<const wchar_t> wcsSubject = PtrToStringChars(subject);
			pin_ptr<const wchar_t> wcsAuthor = PtrToStringChars(author);
			pin_ptr<const wchar_t> wcsKeywords = PtrToStringChars(keywords);
			pin_ptr<const wchar_t> wcsComments = PtrToStringChars(comments);
			pin_ptr<const wchar_t> wcsLastSavedBy = PtrToStringChars(lastSavedBy);
			pin_ptr<const wchar_t> wcsApplication = PtrToStringChars(application);
			pin_ptr<const wchar_t> wcsSecurity = PtrToStringChars(security);
			pin_ptr<const wchar_t> wcsOwner = PtrToStringChars(owner);
			pin_ptr<const wchar_t> wcsCopyright = PtrToStringChars(copyright);
			pin_ptr<const wchar_t> wcsTrademark = PtrToStringChars(trademark);
			pin_ptr<const wchar_t> wcsNotes = PtrToStringChars(notes);

			try
			{
				idxRecord = m_percont->CreateContainerRecord(
					wc2s(wcsLanguage).c_str(),
					wc2s(wcsTitle).c_str(),
					wc2s(wcsSubject).c_str(),
					wc2s(wcsAuthor).c_str(),
					wc2s(wcsKeywords).c_str(),
					wc2s(wcsComments).c_str(),
					wc2s(wcsLastSavedBy).c_str(),
					wc2s(wcsApplication).c_str(),
					wc2s(wcsSecurity).c_str(),
					wc2s(wcsOwner).c_str(),
					wc2s(wcsCopyright).c_str(),
					wc2s(wcsTrademark).c_str(),
					wc2s(wcsNotes).c_str()
				);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>Create a new data source record and return its index.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateDataSource(long indexInsert, System::Guid idDataSourceType, System::Guid idVendor, System::Guid idEquipment,
		String^ serialNumberDS, String^ versionDS, String^ nameDS, String^ ownerDS, String^ locationDS, String^ timeZoneDS)
	{
		long	idxRecord = -1;

		if (IsInitialized())
		{
			pin_ptr<const wchar_t> wcsSerialNumberDS = PtrToStringChars(serialNumberDS);
			pin_ptr<const wchar_t> wcsVersionDS = PtrToStringChars(versionDS);
			pin_ptr<const wchar_t> wcsNameDS = PtrToStringChars(nameDS);
			pin_ptr<const wchar_t> wcsOwnerDS = PtrToStringChars(ownerDS);
			pin_ptr<const wchar_t> wcsLocationDS = PtrToStringChars(locationDS);
			pin_ptr<const wchar_t> wcsTimeZoneDS = PtrToStringChars(timeZoneDS);

			try
			{
				GUID	idDataSourceTypeLocal;
				GUID	idVendorLocal;
				GUID	idEquipmentLocal;

				//	Convert the GUIDs
				GUIDFromGuid(idDataSourceTypeLocal, idDataSourceType );
				GUIDFromGuid(idVendorLocal, idVendor );
				GUIDFromGuid(idEquipmentLocal, idEquipment );

				//	Create the record
				idxRecord = m_percont->CreateDataSourceRecord(
					indexInsert,
					idDataSourceTypeLocal,
					idVendorLocal,
					idEquipmentLocal,

					wc2s(wcsSerialNumberDS).c_str(),
					wc2s(wcsVersionDS).c_str(),
					wc2s(wcsNameDS).c_str(),
					wc2s(wcsOwnerDS).c_str(),
					wc2s(wcsLocationDS).c_str(),
					wc2s(wcsTimeZoneDS).c_str()
				);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>Create a new observation record and return its index.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateObservation(long indexInsert, String^ name, CPQDIFTimeStamp% timeCreate, CPQDIFTimeStamp% timeStart, long idTriggerMethod, CPQDIFTimeStamp% timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger)
	{
		long	idxRecord = -1;
		pin_ptr<const wchar_t> wcsName = PtrToStringChars(name);
		TIMESTAMPPQDIF	timeCreateLocal;
		TIMESTAMPPQDIF	timeStartLocal;
		TIMESTAMPPQDIF	timeTriggeredLocal;
		UINT4 * 		aidxChannelTriggerLocal = NULL;

		if (IsInitialized())
		{
			try
			{
				//	Convert the timestamps
				timeCreateLocal.day = timeCreate.day;
				timeCreateLocal.sec = timeCreate.sec;
				timeStartLocal.day = timeStart.day;
				timeStartLocal.sec = timeStart.sec;
				timeTriggeredLocal.day = timeTriggered.day;
				timeTriggeredLocal.sec = timeTriggered.sec;

				//	Convert the trigger array
				aidxChannelTriggerLocal = NewArrayFromNetLong(aidxChannelTrigger);

				//	Create the record
				idxRecord = m_percont->CreateObservationRecord(
					indexInsert,
					wc2s(wcsName).c_str(),
					&timeCreateLocal,
					&timeStartLocal,
					(UINT4) idTriggerMethod,
					&timeTriggeredLocal,
					aidxChannelTrigger->Length,
					aidxChannelTriggerLocal
				);

				//	Clean up
				if( aidxChannelTriggerLocal )
				{
					delete [] aidxChannelTriggerLocal;
				}
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>Create a new observation record and return its index.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateObservation(long indexInsert, String^ name, double timeCreate, double timeStart, long idTriggerMethod, double timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger)
	{
		long	idxRecord = -1;
		TIMESTAMPPQDIF	timeCreateLocal;
		TIMESTAMPPQDIF	timeStartLocal;
		TIMESTAMPPQDIF	timeTriggeredLocal;
		CPQDIFTimeStamp timeCreateLocalNet;
		CPQDIFTimeStamp timeStartLocalNet;
		CPQDIFTimeStamp timeTriggeredLocalNet;

		try
		{
			//	Convert from double to pqlib time stamp struct.
			theSupport.SetTimeStampFromDate( timeCreateLocal   , timeCreate    );
			theSupport.SetTimeStampFromDate( timeStartLocal    , timeStart	   );
			theSupport.SetTimeStampFromDate( timeTriggeredLocal, timeTriggered );

			// Convert to .Net time stamp class.
			timeCreateLocalNet.day = timeCreateLocal.day;
			timeCreateLocalNet.sec = timeCreateLocal.sec;
			timeStartLocalNet.day = timeStartLocal.day;
			timeStartLocalNet.sec = timeStartLocal.sec;
			timeTriggeredLocalNet.day = timeTriggeredLocal.day;
			timeTriggeredLocalNet.sec = timeTriggeredLocal.sec;

			//	Create the record
			idxRecord = RecordCreateObservation(indexInsert, name, timeCreateLocalNet, timeStartLocalNet, idTriggerMethod, timeTriggeredLocalNet, aidxChannelTrigger);
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxRecord;
	}

	/// <summary>Create a new observation record and return its index.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateObservation3(long indexInsert, String^ name, CPQDIFTimeStamp% timeCreate, CPQDIFTimeStamp% timeStart, long idTriggerMethod)
	{
		long	idxRecord = -1;
		pin_ptr<const wchar_t> wcsName = PtrToStringChars(name);
		TIMESTAMPPQDIF	timeCreateLocal;
		TIMESTAMPPQDIF	timeStartLocal;

		if (IsInitialized())
		{
			try
			{
				//	Convert the timestamps
				timeCreateLocal.day = timeCreate.day;
				timeCreateLocal.sec = timeCreate.sec;
				timeStartLocal.day = timeStart.day;
				timeStartLocal.sec = timeStart.sec;

				//	Create the record
				idxRecord = m_percont->CreateObservationRecord(
					indexInsert,
					wc2s(wcsName).c_str(),
					&timeCreateLocal,
					&timeStartLocal,
					(UINT4) idTriggerMethod,
					NULL,
					0,
					NULL
				);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>Create a new observation record and return its index.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateObservation3(long indexInsert, String^ name, DATE timeCreate, DATE timeStart, long idTriggerMethod)
	{
		long	idxRecord = -1;
		pin_ptr<const wchar_t> wcsName = PtrToStringChars(name);
		TIMESTAMPPQDIF	timeCreateLocal;
		TIMESTAMPPQDIF	timeStartLocal;
		CPQDIFTimeStamp timeCreateLocalNet;
		CPQDIFTimeStamp timeStartLocalNet;

		try
		{
			//	Convert the timestamps
			theSupport.SetTimeStampFromDate( timeCreateLocal   , timeCreate    );
			theSupport.SetTimeStampFromDate( timeStartLocal    , timeStart	   );

			// Convert to .Net time stamp class.
			timeCreateLocalNet.day = timeCreateLocal.day;
			timeCreateLocalNet.sec = timeCreateLocal.sec;
			timeStartLocalNet.day = timeStartLocal.day;
			timeStartLocalNet.sec = timeStartLocal.sec;

			//	Create the record
			idxRecord = RecordCreateObservation3(indexInsert, name, timeCreateLocalNet, timeStartLocalNet, idTriggerMethod);
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxRecord;
	}

	/// <summary>Get a data source record.
	/// <param name="index">Index of the data source record sought.</param>
	/// <param name="hRecordDS">Out param returning the handle of the data source record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordRequestDataSource(long index, System::IntPtr% hRecordDS)
	{
		bool			status = false;
		CPQDIFRecord	*precBase;
		GUID			tagRecord;

		if (IsInitialized())
		{
			try
			{
				//	Find the observation and associated data source
				precBase = m_percont->GetRecordFull( index );
				if( precBase )
				{
					//	Is this a data source?
					precBase->HeaderGetTag( tagRecord );
					if( PQDIF_IsEqualGUID( tagRecord, tagRecDataSource ) )
					{
						hRecordDS = (System::IntPtr) precBase;
						status = true;
					}
				}	//	if( found obs record )
			}
			catch (...)
			{
				;
			}
		}
		return status;
	}

	/// <summary>Release a data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordReleaseDataSource( System::IntPtr hRecordDS)
	{
		bool	status = false;

		if( ValidateDataSource( hRecordDS ) )
		{
			//	Don't have to do anything to release it
			//	(it was only a cast)
			status = true;
		}
		return status;
	}

	/// <summary>
	/// Add a new channel definition to this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="name"></param>
	/// <param name="idPhase"></param>
	/// <param name="idQuantityType"></param>
	/// <returns>Index of new channel definition.</returns>
	/// </summary>
	long CPQDIFNet::DataSourceAddChannelDefn( System::IntPtr hRecordDS, String^ name, long idPhase, System::Guid idQuantityType)
	{
		long				idxChannelDefn = -1;
		CPQDIF_R_DataSource *pds;
		GUID				idQuantityTypeLocal;

		try
		{
			pin_ptr<const wchar_t> wcsName = PtrToStringChars(name);

			//	Convert GUID
			GUIDFromGuid(idQuantityTypeLocal, idQuantityType );

			//	Create the channel def'n
			pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				idxChannelDefn = pds->AddChannelDefn(
							wc2s(wcsName).c_str(),
							(UINT4) idPhase,
							idQuantityTypeLocal);
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxChannelDefn;
	}

	/// <summary>
	/// Add a new series definition to this channel definition.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannelDefn"></param>
	/// <param name="idQuantityUnits"></param>
	/// <param name="idValueType"></param>
	/// <param name="idStorageMethod"></param>
	/// <returns>Index of new channel definition.</returns>
	/// </summary>
	long CPQDIFNet::DataSourceAddSeriesDefn( System::IntPtr hRecordDS, long idxChannelDefn, long idQuantityUnits, System::Guid idValueType, long idStorageMethod)
	{
		long				idxSeriesDefn = -1;
		CPQDIF_R_DataSource *pds;
		GUID				idValueTypeLocal;

		try
		{
			//	Convert GUID
			GUIDFromGuid(idValueTypeLocal, idValueType);

			//	Create the series def'n
			pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				idxSeriesDefn = pds->AddSeriesDefn(
							idxChannelDefn,
					(UINT4) idQuantityUnits,
							idValueTypeLocal,
					(UINT4) idStorageMethod);
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxSeriesDefn;
	}

	/// <summary>
	/// Add series data to this observation record.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="arValues">Series data to add.</param>
	/// <returns>Index of new series or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::ObservationAddSeriesDouble( System::IntPtr hRecordObs, long idxChannel, Array^ arValues)
	{
		return ObservationAddSeriesData(hRecordObs, idxChannel, arValues);
	}

	/// <summary>
	/// Add a channel to this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannelDefn">New channel.</param>
	/// <returns>Id of new channel or -1 if fails.</returns>
	/// </summary>
	long CPQDIFNet::ObservationAddChannel( System::IntPtr hRecordObserv, long idxChannelDefn)
	{
		long					idxChannel = -1;
		CPQDIF_R_Observation	*pobs;

		//	Create the channel
		pobs = ValidateObservation( hRecordObserv );
		if( pobs )
		{
			idxChannel = pobs->AddChannel( idxChannelDefn );
		}
		return idxChannel;
	}

	/// <summary>Get a settings record.
	/// <param name="index">Index of the settings record.</param>
	/// <param name="hRecordSettings">Out param returning the handle of the settings record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordRequestSettings(long index, System::IntPtr% hRecordSettings)
	{
		bool					status = false;
		CPQDIFRecord			*precBase;
		CPQDIF_R_Settings		*psett = NULL;
		GUID					tagRecord;

		if (IsInitialized())
		{
			//	Find the settings record
			precBase = m_percont->GetRecordFull( index );
			if( precBase )
			{
				//	Is this an observation?
				precBase->HeaderGetTag( tagRecord );
				if( PQDIF_IsEqualGUID( tagRecord, tagRecMonitorSettings ) )
				{
					//	All we do is cast the sucker ...
					psett = (CPQDIF_R_Settings *) precBase;
					status = true;
				}
			}	//	if( found record )

			if( status )
			{
				hRecordSettings = (System::IntPtr) psett;
			}
		}
		return status;
	}

	/// <summary>
	/// Release a settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordReleaseSettings( System::IntPtr hRecordSettings)
	{
		bool	status = false;

		if( ValidateSettings( hRecordSettings ) )
		{
			//	Don't have to do anything to release it
			//	(it was only a cast)
			status = true;
		}
		return status;
	}

	/// <summary>
	/// Set the channel transformer settings.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="xdTransformerTypeID"></param>
	/// <param name="xdSystemSideRatio"></param>
	/// <param name="xdMonitorSideRatio"></param>
	/// <param name="xdFreqResponse"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetChanTrans( System::IntPtr hRecordSettings, long idxChannel, long xdTransformerTypeID, double xdSystemSideRatio, double xdMonitorSideRatio, cli::array<double>^ xdFreqResponse)
	{
		bool				status = false;
		CPQDIF_R_Settings	*psett;
		CPQDIF_E_Vector 	*pvect = NULL;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Translate frequency response
				pvect = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				if( pvect )
				{
					SetVector( *pvect, xdFreqResponse );
				}

				//	Do it
				status = psett->SetChanTrans(
					idxChannel,
					xdTransformerTypeID,
					xdSystemSideRatio,
					xdMonitorSideRatio,
					pvect );
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}

		//	Clean up
		if( pvect )
		{
			delete pvect;
		}
		return status;
	}

	/// <summary>
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="calTimeSkew"></param>
	/// <param name="calOffset"></param>
	/// <param name="calRatio"></param>
	/// <param name="calMustUseARCal"></param>
	/// <param name="calApplied"></param>
	/// <param name="calRecorded"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetChanCal( System::IntPtr hRecordSettings, long idxChannel, double calTimeSkew, double calOffset, double calRatio, bool calMustUseARCal, cli::array<double>^ calApplied, cli::array<double>^ calRecorded)
	{
		bool				status = false;
		CPQDIF_R_Settings	*psett;
		CPQDIF_E_Vector 	*pvectApplied = NULL;
		CPQDIF_E_Vector 	*pvectRecorded = NULL;

		try
			{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
				{
				//	Translate vectors
				pvectApplied = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				if( pvectApplied )
					{
					SetVector( *pvectApplied, calApplied );
					}
				pvectRecorded = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				if( pvectRecorded )
					{
					SetVector( *pvectRecorded, calRecorded );
					}

				//	Do it
				status = psett->SetChanCal(
					idxChannel,
					calTimeSkew,
					calOffset,
					calRatio,
					calMustUseARCal == TRUE,
					pvectApplied,
					pvectRecorded );
				}
			}
		catch( ... )
			{
			// e->Delete();
			status = false;
			}

		//	Clean up
		if( pvectApplied )
		{
			delete pvectApplied;
		}
		if( pvectRecorded )
		{
			delete pvectRecorded;
		}
		return status;
	}

	/// <summary>
	/// Get the number of channels.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <returns>The number of channels.</returns>
	/// </summary>
	long CPQDIFNet::SettingsGetChanCount( System::IntPtr hRecordSettings)
	{
		long				count = 0;
		CPQDIF_R_Settings	*psett;

		psett = ValidateSettings( hRecordSettings );
		if( psett )
		{
			count = psett->GetCountChannels();
		}
		return count;
	}

	/// <summary>
	/// Add a new channel definition.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannelDefn">New channel definition.</param>
	/// <returns>Index of new channel or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::SettingsAddChannel( System::IntPtr hRecordSettings, long idxChannelDefn)
	{
		long				idxChannel = -1;
		CPQDIF_R_Settings	*psett;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Add it
				idxChannel = psett->AddChannel(idxChannelDefn);
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxChannel;
	}

	/// <summary>
	/// Add a new channel definition.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannelDefn">New channel definition.</param>
	/// <param name="idTriggerType">Trigger type.</param>
	/// <returns>Index of new channel or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::SettingsAddChannel2( System::IntPtr hRecordSettings, long idxChannelDefn, long idTriggerType)
	{
		long				idxChannel = -1;
		CPQDIF_R_Settings	*psett;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Add it
				idxChannel = psett->AddChannel(idxChannelDefn, idTriggerType);
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxChannel;
	}

	/// <summary>
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="triggerShapeParam">Array of double values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetTriggerShapeParam( System::IntPtr hRecordSettings, long idxChannel, cli::array<double>^ triggerShapeParam)
	{
		bool			status = false;
		CPQDIF_E_Vector *pvectTriggerShapeParam = NULL;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Translate triggerShapeParamLocal
				pvectTriggerShapeParam = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				if( pvectTriggerShapeParam )
				{
					SetVector( *pvectTriggerShapeParam, triggerShapeParam );
				}

				status = psett->SetTriggerShapeParam(idxChannel, pvectTriggerShapeParam );
			}
		}
		catch( ... )
		{
			// e->Delete();
		}

		//	Clean up
		if( pvectTriggerShapeParam )
		{
			delete pvectTriggerShapeParam;
		}
		return status;
	}

	//
	//
	//	Deprecated
	//
	/// TODO - Comment says this is deprecated but it is used by SettingsGetChannel2 so we need to port it.
	/// 	   Most arguments are not used.
	bool CPQDIFNet::SettingsGetChannel( System::IntPtr hRecordSettings, long idxChannel, long% idxChannelDefn,
		long% /*triggerTypeID*/,
		double% /*fullScale*/, double% /*noiseFloor*/,
		double% /*triggerLow*/,	double& /*triggerHigh*/, double% /*triggerRate*/, System::Array^% /*triggerShapeParam*/)
	{
		bool				status = false;
		UINT4				idxChannelDefnLocal;
		CPQDIF_R_Settings	*psett;

		try
		{
			if ((psett = ValidateSettings( hRecordSettings)) != NULL)
			{
					//	Do it
					status = psett->GetChannelInfo(
						idxChannel,
						idxChannelDefnLocal);

					//	Copy local stuff over
					idxChannelDefn = (long) idxChannelDefnLocal;
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get channel transformer information.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="xdTransformerTypeID"></param>
	/// <param name="xdSystemSideRatio"></param>
	/// <param name="xdMonitorSideRatio"></param>
	/// <param name="xdFreqResponse"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetChanTrans( System::IntPtr hRecordSettings, long idxChannel, long% xdTransformerTypeID, double% xdSystemSideRatio, double% xdMonitorSideRatio, System::Array^% xdFreqResponse)
	{
		bool				status = false;
		UINT4				xdTransformerTypeIDLocal;
		REAL8				xdSystemSideRatioLocal;
		REAL8				xdMonitorSideRatioLocal;
		CPQDIF_R_Settings	*psett;
		CPQDIF_E_Vector 	*pvect = NULL;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Create a vector to be filled up
				pvect = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				if( pvect )
				{
					//	Do it
					status = psett->GetChanTrans(
						idxChannel,
						xdTransformerTypeIDLocal,
						xdSystemSideRatioLocal,
						xdMonitorSideRatioLocal,
						*pvect );

					//	Copy local stuff over
					xdTransformerTypeID = static_cast<long>(xdTransformerTypeIDLocal);
					xdSystemSideRatio = static_cast<long>(xdSystemSideRatioLocal);
					xdMonitorSideRatio = static_cast<long>(xdMonitorSideRatioLocal);

					//	Translate vector
					xdFreqResponse = NewArrayFromVector( *pvect );
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}

		//	Clean up
		if( pvect )
		{
			delete pvect;
		}
		return status;
	}

	/// <summary>
	/// Get channel transformer information.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="calTimeSkew"></param>
	/// <param name="calOffset"></param>
	/// <param name="calRatio"></param>
	/// <param name="calMustUseARCal"></param>
	/// <param name="calApplied"></param>
	/// <param name="calRecorded"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetChanCal( System::IntPtr hRecordSettings, long idxChannel, double% calTimeSkew, double% calOffset, double% calRatio, bool% calMustUseARCal, System::Array^% calApplied, System::Array^% calRecorded)
	{
		bool				status = false;
		REAL8				calTimeSkewLocal;
		REAL8				calOffsetLocal;
		REAL8				calRatioLocal;
		bool				calMustUseARCalLocal;
		CPQDIF_R_Settings	*psett;
		CPQDIF_E_Vector 	*pvectApplied = NULL;
		CPQDIF_E_Vector 	*pvectRecorded = NULL;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Create vectors to be filled up
				pvectApplied = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				pvectRecorded = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				if( pvectApplied && pvectRecorded )
				{
					//	Do it
					status = psett->GetChanCal(
						idxChannel,
						calTimeSkewLocal,
						calOffsetLocal,
						calRatioLocal,
						calMustUseARCalLocal,
						*pvectApplied,
						*pvectRecorded );

					calTimeSkew = calTimeSkewLocal;
					calOffset = calOffsetLocal;
					calRatio = calRatioLocal;
					calMustUseARCal = calMustUseARCalLocal;

					//	Translate vectors
					calApplied = NewArrayFromVector( *pvectApplied );
					calRecorded = NewArrayFromVector( *pvectRecorded );
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}

		//	Clean up
		if( pvectApplied )
		{
			delete pvectApplied;
		}
		if( pvectRecorded )
		{
			delete pvectRecorded;
		}
		return status;
	}

	/// <summary>
	/// Create a new settings record.
	/// <param name="indexInsert">Index where to insert the new record.</param>
	/// <param name="timeEffective"></param>
	/// <param name="timeInstalled"></param>
	/// <param name="timeRemoved"></param>
	/// <param name="useCal"></param>
	/// <param name="useTrans"></param>
	/// <returns>Index of the new record or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateSettings(long indexInsert, CPQDIFTimeStamp% timeEffective, CPQDIFTimeStamp% timeInstalled, CPQDIFTimeStamp% timeRemoved, bool useCal, bool useTrans)
	{
		long				idxRecord = -1;
		TIMESTAMPPQDIF		timeEffectiveLocal;
		TIMESTAMPPQDIF		timeInstalledLocal;
		TIMESTAMPPQDIF		timeRemovedLocal;

		if (IsInitialized())
		{
			try
			{
				//	Translate times
				timeEffectiveLocal.day = timeEffective.day;
				timeEffectiveLocal.sec = timeEffective.sec;
				timeInstalledLocal.day = timeInstalled.day;
				timeInstalledLocal.sec = timeInstalled.sec;
				timeRemovedLocal.day = timeRemoved.day;
				timeRemovedLocal.sec = timeRemoved.sec;

				//	Create it
				idxRecord = m_percont->CreateMonitorSettingsRecord(
					indexInsert,
					&timeEffectiveLocal,
					&timeInstalledLocal,
					&timeRemovedLocal,
					useCal == true,
					useTrans == true);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>
	/// Create a new settings record.
	/// <param name="indexInsert">Index where to insert the new record.</param>
	/// <param name="timeEffective"></param>
	/// <param name="timeInstalled"></param>
	/// <param name="timeRemoved"></param>
	/// <param name="useCal"></param>
	/// <param name="useTrans"></param>
	/// <returns>Index of the new record or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateSettings(long indexInsert, double timeEffective, double timeInstalled, double timeRemoved, bool useCal, bool useTrans)
	{
		long				idxRecord = -1;
		TIMESTAMPPQDIF		timeEffectiveLocal;
		TIMESTAMPPQDIF		timeInstalledLocal;
		TIMESTAMPPQDIF		timeRemovedLocal;
		CPQDIFTimeStamp 	timeEffectiveLocalNet;
		CPQDIFTimeStamp 	timeInstalledLocalNet;
		CPQDIFTimeStamp 	timeRemovedLocalNet;

		try
		{
			//	Translate times
			theSupport.SetTimeStampFromDate( timeEffectiveLocal, timeEffective );
			theSupport.SetTimeStampFromDate( timeInstalledLocal, timeInstalled );
			theSupport.SetTimeStampFromDate( timeRemovedLocal, timeRemoved );

			// Convert to .Net time stamp class.
			timeEffectiveLocalNet.day = timeEffectiveLocal.day;
			timeEffectiveLocalNet.sec = timeEffectiveLocal.sec;
			timeInstalledLocalNet.day = timeInstalledLocal.day;
			timeInstalledLocalNet.sec = timeInstalledLocal.sec;
			timeRemovedLocalNet.day = timeRemovedLocal.day;
			timeRemovedLocalNet.sec = timeRemovedLocal.sec;

			//	Create it
			idxRecord = RecordCreateSettings(indexInsert, timeEffectiveLocalNet, timeInstalledLocalNet, timeRemovedLocalNet, useCal, useTrans);
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxRecord;
	}

	/// <summary>
	/// Get the connection type.
	/// <param name="hRecordSettings">The setttings record.</param>
	/// <param name="connectionType">Out param containing the connection type.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetConnectType( System::IntPtr hRecordSettings, unsigned long% connectionType)
	{
		bool				status = false;
		CPQDIF_R_Settings	*psett;
		UINT4				connectionTypeLocal = 0;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if ((status = psett->GetConnectionInfo(connectionTypeLocal)) == true)
				{
					connectionType = connectionTypeLocal;
				}
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}


	/// <summary>
	/// Set the connection type.
	/// <param name="hRecordSettings">The setttings record.</param>
	/// <param name="connectionType">connection type.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetConnectType( System::IntPtr hRecordSettings, unsigned long  connectionType)
	{
		bool				status = false;
		CPQDIF_R_Settings	*psett;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetConnectionInfo(connectionType);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get information for this settings record.
	/// <param name="hRecordSettings">The setttings record.</param>
	/// <param name="timeEffective">.</param>
	/// <param name="timeInstalled">.</param>
	/// <param name="timeRemoved">.</param>
	/// <param name="useCal">.</param>
	/// <param name="useTrans">.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetInfo( System::IntPtr hRecordSettings, CPQDIFTimeStamp% timeEffective, CPQDIFTimeStamp% timeInstalled, CPQDIFTimeStamp% timeRemoved, bool% useCal, bool% useTrans)
	{
		bool				status = true;
		CPQDIF_R_Settings	*psett;
		TIMESTAMPPQDIF		timeEffectiveLocal;
		TIMESTAMPPQDIF		timeInstalledLocal;
		TIMESTAMPPQDIF		timeRemovedLocal;

		try
		{
			psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Get info (duh!)
				bool b1;
				bool b2;
				status = psett->GetInfo(
						timeEffectiveLocal,
						timeInstalledLocal,
						timeRemovedLocal,
						b1,
						b2 );
				useCal = b1;
				useTrans = b2;

				// Return the dates.
				timeEffective.day = timeEffectiveLocal.day;
				timeEffective.sec = timeEffectiveLocal.sec;
				timeInstalled.day = timeInstalledLocal.day;
				timeInstalled.sec = timeInstalledLocal.sec;
				timeRemoved.day = timeRemovedLocal.day;
				timeRemoved.sec = timeRemovedLocal.sec;
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get information for this settings record.
	/// <param name="hRecordSettings">The setttings record.</param>
	/// <param name="timeEffective">.</param>
	/// <param name="timeInstalled">.</param>
	/// <param name="timeRemoved">.</param>
	/// <param name="useCal">.</param>
	/// <param name="useTrans">.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetInfo( System::IntPtr hRecordSettings, double% timeEffective, double% timeInstalled, double% timeRemoved, bool% useCal, bool% useTrans)
	{
		bool				status = true;
		CPQDIFTimeStamp 	tsEffective;
		CPQDIFTimeStamp 	tsInstalled;
		CPQDIFTimeStamp 	tsRemoved;

		try
		{
			if ((status = SettingsGetInfo(hRecordSettings, tsEffective, tsInstalled, tsRemoved, useCal, useTrans)) == true)
			{
				timeEffective = tsEffective.ToOADate();
				timeInstalled = tsInstalled.ToOADate();
				timeRemoved = tsRemoved.ToOADate();
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get information for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idDataSourceType">.</param>
	/// <param name="idVendor">.</param>
	/// <param name="idEquipment">.</param>
	/// <param name="serialNumberDS">.</param>
	/// <param name="versionDS">.</param>
	/// <param name="nameDS">.</param>
	/// <param name="ownerDS">.</param>
	/// <param name="locationDS">.</param>
	/// <param name="timeZoneDS">.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceGetInfo( System::IntPtr hRecordDS, Guid% idDataSourceType, Guid% idVendor, Guid% idEquipment, String^% serialNumberDS, String^% versionDS, String^% nameDS,
		String^% ownerDS, String^% locationDS, String^% timeZoneDS)
	{
		bool				status = false;
		CPQDIF_R_DataSource *pds;
		GUID				idDataSourceTypeLocal;
		GUID				idVendorLocal;
		GUID				idEquipmentLocal;
		string				serialNumberDSLocal;
		string				versionDSLocal;
		string				nameDSLocal;
		string				ownerDSLocal;
		string				locationDSLocal;
		string				timeZoneDSLocal;
		wstring 			wsTemp;

		try
		{
			//	Get the info
			pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				status = pds->GetInfo(
							idDataSourceTypeLocal,
							idVendorLocal,
							idEquipmentLocal,
							serialNumberDSLocal,
							versionDSLocal,
							nameDSLocal,
							ownerDSLocal,
							locationDSLocal,
							timeZoneDSLocal );

				if( status )
				{
					// Copy the GUID to a System::Guid.
					idDataSourceType = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idDataSourceTypeLocal, System::Guid::typeid);
					idVendor = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idVendorLocal, System::Guid::typeid);
					idEquipment = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idEquipmentLocal, System::Guid::typeid);

					wsTemp = c2ws(serialNumberDSLocal.c_str());
					serialNumberDS = gcnew String(wsTemp.c_str());

					wsTemp = c2ws(versionDSLocal.c_str());
					versionDS = gcnew String(wsTemp.c_str());

					wsTemp = c2ws(nameDSLocal.c_str());
					nameDS = gcnew String(wsTemp.c_str());

					wsTemp = c2ws(ownerDSLocal.c_str());
					ownerDS = gcnew String(wsTemp.c_str());

					wsTemp = c2ws(locationDSLocal.c_str());
					locationDS = gcnew String(wsTemp.c_str());

					wsTemp = c2ws(timeZoneDSLocal.c_str());
					timeZoneDS = gcnew String(wsTemp.c_str());
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Return the name of the GUID tag specified by index.
	/// <param name="index">Index of GUID.</param>
	/// <returns>Name of the GUID or nullptr if invalid.</returns>
	/// </summary>
	String^ CPQDIFNet::get_TagName(long index)
	{
		bool	status;
		string	sTagName;
		wstring wsTagName;
		String^ strTagName = nullptr;

		//	Get the name
		if ((status = theInfo.GetTagName( index, sTagName )) == true)
		{
			wsTagName = c2ws(sTagName.c_str());
			strTagName = gcnew String(wsTagName.c_str());
		}
		return strTagName;
	}

	/// <summary>
	/// Set the name of the GUID tag.
	/// This is a read-only property so this method is a no-op.
	/// TODO Why is this method defined?
	/// <param name="index">Index of GUID.</param>
	/// <param name="newVal">New value for GUID name.</param>
	/// <returns>True.</returns>
	/// </summary>
	bool CPQDIFNet::put_TagName(long index, String^ newVal)
	{
		//	Read-only
		index = index;
		newVal = newVal;

		return true;
	}

	/// <summary>
	/// Return the number of records in this collection.
	/// <param name="hCollection">The collection record.</param>
	/// <param name="count">Out param that returns the count of records in this collection.</param>
	/// <returns>True if successful; false if invalid collection.</returns>
	/// </summary>
	bool CPQDIFNet::CollectionGetCount( System::IntPtr hCollection, long% count)
	{
		bool				status = false;
		CPQDIF_E_Collection *pcoll = (CPQDIF_E_Collection *)hCollection.ToPointer();

		if( pcoll )
		{
			ASSERT_VALID( pcoll );

			count = pcoll->GetCount();
			status = true;
		}
		return status;
	}

	/// <summary>
	/// Return the handle for the specified element in this collection.
	/// <param name="hCollection">The collection.</param>
	/// <param name="index">Index of element sought.</param>
	/// <param name="hElement">Out param that returns a handle to the element.</param>
	/// <returns>True if successful; false if invalid collection or index.</returns>
	/// </summary>
	bool CPQDIFNet::CollectionGetEntry( System::IntPtr hCollection, long index, System::IntPtr% hElement)
	{
		bool				status = false;
		CPQDIF_E_Collection *pcoll = (CPQDIF_E_Collection *) hCollection.ToPointer();
		CPQDIF_Element		*pel;
		long				count;

		if( pcoll )
		{
			ASSERT_VALID( pcoll );

			count = pcoll->GetCount();
			if( index < count )
			{
				pel = pcoll->GetElement( index );
				if( pel )
				{
					hElement = (System::IntPtr) pel;
					status = true;
				}
			}
		}
		return status;
	}

	/// <summary>
	/// Get information for the record specified by index.
	/// <param name="index">The index of the record whose information is sought.</param>
	/// <param name="tagRecordType">Out param that returns the GUID for this record type.</param>
	/// <param name="nameRecordType">Name of the record type.</param>
	/// <param name="sizeHeader">Out param that returns the size of the recored header.</param>
	/// <param name="sizeRecord">Out param that returns the size of the record.</param>
	/// <param name="posThisRecord">Out param that returns the offset of this record in the file.</param>
	/// <param name="posNextRecord">Out param that returns the offset of the next record in the file.</param>
	/// <returns>True if successful; false if not or if we have not been initialized.</returns>
	/// </summary>
	bool CPQDIFNet::RecordGetInfo2(long index, Guid% tagRecordType, String^% nameRecordType, long% sizeHeader, long% sizeRecord, long% posThisRecord, long% posNextRecord)
	{
		return RecordGetInfo( index, tagRecordType, nameRecordType, sizeHeader, sizeRecord, posThisRecord, posNextRecord);
	}

	/// <summary>Create a new record in the container and return its index.
	/// Parameters are self explanatory.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateContainer2(String^ language, String^ title, String^ subject, String^ author, String^ keywords, String^ comments, String^ lastSavedBy, String^ application, String^ security, String^ owner, String^ copyright, String^ trademark, String^ notes)
	{
		return RecordCreateContainer( language, title, subject, author, keywords, comments, lastSavedBy, application, security, owner, copyright, trademark, notes);
	}

	/// <summary>Create a new record in the container and return its index.
	/// Parameters are self explanatory.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateContainer3(String^ fileName, CPQDIFTimeStamp% dtCreate, long lMajor, long lMinor, long lCompatMajor, long lCompatMinor)
	{
		long			idxRecord = -1;
		TIMESTAMPPQDIF	timeCreateLocal;

		if (IsInitialized())
		{
			pin_ptr<const wchar_t> wcsFileName = PtrToStringChars(fileName);
			try
			{
				//	Convert the timestamps
				timeCreateLocal.day = dtCreate.day;
				timeCreateLocal.sec = dtCreate.sec;

				idxRecord = m_percont->CreateContainerRecord(
					wc2s(wcsFileName).c_str(), &timeCreateLocal, lMajor, lMinor, lCompatMajor, lCompatMinor);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>Create a new record in the container and return its index.
	/// Parameters are self explanatory.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateContainer3(String^ fileName, double dtCreate, long lMajor, long lMinor, long lCompatMajor, long lCompatMinor)
	{
		long			idxRecord = -1;
		TIMESTAMPPQDIF	timeCreateLocal;
		CPQDIFTimeStamp timeCreateLocalNet;

		if (IsInitialized())
		{
			pin_ptr<const wchar_t> wcsFileName = PtrToStringChars(fileName);
			try
			{
				//	Convert the timestamps
				theSupport.SetTimeStampFromDate( timeCreateLocal, dtCreate );
				timeCreateLocalNet.day = timeCreateLocal.day;
				timeCreateLocalNet.sec = timeCreateLocal.sec;

				idxRecord = RecordCreateContainer3(fileName, timeCreateLocalNet, lMajor, lMinor, lCompatMajor, lCompatMinor);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>Set the data for a container recored.
	/// Parameters are self explanatory.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	bool CPQDIFNet::ContainerSetInfo(String^ language, String^ title, String^ subject, String^ author, String^ keywords, String^ comments, String^ lastSavedBy, String^ application, String^ security, String^ owner, String^ copyright, String^ trademark, String^ notes)
	{
		bool				status = false;
		CPQDIFRecord		*precBase;
		CPQDIF_R_Container	*pc = NULL;
		GUID				tagRecord;

		if (IsInitialized())
		{
			pin_ptr<const wchar_t> wcsLanguage = PtrToStringChars(language);
			pin_ptr<const wchar_t> wcsTitle = PtrToStringChars(title);
			pin_ptr<const wchar_t> wcsSubject = PtrToStringChars(subject);
			pin_ptr<const wchar_t> wcsAuthor = PtrToStringChars(author);
			pin_ptr<const wchar_t> wcsKeywords = PtrToStringChars(keywords);
			pin_ptr<const wchar_t> wcsComments = PtrToStringChars(comments);
			pin_ptr<const wchar_t> wcsLastSavedBy = PtrToStringChars(lastSavedBy);
			pin_ptr<const wchar_t> wcsApplication = PtrToStringChars(application);
			pin_ptr<const wchar_t> wcsSecurity = PtrToStringChars(security);
			pin_ptr<const wchar_t> wcsOwner = PtrToStringChars(owner);
			pin_ptr<const wchar_t> wcsCopyright = PtrToStringChars(copyright);
			pin_ptr<const wchar_t> wcsTrademark = PtrToStringChars(trademark);
			pin_ptr<const wchar_t> wcsNotes = PtrToStringChars(notes);

			try
			{
				//	Find the observation and associated data source
				precBase = m_percont->GetRecord( 0 );
				if( precBase )
				{
					//	Is this a container?
					precBase->HeaderGetTag( tagRecord );
					if( PQDIF_IsEqualGUID( tagRecord, tagContainer ) )
					{
						//	All we do is cast the sucker ...
						pc = (CPQDIF_R_Container *) precBase;

						status = pc->SetInfo(
							wc2s(wcsLanguage).c_str(),
							wc2s(wcsTitle).c_str(),
							wc2s(wcsSubject).c_str(),
							wc2s(wcsAuthor).c_str(),
							wc2s(wcsKeywords).c_str(),
							wc2s(wcsComments).c_str(),
							wc2s(wcsLastSavedBy).c_str(),
							wc2s(wcsApplication).c_str(),
							wc2s(wcsSecurity).c_str(),
							wc2s(wcsOwner).c_str(),
							wc2s(wcsCopyright).c_str(),
							wc2s(wcsTrademark).c_str(),
							wc2s(wcsNotes).c_str()
							);
					}
				}
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return status;
	}

	/// <summary>Create a new data source record and return its index.
	/// Parameters are self explanatory.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateDataSource2(long indexInsert, System::Guid idDataSourceType, System::Guid idVendor, System::Guid idEquipment, String^ serialNumberDS, String^ versionDS, String^ nameDS, String^ ownerDS, String^ locationDS, String^ timeZoneDS)
	{
		long	idxRecord = -1;

		if (IsInitialized())
		{
			pin_ptr<const wchar_t> wcsSerialNumberDS = PtrToStringChars(serialNumberDS);
			pin_ptr<const wchar_t> wcsVersionDS = PtrToStringChars(versionDS);
			pin_ptr<const wchar_t> wcsNameDS = PtrToStringChars(nameDS);
			pin_ptr<const wchar_t> wcsOwnerDS = PtrToStringChars(ownerDS);
			pin_ptr<const wchar_t> wcsLocationDS = PtrToStringChars(locationDS);
			pin_ptr<const wchar_t> wcsTimeZoneDS = PtrToStringChars(timeZoneDS);

			try
			{
				GUID	idDataSourceTypeLocal;
				GUID	idVendorLocal;
				GUID	idEquipmentLocal;

				//	Convert the GUIDs
				GUIDFromGuid(idDataSourceTypeLocal, idDataSourceType );
				GUIDFromGuid(idVendorLocal, idVendor );
				GUIDFromGuid(idEquipmentLocal, idEquipment );

				//	Create the record
				idxRecord = m_percont->CreateDataSourceRecord(
					indexInsert,
					idDataSourceTypeLocal,
					idVendorLocal,
					idEquipmentLocal,

					wc2s(wcsSerialNumberDS).c_str(),
					wc2s(wcsVersionDS).c_str(),
					wc2s(wcsNameDS).c_str(),
					wc2s(wcsOwnerDS).c_str(),
					wc2s(wcsLocationDS).c_str(),
					wc2s(wcsTimeZoneDS).c_str()
					);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>
	/// Add a new channel definition to the data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="name"></param>
	/// <param name="idPhase"></param>
	/// <param name="idQuantityType"></param>
	/// <returns>Index of new channel definition.</returns>
	/// </summary>
	long CPQDIFNet::DataSourceAddChannelDefn2( System::IntPtr hRecordDS, String^ name, long idPhase, System::Guid idQuantityType)
	{
		return DataSourceAddChannelDefn( hRecordDS, name, idPhase, idQuantityType);
	}

	/// <summary>
	/// Add a new channel definition to the data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="name"></param>
	/// <param name="idPhase"></param>
	/// <param name="idQM"></param>
	/// <param name="idQuantityType"></param>
	/// <returns>Index of new channel definition.</returns>
	/// </summary>
	long CPQDIFNet::DataSourceAddChannelDefn3( System::IntPtr hRecordDS, String^ name, long idPhase, long idQM, System::Guid idQuantityType)
	{
		long				idxChannelDefn = -1;
		CPQDIF_R_DataSource *pds;
		GUID				idQuantityTypeLocal;

		try
		{
			pin_ptr<const wchar_t> wcsName = PtrToStringChars(name);

			//	Convert GUID
			GUIDFromGuid(idQuantityTypeLocal, idQuantityType);

			//	Create the channel def'n
			pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				idxChannelDefn = pds->AddChannelDefn2(
							wc2s(wcsName).c_str(),
					(UINT4) idPhase,
					(UINT4) idQM,
							idQuantityTypeLocal);
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxChannelDefn;
	}

	/// <summary>
	/// Get information for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idDataSourceType">.</param>
	/// <param name="idVendor">.</param>
	/// <param name="idEquipment">.</param>
	/// <param name="serialNumberDS">.</param>
	/// <param name="versionDS">.</param>
	/// <param name="nameDS">.</param>
	/// <param name="ownerDS">.</param>
	/// <param name="locationDS">.</param>
	/// <param name="timeZoneDS">.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceGetInfo2( System::IntPtr hRecordDS, System::Guid% idDataSourceType, System::Guid% idVendor, System::Guid% idEquipment, String^% serialNumberDS, String^% versionDS, String^% nameDS,
		String^% ownerDS, String^% locationDS, String^% timeZoneDS)
	{
		return DataSourceGetInfo(hRecordDS, idDataSourceType, idVendor, idEquipment, serialNumberDS, versionDS, nameDS, ownerDS, locationDS, timeZoneDS);
	}

	/// <summary>Create a new observation record and return its index.
	/// Parameters are self explanatory.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateObservation2(long indexInsert, String^ name, CPQDIFTimeStamp% timeCreate, CPQDIFTimeStamp% timeStart, long idTriggerMethod, CPQDIFTimeStamp% timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger)
	{
		return RecordCreateObservation(indexInsert, name, timeCreate, timeStart, idTriggerMethod, timeTriggered, aidxChannelTrigger);
	}

	/// <summary>Create a new observation record and return its index.
	/// Parameters are self explanatory.
	/// <returns>Index of newly created record.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateObservation2(long indexInsert, String^ name, double timeCreate, double timeStart, long idTriggerMethod, double timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger)
	{
		return RecordCreateObservation(indexInsert, name, timeCreate, timeStart, idTriggerMethod, timeTriggered, aidxChannelTrigger);
	}

	/// <summary>Get information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="timeStart">Out param that returns the start time.</param>
	/// <param name="name">Out param that returns the name for this record.</param>
	/// <param name="countChannels">Out param that returns the number of channels.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetInfo2( System::IntPtr hRecordObserv, CPQDIFTimeStamp% timeStart, String^% name, long% countChannels)
	{
		return ObservationGetInfo(hRecordObserv, timeStart, name, countChannels);
	}

	/// <summary>Get information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="timeStart">Out param that returns the start time.</param>
	/// <param name="name">Out param that returns the name for this record.</param>
	/// <param name="countChannels">Out param that returns the number of channels.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetInfo2( System::IntPtr hRecordObserv, double% timeStart, String^% name, long% countChannels)
	{
		return ObservationGetInfo(hRecordObserv, timeStart, name, countChannels);
	}

	/// <summary>Get information for this channel in this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Out param that returns the index of the channel.</param>
	/// <param name="name">Out param that returns the name for this channel.</param>
	/// <param name="idPhase"></param>
	/// <param name="idQuantityType"></param>
	/// <param name="countSeries">Out param that returns the number of series values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetChannelInfo2( System::IntPtr hRecordObserv, long idxChannel, String^% name, long% idPhase, Guid %idQuantityType, long% countSeries)
	{
		return ObservationGetChannelInfo(hRecordObserv, idxChannel, name, idPhase, idQuantityType, countSeries);
	}

	/// <summary>Get information for this channel in this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Out param that returns the index of the channel.</param>
	/// <param name="name">Out param that returns the name for this channel.</param>
	/// <param name="idPhase"></param>
	/// <param name="idQuantityType"></param>
	/// <param name="countSeries">Out param that returns the number of series values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetChannelInfo3( System::IntPtr hRecordObserv, long idxChannel, String^% name, long% idPhase, long% idQuantityMeasured, Guid %idQuantityType, long% countSeries)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		string					nameLocal;
		UINT4					idPhaseLocal;
		GUID					idQuantityTypeLocal;
		UINT4					idQuantityMeasuredLocal;
		wstring 				wNameLocal;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				//	Make sure we're in the right range.
				if( idxChannel >= 0 && idxChannel < pobs->GetCountChannels() )
				{
					countSeries = pobs->GetCountSeries( static_cast<long>(idxChannel) );

					//	Get full channel info
					status = pobs->GetChannelInfo( static_cast<long>(idxChannel), nameLocal, idPhaseLocal, idQuantityTypeLocal, idQuantityMeasuredLocal );
					if( status )
					{
						wNameLocal = c2ws(nameLocal.c_str());
						name = gcnew String(wNameLocal.c_str());

						idPhase = (long) idPhaseLocal;
						idQuantityMeasured = (long) idQuantityMeasuredLocal;

						// Copy the GUID to a System::Guid.
						idQuantityType = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idQuantityTypeLocal, System::Guid::typeid);
					}
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get series extended data from this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="tag">Guid identifying series extended data.</param>
	/// <param name="value">Out param that returns the data as a System::Object.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesExtendedData( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Guid tag, System::Object^% value)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		long					typePhysical;
		PQDIFValue				theValue;
		GUID					tagLocal;

		try
		{
			// Convert from System::Guid to our GUID structure.
			GUIDFromGuid(tagLocal, tag);

			pobs = ValidateObservation( hRecordObserv );

			if( pobs )
			{
				if ( pobs->GetSeriesExtendedData(idxChannel, idxSeries, tagLocal, typePhysical, theValue))
				{
					value = ConvertScalar(typePhysical, theValue);
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get channel extended data from this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="tag">Guid identifying channel extended data.</param>
	/// <param name="value">Out param that returns the data as a System::Object.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetChannelExtendedData( System::IntPtr hRecordObserv, long idxChannel, Guid tag, System::Object^% value)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		long					typePhysical;
		PQDIFValue				theValue;
		GUID					idTagLocal;

		try
		{
			// Convert from System::Guid to our GUID structure.
			GUIDFromGuid(idTagLocal, tag);

			pobs = ValidateObservation(hRecordObserv);

			if (pobs)
			{
				if (pobs->GetChannelExtendedData(idxChannel, idTagLocal, typePhysical, theValue))
				{
					value = ConvertScalar(typePhysical, theValue);
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get the index of the channel definition.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="lChanDefn">Out param that returns the channel definition index.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetChannelDefnIdx( System::IntPtr hRecordObserv, long idxChannel, long% lChanDefn)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		long					idxChannelDefn;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if ((status = pobs->GetChannelDefnIdx( idxChannel, idxChannelDefn )) == true)
			{
				lChanDefn = idxChannelDefn;
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get extended data from this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="tag">Guid identifying observation extended data.</param>
	/// <param name="vValue">Out param that returns the data as a System::Object.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetExtendedData( System::IntPtr hRecordObserv, System::Guid tag, System::Object^% vValue)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		long					typePhysical;
		PQDIFValue				theValue;
		GUID					idTagLocal;

		try
		{
			// Convert from System::Guid to our GUID structure.
			GUIDFromGuid(idTagLocal, tag);

			pobs = ValidateObservation( hRecordObserv );

			if( pobs )
			{

				if (pobs->GetObservationExtendedData(idTagLocal, typePhysical, theValue))
				{
					vValue = ConvertScalar(typePhysical, theValue);
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get this element's Guid.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="tagElement">Out param that takes the Guid for this element type.</param>
	/// <param name="nameTagElement">Out param that takes the name for this element type.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ElementGetTag2(System::IntPtr hElement, Guid% tagElement, String^% nameTagElement)
	{
		return ElementGetTag(hElement, tagElement, nameTagElement);
	}

	/// <summary>
	/// Retrieve the value of this string element.
	/// <param name="hElement">Handle to the element.</param>
	/// <param name="sValue">Out param that returns the string.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::VectorGetString2( System::IntPtr hElement, String^% sValue)
	{
		return ElementGetString(hElement, sValue);
	}

	/// <summary>
	/// Set the value of the vector to a String value.
	/// <param name="hVector">Handle to the Vector.</param>
	/// <param name="value">New String value.</param>
	/// <returns>True if successful; false if scalar is not valid.</returns>
	/// </summary>
	bool CPQDIFNet::VectorSetString2( System::IntPtr hVector, String^ value)
	{
		return VectorSetString(hVector, value);
	}

	/// <summary>
	/// Add series data to this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="arValues">Series data to add.</param>
	/// <returns>Index of new series or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::ObservationAddSeriesData( System::IntPtr hRecordObserv, long idxChannel, Array^ arValues)
	{
		long					idxSeries = -1;
		bool					isConverted = false;
		CPQDIF_R_Observation	*pobs;
		CPQDIF_E_Vector 		*pvect;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				//	Create vector
				pvect = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
				if( pvect )
				{
					//	Convert array -- any type
					isConverted = SetVector(*pvect, arValues);

					if( isConverted )
					{
						//	Create the series
						idxSeries = pobs->AddSeriesVector(
							idxChannel,
							pvect );
					}
					else
					{
						//	Couldn't convert - destroy
						delete pvect;
					}
				}	//	Created vector
			}	//	Got a valid observation
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxSeries;
	}

	/// <summary>
	/// Add series data to this observation record.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxChannelShared"></param>
	/// <param name="idxSeriesShared"></param>
	/// <returns>Index of new series or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::ObservationAddSeriesShared( System::IntPtr hRecordObs, long idxChannel, long idxChannelShared, long idxSeriesShared)
	{
		long					idxSeries = -1;
		CPQDIF_R_Observation	*pobs;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				idxSeries = pobs->AddSeriesShared( idxChannel, idxChannelShared, idxSeriesShared );
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxSeries;
	}

	/// <summary>
	/// Set the channel frequency.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="freq">New frequency value.</param>
	/// <returns>True if successful; false if fail.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationSetChannelFreq( System::IntPtr hRecordObs, long idxChannel, double freq)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				CPQDIF_E_Collection * pcol = pobs->GetOneChannel( idxChannel );

				if( pcol )
				{
					CPQDIF_E_Scalar * psc = pobs->FindScalarInCollection( pcol, tagChannelFrequency );
					if( !psc )
					{
						//
						//
						//	Create it !
						//
						psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
						if( psc )
						{
							pcol->Add( psc );
						}
					}
					//
					//
					//	Init the scalar
					//
					if( psc )
					{
						psc->SetTag( tagChannelFrequency );
						psc->SetValueREAL8( freq );
						status = true;
					}
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>Get the frequency value for this channel in this observation record.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="freq">Out param that returns the frequency.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetChannelFreq( System::IntPtr hRecordObs, long idxChannel, double% freq)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				CPQDIF_E_Collection * pcol = pobs->GetOneChannel( idxChannel );

				if( pcol )
				{
					CPQDIF_E_Scalar * psc = pobs->FindScalarInCollection( pcol, tagChannelFrequency );
					if( psc )
					{
						double val;
						psc->GetValueREAL8( val );
						freq = val;
						status = true;
					}
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>
	/// Set the channel harmonic ID.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="harmonic">New harmonic ID value.</param>
	/// <returns>True if successful; false if fail.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationSetChannelHarmonicID( System::IntPtr hRecordObs, long idxChannel, int harmonic )
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				CPQDIF_E_Collection * pcol = pobs->GetOneChannel( idxChannel );

				if( pcol )
				{
					CPQDIF_E_Scalar * psc = pobs->FindScalarInCollection( pcol, tagChannelGroupID );
					if( !psc )
					{
						//
						//
						//	Create it !
						//
						psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR );
						if( psc )
						{
							pcol->Add( psc );
						}
					}
					//
					//
					//	Init the scalar
					//
					if( psc )
					{
						psc->SetTag( tagChannelGroupID );
						psc->SetValueINT2( (INT2)harmonic );
						status = true;
					}
				}
			}
		}
		catch( ... )
		{
		}
		return status;
	}

	/// <summary>Get the harmonic ID value for this channel in this observation record.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="harmonic">Out param that returns the harmonic ID value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetChannelHarmonicID( System::IntPtr hRecordObs, long idxChannel, int% harmonic )
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				CPQDIF_E_Collection * pcol = pobs->GetOneChannel( idxChannel );

				if( pcol )
				{
					CPQDIF_E_Scalar * psc = pobs->FindScalarInCollection( pcol, tagChannelGroupID );
					if( psc )
					{
						INT2 i2Value;
						INT4 i4Value;
						if( psc->GetValueINT2( i2Value ) )
						{
							harmonic = (int)i2Value;
							status = true;
						}
						else if( psc->GetValueINT4( i4Value ) )
						{
							harmonic = (int)i4Value;
							status = true;
						}
					}
				}
			}
		}
		catch( ... )
		{
		}
		return status;
	}

	/// <summary>
	/// Set series scale value.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="scale"></param>
	/// <param name="offset"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationSetSeriesScale( System::IntPtr hRecordObs, long idxChannel, long idxSeries, double scale, double offset)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				status = pobs->SetSeriesScale( idxChannel, idxSeries, scale, offset );
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>
	/// Get series scale value.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="scale"></param>
	/// <param name="offset"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesScale( System::IntPtr hRecordObs, long idxChannel, long idxSeries, double% scale, double% offset)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		double					scaleLocal;
		double					offsetLocal;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				if ((status = pobs->GetSeriesScale( idxChannel, idxSeries, scaleLocal, offsetLocal )) == true)
				{
					scale = scaleLocal;
					offset = offsetLocal;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>
	/// Get the series data for this observation record.
	/// <param name="hRecordObs">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="arValues">Out param that returns array of series values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesRawData( System::IntPtr hRecordObs, long idxChannel, long idxSeries, Array^% arValues)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		CPQDIF_E_Vector 		*pvect;

		try
		{
			pobs = ValidateObservation( hRecordObs );
			if( pobs )
			{
				//	Convert the vector data into a variant
				pvect = pobs->GetSeriesValueVector( idxChannel, idxSeries );
				if( pvect )
				{
					arValues = NewArrayFromVector(*pvect);
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>
	/// Get the data for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="arValues">Out param that returns array of series values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesResolvedData( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Array^% arValues)
	{
		return ObservationGetSeriesData(hRecordObserv, idxChannel, idxSeries, arValues);
	}

	/// <summary>
	/// Get the data for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="arValues">Out param that returns array of series values.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesResolvedTimeStamp( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Array^% arValues)
	{
		return ObservationGetSeriesData(hRecordObserv, idxChannel, idxSeries, arValues);
	}

	/// <summary>
	/// Add a new series definition to this channel definition.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannelDefn"></param>
	/// <param name="idQuantityUnits"></param>
	/// <param name="idValueType"></param>
	/// <param name="idCharacteristicType"></param>
	/// <param name="idStorageMethod"></param>
	/// <returns>Index of new series definition.</returns>
	/// </summary>
	long CPQDIFNet::DataSourceAddSeriesDefn2( System::IntPtr hRecordDS, long idxChannelDefn, long idQuantityUnits, System::Guid idValueType, System::Guid idCharacteristicType, long idStorageMethod)
	{
		long				idxSeriesDefn = -1;
		CPQDIF_R_DataSource *pds;
		GUID				idValueTypeLocal;
		GUID				idCharacteristicTypeLocal;

		try
		{
			//	Convert GUIDs
			GUIDFromGuid(idValueTypeLocal, idValueType );
			GUIDFromGuid(idCharacteristicTypeLocal, idCharacteristicType );

			//	Create the series def'n
			pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				idxSeriesDefn = pds->AddSeriesDefn2(
							idxChannelDefn,
					(UINT4) idQuantityUnits,
							idValueTypeLocal,
							idCharacteristicTypeLocal,
					(UINT4) idStorageMethod );
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return idxSeriesDefn;
	}

	/// <summary>
	/// Return the number of records in this file.
	/// <param name="count">Out param that returns the number of records.</param>
	/// <returns>True if successful; False if fail.</returns>
	/// </summary>
	bool CPQDIFNet::RecordGetCount2(long% count)
	{
		long	countLocal;

		countLocal = RecordGetCount();
		count = countLocal;
		return true;
	}

	/// <summary>
	/// Remove and delete the record at index.
	/// <param name="index">Index of record to delete.</param>
	/// <returns>True if successful; False if index is invalid or we are not initialized.</returns>
	/// </summary>
	bool CPQDIFNet::RecordDestroy2(long index)
	{
		return RecordDestroy(index);
	}

	/// <summary>
	/// Get a data source record.
	/// <param name="index">Index of the data source record sought.</param>
	/// <param name="hRecordDS">Out param returning the handle of the data source record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordRequestDataSource2(long index, System::IntPtr% hRecordDS)
	{
		return RecordRequestDataSource(index, hRecordDS);
	}

	/// <summary>
	/// Release a data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordReleaseDataSource2( System::IntPtr hRecordDS)
	{
		return RecordReleaseDataSource(hRecordDS);
	}

	/// <summary>
	/// Create a new settings record.
	/// <param name="indexInsert">Index where to insert the new record.</param>
	/// <returns>Index of the new record or -1 if fail.</returns>
	/// </summary>
	long CPQDIFNet::RecordCreateSettings2(long indexInsert)
	{
		long	idxRecord = -1;

		if (IsInitialized())
		{
			try
			{
				//	Create it
				idxRecord = m_percont->CreateMonitorSettingsRecord(indexInsert);
			}
			catch( ... )
			{
				// e->Delete();
			}
		}
		return idxRecord;
	}

	/// <summary>
	/// Get a settings record.
	/// <param name="idxRecord">Index of the settings record sought.</param>
	/// <param name="hRecordSettings">Out param returning the handle of the settings record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordRequestSettings2(long idxRecord, System::IntPtr% hRecordSettings)
	{
		return RecordRequestSettings(idxRecord, hRecordSettings);
	}

	/// <summary>
	/// Release this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordReleaseSettings2( System::IntPtr hRecordSettings)
	{
		return RecordReleaseSettings(hRecordSettings);
	}

	/// <summary>Get an observation record specified by index.
	/// <param name="index">Record to get.</param>
	/// <param name="hRecordObserv">Out param that returns a handle to the record.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordRequestObservation2(long index, System::IntPtr% hRecordObserv)
	{
		return RecordRequestObservation(index, hRecordObserv);
	}

	/// <summary>Free the specified observation record.
	/// <param name="hRecordObserv">The record to free.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::RecordReleaseObservation2( System::IntPtr hRecordObserv)
	{
		return RecordReleaseObservation(hRecordObserv);
	}

	/// <summary>
	/// Get information for this settings record.
	/// <param name="hRecordSettings">The setttings record.</param>
	/// <param name="timeEffective">.</param>
	/// <param name="timeInstalled">.</param>
	/// <param name="timeRemoved">.</param>
	/// <param name="useCal">.</param>
	/// <param name="useTrans">.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetInfo2( System::IntPtr hRecordSettings, double% timeEffective, double% timeInstalled, double% timeRemoved, bool% useCal, bool% useTrans)
	{
		return SettingsGetInfo(hRecordSettings, timeEffective, timeInstalled, timeRemoved, useCal, useTrans);
	}

	/// <summary>
	/// Set the channel transformer settings.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="xdTransformerTypeID"></param>
	/// <param name="xdSystemSideRatio"></param>
	/// <param name="xdMonitorSideRatio"></param>
	/// <param name="xdFreqResponse"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetChanTrans2( System::IntPtr hRecordSettings, long idxChannel, long xdTransformerTypeID, double xdSystemSideRatio, double xdMonitorSideRatio, cli::array<double>^ xdFreqResponse)
	{
		return SettingsSetChanTrans(hRecordSettings, idxChannel, xdTransformerTypeID, xdSystemSideRatio, xdMonitorSideRatio, xdFreqResponse);
	}

	/// <summary>
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="calTimeSkew"></param>
	/// <param name="calOffset"></param>
	/// <param name="calRatio"></param>
	/// <param name="calMustUseARCal"></param>
	/// <param name="calApplied"></param>
	/// <param name="calRecorded"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetChanCal2( System::IntPtr hRecordSettings, long idxChannel, double calTimeSkew, double calOffset, double calRatio, bool calMustUseARCal, cli::array<double>^ calApplied, cli::array<double>^ calRecorded)
	{
		return SettingsSetChanCal(hRecordSettings, idxChannel, calTimeSkew, calOffset, calRatio, calMustUseARCal, calApplied, calRecorded);
	}

	/// <summary>
	/// Get the channel count.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <returns>The number of channels.</returns>
	/// </summary>
	long CPQDIFNet::SettingsGetChanCount2( System::IntPtr hRecordSettings)
	{
		return SettingsGetChanCount(hRecordSettings);
	}

	/// TODO - SettingsGetChannel is deprecated.
	bool CPQDIFNet::SettingsGetChannel2( System::IntPtr hRecordSettings, long idxChannel, long% idxChannelDefn, long% triggerTypeID, double% fullScale, double% noiseFloor, double% triggerLow,
				double& triggerHigh, double% triggerRate, System::Array^% triggerShapeParam)
	{
		return SettingsGetChannel(hRecordSettings, idxChannel, idxChannelDefn, triggerTypeID, fullScale, noiseFloor, triggerLow, triggerHigh, triggerRate, triggerShapeParam);
	}

	/// <summary>
	/// Get channel transformer information.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="xdTransformerTypeID"></param>
	/// <param name="xdSystemSideRatio"></param>
	/// <param name="xdMonitorSideRatio"></param>
	/// <param name="xdFreqResponse"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetChanTrans2( System::IntPtr hRecordSettings, long idxChannel, long% xdTransformerTypeID, double% xdSystemSideRatio, double% xdMonitorSideRatio, System::Array^% xdFreqResponse)
	{
		return SettingsGetChanTrans(hRecordSettings, idxChannel, xdTransformerTypeID, xdSystemSideRatio, xdMonitorSideRatio, xdFreqResponse);
	}

	/// <summary>
	/// Get channel transformer information.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel"></param>
	/// <param name="calTimeSkew"></param>
	/// <param name="calOffset"></param>
	/// <param name="calRatio"></param>
	/// <param name="calMustUseARCal"></param>
	/// <param name="calApplied"></param>
	/// <param name="calRecorded"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetChanCal2( System::IntPtr hRecordSettings, long idxChannel, double% calTimeSkew, double% calOffset, double% calRatio, bool% calMustUseARCal, System::Array^% calApplied, System::Array^% calRecorded)
	{
		return SettingsGetChanCal(hRecordSettings, idxChannel, calTimeSkew, calOffset, calRatio, calMustUseARCal, calApplied, calRecorded);
	}

	/// <summary>
	/// Add a channel to this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannelDefn">New channel.</param>
	/// <returns>Id of new channel or -1 if fails.</returns>
	/// </summary>
	long CPQDIFNet::ObservationAddChannel2( System::IntPtr hRecordObserv, long idxChannelDefn)
	{
		return ObservationAddChannel(hRecordObserv, idxChannelDefn);
	}

	/// <summary>Get information for the series data in this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="idQuantityUnits"></param>
	/// <param name="idValueType"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesInfo2( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, int% idQuantityUnits, Guid% idValueType)
	{
		return ObservationGetSeriesInfo( hRecordObserv, idxChannel, idxSeries, idQuantityUnits, idValueType );
	}

	/// <summary>Get information for the series data in this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="idQuantityUnits"></param>
	/// <param name="idQuantityCharacteristic"></param>
	/// <param name="idValueType"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesInfo3( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, int% idQuantityUnits, Guid% idQuantityCharacteristic, Guid% idValueType)
	{
		bool					status = false;
		CPQDIF_R_Observation	*pobs;
		UINT4					idQuantityUnitsLocal;
		GUID					idValueTypeLocal;
		GUID					idQuantityCharacteristicLocal;

		try
		{
			pobs = ValidateObservation( hRecordObserv );
			if( pobs )
			{
				status = pobs->GetSeriesInfo( idxChannel, idxSeries, idQuantityUnitsLocal, idQuantityCharacteristicLocal, idValueTypeLocal );
				if( status )
				{
					//	Translate information
					idQuantityUnits = idQuantityUnitsLocal;

					// Copy the GUIDs from System::Guid.
					idQuantityCharacteristic = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idQuantityCharacteristicLocal, System::Guid::typeid);
					idValueType = (System::Guid%)Marshal::PtrToStructure((System::IntPtr)&idValueTypeLocal, System::Guid::typeid);

					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Get trigger information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idTriggerMethod"></param>
	/// <param name="arTriggerChannels"></param>
	/// <param name="timeTriggered"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetTriggerInfo2( System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, CPQDIFTimeStamp% timeTriggered)
	{
		return ObservationGetTriggerInfo(hRecordObserv, idTriggerMethod, arTriggerChannels, timeTriggered);
	}

	/// <summary>
	/// Get trigger information for this observation record.
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idTriggerMethod"></param>
	/// <param name="arTriggerChannels"></param>
	/// <param name="timeTriggered"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetTriggerInfo2( System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, double% timeTriggered)
	{
		return ObservationGetTriggerInfo(hRecordObserv, idTriggerMethod, arTriggerChannels, timeTriggered);
	}

	/// <summary>
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationSetSeriesBaseQty2( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double value)
	{
		return ObservationSetSeriesBaseQty(hRecordObserv, idxChannel, idxSeries, value);
	}

	/// <summary>
	/// <param name="hRecordObserv">The observation record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value"></param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ObservationGetSeriesBaseQty2( System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value)
	{
		return ObservationGetSeriesBaseQty( hRecordObserv, idxChannel, idxSeries, value );
	}

	/// <summary>
	/// Get the scalar data stored in this element.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="value">Out param that returns the scalar data stored in this element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::ScalarGetData2( System::IntPtr hElement, System::Object^% value)
	{
		return ElementGetScalarData(hElement, value);
	}

	/// <summary>
	/// Get the scalar data at the specified index in this vector.
	/// <param name="hElement">Handle to the elememt, which should be a vector.</param>
	/// <param name="index">Index of datum in vector to retrieve.</param>
	/// <param name="value">Out param that returns the scalar data stored in this element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::VectorGetData2( System::IntPtr hElement, long index, System::Object^% value)
	{
		return ElementGetVectorData(hElement, index, value);
	}

	/// <summary>
	/// Get the number of items in the vector contained in this element.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="count">Out param that takes the number of items in the vector contained in this element.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::VectorGetCount2( System::IntPtr hElement, long% count)
	{
		return ElementGetVectorCount( hElement, count );
	}

	/// <summary>
	/// Get the vector stored in this element.
	/// <param name="hElement">Handle to the elememt.</param>
	/// <param name="arValues">Out param that returns the vector as an array.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::VectorGetArray2(System::IntPtr hElement, Array^% arValues)
	{
		return ElementGetVectorArray( hElement, arValues );
	}

	/// <summary>
	/// Check if the scalar in this element is a string.
	/// <param name="hElement">Handle to the element.</param>
	/// <returns>True if the data is a string; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::VectorIsString2( System::IntPtr hElement)
	{
		return ElementIsString( hElement );
	}

	/// <summary>Set the trigger low value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dblValue">New trigger low value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetTriggerLow( System::IntPtr hRecordSettings, long idxChannel, double dblValue)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetTriggerLow(idxChannel, dblValue);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the trigger high value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dblValue">New trigger high value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetTriggerHigh( System::IntPtr hRecordSettings, long idxChannel, double dblValue)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetTriggerHigh(idxChannel, dblValue);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the trigger rate value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">New trigger rate value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetTriggerRate( System::IntPtr hRecordSettings, long idxChannel, double dblValue)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetTriggerRate(idxChannel, dblValue);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the full scale value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">New full scale value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetFullScale( System::IntPtr hRecordSettings, long idxChannel, double dblValue)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetFullScale(idxChannel, dblValue);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the noise floor value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">New noise floor value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetNoiseFloor( System::IntPtr hRecordSettings, long idxChannel, double dblValue)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetNoiseFloor(idxChannel, dblValue);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the nominal frequency for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dblValue">New nominal frequency value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetNominalFrequency( System::IntPtr hRecordSettings, double dblValue)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetNominalFrequency(dblValue);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the use calibration value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="value">New use calibration value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetUseCalibration( System::IntPtr hRecordSettings, bool value)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetUseCalibration(value);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the use transducer value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="value">New use transducer value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetUseTransducer( System::IntPtr hRecordSettings, bool value)
	{
		bool status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				status = psett->SetUseTransducer(value);
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the effective date value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dtVal">New effective date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetEffective( System::IntPtr hRecordSettings, CPQDIFTimeStamp% dtVal)
	{
		bool	status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Translate times
				TIMESTAMPPQDIF		tsVal;
				tsVal.day =dtVal.day;
				tsVal.sec = dtVal.sec;

				status = psett->SetEffective(tsVal);
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the effective date value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dtVal">New effective date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetEffective( System::IntPtr hRecordSettings, double dtVal)
	{
		bool	status = false;

		try
		{
			//	Translate times
			TIMESTAMPPQDIF		tsVal;
			CPQDIFTimeStamp 	tsValNet;
			theSupport.SetTimeStampFromDate( tsVal, dtVal );
			tsValNet.day = tsVal.day;
			tsValNet.sec = tsVal.sec;

			status = SettingsSetEffective(hRecordSettings, tsValNet);
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the installed date value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dtVal">New installed date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetInstalled( System::IntPtr hRecordSettings, CPQDIFTimeStamp% dtVal)
	{
		bool	status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Translate times
				TIMESTAMPPQDIF		tsVal;
				tsVal.day = dtVal.day;
				tsVal.sec = dtVal.sec;

				status = psett->SetInstalled(tsVal);
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the installed date value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dtVal">New installed date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetInstalled( System::IntPtr hRecordSettings, double dtVal)
	{
		bool	status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Translate times
				TIMESTAMPPQDIF		tsVal;
				CPQDIFTimeStamp 	tsValNet;
				theSupport.SetTimeStampFromDate( tsVal, dtVal );
				tsValNet.day = tsVal.day;
				tsValNet.sec = tsVal.sec;

				status = SettingsSetInstalled(hRecordSettings, tsValNet);
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the removed date value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dtVal">New removed date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetRemoved( System::IntPtr hRecordSettings, CPQDIFTimeStamp% dtVal)
	{
		bool	status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Translate times
				TIMESTAMPPQDIF		tsVal;
				tsVal.day = dtVal.day;
				tsVal.sec = dtVal.sec;

				status = psett->SetRemoved(tsVal);
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the removed date value for this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dtVal">New removed date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsSetRemoved( System::IntPtr hRecordSettings, double dtVal)
	{
		bool	status = false;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				//	Translate times
				TIMESTAMPPQDIF		tsVal;
				CPQDIFTimeStamp 	tsValNet;
				theSupport.SetTimeStampFromDate( tsVal, dtVal );
				tsValNet.day = tsVal.day;
				tsValNet.sec = tsValNet.sec;

				status = SettingsSetRemoved(hRecordSettings, tsValNet);
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the effective date value for this data source record.
	/// <param name="hRecordDS">The settings record.</param>
	/// <param name="dtVal">New effective date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetEffective( System::IntPtr hRecordDS, CPQDIFTimeStamp% dtVal)
	{
		bool	status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				//	Translate times
				TIMESTAMPPQDIF		tsVal;
				tsVal.day = dtVal.day;
				tsVal.sec = dtVal.sec;

				status = pds->SetEffective(tsVal);
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>
	/// Set the effective date value for this data source record.
	/// <param name="hRecordDS">The settings record.</param>
	/// <param name="dtVal">New effective date value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetEffective( System::IntPtr hRecordDS, double dtVal)
	{
		bool	status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				//	Translate times
				TIMESTAMPPQDIF		tsVal;
				CPQDIFTimeStamp 	tsValNet;
				theSupport.SetTimeStampFromDate( tsVal, dtVal );
				tsValNet.day = tsVal.day;
				tsValNet.sec = tsVal.sec;

				status = DataSourceSetEffective(hRecordDS, tsValNet);
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>Set the series definition nominal value for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value">New nominal value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetSeriesDefnNominal( System::IntPtr hRecordDS, long idxChannel, long idxSeries, double value)
	{
		bool status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				status = pds->SetSeriesDefnNominal( idxChannel, idxSeries, value );
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the series definition digits for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value">New digits value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetSeriesDefnDigits( System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value)
	{
		bool status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				status = pds->SetSeriesDefnDigits( idxChannel, idxSeries, value );
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the series definition resolution for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="dblValue">New resolution value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetSeriesDefnResolution( System::IntPtr hRecordDS, long idxChannel, long idxSeries, double dblValue)
	{
		bool status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				status = pds->SetSeriesDefnResolution( idxChannel, idxSeries, dblValue );
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the series definition display for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value">New display value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetSeriesDefnDisplay( System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value)
	{
		bool status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				status = pds->SetSeriesDefnDisplay( idxChannel, idxSeries, value );
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the series definition units for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value">New units value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetSeriesDefnUnits( System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value)
	{
		bool status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				status = pds->SetSeriesDefnUnits( idxChannel, idxSeries, value );
			}
		}
		catch( ... )
		{
			status = false;
		}
		return status;
	}

	/// <summary>Set the series definition prefix for this data source record.
	/// <param name="hRecordDS">The data source record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="idxSeries">Index of the series.</param>
	/// <param name="value">New prefix value.</param>
	/// <returns>True if successful; false if not.</returns>
	/// </summary>
	bool CPQDIFNet::DataSourceSetSeriesDefnPrefix( System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value)
	{
		bool status = false;

		try
		{
			CPQDIF_R_DataSource *	pds = ValidateDataSource( hRecordDS );
			if( pds )
			{
				status = pds->SetSeriesDefnPrefix( idxChannel, idxSeries, value );
			}
		}
		catch( ... )
		{
			//status = false;
		}
		return status;
	}

	/// <summary>Get the value of the trigger low from this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">Out param that returns the trigger low value.</param>
	/// <returns>True if successful; false if not a valid settings record.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetTriggerLow( System::IntPtr hRecordSettings, long idxChannel, double% dblValue)
	{
		bool	status = false;
		double	dVal;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if (psett->GetTriggerLow(idxChannel, dVal) == true)
				{
					dblValue = dVal;
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>Get the value of the trigger high from this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">Out param that returns the trigger high value.</param>
	/// <returns>True if successful; false if not a valid settings record.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetTriggerHigh( System::IntPtr hRecordSettings, long idxChannel, double% dblValue)
	{
		bool	status = false;
		double	dVal;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if (psett->GetTriggerHigh(idxChannel, dVal) == true)
				{
					dblValue = dVal;
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>Get the value of the trigger rate from this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">Out param that returns the trigger rate.</param>
	/// <returns>True if successful; false if not a valid settings record.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetTriggerRate( System::IntPtr hRecordSettings, long idxChannel, double% dblValue)
	{
		bool	status = false;
		double	dVal;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if (psett->GetTriggerRate(idxChannel, dVal) == true)
				{
					dblValue = dVal;
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>Get the full scale value from this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">Out param that returns the full scale value.</param>
	/// <returns>True if successful; false if not a valid settings record.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetFullScale( System::IntPtr hRecordSettings, long idxChannel, double% dblValue)
	{
		bool	status = false;
		double	dVal;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if (psett->GetFullScale(idxChannel, dVal) == true)
				{
					dblValue = dVal;
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>Get the value of the noise floor from this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of the channel.</param>
	/// <param name="dblValue">Out param that returns the noise floor.</param>
	/// <returns>True if successful; false if not a valid settings record.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetNoiseFloor( System::IntPtr hRecordSettings, long idxChannel, double% dblValue)
	{
		bool	status = false;
		double	dVal;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if (psett->GetNoiseFloor(idxChannel, dVal) == true)
				{
					dblValue = dVal;
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}

	/// <summary>Get the value of the nominal frequency from this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="dblValue">Out param that returns the nominal frequency.</param>
	/// <returns>True if successful; false if not a valid settings record.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetNominalFrequency( System::IntPtr hRecordSettings, double% dblValue)
	{
		bool	status = false;
		double	dVal;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if (psett->GetNominalFrequency(dVal) == true)
				{
					dblValue = dVal;
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}


	/// <summary>Get the value of the channel definition from this settings record.
	/// <param name="hRecordSettings">The settings record.</param>
	/// <param name="idxChannel">Index of channel whose definition is to be returned.</param>
	/// <param name="idxDefn">Out param that returns the index of the channel definition.</param>
	/// <returns>True if successful; false if not a valid settings record.</returns>
	/// </summary>
	bool CPQDIFNet::SettingsGetChannelDefn( System::IntPtr hRecordSettings, long idxChannel, long% idxDefn)
	{
		bool	status = false;
		UINT4	channelDef;

		try
		{
			CPQDIF_R_Settings * psett = ValidateSettings( hRecordSettings );
			if( psett )
			{
				if (psett->GetChannelInfo(idxChannel, channelDef) == true)
				{
					idxDefn = channelDef;
					status = true;
				}
			}
		}
		catch( ... )
		{
			// e->Delete();
		}
		return status;
	}


	/// <summary>Convert DateTime to CPQDIFTimeStamp.</summary>
	/// <param name="dt">The DateTime to convert.</param>
	/// <returns>CPQDIFTimeStamp version of dt.</returns>
	CPQDIFTimeStamp CPQDIFNet::DateTimeToDT(System::DateTime dt)
	{
		CPQDIFTimeStamp pqdt;
		//System::UInt64 dtft = dt.ToFileTimeUtc();

		pqdt.FromOADate(dt.ToFileTimeUtc());

		return pqdt;
	}


	/// <summary>Convert CPQDIFTimeStamp to DateTime.</summary>
	/// <param name="pqdt">The CPQDIFTimeStamp to convert.</param>
	/// <returns>DateTime version of pqdt.</returns>
	System::DateTime CPQDIFNet::DTToDateTime(CPQDIFTimeStamp pqdt)
	{
		return System::DateTime::FromOADate(pqdt.ToOADate());
	}

}

