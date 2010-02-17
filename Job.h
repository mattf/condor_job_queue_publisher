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

#ifndef _JOB_H
#define _JOB_H

#include <string>
#include <map>
#include <set>

#include "AttributeValueType.h"

using std::string;
using std::map;
using std::set;

class SubmissionObject;

class Attribute
{
public:
	Attribute(AttributeValueType _type, const string _value);
	Attribute(const Attribute &_attribute);
	Attribute();
	Attribute & operator=(const Attribute &_attribute);
	~Attribute();

	AttributeValueType GetType() const;
	const string GetValue() const;

private:
	AttributeValueType m_type;
	string m_value;
};

class Job
{
public:
	typedef map<const string, Attribute> AttributeMapType;

	Job(const string _key,
		const Job *_parent);
	Job(const Job &_job);
	Job();
	Job & operator=(const Job &_job);
	~Job();

	const string GetKey() const;

	bool Get(const string name, Attribute &attribute) const;
	void Set(const string name, const string value);
	void Delete(const string name);

	const AttributeMapType & GetAttributes() const;

		// XXX: Add iterator to Job and remove this
	const AttributeMapType & GetParentAttributes() const;

private:
	string m_key;
	const Job * m_parent;
	AttributeMapType m_attributes;
};

#endif /* _JOB_H */
