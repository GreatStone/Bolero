GXX=g++ -std=c++11
CFLAGS=-O0 -g -Wall -Werror

SRC:=$(wildcard serv/*.cpp util/*.cpp)
INCLUDE=-I../../include\
	-I./
LIBS=-L../../libs
OBJ:=$(patsubst %cpp,%o,$(SRC))
LINKER=ar crs

ALL:libstorage.a

%.o:%.cpp
	echo "build " $@
	${GXX} -c ${CFLAGS} ${INCLUDE} ${LIBS} $< -o $@

libstorage.a: ${OBJ}
	${LINKER} libstorage.a ${OBJ}

clean:
	rm ${OBJ} -f
	rm libstorage.a
