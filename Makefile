SRCDIR = src
INCLUDEDIR = includes
OBJDIR = obj

CC = gcc
CFLAGS = -Wall -Wextra -I$(INCLUDEDIR) -pthread -ansi

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DEPS = $(wildcard $(INCLUDEDIR)/*.h)

TARGET = simulador_kernel

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(TARGET)