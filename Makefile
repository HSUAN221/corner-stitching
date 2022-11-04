# Makefile 

# Nom du compilateur
CC = g++

# Options de compilation: optimisation, debug etc...
OPT = -O3
Linking = 
INC = -I src/


EXE = Lab1

DIR_OBJ = ./obj
DIR_BIN = ./bin


OBJS =  \
    demo_example.o \

# Linking object files
exe :  $(OBJS) 
	$(CC) -o $(EXE) \
    $(OBJS) \
    $(OPT) $(Linking)


# echo something
	@echo "   ***************** successful *****************   "                                                                                      
	@echo "    |  Author:  Luca Wei                            "                       
	@echo "    |  Version: 0.0.1                               "

# Defining the flags of objects
%.o: examples/%.cpp
	@$(CC) $< $(OPT) $(INC) -c 


# Removing object files
clean :
	@/bin/rm -f *.dat
	@/bin/rm -f *.x
	@/bin/rm -f *.q

cleanall : 
	@/bin/rm -f $(OBJS) $(EXE)
	@/bin/rm -f *.dat
	@/bin/rm -f *.x
	@/bin/rm -f *.q
    
config :
	if [ ! -d obj ] ; then mkdir obj ; fi
	if [ ! -d run ] ; then mkdir bin ; fi
