CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -O3 -march=native -fPIC



dataFile/dataFile.o: dataFile/dataFile.hpp dataFile/dataFile.cpp
	${CXX} ${CXXFLAGS} -c dataFile/dataFile.cpp -o dataFile/dataFile.o

dataFile/dataFile.hpp: dataFile/template.hpp scripts/cppDataFileGen.py data/BaseData.json data/BaseData.json data/EffectData.json data/ItemData.json scripts/pyLib.py extrapolatedData/mixData.json
	PYTHONPATH=. python scripts/cppDataFileGen.py

extrapolatedData/mixData.json: data/EffectData.json data/ItemData.json 
	PYTHONPATH=. python scripts/extrapolateData.py

build: dataFile/dataFile.o


test:
	which python && pwd