CXX = g++
CXXFLAGS = -std=c++11 -I$(RUNTIME) -I$(SRC) -lwiringPi -lrt
CPPFLAGS = -MMD -MF $(@:%.o=%.d) -MT '$(@:%.o=%.d) $@' -I$(SRC) -I$(RUNTIME)
LDFLAGS = -lpthread
RM = rm -rf
TARGET = dznpi
VERSION = 2.3.2
CURRENT := $(shell dzn query | grep '*' | sed 's,\* ,,')

ifneq ($(VERSION),$(CURRENT))
$(info current version: $(CURRENT) is not equal to selected version: $(VERSION))
endif

SRC = ./src
RUNTIME = ./lib

SRCS = $(wildcard $(SRC)/*.cc)
SRCS += $(wildcard $(RUNTIME)/*.cc)

OBJS = $(subst .cc,.o,$(SRCS))

all:
	make $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

runtime: | $(RUNTIME)/dzn
	for f in $(shell dzn ls -R /share/runtime/c++ | sed 's,/c++/,,' | tail -n +3); do \
	dzn cat --version=$(VERSION) /share/runtime/c++/$$f > $(RUNTIME)/$$f; \
	done
	touch $@

$(RUNTIME)/dzn:
	mkdir -p $@

generate: $(wildcard *.dzn)
	for f in $^; do dzn -v code --version=$(VERSION) -l c++ -o $(SRC) --depends=.d $$f; done
	touch $@

clean:
	$(RM) $(filter-out runtime.o pump.o, $(OBJS))
	$(RM) $(TARGET)

clean_generated:
	$(RM) `grep -h dzn $(SRC)/*.dzn.d | sed -e 's,:.*,,' -e 's,%,.,g'`
	$(RM) $(RUNTIME)
	$(RM) runtime generate

distclean: clean clean_generated
	$(RM) *~

-include $(wildcard $(SRC)/*.d $(RUNTIME)/*.d)
