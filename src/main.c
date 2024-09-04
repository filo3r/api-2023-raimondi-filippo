/*

   PROGETTO DI ALGORITMI E STRUTTURE DATI
   Filippo Raimondi
   Politecnico di Milano
   Anno Accademico 2022-2023
   Versione 21

 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_AUTO 512
#define MAX_STAZIONI 60000
#define MAX_INT 2147483647
#define BLANK -1
#define CANC -2

typedef int32_t INT;

// Struttura nodo lista concatenata (gestione stazioni di servizio)
typedef struct STAZIONE {
    INT distanza;
    INT num_auto;
    INT autonomia[2 * MAX_AUTO]; // Tabella hash per gestire il parco auto
    INT max_autonomia;
    struct STAZIONE *next;
} STAZIONE;


// Struttura per l'array delle stazioni
typedef struct {
    INT distanza;
    INT max_autonomia;
} ARRAY_STAZIONE;



/* FUNZIONI AUSILIARIE */
// Funzione per inizializzare la tabella hash con valori non validi
void inizializza_tabella (STAZIONE **nuovo_nodo) {

    for (INT i = 0; i < 2 * MAX_AUTO; i++){
        (*nuovo_nodo)->autonomia[i] = BLANK;
    }
}


// Funzione di hash per calcolare l'indice nella tabella (metodo della divisione)
INT funzione_hash (INT autonomia){

    INT indice = (INT)(autonomia % (2 * MAX_AUTO));
    return indice;
}


// Funzione per convertire una lista concatenata in un array e contare il numero di stazioni
INT lista_in_array (STAZIONE *testa, ARRAY_STAZIONE **stazioni, INT num_stazioni){

    if (testa == NULL){
        return num_stazioni;
    } else {
        (*stazioni)[num_stazioni].distanza = testa->distanza;
        (*stazioni)[num_stazioni].max_autonomia = testa->max_autonomia;
        num_stazioni++;
        return lista_in_array (testa->next, stazioni, num_stazioni);
    }
}


// Funzione per cercare un elemento in un array ordinato (ricerca binaria)
INT ricerca_indice_stazione (ARRAY_STAZIONE *stazioni, INT num_stazioni, INT distanza){

    INT inizio = 0;
    INT medio = 0;
    INT fine = num_stazioni - 1;

    while (inizio <= fine){
        medio = (inizio + fine) / 2;
        if (stazioni[medio].distanza == distanza){
            return medio;
        }
        if (stazioni[medio].distanza < distanza){
            inizio = medio + 1;
        } else {
            fine = medio - 1;
        }
    }

    return -1;
}


// Funzione per liberare la memoria occupata da una lista concatenata
void dealloca_lista (STAZIONE *testa){

    STAZIONE *cursore = testa;

    while (cursore != NULL){
        STAZIONE *successore = cursore->next;
        free (cursore);
        cursore = successore;
    }
}



/* AGGIUNGI STAZIONE */
void aggiungi_stazione (STAZIONE **testa, INT distanza){

    STAZIONE *nuovo_nodo = NULL;
    STAZIONE *cursore = NULL;
    INT autonomia_temp = 0;
    INT indice = 0;

    // Se la stazione non va inserita perché ha la stessa distanza della testa della lista
    if (*testa != NULL && (*testa)->distanza == distanza){
        printf("non aggiunta\n");
        return;

        // Se la stazione va inserita prima della testa della lista
    } else if (*testa == NULL || distanza < (*testa)->distanza){
        nuovo_nodo = (STAZIONE *) malloc (sizeof(STAZIONE));
        nuovo_nodo->distanza = distanza;
        nuovo_nodo->max_autonomia = 0;
        inizializza_tabella (&nuovo_nodo);
        // Gestione del parco auto
        if (scanf("%d", &nuovo_nodo->num_auto) < 0)
            exit(EXIT_FAILURE);
        for (INT i = 0; i < nuovo_nodo->num_auto; i++){
            if (scanf("%d", &autonomia_temp) < 0)
                exit(EXIT_FAILURE);
            indice = funzione_hash (autonomia_temp);
            // Gestione delle collisioni con il metodo dell'indirizzamento aperto
            while (nuovo_nodo->autonomia[indice] != BLANK){
                indice = (indice + 1) % (2 * MAX_AUTO);
            }
            nuovo_nodo->autonomia[indice] = autonomia_temp;
            if (autonomia_temp > nuovo_nodo->max_autonomia)
                nuovo_nodo->max_autonomia = autonomia_temp;
        }
        nuovo_nodo->next = *testa;
        *testa = nuovo_nodo;
        printf("aggiunta\n");
        return;

        // Trovo la posizione dove inserire la nuova stazione (se non esiste già)
    } else {
        cursore = *testa;
        while (cursore->next != NULL && cursore->next->distanza <= distanza){
            if (cursore->next->distanza == distanza){
                printf("non aggiunta\n");
                return;
            }
            cursore = cursore->next;
        }
        nuovo_nodo = (STAZIONE *) malloc (sizeof(STAZIONE));
        nuovo_nodo->distanza = distanza;
        nuovo_nodo->max_autonomia = 0;
        inizializza_tabella (&nuovo_nodo);
        // Gestione del parco auto
        if (scanf("%d", &nuovo_nodo->num_auto) < 0)
            exit(EXIT_FAILURE);
        for (INT i = 0; i < nuovo_nodo->num_auto; i++){
            if (scanf("%d", &autonomia_temp) < 0)
                exit(EXIT_FAILURE);
            indice = funzione_hash (autonomia_temp);
            // Gestione delle collisioni con il metodo dell'indirizzamento aperto
            while (nuovo_nodo->autonomia[indice] != BLANK){
                indice = (indice + 1) % (2 * MAX_AUTO);
            }
            nuovo_nodo->autonomia[indice] = autonomia_temp;
            if (autonomia_temp > nuovo_nodo->max_autonomia)
                nuovo_nodo->max_autonomia = autonomia_temp;
        }
        nuovo_nodo->next = cursore->next;
        cursore->next = nuovo_nodo;
        printf("aggiunta\n");
        return;
    }
}



/* DEMOLISCI STAZIONE */
void demolisci_stazione (STAZIONE **testa, INT distanza){

    STAZIONE *cursore = *testa;
    STAZIONE *precedente = NULL;

    // Cerco se la stazione esiste
    while (cursore != NULL && cursore->distanza < distanza){
        precedente = cursore;
        cursore = cursore->next;
    }

    // La stazione non esiste
    if (cursore == NULL || cursore->distanza > distanza){
        printf("non demolita\n");
        return;
    }

    // La stazione da demolire è la testa
    if (precedente == NULL){
        *testa = (*testa)->next;
        free (cursore);
        printf("demolita\n");
        return;

        // La stazione da demolire è un nodo qualsiasi
    } else {
        precedente->next = cursore->next;
        free (cursore);
        printf("demolita\n");
        return;
    }
}



/* AGGIUNGI AUTO */
void aggiungi_auto (STAZIONE **testa, INT distanza, INT autonomia){

    STAZIONE *cursore = *testa;
    INT indice = 0;

    // Cerco se la stazione esiste
    while (cursore != NULL && cursore->distanza < distanza){
        cursore = cursore->next;
    }

    // La stazione non esiste
    if (cursore == NULL || cursore->distanza > distanza){
        printf("non aggiunta\n");
        return;

        // La stazione esiste
    } else {
        cursore->num_auto++;
        indice = funzione_hash (autonomia);
        // Gestione delle collisioni con il metodo dell'indirizzamento aperto
        while (cursore->autonomia[indice] != BLANK && cursore->autonomia[indice] != CANC){
            indice = (indice + 1) % (2 * MAX_AUTO);
        }
        cursore->autonomia[indice] = autonomia;
        if (autonomia > cursore->max_autonomia)
            cursore->max_autonomia = autonomia;
        printf("aggiunta\n");
        return;
    }
}



/* ROTTAMA AUTO */
void rottama_auto (STAZIONE **testa, INT distanza, INT autonomia){

    STAZIONE *cursore = *testa;
    INT indice = 0;

    // Cerco se la stazione esiste
    while (cursore != NULL && cursore->distanza < distanza){
        cursore = cursore->next;
    }

    // La stazione non esiste
    if (cursore == NULL || cursore->distanza > distanza){
        printf("non rottamata\n");
        return;

        // La stazione esiste
    } else {
        // Cerco l'auto da rottamare
        indice = funzione_hash (autonomia);
        while (cursore->autonomia[indice] != BLANK){
            // Non trovo l'auto, indice++
            if (cursore->autonomia[indice] != autonomia){
                indice = (indice + 1) % (2 * MAX_AUTO);
                // Auto trovata
            } else {
                cursore->autonomia[indice] = CANC;
                cursore->num_auto--;
                if (cursore->max_autonomia == autonomia){
                    cursore->max_autonomia = 0;
                    for (INT i = 0; i < 2 * MAX_AUTO; i++){
                        if (cursore->autonomia[i] > cursore->max_autonomia){
                            cursore->max_autonomia = cursore->autonomia[i];
                        }
                    }
                }
                printf("rottamata\n");
                return;
            }
        }
        // Auto non esiste, non rottamo
        if (cursore->autonomia[indice] != autonomia){
            printf("non rottamata\n");
            return;
        } else {
            return;
        }
    }
}



/* PIANIFICA PERCORSO */
void pianifica_percorso (ARRAY_STAZIONE *stazioni, INT num_stazioni, INT partenza, INT arrivo){

    // Inizializzo le variabili
    INT num_fermate = 0;
    INT *fermate = (INT *) malloc (num_stazioni * sizeof(INT));
    INT *tappe_minime = (INT *) malloc (num_stazioni * sizeof(INT));
    INT distanza = 0;
    INT min_distanza = 0;
    INT *distanze = (INT *) malloc (num_stazioni * sizeof(INT));
    INT *precedente = (INT *) malloc (num_stazioni * sizeof(INT));
    bool *visitata = (bool *) malloc (num_stazioni * sizeof(bool));
    INT m = 0;
    INT autonomia = 0;
    INT indice_stazione = 0;

    // Inizializzo distanze e tappe minime di tutte le stazioni
    for (INT i = 0; i < num_stazioni; i++){
        // Imposto il numero minimo di tappe iniziale a un valore massimo
        tappe_minime[i] = MAX_INT;
        // Imposto la distanza iniziale a un valore massimo
        distanze[i] = MAX_INT;
        // Inizializzo le stazioni precedenti come non definite
        precedente[i] = BLANK;
        // Inizializzo le stazioni come non visitate
        visitata[i] = false;
    }

    // Inizializzo la distanza e le tappe minime della stazione di partenza a zero
    // Imposto il numero minimo di tappe della stazione di partenza a zero
    tappe_minime[partenza] = 0;
    // Imposto la distanza della stazione di partenza a zero
    distanze[partenza] = 0;

    // Utilizzo l'algoritmo di Dijkstra per trovare il percorso con meno fermate
    for (INT j = 0; j < num_stazioni - 1; j++){
        // Variabile per tenere traccia della stazione corrente
        m = BLANK;
        // Variabile per tenere traccia della distanza minima
        min_distanza = MAX_INT;

        // Trovo la stazione non visitata con distanza minima
        for (INT n = 0; n < num_stazioni; n++){
            if (!visitata[n] && distanze[n] < min_distanza){
                // Aggiorno la stazione corrente con la stazione non visitata con la distanza minima
                m = n;
                // Aggiorno la distanza minima
                min_distanza = distanze[n];
            }
        }

        // Se non posso raggiungere nessuna stazione termino
        if (m == BLANK)
            break;
        // Imposto la stazione corrente come visitata
        visitata[m] = true;

        // Aggiorno le distanze delle stazioni vicine
        for (INT n = 0; n < num_stazioni; n++){
            // Calcolo la distanza tra la stazione corrente e la stazione vicina (valore assoluto)
            distanza = abs (stazioni[m].distanza - stazioni[n].distanza);
            // Utilizzo l'autonomia massima della stazione corrente
            autonomia = stazioni[m].max_autonomia;
            // Verifico se la stazione vicina non è stata visitata, se la distanza è inferiore o uguale all'autonomia
            // dell'auto della stazione corrente e se la distanza della stazione corrente non è ancora "MAX_INT"
            if (!visitata[n] && distanza <= autonomia && distanze[m] != MAX_INT){
                if (distanze[m] + 1 < distanze[n]){
                    // Aggiorno la distanza della stazione vicina
                    distanze[n] = distanze[m] + 1;
                    // Imposto la stazione corrente come precedente a quella vicina
                    precedente[n] = m;
                    // Aggiorno il numero minimo di tappe della stazione vicina
                    tappe_minime[n] = tappe_minime[m] + 1;
                } else if (distanze[m] + 1 == distanze[n] && tappe_minime[m] + 1 < tappe_minime[n]){
                    // Aggiorno il numero minimo di tappe della stazione vicina
                    tappe_minime[n] = tappe_minime[m] + 1;
                    // Imposto la stazione corrente come precedente a quella vicina
                    precedente[n] = m;
                }
            }
        }
    }

    // Verifico se è stato trovato un percorso
    if (distanze[arrivo] == MAX_INT){
        printf("nessun percorso\n");
    } else {
        // Costruisco il percorso
        indice_stazione = arrivo;
        while (indice_stazione != BLANK){
            // Memorizzo l'indice della stazione nel percorso finale
            fermate[num_fermate++] = indice_stazione;
            // Passo alla stazione precedente
            indice_stazione = precedente[indice_stazione];
        }
        // Stampo il percorso (al contrario)
        for (INT i = num_fermate - 1; i >= 0; i--){
            if (i != 0)
                printf("%d ", stazioni[fermate[i]].distanza);
            else
                printf("%d\n", stazioni[fermate[i]].distanza);
        }
    }

    // Dealloco la memoria
    free (fermate);
    free (tappe_minime);
    free (distanze);
    free (precedente);
    free (visitata);
}



// MAIN
INT main (INT argc, char *argv[]){

    char comando[19] = "";
    STAZIONE *testa = NULL;
    INT distanza = 0;
    INT autonomia = 0;
    INT partenza = 0;
    INT arrivo = 0;
    ARRAY_STAZIONE *stazioni = NULL;
    INT num_stazioni = 0;

    // Lettura input, leggo il comando
    while (scanf("%s", comando) == 1){
        // Eseguo la funzione del comando
        if (strcmp(comando, "aggiungi-stazione") == 0){
            if (scanf("%d", &distanza) < 0)
                exit(EXIT_FAILURE);
            aggiungi_stazione (&testa, distanza);
        }
        if (strcmp(comando, "demolisci-stazione") == 0){
            if (scanf("%d", &distanza) < 0)
                exit(EXIT_FAILURE);
            demolisci_stazione (&testa, distanza);
        }
        if (strcmp(comando, "aggiungi-auto") == 0){
            if (scanf("%d", &distanza) < 0)
                exit(EXIT_FAILURE);
            if (scanf("%d", &autonomia) < 0)
                exit(EXIT_FAILURE);
            aggiungi_auto (&testa, distanza, autonomia);
        }
        if (strcmp(comando, "rottama-auto") == 0){
            if (scanf("%d", &distanza) < 0)
                exit(EXIT_FAILURE);
            if (scanf("%d", &autonomia) < 0)
                exit(EXIT_FAILURE);
            rottama_auto (&testa, distanza, autonomia);
        }
        if (strcmp(comando, "pianifica-percorso") == 0){
            if (scanf("%d", &partenza) < 0)
                exit(EXIT_FAILURE);
            if (scanf("%d", &arrivo) < 0)
                exit(EXIT_FAILURE);
            stazioni = (ARRAY_STAZIONE *) malloc (MAX_STAZIONI * sizeof(ARRAY_STAZIONE));
            num_stazioni = lista_in_array (testa, &stazioni, 0);
            partenza = ricerca_indice_stazione (stazioni, num_stazioni, partenza);
            arrivo = ricerca_indice_stazione (stazioni, num_stazioni, arrivo);
            pianifica_percorso (stazioni, num_stazioni, partenza, arrivo);
            free (stazioni);
        }
    }

    // Deallocazione memoria
    dealloca_lista (testa);


    return 0;
}
