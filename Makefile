run:par1 par2 rm

par1:travellingSalesPerson.cpp
	g++ travellingSalesPerson.cpp -lGL -lglut -lGLU

par2:a.out 
	./a.out

rm:a.out
	rm a.out



