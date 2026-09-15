/*
**  Class:              CPQDIF_R_Container
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

class CPQDIF_R_Container : public CPQDIF_R_General
    {
    public:
        CPQDIF_R_Container();
        virtual ~CPQDIF_R_Container();

    // Operations
    public:
        bool GetInfo();

    bool CPQDIF_R_Container ::SetInfo
            (
            const   char *  language,
            const   char *  title,
            const   char *  subject,
            const   char *  author,
            const   char *  keywords,
            const   char *  comments,
            const   char *  lastSavedBy,
            const   char *  application,
            const   char *  security,
            const   char *  owner,
            const   char *  copyright,
            const   char *  trademarks,
            const   char *  notes
            );

        bool GetCompressionInfo
	        (
            UINT4&  styleComp,		//	Compression style (output)
            UINT4&  algComp			//	Compression algorithm (output)
	        );	

    //  Local data
    private:
        
    };

