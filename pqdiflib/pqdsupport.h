/*
**  Class:          CPQDIF_COMSupport
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


#ifndef _CPQDIF_COMSupport_INC_
#define _CPQDIF_COMSupport_INC_
//  class CPQDIF_COMSupport

#ifdef PQDIF_USE_COM

class CPQDIF_COMSupport
    {
    public:
	    CPQDIF_COMSupport();
        ~CPQDIF_COMSupport();

    // Attributes
    public:

    // Operations
    public:
        bool    NewVariantArrayFromGUID
            ( 
                    VARIANT&    var,
            const   GUID&       guid
            );
        bool    SetGUIDFromVariantArray
            (
                    GUID&       guid,
            const   VARIANT&    var
            ) const;
        bool    NewVariantArrayFromVector
            ( 
            VARIANT&            var,
            CPQDIF_E_Vector&    vect
            );
        bool    NewVariantArrayFromDouble
            ( 
            VARIANT&    var,
            double *    arValues,
            long        countValues
            );
        bool    NewArrayUINT4FromVariant
            (
            UINT4 **    aidxChannelTrigger, 
            long&       countTriggersLocal, 
            VARIANT&    aidxChannelTriggerVar
            );

        bool    SetVectorArrayFromVariant( CPQDIF_E_Vector& vect, VARIANT& var );

        bool    NewComStringFromVector( CPQDIF_E_Vector& vect, BSTR * str );
        bool    SetVectorArrayFromString( CPQDIF_E_Vector& vect, BSTR * str );

        bool    SetDateFromTimeStamp( DATE& date, const TIMESTAMPPQDIF& ts );
        bool    SetTimeStampFromDate( TIMESTAMPPQDIF& ts, const DATE& date );

    // Implementation
    protected:
    };


//  The one and only support object (Singleton)
extern CPQDIF_COMSupport    theSupport;

#endif // PQDIF_USE_COM

#endif // _CPQDIF_COMSupport_INC_
