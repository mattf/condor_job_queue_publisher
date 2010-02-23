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

#ifndef __QUILL_ENUMS_H__
#define __QUILL_ENUMS_H__

enum QuillErrCode {	    QUILL_FAILURE,
						FAILURE_QUERY_PROCADS_HOR,
						FAILURE_QUERY_PROCADS_VER,
						FAILURE_QUERY_CLUSTERADS_HOR,
						FAILURE_QUERY_CLUSTERADS_VER,
						FAILURE_QUERY_HISTORYADS_HOR,
						FAILURE_QUERY_HISTORYADS_VER,
						JOB_QUEUE_EMPTY,
						HISTORY_EMPTY,
						DONE_JOBS_CURSOR,
						DONE_HISTORY_HOR_CURSOR,
						DONE_HISTORY_VER_CURSOR,
						DONE_CLUSTERADS_CURSOR,
						DONE_PROCADS_CURSOR,
						DONE_PROCADS_CUR_CLUSTERAD,
						QUILL_SUCCESS};

enum FileOpErrCode {    FILE_OPEN_ERROR,
						FILE_READ_ERROR,
						FILE_WRITE_ERROR,
						FILE_FATAL_ERROR,
						FILE_READ_EOF,
                        FILE_READ_SUCCESS,
                        FILE_OP_SUCCESS};
#endif







