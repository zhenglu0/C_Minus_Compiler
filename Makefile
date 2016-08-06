##############################################################
# Compiler and flags to use
##############################################################

CXX       = g++
CXXFLAGS  = -g -Wall 
#-Werror

##############################################################
# You shouldn't have to change anything below this line.
##############################################################

C_MINUS  = main
C_MINUS_OBJS = main.o grammar.o lexical.o utility.o

###

$(C_MINUS): $(C_MINUS_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(C_MINUS_OBJS)

clean:
	rm -f $(C_MINUS) $(C_MINUS_OBJS)


###

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

###		
			
grammar.o: grammar.cpp grammar.h lexical.h utility.h globalVar.h

lexical.o: lexical.cpp lexical.h utility.h globalVar.h

utility.o: utility.cpp utility.h globalVar.h

main.o:  main.cpp grammar.h lexical.h utility.h globalVar.h
