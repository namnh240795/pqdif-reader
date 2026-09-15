/*
**  Class:              PQDIFIterator
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



class PQController;

class PQDIFIterator
    {
    public:
        PQDIFIterator
            (
            PQController *          pcont,
            BYTE *                  buffer, 
            SIZE4                   size,
            long                    pos,
            CPQDIF_E_Collection *   pcoll
            );
        ~PQDIFIterator();

        bool ParseCollection( void );

    protected:
        //  Who wants the results?
        PQController *          m_pcont;

        //  Keep track of the record body buffer
        //  and our position in it
        BYTE *                  m_buffer;
        long                    m_pos;
        long                    m_size;

        //  Keep track of how far deep into the PQDIF record we are
        CPQDIF_E_Collection *   m_pcoll;

    private:
    
    };
