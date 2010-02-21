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
	g_jobs.clear();
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

		Job parent;
		if (g_jobs.end() == element) {
			parent = Job(cluster_key, NULL);
			g_jobs[cluster_key] = parent;
		} else {
			parent = (*element).second;
		}

		g_jobs[key] = Job(key, &parent);
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
	string key = _key, name = _name, value = _value;

	JobCollectionType::iterator element = g_jobs.find(key);

	if (g_jobs.end() == element) {
		printf("error reading %s: no such job '%s' for '%s = %s'\n",
				m_reader->GetJobLogFileName(), _key, _name, _value);
		return false;
	}

	(*element).second.Set(name, value);

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
