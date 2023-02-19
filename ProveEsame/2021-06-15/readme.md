# 2021-06-15 Magazzino

## Pacco

Quasi ADT contenente il codice del pacco e la terna (corridoio, scaffale e slot) per rappresentarne la posizione


## Scaffale

Quasi ADT

Array di `int` lunghezza fissa `K`. Ogni cella contiene l'indice di un pacco nel vettore ordinato del `Magazzino`

## Corridoio

ADT di 1 classe

Array di `Scaffale` dinamicamente allocato

## Magazzino

ADT di 1 classe

Array di `Corridoio` dinamicamente allocato

Array ordinato di `Pacco` dinamicamente allocato per eseguire ricerca dicotomica (costo logaritmico) dato il codice del pacco e per ricavare la posizione di un pacco dato l'indice memorizzato in ogni slot.


