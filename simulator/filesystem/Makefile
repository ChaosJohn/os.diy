CC = gcc 
CFLAG = 
EXECUTABLE = test.out

all: $(EXECUTABLE) 
#all: test.out

#run: 
	#shell ./$(EXECUTABLE) 

#.c.o: 
	#$(CC) $(CFLAG) -c -o $*.o $< 

$(EXECUTABLE): test.o fs.o utils.o permission.o cmd.o mm.o jobs.o 
	$(CC) $(CFLAG) -o $(EXECUTABLE) \
		test.o fs.o utils.o permission.o cmd.o mm.o jobs.o 

clean: 
	rm -rf *.o 

dist-clean: clean 
	rm -rf $(EXECUTABLE) 

