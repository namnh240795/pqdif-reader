/*
**  Class:          CPQPtrArray
**  Description:
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


class CPQPtrArray
{
public:
	CPQPtrArray();
	~CPQPtrArray();

public:
    int GetSize( void ) const;
    void SetSize( int NewSize, int GrowBy = -1 );

    void * GetAt( int idx ) const;
    void SetAt( int idx, void * value );
    void SetAtGrow( int idx, void * value );

    void InsertAt( int indexToInsert, void * prec, int nCount = 1 );
    void RemoveAt( int indexToInsert );

    int Add( void * value );

    void * operator[] ( int idx ) const;

#ifdef _DEBUG
    bool Test( void );
#endif

protected:
    void **  m_data;
    int     m_size;
    int     m_max;
    int     m_growBy;
};
