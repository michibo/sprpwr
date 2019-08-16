CC=
CXX=clang++
RM=rm -f

CPPFLAGS=-O3 -march=native -std=c++11 -Wall
LDFLAGS=-O3 -march=native

LDLIBS=-lgmpxx -lgmp

SRCS=test.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all : test

test : $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend test

include .depend
