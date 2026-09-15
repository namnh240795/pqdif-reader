/*
**  Class:              PQAlloc
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

#ifndef PQALLOC_INC
#define PQALLOC_INC

class PQAlloc
    {

    public:

        PQAlloc();
        ~PQAlloc();

        void Reinitialize(long offset = 0L);

        //  Routines to add specific PQDIF elements
        long addCollection
            ( 
            UINT4   count,
            SIZE4   &size
            );

        long addCollectionValue
            ( 
            UINT4                   count, 
            const GUID&             tag, 
            long                    idxCE
            );

        bool addScalarValue
            (
            long                    typePhysical,
            PQDIFValue              value,
            const GUID&             tag, 
            long                    idxCE
            );

        bool addVectorValue
            (
            long                    typePhysical,
            long                    count, 
            BYTE *                  values,
            const GUID &            tag, 
            long                    idxCE
            );

        //  Methods to stream results.
        long WriteListToFile(FILE *pf);
        long WriteListToStream( CPQDIF_StreamIO *pf );

    private:

        //  Buffer for managing data.
        vector<BYTE> data;

        //  Offset to be added to the idx
        //  parameter returned by allocate
        //  Used to allow resetting allocators
        //  but return correct file offset
        long idxOffset; 

        //  Allocate a block. Returns the index of the block. >= 0
        //  if success.
        long allocate(size_t siz);

        //  Convert a buffer offset into a pointer. The pointer
        //  is valid until the next allocate.
        inline BYTE * at( long idx )
        {
            return &data[ idx - idxOffset ];
        }

        //  Pack a value into a buffer.
        static bool convertValue
            (
            long        typePhysical,
            PQDIFValue  value,
            BYTE *      pdata
            );

    };


#endif
