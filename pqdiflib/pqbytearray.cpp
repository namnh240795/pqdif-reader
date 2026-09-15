/*
**  Class:
**  Description:
**  --------------------------------------------------------------------------
**
**  Copyright 2021 PQDIF Authors
**
**  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
**  either express or implied. See the License for the specific language governing permissions and limitations under the License.
**
**  See the LICENSE file distributed with this work for copyright and licensing information, the AUTHORS file for a list of
**  copyright holders, and the CONTRIBUTORS file for the list of contributors.
**
**  SPDX - License - Identifier: Apache - 2.0
*/


#include "PQDIF_classes.h"


CPQByteArray::CPQByteArray()
{
    m_data = NULL;
    m_size = 0;
    m_max = 0;
    m_growBy = 64;
}


CPQByteArray::~CPQByteArray()
{
    if( m_data )
    {
        free( m_data );
    }
}


bool CPQByteArray::SetSize( int NewSize, int GrowBy )
{

    if( GrowBy > 0 )
        m_growBy = GrowBy;

    //  Grow?
    if( NewSize > m_max )
    {
        int max = (( NewSize/m_growBy ) + 1)*m_growBy;
        BYTE * data = (BYTE *) realloc( (void *) m_data, max );
		if( data != NULL || max == 0 )
		{
			m_data = data;
			m_size = NewSize;
			m_max = max;
		}
    }
	else
	{
		m_size = NewSize;
	}

    return ( m_size == NewSize );

}


int CPQByteArray::Add( BYTE value )
{
    int     pos = m_size;

    if( SetSize( m_size + 1 ) )
    {
        m_data[ pos ] = value;
        return pos;
    }
    else
    {
        return -1;
    }
}


int CPQByteArray::Append( BYTE * values, int count )
{
    int     pos = m_size;

    if( SetSize( m_size + count, m_growBy ) )
    {
        memcpy( m_data + pos, values, count );
        return pos;
    }
    else
    {
        return -1;
    }
}



