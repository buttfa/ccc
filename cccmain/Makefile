#### General parameters 
# Set Target type (lib or app)
TARGET_TYPE=lib

# Set Language (c or cpp)
LANGUAGE=cpp
# Set Compiler
COMPILER=g++
# Set Linker
LINKER=g++

# Set Source folders (Please write each folder containing source files into it. And enclosed in single quotation marks.)
SOURCE_FOLDERS='./cccmain/src/'
# Set Header folders (Please write each folder containing header files into it. And enclosed in single quotation marks.)
HEADER_FOLDERS='./cccproject/inc/ ./cccproject/inc/ccc ./.'
# Set Object files path
OBJ_PATH=./cccmain/build/obj/

#### If TARGET_TYPE=lib, Please set the following parameters.
# Set Lib type (static or dynamic)
LIB_TYPE=static
# LIB_PATH is an optional parameter used to set the storage path for compiled library file. (default to./build/lib)
LIB_PATH=./build/lib/
LIB_NAME=cccmain.a
# Set the folder path for storing the header files of the library project. (Will preserve the file organization structure)
HEADER_PATH=./cccmain/build/inc/

PROJECT_ARGS=LANGUAGE=$(LANGUAGE) COMPILER=$(COMPILER) COMPILE_FLAGS='$(COMPILE_FLAGS)' LINKER=$(LINKER) LINK_FLAGS=$(LINK_FLAGS) LIBRARY_FILES=$(LIBRARY_FILES) TARGET_TYPE=$(TARGET_TYPE) SOURCE_FOLDERS=$(SOURCE_FOLDERS) HEADER_FOLDERS=$(HEADER_FOLDERS) OBJ_PATH=$(OBJ_PATH) LIB_NAME=$(LIB_NAME) LIB_TYPE=$(LIB_TYPE) LIB_PATH=$(LIB_PATH) HEADER_PATH=$(HEADER_PATH) APP_PATH=$(APP_PATH) APP_NAME=$(APP_NAME) -f ./third_party/make-tmpl/Makefile

compile:
	make $(PROJECT_ARGS)

info:
	make info $(PROJECT_ARGS)

clean:
	make clean $(PROJECT_ARGS)
