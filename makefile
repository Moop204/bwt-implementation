default: decode encode search

search: src/search.cpp src/bwt.cpp src/bwt.h
	g++ -o out/bwtsearch src/search.cpp src/bwt.cpp src/bwt.h

decode: src/decode.cpp src/bwt.cpp src/bwt.h 
	g++ -o out/bwtdecode src/decode.cpp src/bwt.cpp src/bwt.h

encode: src/encoder.cpp 
	g++ -o out/encode src/encoder.cpp

