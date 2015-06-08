CC = gcc 
CFLAG = 
EXECUTABLE = MiniOS 

all: $(EXECUTABLE) 
#all: test.out

#run: 
	#shell ./$(EXECUTABLE) 

#.c.o: 
	#$(CC) $(CFLAG) -c -o $*.o $< 

$(EXECUTABLE): main.o fs.o utils.o permission.o cmd.o mm.o jobs.o 
	$(CC) $(CFLAG) -o $(EXECUTABLE) \
		main.o fs.o utils.o permission.o cmd.o mm.o jobs.o 

clean: 
	rm -rf *.o 

dist-clean: clean 
	rm -rf $(EXECUTABLE) 

