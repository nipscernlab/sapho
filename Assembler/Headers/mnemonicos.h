#define NMNEMAX 999999    // o preco por nao usar C++ com o flex e bison

int  m_count;             // num  de  opcodes cadastrados para gerar circuitos dimanicamente
char m_name[NMNEMAX][64]; // nome dos opcodes cadastrados

void add_mne(char *mne);  // cadastra opcode