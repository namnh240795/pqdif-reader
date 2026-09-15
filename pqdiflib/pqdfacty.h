/*
**  Class:          CPQDIF_Factory
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

#include "pqdif_lg.h"

//  Interface declarations
class CPQDIF_StreamIO;
class CPQDIF_StreamProcessor;
class CPQDIF_Element;
class IPQDIF_Importer;

//  Parameters
enum PF_PersistController
    {
    PFPC_FlatFile,
    PFPC_HCOM       //  Hank DMM (550x) implementation
    };

enum PF_StreamIO
    {
    PSIO_FlatFile,
    PSIO_Chunk,
    PSIO_HCOM       //  Hank DMM (550x) implementation
    };

enum PF_Record
    {
    PFR_Record,
    PFR_Container,
    PFR_DataSource,
    PFR_MonitorSettings,
    PFR_Observation
    };


class CPQDIF_Factory
    {
    public:
	    CPQDIF_Factory();
        ~CPQDIF_Factory();

    // Attributes
    public:

    // Operations
    public:
#ifndef _PQDIF_NO_PERSIST_CONTROLLER
        CPQDIF_PersistController *  NewPersistController( PF_PersistController which );
#endif

        CPQDIF_StreamIO *           NewStreamIO( PF_StreamIO which );
        CPQDIF_StreamProcessor *    NewStreamProcessor( long which );

        CPQDIF_Element *            NewElement( long which );
        
        CPQDIFRecord *              NewRecord( PF_Record which );
        CPQDIF_R_Observation *      NewObservationWrapper
            (
            CPQDIFRecord *  precBase,
            CPQDIFRecord *  precDataSource
            );
        CPQDIF_R_Observation *      NewObservationWrapper2
            (
            CPQDIFRecord *  precBase,
            CPQDIFRecord *  precDataSource,
            CPQDIFRecord *  precSettings
            );

    // Implementation
    protected:
    };


//  The one and only factory object (Singleton)
extern CPQDIF_Factory  theFactory;
