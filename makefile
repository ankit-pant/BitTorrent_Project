#Flash_Share Torrentz
#By:
#  Ankit Pant
#  2018201035

all: client_2018201035 tracker1 tracker2
CC = g++
CFLAGS = -Wall -std=c++14
LIBS1 = -lssl -lcrypto
DEPS1 = gen_mtor.h share_wth_trakr.h get_frm_trakr.h get_frm_peer
OBJ1 = gen_mtor.o share_wth_trakr.o get_frm_trakr.o get_frm_peer.o remove_frm_trakr.o client.o
LIBS2 = -pthread
DEPS2 = trakr.h
OBJ2 = trakr1.o
OBJ3 = trakr2.o

%.o: %.cpp $(DEPS1)
	$(CC) -c -o  $@ $< $(CFLAGS) $(LIBS1)

%.o: %.cpp $(DEPS2)
	$(CC) -c -o  $@ $< $(CFLAGS) $(LIBS2)

client_2018201035: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS1)

tracker1: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS2)

tracker2: $(OBJ3)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS2)


