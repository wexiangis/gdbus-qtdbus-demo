
## arm
#cross:=arm-linux-gnueabihf

## hi3516
#cross:=arm-himix200-linux

## ubuntu
# do nothing

host:=
cc:=gcc

ifdef cross
	host=$(cross)
	cc=$(cross)-gcc
endif

ROOT=$(shell pwd)

INC += -I./libs/include
INC += -I./libs/include/gio-unix-2.0
INC += -I./libs/include/glib-2.0
INC += -I./libs/include/dbus-1.0
LIBS += -L./libs/lib
CFLAG +=  -O3 -g -W -Wall
CFLAG += -lpthread -lglib-2.0 -lgio-2.0 -lgobject-2.0 -lffi -lz -lgmodule-2.0

target:
	$(cc) -o s server.c comm.c test.c $(INC) $(LIBS) $(CFLAG)
	$(cc) -o c client.c comm.c test.c $(INC) $(LIBS) $(CFLAG)

xml:
	./libs/bin/gdbus-codegen --interface-prefix=org.wx.test --generate-c-code=test test.xml

pkg: zlib libffi glib expat dbus

zlib:
	@tar -xzf ./pkg/zlib-1.2.11.tar.gz -C ./libs && \
	cd $(ROOT)/libs/zlib-1.2.11 && \
	export CC=$(cc) && \
	./configure --prefix=$(ROOT)/libs && \
	make -j4 && make install && \
	export CC=gcc && \
	cd - && \
	rm $(ROOT)/libs/zlib-1.2.11 -rf

libffi:
	@tar -xzf ./pkg/libffi-3.2.1.tar.gz -C ./libs && \
	cd $(ROOT)/libs/libffi-3.2.1 && \
	./configure --prefix=$(ROOT)/libs --host=$(host) CC=$(cc) && \
	make -j4 && make install && \
	cp $(ROOT)/libs/lib/libffi-3.2.1/include/* $(ROOT)/libs/include/ && \
	rm $(ROOT)/libs/lib/libffi-3.2.1 -rf && \
	cd - && \
	rm $(ROOT)/libs/libffi-3.2.1 -rf

glib:
	@tar -xJf ./pkg/glib-2.40.2.tar.xz -C ./libs && \
	cd $(ROOT)/libs/glib-2.40.2 && \
	cp ../../pkg/arm-linux.cache ./ && \
	./configure --prefix=$(ROOT)/libs --host=$(host) ZLIB_LIBS="-lz -L$(ROOT)/libs/lib" ZLIB_CFLAGS=-I$(ROOT)/libs/include LIBFFI_LIBS="-lffi -L$(ROOT)/libs/lib" LIBFFI_CFLAGS=-I$(ROOT)/libs/include --cache-file=arm-linux.cache CC=$(cc) && \
	sed -i "/#include \"config.h\"/i\#pragma GCC diagnostic push\"-Wformat-nonliteral\"" ./glib/gdate.c && \
	sed -i "/#include \"config.h\"/i\#pragma GCC diagnostic ignored\"-Wformat-nonliteral\"" ./glib/gdate.c && \
	make -j4 && make install && \
	cp ./glib/glibconfig.h $(ROOT)/libs/include/glib-2.0 && \
	cd - && \
	rm $(ROOT)/libs/glib-2.40.2 -rf

expat:
	@tar -xJf ./pkg/expat-2.2.7.tar.xz -C ./libs && \
	cd $(ROOT)/libs/expat-2.2.7 && \
	./configure --prefix=$(ROOT)/libs --host=$(host) && \
	make -j4 && make install && \
	cd - && \
	rm $(ROOT)/libs/expat-2.2.7 -rf

dbus:
	@tar -xzf ./pkg/dbus-1.13.4.tar.gz -C ./libs && \
	cd $(ROOT)/libs/dbus-1.13.4 && \
	./configure --prefix=$(ROOT)/libs --host=$(host) EXPAT_LIBS="-lexpat -L$(ROOT)/libs/lib" EXPAT_CFLAGS=-I$(ROOT)/libs/include GLIB_LIBS="-lffi -lgobject-2.0 -lglib-2.0 -lgio-2.0 -L$(ROOT)/libs/lib" GLIB_CFLAGS="-I$(ROOT)/libs/include/glib-2.0 -I$(ROOT)/libs/include/glib-2.0/gobject -I$(ROOT)/libs/include/glib-2.0/glib" && \
	make -j4 && make install && \
	cd - && \
	rm $(ROOT)/libs/dbus-1.13.4 -rf

clean:
	rm ./s ./c -rf

cleanall:
	rm ./s ./c ./libs/* -rf

