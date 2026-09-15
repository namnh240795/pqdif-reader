/*
**  Class:			CPQDIF_MemoryStreamIO
**  Description:	Implementation of stream IO that uses a managed MemoryStream class.
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

#include "PQDIF_classes.h"
#include "str_memory.h" //  System.IO.MemoryStream implementation.


using namespace System;
using namespace System::IO;


//  Local constants
const   long    sizeDefaultGrowBy = 16 * 1024;


//  Construction
//  ============


CPQDIF_MemoryStreamIO::CPQDIF_MemoryStreamIO(System::IO::MemoryStream^ pstrm)
{
	m_pstrm = pstrm;
	m_canWriteFull = TRUE;
	m_canWriteInc = TRUE;
}


CPQDIF_MemoryStreamIO::~CPQDIF_MemoryStreamIO()
{
	m_canWriteFull = FALSE;
	m_canWriteInc = FALSE;
}


// Operations
bool CPQDIF_MemoryStreamIO::SeekPos(long pos)
{
	bool status = true;

	// We know memory streams support Seek so we don't test.

	// If pos is beyond the current size of the stream, set
	// the size to that value before seeking.
	if (m_pstrm->Length < pos)
	{
		m_pstrm->SetLength(pos);
	}

	m_pstrm->Seek(pos, System::IO::SeekOrigin::Begin);

	return status;
}


bool CPQDIF_MemoryStreamIO::GetPos(long& pos)
{
	bool status = true;

	pos = (long)m_pstrm->Position;

	return status;
}


bool CPQDIF_MemoryStreamIO::SeekEnd(void)
{
	bool status = true;

	m_pstrm->Seek(0, System::IO::SeekOrigin::End);

	return status;
}



BYTE * CPQDIF_MemoryStreamIO::ReadBlock(long size, long& actualSize)
{
	BYTE *  buffRet = NULL;
	long    sizeRead = 0;
	bool    status = FALSE;

	//  Init
	sizeRead = 0;

	//  Clear buffers
	m_buffRead.SetSize(0);
	m_posRead = 0;
	m_buffWrite.SetSize(0);
	m_posWrite = 0;

	try
	{
		//  Read a buffer from the stream.
		cli::array<System::Byte>^ _Data = gcnew cli::array<System::Byte>(size);
		sizeRead = m_pstrm->Read(_Data, 0, size);

		// Copy to our unmanaged buffer.
		if (sizeRead > 0)
		{
			//  Size the buffer first
			m_buffRead.SetSize(size);

			// Copy it.
			System::Runtime::InteropServices::Marshal::Copy(_Data, 0, IntPtr((void *)m_buffRead.GetData()), sizeRead);

			//  Decode it.
			m_buffRead.SetSize(sizeRead);
			status = ExecuteProcessorDecode();
			if (status)
			{
				//  Pass back the decoded buffer
				buffRet = m_buffWrite.GetData();
				actualSize = (long)m_buffWrite.GetSize();
			}
		}
	}
	catch (...)
	{
		;
	}

	return buffRet;
}


bool CPQDIF_MemoryStreamIO::WriteBlock(long &sizeActual)
{
	bool    status = false;

	m_buffWrite.SetSize(0);
	m_posWrite = 0;
	sizeActual = 0;

	try
	{
		//  Encode the block
		status = ExecuteProcessorEncode();
		if (status)
		{
			//  The write buffer should contain the output
			sizeActual = m_buffWrite.GetSize();

			// Copy to a managed array of sizeActual bytes.
			cli::array<System::Byte>^ _Data = gcnew cli::array<System::Byte>(sizeActual);
			System::Runtime::InteropServices::Marshal::Copy(IntPtr((void *)m_buffWrite.GetData()), _Data, 0, sizeActual);

			// Write to the end of our memory stream.
			m_pstrm->Write(_Data, 0, sizeActual);
			status = true;
		}
	}
	catch (...)
	{
		;
	}

	return status;
}





