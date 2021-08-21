DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./lib
AR      = ar
RANLIB  = ranlib
TAR_GET = libinterface.a
SOTAR_GET = libinterface.so
SRC = $(wildcard ${DIR_SRC}/*.cpp)  
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 

LIB_TARGET = ${DIR_BIN}/${TAR_GET}
LIBSO_TARGET = $(DIR_BIN)/${SOTAR_GET}
CC = g++
CFLAGS = -g -Wall -I${DIR_INC} -lcrypto -ljson

ALL:$(LIB_TARGET) $(LIBSO_TARGET)

# 生成静态库
${LIB_TARGET}:${OBJ}
	${AR} cru ${LIB_TARGET} $(OBJ)
	${RANLIB} ${LIB_TARGET}

# 生成动态库
${LIBSO_TARGET}:$(OBJ)
	$(CC) $(CFLAGS) -fPIC -shared $(OBJ) -o $(LIBSO_TARGET)	

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
.PHONY:clean
clean:
	rm ${DIR_OBJ}/*.o $(LIBSO_TARGET) $(LIB_TARGET)
