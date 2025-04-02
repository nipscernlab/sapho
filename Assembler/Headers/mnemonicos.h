#define NMNEMAX 999999           // o preco por nao usar C++ com o flex e bison

extern int  m_count;             // num  de  opcodes cadastrados para gerar circuitos dimanicamente
extern char m_name[NMNEMAX][64]; // nome dos opcodes cadastrados

void mne_add(char *mne);         // cadastra opcode