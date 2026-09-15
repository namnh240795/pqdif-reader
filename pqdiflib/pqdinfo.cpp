/*
**  Class:          CPQDIF_Info
**  Description:    Implements informational member functions so that certain types of information can be encapsulated here. 
**                  This allows us to concentrate knowledge at one point so it can be more easily managed.
**  
**                  This class is a Singleton class; this means that only one instance of it
**                  should ever be created. This instance is made global, and is one of the
**                  only global objects in the entire system.
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

//  The one and only information object (Singleton)
//  ===================================
CPQDIF_Info  theInfo;


//  String for unrecognized tags and IDs.
static const char * szUnrecognized = "< Unrecognized >";
static const char * szEmpty = "";

//  Sorting predicated for Name Info classes
class CLess_NI_Tag // : public less<NI_Tag>
{
public:
    inline bool operator()(const NI_Tag &x, const NI_Tag &y) const
    {
        return ( memcmp( &x.m_tag, &y.m_tag, sizeof( GUID ) ) < 0 );
    }
};


class CLess_NI_ID_GUID // : public less<NI_ID_GUID>
{
public:
    inline bool operator()(const NI_ID_GUID &x, const NI_ID_GUID &y) const
    {
        return ( memcmp( &x.m_id, &y.m_id, sizeof( GUID ) ) < 0 );
    }
};


class CLess_NI_ID_Int // : public less<NI_ID_Int>
{
public:
    bool operator()(const NI_ID_Int &x, const NI_ID_Int &y) const
    {
        int iResult = memcmp( &x.m_tag, &y.m_tag, sizeof( GUID ) );
        if( iResult == 0 )
        {
            if( x.m_id < y.m_id )
                iResult = -1;
        }
        return iResult < 0;
    }
};


//  Local macros for adding entries
//  -------------------------------
#ifdef _PQDIF_ALIAS_TAG
    #define ADD_TAG_ENTRY( _tag ) \
        nameTag.m_tag = _tag; \
        nameTag.m_name = #_tag; \
        nameTag.m_alias = (void *)_PQDIF_ALIAS_##_tag; \
        m_listTags.push_back( nameTag );
#else
    #define ADD_TAG_ENTRY( _tag ) \
        nameTag.m_tag = _tag; \
        nameTag.m_name = #_tag; \
        m_listTags.push_back( nameTag );
#endif


#ifdef _PQDIF_ALIAS_ID

    #define ADD_ID_GUID_ENTRY( _id, _tag ) \
        nameGuid.m_id = _id; \
        nameGuid.m_tag = _tag; \
        nameGuid.m_name = #_id; \
        nameGuid.m_alias = (void *)_PQDIF_ALIAS_##_id; \
        m_listGUIDs.push_back( nameGuid );

    #define ADD_ID_UINT4_ENTRY( _id, _tag )\
        nameInt.m_id = _id; \
        nameInt.m_tag = _tag; \
        nameInt.m_name = #_id; \
        nameInt.m_alias = (void *)_PQDIF_ALIAS_##_id; \
        m_listInts.push_back( nameInt );

#else

    #define ADD_ID_GUID_ENTRY( _id, _tag ) \
        nameGuid.m_id = _id; \
        nameGuid.m_tag = _tag; \
        nameGuid.m_name = #_id; \
        m_listGUIDs.push_back( nameGuid );

    #define ADD_ID_UINT4_ENTRY( _id, _tag )\
        nameInt.m_id = _id; \
        nameInt.m_tag = _tag; \
        nameInt.m_name = #_id; \
        m_listInts.push_back( nameInt );

#endif


#define ADD_TYPE_ENTRY( _type )\
    nameType.m_type = _type; \
    nameType.m_name = #_type; \
    m_listTypes.push_back( nameType );




//  Construction
//  ============

BYTE CPQDIF_Info::m_sizeDataTable[] =
{
	 1,  1,  2,  4,  1,  1,  1,  1,  1,  1,
	 1,  2,  1,  1,  1,  1,  1,  1,  1,  1,
	 1,  2,  4,  1,  1,  1,  1,  1,  1,  1,
	 1,  2,  4,  1,  1,  1,  1,  1,  1,  1,
	 4,  8,  8, 16,  1,  1,  1,  1,  1,  1,
	12,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	16,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};


CPQDIF_Info::CPQDIF_Info()
{
    NI_Tag      nameTag;
    NI_ID_GUID  nameGuid;
    NI_ID_Int   nameInt;
    NI_Type     nameType;


    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_BOOLEAN1 ) == sizeof( BOOL1 ) );
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_BOOLEAN2 ) == sizeof( BOOL2 ) );
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_BOOLEAN4 ) == sizeof( BOOL4 ) );
    
	ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_CHAR1 ) == sizeof( CHAR1 ) );
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_CHAR2 ) == sizeof( CHAR2 ) );
    
	ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_INTEGER1 ) == sizeof( INT1 ) );
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_INTEGER2 ) == sizeof( INT2 ) );
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_INTEGER4 ) == sizeof( INT4 ) );
    
	ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_UNS_INTEGER1 ) == sizeof( UINT1 ) );      
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_UNS_INTEGER2 ) == sizeof( UINT2 ) );       
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_UNS_INTEGER4 ) == sizeof( UINT4 ) );
    
	ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_REAL4 ) == sizeof( REAL4 ) );       
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_REAL8 ) == sizeof( REAL8 ) );
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_COMPLEX8 ) == sizeof( COMPLEX8 ) );     
    ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_COMPLEX16 ) == sizeof( COMPLEX16 ) );
    
	ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_TIMESTAMPPQDIF ) == sizeof( TIMESTAMPPQDIF ) );
    
	ASSERT( GetNumBytesOfType( ID_PHYS_TYPE_GUID ) == sizeof( GUID ) );

    //  Treat the 'blank' tag a little differently
    nameTag.m_tag = tagBlank;
    nameTag.m_name = "< Blank >";
    m_listTags.push_back( nameTag );

    //  Tag names
    #include "name_tag.inc"
    #ifdef _PQDIF_EXTENDED_TAGS
        #include "name_tag_extended.inc"
    #endif

    //  ID names
    #include "name_id.inc"
    #ifdef _PQDIF_EXTENDED_TAGS
        #include "name_id_extended.inc"
    #endif

    //  Physical type names
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_BOOLEAN1 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_BOOLEAN2 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_BOOLEAN4 );

    ADD_TYPE_ENTRY( ID_PHYS_TYPE_CHAR1 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_CHAR2 );

    ADD_TYPE_ENTRY( ID_PHYS_TYPE_INTEGER1 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_INTEGER2 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_INTEGER4 );

    ADD_TYPE_ENTRY( ID_PHYS_TYPE_UNS_INTEGER1 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_UNS_INTEGER2 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_UNS_INTEGER4 );

    ADD_TYPE_ENTRY( ID_PHYS_TYPE_REAL4 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_REAL8 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_COMPLEX8 );
    ADD_TYPE_ENTRY( ID_PHYS_TYPE_COMPLEX16 );

    ADD_TYPE_ENTRY( ID_PHYS_TYPE_TIMESTAMPPQDIF );

    ADD_TYPE_ENTRY( ID_PHYS_TYPE_GUID );

    //  Sort the lists.
    sort( m_listTags.begin(), m_listTags.end(), CLess_NI_Tag() );
    sort( m_listGUIDs.begin(), m_listGUIDs.end(), CLess_NI_ID_GUID() );
    sort( m_listInts.begin(), m_listInts.end(), CLess_NI_ID_Int() );


    #ifdef TEST

        #ifndef TRACE
            #define TRACE printf
        #endif

        //  Check out each tag.
        int iCount = GetTagCount();
        int iTag;
        for( iTag = 0; iTag < iCount; iTag++)
        {
            GUID tag, tag1;
            GUID guidID, guidID1;
            int iID, iID1;
            string s;

            GetTagGUID( iTag, tag );
            GetTagName( iTag, s );
            ASSERT( s == GetNameOfTag( tag ) );
            ASSERT( s == GetNameTag( tag ) );
            ASSERT( s == GetAliasTag( tag ) );
            ASSERT( GetTagFromName( s.c_str(), tag1 ) && tag == tag1 );
            TRACE( "%s %s\n", GetNameTag( tag ), GetAliasTag( tag ) );

            //  Check out each integer value for this tag.
            int iCountInt = GetIDCountInt( tag );
            int iInt;
            for( iInt = 0; iInt < iCountInt; iInt++ )
            {
                s = GetIDNameInt( tag, iInt, iID );
                ASSERT( s == GetNameOfID( tag, iID ) );
                ASSERT( s == GetNameID( tag, iID ) );
                ASSERT( GetIDFromName( s.c_str(), tag1, iID1 )
                    && tag == tag1
                    && iID == iID1 );
                TRACE( "    %s %s\n", GetNameID( tag, iID ), GetAliasID( tag, iID ) );
            }

            //  Check out each GUID value for this tag.
            int iCountGUID = GetIDCountGUID( tag );
            int iGUID;
            for( iGUID = 0; iGUID < iCountGUID; iGUID++ )
            {
                s = GetIDNameGUID( tag, iGUID, guidID );
                ASSERT( s == GetNameOfID( guidID ) );
                ASSERT( s == GetNameID( guidID ) );
                ASSERT( GetIDFromName( s.c_str(), guidID1 )
                    && guidID == guidID1 );
                TRACE( "    %s %s\n", GetNameID( guidID ), GetAliasID( guidID ) );
            }

        }

    #endif

    return;
}


CPQDIF_Info::~CPQDIF_Info()
{
}


const char * CPQDIF_Info::GetNameOfTag( const GUID& tag )
{
    const char *szName = szUnrecognized;

    NI_Tag * pInfo = _FindTag( tag );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
}


const char * CPQDIF_Info::GetNameOfID( const GUID& id )
{
    const char *szName = szUnrecognized;

    NI_ID_GUID * pInfo = _FindGUID( id );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
}


const char * CPQDIF_Info::GetNameOfID( const GUID& tag, int id )
{
    const char *szName = szUnrecognized;

    NI_ID_Int * pInfo = _FindInt( tag, id );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
}


const char * CPQDIF_Info::GetNameOfPhysType( INT4 type )
{
    const char *szName = szUnrecognized;

    NI_Type * pInfo = _FindType( type );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
}


const char * CPQDIF_Info::GetNameTag( const GUID& tag )
{
    const char *szName = NULL;

    NI_Tag * pInfo = _FindTag( tag );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
}


const char * CPQDIF_Info::GetNameID
        (
        const   GUID&   id
        )
{
    const char *szName = NULL;

    NI_ID_GUID * pInfo = _FindGUID( id );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
} 


const char * CPQDIF_Info::GetNameID
        (
        const   GUID&   tag,
                int     id
        )
{
    const char *szName = NULL;

    NI_ID_Int * pInfo = _FindInt( tag, id );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
}


const char * CPQDIF_Info::GetNamePhysType( INT4 type )
{
    const char *szName = NULL;

    NI_Type * pInfo = _FindType( type );
    if( pInfo )
        szName = pInfo->m_name;
    
    return szName;
}


const char * CPQDIF_Info::GetAliasTag( const GUID& tag )
{
    const char *szName = szEmpty;

    NI_Tag * pInfo = _FindTag( tag );
    if( pInfo )
    {
        szName = GET_ALIAS_STRING( pInfo->m_alias );
        if( szName == NULL )
            szName = pInfo->m_name;
    }

    return szName;
}


const char * CPQDIF_Info::GetAliasID( const GUID& id )
{
    const char *szName = szEmpty;

    NI_ID_GUID * pInfo = _FindGUID( id );
    if( pInfo )
    {
        szName = GET_ALIAS_STRING(pInfo->m_alias );
        if( szName == NULL )
            szName = pInfo->m_name;
    }

    return szName;
}


const char * CPQDIF_Info::GetAliasID( const GUID& tag, int id )
{
    const char *szName = szEmpty;

    NI_ID_Int * pInfo = _FindInt( tag, id );
    if( pInfo )
    {
        szName = GET_ALIAS_STRING( pInfo->m_alias );
        if( szName == NULL )
            szName = pInfo->m_name;
    }
    
    return szName;
}


const char * CPQDIF_Info::GetAliasPhysType( INT4 type )
{
    const char *szName = szEmpty;

    NI_Type * pInfo = _FindType( type );
    if( pInfo )
        szName = GET_ALIAS_STRING( pInfo->m_alias );
    
    return szName;
}

bool CPQDIF_Info::GetTagFromName( const char *szName, GUID& tag )
{

    bool bFound = false;
    
    CList_NI_Tag::iterator  iter;
    for( iter = m_listTags.begin(); 
            iter != m_listTags.end();
            ++iter )
    {
        if( _stricmp( szName, iter->m_name ) == 0 )
        {
            tag = iter->m_tag;
            bFound = true;
            break;
        }
    }

    return bFound;
}


bool CPQDIF_Info::GetIDFromName( const char *szName, GUID& valueID )
{

    bool bFound = false;
    
    CList_NI_ID_GUID::iterator  iter;
    for( iter = m_listGUIDs.begin(); 
            iter != m_listGUIDs.end();
            ++iter )
    {
        if( _stricmp( szName, iter->m_name ) == 0 )
        {
            valueID = iter->m_id;
            bFound = true;
            break;
        }
    }

    return bFound;
}


bool CPQDIF_Info::GetIDFromName( const char *szName, GUID& tag, int &valueID )
{

    bool bFound = false;
    
    CList_NI_ID_Int::iterator  iter;
    for( iter = m_listInts.begin(); 
            iter != m_listInts.end();
            ++iter )
    {
        if( _stricmp( szName, iter->m_name ) == 0 )
        {
            tag = iter->m_tag;
            valueID = iter->m_id;
            bFound = true;
            break;
        }
    }

    return bFound;
}


SIZE4 CPQDIF_Info::GetNumBytesOfType( int idType )
{
    SIZE4   rc = 1L;
	if( idType >= 0 && idType < sizeof( m_sizeDataTable )/sizeof( m_sizeDataTable[0] ) )
	{
		rc = (SIZE4)m_sizeDataTable[ idType ];
	}
    return rc;
}


bool CPQDIF_Info::GetTagGUID( int index, GUID& guidTag )
    {
    bool        status = false;
    NI_Tag *    info;

    info = _LookupTag( index );
    if( info )
        {
        guidTag = info->m_tag;
        status = true;
        }

    return status;
    }


bool CPQDIF_Info::GetTagName( int index, string& nameTag )
    {
    bool        status = false;
    NI_Tag *    info;

    info = _LookupTag( index );
    if( info )
        {
        nameTag = info->m_name;
        status = true;
        }

    return status;
    }


int CPQDIF_Info::GetIDCountGUID
            (
            const   GUID&   tag
            )
{
    CList_NI_ID_GUID::iterator    iter;
    int countID = 0;

    for( iter = m_listGUIDs.begin(); 
            iter != m_listGUIDs.end();
            ++iter )
    {
        //  Matched class ID
        if( iter->m_tag == tag )
        {
            countID++;
        }
    }

    return countID;
}


const char * CPQDIF_Info::GetIDNameGUID
            (
            const   GUID&   tag,
                    int     idxID,
                    GUID&   valueID
            )
{
    NI_ID_GUID * info;
    const   char * name = NULL;
    
    info = _LookupGUID( tag, idxID );
    if( info )
    {
        name = info->m_name;
        valueID = info->m_id;
    }

    return name;
}


int CPQDIF_Info::GetIDCountInt
            (
            const   GUID&   tag
            )
{
    CList_NI_ID_Int::iterator    iter;
    int     countID = 0;

    for( iter = m_listInts.begin();
            iter != m_listInts.end();
            ++iter )
    {
        //  Matched class ID
        if( iter->m_tag == tag )
        {
            countID++;
        }
    }

    return countID;
}


const char * CPQDIF_Info::GetIDNameInt
            (
            const   GUID&   tag,
                    int     idxID,
                    int&    valueID
            )
{
    NI_ID_Int * info;
    const   char * name = NULL;
    
    info = _LookupInt( tag, idxID );
    if( info )
    {
        name = info->m_name;
        valueID = info->m_id;
    }

    return name;
}



//  Lookup functions
//  ----------------


NI_Tag * CPQDIF_Info::_FindTag( const GUID &tag )
{
    NI_Tag * pInfo = NULL;

    NI_Tag info;
    info.m_tag = tag;

    CList_NI_Tag::iterator  iter;
    iter = lower_bound( m_listTags.begin(), m_listTags.end(),
        info, CLess_NI_Tag() );
    if( iter != m_listTags.end() && iter->m_tag == tag )
    {
        pInfo = &( *iter );
    }

    return pInfo;

}


NI_ID_GUID * CPQDIF_Info::_FindGUID
    (
    const   GUID&   id 
    )
{

    NI_ID_GUID * pInfo = NULL;

    NI_ID_GUID info;
    info.m_id = id;

    CList_NI_ID_GUID::iterator  iter;
    iter = lower_bound( m_listGUIDs.begin(), m_listGUIDs.end(),
        info, CLess_NI_ID_GUID() );
    if( iter->m_id == id )
    {
        pInfo = &( *iter );
    }

    return pInfo;
}          


NI_ID_Int * CPQDIF_Info::_FindInt
    (
    const   GUID&   tag,
            int     id 
    )
{
    NI_ID_Int *pInfo = NULL;
    
    
    NI_ID_Int info;
    info.m_tag = tag;
    info.m_id = id;

    CList_NI_ID_Int::iterator iter;
    iter = lower_bound( m_listInts.begin(), m_listInts.end(),
        info, CLess_NI_ID_Int() );
    if( iter->m_tag == tag
        && iter->m_id == id )
    {
        pInfo = &( *iter );
    }

    return pInfo;
}


NI_Type * CPQDIF_Info::_FindType( INT4 type )
{
         
    NI_Type *  pInfo = NULL;    

    CList_NI_Type::iterator     iter;

    for( iter = m_listTypes.begin(); 
            iter != m_listTypes.end();
            ++iter )
    {
        //  Matched tag?
        if( iter->m_type == (int) type )
        {
            pInfo = &( *iter );
            break;
        }
    }

    return pInfo;
}


NI_Tag * CPQDIF_Info::_LookupTag( int idxLook )
{

    NI_Tag *    info = NULL;

    if( idxLook >= 0 && idxLook < (int)m_listTags.size() )
    {
        info = &m_listTags[ idxLook ];
    }

    return info;
}


NI_ID_GUID * CPQDIF_Info::_LookupGUID
        (
        const   GUID&   tagLook,
                int     idxLook 
        )
{
    CList_NI_ID_GUID::iterator    iter;

    int             idxThis;
    NI_ID_GUID *    info = NULL;

    idxThis = 0;
    for( iter = m_listGUIDs.begin(); 
            iter != m_listGUIDs.end();
            ++iter )
    {
        if( iter->m_tag == tagLook )
        {
            //  Matched class ID
            if( idxThis == idxLook )
            {
                info = &(*iter);
                break;
            }
            idxThis++;
        }
    }

    return info;
}


NI_ID_Int * CPQDIF_Info::_LookupInt
        ( 
        const   GUID&   tag,
                int     idxLook 
        )
{
    CList_NI_ID_Int::iterator    iter;

    int         idxThis;
    NI_ID_Int * info = NULL;

    idxThis = 0;
    for( iter = m_listInts.begin(); 
            iter != m_listInts.end();
            ++iter )
    {
        //  Matched owner?
        if( iter->m_tag == tag )
        {
            if( idxThis == idxLook )
            {
                info = &(*iter);
                break;
            }
            idxThis++;
        }
    }

    return info;
}
