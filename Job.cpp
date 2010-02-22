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

#include "Job.h"

Job::Job(const string _key,
		 const Job *_parent):
	m_key(_key),
	m_parent(_parent)
{
}

Job::Job()
{
}

Job::Job(const Job &_job)
{
	*this = _job;
}

Job &Job::operator=(const Job &_job)
{
	m_key = _job.m_key;
	m_parent = _job.m_parent;

	return *this;
}

Job::~Job()
{
}

const string
Job::GetKey() const
{
	return m_key;
}

bool
Job::Get(const string _name, fstring &attribute) const
{
		// Lookup locally, fall back on parent

	AttributeMapType::const_iterator element =
		m_attributes.find(fstring(_name));

	if (m_attributes.end() == element) {
		if (m_parent) {
			return m_parent->Get(_name, attribute);
		}

		return false;
	}

	attribute = (*element).second;

	return true;
}

void
Job::Set(const string _name, const string _value)
{
	m_attributes[fstring(_name)] = fstring(_value);
}

void
Job::Delete(const string _name)
{
	m_attributes.erase(fstring(_name));
}

const Job::AttributeMapType &
Job::GetAttributes() const
{
	return m_attributes;
}

const Job::AttributeMapType &
Job::GetParentAttributes() const
{
	static AttributeMapType s_attrs;
	if (m_parent) {
		return m_parent->GetAttributes();
	} else {
		return s_attrs;
	}
}
