/***************************************************************
 *
 * Copyright (C) 2009 Red Hat, Inc.
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

#include "JobPublisherJobLogConsumer.h"

#include "Globals.h"

#include "Job.h"


JobPublisherJobLogConsumer::JobPublisherJobLogConsumer()
{ }

JobPublisherJobLogConsumer::~JobPublisherJobLogConsumer()
{ }

void
JobPublisherJobLogConsumer::Reset()
{
		// When deleting jobs, to avoid problems with jobs referencing
		// deleted clusters, we must be sure to delete the clusters
		// last

/*
	for (JobCollectionType::iterator i = g_jobs.begin();
		 g_jobs.end() != i; i++) {
			// Skip clusters for now
		if ('0' != (*i).second->GetKey()[0]) {
			delete (*i).second;
			g_jobs.erase(i);
		}
	}

	for (JobCollectionType::iterator i = g_jobs.begin();
		 g_jobs.end() != i; i++) {
		delete (*i).second;
		g_jobs.erase(i);
	}
*/
}

bool
JobPublisherJobLogConsumer::NewClassAd(const char *_key,
									const char */*type*/,
									const char */*target*/)
{
		// assert(key);
		// assert(key not in g_jobs);

	string key = _key;

	if ('0' == key[0]) {
			// Cluster ad
		if (g_jobs.end() == g_jobs.find(key)) {
			g_jobs[key] = Job(key, NULL);
		}
	} else {
			// Job ad
		int cluster, proc;
		sscanf(_key, "%d.%d", &cluster, &proc); // XXX

		char _cluster_key[15]; // ceiling(log(2^32)) + len(0.-1) + NULL = 15
		sprintf(_cluster_key, "0%d.-1", cluster); // XXX

		string cluster_key = _cluster_key;

		JobCollectionType::iterator element = g_jobs.find(cluster_key);

        // TODO this code assumes that we will always get the parent 
        // classad before its child from the job log...this is not strictly
        // guaranteed (e.g., compressed log?)
		if (g_jobs.end() == element) {
            // didn't find an existing job so create a new one
			g_jobs[cluster_key] = Job(key, NULL);
		} else {
            // found an existing job - we'll assume it is the target parent
            // of this new child so we need to get its job ad first
            g_jobs[key] = Job(key, &(*element).second);
		}
	}

	return true;
}

bool
JobPublisherJobLogConsumer::DestroyClassAd(const char *_key)
{
	g_jobs.erase(_key);

	return true;
}


bool
JobPublisherJobLogConsumer::SetAttribute(const char *_key,
									  const char *_name,
									  const char *_value)
{
	JobCollectionType::iterator element = g_jobs.find(_key);

	if (g_jobs.end() == element) {
		printf("error reading %s: no such job '%s' for '%s = %s'\n",
				m_reader->GetJobLogFileName(), _key, _name, _value);
		return false;
	}

	(*element).second.Set(_name, _value);

	return true;
}

bool
JobPublisherJobLogConsumer::DeleteAttribute(const char *_key,
										 const char *_name)
{
	JobCollectionType::iterator element = g_jobs.find(_key);

	if (g_jobs.end() == element) {
		printf("error reading %s: no such job '%s' for 'delete %s'\n",
				m_reader->GetJobLogFileName(), _key, _name);
		return false;
	}

	(*element).second.Delete(_name);

	return true;
}
