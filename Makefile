all: preprocess encode decode
preprocess: preprocess.cc defns.h
	g++ -std=c++14 -Wall -Wextra -pedantic -o preprocess preprocess.cc
encode: encode.cc defns.h
	g++ -std=c++14 -Wall -Wextra -pedantic -o encode encode.cc
deocde: decode.cc defns.h
	g++ -std=c++14 -Wall -Wextra -pedantic -o decode decode.cc
clean:
	rm -f preprocess
	rm -f encode
	rm -f decode
world: clean preprocess encode decode
