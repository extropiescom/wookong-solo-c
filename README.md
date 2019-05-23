## GCC COMMAND
```
gcc -g -o test  ./src/solo.c ./src/cJSON/cJSON.c ./src/util.c ./src/sign.c ./src/wrapper.c test.c ./src/core/dll/libEWallet.so -L. -Wl,-rpath=.^C
```
or just
```
make
```
## DEBUG using root
launch you vscode using flowing cmd
```
sudo code --user-data-dir="~/.vscode-root"
```
## HOW TO FIND THE .so
```
export LD_LIBRARY_PATH=/home/wookong-solo-c/test/:$LD_LIBRARY_PATH
```
export LD_LIBRARY_PATH=/home/chester/wookong-solo-rust/src/:$LD_LIBRARY_PATH