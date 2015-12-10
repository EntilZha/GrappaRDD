GRAPPA_IMPLICIT_RULES:=on

include $(GRAPPA_PREFIX)/share/Grappa/grappa.mk

LDFLAGS += -Wl,-rpath,$(GRAPPA_PREFIX)/lib

RDD_test.o: CXXFLAGS += -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MODULE=RDD_test

main: main.o

RDD_test: RDD_test.o

main.o: RDD_lib

RDD_test.o: RDD_lib

RDD_lib: RDD.hpp GrappaContext.hpp

clean:
	rm -f main.o main RDD_test.o RDD_test
