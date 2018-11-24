LIBFLAG = $(shell pkg-config  allegro --libs --cflags)

default: dots-boxes

dots-boxes: dots-boxes.c
		cc dots-boxes.c -o dots-boxes $(LIBFLAG)

clean:
	 rm -f dots-boxes
