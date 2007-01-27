#
# Matt McCutchen's Big Integer Library
# http://hashproduct.metaesthetics.net/bigint/
#

# The implicit rules we need
%.tag : %
	touch $@
%.o : %.cc
	g++ -c -O -Wall -Wextra -pedantic $<

# Default target
all : library sample

library : BigUnsigned.o BigInteger.o BigUnsignedInABase.o BigIntegerUtils.o

# Extra dependencies from `#include'
BigUnsigned.hh.tag : NumberlikeArray.hh.tag
BigUnsigned.o : BigUnsigned.hh.tag
BigInteger.hh.tag : BigUnsigned.hh.tag
BigInteger.o : BigInteger.hh.tag
BigUnsignedInABase.hh.tag : BigUnsigned.hh.tag
BigUnsignedInABase.o : BigUnsignedInABase.hh.tag
BigIntegerUtils.hh.tag : BigInteger.hh.tag
BigIntegerUtils.o : BigIntegerUtils.hh.tag

# sample program
sample : library sample.cc
	g++ -osample *.o sample.cc

clean :
	rm -f *.tag *.o sample

# The `.tag' mechanism allows for proper recompilation when a header file
# changes, considering that some header files may include others.
#
# If a header file X includes other header
# files Y and Z, we create a file X.tag that depends
# on X, Y.tag, and Z.tag.  Other headers that include X should
# depend on X.tag.
#
# Suppose Y is subsequently changed.  X doesn't get ``recompiled'',
# but anything that includes X should be recompiled.  Well, Y.tag becomes
# out-of-date, so X.tag becomes out-of-date, so anything depending on X.tag
# (that is, anything including X) becomes out-of-date.  Magic!
#
# In this system, the tag files are empty files used only for their
# timestamps.  If one wished to use precompiled headers, one could use
# ``.gch'' files exactly how ``.tag'' files are used now, except that
# their contents would be meaningful.
#
# However, the same sort of ``deadly diamond'' problem that surfaces with
# multiple inheritance also applies to precompiled headers.  The ``#ifndef''
# mechanism that prevents multiple inclusion doesn't work when headers are
# compiled independently in a hierarchical structure, since the second
# inclusion of a file won't even know there was a first inclusion.  For
# this reason, I just use ``.tag''s.
