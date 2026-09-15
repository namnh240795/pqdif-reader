/*
**  Class:
**  Description:	This file is included just after the windows.h and STL library includes
**					using namespace std; declaration and just before the standard PQDIF headers
**					in PQDIF_classes.h
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


#ifdef __BORLANDC__
#define _stricmp stricmp
#else
#if _MSC_VER >= 1100
#include <crtdbg.h>
#endif
#include <comdef.h>
#include <atlconv.h>
#endif

#undef ASSERT
#define ASSERT(x)

#undef ASSERT_VALID
#define ASSERT_VALID(x)

#undef _T
#define _T
