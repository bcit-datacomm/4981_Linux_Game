CXXFLAGS := -Wall -pedantic -pipe -std=c++14
CLIBS := -pthread -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
CLIBSSERVER := -pthread -fopenmp
APPNAME := Linux_Game
ODIR := bin
SRC := src
HEAD := $(SRC)
OBJS := $(ODIR)/%.o
SRCOBJS := $(SRC)/%.cpp
SRCWILD := $(wildcard $(SRC)/*.cpp)
HEADWILD := $(wildcard $(HEAD)/*.h)
EXEC := $(ODIR)/$(APPNAME)
DEPS := $(EXEC).d

all: $(patsubst $(SRCOBJS), $(OBJS), $(SRCWILD))
# Command takes all bin .o files and creates an executable called chess in the bin folder
	$(CXX) $^ $(CFLAGS) $(CXXFLAGS) $(CLIBS) -o $(EXEC)

$(ODIR):
	@mkdir -p $(ODIR)

# Create dependency file for make and manually adjust it silently to work with other directories
$(DEPS): $(SRCWILD) $(HEADWILD) | $(ODIR) 
# Compile the non-system dependencies and store it in outputdir/execname.d
	@$(CXX) -MM $(CFLAGS) $(CXXFLAGS) $(SRCWILD) > $(DEPS)
# Copy the contents to a temp file
	@cp $(DEPS) $(DEPS).tmp
# Take the temp file contents, do a regex text replace to change all .o strings into
# outputdir/.o strings, and store the result in outputdir/execname.d
	@sed -e 's/\w\+\.o/$(ODIR)\/&/g' < $(DEPS).tmp > $(DEPS)
# Delete the temp file
	@$(RM) $(DEPS).tmp

# Add the dependencies into make and don't throw an error if it doesn't exist
# Also don't generate dependency file during a clean
ifeq (,$(filter server clean, $(MAKECMDGOALS)))
-include $(DEPS)
endif

ifneq (,$(findstring debug,$(MAKECMDGOALS)))
	CXXFLAGS += -g -pg
else
	CXXFLAGS += -O3 -march=native -flto -DNDEBUG
endif

release: all
debug: all

# Target is any bin .o file, prereq is the equivalent src .cpp file
$(OBJS): $(SRCOBJS)
# Command compiles the src .cpp file with the listed flags and turns it into a bin .o file
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $(patsubst $(SRCOBJS), $(OBJS), $<)

server: rserver

dserver: $(wildcard $(SRC)/server/*.c)
	gcc -Wall -pedantic -pipe -std=c11 -g -pg $(wildcard $(SRC)/server/*.c) $(CLIBSSERVER) -o $(CURDIR)/$(ODIR)/server

rserver: $(wildcard $(SRC)/server/*.c)
	gcc -Wall -pedantic -pipe -std=c11 -O3 -march=native -flto -DNDEBUG $(wildcard $(SRC)/server/*.c) $(CLIBSSERVER) -o $(CURDIR)/$(ODIR)/server

# Prevent clean from trying to do anything with a file called clean
.PHONY: clean

# Deletes the executable and all .o and .d files in the bin folder
clean: |$(ODIR)
	$(RM) $(EXEC) $(wildcard $(ODIR)/server*) $(wildcard $(EXEC).*) $(wildcard $(ODIR)/*.d*) $(wildcard $(ODIR)/*.o)

