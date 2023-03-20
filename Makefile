CXX      = g++
CXXFLAGS = -Wall -pedantic -g
LIBS     = 

TARGET   = mila
BUILDDIR = build

SRCS := $(shell find compiler/ -name '*.cpp')


all: $(TARGET)

$(TARGET): $(SRCS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILDDIR)/$@ $(LIBS)

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
