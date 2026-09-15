/*
**  Class:          CPQDIF_E_Collection
**  Description:    Implements the PQDIF collection element
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



//  Local constants
const char cTerminator = '\0';


//  Relational operators for for ordering a collection.
bool operator<( const CPQDIF_Element * pel, const GUID &tag )
{
    return ( memcmp( &(pel->GetTag()), &tag, sizeof( tag ) ) < 0 );
}

bool operator<( const GUID &tag, const CPQDIF_Element * pel )
{
    return ( memcmp( &tag, &(pel->GetTag()), sizeof( tag ) ) < 0 );
}

class CPQDIF_Element_Less : public less<const GUID *>
{
public:
     
    bool operator() ( const CPQDIF_Element * pel, const GUID &tag )
    {
        return ( memcmp( &(pel->GetTag()), &tag, sizeof( tag ) ) < 0 );
    }

    bool operator() ( const GUID &tag, const CPQDIF_Element * pel )
    {
        return ( memcmp( &tag, &(pel->GetTag()), sizeof( tag ) ) < 0 );
    }

    bool operator()( const CPQDIF_Element *pel1, const CPQDIF_Element *pel2 )
    {
        return ( memcmp(  &(pel1->GetTag()), &(pel2->GetTag()), sizeof( GUID ) ) < 0 );
    }

} g_pqdif_element_less;



//  Construction
//  ============

CPQDIF_E_Collection::CPQDIF_E_Collection()
{
    m_array.reserve( 32 );
}


CPQDIF_E_Collection::~CPQDIF_E_Collection()
{

    //  Destroy all elements in the array
    CArrayElements::iterator iter;
    for( iter = m_array.begin(); iter != m_array.end(); iter++ )
    {
        delete *iter;
        *iter = NULL;
    }

}


long CPQDIF_E_Collection::GetCount( void ) const
{
    return static_cast<long>(m_array.size());
}


CPQDIF_Element * CPQDIF_E_Collection::GetElement( long index ) const
{
    CPQDIF_Element *    pel = NULL;

    if( index >= 0 && index < GetCount() )
    {
        pel = m_array[ index ];
    }

    return pel;
}


CPQDIF_Element * CPQDIF_E_Collection::GetElement( const GUID& tag, long elementType ) const
{
    CPQDIF_Element *  pelReturn = NULL;

    //  See if specified element is in the collection.
    CArrayElements::const_iterator iter;
    iter = lower_bound( m_array.begin(), m_array.end(), tag, g_pqdif_element_less );
    while( pelReturn == NULL
        && iter != m_array.end()
        && *iter != NULL
        && PQDIF_IsEqualGUID( (*iter)->GetTag(), tag ) )
    {
        //  Matching type.
        if( elementType == -1
            || elementType == (*iter)->GetElementType() )
        {
            pelReturn = *iter;
        }
        iter++;
    }

    return pelReturn;
}


void CPQDIF_E_Collection::SetRecord( CPQDIFRecord * pRecord )
{
    CPQDIF_Element *    pel;

    //  Call the base class method.
    CPQDIF_Element::SetRecord( pRecord );

    //  Do the same thing to the objects we own
    CArrayElements::iterator iter;
    for( iter = m_array.begin(); iter != m_array.end(); iter++ )
    {
        pel = *iter;
        ASSERT_VALID( pel );
        if( pel )
        {
            pel->SetRecord( pRecord );
        }
    }

    return;
}


void CPQDIF_E_Collection::Add( CPQDIF_Element * pel )
{

    //  Add the element to the collection.
    CArrayElements::iterator iter = upper_bound( m_array.begin(), m_array.end(), pel->GetTag(), g_pqdif_element_less );
    m_array.insert( iter, pel );

    //  Set the element to reference the collections record.
    if( pel )
        pel->SetRecord( GetRecord() );
    
    //  Signal that the record has changed.
    if( m_pRecord )
        m_pRecord->SetChanged( true );
}


//  Should this delete the element or not?
void CPQDIF_E_Collection::RemoveAt( long index )
{

    //  If a valid element was specified then ....
    ASSERT( index >= 0 && index < GetCount() );
    if( index >= 0 && index < GetCount() )
    {

        //  Dereference the element.
        CArrayElements::iterator where = m_array.begin();
        where += index;
        CPQDIF_Element * pel = *where;

        //  Clear the element's reference to the record.
        if( pel )
            pel->SetRecord( NULL );

        //  Remove it from the list.
        m_array.erase( where );

        //  Mark the collection as dirty.
        if( m_pRecord )
            m_pRecord->SetChanged( true );
    
    }
}


void CPQDIF_E_Collection::AddOrReplace( CPQDIF_Element * pel )
{

    bool                bFound = false;
    CPQDIF_Element *    pelToCompare = NULL;
    const GUID &        tagToAdd = pel->GetTag();

    //  See if this tag already exists
    CArrayElements::iterator iter;
    iter = lower_bound( m_array.begin(), m_array.end(), tagToAdd, g_pqdif_element_less );
    if( iter != m_array.end() )
    {
        pelToCompare = *iter;
        if( pelToCompare )
        {

            //  Matching tags?
            if( PQDIF_IsEqualGUID( pelToCompare->GetTag(), tagToAdd ) )
            {

                //  Delete the old element.
                delete pelToCompare;

                //  Add the new element to the collection.
                *iter = pel;

                //  Signal that the element is found.
                bFound = true;            
            
            }
        }
    }

    // If an existing element was not found then add the element.
    if( !bFound )
    {
        m_array.insert( iter, pel );
    }

    //  Set the element to reference the collections record.
    if( pel )
        pel->SetRecord( GetRecord() );
    
    //  Signal that the record has changed.
    if( m_pRecord )
        m_pRecord->SetChanged( true );
        
}


void CPQDIF_E_Collection::SetVectorString
            ( 
            const   GUID&   tagElement,
            const   char *  text,
                    bool    allowReplace
            )
{
    CPQDIF_E_Vector *   pvectString;

    if( text )
    {
        //  Create the new vector to hold the string
        pvectString = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR );
        if( pvectString )
        {
            //  Set it up ...
            pvectString->SetTag( tagElement );
            pvectString->SetPhysicalType( ID_PHYS_TYPE_CHAR1 );
            pvectString->SetValues( text );

            //  Add it
            if( allowReplace )
                AddOrReplace( pvectString );
            else
                Add( pvectString );
        }
    }
}


//  Parameterized SetVector...() functions
//  ======================================
#define SETVECTOR( nametype, type, idtype ) \
void CPQDIF_E_Collection::SetVector##nametype \
        (   const   GUID&   tagElement, \
            const   type *  values, \
                    long    count, \
                    bool    allowReplace ) \
{ \
    CPQDIF_E_Vector *   pvect; \
    if( values ) \
    { \
        /*  Create the new vector to hold the string */ \
        pvect = (CPQDIF_E_Vector *) theFactory.NewElement( ID_ELEMENT_TYPE_VECTOR ); \
        if( pvect ) \
        { \
            /*  Set it up ... */ \
            pvect->SetTag( tagElement ); \
            pvect->SetPhysicalType( idtype ); \
            pvect->SetValues##nametype( values, count ); \
            /*  Add it */ \
            if( allowReplace ) \
                AddOrReplace( pvect ); \
            else \
                Add( pvect ); \
        } \
    } \
}


SETVECTOR( INT1, INT1, ID_PHYS_TYPE_INTEGER1 );
SETVECTOR( INT2, INT2, ID_PHYS_TYPE_INTEGER2 );
SETVECTOR( INT4, INT4, ID_PHYS_TYPE_INTEGER4 );
SETVECTOR( UINT4, UINT4, ID_PHYS_TYPE_UNS_INTEGER4 );
SETVECTOR( REAL4, REAL4, ID_PHYS_TYPE_REAL4 );
SETVECTOR( REAL8, REAL8, ID_PHYS_TYPE_REAL8 );
SETVECTOR( TimeStamp, TIMESTAMPPQDIF, ID_PHYS_TYPE_TIMESTAMPPQDIF );


bool CPQDIF_E_Collection::GetVectorString
            ( 
            const   GUID&   tagElement,
                    char *  text,
                    long    max
            ) const
{
            bool                status = false;
    const   CPQDIF_Element *    pel;
            string              values;

    pel = GetElement( tagElement );
    if( pel && pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR )
    {
        const CPQDIF_E_Vector *   pvect = (const CPQDIF_E_Vector *) pel;
        status = pvect->GetValues( values );
        if( status )
        {
            strncpy( text, values.c_str(), max );
            text[ max - 1 ] = cTerminator;
        }
    }

    return status;
}


//  Parameterized GetVector...() functions
//  ======================================
#define GETVECTOR( nametype, type ) \
long CPQDIF_E_Collection::GetVector##nametype \
            (  \
            const   GUID&   tagElement, \
                    type *  values, \
                    long    max \
            ) const \
{ \
            long                sizeActual = 0; \
    const   CPQDIF_Element *    pel; \
    pel = GetElement( tagElement ); \
    if( pel && pel->GetElementType() == ID_ELEMENT_TYPE_VECTOR ) \
    { \
        const CPQDIF_E_Vector *   pvect = (const CPQDIF_E_Vector *) pel; \
        sizeActual = pvect->GetValues##nametype( values, max ); \
    } \
    return sizeActual; \
}


GETVECTOR( INT1, INT1 );
GETVECTOR( INT2, INT2 );
GETVECTOR( INT4, INT4 );
GETVECTOR( UINT4, UINT4 );
GETVECTOR( REAL4, REAL4 );
GETVECTOR( REAL8, REAL8 );
GETVECTOR( TimeStamp, TIMESTAMPPQDIF );


//  Parameterized SetScalar...() functions
//  ======================================
#define SETSCALAR( nametype, type ) \
void CPQDIF_E_Collection::SetScalar##nametype \
            (  \
            const   GUID&   tagElement, \
                    type    value, \
                    bool    allowReplace \
            ) \
{ \
    CPQDIF_E_Scalar *   psc; \
    psc = (CPQDIF_E_Scalar *) theFactory.NewElement( ID_ELEMENT_TYPE_SCALAR ); \
    if( psc ) \
    { \
        psc->SetTag( tagElement ); \
        psc->SetValue##nametype( value ); \
        if( allowReplace ) \
            AddOrReplace( psc ); \
        else \
            Add( psc ); \
    } \
}


SETSCALAR( BOOL4, bool )
SETSCALAR( INT2, INT2 )
SETSCALAR( UINT2, UINT2 )
SETSCALAR( INT4, INT4 )
SETSCALAR( UINT4, UINT4 )
SETSCALAR( REAL4, REAL4 )
SETSCALAR( REAL8, REAL8 )
SETSCALAR( COMPLEX8, COMPLEX8 )
SETSCALAR( COMPLEX16, COMPLEX16 )
SETSCALAR( GUID, GUID )
SETSCALAR( TimeStamp, TIMESTAMPPQDIF )


//  Parameterized GetScalar...() functions
//  ======================================
#define GETSCALAR( nametype, type ) \
bool CPQDIF_E_Collection::GetScalar##nametype \
        ( \
        const   GUID&       tagElement, \
                type&       value \
        ) const \
{ \
    bool    status = false; \
    const CPQDIF_Element *    pel; \
    pel = GetElement( tagElement ); \
    if( pel && pel->GetElementType() == ID_ELEMENT_TYPE_SCALAR ) \
    { \
        const CPQDIF_E_Scalar *   psc = (const CPQDIF_E_Scalar *) pel; \
        status = psc->GetValue##nametype( value ); \
    } \
    return status; \
}


GETSCALAR( BOOL4, bool )
GETSCALAR( INT2, INT2 )
GETSCALAR( UINT2, UINT2 )
GETSCALAR( INT4, INT4 )
GETSCALAR( UINT4, UINT4 )
GETSCALAR( REAL4, REAL4 )
GETSCALAR( REAL8, REAL8 )
GETSCALAR( COMPLEX8, COMPLEX8 )
GETSCALAR( COMPLEX16, COMPLEX16 )
GETSCALAR( GUID, GUID )
GETSCALAR( TimeStamp, TIMESTAMPPQDIF )
