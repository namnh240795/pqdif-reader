/*
**  Class:          CPQDIF_Info
**  Description:	Base info class
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

class NameInfo
{
public:
    inline NameInfo()
    {
        m_name = NULL;
        m_alias = NULL;
    }
    const char *    m_name;   //  Name of item in the list (tag, ID, etc.)
    const void *    m_alias;  //  Alias for the item. This is declared as pointer to void
                              //  to allow for indirection when multiple languages are supported.
};

// Macro to derefernce an alias. Can be over-ridden.
#define GET_ALIAS_STRING( alias ) ((const char *)(alias))


//  Sub info classes
//  ----------------
class NI_Tag : public NameInfo
{
public:    
    GUID            m_tag;
};

class NI_ID_GUID : public NameInfo
{
public:
    GUID            m_tag;  //  Owner
    GUID            m_id;   //  ID value
};

class NI_ID_Int : public NameInfo
{
public:
    GUID            m_tag;  //  Owner
    int             m_id;   //  ID value
};

class NI_Type : public NameInfo
{
public:
    int             m_type; //  Physical type
};


//  List classes
//  ------------
typedef vector<NI_Tag> CList_NI_Tag;
typedef vector<NI_ID_GUID> CList_NI_ID_GUID;
typedef vector<NI_ID_Int> CList_NI_ID_Int;
typedef vector<NI_Type> CList_NI_Type;



class CPQDIF_Info  
    {
    public:
	    CPQDIF_Info();
        ~CPQDIF_Info();

    // Attributes
    public:

    // Operations
    public:

        //  Name encoding. These methods always return a
        //  valid string. "< Unrecognized >" is returned if
        //  the requested value is not found.
        const char * GetNameOfTag( const GUID& tag );
        const char * GetNameOfID( const GUID& valueID );   
        const char * GetNameOfID( const GUID& tagOwner, int valueID );
        const char * GetNameOfPhysType( INT4 type );

        //  Same as NameOf functions execept that NULL is
        //  returned for undefined requests.
        const char * GetNameTag( const GUID& tag );
        const char * GetNameID( const GUID& valueID );   
        const char * GetNameID( const GUID& tagOwner, int valueID );
        const char * GetNamePhysType( INT4 type );

        //  Alias encoding functions. "" is returned if the requested
        //  value is not found.
        const char * GetAliasTag( const GUID& tag );
        const char * GetAliasID( const GUID& valueID );   
        const char * GetAliasID( const GUID& tagOwner, int valueID );
        const char * GetAliasPhysType( INT4 type );

        //  Name decoding. These methods return true if
        //  the requested value is found.
        bool GetTagFromName( const char *szName, GUID& tag );
        bool GetIDFromName( const char *szName, GUID& valueID );
        bool GetIDFromName( const char *szName, GUID& tagOwner, int &valueID );

        //  Data manipulation
        static SIZE4 GetNumBytesOfType( int typePhysical );
        static inline SIZE4 padSizeTo4Bytes( SIZE4 sizeOrig )
		{
			return ( ( sizeOrig + 3 ) & ~3 );
		}
        
        static inline BYTE * GetPtrToDataValue( int /*typePhysical*/, PQDIFValue& value )
		{
			return (BYTE *)&value;
		}
        static inline const BYTE * GetPtrToDataValue( int /*typePhysical*/, const PQDIFValue& value )
		{
			return (const BYTE *)&value;
		}

        //  Tag enumeration
        size_t GetTagCount( void )
            { return m_listTags.size(); }
        bool GetTagGUID( int index, GUID& guidTag );
        bool GetTagName( int index, string& nameTag );

        //  GUID ID enumeration
        int GetIDCountGUID
            (
            const   GUID&   tagOwner
            );
        const char * GetIDNameGUID
            (
            const   GUID&   tagOwner,
                    int     idxID,
                    GUID&   valueID
            );

        //  Integer ID enumeration
        int GetIDCountInt
            (
            const   GUID&   tagOwner
            );
        const char * GetIDNameInt
            (
            const   GUID&   tagOwner,
                    int     idxID,
                    int&    valueID
            );

    // Implementation
    protected:
        BYTE * _GetPtrToDataValue( int typePhysical, const PQDIFValue& value );
        
        NI_Tag *        _FindTag( const GUID &tag );
        NI_ID_GUID *    _FindGUID( const GUID& id );          
        NI_ID_Int *     _FindInt( const GUID& tag, int id );            
        NI_Type *       _FindType( INT4 type );            


        NI_Tag *        _LookupTag( int idxLook );
        NI_ID_GUID *    _LookupGUID
            (
            const   GUID&   tagOwner,
                    int     idxLook 
            );          
        NI_ID_Int *     _LookupInt
            (
            const   GUID&   tagOwner,
                    int     idxLook 
            );

    //  Data
    protected:
        CList_NI_Tag        m_listTags;
        CList_NI_ID_GUID    m_listGUIDs;
        CList_NI_ID_Int     m_listInts;
        CList_NI_Type       m_listTypes;


	//	Lookup table for finding the size of a data type.
	protected: 
		static BYTE			m_sizeDataTable[70];

    };


//  The one and only information object (Singleton)
extern CPQDIF_Info  theInfo;

