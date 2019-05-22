gcc -g -o test  ./src/solo.c ./src/cJSON/cJSON.c ./src/util.c ./src/wrapper.c test.c ./src/core/dll/libEWallet.so -L. -Wl,-rpath=.^C
export LD_LIBRARY_PATH=/home/wookong-solo-lib-c/src/core/dll/:$LD_LIBRARY_PATH
sudo -s