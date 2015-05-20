OBJS = serial2tcp

#for shuncom platform
#CLOUD_PLATFORM=-DCLOUD_PLAT_SZ
#for jd platform
#CLOUD_PLATFORM=-DCLOUD_PLAT_JD
#for shuncom ap gateway
CLOUD_PLATFORM=

CFLAGS += -Wall -O2 $(CLOUD_PLATFORM)

objs_list = 
objs_serial = serial.o
objs_socket = socket.o

all: $(OBJS)

serial2tcp: main.o $(objs_serial) $(objs_list) $(objs_socket)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -levent

clean:
	rm -rf *.o $(OBJS)

.PHONY: clean
