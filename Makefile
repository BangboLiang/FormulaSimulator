all:
	g++ ElectricalTransport.cc -o ElectricalTransport
	g++ PPTaskGenerator.cpp -o PPTaskGenerator
	g++ Solstice.cc -o Solstice
clean:
	rm -rf ElectricalTransport PPTaskGenerator Solstice
