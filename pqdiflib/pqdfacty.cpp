/*
**  Class:          CPQDIF_Factory 
**  Description:    Implements two sets of member functions: The "abstract factory" methods such as  NewPersistController() are 
**                  used to create objects in such a way that the caller does not need to  know the exact "concrete" class.
**
**                  This class is a Singleton class; this means that only one instance of it
**                  should ever be created. This instance is made global, and is one of the
**                  only global objects in the entire system.
**
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

//  The one and only factory object (Singleton)
CPQDIF_Factory  theFactory;

//  Construction
//  ============

CPQDIF_Factory::CPQDIF_Factory()
    {
    }

CPQDIF_Factory::~CPQDIF_Factory()
    {
    }


#ifndef _PQDIF_NO_PERSIST_CONTROLLER
//  Creates a new persistence contoller of the specified concrete class.
//
CPQDIF_PersistController *  CPQDIF_Factory::NewPersistController( PF_PersistController which )
    {
    CPQDIF_PersistController *  ppc = NULL;

    switch( which )
        {
        case PFPC_FlatFile:
            ppc = new CPQDIF_PC_FlatFile();
            break;

        default:
            //  Cannot support...
            ppc = NULL;
            break;
        }

    return ppc;
    }
#endif


//  Creates a new stream object of the specified concrete class.
//
CPQDIF_StreamIO *           CPQDIF_Factory::NewStreamIO( PF_StreamIO which )
    {
    CPQDIF_StreamIO *   pstream = NULL;

    switch( which )
        {
        case PSIO_FlatFile:
            pstream = new CPQDIF_S_FlatFile();
            break;

        case PSIO_Chunk:
            pstream = new CPQDIF_S_Chunk();
            break;

#ifdef _PQDIF_HANK_PQDIF_STUFF
        case PSIO_HCOM:
            pstream = new CPQDIF_S_HCOM();
            break;
#endif
        default:
            //  Cannot support...
            pstream = NULL;
            break;
        }

    return pstream;
    }


//  Creates a new stream processor object. The list of concrete classes
//  is extensible.
//
CPQDIF_StreamProcessor *    CPQDIF_Factory::NewStreamProcessor( long which )
    {
    CPQDIF_StreamProcessor *    pproc = NULL;

    switch( which )
        {
        case ID_COMP_ALG_NONE:
            pproc = new CPQDIF_SP_Nothing();
            break;

        case ID_COMP_ALG_ZLIB:
            pproc = new CPQDIF_SP_ZLIB();
            break;

        case ID_COMP_ALG_PKZIPCL:
            //  No longer supported
            //pproc = new CPQDIF_SP_PKZIP();
            break;

        default:
            //  Cannot support...
            pproc = NULL;
            break;
        }

    return pproc;
    }


//  Creates a new PQDIF record object of the specified concrete class.
//
CPQDIFRecord *              CPQDIF_Factory::NewRecord( PF_Record which )
    {
    CPQDIFRecord *  prec = NULL;

    switch( which )
        {
        //  In all cases, create a general record, but with different header tags.
        case PFR_Record:
            prec = new CPQDIF_R_General();
            prec->HeaderSetTag( tagBlank );
            break;

        case PFR_Container:
            prec = new CPQDIF_R_General();
            prec->HeaderSetTag( tagContainer );
            break;

        case PFR_DataSource:
            prec = new CPQDIF_R_General();
            prec->HeaderSetTag( tagRecDataSource );
            break;

        case PFR_MonitorSettings: 
            prec = new CPQDIF_R_General();
            prec->HeaderSetTag( tagRecMonitorSettings );
            break;
        case PFR_Observation:
            prec = new CPQDIF_R_General();
            prec->HeaderSetTag( tagRecObservation );
            break;

        default:
            prec = NULL;
            break;
        }

    return prec;
    }


CPQDIF_R_Observation * CPQDIF_Factory::NewObservationWrapper
            (
            CPQDIFRecord *  precBase,
            CPQDIFRecord *  precDataSource
            )
{
    CPQDIF_R_Observation *  pobs = NULL;
    CPQDIF_R_DataSource *   pds = NULL;

    pobs = new CPQDIF_R_Observation( *precBase );
    if( pobs )
    {
    //
    //
    //  Replaced dynamic cast with static cast.  I don't understand why
    //  dynamic does not work (RTTI is enabled).  Rob has a comment in another
    //  code module in the Hank data manager code that says a static cast has to
    //  be used in this situation since the classes involved have no data members.
    //
//        pds = dynamic_cast<CPQDIF_R_DataSource *>( precDataSource );
        pds = static_cast<CPQDIF_R_DataSource *>( precDataSource );
        if( pds )
        {
            pobs->SetDataSource( pds );
        }
    }

    return pobs;
}


CPQDIF_R_Observation *      CPQDIF_Factory::NewObservationWrapper2
            (
            CPQDIFRecord *  precBase,
            CPQDIFRecord *  precDataSource,
            CPQDIFRecord *  precSettings
            )
    {
    CPQDIF_R_Observation *  pobs = NULL;
    CPQDIF_R_DataSource *   pds;
    CPQDIF_R_Settings *     psett;

    pobs = new CPQDIF_R_Observation( *precBase );
    if( pobs )
        {
        //  Check the casts first
        //
        //
        //  Replaced dynamic cast with static cast.  I don't understand why
        //  dynamic does not work (RTTI is enabled).  Rob has a comment in another
        //  code module in the Hank data manager code that says a static cast has to
        //  be used in this situation since the classes involved have no data members.
        //
        pds = static_cast<CPQDIF_R_DataSource *>( precDataSource );
//        pds = dynamic_cast<CPQDIF_R_DataSource *>( precDataSource );
        if( pds )
            pobs->SetDataSource( pds );

//        psett = dynamic_cast<CPQDIF_R_Settings *>( precSettings );
        psett = static_cast<CPQDIF_R_Settings *>( precSettings );
        if( psett )
            pobs->SetMonitorSettings( psett );
        }

    return pobs;
    }


//  Creates a new PQDIF element object from the specified concrete
//  class.
//
CPQDIF_Element *            CPQDIF_Factory::NewElement( long which )
    {
    CPQDIF_Element *  pel = NULL;

    switch( which )
        {
        case ID_ELEMENT_TYPE_COLLECTION:
            pel = new CPQDIF_E_Collection();
            break;

        case ID_ELEMENT_TYPE_SCALAR:
            pel = new CPQDIF_E_Scalar();
            break;

        case ID_ELEMENT_TYPE_VECTOR:
            pel = new CPQDIF_E_Vector();
            break;

        default:
            pel = NULL;
            break;
        }

    return pel;
    }

