CC = gcc
CFLAGS = -Wall -O2 -Iezxdisp/include
LDFLAGS = -Lezxdisp/src/x11 EZX11.o -lezx

X11_LIB = -lX11

SAMPLES = CT
SAMPLES_OBJS = $(SAMPLES:=.o)

all:	libezx.a EZX11.o $(SAMPLES_OBJS) $(SAMPLES)

libezx.a:
	cd ezxdisp/src/x11 && make libezx.a

% : %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -lezx $(X11_LIB) -lm

%.o : %.c
	$(CC) -I../../include $(CFLAGS) -c $<

clean:
	cd ezxdisp/src/x11 && make clean
	rm -f *.o $(SAMPLES)
