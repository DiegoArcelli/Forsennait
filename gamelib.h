enum Tipo_pericolo {nessun_pericolo, trappola, alieno};
enum Stato_giocatore {vulnerabile, scudo_vita, solo_vita, solo_scudo};
enum Tipo_oggetto {nessun_oggetto, medikit, pozione, materiale, colpi_lanciarazzi};

struct Giocatore {
  enum Stato_giocatore stato;
  int x,y;
  int oggetti[4];
  int nemiciUccisi;
};

struct Cella {
  enum Tipo_pericolo pericolo;
  enum Tipo_oggetto oggetto;
};

struct Piano {
  unsigned short int piano;
  struct Piano *prossimoPiano;
  short int vita;
};

void gioca();
void crea_mappa();
void crea_scacchiera(int p_pericolo[], int p_oggetto[]);
void stampa_scacchiera();
void termina_creazione();
void muovi(struct Giocatore *g);
void dealloca_mappa();
void termina_gioco();
void combatti();
int usa_oggetto(struct Giocatore *g);
void print_file(char name[]);
void oggetto_ottenuto(enum Tipo_oggetto o);
void fine_parita(unsigned short int vincitore);
unsigned short int controlla_giocatore(struct Giocatore *g);
void combattimento_finale();
struct Piano* crea_torre(struct Piano *p, int i);
void stampa_torre(struct Piano *p);
struct Piano* attacco_torre(struct Giocatore *g, struct Piano* p, int *vita);
void print_menu();
void dimezza_scacchiera();
