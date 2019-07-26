
cross:=arm-linux-gnueabihf

host:=
cc:=gcc

ifdef cross
	host=$(cross)
	cc=$(cross)-gcc
endif

#INC += -I/usr/include/glib-2.0 \
#	-I/usr/lib//x86_64-linux-gnu/glib-2.0/include \
#	-I/usr/include/gio-unix-2.0

interfacePrefix="org.wx.test"

ROOT=$(shell pwd)

INC += -I./libs/include
LIBS += -L./libs/lib
CFLAG +=  -O3 -g -W -Wall
CFLAG += -lpthread -lglib-2.0 -lgio-2.0 -lgobject-2.0

all:
	gcc -o s server.c comm.c test.c -g $(INC) $(LIBS) $(CFLAG)
	gcc -o c client.c comm.c test.c -g $(INC) $(LIBS) $(CFLAG)

xml:
	./libs/bin/gdbus-codegen --interface-prefix=$(interfacePrefix) --generate-c-code=test test.xml

zlib:
	@tar -xzf ./pkg/zlib-1.2.11.tar.gz -C ./libs && \
	cd $(ROOT)/libs/zlib-1.2.11 && \
	./configure --prefix=$(ROOT)/libs && \
	make -j4 && make install && \
	cd -

libffi:
	@tar -xzf ./pkg/libffi-3.2.1.tar.gz -C ./libs && \
	cd $(ROOT)/libs/libffi-3.2.1 && \
	./configure --prefix=$(ROOT)/libs --host=$(host) && \
	make -j4 && make install && \
	cd -

glib:
	@tar -xJf ./pkg/glib-2.40.2.tar.xz -C ./libs && \
	cd $(ROOT)/libs/glib-2.40.2 && \
	cp ../../pkg/arm-linux.cache ./ && \
	./configure --prefix=$(ROOT)/libs --host=$(host) ZLIB_LIBS=-L$(ROOT)/libs/lib ZLIB_CFLAGS=-I$(ROOT)/libs/include LIBFFI_LIBS=-L$(ROOT)/libs/lib LIBFFI_CFLAGS=-I$(ROOT)/libs/lib/libffi-3.2.1/include --cache-file=arm-linux.cache && \
	make -j4 && make install && \
	cd -

dbus:
	@tar -xzf ./pkg/dbus-1.13.4.tar.gz -C ./libs && \
	cd $(ROOT)/libs/dbus-1.13.4 && \
	./configure --prefix=$(ROOT)/libs --host=$(host) && \
	make -j4 && make install && \
	cd -

clean:
	rm ./s ./c

cleanall:
	rm ./s ./c ./libs/* -rf


