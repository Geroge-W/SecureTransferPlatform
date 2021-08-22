CC=g++
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TARGET=TcpClient
CFLAGS=-Wall -L /home/lbw/projects/secure_platform/Interface/lib -lpthread -linterface

ALL:$(TARGET)
%.o:%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@ 

clean:
	-rm -rf $(TARGET) $(OBJS)

.PHONY:clean ALL
