GXX=g++ -std=c++11
CFLAGS=-O0 -g -Wall -Werror

SRC:=$(wildcard rpc/*.cpp proto/*.cc forward/*.cpp)
INCLUDE=-I../../include \
	-I./ \
	-I./proto/
LIBS=	-lpthread \
	-lprotobuf \
	-lsnappy \
	-lz \
	-levent

STATIC_LIBS:=$(wildcard ../../libs/*.a)
OBJ:=$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SRC)))
WORK_DIR=$(shell pwd)
TMP_DIR=${WORK_DIR}/temp

LINKER=ar crs

ALL: libproxyrpc.a

%.o:%.cpp
	@echo "[BUILD " $@ "]"
	${GXX} -c ${CFLAGS} ${INCLUDE} ${LIBS} $< -o $@

%.o:%.cc
	@echo "[BUILD " $@ "]"
	${GXX} -c ${CFLAGS} ${INCLUDE} ${LIBS} $< -o $@

libproxyrpc.a: ${OBJ}
	@echo "[RELEASE " ${STATIC_LIBS} "]"
	rm -rf ${TMP_DIR}
	mkdir ${TMP_DIR}
	cp ${STATIC_LIBS} ${TMP_DIR}
	cd ${TMP_DIR} && find . | grep '\.a' | xargs -i ar x {}
	@echo "[LINK " $@ "]"
	${LINKER} libproxyrpc.a ${TMP_DIR}/* ${OBJ}
	rm ${TMP_DIR} -rf

clean:
	rm ${OBJ} -f
	rm libproxyrpc.a -f
	rm test -f

test: test.cpp libproxyrpc.a
	${GXX} ${CFLAGS} ${INCLUDE} ${LIBS} ${OBJ} test.cpp libproxyrpc.a -o test
