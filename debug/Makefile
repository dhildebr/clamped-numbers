srcdir :=     ../src
contribdir := ../contrib
testdir :=    ../test

mainobjdir    := ./obj/src
contribobjdir := ./obj/contrib
testobjdir    := ./obj/test

execdir :=    .

GCC := g++
GCCINCLUDE := -I $(srcdir) -I $(contribdir) -I $(testdir)
GCCFLAGS := -std=gnu++14 -g -O0 -Wall -Wextra $(GCCINCLUDE)

MAINSRC    := clamped_numbers.cc
CONTRIBSRC := gtest/gtest-all.cc
TESTSRC    := clamped_numbers_test.cc all_tests.cc

CPPSRC  := $(srcdir)/$(MAINSRC)            $(contribdir)/$(CONTRIBSRC)           $(testdir)/$(TESTSRC)
CPPOBJ  := $(mainobjdir)/$(MAINSRC:.cc=.o) $(contribobjdir)/$(CONTRIBSRC:.cc=.o) $(testobjdir)/$(TESTSRC:.cc=.o)
EXECBIN := $(EXECDIR)/ClampedNumbersTest.exe

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
