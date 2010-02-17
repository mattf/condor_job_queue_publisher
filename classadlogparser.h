/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

#ifndef _CLASSADLOGPARSER_H_
#define _CLASSADLOGPARSER_H_

#include <limits.h> // for _POSIX_PATH_MAX

#include "condor_open.h"

#include "quill_enums.h"

//used to distinguish between first and successive calls
#define IMPOSSIBLE_OFFSET -10000

//! ClassAdLogParser
/*! \brief Parser for ClassAd Log file
 *
 *  It actually reads and parses ClassAd Log file (job_queue.log)
 */
class ClassAdLogParser
{
public:

		//! constructor	
	ClassAdLogParser();
		//! destructor	
	~ClassAdLogParser();
	
		//
		// accessors
		//
	
	    //! get the file handles
	int     getFileDescriptor();
	FILE *  getFilePointer();

		//! set the file handles
	void    setFilePointer(FILE *fp);
    void setFileDescriptor(int fd);

		//! return the current ClassAd log entry
	ClassAdLogEntry* 	getLastCALogEntry();
		//! return the last ClassAd log entry
	ClassAdLogEntry* 	getCurCALogEntry();
		//! set the next offset. IMPOSSIBLE_OFFSET is just a default value	
	void 	setNextOffset(long offset = IMPOSSIBLE_OFFSET);
	//!	set a job queue file name
	void	setJobQueueName(const char* jqn);
	//!	get a job queue file name
	char*	getJobQueueName();
	//!	get a current file offset
	long	getCurOffset();
	//!	set a current file offset
	void 	setCurOffset(long offset);
	//!	get a current classad log entry data as a New ClassAd command
	QuillErrCode 	getNewClassAdBody(char*& key, 
									  char*& mytype, 
									  char*& targettype);
	//!	get a current classad log entry data as a Destroy ClassAd command
	QuillErrCode 	getDestroyClassAdBody(char*& key);
	//!	get a current classad log entry data as a Set Attribute command
	QuillErrCode 	getSetAttributeBody(char*& key, 
										char*& name, 
										char*& value);
	//!	get a current classad log entry data as a Delete Attribute command
	QuillErrCode 	getDeleteAttributeBody(char*& key, char*& name);

	//!	get the body of a historical sequence number command
	QuillErrCode	getLogHistoricalSNBody(char*& seqnum, char*& timestamp);

	//! read a classad log entry in the current offset of a file
	FileOpErrCode readLogEntry(int &op_type);

	FileOpErrCode openFile();
	FileOpErrCode closeFile();

private:
		//
		// helper functions
		// 
	int 	readHeader(FILE *fp, int& op_type);
	int 	readword(FILE *fp, char *&);
	int 	readword(int, char *&);
	int 	readline(FILE *fp, char *&);
	int 	readline(int, char *&);


	int	    readLogHistoricalSNBody(FILE *fp);	
	int 	readNewClassAdBody(FILE *fp);
	int 	readDestroyClassAdBody(FILE *fp);
	int 	readSetAttributeBody(FILE *fp);
	int 	readDeleteAttributeBody(FILE *fp);
	int 	readBeginTransactionBody(FILE *fp);
	int 	readEndTransactionBody(FILE *fp);
		
		//
		// data
		//	
	char	job_queue_name[_POSIX_PATH_MAX];//!< job queue log file path
	long	nextOffset;						//!< next offset

	ClassAdLogEntry		curCALogEntry; 	//!< current ClassAd log entry
	ClassAdLogEntry		lastCALogEntry; //!< last ClassAd log entry 

	FILE 	*log_fp;
};

#endif /* _CLASSADLOGPARSER_H_ */
