/*
**  Class:
**  Description:	Helper methods for CPQDIFNet.
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
#include "utils.h"

using namespace std;

/// <summary>
/// Convert an ASCII string to a wide/unicode string.
/// <param name="src">Ascii string to convert.</param>
/// <param name="dst">Holds the converted string.</param>
/// <returns>dst passed by reference</returns>
/// </summary>
std::wstring& s2ws(const std::string& src, std::wstring& dst)
{
	int len;
	int slength = (int)src.length() + 1;

	len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, src.c_str(), slength, buf, len);
	dst = buf;
	delete[] buf;

	return dst;
}


/// <summary>
/// Convert an ASCII string to a wide/unicode string.
/// <param name="src">Ascii string to convert.</param>
/// <returns>New wide character string.</returns>
/// </summary>
std::wstring c2ws(char const *src)
{
	int len;
	int slength = (int)strlen(src) + 1;

	len = MultiByteToWideChar(CP_ACP, 0, src, slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, src, slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	return r;
}

/// <summary>
/// Convert a UNICODE string to an ASCII string.
/// <param name="src">UNICODE string to convert.</param>
/// <returns>New ASCII string.</returns>
/// </summary>
std::string wc2s(wchar_t const *src)
{
	int len;
	int wlength = 0;

	if (src != NULL)
	{
		wlength = (int)wcslen(src) + 1;
		len = WideCharToMultiByte(CP_ACP, 0, src, wlength, NULL, 0, NULL, NULL);
		char* buf = new char[len];
		WideCharToMultiByte(CP_ACP, 0, src, wlength, buf, len, NULL, NULL);
		std::string r(buf);
		delete[] buf;
		return r;
	}
	else
	{
		return std::string("");
	}
}


/// <summary>
/// Convert wide character string to ASCII string.
/// <param name="src">Wide character string to convert.</param>
/// <param name="dst">Holds the converted string.</param>
/// <returns>dst passed by reference</returns>
/// </summary>
std::string &ws2s(const std::wstring& src, std::string& dst)
{

	if (!src.empty())
	{
		int length = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
		dst.resize( length - 1 );
		WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &dst[0], length, NULL, NULL);
	}
	else
	{
		dst.clear();
	}

	return dst;
}