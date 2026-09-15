/*
**  Class
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

#include "PQDIF_classes.h"

const   int sizeElement = sizeof( void * );


CPQPtrArray::CPQPtrArray()
{
    m_data = (void **) malloc( 16 * sizeElement );
    m_size = 0;
    m_max = 16;
    m_growBy = 16;
}


CPQPtrArray::~CPQPtrArray()
{
    if( m_data )
    {
        free( m_data );
    }
}


int CPQPtrArray::GetSize( void ) const
{
    return m_size;
}


void CPQPtrArray::SetSize( int NewSize, int GrowBy )
{
    m_size = NewSize;
    if( GrowBy > 0 )
        m_growBy = GrowBy;

    //  Grow?
    if( NewSize > m_max )
    {
        m_max = NewSize + m_growBy;
        m_data = (void **) realloc( (void *) m_data, m_max * sizeElement );
    }
}


void * CPQPtrArray::GetAt( int idx ) const
{
    void *  bogus = NULL;

    if( idx >=0 && idx < m_size )
    {
        return m_data[ idx ];
    }
    else
    {
        return bogus;
    }
}


void CPQPtrArray::SetAt( int idx, void * value )
{
    if( idx >= 0 && idx < m_size )
    {
        m_data[ idx ] = value;
    }
}


void CPQPtrArray::SetAtGrow(int nIndex, void* newElement)
    {
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);

	if (nIndex >= m_size)
		SetSize(nIndex+1);
	m_data[nIndex] = newElement;

    }

void CPQPtrArray::InsertAt( int nIndex, void * newElement, int nCount )
{
	if (nIndex >= m_size)
	    {
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so indexToInsert is valid
	    }
    else
        {
		// inserting in the middle of the array
		int nOldSize = m_size;
		SetSize(m_size + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_data[nIndex+nCount], &m_data[nIndex],
			(nOldSize-nIndex) * sizeof(void*));

		// re-init slots we copied from

		memset(&m_data[nIndex], 0, nCount * sizeof(void*));

        }

	// insert new value in the gap
	ASSERT(nIndex + nCount <= m_size);
	while (nCount--)
		m_data[nIndex++] = newElement;

}

void CPQPtrArray::RemoveAt( int indexToRemove )
{
    if( indexToRemove >= 0 && indexToRemove < m_size )
    {
        for( int idx = indexToRemove + 1; idx < m_size; idx++ )
        {
            m_data[ idx - 1 ] = m_data[ idx ];
        }

        SetSize( GetSize() - 1 );
    }
}


int CPQPtrArray::Add( void * value )
{
    int     pos = m_size;

    SetSize( m_size + 1, m_growBy );
    if( m_data )
    {
        m_data[ pos ] = value;
        return pos;
    }
    else
    {
        return -1;
    }
}


void * CPQPtrArray::operator[] ( int idx ) const
{
    return GetAt( idx );
}


#ifdef _DEBUG
bool CPQPtrArray::Test( void )
{
    bool    failed = false;
    void *  ptr1 = (void *) 1;
    void *  ptr2 = (void *) 2;
    void *  ptr3 = (void *) 3;
    void *  ptr4 = (void *) 4;

    SetSize( 16, 4 );
    if( m_size != 16 || m_growBy != 4 )
        failed = true;

    SetSize( 0 );
    if( m_size != 0 || m_growBy != 4 )
        failed = true;

    Add( ptr1 );
    Add( ptr2 );
    if( m_size != 2 )
        failed = true;
    if( m_data[0] != ptr1 || m_data[1] != ptr2 )
        failed = true;

    Add( ptr3 );
    InsertAt( 1, ptr4 );
    if( m_size != 4 )
        failed = true;
    if( m_data[0] != ptr1 || m_data[1] != ptr4 || m_data[2] != ptr2 || m_data[3] != ptr3 )
        failed = true;    

    SetSize( 2, 16 );
    if( m_size != 2 )
        failed = true;
    if( m_growBy != 16 )
        failed = true;
    if( m_data[0] != ptr1 || m_data[1] != ptr4 )
        failed = true;    

    SetSize( 3 );
    if( m_size != 3 )
        failed = true;
    if( m_growBy != 16 )
        failed = true;

    if( failed )
    {
        //TRACE( "CPQPtrArray::Test - *FAILED*" );
        return false;
    }

    return true;
}
#endif

