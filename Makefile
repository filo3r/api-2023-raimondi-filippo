# Definire il compilatore da usare
CC = gcc

# Definire i flag di compilazione: -Wall per mostrare tutti gli avvisi, -Wextra per mostrare avvisi extra, -Iinclude per includere i file di intestazione
CFLAGS = -Wall -Wextra -Iinclude

# Definire il nome dell'eseguibile
TARGET = bin/api-2023-raimondi-filippo

# Definire le cartelle
SRC_DIR = src
OBJ_DIR = obj

# Definire i file sorgente e i corrispondenti file oggetto
SRC = $(SRC_DIR)/main.c
OBJ = $(OBJ_DIR)/main.o

# Definire la regola predefinita: compilare tutto
all: $(TARGET)

# Definire la regola per compilare l'eseguibile finale
$(TARGET): $(OBJ)
	@mkdir -p bin  # Creare la cartella bin se non esiste
	$(CC) -o $@ $^  # Compilare i file oggetto in un eseguibile

# Definire la regola per compilare i file .c in file .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  # Creare la cartella obj se non esiste
	$(CC) $(CFLAGS) -c $< -o $@  # Compilare il file sorgente .c in un file oggetto .o

# Definire la regola per pulire i file oggetto e l'eseguibile
clean:
	rm -f $(OBJ) $(TARGET)  # Rimuovere i file oggetto e l'eseguibile

# Dichiarare regole aggiuntive per facilitare la gestione
.PHONY: all clean  # Dichiarare che 'all' e 'clean' non sono file ma nomi di comandi
