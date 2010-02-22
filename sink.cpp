#include <qpid/messaging/Connection.h>
#include <qpid/messaging/MapView.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Session.h>

#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

using namespace std;
using namespace qpid::messaging;

void
usage(char *argv[])
{
	cerr <<	"usage: " << argv[0]
		 << "--address <queue or topic> "
		 << "[--broker <broker url>]"
		 << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	string broker = "amqp:tcp:127.0.0.1:5672";
	string address;

	static struct option options[] = {
		{"broker", 1, NULL, 'b'},
		{"address", 1, NULL, 'a'},
		{NULL, NULL, NULL, NULL}
	};

	int c;
	while (1) {
		c = getopt_long(argc, argv, ":b:a:", options, NULL);
		if (-1 == c) break;

		switch (c) {
		case 'b':
			broker = optarg;
			break;
		case 'a':
			address = optarg;
			break;
		case ':':
			cerr << argv[optind - 1] << " requires an argument" << endl;
			usage(argv);
			break;
		case '?':
			cerr << "unknown argument: " << argv[optind - 1] << endl;
			usage(argv);
			break;
		default:
			usage(argv);
			break;
		}
	}

	if (address.empty()) {
		cerr << "--address required" << endl;
		usage(argv);
	}

	cout << "config: address = " << address << "; broker = " << broker << endl;

	Connection connection;
	connection.open(broker);
	Session session = connection.newSession();
	Receiver receiver = session.createReceiver(address);

	while (1) {
		Message message = receiver.fetch();
		MapView content(message);
		cout << content << endl;
		session.acknowledge();
	}

	receiver.close();
	connection.close();

	return 0;
}
