CXX      = g++
CXXFLAGS = -Wall -pedantic -g
LIBS     = 

OUT      = exec


exec: main.cpp Lexer/Lexer.cpp Parser/Parser.cpp Parser/Descent.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(OUT) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
