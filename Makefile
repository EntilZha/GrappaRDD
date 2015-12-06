GRAPPA_IMPLICIT_RULES:=on

include $(GRAPPA_PREFIX)/share/Grappa/grappa.mk

main: main.o

main.o: RDD.hpp GrappaContext.hpp

clean:
	rm -f main.o main
