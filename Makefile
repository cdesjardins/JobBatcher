CXXFLAGS += -I $(BOOSTDIR) -Wall
LDFLAGS += -lboost_system -lboost_thread -lrt -lpthread -L $(BOOSTDIR)/stage/lib

all: main.o
	g++ $^ $(LDFLAGS) -o jobbatcherdemo

main.o: main.cpp
clean:
	-rm *.o jobbatcherdemo
