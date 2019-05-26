# Prova finale 2019 Forsennait
Prova finale di programmazione valida per il corso di Programmazione I con Laboratorio (a.a. 2018/2019).

Questo repository deve contenere solo file sorgente (non file eseguibili).


Aggiungere qui sotto il

Nome: Diego

Cognome: Arcelli

Matricola: 311045

Lo scontro finale viene gestito nella seguente maniera:
qualora i due giocatori si incontrino nella medesima casella il combattimento avrà inizio. Ogni giocatore edificherà una torre costituita da un numero di piani pari al materiale raccolto dal giocatore durante la partita (qundi fino ad un massimo di 3 piani).
Una volta iniziato il combattimento i due giocatori si attaccheranno a turni. Inizierà il giocatore con il maggior numero di alieni uccisi. Qualora il numero delle uccisioni sia equivalente, verrà scelto casualmente uno dei due giocatori per iniziare lo scontro.
Ad ogni turno il giocatore potrà scegliere tra due alternative:
- Attaccare con il lanciarazzi, infliggendo all'avversario un danno compreso tra 50 e 100
- Attaccare normalmente, infliggendo un danno compreso tra un massimo di 50 e un minimo pari al numero di alieni uccisi dal giocatore.

Attaccare con il lanciarazzi fa decrementare di un'unità i missili presenti nell'inventario del giocatore. Ogni piano sarà caratterizzato da una statistica vita inizializzata a 100. Il piano verrà distrutto quando tale statistica scenderà sotto lo 0.
Quando la torre verrà totalmente distrutta, il danno verrà inflitto direttamente al giocatore.

Note:
È stata aggiunta una variabile di tipo int alla struct Piano, denominata vita, atta a memorizzare la vita del piano. Quando scende sotto lo 0 il piano viene distrutto.
La mappa si dimezza ogni 10 turni. L'area colorata di rosso rappresenta la parte della mappa all'interno della quale i due giocatori devono recarsi prima che avvenga il ridimensionamento della scacchiera. Chi rimane fuori da tale area viene eliminato, dunque vi è la possibilita che perdano entrambi i giocatori.
Il ridimensionamento della mappa non avviene attraverso la distruzione della scacchiera e la creazione di una nuova, ma l'area della mappa colorata in rosso viene copiata per poi divenire la nuova area di gioco. Così facendo pericoli e oggetti di ogni casella e la posizione dei giocatori all'interno della scacchiera rimangono invariati, senza venire nuovamente rigenerati casualmente.
Ogni cella è costituita da due cifre, la prima (quella a sinistra) rappresenta il tipo di pericolo e l'altra indica il tipo di oggetto contenuto nella cella. I valori per i pericoli sono:
- 0, per nessun pericolo
- 1, per la trappola
- 2 per l'alieno

Per gli oggetti invece i possibili valori sono:
- 0, per nessun oggetto
- 1, per il medikit
- 2, per la pozione
- 3, per il materiale
- 4 per il colpo di lanciarazzi

I vari file di testo presenti, oltre ai file sorgente, contengono delle ASCII art che vengono utilizzate per abbellire la grafica del programma. Il contenuto di tali file viene stampato a schermo mediante un'apposita funzione (print_file) che legge il file che gli viene passato come parametro e lo stampa.

