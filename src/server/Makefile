CC = gcc
TARGET = chat-server
OBJS = ../chat.o server.o main.o
LDLIBS = -lpthread

all : $(TARGET)

main.o: ../chat.h ../chat.c server.h server.c main.c
server.o: ../chat.h ../chat.c server.h

$(TARGET) : $(OBJS)
	$(CC) -o ../../bin/$(@) $(OBJS) $(LDLIBS)

clean:
	@rm -f *.o
	@rm -f ../../bin/$(TARGET)