

all : Internals.o 

Internal.o : Utilities/Internal.cpp Utilities.Internal.h
	gcc Utilities/Internal.cpp

#AudioInternals.o : AudioInternals.cpp AudioInternals.h
#	gcc AudioInternals.cpp
