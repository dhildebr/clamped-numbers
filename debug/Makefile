srcdir        := ../src
contribdir    := ../contrib
testdir       := ../test
mainobjdir    := ./obj/src
contribobjdir := ./obj/contrib
testobjdir    := ./obj/test
execdir       := .

GCC := g++
GCCINCLUDE := -I $(srcdir) -I $(contribdir) -I $(testdir)
GCCFLAGS := -std=gnu++14 -g -O0 -Wall -Wextra $(GCCINCLUDE)

CPPHEAD := $(srcdir)/clamped_numbers.hh \
           $(contribdir)/gtest/gtest.h
CPPSRC  := $(srcdir)/clamped_numbers.cc \
           $(contribdir)/gtest/gtest-all.cc \
           $(testdir)/clamped_numbers_test.cc $(testdir)/all_tests.cc
CPPOBJ  := $(mainobjdir)/clamped_numbers.o \
           $(contribobjdir)/gtest/gtest-all.o \
           $(testdir)/clamped_numbers_test.o $(testdir)/all_tests.o
EXECBIN := $(execdir)/ClampedNumbersTest.exe

# Primary all-target just aliases building of the executable
all: $(EXECBIN)
	@ echo 'Built target all.'

# Link object files into final unit test executable
$(EXECBIN): $(CPPOBJ)
	$(GCC) $(GCCFLAGS) -o $@ $(CPPOBJ)

# Compile main source files
$(mainobjdir)/%.o: $(srcdir)/%.cc
	@ mkdir -pv $(mainobjdir)
	$(GCC) $(GCCFLAGS) -c $< -o $@

# Compile Google Test files
$(contribobjdir)/gtest/%.o: $(contribdir)/gtest/%.cc
	@ mkdir -pv $(contribobjdir)/gtest
	$(GCC) $(GCCFLAGS) -c $< -o $@

# Compile unit tests
$(testobjdir)/%.o: $(testdir)/%.cc
	@ mkdir -pv $(testobjdir)
	$(GCC) $(GCCFLAGS) -c $< -o $@

# Remove all object files and executable
clean:
	- rm $(CPPOBJ) $(EXECBIN)

.PHONY: all clean spotless
