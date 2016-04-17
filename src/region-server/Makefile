GXX=g++ -std=c++11
CFLAGS=-O0 -g -Wall -Werror

SRC:=$(wildcard serv/*.cpp util/*.cpp proto/*.cc)
INCLUDE=-I../../include \
	-I./ \
	-I./proto/
LIBS=-L/home/vagrant/hadoop/lib/native -lhdfs \
	-lpthread \
	-lprotobuf \
	-lsnappy\
	-lz
STATIC_LIBS:=$(wildcard ../../libs/*.a)
OBJ:=$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SRC)))
WORK_DIR=$(shell pwd)
TMP_DIR=${WORK_DIR}/temp

LINKER=ar crs

ALL:libstorage.a region_server

region_server: region_main.cpp libstorage.a
	@echo "[BUILD " $@ "]"
	${GXX} ${CFLAGS} ${INCLUDE} ${LIBS} region_main.cpp libstorage.a -o region_server

%.o:%.cpp
	@echo "[BUILD " $@ "]"
	${GXX} -c ${CFLAGS} ${INCLUDE} ${LIBS} $< -o $@

%.o:%.cc
	@echo "[BUILD " $@ "]"
	${GXX} -c ${CFLAGS} ${INCLUDE} ${LIBS} $< -o $@

libstorage.a: ${OBJ}
	@echo "[RELEASE " ${STATIC_LIBS} "]"
	rm -rf ${TMP_DIR}
	mkdir ${TMP_DIR}
	cp ${STATIC_LIBS} ${TMP_DIR}
	cd ${TMP_DIR} && find . | grep '\.a' | xargs -i ar x {}
	@echo "[LINK " $@ "]"
	${LINKER} libstorage.a ${TMP_DIR}/* ${OBJ}
	rm ${TMP_DIR} -rf

clean:
	rm ${OBJ} -f
	rm libstorage.a -f
	rm region_server -f
