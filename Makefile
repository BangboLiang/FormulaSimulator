all:
	g++ ElectricalTransport.cc -o ElectricalTransport
	g++ PPTaskGenerator.cpp -o PPTaskGenerator

clean:
	rm ElectricalTransport PPTaskGenerator