#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <boost/flyweight.hpp>
using namespace ::boost::flyweights;

using namespace std;

//typedef string Id;
//typedef string Name;
//typedef string Value;
typedef flyweight<string> Id;
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
