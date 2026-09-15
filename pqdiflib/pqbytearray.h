/*
**  Class:          CPQByteArray
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

class CPQByteArray
{
public:
	CPQByteArray();
	~CPQByteArray();

public:
    inline int GetSize( void ) const
    {
        return m_size;
    }
    bool SetSize( int NewSize, int GrowBy = -1 );

    inline BYTE GetAt( int idx ) const
    {
        ASSERT( m_data != NULL && idx >= 0 && idx < m_size );
        return m_data[ idx ];
    }
    inline void SetAt( int idx, BYTE value )
    {
        ASSERT( m_data != NULL && idx >= 0 && idx < m_size );
        m_data[ idx ] = value;
    }
    BYTE& ElementAt( int idx ) const
    {
        ASSERT( m_data != NULL && idx >= 0 && idx < m_size );
        return m_data[ idx ];
    }

    inline const BYTE * CPQByteArray::GetData( void ) const { return m_data; }
    inline BYTE * CPQByteArray::GetData( void ) { return m_data; }

    int Add( BYTE value );
    int Append( BYTE * values, int count );

    BYTE operator[] ( int idx ) const
    {
        ASSERT( m_data != NULL && idx >= 0 && idx < m_size );
        return m_data[ idx ];
    }
    BYTE& operator[] ( int idx )
    {
        ASSERT( m_data != NULL && idx >= 0 && idx < m_size );
        return m_data[ idx ];
    }

protected:
    BYTE *  m_data;
    int     m_size;
    int     m_max;
    int     m_growBy;
};
