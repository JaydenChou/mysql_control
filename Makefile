TARGET = example
OBJS = example.o
CPPFLAGS = -g `mysql_config --cflags` -I./include
CC = g++
LIB = `mysql_config --libs_r`

$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIB) 

example.o : example.cpp

.PHONY : clean
clean :
	rm $(TARGET) $(OBJS)