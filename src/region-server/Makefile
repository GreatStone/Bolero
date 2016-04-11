GXX=g++ -std=c++11
CFLAGS=-O0 -g -Wall -Werror

SRC:=$(wildcard serv/*.cpp util/*.cpp proto/*.cc)
INCLUDE=-I../../include \
	-I./ \
	-I./proto/
LIBS=-L/home/vagrant/hadoop/lib/native -lhdfs \
	-lpthread
STATIC_LIBS:=$(wildcard ../../libs/*.a)
OBJ:=$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SRC)))
WORK_DIR=$(shell pwd)
TMP_DIR=${WORK_DIR}/temp

LINKER=ar crs

ALL:libstorage.a

%.o:%.cpp
	echo "build " $@
	${GXX} -c ${CFLAGS} ${INCLUDE} ${LIBS} $< -o $@

%.o:%.cc
	echo "build " $@
	${GXX} -c ${CFLAGS} ${INCLUDE} ${LIBS} $< -o $@

libstorage.a: ${OBJ}
	echo ${STATIC_LIBS}
	rm -rf ${TMP_DIR}
	mkdir ${TMP_DIR}
	cp ${STATIC_LIBS} ${TMP_DIR}
	cd ${TMP_DIR} && find . | grep '\.a' | xargs -i ar x {}
	${LINKER} libstorage.a ${TMP_DIR}/* ${OBJ}
	rm ${TMP_DIR} -rf

clean:
	rm ${OBJ} -f
	rm libstorage.a -f
	rm test -f

test: test.cpp libstorage.a
	${GXX} ${INCLUDE} test.cpp libstorage.a -o test ${LIBS}
