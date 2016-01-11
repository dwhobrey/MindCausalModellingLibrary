## Automake file
PLATOLIBDIR = $(srcdir)/../../Library
PACKAGES = $(srcdir)/../../Packages

# Include Plato's header when compiling this model.
AM_CPPFLAGS = -I$(PLATOLIBDIR)/include

bin_PROGRAMS = primarymindmodel
RECONFIGURESOURCES = primarymindmodel
include $(srcdir)/sources.in

# Link with the plato library.
primarymindmodel_LDADD = $(PLATOLIBDIR)/build/libplato.a $(PACKAGES)/pcre/build/libpcre.a

