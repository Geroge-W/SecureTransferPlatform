CC=g++
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TARGET=TcpServer
CFLAGS=-Wall -lpthread

ALL:$(TARGET)
%.o:%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@ 

clean:
	-rm -rf $(TARGET) $(OBJS)

.PHONY:clean ALL
