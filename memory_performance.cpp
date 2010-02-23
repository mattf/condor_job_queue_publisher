/***************************************************************
 *
 * Copyright (C) 2010 Red Hat, Inc.
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

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <boost/flyweight.hpp>
using namespace ::boost::flyweights;

using namespace std;

//typedef string Name;
//typedef string Value;
typedef string Id;
typedef flyweight<string> Name;
typedef flyweight<string> Value;
typedef map<Name, Value> Job;
typedef map<Id, Job> Jobs;

int
main(int argc, char *argv[])
{
	Jobs jobs;

	ifstream data;
	data.open(argv[1], ios::in);

	char _value[8192];
//	string id, name, value;
	flyweight<string> id, name, value;
	while (!data.eof()) {
		data >> id >> name; data.getline(_value, 8192); value = _value;
//		cout << "id: " << id << " name: " << name << " value: " << value << endl;
		jobs[id][name] = value;
	}

	cout << "DONE" << endl;

//	sleep(120);

	return 0;
}
