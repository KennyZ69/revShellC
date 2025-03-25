# comp flags
FLAGS = -Wall -Wextra -02

SRC = $(wildcard *.c)
HEAD = headers/encrypt.h headers/util.h

# the executable
EXE = rsc

OUTDIR = out

OBJ = $(patsubst %.c,$(OUTDIR)/%.o,$(SRC))

all: $(OUTDIR) $(OUTDIR)/$(EXE)

# create the out dir
$(OUTDIR):
	mkdir -p $(OUTDIR)

# link exe
# $(EXE): $(OBJ)
# 	gcc $(OBJ) -o $(EXE)

# compile c files to o files in the out dir
$(OUTDIR)/%.o: %.c $(HEADERS) | $(OUTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/$(EXE): $(OBJ)
	gcc $(OBJ) -o $(OUTDIR)/$(EXE)

clean: 
	rm -rf $(OUTDIR)

run:
	$(OUTDIR)/$(EXE)

.PHONY: all clean run
