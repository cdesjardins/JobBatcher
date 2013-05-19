CXXFLAGS += -I ~/boost_1_53_0
LDFLAGS += -lboost_system -lboost_thread -lrt -lpthread -L ~/boost_1_53_0/stage/lib

all: main.o
	g++ $^ $(LDFLAGS)
	
