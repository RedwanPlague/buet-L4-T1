cc = g++
flags = -std=c++17
linkers = -lglut -lGLU -lGL

run: r.out
	./r.out

r.out: main.cpp *.h
	$(cc) $(flags) main.cpp -o r.out $(linkers)

debug: d.out
	gdb d.out

d.out: main.cpp *.h
	$(cc) $(flags) -g main.cpp -o d.out $(linkers)

clean:
	rm r.out d.out
