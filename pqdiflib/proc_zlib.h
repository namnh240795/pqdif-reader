/*
**  Class:          CPQDIF_SP_ZLIB
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

class CPQDIF_SP_ZLIB : public CPQDIF_StreamProcessor
    {
    public:
        CPQDIF_SP_ZLIB();
        ~CPQDIF_SP_ZLIB();

    // Attributes
    public:

    // Operations
    public:
        virtual bool StreamEncode( void );
        virtual bool StreamDecode( void );

    //  Member data
    private:
        BYTE *  m_bufferOutput;
        int     m_sizeOutput;
    };

