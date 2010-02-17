FILES=JobLogReader.o classadlogentry.o prober.o classadlogparser.o

all: JobLogReader.o classadlogentry.o prober.o classadlogparser.o

clean:
	rm -f $(FILES)
