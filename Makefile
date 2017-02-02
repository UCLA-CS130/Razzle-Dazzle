GTEST_DIR = googletest/googletest
COVFLAGS = -std=c++0x -isystem $(GTEST_DIR)/include -pthread -fprofile-arcs -ftest-coverage

all:
	g++ config_parser.cc connection.cc webserver.cc server.cc -o webserver -std=c++0x -g -Wall -Werror -lboost_system -lpthread

run:
	g++ config_parser.cc connection.cc reply.cc webserver.cc -o webserver -std=c++0x -g -Wall -Werror -lboost_system -lpthread
	./webserver config_file

clean:
	rm webserver

clean-emacs:
	rm *~

clean-tests:
	rm config_parser_test server_test connection_test

test:
	g++  $(COVFLAGS) config_parser_test.cc config_parser.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o config_parser_test
	g++ $(COVFLAGS) server_test.cc server.cc connection.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o server_test -lboost_system 
	g++ $(COVFLAGS) connection_test.cc connection.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o connection_test -lboost_system 
	./server_test && ./connection_test && ./config_parser_test

integration:
	python integration.py

coverage: test
	gcov -r server.cc connection.cc config_parser.cc
