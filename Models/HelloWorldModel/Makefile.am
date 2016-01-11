## Automake file
PLATOLIBDIR = $(srcdir)/../../Library
PACKAGES = $(srcdir)/../../Packages

# Include Plato's header when compiling this model.
AM_CPPFLAGS = -I$(PLATOLIBDIR)/include

bin_PROGRAMS = helloworldmodel
helloworldmodel_SOURCES = HelloWorldModel.cpp

# Link with the plato library.
helloworldmodel_LDADD = $(PLATOLIBDIR)/build/libplato.a $(PACKAGES)/pcre/build/libpcre.a

