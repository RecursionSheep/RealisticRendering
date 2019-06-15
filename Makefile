STD11 = -std=c++11
OPT = -O3
OPENMP = -fopenmp
DEPS = render.h KDTree.h hitPoint.h shape.h bezierSpin.h bezierCurve.h rect.h sphere.h texture.h util.h model.h stb_image.h
OBJ = main.o render.o KDTree.o hitPoint.o bezierSpin.o bezierCurve.o rect.o sphere.o texture.o util.o

main: $(OBJ)
	g++ -o $@ $^ $(STD11) $(OPT) $(OPENMP)

%.o: %.cpp $(DEPS)
	g++ -c -o $@ $< $(STD11) $(OPT) $(OPENMP)

clean:
	del *.o
