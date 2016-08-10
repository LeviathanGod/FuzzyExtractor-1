OBJS=main.o minutiae_template.o circle.o code.o fingerprint_reader.o
BIN=fuzzy_extractor
LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lfjfx -lthimble
FLAGS= -ggdb
INCLUDES=-I"include"
LIB_DIR=-L"lib"

$(BIN): $(OBJS)
	g++ $(OBJS) -o $(BIN) $(LIBS) $(LIB_DIR) $(FLAGS)

%.o: %.cpp $(subst .o,.h,$(OBJS))
	g++ -c $< -o $@ $(FLAGS) $(INCLUDES)
