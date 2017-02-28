CXX = g++
CXXFLAGS = -std=c++11 -Ilib -lwiringPi -include src/Handwritten/external_types.hh
LDFLAGS = -lpthread
RM = rm -f
TARGET = dznpi

SRCS = $(wildcard src/Handwritten/*.cc)
SRCS += $(wildcard src/Generated/*.cc)
SRCS += $(wildcard lib/*.cc)

OBJS = $(subst .cc,.o,$(SRCS))

all: dznpi

dznpi: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)

distclean: clean
	$(RM) *~ .depend

include .depend
