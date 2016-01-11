## Automake file
PLATOLIBDIR = $(srcdir)/../../Library
PACKAGES = $(srcdir)/../../Packages

# Include Plato's header when compiling this model.
AM_CPPFLAGS = -I$(PLATOLIBDIR)/include

bin_PROGRAMS = unittestmodel
unittestmodel_SOURCES = UnitTestModel.cpp

# Link with the plato library.
unittestmodel_LDADD = $(PLATOLIBDIR)/build/libplato.a $(PACKAGES)/pcre/build/libpcre.a

