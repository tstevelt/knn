PROG = knn
LIB = knn.a
XLIB = -lm
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)

FILES = \
	$(LIB)(knn.o)			\
	$(LIB)(shslib.o)		\
	$(LIB)(getargs.o)		\
	$(LIB)(AppendTest.o)	\
	$(LIB)(AppendTrain.o)	\
	$(LIB)(calc_dist.o)		\
	$(LIB)(count_lines.o)	\
	$(LIB)(test.o)			\
	$(LIB)(normalize.o)		\
	$(LIB)(predict.o)		\
	$(LIB)(check_and_save.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): knn.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

