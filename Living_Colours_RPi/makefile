CXX = g++
CXXFLAGS = -std=c++11 -I$(RUNTIME) -I$(SRC) -lwiringPi -include $(SRC)/external_types.hh
CPPFLAGS = -MMD -MF $(@:%.o=%.d) -MT '$(@:%.o=%.d) $@' -I$(SRC) -I$(RUNTIME)
LDFLAGS = -lpthread
RM = rm -f
TARGET = dznpi

SRC = ./src
RUNTIME = ./lib

SRCS = $(wildcard $(SRC)/*.cc)
SRCS += $(wildcard $(RUNTIME)/*.cc)

OBJS = $(subst .cc,.o,$(SRCS))

all: generate
	make $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

generate: $(wildcard *.dzn)
	for f in $^; do dzn -v code -l c++ -o $(SRC) --depends=.d $$f; done
	touch $@

clean:
	$(RM) $(filter-out runtime.o pump.o, $(OBJS))
	$(RM) $(TARGET)

distclean: clean
	$(RM) *~

-include $(wildcard $(SRC)/*.d $(RUNTIME)/*.d)
