PROG		= simulation
ifeq ($(MACHINE),s9)
	LDFLAGS		+= -lm -lstdc++ -llua -L/home/khairulinme/nsci/packages/lua5.1/lua-5.1.5/lib -ldl
	CPPFLAGS 	+= -std=c++0x -Wall -Werror -Iinclude -I/home/khairulinme/nsci/packages/lua5.1/lua-5.1.5
else
	LDFLAGS		+= -lm -lstdc++ -llua5.1
	CPPFLAGS	+= -std=c++0x -Wall -Werror -Iinclude -I/usr/include
endif

OBJFILES 	= simulation.o fft.o point.o luautils.o progressbar.o model.o

all: $(PROG)


$(PROG): $(OBJFILES)
	$(CC) $(OBJFILES) $(LDFLAGS) -o $@

simulation.o: simulation.cpp

fft.o: fft.cpp

point.o: point.cpp include/point.h

luautils.o: luautils.cpp include/luautils.h

progressbar.o: progressbar.cpp include/progressbar.h

model.o: model.cpp include/model.h

clean:
	rm -fv $(PROG) *.o

clena: clean

plot_1: plot1.gnuplot cluster.log
	gnuplot $<

run: $(PROG)
	time ./$<

pack: cluster-*.log run.log
	tar cvjf output/cluster-`date +%b%d-%H%M`.tar.bz2 cluster-*.log run.log
	rm -fv cluster-*.log run.log

clean_logs:
	rm -fv cluster-*.log

