# all:
# 	g++ ElectricalTransport.cc -o ElectricalTransport
# 	g++ PPTaskGenerator.cpp -o PPTaskGenerator
# 	g++ Solstice.cc -o Solstice

# clean:
# 	rm -rf ElectricalTransport PPTaskGenerator Solstice
CXX = g++
INCLUDE_PATH = ~/or-tools_x86_64_Ubuntu-22.04_cpp_v9.11.4210/include/
LIB_PATH = ~/or-tools_x86_64_Ubuntu-22.04_cpp_v9.11.4210/lib/
LIBS = -lortools

#targets
TARGETS = OpticalSolution ElectricalTransport PPTaskGenerator Solstice Convert

#sources
SRCS = OpticalSolution.cc ElectricalTransport.cc PPTaskGenerator.cc Solstice.cc Convert.cc

all: $(TARGETS)

%: %.cc
	$(CXX) -I $(INCLUDE_PATH) $< -L $(LIB_PATH) $(LIBS) -o $@

clean:
	rm -f $(TARGETS)