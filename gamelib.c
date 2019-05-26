#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "gamelib.h"
#define clear() printf("\033[H\033[J")
#define GREEN   "\x1b[1;32m"
#define RESET   "\x1b[0m"
#define CYAN  "\x1b[1;36m"
#define RED "\x1b[1;31m"
#define LGREEN "\x1b[0;32m"

static struct Cella *scacchiera=NULL;
static struct Giocatore Ninja;
static struct Giocatore Ciccio;
static unsigned short int N;
static unsigned int turni;

//funzione che stampa a schermo il contenuto di un file di testo
//verrà utilizzata per stampare ascii art
void print_file(char name[]){
  int c;
  FILE *file;
  file = fopen(name, "r");
  if (file) {
    while ((c = getc(file)) != EOF)
      putchar(c);
    fclose(file);
  }
}

//funzione che stampa la grafica del menù iniziale del gioco
void print_menu(){
  printf(LGREEN);
  print_file("menu");
	printf(RESET);
}

//funzione che crea la scacchiera e la popola risepetto alle probabilità inserite
void crea_scacchiera(int p_pericolo[], int p_oggetto[]){
  scacchiera = malloc(sizeof(struct Cella)*N*N);
  enum Tipo_pericolo p;
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      struct Cella c;
      p = rand() % 100+1;
      if(p<=p_pericolo[0])
        c.pericolo = nessun_oggetto;
      else if(p>p_pericolo[0] && p<=(p_pericolo[1]+p_pericolo[0]))
        c.pericolo = alieno;
      else
        c.pericolo = trappola;
      p = rand() % 100 + 1;
      if(p<=p_oggetto[0])
        c.oggetto = nessun_oggetto;
      else if(p>p_oggetto[0] && p<=(p_oggetto[0]+p_oggetto[1]))
        c.oggetto = medikit;
      else if(p>(p_oggetto[0]+p_oggetto[1]) && p<=(p_oggetto[0]+p_oggetto[1]+p_oggetto[2]))
        c.oggetto = pozione;
      else if(p>(p_oggetto[0]+p_oggetto[1]+p_oggetto[2]) && p<=(p_oggetto[0]+p_oggetto[1]+p_oggetto[2]+p_oggetto[3]))
        c.oggetto = materiale;
      else
        c.oggetto = colpi_lanciarazzi;
      scacchiera[i*N+j]=c;
    }
  }
  //si posizionano ciccio e ninja sulla mappa e si verifica che le coordinate non coincidano
  do {
    Ninja.x = rand() % N;
    Ninja.y = rand() % N;
    Ciccio.x = rand() % N;
    Ciccio.y = rand() % N;
  } while (Ninja.x == Ciccio.x && Ninja.y == Ciccio.y && N>1);
}

//funzione che stampa la scacchiera
void stampa_scacchiera(){
  printf(RESET);
  int l=(N-N/2)/2;
  if(turni!=-1){
    printf("\nTurno %d\n",turni+1);
  }
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      if(j==0 && turni==-1){
        printf("\t\t");
      }
      if(i==Ninja.x && j==Ninja.y)
        printf(CYAN "[Ni]" RESET);
      else if(i==Ciccio.x && j==Ciccio.y)
        printf(GREEN "[Ci]" RESET);
      else if(i>=l && i<=N-l-1 && j>=l && j<=N-l-1){
        printf( RESET "[%d%d]", scacchiera[i*N+j].pericolo, scacchiera[i*N+j].oggetto);
      } else {
        printf( RED "[%d%d]", scacchiera[i*N+j].pericolo, scacchiera[i*N+j].oggetto);
        printf(RESET);
      }
    }
    printf("\n");
  }
}

//funzione che gestisce l'utilizzo degli oggetti da parte dei giocatori
int usa_oggetto(struct Giocatore *g){
  int scelta,ex=0,usato=0;
  clear();
  stampa_scacchiera();
  do{
    if(controlla_giocatore(g)==0){
      printf(CYAN);
      printf("\nTurno di Ninja\n");
    } else if(controlla_giocatore(g)==1){
      printf(GREEN);
      printf("\nTurno di Ciccio\n");
    }
    printf("\n\nInventario:\n Medikit: %d",g->oggetti[0]);
    printf("\n Pozione: %d",g->oggetti[1]);
    printf("\n Materiale: %d",g->oggetti[2]);
    printf("\n Colpi lanciarazzi: %d",g->oggetti[3]);
    printf("\n\nSelezione oggetto da utilizzare:\n1) Medikit\n2) Pozione\n3) Nessuno\n");
    scanf("%d",&scelta);
    switch (scelta) {
      case 1:
        if(g->oggetti[0]>0 && g->stato!=solo_vita && g->stato!=scudo_vita){
          if(g->stato == vulnerabile)
            g->stato = solo_vita;
          else if(g->stato == solo_scudo)
            g->stato = scudo_vita;
          g->oggetti[0]--;
          usato = 1;
          ex=1;
          clear();
          printf("\nMedikit usato\n");
        } else {
          clear();
          printf(RED "\nAttualmente non puoi usare il medikit\n" RESET);
          stampa_scacchiera();
        }
        break;
      case 2:
        if(g->oggetti[1]>0 && g->stato!=solo_scudo && g->stato!=scudo_vita){
          if(g->stato == vulnerabile)
            g->stato = solo_scudo;
          else if(g->stato == solo_vita)
            g->stato = scudo_vita;
          g->oggetti[1]--;
          usato = 1;
          ex=1;
          clear();
          printf("\nPozione usata\n");
        } else {
          clear();
          printf(RED "\nAttualmente non puoi usare la pozione\n" RESET);
          stampa_scacchiera();
        }
        break;
      case 3:
        clear();
        usato=0;
        ex=1;
        break;
      default:
        clear();
        printf(RED "\nInserisci un valore valido\n" RESET);
        stampa_scacchiera();
        break;
    }
    //stampa_scacchiera();
  }while(ex==0);
  return usato;
}

//funzione che crea un nuovo piano della torre
struct Piano* crea_torre(struct Piano *pPiano, int i){
  struct Piano *newPiano = malloc(sizeof(struct Piano));
  newPiano->piano = i;
  newPiano->vita = 100;
  newPiano->prossimoPiano = NULL;
  if(pPiano==NULL){
    pPiano = newPiano;
  } else {
    newPiano->prossimoPiano = pPiano;
    pPiano = newPiano;
  }
  return pPiano;
}

//funzione che stampa a schermo la torre di un gicaotre con la vita di ogni piano
void stampa_torre(struct Piano *pPiano){
  if(pPiano == NULL){
    printf("\n");
  } else {
    struct Piano *p = pPiano;
    do {
      printf(" __________________\n");
      printf("|                  |\n");
      if(p->vita==100)
        printf("|       %d%%       |\n",p->vita);
      else if(p->vita<100 && p->vita>9)
        printf("|       %d %%       |\n",p->vita);
      else if(p->vita<10)
        printf("|        %d%%        |\n",p->vita);
      printf("|__________________|\n");
      p = p->prossimoPiano;
    } while(p!=NULL);
  }
  printf("\n");
}

//funzione che gestisce l'attacco alla torre da parte di un giocatore
struct Piano* attacco_torre(struct Giocatore *g, struct Piano* p, int *vita){
  int scelta=0,prob=0,diff;
  do {
    printf("\nSelezione mossa:\n1) Attacco normale\n2) Lanciarazzi (%d)\n",g->oggetti[3]);
    scanf("%d",&scelta);
    switch (scelta) {
      case 1:
        diff=50-(g->nemiciUccisi)-1;
        prob = rand()%diff+(g->nemiciUccisi);
        if(g->nemiciUccisi>=50)
          prob = 50;
        break;
      case 2:
        if(g->oggetti[3]>0){
          prob = rand()%51+50;
          g->oggetti[3]--;
        } else {
          printf("\nNon hai missili da sparare\n");
          scelta=3;
        }
        break;
      default:
        printf("\nInserisci un valore valido\n");
        break;
    }
  } while (scelta!=2 && scelta!=1);
  printf("\nDanno effettuato: %d\n",prob);
  if(p!=NULL){
    p->vita-=prob;
  } else {
    *vita-=prob;
  }
  if(p!=NULL){
    if(p->vita<=0){
      if(p == NULL){
        printf("\nLa torre non è stata ancora edificata\n");
      } else {
        struct Piano *t = p->prossimoPiano;
        free(p);
        p = t;
      }
    }
  }
  if(p!=NULL)
    printf("\nVita piano %d: %d\n",p->piano,p->vita);
  return p;
}

//funzione che gestisce il combattimento_finale
void combattimento_finale(){
  int vita_ciccio = 100;
  int vita_ninja = 100;
  struct Piano *pCiccio=NULL;
  struct Piano *pNinja=NULL;
  for(int i=1;i<Ninja.oggetti[materiale]+1;i++)
      pNinja = crea_torre(pNinja,i);
  for(int i=1;i<Ciccio.oggetti[materiale]+1;i++)
      pCiccio = crea_torre(pCiccio,i);
  int t=0,v=0;
  if(Ninja.nemiciUccisi>Ciccio.nemiciUccisi)
    t=1;
  else if(Ninja.nemiciUccisi<Ciccio.nemiciUccisi)
    t=0;
  else
    t=rand()%2;
  do {
    clear();
    printf(GREEN);
    if(pCiccio!=NULL)
      printf("\n  Torre di Ciccio\n");
    stampa_torre(pCiccio);
    printf("  Vita di Ciccio: %d\n",vita_ciccio);
    printf(CYAN);
    if(pNinja!=NULL)
      printf("\n  Torre di Ninja\n");
    stampa_torre(pNinja);
    printf("  Vita di Ninja: %d\n",vita_ninja);
    printf(RESET);
    if(t%2==0){
      printf(GREEN "\nTurno di Ciccio\n");
      pNinja=attacco_torre(&Ciccio,pNinja,&vita_ninja);
    } else {
      printf(CYAN "\nTurno di Ninja\n");
      pCiccio=attacco_torre(&Ninja,pCiccio,&vita_ciccio);
    }
    t++;
    if(vita_ninja<=0){
      v=0;
    } else if(vita_ciccio<=0){
      v=1;
    }
  } while (vita_ninja>0 && vita_ciccio>0);
  clear();
  if(v==0)
    printf(CYAN "\n\t\t\t   Ciccio ha sconfitto Ninja\n" RESET);
  else
    printf(GREEN "\n\t\t\t   Ninja ha sconfitto Ciccio\n" RESET);
  fine_parita(v);
}

//funzione che gestisce lo spostamento dei giocatori
void muovi(struct Giocatore *g){
  int scelta,oggetto=0;
  char fight;
  int r;
  if(Ninja.x == Ciccio.x && Ninja.y == Ciccio.y){
    combattimento_finale();
  }
  if(scacchiera!=NULL){
    do{
      stampa_scacchiera();
      if(controlla_giocatore(g)==0){
        printf(CYAN);
        printf("\nTurno di Ninja\n");
      } else if(controlla_giocatore(g)==1){
        printf(GREEN);
        printf("\nTurno di Ciccio\n");
      }
      printf("\nStato: ");
      switch (g->stato) {
        case 0:
          printf("vulnerabile\n");
          break;
        case 1:
          printf("scudo e vita\n");
          break;
        case 2:
          printf("vita\n");
          break;
        case 3:
          printf("scudo\n");
          break;
      }
      printf("\nIniserisci direzione:\n1) Nord\n2) Sud \n3) Est\n4) Ovest\n5) Usa oggetto\n");
      scanf("%d",&scelta);
      int x=g->x, y=g->y;
      switch (scelta) {
        case 1:
          x--;
          break;
        case 2:
          x++;
          break;
        case 3:
          y++;
          break;
        case 4:
          y--;
          break;
        case 5:
          oggetto = usa_oggetto(g);
          break;
        default:
          clear();
          printf( RED "\nInserire un valore valido\n" RESET);
          break;
      }
      if(x<0 || x>N-1 || y<0 || y>N-1){
        clear();
        printf(RED "\nNon uscire dai bordi della mappa\n" RESET);
        scelta=0;
      } else {
        g->x=x;
        g->y=y;
      }
    } while((scelta<1 || scelta > 5) || (oggetto==0 && scelta==5));
    if(Ninja.x == Ciccio.x && Ninja.y == Ciccio.y){
      combattimento_finale();
    } else {
      switch (scacchiera[g->x*N+g->y].pericolo) {
        case nessun_pericolo:
          if(scacchiera[g->x*N+g->y].oggetto>0 && scelta!=5){
            if(g->oggetti[scacchiera[g->x*N+g->y].oggetto-1]<3){
              g->oggetti[scacchiera[g->x*N+g->y].oggetto-1]++;
              clear();
              printf("\nOggetto aggiunto: ");
              oggetto_ottenuto(scacchiera[g->x*N+g->y].oggetto);
              scacchiera[g->x*N+g->y].oggetto = 0;
            } else {
              clear();
              printf("\nInventario pieno per ");
              oggetto_ottenuto(scacchiera[g->x*N+g->y].oggetto);
            }
          } else if(scelta!=5){
            clear();
            printf("\nNon hai ottenuto nulla\n");
          }
          break;
        case trappola:
          clear();
          if(controlla_giocatore(g)==0)
            printf(CYAN "\n\t\t   Ninja ");
          else if(controlla_giocatore(g)==1)
            printf(GREEN "\n\t\t\tCiccio ");
          printf("è stato ucciso da una trappola\n");
          fine_parita(controlla_giocatore(g));
          break;
        case alieno:
          //gestione del combattimnto contro l'alieno
          clear();
          print_file("alien");
          printf("\nVuoi combattere contro l'alieno? (s/n): ");
          scanf(" %c",&fight);
          if(fight=='s'){
            r=rand()%101;
            clear();
            printf("\nAlieno ucciso\n");
            scacchiera[g->x*N+g->y].pericolo = 0;
            g->nemiciUccisi++;
            if(scacchiera[g->x*N+g->y].oggetto>0 && scelta!=5){
              if(g->oggetti[scacchiera[g->x*N+g->y].oggetto-1]<3){
                g->oggetti[scacchiera[g->x*N+g->y].oggetto-1]++;
                printf("\nOggetto aggiunto: ");
                oggetto_ottenuto(scacchiera[g->x*N+g->y].oggetto);
                scacchiera[g->x*N+g->y].oggetto = 0;
              } else {
                printf("\nInventario pieno per ");
                oggetto_ottenuto(scacchiera[g->x*N+g->y].oggetto);
              }
            } else if(scelta!=5){
              printf("\nNon hai ottenuto nulla\n");
            }
            if(r<35 && g->stato==vulnerabile){
              clear();
              printf("\n\t\t\t L'alieno ha ucciso");
              if(controlla_giocatore(g)==0)
                printf(" Ninja\n");
              else if(controlla_giocatore(g)==1)
                printf(" Ciccio\n");
              fine_parita(controlla_giocatore(g));
            } else if(r<35 && r>12){
              if(g->stato==solo_vita){
                printf("\nHai perso la vita\n");
                g->stato=vulnerabile;
              } else if(g->stato==scudo_vita){
                printf("\nHai perso lo scudo\n");
                g->stato=solo_vita;
              }
            } else if(r<=12){
              if(g->stato==solo_scudo){
                printf("\nHai perso lo scudo\n");
                g->stato=vulnerabile;
              } else if(g->stato==scudo_vita){
                printf("\nHai perso la vita\n");
                g->stato=solo_vita;
              }
            }
          } else {
            clear();
            printf("\nScontro evitato\n");
          }
          break;
        default:
          break;
      }
    }
  }
  printf(RESET);
}

//funzione che acquisice la dimensione della mappa e le probabilità di oggetti e pericoli
void crea_mappa(){
  //inizializzazione statistiche giocatori
  Ninja.stato=solo_vita;
  Ciccio.stato=solo_vita;
  Ninja.nemiciUccisi=0;
  Ciccio.nemiciUccisi=0;
  for(int i=0;i<4;i++){
    Ninja.oggetti[i]=0;
    Ciccio.oggetti[i]=0;
  }
  time_t t;
  char conf;
  srand((unsigned) time(&t));
  turni=-1;
  //acquisizione dimensione mappa
  do {
    int tot=0,prob,c=0;
    int p_pericolo[3];
    int p_oggetto[5];
    clear();
    do {
      printf(LGREEN  "\nInserisci dimensione della mappa: ");
      scanf("%hd",&N);
      if(N<=0){
        printf("\nInserisci valori maggiori di 0\n");
      }
    } while(N<=0);
    clear();
    //acquisizione probabilità pericoli
    for(int i=0;i<3;i++)
      p_pericolo[i]=0;
    do {
      printf("\nIniserisci probabilità per");
      switch (c) {
        case 0:
          printf(" nessun pericolo: ");
          break;
        case 1:
          printf(" alieni: ");
          break;
      }
      scanf("%d",&prob);
      if(prob>(100-tot) || prob<0){
        prob=0;
        printf("\nInserisci valori compresi tra 0 e %d\n",100-tot);
      } else {
        tot+=prob;
        if(c==0){
          p_pericolo[0]=prob;
        } else if(c==1){
          p_pericolo[1]=prob;
        }
        c++;
        if(tot==100)
          c=2;
      }
    } while(c<2);
    p_pericolo[2]=100-tot;
    clear();
    tot=0;
    c=0;
    //acquisizione probabilità oggetti
    for(int i=0;i<5;i++)
      p_oggetto[i]=0;
    do {
      printf("\nIniserisci probabilità per");
      switch (c) {
        case 0:
          printf(" nessun oggetto: ");
          break;
        case 1:
          printf(" medikit: ");
          break;
        case 2:
          printf(" pozione: ");
          break;
        case 3:
          printf(" materiale: ");
          break;
      }
      scanf("%d",&prob);
      if(prob>(100-tot) || prob<0){
        prob=0;
        printf("\nInserisci valori compresi tra 0 e %d\n",100-tot);
      } else {
        tot+=prob;
        if(c==0){
          p_oggetto[0]=prob;
        } else if(c==1){
          p_oggetto[1]=prob;
        } else if(c==2){
          p_oggetto[2]=prob;
        } else if(c==3){
          p_oggetto[3]=prob;
        }
        c++;
        if(tot==100)
          c=4;
      }
    } while(c<4);
    p_oggetto[4]=100-tot;
    crea_scacchiera(p_pericolo, p_oggetto);
    clear();
    printf("\n\tResoconto:\n");
    printf("\n\tDimensione mappa: %d\n", N);
    printf("\n\tProbabilità pericoli selezionata:\n");
    printf("\tNessun pericolo   %d%%\n",p_pericolo[0]);
    printf("\tAlieni            %d%%\n",p_pericolo[1]);
    printf("\tTrappola          %d%%\n",p_pericolo[2]);
    printf("\n\tProbabilità oggetti selezionata:\n");
    printf("\tNessun oggetto    %d%%\n",p_oggetto[0]);
    printf("\tMedikit           %d%%\n",p_oggetto[1]);
    printf("\tPozione           %d%%\n",p_oggetto[2]);
    printf("\tMateriale         %d%%\n",p_oggetto[3]);
    printf("\tLanciarazzi       %d%%\n",p_oggetto[4]);
    printf("\n\tProseguire? (s/n): ");
    scanf(" %c",&conf);
  } while (conf!='s');
  printf(RESET);
  clear();
	print_menu();
  stampa_scacchiera();
}

void gioca(){
  if(scacchiera!=NULL){
    clear();
    turni = 0;
    while(N>0){
      if(turni%2==0){
        muovi(&Ninja);
      } else {
        muovi(&Ciccio);
      }
      turni++;
      if(turni%10==0 && N!=0){
        dimezza_scacchiera();
      }
    }
    turni = -1;
  } else {
    clear();
    printf(RED "\n\t\t\t\t  Mappa non creata\n" RESET);
    print_menu();
  }
}

//funzione per dallocare la schacchiera dalla memoria
void dealloca_mappa(){
  if(scacchiera!=NULL){
    free(scacchiera);
    scacchiera=NULL;
  }
}

void oggetto_ottenuto(enum Tipo_oggetto o){
  switch (o) {
    case 1:
      printf("medikit\n");
      break;
    case 2:
      printf("pozione\n");
      break;
    case 3:
      printf("materiale\n");
      break;
    case 4:
      printf("colpo di lanciarazzi\n");
      break;
    default:
      break;
  }
}

//funzione che controllolla se in un determinato turno si sta gestendo ciccio o ninja
unsigned short int controlla_giocatore(struct Giocatore *g){
  if(g->x==Ninja.x && g->y==Ninja.y){
    return 0;
  } else if(g->x==Ciccio.x && g->y==Ciccio.y){
    return 1;
  }
  return 2;
}

//funzione che gestisce l'output dell'esito finale della parita
void fine_parita(unsigned short int vincitore){
  if(vincitore==0){
    printf(CYAN);
    print_file("lapide_ninja");
    sleep(3);
    clear();
    printf(GREEN "\nIL VINCITORE DELLA SFIDA E'\n");
    print_file("ciccio");
    printf("\nUccisoni Ciccio: %d\n",Ciccio.nemiciUccisi);
    printf("\nUccisoni Ninja: %d\n",Ninja.nemiciUccisi);
    printf(RESET);
  } else if(vincitore==1){
    printf(GREEN);
    print_file("lapide_ciccio");
    sleep(3);
    clear();
    printf(CYAN "\nIL VINCITORE DELLA SFIDA E'\n");
    print_file("ninja");
    printf("\nUccisoni Ninja: %d\n",Ninja.nemiciUccisi);
    printf("\nUccisoni Ciccio: %d\n",Ciccio.nemiciUccisi);
    printf(RESET);
  } else if(vincitore==2){
    print_file("lapide_entrambi");
    printf("\nUccisoni Ciccio: %d\n",Ciccio.nemiciUccisi);
    printf("\nUccisoni Ninja: %d\n",Ninja.nemiciUccisi);
  }
  sleep(6);
  dealloca_mappa();
  N=0;
  clear();
  print_menu();
}

//funzione che gestisce la chiusura del gioco
void termina_gioco(){
  if(scacchiera!=NULL){
    dealloca_mappa();
  }
  clear();
  for(int i=3;i>0;i--){
    printf("\nGRAZIE PER AVER GIOCATO\n\n");
    print_file("chiusura");
    printf("\nIl gioco si chiuderà tra %d\n",i);
    sleep(1);
    clear();
  }
  clear();
}

//funzione che dimezza la mappa compiando l'area centrale
void dimezza_scacchiera(){
  struct Cella *new;
  int ninja_morto=0;
  int ciccio_morto=0;
  int l=(N-N/2)/2;
  int N_new = N-l*2;
  //si controlla se ciccio e ninja sono rimasti fuori dall'area che verrà copiata
  if(Ninja.x>=l && Ninja.x<=N-l-1 && Ninja.y>=l && Ninja.y<=N-l-1){
    Ninja.x = Ninja.x-l;
    Ninja.y = Ninja.y-l;
  } else {
    ninja_morto = 1;
  }
  if(Ciccio.x>=l && Ciccio.x<=N-l-1 && Ciccio.y>=l && Ciccio.y<=N-l-1){
    Ciccio.x = Ciccio.x-l;
    Ciccio.y = Ciccio.y-l;
  } else {
    ciccio_morto = 1;
  }
  if(N==2){
    N_new = 1;
    Ninja.x = 0;
    Ninja.y = 0;
    Ciccio.x = 0;
    Ciccio.y = 0;
  }
  new = malloc(sizeof(struct Cella) * N_new * N_new);
  int x=0,y=0;
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      if(i>=l && i<=N-l-1 && j>=l && j<=N-l-1){
        new[x*N_new+y] = scacchiera[i*N+j];
        y++;
      }
    }
    if(i>=l && i<=N-l-1){
      x++;
      y=0;
    }
  }
  N=N_new;
  free(scacchiera);
  scacchiera=new;
  clear();
  if(ninja_morto==1 && ciccio_morto==1){
    printf(RED "\n\t\t\t\t       Entrambi sono rimasti fuori dalla mappa\n\n");
    fine_parita(2);
  } else if(ninja_morto==1){
    printf(CYAN "\n\t\t    Ninja e' rimasto fuori dalla mappa\n\n");
    fine_parita(0);
  } else if(ciccio_morto==1){
    printf(GREEN "\n\t\t    Ciccio e' rimasto fuori dalla mappa\n\n");
    fine_parita(1);
  }
}
