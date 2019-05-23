cc = gcc
prom = libSolo.so
deps = ./src/solo.h
obj =	./src/util.o \
		./src/sign.o \
		./src/wrapper.o \
		./src/solo.o \
		./src/cJSON/cJSON.o
so = ./lib/libEWallet.so

all: $(prom)


$(prom): $(obj)
	$(cc) -shared  $(obj) $(so) -o $(prom) 


%.o: %.c $(deps)
	$(cc) -g -c -fPIC $< -o  $@

clean:
	rm -rf $(obj) $(prom)
