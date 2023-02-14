## Strutture dati

### ADT grafo

Vedi `graph.h` e `graph.c`

Matrice delle adiacenze
- 0 se il cunicolo non ha trappole
- 1 se il cunicolo ha trappole
- -1 se il cunicolo non esiste

Tabella di simboli (vedi `st.h`)
Corrispondenza tra Stanza e indice (int) nella matrice delle adiacenze

### Struct Stanza

Dichiarata in `st.h`
Tipo composto contenente le informazioni riguardati una stanza

### ADT Path

ADT di 1 classe utilizzato per memorizzare un cammino nel labirinto (vedi `path.h`)

Implementato come ADT perché la funzione `GRAPHpathCheck` richiede di salvare la ricchezza massima accumulabile, quindi ho bisogno che il cammino sia passato by reference. Siccome il prototipo di `GRAPHpathCheck` non prevede puntatori, ho incluso il puntatore nella definizione del tipo `Path`.

La sequenza di stanze è memorizzata come vettore dinamico di interi che rappresentano l'indice di ogni stanza (vertice) all'interno del grafo.

### Lettura del grafo

Il file che contiene il grafo è in formato standard: S vertici seguiti da un numero indefinito di archi.
Ho modificato la funzione standard di lettura per acquisire le informazioni extra sulle stanze (vertici).
La presenza di trappole lungo i cunicoli è registrata come peso dell'arco.

## Problema di verifica

### Lettura del percorso

Scorro la sequenza di nomi delle stanze con un while
Ottengo l'indice corrispondente tramite ST
Inserisco nel vettore contenuto in Path

### Verifica di validità del percorso

Scorro la sequenza di indici
Controllo che esista l'arco tra ogni vertice e il precedente nella sequenza
Se i contatori si azzerano o ritorno all'entrata, concludo il cammino di conseguenza.

Per raccogliere la ricchezza complessiva massima, prendo il tesoro nella stanza in cui il delta tra il valore del tesoro e quello delle monete è massimo. In tutte le altre stanze prendo le monete.

## Problema di ricerca

Principio di moltiplicazione
Ad ogni stanza, discendo nei cunicoli uscenti

Non ho messo controlli che evitino i cicli perché il testo prevede che l'entrata e l'uscita coincidano, ovvero che esista almeno un ciclo.
Girare in tondo tra un gruppo di stanze le cui ricchezze sono già state prelevate è chiaramente inutile, ma non ho trovato strategie di pruning semplici per poterlo evitare.
Entrare in una stanze e tornare indietro da dove si è venuti non è da escludere in quanto potrebbe permettere di raccogliere un tesoro non raggiungibile in altro modo.

Il prototipo della funzione ricorsiva prevede return di tipo `Path`, che suppongo serva per restituire la soluzione migliore.

```c
Path GRAPHpathBestR(Graph g, int M, int PF, ...);
```

Siccome il tipo Path (per come è definito) è passato alla funzione by reference, ho utilizzato l'argomento `bestSol` per salvare il miglior percorso trovato, seguendo il modello visto a lezione.
