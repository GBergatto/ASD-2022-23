# 2021-02-16 Conferenza

## Strutture dati

### Articolo

Quasi ADT

Struct contenente i campi utili a rapprensentare un articolo: titolo, relatore, slot e argomento

### Articoli

Tabella di simboli implementata come vettore ordinato.
Permette la ricerca di un `ARTICOLO` dato il titolo.

Inoltre, permette anche la ricerca inversa `searchByIndex` a partire dall'indice del vettore. Questo viene utilizzato dalla funzione ricorsiva che costruisce il programma ottimale.

### Programma

ADT di 1 classe contenente una matrice di strighe dinamicamente allocata e i paramentri R e S per comodità.

## Problema di verifica

### Formato del file contenente il programma

R righe formate da S stringhe rappresentati gli slot.
Ogni stringa corrisponde al titolo dell'articolo presentato durante quello slot oppure vale `-` per rappresentare uno slot vuoto.

```
titolo1 titolo1 - titolo4
titolo5 titolo6 titolo7 titolo7
...
```

Suppongo che tutti i titoli siano distinti, ovvero che nessun articolo si ripeta all'interno di un programma.

### Verifica di validità del programma

Scorro ogni stanza con riferimento ad uno slot e al precedente.
Alla fine di ogni blocco di slot contenenti lo stesso articolo, controllo che tale blocco fosse delle dimensioni giuste per quell'articolo.

Scorro il programma per slot (per colonne) e aggiungo i nomi degli autori ad un insieme. Se il numero di autori è diverso dal numero di slot non vuoti, slot in stanze diverse sono occupati dallo stesso autore, quindi il programma non è valido.

Durante la prima interazione sul programma, controllo anche che tutti gli articoli del programma siano presenti nella lista di articoli.
Durante la seconda, utilizzo un insieme per verificare che tutti gli articoli della lista siano stati inclusi nel programma.

## Problema di ricerca e ottimizzazione

Modello delle disposizioni semplici.

Uso un vettore di interi per segnare gli articoli inseriti nel programma. La corrispondenza tra indice e `ARTICOLO` è garantita dalla funzione `ARTICOLIsearchByIndex`.

Siccome ogni articolo richiede un determinato numero di slot, controllo che questi siano disponibili e, nel caso, li riempio tutti in un colpo solo. In questo modo, evito un gran numero di programmi che risulterebbero invalidi perché non rispettano il vincolo degli slot per ogni articolo.

La ricorsione termina quando il programma è pieno. Ne controllo la validità con la funzione scritta per il problema di verifica e ne calcolo il punteggio.
