PRONAME		=dp2pcsc



CC			=gcc
OBJECTS		=$(PRONAME).o Queue.o friend.o listen_thread.o talk_thread.o show_thread.o
CFLAGS		=-std=gnu11 -g -lpthread  -Wall

### todo
terminal-run:$(PRONAME)
	gnome-terminal --hide-menubar --working-directory=$(PWD) -t "$(PRONAME)" -x bash -c "./$(PRONAME);"

count-code:
	echo "c file"|find ./ -name '*.h' |xargs wc -l
	echo "h file"|find ./ -name '*.c' |xargs wc -l
	echo "sh file"|find ./ -name '*.sh' |xargs wc -l

### .elf
$(PRONAME):$(OBJECTS)
	$(CC) -o $(PRONAME) $(OBJECTS)  $(CFLAGS)


### .o
$(PRONAME).o:client.h 

friend.o:friend.h

listen_thread.o:listen_thread.h

talk_thread.o:talk_thread.h

show_thread.o:show_thread.h

Queue.o:Queue.h

.PHONY:clean 
clean:
	rm $(OBJECTS) 
