/*
**  Class:              PQController
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

class CPQDIF_E_Collection;

class PQController
    {
    public:
        PQController();
        virtual ~PQController();

        virtual void ParseRecord
            ( 
            BYTE *                  buffer, 
            SIZE4                   size, 
            CPQDIF_E_Collection *   pcollMain
            );

        //  Routines to add specific PQDIF elements

        //  If return value is TRUE, the Iterator object
        //  will continue iterating through the elements
        //  in the collection. If FALSE, the collection
        //  will be skipped.
        virtual CPQDIF_E_Collection * acceptCollection
            ( 
            CPQDIF_E_Collection *   pcoll,
            int                     index,
            const GUID&             tag
            );
        virtual CPQDIF_E_Scalar * acceptScalar
            ( 
            CPQDIF_E_Collection *   pcoll,
            int                     index,
            const GUID&             tag, 
            long                    typePhysical,
            void *                  pdata
            );
        virtual CPQDIF_E_Vector * acceptVector
            ( 
            CPQDIF_E_Collection *   pcoll,
            int                     index,
            const GUID&             tag, 
            long                    typePhysical,
            c_vector *              pvector,
            void *                  pdata
            );

        static bool decodeValue
            ( 
            long        typePhysical,
            void *      pdata,
            PQDIFValue& value
            );

    protected:
        
    };
