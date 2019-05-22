## GCC COMMAND
```
gcc -g -o test  ./src/solo.c ./src/cJSON/cJSON.c ./src/util.c ./src/sign.c ./src/wrapper.c test.c ./src/core/dll/libEWallet.so -L. -Wl,-rpath=.^C
```
## DEBUG using root
launch you vscode using flowing cmd
```
sudo code --user-data-dir="~/.vscode-root"
```
## HOW TO FIND THE .so
```
export LD_LIBRARY_PATH=/home/wookong-solo-c/src/core/dll/:$LD_LIBRARY_PATH
sudo -s
```