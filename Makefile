OBJS=main.o minutiae_template.o circle.o code.o
BIN=fuzzy_extractor
# LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc
FLAGS= -O3 # -ggdb

$(BIN): $(OBJS)
	g++ $(OBJS) -o $(BIN) $(LIBS) $(FLAGS)

%.o: %.cpp $(subst .o,.h,$(OBJS))
	g++ -c $< -o $@ $(FLAGS)
