build: FileParser.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

clean:
	rm build