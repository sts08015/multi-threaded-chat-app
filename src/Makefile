TOPTARGETS = all clean
SUBDIRS = server client

$(TOPTARGETS) : chat.o $(SUBDIRS)
ifeq ($(MAKECMDGOALS),clean)
	@rm -f *.o
endif

chat.o : chat.h chat.c

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY : $(TOPTARGETS) $(SUBDIRS)