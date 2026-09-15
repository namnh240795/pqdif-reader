/*
**  Class:
**  Description:	Provides a .NET managed API to pqdiflib.
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
#include <string>

#include "support.h"
#include "utils.h"
#include "pqdif_classes_wrapper.h"
#include "pqdif_lg.h"
#include "PQDIFTimeStamp.h"
#include "str_memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PQDIFNet {

	/// <summary>
	/// Provides the .NET equivalent of the API defined in PQDcom4.
	/// </summary>
	public ref class CPQDIFNet : IDisposable
	{
	private:
		CPQDIF_PC_FlatFile *	m_percont;	//	Persistence controller

	private:

#pragma region Helper methods
		/// <summary>Do we have a persistence controller?</summary>
		bool IsInitialized();

		/// <summary>
		/// Is this a valid element handle?
		/// <param name="hElement">Handle to test.</param>
		/// <returns>hElement cast to an element pointer.</returns>
		/// </summary>
		CPQDIF_Element* ValidateElement(System::IntPtr hElement);

		/// <summary>
		/// Is this a valid collection handle?
		/// <param name="hElement">Handle to test.</param>
		/// <returns>hElement cast to a collection pointer or null if this is not a valid handle.</returns>
		/// </summary>
		CPQDIF_E_Collection* ValidateCollection(System::IntPtr hElement);

		/// <summary>
		/// Is this a valid scalar handle?
		/// <param name="hElement">Handle to test.</param>
		/// <returns>hElement cast to a scalar pointer or null if this is not a valid handle.</returns>
		/// </summary>
		CPQDIF_E_Scalar* ValidateScalar(System::IntPtr hElement);

		/// <summary>
		/// Is this a valid vector handle?
		/// <param name="hElement">Handle to test.</param>
		/// <returns>hElement cast to a vector pointer or null if this is not a valid handle.</returns>
		/// </summary>
		CPQDIF_E_Vector* ValidateVector(System::IntPtr hElement);

		/// <summary>
		/// Is this a valid observation record handle?
		/// <param name="hRecordObserv">Handle to test.</param>
		/// <returns>hElement cast to an observation record pointer or null if
		/// this is not a valid handle.
		/// </returns>
		/// </summary>
		CPQDIF_R_Observation* ValidateObservation(System::IntPtr hRecordObserv);

		/// <summary>
		/// Is this a valid data source record handle?
		/// <param name="hRecDS">Handle to test.</param>
		/// <returns>hElement cast to a data source record pointer or null if
		/// this is not a valid handle.
		/// </returns>
		/// </summary>
		CPQDIF_R_DataSource* ValidateDataSource(System::IntPtr hRecDS);

		/// <summary>
		/// Is this a valid settings record handle?
		/// <param name="hRecSettings">Handle to test.</param>
		/// <returns>hElement cast to a settings record pointer or null if
		/// this is not a valid handle.
		/// </returns>
		/// </summary>
		CPQDIF_R_Settings* ValidateSettings(System::IntPtr hRecSettings);
#pragma endregion

	public:

#pragma region Properties
		/// <summary>
		/// The pqd file name.
		/// </summary>
		property String^ FlatFileName
		{
			virtual String^ get();
			virtual void set(String^ newVal);
		}

		/// <summary>
		/// The compression algorithm.
		/// </summary>
		property long CompressionAlgorithm
		{
			virtual long get();
			virtual void set(long newVal);
		}

		/// <summary>
		/// </summary>
		property bool CanWriteIncremental
		{
			virtual bool get();
		}

		/// <summary>
		/// The number of tags in the pqd file.
		/// </summary>
		property long TagCount
		{
			virtual long get();
		}

		/// <summary>
		/// The compression type.
		/// </summary>
		property long CompressionStyle
		{
			virtual long get();
			virtual void set(long newVal);
		}

		/// <summary>
		/// The number of records in this file.
		/// </summary>
		property long RecordCount
		{
			virtual long get();
		}
#pragma endregion properties

#pragma region Constructors, Destructors
		/// <summary>
		/// Default constructor.  Instantiate our one and only instance of
		/// the persistence controller.
		/// </summary>
		CPQDIFNet();

		/// <summary>
		/// Destructor.  Free our instance of the persistence controller.
		/// </summary>
		~CPQDIFNet();
#pragma endregion

#pragma region Public Methods
		/// <summary>
		/// For compatibility with PQDCom.	Returns value of RecordCount property.
		/// </summary>
		long RecordGetCount();

		/// <summary>
		/// Read the pqd file specified by the FlatFile property.
		/// </summary>
		bool Read();

		/// <summary>
		/// Close the pqd file.
		/// </summary>
		bool Close();

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
		bool RecordGetInfo(long index, Guid% tagRecordGUID, String^% nameRecordType, long% sizeHeader, long% sizeRecord, long% posThisRecord, long% posNextRecord);

		/// <summary>
		/// Get the main collection from the record indicated by index.
		/// <param name="index">Index of the record.</param>
		/// <param name="hCollection">Out param that returns the handle of the collection as an unsigned 64-bit value."
		/// This handle is passed to other APIs to access the contents of the collection.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordGetCollection(long index, System::IntPtr% hCollection);

		/// <summary>
		/// Get this element's Guid.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="tagElement">Out param that returns the Guid for this element type.</param>
		/// <param name="nameTagElement">Out param that returns the name for this element type.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetTag(System::IntPtr hElement, Guid% tagElement, String^% nameTagElement);

		/// <summary>
		/// Get the element type and the type of the data it holds.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="typeElement">Out param that takes the element type.</param>
		/// <param name="typePhysical">Out param that takes the physical type of the data in this element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetType(System::IntPtr hElement, long% typeElement, long% typePhysical);

		/// <summary>
		/// Get the scalar data stored in this element.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="value">Out param that returns the scalar data stored in this element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetScalarData(System::IntPtr hElement, System::Object^% value);

		/// <summary>
		/// Get the scalar data at the specified index in this vector.
		/// <param name="hElement">Handle to a vector element.</param>
		/// <param name="index">Index of datum in vector to retrieve.</param>
		/// <param name="value">Out param that returns the scalar data stored in this element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetVectorData(System::IntPtr hElement, long index, System::Object^% value);

		/// <summary>
		/// Get the number of items in the vector contained in this element.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="count">Out param that takes the number of items in the vector contained in this element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetVectorCount(System::IntPtr hElement, long% count);

		/// <summary>
		/// Create a new element of the specified type.
		/// <param name="typeElement">The type of element to create.</param>
		/// <param name="tag">Guid for this element.</param>
		/// <param name="hElement">Out param that returns a handle to the new element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementCreate(long typeElement, System::Guid tag, System::IntPtr% hElement);

		/// <summary>
		/// Delete the specified element.
		/// <param name="hElement">The element to delete.</param>
		/// <returns>True if successful; false if hElement is invalid.</returns>
		/// </summary>
		bool ElementDestroy(System::IntPtr hElement);

		/// <summary>
		/// Add an element to a collection.
		/// <param name="hCollection">The collection.</param>
		/// <param name="hElement">The element to add.</param>
		/// <returns>True if successful; false if hCollection and, or hElement is, are invalid.</returns>
		/// </summary>
		bool CollectionAddEntry(System::IntPtr hCollection, System::IntPtr hElement);

		/// <summary>Deprecated.</summary>
		bool CollectionInsertEntry(System::IntPtr hCollection, long indexToInsert, System::IntPtr pElement);

		/// <summary>
		/// Remove an element from a collection.
		/// <param name="hCollection">The collection from which we remove the element.</param>
		/// <param name="index">Index in the collection of the element to remove.</param>
		/// <returns>True if successful; false if hCollection is invalid.</returns>
		/// </summary>
		bool CollectionRemoveEntry(System::IntPtr hCollection, long index);

		/// <summary>
		/// Remove and delete an element from a collection.
		/// <param name="hCollection">The collection.</param>
		/// <param name="index">Index of the element in the collection to remove.</param>
		/// <returns>True if successful; false if hCollection is invalid.</returns>
		/// </summary>
		bool CollectionDestroyEntry(System::IntPtr hCollection, long index);

		/// <summary>
		/// Call WriteNew() on our persistence controller.
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool  WriteNew();

		/// <summary>
		/// Call WriteIncremental() on our persistence controller.
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool WriteIncremental();

		/// <summary>
		/// Call WriteToStream() on our persistence controller.
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool WriteToStream(System::IO::MemoryStream^ pstrm);

		/// <summary>
		/// Get the vector stored in this element.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="arValues">Out param that returns the vector as an array.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetVectorArray(System::IntPtr  hElement, Array^% arValues);

		/// <summary>
		/// Checks if the scalar in this element is a string.
		/// <param name="hElement">Handle to the element.</param>
		/// <returns>True if the data is a string; false if not.</returns>
		/// </summary>
		bool ElementIsString(System::IntPtr hElement);

		/// <summary>
		/// Retrieves the value from a string element.
		/// <param name="hElement">Handle to the element.</param>
		/// <param name="sValue">Out param that returns the string.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetString(System::IntPtr hElement, String^% sValue);

		//	NOT IMPLEMENTED
		/// TODO Not sure what to do with this but made signature .Net compatible.
		bool RecordCreateByNameInsert(String^ nameTagRecordType, long indexToInsert);

		/// <summary>
		/// Remove and delete the record at index.
		/// <param name="index">Index of record.</param>
		/// <returns>True if successful; False if index is invalid or we are not initialized.</returns>
		/// </summary>
		bool RecordDestroy(long index);

		/// <summary>
		/// Sets the value of the scalar to value.
		/// <param name="hScalar">Handle to the scalar.</param>
		/// <param name="value">New value as a System::Object.</param>
		/// <returns>True if successful; false if scalar is not valid.</returns>
		/// </summary>
		bool ScalarSetValue(System::IntPtr hScalar, System::Object^ value);

		/// <summary>
		/// Sets the value of the vector to this String value.
		/// <param name="hVector">Handle to the Vector.</param>
		/// <param name="value">New String value.</param>
		/// <returns>True if successful; false if scalar is not valid.</returns>
		/// </summary>
		bool VectorSetString(System::IntPtr hVector, String^ value);

		/// <summary>
		/// Sets the values of the vector to the specified array.
		/// <param name="hVector">Handle to the vector.</param>
		/// <param name="arValues">Values.</param>
		/// <returns>True if successful.</returns>
		/// </summary>
		bool VectorSetArray(System::IntPtr hVector, Array^ arValues);

		/// <summary>
		/// Read bytes into the specified character array.  The number of bytes to read
		/// is determined by the length of the array.
		/// <param name="ar">Character array to hold the data.</param>
		/// <returns>True if successful.</returns>
		/// </summary>
		bool ReadChunk(cli::array<unsigned char>^% ar);

		/// <summary>
		/// Write bytes from the specified character array to the file.  The number of bytes to
		/// write is determined by the length of the array.
		/// <param name="ar">Character array to write.</param>
		/// <returns>True if successful.</returns>
		/// </summary>
		bool WriteChunk(cli::array<unsigned char>^ ar);

		/// <summary>
		/// Creates a new instance of the Persistence Controller and frees any current instance first.
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool New();

		/// <summary>Get the tag name of the Guid specified by index.
		/// <param name="index">Index of Guid.</param>
		/// <returns>The Guid's tag name.</returns>
		/// </summary>
		String^ GetTagName(long index);

		/// <summary>
		/// Get the data for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="arValues">Out param that returns array of series values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesData(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Array^% arValues);

		/// <summary>Get information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="timeStart">Out param that returns the start time.</param>
		/// <param name="name">Out param that returns the name for this record.</param>
		/// <param name="countChannels">Out param that returns the number of channels.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetInfo(System::IntPtr hRecordObserv, CPQDIFTimeStamp% timeStart, String^% name, long% countChannels);

		/// <summary>Get information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="timeStart">Out param that returns the start time.</param>
		/// <param name="name">Out param that returns the name for this record.</param>
		/// <param name="countChannels">Out param that returns the number of channels.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetInfo(System::IntPtr hRecordObserv, double% timeStart, String^% name, long% countChannels);

		/// <summary>Get information for this channel in an observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Out param that returns the index of the channel.</param>
		/// <param name="name">Out param that returns the name for this channel.</param>
		/// <param name="idPhase"></param>
		/// <param name="idQuantityType"></param>
		/// <param name="countSeries">Out param that returns the number of series values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetChannelInfo(System::IntPtr hRecordObserv, long idxChannel, String^% name, long% idPhase, Guid %idQuantityType, long% countSeries);

		/// <summary>Get information for the series data in this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="idQuantityUnits"></param>
		/// <param name="idValueType"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesInfo(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, int% idQuantityUnits, Guid %idValueType);

		/// <summary>Free the specified observation record.
		/// <param name="hRecordObserv">The record to free.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordReleaseObservation(System::IntPtr hRecordObserv);

		/// <summary>Get an observation record specified by index.
		/// <param name="index">Record to get.</param>
		/// <param name="hRecordObserv">Out param that returns a handle to the record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordRequestObservation(long index, System::IntPtr% hRecordObserv);

		/// <summary>
		/// Get the settings record for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="hRecordSettings">Out param that returns the handle of the settings record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSettings(System::IntPtr hRecordObserv, System::IntPtr% hRecordSettings);

		/// <summary>
		/// Get trigger information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idTriggerMethod">The trigger method.</param>
		/// <param name="arTriggerChannels">The trigger channels.</param>
		/// <param name="timeTriggered">The trigger time.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetTriggerInfo(System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, CPQDIFTimeStamp% timeTriggered);

		/// <summary>
		/// Get trigger information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idTriggerMethod">The trigger method.</param>
		/// <param name="arTriggerChannels">The trigger channel.</param>
		/// <param name="timeTriggered">The trigger time.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetTriggerInfo(System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, double% timeTriggered);

		/// <summary>
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesBaseQty(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value);

		/// <summary>
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesNominalQty(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value);

		/// <summary>
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationSetSeriesBaseQty(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value);

		/// <summary>Create a new container record and return its index.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateContainer(String^ language, String^ title, String^ subject, String^ author, String^ keywords, String^ comments,
			String^ lastSavedBy, String^ application, String^ security, String^ owner, String^ copyright, String^ trademark, String^ notes);

		/// <summary>Create a new data source record and return its index.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateDataSource(long indexInsert, System::Guid idDataSourceType, System::Guid idVendor, System::Guid idEquipment,
			String^ serialNumberDS, String^ versionDS, String^ nameDS, String^ ownerDS, String^ locationDS, String^ timeZoneDS);

		/// <summary>Create a new observation record and return its index.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateObservation(long indexInsert, String^ name, CPQDIFTimeStamp% timeCreate, CPQDIFTimeStamp% timeStart, long idTriggerMethod, CPQDIFTimeStamp% timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger);

		/// <summary>Create a new observation record and return its index.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateObservation(long indexInsert, String^ name, double timeCreate, double timeStart, long idTriggerMethod, double timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger);

		/// <summary>Create a new observation record and return its index.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateObservation3(long indexInsert, String^ name, CPQDIFTimeStamp% timeCreate, CPQDIFTimeStamp% timeStart, long idTriggerMethod);

		/// <summary>Create a new observation record and return its index.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateObservation3(long indexInsert, String^ name, DATE timeCreate, DATE timeStart, long idTriggerMethod);

		/// <summary>Get a data source record.
		/// <param name="index">Index of the data source record sought.</param>
		/// <param name="hRecordDS">Out param returning the handle of the data source record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordRequestDataSource(long index, System::IntPtr% hRecordDS);

		/// <summary>Release a data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordReleaseDataSource(System::IntPtr hRecordDS);

		/// <summary>
		/// Add a new channel definition to this data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="name"></param>
		/// <param name="idPhase"></param>
		/// <param name="idQuantityType"></param>
		/// <returns>Index of new channel definition.</returns>
		/// </summary>
		long DataSourceAddChannelDefn(System::IntPtr hRecordDS, String^ name, long idPhase, System::Guid idQuantityType);

		/// <summary>
		/// Add a new series definition to this channel definition.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="idxChannelDefn"></param>
		/// <param name="idQuantityUnits"></param>
		/// <param name="idValueType"></param>
		/// <param name="idStorageMethod"></param>
		/// <returns>Index of new channel definition.</returns>
		/// </summary>
		long DataSourceAddSeriesDefn(System::IntPtr hRecordDS, long idxChannelDefn, long idQuantityUnits, System::Guid idValueType, long idStorageMethod);

		/// <summary>
		/// Add series data to this observation record.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="arValues">Series data to add.</param>
		/// <returns>Index of new series or -1 if fail.</returns>
		/// </summary>
		long ObservationAddSeriesDouble(System::IntPtr hRecordObs, long idxChannel, Array^ arValues);

		/// <summary>
		/// Add a channel to this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannelDefn">New channel.</param>
		/// <returns>Id of new channel or -1 if fails.</returns>
		/// </summary>
		long ObservationAddChannel(System::IntPtr hRecordObserv, long idxChannelDefn);

		/// <summary>Get a settings record.
		/// <param name="index">Index of the settings record.</param>
		/// <param name="hRecordSettings">Out param returning the handle of the settings record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordRequestSettings(long index, System::IntPtr% hRecordSettings);

		/// <summary>
		/// Release a settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordReleaseSettings(System::IntPtr hRecordSettings);

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
		bool SettingsSetChanTrans(System::IntPtr hRecordSettings, long idxChannel, long xdTransformerTypeID, double xdSystemSideRatio, double xdMonitorSideRatio, cli::array<double>^ xdFreqResponse);

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
		bool SettingsSetChanCal(System::IntPtr hRecordSettings, long idxChannel, double calTimeSkew, double calOffset, double calRatio, bool calMustUseARCal, cli::array<double>^ calApplied, cli::array<double>^ calRecorded);

		/// <summary>
		/// Get the number of channels.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <returns>The number of channels.</returns>
		/// </summary>
		long SettingsGetChanCount(System::IntPtr hRecordSettings);

		/// <summary>
		/// Add a new channel definition.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannelDefn">New channel definition.</param>
		/// <returns>Index of new channel or -1 if fail.</returns>
		/// </summary>
		long SettingsAddChannel(System::IntPtr hRecordSettings, long idxChannelDefn);

		/// <summary>
		/// Add a new channel definition.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannelDefn">New channel definition.</param>
		/// <param name="idTriggerType">Trigger type.</param>
		/// <returns>Index of new channel or -1 if fail.</returns>
		/// </summary>
		long SettingsAddChannel2(System::IntPtr hRecordSettings, long idxChannelDefn, long idTriggerType);

		/// <summary>
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel"></param>
		/// <param name="triggerShapeParam">Array of double values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetTriggerShapeParam(System::IntPtr hRecordSettings, long idxChannel, cli::array<double>^ triggerShapeParam);

		//
		//
		//	Deprecated
		//
		/// TODO - Comment says this is deprecated but it is used by SettingsGetChannel2 so we need to port it.
		/// 	   Most arguments are not used.
		bool SettingsGetChannel(System::IntPtr hRecordSettings, long idxChannel, long% idxChannelDefn, long% triggerTypeID, double% fullScale, double% noiseFloor, double% triggerLow,
			double& triggerHigh, double% triggerRate, System::Array^% triggerShapeParam);

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
		bool SettingsGetChanTrans(System::IntPtr hRecordSettings, long idxChannel, long% xdTransformerTypeID, double% xdSystemSideRatio, double% xdMonitorSideRatio, System::Array^% xdFreqResponse);

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
		bool SettingsGetChanCal(System::IntPtr hRecordSettings, long idxChannel, double% calTimeSkew, double% calOffset, double% calRatio, bool% calMustUseARCal, System::Array^% calApplied, System::Array^% calRecorded);

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
		long RecordCreateSettings(long indexInsert, CPQDIFTimeStamp% timeEffective, CPQDIFTimeStamp% timeInstalled, CPQDIFTimeStamp% timeRemoved, bool useCal, bool useTrans);

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
		long RecordCreateSettings(long indexInsert, double timeEffective, double timeInstalled, double timeRemoved, bool useCal, bool useTrans);

		/// <summary>
		/// Get the connection type.
		/// <param name="hRecordSettings">The setttings record.</param>
		/// <param name="connectionType">Out param containing the connection type.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsGetConnectType(System::IntPtr hRecordSettings, unsigned long% connectionType);


		/// <summary>
		/// Set the connection type.
		/// <param name="hRecordSettings">The setttings record.</param>
		/// <param name="connectionType">connection type.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetConnectType(System::IntPtr hRecordSettings, unsigned long  connectionType);

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
		bool SettingsGetInfo(System::IntPtr hRecordSettings, CPQDIFTimeStamp% timeEffective, CPQDIFTimeStamp% timeInstalled, CPQDIFTimeStamp% timeRemoved, bool% useCal, bool% useTrans);

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
		bool SettingsGetInfo(System::IntPtr hRecordSettings, double% timeEffective, double% timeInstalled, double% timeRemoved, bool% useCal, bool% useTrans);

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
		bool DataSourceGetInfo(System::IntPtr hRecordDS, Guid% idDataSourceType, Guid% idVendor, Guid% idEquipment, String^% serialNumberDS, String^% versionDS, String^% nameDS,
			String^% ownerDS, String^% locationDS, String^% timeZoneDS);

		/// <summary>
		/// Return the name of the GUID tag specified by index.
		/// <param name="index">Index of GUID.</param>
		/// <returns>Name of the GUID or nullptr if invalid.</returns>
		/// </summary>
		String^ get_TagName(long index);

		/// <summary>
		/// Set the name of the GUID tag.
		/// This is a read-only property so this method is a no-op.
		/// TODO Why is this method defined?
		/// <param name="index">Index of GUID.</param>
		/// <param name="newVal">New value for GUID name.</param>
		/// <returns>True.</returns>
		/// </summary>
		bool put_TagName(long index, String^ newVal);

		/// <summary>
		/// Return the number of records in this collection.
		/// <param name="hCollection">The collection record.</param>
		/// <param name="count">Out param that returns the count of records in this collection.</param>
		/// <returns>True if successful; false if invalid collection.</returns>
		/// </summary>
		bool CollectionGetCount(System::IntPtr hCollection, long% count);

		/// <summary>
		/// Return the handle for the specified element in this collection.
		/// <param name="hCollection">The collection.</param>
		/// <param name="index">Index of element sought.</param>
		/// <param name="hElement">Out param that returns a handle to the element.</param>
		/// <returns>True if successful; false if invalid collection or index.</returns>
		/// </summary>
		bool CollectionGetEntry(System::IntPtr hCollection, long index, System::IntPtr% hElement);

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
		bool RecordGetInfo2(long index, Guid% tagRecordType, String^% nameRecordType, long% sizeHeader, long% sizeRecord, long% posThisRecord, long% posNextRecord);

		/// <summary>Create a new record in the container and return its index.
		/// Parameters are self explanatory.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateContainer2(String^ language, String^ title, String^ subject, String^ author, String^ keywords, String^ comments, String^ lastSavedBy, String^ application, String^ security, String^ owner, String^ copyright, String^ trademark, String^ notes);

		/// <summary>Create a new record in the container and return its index.
		/// Parameters are self explanatory.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateContainer3(String^ fileName, CPQDIFTimeStamp% dtCreate, long lMajor, long lMinor, long lCompatMajor, long lCompatMinor);

		/// <summary>Create a new record in the container and return its index.
		/// Parameters are self explanatory.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateContainer3(String^ fileName, double dtCreate, long lMajor, long lMinor, long lCompatMajor, long lCompatMinor);

		/// <summary>Set the data for a container recored.
		/// Parameters are self explanatory.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		bool ContainerSetInfo(String^ language, String^ title, String^ subject, String^ author, String^ keywords, String^ comments, String^ lastSavedBy, String^ application, String^ security, String^ owner, String^ copyright, String^ trademark, String^ notes);

		/// <summary>Create a new data source record and return its index.
		/// Parameters are self explanatory.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateDataSource2(long indexInsert, System::Guid idDataSourceType, System::Guid idVendor, System::Guid idEquipment, String^ serialNumberDS, String^ versionDS, String^ nameDS, String^ ownerDS, String^ locationDS, String^ timeZoneDS);

		/// <summary>
		/// Add a new channel definition to the data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="name"></param>
		/// <param name="idPhase"></param>
		/// <param name="idQuantityType"></param>
		/// <returns>Index of new channel definition.</returns>
		/// </summary>
		long DataSourceAddChannelDefn2(System::IntPtr hRecordDS, String^ name, long idPhase, System::Guid idQuantityType);

		/// <summary>
		/// Add a new channel definition to the data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="name"></param>
		/// <param name="idPhase"></param>
		/// <param name="idQM"></param>
		/// <param name="idQuantityType"></param>
		/// <returns>Index of new channel definition.</returns>
		/// </summary>
		long DataSourceAddChannelDefn3(System::IntPtr hRecordDS, String^ name, long idPhase, long idQM, System::Guid idQuantityType);

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
		bool DataSourceGetInfo2(System::IntPtr hRecordDS, System::Guid% idDataSourceType, System::Guid% idVendor, System::Guid% idEquipment, String^% serialNumberDS, String^% versionDS, String^% nameDS,
			String^% ownerDS, String^% locationDS, String^% timeZoneDS);

		/// <summary>Create a new observation record and return its index.
		/// Parameters are self explanatory.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateObservation2(long indexInsert, String^ name, CPQDIFTimeStamp% timeCreate, CPQDIFTimeStamp% timeStart, long idTriggerMethod, CPQDIFTimeStamp% timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger);

		/// <summary>Create a new observation record and return its index.
		/// Parameters are self explanatory.
		/// <returns>Index of newly created record.</returns>
		/// </summary>
		long RecordCreateObservation2(long indexInsert, String^ name, double timeCreate, double timeStart, long idTriggerMethod, double timeTriggered, cli::array<unsigned long>^ aidxChannelTrigger);

		/// <summary>Get information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="timeStart">Out param that returns the start time.</param>
		/// <param name="name">Out param that returns the name for this record.</param>
		/// <param name="countChannels">Out param that returns the number of channels.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetInfo2(System::IntPtr hRecordObserv, CPQDIFTimeStamp% timeStart, String^% name, long% countChannels);

		/// <summary>Get information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="timeStart">Out param that returns the start time.</param>
		/// <param name="name">Out param that returns the name for this record.</param>
		/// <param name="countChannels">Out param that returns the number of channels.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetInfo2(System::IntPtr hRecordObserv, double% timeStart, String^% name, long% countChannels);

		/// <summary>Get information for this channel in this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Out param that returns the index of the channel.</param>
		/// <param name="name">Out param that returns the name for this channel.</param>
		/// <param name="idPhase"></param>
		/// <param name="idQuantityType"></param>
		/// <param name="countSeries">Out param that returns the number of series values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetChannelInfo2(System::IntPtr hRecordObserv, long idxChannel, String^% name, long% idPhase, Guid %idQuantityType, long% countSeries);

		/// <summary>Get information for this channel in this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Out param that returns the index of the channel.</param>
		/// <param name="name">Out param that returns the name for this channel.</param>
		/// <param name="idPhase"></param>
		/// <param name="idQuantityType"></param>
		/// <param name="countSeries">Out param that returns the number of series values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetChannelInfo3(System::IntPtr hRecordObserv, long idxChannel, String^% name, long% idPhase, long% idQuantityMeasured, Guid %idQuantityType, long% countSeries);

		/// <summary>
		/// Get series extended data from this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="tag">Guid identifying series extended data.</param>
		/// <param name="value">Out param that returns the data as a System::Object.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesExtendedData(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Guid tag, System::Object^% value);

		/// <summary>
		/// Get channel extended data from this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="tag">Guid identifying channel extended data.</param>
		/// <param name="value">Out param that returns the data as a System::Object.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetChannelExtendedData(System::IntPtr hRecordObserv, long idxChannel, Guid tag, System::Object^% value);

		/// <summary>
		/// Get the index of the channel definition.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="lChanDefn">Out param that returns the channel definition index.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetChannelDefnIdx(System::IntPtr hRecordObserv, long idxChannel, long% lChanDefn);

		/// <summary>
		/// Get extended data from this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="tag">Guid identifying observation extended data.</param>
		/// <param name="vValue">Out param that returns the data as a System::Object.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetExtendedData(System::IntPtr hRecordObserv, System::Guid tag, System::Object^% vValue);

		/// <summary>
		/// Get this element's Guid.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="tagElement">Out param that takes the Guid for this element type.</param>
		/// <param name="nameTagElement">Out param that takes the name for this element type.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ElementGetTag2(System::IntPtr hElement, Guid% tagElement, String^% nameTagElement);

		/// <summary>
		/// Retrieve the value of this string element.
		/// <param name="hElement">Handle to the element.</param>
		/// <param name="sValue">Out param that returns the string.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool VectorGetString2(System::IntPtr hElement, String^% sValue);

		/// <summary>
		/// Set the value of the vector to a String value.
		/// <param name="hVector">Handle to the Vector.</param>
		/// <param name="value">New String value.</param>
		/// <returns>True if successful; false if scalar is not valid.</returns>
		/// </summary>
		bool VectorSetString2(System::IntPtr hVector, String^ value);

		/// <summary>
		/// Add series data to this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="arValues">Series data to add.</param>
		/// <returns>Index of new series or -1 if fail.</returns>
		/// </summary>
		long ObservationAddSeriesData(System::IntPtr hRecordObserv, long idxChannel, Array^ arValues);

		/// <summary>
		/// Add series data to this observation record.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxChannelShared"></param>
		/// <param name="idxSeriesShared"></param>
		/// <returns>Index of new series or -1 if fail.</returns>
		/// </summary>
		long ObservationAddSeriesShared(System::IntPtr hRecordObs, long idxChannel, long idxChannelShared, long idxSeriesShared);

		/// <summary>
		/// Set the channel frequency.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="freq">New frequency value.</param>
		/// <returns>True if successful; false if fail.</returns>
		/// </summary>
		bool ObservationSetChannelFreq(System::IntPtr hRecordObs, long idxChannel, double freq);

		/// <summary>Get the frequency value for this channel in this observation record.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="freq">Out param that returns the frequency.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetChannelFreq(System::IntPtr hRecordObs, long idxChannel, double% freq);

		/// <summary>
		/// Set the channel harmonic ID.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="harmonic">New harmonic ID value.</param>
		/// <returns>True if successful; false if fail.</returns>
		/// </summary>
		bool ObservationSetChannelHarmonicID(System::IntPtr hRecordObs, long idxChannel, int harmonic);

		/// <summary>Get the harmonic ID value for this channel in this observation record.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="harmonic">Out param that returns the harmonic ID value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetChannelHarmonicID(System::IntPtr hRecordObs, long idxChannel, int% harmonic);

		/// <summary>
		/// Set series scale value.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="scale"></param>
		/// <param name="offset"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationSetSeriesScale(System::IntPtr hRecordObs, long idxChannel, long idxSeries, double scale, double offset);

		/// <summary>
		/// Get series scale value.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="scale"></param>
		/// <param name="offset"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesScale(System::IntPtr hRecordObs, long idxChannel, long idxSeries, double% scale, double% offset);

		/// <summary>
		/// Get the series data for this observation record.
		/// <param name="hRecordObs">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="arValues">Out param that returns array of series values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesRawData(System::IntPtr hRecordObs, long idxChannel, long idxSeries, Array^% arValues);

		/// <summary>
		/// Get the data for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="arValues">Out param that returns array of series values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesResolvedData(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Array^% arValues);

		/// <summary>
		/// Get the data for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="arValues">Out param that returns array of series values.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesResolvedTimeStamp(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, Array^% arValues);

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
		long DataSourceAddSeriesDefn2(System::IntPtr hRecordDS, long idxChannelDefn, long idQuantityUnits, System::Guid idValueType, System::Guid idCharacteristicType, long idStorageMethod);

		/// <summary>
		/// Return the number of records in this file.
		/// <param name="count">Out param that returns the number of records.</param>
		/// <returns>True if successful; False if fail.</returns>
		/// </summary>
		bool RecordGetCount2(long% count);

		/// <summary>
		/// Remove and delete the record at index.
		/// <param name="index">Index of record to delete.</param>
		/// <returns>True if successful; False if index is invalid or we are not initialized.</returns>
		/// </summary>
		bool RecordDestroy2(long index);

		/// <summary>
		/// Get a data source record.
		/// <param name="index">Index of the data source record sought.</param>
		/// <param name="hRecordDS">Out param returning the handle of the data source record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordRequestDataSource2(long index, System::IntPtr% hRecordDS);

		/// <summary>
		/// Release a data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordReleaseDataSource2(System::IntPtr hRecordDS);

		/// <summary>
		/// Create a new settings record.
		/// <param name="indexInsert">Index where to insert the new record.</param>
		/// <returns>Index of the new record or -1 if fail.</returns>
		/// </summary>
		long RecordCreateSettings2(long indexInsert);

		/// <summary>
		/// Get a settings record.
		/// <param name="idxRecord">Index of the settings record sought.</param>
		/// <param name="hRecordSettings">Out param returning the handle of the settings record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordRequestSettings2(long idxRecord, System::IntPtr% hRecordSettings);

		/// <summary>
		/// Release this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordReleaseSettings2(System::IntPtr hRecordSettings);

		/// <summary>Get an observation record specified by index.
		/// <param name="index">Record to get.</param>
		/// <param name="hRecordObserv">Out param that returns a handle to the record.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordRequestObservation2(long index, System::IntPtr% hRecordObserv);

		/// <summary>Free the specified observation record.
		/// <param name="hRecordObserv">The record to free.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool RecordReleaseObservation2(System::IntPtr hRecordObserv);

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
		bool SettingsGetInfo2(System::IntPtr hRecordSettings, double% timeEffective, double% timeInstalled, double% timeRemoved, bool% useCal, bool% useTrans);

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
		bool SettingsSetChanTrans2(System::IntPtr hRecordSettings, long idxChannel, long xdTransformerTypeID, double xdSystemSideRatio, double xdMonitorSideRatio, cli::array<double>^ xdFreqResponse);

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
		bool SettingsSetChanCal2(System::IntPtr hRecordSettings, long idxChannel, double calTimeSkew, double calOffset, double calRatio, bool calMustUseARCal, cli::array<double>^ calApplied, cli::array<double>^ calRecorded);

		/// <summary>
		/// Get the channel count.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <returns>The number of channels.</returns>
		/// </summary>
		long SettingsGetChanCount2(System::IntPtr hRecordSettings);

		/// TODO - SettingsGetChannel is deprecated.
		bool SettingsGetChannel2(System::IntPtr hRecordSettings, long idxChannel, long% idxChannelDefn, long% triggerTypeID, double% fullScale, double% noiseFloor, double% triggerLow,
					double& triggerHigh, double% triggerRate, System::Array^% triggerShapeParam);

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
		bool SettingsGetChanTrans2(System::IntPtr hRecordSettings, long idxChannel, long% xdTransformerTypeID, double% xdSystemSideRatio, double% xdMonitorSideRatio, System::Array^% xdFreqResponse);

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
		bool SettingsGetChanCal2(System::IntPtr hRecordSettings, long idxChannel, double% calTimeSkew, double% calOffset, double% calRatio, bool% calMustUseARCal, System::Array^% calApplied, System::Array^% calRecorded);

		/// <summary>
		/// Add a channel to this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannelDefn">New channel.</param>
		/// <returns>Id of new channel or -1 if fails.</returns>
		/// </summary>
		long ObservationAddChannel2(System::IntPtr hRecordObserv, long idxChannelDefn);

		/// <summary>Get information for the series data in this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="idQuantityUnits"></param>
		/// <param name="idValueType"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesInfo2(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, int% idQuantityUnits, Guid% idValueType);

		/// <summary>Get information for the series data in this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="idQuantityUnits"></param>
		/// <param name="idQuantityCharacteristic"></param>
		/// <param name="idValueType"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesInfo3(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, int% idQuantityUnits, Guid% idQuantityCharacteristic, Guid% idValueType);

		/// <summary>
		/// Get trigger information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idTriggerMethod"></param>
		/// <param name="arTriggerChannels"></param>
		/// <param name="timeTriggered"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetTriggerInfo2(System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, CPQDIFTimeStamp% timeTriggered);

		/// <summary>
		/// Get trigger information for this observation record.
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idTriggerMethod"></param>
		/// <param name="arTriggerChannels"></param>
		/// <param name="timeTriggered"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetTriggerInfo2(System::IntPtr hRecordObserv, long% idTriggerMethod, Array^% arTriggerChannels, double% timeTriggered);

		/// <summary>
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationSetSeriesBaseQty2(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double value);

		/// <summary>
		/// <param name="hRecordObserv">The observation record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value"></param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ObservationGetSeriesBaseQty2(System::IntPtr hRecordObserv, long idxChannel, long idxSeries, double% value);

		/// <summary>
		/// Get the scalar data stored in this element.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="value">Out param that returns the scalar data stored in this element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool ScalarGetData2(System::IntPtr hElement, System::Object^% value);

		/// <summary>
		/// Get the scalar data at the specified index in this vector.
		/// <param name="hElement">Handle to the elememt, which should be a vector.</param>
		/// <param name="index">Index of datum in vector to retrieve.</param>
		/// <param name="value">Out param that returns the scalar data stored in this element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool VectorGetData2(System::IntPtr hElement, long index, System::Object^% value);

		/// <summary>
		/// Get the number of items in the vector contained in this element.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="count">Out param that takes the number of items in the vector contained in this element.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool VectorGetCount2(System::IntPtr hElement, long% count);

		/// <summary>
		/// Get the vector stored in this element.
		/// <param name="hElement">Handle to the elememt.</param>
		/// <param name="arValues">Out param that returns the vector as an array.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool VectorGetArray2(System::IntPtr  hElement, Array^% arValues);

		/// <summary>
		/// Check if the scalar in this element is a string.
		/// <param name="hElement">Handle to the element.</param>
		/// <returns>True if the data is a string; false if not.</returns>
		/// </summary>
		bool VectorIsString2(System::IntPtr hElement);

		/// <summary>Set the trigger low value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dblValue">New trigger low value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetTriggerLow(System::IntPtr hRecordSettings, long idxChannel, double dblValue);

		/// <summary>Set the trigger high value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dblValue">New trigger high value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetTriggerHigh(System::IntPtr hRecordSettings, long idxChannel, double dblValue);

		/// <summary>Set the trigger rate value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">New trigger rate value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetTriggerRate(System::IntPtr hRecordSettings, long idxChannel, double dblValue);

		/// <summary>Set the full scale value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">New full scale value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetFullScale(System::IntPtr hRecordSettings, long idxChannel, double dblValue);

		/// <summary>Set the noise floor value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">New noise floor value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetNoiseFloor(System::IntPtr hRecordSettings, long idxChannel, double dblValue);

		/// <summary>Set the nominal frequency for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dblValue">New nominal frequency value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetNominalFrequency(System::IntPtr hRecordSettings, double dblValue);

		/// <summary>Set the use calibration value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="value">New use calibration value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetUseCalibration(System::IntPtr hRecordSettings, bool value);

		/// <summary>Set the use transducer value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="value">New use transducer value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetUseTransducer(System::IntPtr hRecordSettings, bool value);

		/// <summary>
		/// Set the effective date value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dtVal">New effective date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetEffective(System::IntPtr hRecordSettings, CPQDIFTimeStamp% dtVal);

		/// <summary>
		/// Set the effective date value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dtVal">New effective date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetEffective(System::IntPtr hRecordSettings, double dtVal);

		/// <summary>
		/// Set the installed date value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dtVal">New installed date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetInstalled(System::IntPtr hRecordSettings, CPQDIFTimeStamp% dtVal);

		/// <summary>
		/// Set the installed date value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dtVal">New installed date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetInstalled(System::IntPtr hRecordSettings, double dtVal);

		/// <summary>
		/// Set the removed date value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dtVal">New removed date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetRemoved(System::IntPtr hRecordSettings, CPQDIFTimeStamp% dtVal);

		/// <summary>
		/// Set the removed date value for this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dtVal">New removed date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool SettingsSetRemoved(System::IntPtr hRecordSettings, double dtVal);

		/// <summary>
		/// Set the effective date value for this data source record.
		/// <param name="hRecordDS">The settings record.</param>
		/// <param name="dtVal">New effective date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetEffective(System::IntPtr hRecordDS, CPQDIFTimeStamp% dtVal);

		/// <summary>
		/// Set the effective date value for this data source record.
		/// <param name="hRecordDS">The settings record.</param>
		/// <param name="dtVal">New effective date value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetEffective(System::IntPtr hRecordDS, double dtVal);

		/// <summary>Set the series definition nominal value for this data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value">New nominal value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetSeriesDefnNominal(System::IntPtr hRecordDS, long idxChannel, long idxSeries, double value);

		/// <summary>Set the series definition digits for this data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value">New digits value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetSeriesDefnDigits(System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value);

		/// <summary>Set the series definition resolution for this data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="dblValue">New resolution value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetSeriesDefnResolution(System::IntPtr hRecordDS, long idxChannel, long idxSeries, double dblValue);

		/// <summary>Set the series definition display for this data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value">New display value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetSeriesDefnDisplay(System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value);

		/// <summary>Set the series definition units for this data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value">New units value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetSeriesDefnUnits(System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value);

		/// <summary>Set the series definition prefix for this data source record.
		/// <param name="hRecordDS">The data source record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="idxSeries">Index of the series.</param>
		/// <param name="value">New prefix value.</param>
		/// <returns>True if successful; false if not.</returns>
		/// </summary>
		bool DataSourceSetSeriesDefnPrefix(System::IntPtr hRecordDS, long idxChannel, long idxSeries, long value);

		/// <summary>Get the value of the trigger low from this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">Out param that returns the trigger low value.</param>
		/// <returns>True if successful; false if not a valid settings record.</returns>
		/// </summary>
		bool SettingsGetTriggerLow(System::IntPtr hRecordSettings, long idxChannel, double% dblValue);

		/// <summary>Get the value of the trigger high from this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">Out param that returns the trigger high value.</param>
		/// <returns>True if successful; false if not a valid settings record.</returns>
		/// </summary>
		bool SettingsGetTriggerHigh(System::IntPtr hRecordSettings, long idxChannel, double% dblValue);

		/// <summary>Get the value of the trigger rate from this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">Out param that returns the trigger rate.</param>
		/// <returns>True if successful; false if not a valid settings record.</returns>
		/// </summary>
		bool SettingsGetTriggerRate(System::IntPtr hRecordSettings, long idxChannel, double% dblValue);

		/// <summary>Get the full scale value from this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">Out param that returns the full scale value.</param>
		/// <returns>True if successful; false if not a valid settings record.</returns>
		/// </summary>
		bool SettingsGetFullScale(System::IntPtr hRecordSettings, long idxChannel, double% dblValue);

		/// <summary>Get the value of the noise floor from this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of the channel.</param>
		/// <param name="dblValue">Out param that returns the noise floor.</param>
		/// <returns>True if successful; false if not a valid settings record.</returns>
		/// </summary>
		bool SettingsGetNoiseFloor(System::IntPtr hRecordSettings, long idxChannel, double% dblValue);

		/// <summary>Get the value of the nominal frequency from this settings record.
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="dblValue">Out param that returns the nominal frequency.</param>
		/// <returns>True if successful; false if not a valid settings record.</returns>
		/// </summary>
		bool SettingsGetNominalFrequency(System::IntPtr hRecordSettings, double% dblValue);


		/// <summary>Get the value of the channel definition from this settings record.</summary>
		/// <param name="hRecordSettings">The settings record.</param>
		/// <param name="idxChannel">Index of channel whose definition is to be returned.</param>
		/// <param name="idxDefn">Out param that returns the index of the channel definition.</param>
		/// <returns>True if successful; false if not a valid settings record.</returns>
		bool SettingsGetChannelDefn(System::IntPtr hRecordSettings, long idxChannel, long% idxDefn);

		/// <summary>Convert DateTime to CPQDIFTimeStamp.</summary>
		/// <param name="dt">The DateTime to convert.</param>
		/// <returns>CPQDIFTimeStamp version of dt.</returns>
		static CPQDIFTimeStamp DateTimeToDT(System::DateTime dt);

		/// <summary>Convert CPQDIFTimeStamp to DateTime.</summary>
		/// <param name="pqdt">The CPQDIFTimeStamp to convert.</param>
		/// <returns>DateTime version of pqdt.</returns>
		static System::DateTime DTToDateTime(CPQDIFTimeStamp pqdt);
#pragma endregion
	};	// CPQDIFNet class.
}	// PQDiffNet namespace.
