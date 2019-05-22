cc = gcc
prom = test
deps = src/solo.h
obj =	./src/util.o \
		./src/sign.o \
		./src/wrapper.o \
		./src/solo.o \
		./src/cJSON/cJSON.o \
		test.o
so = ./src/core/dll/libEWallet.so

all: $(prom)


$(prom): $(obj)
	$(cc) -g -o $(prom) $(obj) $(so) -L. -Wl,-rpath=.^C


%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)
