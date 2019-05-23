cc = gcc
prom = test
deps = ../src/solo.h
obj = test.o
so = ./libSolo.so

all: $(prom)


$(prom): $(obj)
	$(cc) -g -o $(prom) $(obj) $(so)


%.o: %.c $(deps)
	$(cc) -g -c $< -o  $@

clean:
	rm -rf $(obj) $(prom)
