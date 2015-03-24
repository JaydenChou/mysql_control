TARGET = example
OBJS = field.o mysql_control.o example.o
CPPFLAGS = -g `mysql_config --cflags`
CC = g++
LIB = `mysql_config --libs_r`
$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIB)

$(OBJS) : field.h mysql_control.h

field.o : field.cpp
mysql_control.o : mysql_control.cpp
example.o : example.cpp

.PHONY : clean
clean :
	rm main $(OBJS)