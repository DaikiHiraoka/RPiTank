CC = g++
CAM = 1
CFLAG = -Wall -Wextra -02
INCLUDES = -I/usr/local/include/opencv
LDFLAGS = -pthread -lrt -lwiringPi -ldl -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lm
SRCS =	./camera.cpp ./thread_camera.cpp

OBJS = 
#OUTDIR = ./bin
#TARGET = ./RPiTank

.c:
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c  $(INCLUDES)

default: all

#$(TARGET): $(OBJS)
#	@echo "Compiling $<..."
#	$(CC) $(CFLAGS) -c $(OBJS) $(LDFLAGS)

all: $(SRCS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDES)
	@echo "Obj"
#	OBJS+=$(SRCS:.cpp=.o)

clean:
	$(RM) *.o
