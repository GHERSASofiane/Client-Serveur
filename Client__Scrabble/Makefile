# change application name here (executable output name)
TARGET=scrabble

GTKLIB=`pkg-config --libs --cflags gtk+-3.0`
  
OBJS0=	Function/Function_Main.c
OBJS1=	Function/Function_Connection.c
OBJS2=	main.c

# Jeux de tests
  
OBJS_test_0=	Test/Function_Main.c
OBJS_test_1=	Test/Function_Connection.c
OBJS_test_2=	main.c

 
all: clean
	gcc -o $(TARGET) $(OBJS0) $(OBJS1) $(OBJS2) $(GTKLIB)
	./$(TARGET)
	clean

tests: clean
	gcc -o $(TARGET) $(OBJS_test_0) $(OBJS_test_1) $(OBJS_test_2) $(GTKLIB)
	./$(TARGET)
	clean
	
clean:
	rm -rf *.o