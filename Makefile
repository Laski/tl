all: parser

OBJS = parser.o  \
       main.o    \
       tokens.o  \

CPPFLAGS = `llvm-config --cppflags`
LDFLAGS = `llvm-config --ldflags`
LIBS = `llvm-config --libs`

clean:
	$(RM) -rf parser.cpp parser.hpp parser tokens.cpp $(OBJS)

parser.cpp: parser.y
	bison -d -o $@ $^
	
parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	flex -o $@ $^

%.o: %.cpp
	g++ -c -o $@ $<

parser: $(OBJS)
	g++ -o $@ $(OBJS)

