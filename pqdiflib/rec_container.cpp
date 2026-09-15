/*
**  Class:          CPQDIF_R_Container
**  Description:    Implements a PQDIF record "wrapper" for the data source record. You can cast a standard record object to this class.
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


//  Operations

bool CPQDIF_R_Container::GetCompressionInfo
	(
    UINT4&  styleComp,		//	Compression style (output)
    UINT4&  algComp			//	Compression algorithm (output)
	)
{
	bool		status = false;
	GUID		tagRecord;
	PQDIFValue	value;

	//	Verify that the record has the right header tag
	status = HeaderGetTag( tagRecord );
	if( status && PQDIF_IsEqualGUID( tagRecord, tagContainer) && m_pcollMain )
	{
		//	It's the right type of record.
		//	
		status = GetScalarValueInCollection( m_pcollMain, 
			tagCompressionStyleID, ID_PHYS_TYPE_UNS_INTEGER4, value );
		if( status )
		{
			//	We got compression style.
			styleComp = value.uint4;

			//	Now try to get compression algorithm
			status = GetScalarValueInCollection( m_pcollMain, 
				tagCompressionAlgorithmID, ID_PHYS_TYPE_UNS_INTEGER4, value );
			if( status )
			{
				//	We got compression algorithm. All done.
				algComp = value.uint4;
			}
		}
	}

	return status;
}


bool CPQDIF_R_Container::SetInfo
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
            )
    {
    bool rc = false;

    if( m_pcollMain )
        {
        //  Add string entries
        m_pcollMain->SetVectorString( tagLanguage, language );
        m_pcollMain->SetVectorString( tagTitle, title );
        m_pcollMain->SetVectorString( tagSubject, subject );
        m_pcollMain->SetVectorString( tagAuthor, author );
        m_pcollMain->SetVectorString( tagKeywords, keywords );
        m_pcollMain->SetVectorString( tagComments, comments );
        m_pcollMain->SetVectorString( tagLastSavedBy, lastSavedBy );
        m_pcollMain->SetVectorString( tagApplication, application );
        m_pcollMain->SetVectorString( tagSecurity, security );

        m_pcollMain->SetVectorString( tagOwner, owner );
        m_pcollMain->SetVectorString( tagCopyright, copyright );
        m_pcollMain->SetVectorString( tagTrademarks, trademarks );

        m_pcollMain->SetVectorString( tagNotes, notes );

        rc = true;
        }

    return rc;
    }

