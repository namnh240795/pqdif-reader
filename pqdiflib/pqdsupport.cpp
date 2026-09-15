/*
**  Class:          CPQDIF_COMSupport
**  Description:    Implements support member functions so that make dealing with COM/OLE/ActiveX much easier.
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

#ifdef PQDIF_USE_COM

#include "pqdsupport.h"

const GUID ID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
    
//  The one and only support object (Singleton)
CPQDIF_COMSupport   theSupport;



//  Construction
//  ============

CPQDIF_COMSupport::CPQDIF_COMSupport()
    {
    }

CPQDIF_COMSupport::~CPQDIF_COMSupport()
    {
    }


bool    CPQDIF_COMSupport::NewVariantArrayFromGUID
            ( 
                    VARIANT&    var,
            const   GUID&       guid
            )
    {
            bool    status = false;
    const   int     countLongs = 4;
            BYTE *  pbData;

    //  Set the variant to a 1-byte array
    VariantInit( &var );
	var.vt = VT_ARRAY + VT_I4;
	var.parray = SafeArrayCreateVector( VT_I4, 0, countLongs );

	// If a safe array of doubles was passed then ...
	if( var.parray != NULL)
    	{
		if( !FAILED( SafeArrayAccessData( var.parray, (void **) &pbData ) ) )
		    {
			//  Copy the contents of the GUID
            memcpy( pbData, &guid, countLongs * sizeof( long ) );
			SafeArrayUnaccessData( var.parray );
            status = true;
	    	}
    	}	

    return status;
    }


bool    CPQDIF_COMSupport::SetGUIDFromVariantArray
            (
                    GUID&       guid,
            const   VARIANT&    var
            ) const
    {
    bool    status = false;
    long    lLower;
	long    lUpper;
	long    countItems;
    BYTE *  pbData;

	guid = ID_NULL;

    //  Valid array of long [ 4 ]?
    if( ( var.vt & VT_ARRAY ) /* && ( var.vt & VT_I4 )*/ && var.parray )
    	{
        //  Validate the size of the array
		SafeArrayGetUBound( var.parray, 1, &lUpper );
		SafeArrayGetLBound( var.parray, 1, &lLower );
		countItems = (lUpper - lLower + 1);

        if( countItems == 4 )
        {
        	VARIANT HUGEP *pvs;

			if (!FAILED(SafeArrayAccessData(var.parray, reinterpret_cast<void HUGEP**>(&pvs))))
			{
				if (var.vt & VT_VARIANT)
				{
					INT4 * pInt = (INT4 *)&guid;

					for (int count = 0; count < 4; count++, pInt++)
                    {   
                        switch( pvs[ count ].vt & VT_TYPEMASK )
                        {

                        case VT_I1:
                            *pInt = (INT4)pvs[count].cVal;
                            break;

                       case VT_I2:
                            *pInt = (INT4)pvs[count].iVal;
                            break;

                        case VT_I4:
						    *pInt = pvs[count].lVal;
                            break;

                        case VT_UI1:
						    *pInt = pvs[count].bVal;
                            break;

                        case VT_UI2:
						    *pInt = pvs[count].uiVal;
                            break;
                        
                        case VT_UI4:
						    *pInt = pvs[count].ulVal;
                            break;

                        }
                    }
				}
				else
				{
					pbData = (BYTE *)pvs;
					//  Copy the GUID
					memcpy( &guid, pbData, sizeof( guid ) );
					status = true;
				}
                
				SafeArrayUnaccessData( var.parray );
                }
            }   //  Got safe array data
	    }   //  Valid safe array

    return status;
    }


bool    CPQDIF_COMSupport::NewVariantArrayFromVector
        ( 
        VARIANT&            var,
        CPQDIF_E_Vector&    vect
        )
    {
    bool            status = false;
    VARTYPE         vtSingle;
    long            typePhysical;
    long            sizePhysical;
    long            countItems;
    BYTE *          pbData;
    INT1 *          pdataSource;

    //  Check the physical type for the size we need...
    //  (and how many items are in the array)
    typePhysical = vect.GetPhysicalType();
    sizePhysical = theInfo.GetNumBytesOfType( typePhysical );
    vect.GetCount( countItems );

    _variant_t varV;
    //  Grab the first item to see what variant type the array should be...
    status = vect.GetValue( 0, varV );
    if( status )
        {
        //  Got the variant type
        vtSingle = varV.vt;
        status = false;

        //  Set the variant to an array of this type
        VariantInit( &var );

	    var.vt = VT_ARRAY | vtSingle;
	    var.parray = SafeArrayCreateVector( vtSingle, 0, countItems );

	    // If a safe array of doubles was passed then ...
	    if( var.parray != NULL)
    	    {
		    if( !FAILED( SafeArrayAccessData( var.parray, (void **) &pbData ) ) )
		        {
                pdataSource = reinterpret_cast<INT1 *>(vect.GetRawData());
                if( pdataSource )
                    {
                    //  Copy the entire block of data across by physical item size
                    memcpy( pbData, pdataSource, countItems * sizePhysical );
                    status = true;
                    }

                SafeArrayUnaccessData( var.parray );
                }   //  Got safe array data
	    	}   //  Valid safe array
        }   //  Valid data type

    return status;
    }


bool    CPQDIF_COMSupport::NewVariantArrayFromDouble
            ( 
            VARIANT&    var,
            double *    arValues,
            long        countItems
            )
    {
    bool            status = false;
    VARTYPE         vtSingle;
    long            sizePhysical;
    BYTE *          pbData;

    //  Set the variant to an array of this type
    sizePhysical = sizeof( double );
    vtSingle = VT_R8;
    VariantInit( &var );
	var.vt = VT_ARRAY | vtSingle;
	var.parray = SafeArrayCreateVector( vtSingle, 0, countItems );

	// If a safe array of doubles was passed then ...
	if( var.parray != NULL)
    	{
		if( !FAILED( SafeArrayAccessData( var.parray, (void **) &pbData ) ) )
		    {
            if( arValues )
                {
                //  Copy the entire block across
                memcpy( pbData, (BYTE *) arValues, sizePhysical * countItems );
                status = true;
                }

            SafeArrayUnaccessData( var.parray );
            }   //  Got safe array data
	    }   //  Valid safe array

    return status;
    }

long GetPhysicalTypeFromVariant( const VARIANT& var, long typeElement )
    {
    long    typePhysical = 0;
    BOOL    isArray = FALSE;
    int     countDims = 0;
    long    lUpper;
    long    lLower;
    long    countItemsLastDim = 0;

    SAFEARRAY *     psa = NULL;

    //  Is this an array?
    if( var.vt & VT_ARRAY )
        {
        //  Which safearray?
        if( var.vt & VT_BYREF )
            {
            if( var.pparray )
                psa = *var.pparray;
            }
        else
            {
            psa = var.parray;
            }

        if( psa )
            {
            isArray = TRUE;
            countDims = SafeArrayGetDim( psa );

            //  Count the number of items in the last dimension...
            SafeArrayGetUBound( var.parray, countDims, &lUpper );
            SafeArrayGetLBound( var.parray, countDims, &lLower );
            countItemsLastDim = (lUpper - lLower + 1);
            }
        }

    //  Is this a vector? If so, we only consider it an "array" if there
    //  are two dimensions.
    if( typeElement == ID_ELEMENT_TYPE_VECTOR && isArray )
        {
        if( countDims < 2 )
            {
            isArray = FALSE;
            }
        }

    switch( var.vt & VT_TYPEMASK )
        {
        case VT_BOOL:
            typePhysical = ID_PHYS_TYPE_BOOLEAN2           ;
            break;

        case VT_I1:
            typePhysical = ID_PHYS_TYPE_INTEGER1           ;
            break;

        case VT_I2:
            typePhysical = ID_PHYS_TYPE_INTEGER2           ;
            break;

        case VT_I4:
            typePhysical = ID_PHYS_TYPE_INTEGER4           ;
            if( isArray && countItemsLastDim == 4 )
                {
                typePhysical = ID_PHYS_TYPE_GUID;
                }
            break;

        //  Real/complex
        case VT_R4:
            typePhysical = ID_PHYS_TYPE_REAL4              ;
            if( isArray && countItemsLastDim == 2 )
                {
                typePhysical = ID_PHYS_TYPE_COMPLEX8;
                }
            break;

        case VT_R8:
            typePhysical = ID_PHYS_TYPE_REAL8;              
            if( isArray && countItemsLastDim == 2 )
                {
                typePhysical = ID_PHYS_TYPE_COMPLEX16;
                }
            break;

        //  Date/time
        case VT_DATE:
            typePhysical = ID_PHYS_TYPE_TIMESTAMPPQDIF     ;
            break;
        }

    return typePhysical;
    }


bool    CPQDIF_COMSupport::SetVectorArrayFromVariant( CPQDIF_E_Vector& vect, VARIANT& values )
    {
    bool                status = false;
    long                typePhysical;
    unsigned int        sizeSingle;
    long                sizeType = 0;
    BYTE *              pbData;
    INT1 *              pdataTarget;

    SAFEARRAY *     psa = NULL;

    long    lLower;
	long    lUpper;
	long    countItems;
	long    countDimensions;
	long    countItemsDim2 = 0;

    //  Make sure it is an array
    if( values.vt & VT_ARRAY )
        {
        //  Is valid physical type set?
        //typePhysical = vect.GetPhysicalType();
        typePhysical = 0;
        if( typePhysical == 0 )
            {
            typePhysical = GetPhysicalTypeFromVariant( 
                    values, vect.GetElementType() );

            vect.SetPhysicalType( typePhysical );
            sizeType = theInfo.GetNumBytesOfType( typePhysical );
            }

        //  Which safearray?
        if( values.vt & VT_BYREF )
            {
            if( values.pparray )
                psa = *values.pparray;
            }
        else
            {
            psa = values.parray;
            }

		if( !FAILED( SafeArrayAccessData( psa, (void **)&pbData ) ) )
		    {
            //  Get the physical size of each element
            sizeSingle = SafeArrayGetElemsize( values.parray );

            // Get the size of the new array.
			SafeArrayGetUBound( values.parray, 1, &lUpper );
			SafeArrayGetLBound( values.parray, 1, &lLower );
			countItems = (lUpper - lLower + 1);

            //  Is there a second dimension?
            countDimensions = SafeArrayGetDim( values.parray );
            if( countDimensions > 1 )
                {
			    SafeArrayGetUBound( values.parray, 2, &lUpper );
			    SafeArrayGetLBound( values.parray, 2, &lLower );
			    countItemsDim2 = (lUpper - lLower + 1);
                }

            //  Configure vector
            vect.SetCount( countItems );

            pdataTarget = reinterpret_cast<INT1 *>(vect.GetRawData());

            if( pdataTarget && (int) sizeType == (int) sizeSingle )
                {
			    //  Copy the caller's data into the array.
                memcpy( pdataTarget, pbData, countItems * sizeSingle );
                status = true;
                }

            SafeArrayUnaccessData( psa );
            }
		}

	return status;
    }


bool    CPQDIF_COMSupport::NewArrayUINT4FromVariant
            (
            UINT4 **    aidxChannelTriggerLocal, 
            long&       countItems, 
            VARIANT&    var
            )
    {
    bool    status = false;
    long    lLower;
	long    lUpper;
    BYTE *  pbData;
    long *  pSource;
    UINT4 * pTarget;

    //  Init
    *aidxChannelTriggerLocal = NULL;
    countItems = 0;

    //  Valid array of long?
    if( ( var.vt & VT_ARRAY ) && ( var.vt & VT_I4 ) && var.parray )
    	{
        //  Validate the size of the array
		SafeArrayGetUBound( var.parray, 1, &lUpper );
		SafeArrayGetLBound( var.parray, 1, &lLower );
		countItems = (lUpper - lLower + 1);

        if( countItems > 0 )
            {
            if( !FAILED( SafeArrayAccessData( var.parray, (void **) &pbData ) ) )
		        {
                //  Allocate space
                *aidxChannelTriggerLocal = new UINT4[ countItems ];

                //  Create temp pointers (to be incremented) and copy 
                pSource = (long *) pbData;
                pTarget = *aidxChannelTriggerLocal;
                for( long idx = 0; idx < countItems; idx++, pSource++, pTarget++ )
                    {
                    *pTarget = (UINT4) *pSource;
                    }

                status = true;
                SafeArrayUnaccessData( var.parray );
                }
            }   //  Got safe array data
	    }   //  Valid safe array

    return status;

    }



bool    CPQDIF_COMSupport::NewComStringFromVector( CPQDIF_E_Vector& vect, BSTR * str )
    {
    bool            status = false;
    long            countItems;
    string          strOutput;
    INT1 *          pdataSource;

    //  Init
    strOutput = "";
    vect.GetCount( countItems );

    switch( vect.GetPhysicalType() )
        {
        case ID_PHYS_TYPE_CHAR1:
            pdataSource = reinterpret_cast<INT1 *>(vect.GetRawData());
            if( pdataSource )
                {
                //  Convert it to a string
                USES_CONVERSION;
                strOutput = (const char *) pdataSource;
                *str = SysAllocString(_bstr_t(strOutput.c_str()));
                status = true;
                }
            break;

        case ID_PHYS_TYPE_CHAR2:
            //  NOT SUPPORTED
            break;

        default:
            status = true;
            break;
        }

    return status;
    }


bool    CPQDIF_COMSupport::SetVectorArrayFromString( CPQDIF_E_Vector& vect, BSTR * str )
    {
    bool            status = false;

    USES_CONVERSION;

    //  Init
    vect.SetValues( W2A(*str) );

    return status;
    }


bool    CPQDIF_COMSupport::SetDateFromTimeStamp( DATE& date, const TIMESTAMPPQDIF& ts )
    {
    date   =     (double) ts.day
            +  ( (double) ts.sec / (double) SECONDS_PER_DAY );

    return true;
    }


bool    CPQDIF_COMSupport::SetTimeStampFromDate( TIMESTAMPPQDIF& ts, const DATE& date )
    {
    //  Take integral portion for the day
    ts.day = (UINT4) date;

    //  Take the fractional portion for the seconds
    ts.sec = ( (REAL8) SECONDS_PER_DAY ) * ( (REAL8) date - (REAL8) ts.day );

    return true;
    }

#endif // PQDIF_USE_COM