/*
**  Class:          
**  Description:	Main Include File
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

//  Windows stuff...
#include <windows.h>

//	Array template from Standard Template Library (STL)
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

//  Math library support
#include <math.h>


#include "pqdif_custom_1.h" // include implementation specific items

//  PQDIF headers
#include "pqdif_ph.h"   //  Physical definitions
#include "pqdif_lg.h"   //  Logical definitions
#include "pqdif_id.h"   //  Extensible ID definitions

//  General purpose classes
#include "pqbytearray.h"    //  Byte array class - mostly compatible with MFC version
#include "pqptrarray.h"     //  Pointer array class - mostly compatible with MFC version

//  Persistence controllers - Concrete uses an enum defined in pqdfacty.h
#include "pcn_base.h"   //  Interface (virtual base class)

//  Stream classes
#include "str_base.h"   //  Interface (virtual base class)
#include "str_flat.h"   //  Flat file (for reading/writing temporary files)
#include "str_chnk.h"   //  Chunked streams

//  Stream processor classes
#include "proc_bas.h"   //  Interface (virtual base class)
#include "proc_not.h"   //  Concrete: Nothing
#include "proc_zlib.h"  //  Concrete: ZLIB

//  Record classes
#include "rec_base.h"       //  Record base class
#include "rec_general.h"    //  General record class (or "generic")
#include "rec_container.h"  //  Container record class - cast from general
#include "rec_datasource.h" //  Data source record class - cast from general
#include "rec_observ.h"     //  Observation record class - cast from general
#include "rec_settings.h"   //  Settings record class - wrapper around general

//  Element classes
#include "el_base.h"    //  Interface (virtual base class)
#include "el_coll.h"    //  Concrete: Collection
#include "el_scal.h"    //  Concrete: Scalar
#include "el_vect.h"    //  Concrete: Vector

//  Serialization controller, iterator and allocator
#include "ser_cont_el.h"
#include "ser_iter_el.h"
#include "ser_alloc.h"

//  Singleton objects
#include "pqdfacty.h"       //  Factory object for creating other PQDIF objects
#include "pqdinfo.h"        //  PQDIF information object (Singleton)

//  Persistence controller - uses an enum defined in pqdfacty.h
#include "pcn_flat.h"   //  Concrete: Flat file controller

#include "pqdif_custom_2.h" // include implementation specific items

