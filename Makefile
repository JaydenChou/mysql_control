TARGET = main
OBJS = field.o mysql_control.o main.o
CFLAGS = -g
CC = g++
LIB = -lmysqlclient
$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIB)

$(OBJS) : field.h mysql_control.h

field.o : field.cpp
mysql_control.o : mysql_control.cpp
mian.o : mian.cpp

.PHONY : clean
clean :
	rm main $(OBJS)