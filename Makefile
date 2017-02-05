GTEST_DIR = googletest/googletest
STDFLAGS = -std=c++0x -g -Wall -Werror -pthread -lboost_system
COVFLAGS = -isystem $(GTEST_DIR)/include -fprofile-arcs -ftest-coverage

all:
	g++ config_parser.cc connection.cc server.cc reply.cc webserver.cc -o webserver $(STDFLAGS) -lboost_system

run:
	g++ config_parser.cc connection.cc server.cc reply.cc webserver.cc -o webserver $(STDFLAGS) -lboost_system
	./webserver config_file

clean:
	rm webserver

clean-emacs:
	rm *~

clean-tests:
	rm config_parser_test server_test connection_test

test:
	./build_tests.sh
	g++ server_test.cc server.cc connection.cc reply.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o server_test $(STDFLAGS) $(COVFLAGS)
	g++ connection_test.cc connection.cc reply.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o connection_test $(STDFLAGS) $(COVFLAGS)
	./server_test && ./connection_test && ./config_parser_test

integration:
	python integration.py

coverage: test
	gcov -r server.cc connection.cc config_parser.cc

clean-coverage:
	rm *gcov *gcda *gcno
