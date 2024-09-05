# Prova Finale di Algoritmi e Strutture Dati 2022-2023
La Prova Finale di Algoritmi e Strutture Dati richiede di applicare le tecniche apprese nel corso di Algoritmi e Principi dell'Informatica per sviluppare un programma in linguaggio C.
## Descrizione
Il programma gestisce operazioni su un'ipotetica autostrada composta da stazioni di servizio. Ogni stazione è caratterizzata da una distanza dall'inizio dell'autostrada e da un parco veicoli con autonomie specifiche. Il programma riceve una serie di comandi per aggiungere o rimuovere stazioni e veicoli, e per pianificare percorsi ottimali tra le stazioni.  
L'obiettivo è calcolare il percorso che minimizza il numero di tappe tra una stazione di partenza e una di arrivo, rispettando condizioni come la possibilità di tornare indietro e il cambio obbligatorio del veicolo a ogni tappa.  
[Specifiche](specifiche/SpecificheProgetto_2022-2023.pdf)  
[Implementazione](specifiche/ImplementazioneProgetto_2022-2023.pdf)  
## Installazione e Utilizzo
Scarica il repository dal tuo terminale:  
```bash
git clone https://github.com/filo3r/api-2023-raimondi-filippo.git
```
Accedi alla directory del progetto:  
```bash  
cd api-2023-raimondi-filippo
```
Compila il progetto:  
```bash  
make
```
Esegui il programma passando un file di input, sostituisci `percorso/input.txt` con il file che desideri utilizzare:  
```bash  
./bin/api-2023-raimondi-filippo < percorso/input.txt
```
Elimina i file compilati:  
```bash
make clean
```
## File di Input
Il file di input contiene una sequenza di comandi, uno per riga, con il seguente formato:  
`aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n`  
`demolisci-stazione distanza`  
`aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere`  
`rottama-auto distanza-stazione autonomia-auto-da-rottamare`  
`pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo`  
  
Alcuni file di input per testare il programma sono disponibili nella cartella `test`.  
Puoi utilizzare questi file per eseguire il programma e verificarne il funzionamento.
