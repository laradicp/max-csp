#detecta se o sistema é de 32 ou 64 bits
N_BITS = $(shell getconf LONG_BIT)
ifeq ($(N_BITS),32)
   SYSTEM  = x86_sles10_4.1
   BITS_OPTION = -m32
else
   #SYSTEM  = x86-64_sles10_4.1
   SYSTEM = x86-64_linux
   BITS_OPTION = -m64
endif


LIBFORMAT = static_pic

####diretorios com as libs do cplex
CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio2211/cplex
CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio2211/concert
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

####diretorios com as libs do gurobi
#GUROBIDIR =/opt/gurobi550/linux64
#GUROBIINC =$(GUROBIDIR)/include/
#GUROBILIB =$(GUROBIDIR)/lib/

#### define o compilador
CPPC = g++ -O3 -std=c++11
#############################

#### opcoes de compilacao e includes
CCOPT = $(BITS_OPTION) -g -fPIC -fexceptions -Wall -DNDEBUG -DIL_STD -std=c++11 -fpermissive
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) #-I$(GUROBIINC)
#############################

#### flags do linker
#CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -L$(GUROBILIB) -lgurobi_c++ -lgurobi55 -lgmpxx -lgmp -lm -lpthread -ldl        #Without lgmpxx  
#CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -L$(GUROBILIB) -lgurobi_c++ -lgurobi55 -lm -lpthread -ldl       #Without lgmpxx        
CCLNFLAGS = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) -lilocplex -lcplex -lconcert -lm -lpthread -ldl
             # -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert                                                        #Without Gurobi and lgmpxx
#############################

#### diretorios com os source files e com os objs files
SRCDIR = src
OBJDIR = obj
#############################

#### lista de todos os srcs e todos os objs
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
#############################

#### regra principal, gera o executavel
instance-generator.exe: $(OBJS)
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CPPC) $(BITS_OPTION) $(OBJS) -o $@ $(CCLNFLAGS)
############################

all: instance-generator.exe

#inclui os arquivos de dependencias
-include $(OBJS:.o=.d)

#regra para cada arquivo objeto: compila e gera o arquivo de dependencias do arquivo objeto
#cada arquivo objeto depende do .c e dos headers (informacao dos header esta no arquivo de dependencias gerado pelo compiler)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CPPC) $(CCFLAGS) -c $< -o $@
#	@echo  "\033[32m \ncreating $< dependency file: \033[0m"
	$(CPPC)  -MM $< > $(basename $@).d
	@mv -f $(basename $@).d $(basename $@).d.tmp #proximas tres linhas colocam o diretorio no arquivo de dependencias (g++ nao coloca, surprisingly!)
	@sed -e 's|.*:|$(basename $@).o:|' < $(basename $@).d.tmp > $(basename $@).d
	@rm -f $(basename $@).d.tmp

#delete objetos e arquivos de dependencia
clean:
	@echo "\033[31mcleaning obj directory \033[0m"
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d instance-generator.exe
rebuild: clean instance-generator.exe

