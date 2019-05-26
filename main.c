#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gamelib.h"

int main(){

	int scelta;
	int esc = 0;
	printf("Si consiglia di passare alla modalita' schermo intero per evitare anomalie grafiche\n");
	sleep(3);
	printf("\033[H\033[J");
	printf("Due giocatori Ninja e Ciccio si sfidano a Forsennait\n");
	sleep(3);
	printf("un gioco dove il territorio è pieno di insidie e si restringe dopo un certo numero di turni.\n");
	sleep(3);
	printf("Se sopravviveranno, si dovranno scontrare uno contro uno.\n");
	sleep(3);
	printf("\033[H\033[J");
	print_menu();
	do {
		printf("\x1b[0;32m" "\n\t\t\t\t  Insescisci scelta: ");
		scanf("%d",&scelta);
		switch (scelta) {
			case 1:
				printf("\033[H\033[J");
				print_menu();
				crea_mappa();
				break;
			case 2:
				gioca();
				break;
			case 3:
				termina_gioco();
				esc = 1;
				break;
			default:
				printf("\033[H\033[J");
				printf("\x1b[1;31m");
				printf("\n\t\t\t\t  Errore inserire i valori validi\n");
				print_menu();
				break;
		}
	} while(esc==0);

	return 0;
}
