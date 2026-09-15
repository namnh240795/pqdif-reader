/*
**  Class:
**  Description:    Definitions of some helper methods for CPQDiffNet.
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

#include <string>

/// <summary>Convert an ASCII string to a wide/unicode string.</summary>
std::wstring& s2ws(const std::string& src, std::wstring& dst);

/// <summary>Convert a "C" ASCII string to a wide/unicode string.</summary>
std::wstring c2ws(char const *src);

/// <summary>Convert a UNICODE string to an ASCII string.</summary>
std::string wc2s(wchar_t const *src);

/// <summary>Convert wstring to string.</summary>
std::string &ws2s(const std::wstring& src, std::string& dst);
