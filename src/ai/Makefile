CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -IC:/msys64/ucrt64/include/SDL2/
LIBS = -lSDL2
SRCS = Snake.c
OBJS = $(SRCS:.c=.o)
MAIN = Snake

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)


.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


clean:
	$(RM) *.o *~ $(MAIN)
