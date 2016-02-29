CC = gcc
CFLAG = -Wall -Wextra -02
<<<<<<< HEAD
INCLUDES = -I/usr/local/include/opencv
LDFLAGS = -pthread -lrt -lwiringPi -ldl -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lm
SRCS =	./main.c ./make_new_thread.c \
	./server/thread_server.c ./server/server.c ./server/command_analysis.c \
	./motor/motor.c ./motor/thread_motor.c \
	./camera/camera.c ./camera/thread_camera.c
OBJS = $(SRCS:.c=.o)
#OUTDIR = ./bin
TARGET = ./RPiTank

.c.o:
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
=======
LDFLAGS = -pthread -lrt -lwiringPi
SRCS = ./test.c ./make_new_thread.c ./server/thread_server.c ./server/server.c ./server/command_analysis.c ./motor/motor.c ./motor/thread_motor.c
OBJS = $(SRCS:.c=.o)
TARGET = ./test

.c.o:
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
>>>>>>> hiraoka/master

default: all

$(TARGET): $(OBJS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

all: $(TARGET)

clean:
	$(RM) *.o
	$(RM) ./motor/*.o
	$(RM) ./server/*.o
<<<<<<< HEAD
	$(RM) ./camera/*.o
=======
>>>>>>> hiraoka/master
	$(RM) $(TARGET)