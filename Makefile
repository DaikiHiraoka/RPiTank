CC = gcc
#CC = g++
CAM = camera/
CFLAG = -Wall -Wextra -02
INCLUDES = -I /usr/local/include/opencv
LDFLAGS = -pthread -lrt -lwiringPi -ldl -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lm
SRCS =	./main.c ./make_new_thread.c \
	./server/thread_server.c ./server/server.c ./server/command_analysis.c \
	./motor/motor.c ./motor/thread_motor.c

OBJS = $(SRCS:.c=.o)
CAMOBJS = $(CAM)camera.o $(CAM)thread_camera.o
#OUTDIR = ./bin
TARGET = ./RPiTank

#include $(CAM)Makefile

.c.o:
	@echo "Compiling $<..."
	cd $(CAM); make
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

default: all

$(TARGET): $(OBJS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(CAMOBJS) $(LDFLAGS)

all: $(TARGET)

clean:
	$(RM) *.o
	$(RM) ./motor/*.o
	$(RM) ./server/*.o
	$(RM) ./camera/*.o
	$(RM) $(TARGET)