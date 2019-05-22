#define PAEW_SIG_MAX_LEN 0x80
#define PAEW_TX_MAX_LEN 1024

char* sign_tx(int coin, char* tx);
char* sign_utxo(int coin, char* tx, char* utxo);

