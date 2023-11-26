#include <stdio.h>
#include <stdlib.h>

// Structure pour la matrice d'adjacence
typedef struct {
    int** matrix;
    int size;
} AdjacencyMatrix;

// Structure pour représenter une station et les opérations qui lui sont attribuées
typedef struct {
    int* operations;
    int count;
} Station;

// Fonction pour initialiser la matrice d'adjacence
AdjacencyMatrix initMatrix(int size) {
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.size = size;

    // Allouer de la mémoire pour la matrice
    adjacencyMatrix.matrix = (int**)malloc((size + 1) * sizeof(int*));
    for (int i = 1; i <= size; i++) {
        adjacencyMatrix.matrix[i] = (int*)malloc((size + 1) * sizeof(int));
        for (int j = 1; j <= size; j++) {
            adjacencyMatrix.matrix[i][j] = 0;  // Initialiser à zéro
        }
    }

    return adjacencyMatrix;
}

// Fonction pour lire les contraintes depuis le fichier et mettre à jour la matrice d'adjacence
void readConstraints(FILE* fichier, AdjacencyMatrix* adjacencyMatrix) {
    int op1, op2;

    // Lire les contraintes du fichier et les ajouter dans la matrice
    while (fscanf(fichier, "%d %d", &op1, &op2) == 2) {
        adjacencyMatrix->matrix[op1][op2] = 1;
        adjacencyMatrix->matrix[op2][op1] = 1;
    }
}

// Fonction pour libérer la mémoire allouée pour la matrice
void freeMatrix(AdjacencyMatrix* adjacencyMatrix) {
    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        free(adjacencyMatrix->matrix[i]);
    }
    free(adjacencyMatrix->matrix);
}

// Fonction pour répartir les opérations par station
void assignOperations(const AdjacencyMatrix* adjacencyMatrix) {
    int maxStations = 1;  // Nombre initial de stations
    Station* stations = (Station*)malloc(maxStations * sizeof(Station));

    // Initialiser la première station
    stations[0].operations = (int*)malloc((adjacencyMatrix->size + 1) * sizeof(int));
    stations[0].count = 0;

    // Liste pour stocker les paires d'opérations incompatibles
    int** incompatiblePairs = (int**)malloc(adjacencyMatrix->size * sizeof(int*));
    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        incompatiblePairs[i - 1] = (int*)malloc((adjacencyMatrix->size + 1) * sizeof(int));
        for (int j = 1; j <= adjacencyMatrix->size; j++) {
            incompatiblePairs[i - 1][j] = 0;
        }
    }

    // Trouver les paires d'opérations incompatibles
    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        for (int j = i + 1; j <= adjacencyMatrix->size; j++) {
            if (adjacencyMatrix->matrix[i][j] == 1) {
                incompatiblePairs[i - 1][j] = 1;
                incompatiblePairs[j - 1][i] = 1;
            }
        }
    }

    // Afficher les paires d'opérations incompatibles
    printf("Operations non realisables par la meme station :\n");
    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        for (int j = i + 1; j <= adjacencyMatrix->size; j++) {
            if (incompatiblePairs[i - 1][j] == 1) {
                printf("%d et %d\n", i, j);
            }
        }
    }
    printf("\n");

    // Attribuer les opérations aux stations
    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        int assigned = 0;
        // Parcourir les stations existantes
        for (int j = 0; j < maxStations; j++) {
            int conflict = 0;
            // Vérifier si l'opération est en conflit avec les opérations déjà attribuées à la station
            for (int k = 0; k < stations[j].count; k++) {
                if (incompatiblePairs[i - 1][stations[j].operations[k]] == 1) {
                    conflict = 1;
                    break;
                }
            }
            // Si pas de conflit, attribuer l'opération à la station
            if (!conflict) {
                stations[j].operations[stations[j].count++] = i;
                assigned = 1;
                break;
            }
        }
        // Si toutes les stations sont occupées, ajouter une nouvelle station
        if (!assigned) {
            maxStations++;
            stations = (Station*)realloc(stations, maxStations * sizeof(Station));
            stations[maxStations - 1].operations = (int*)malloc((adjacencyMatrix->size + 1) * sizeof(int));
            stations[maxStations - 1].count = 0;
            stations[maxStations - 1].operations[stations[maxStations - 1].count++] = i;
        }
    }

    // Afficher la répartition des opérations par station
    printf("\nRepartition des operations par station :\n");
    for (int i = 0; i < maxStations; i++) {
        printf("Station %d : ", i + 1);
        for (int j = 0; j < stations[i].count; j++) {
            printf("%d ", stations[i].operations[j]);
        }
        printf("\n");
    }

    // Libérer la mémoire allouée
    for (int i = 0; i < maxStations; i++) {
        free(stations[i].operations);
    }
    free(stations);

    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        free(incompatiblePairs[i - 1]);
    }
    free(incompatiblePairs);
}


int main() {
    FILE* fichier = fopen("exclusions.txt", "r");

    if (fichier == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    int maxOperation = 0;  // pour déterminer la taille de la matrice
    int op1, op2;

    // Trouver le nombre maximum d'opérations
    while (fscanf(fichier, "%d %d", &op1, &op2) == 2) {
        maxOperation = (op1 > maxOperation) ? op1 : maxOperation;
        maxOperation = (op2 > maxOperation) ? op2 : maxOperation;
    }

    // Réinitialiser le curseur du fichier
    fseek(fichier, 0, SEEK_SET);

    // Initialiser la matrice d'adjacence
    AdjacencyMatrix adjacencyMatrix = initMatrix(maxOperation);

    // Lire les contraintes et mettre à jour la matrice
    readConstraints(fichier, &adjacencyMatrix);

    // Fermer le fichier
    fclose(fichier);

    // Répartir les opérations par station
    assignOperations(&adjacencyMatrix);

    // Libérer la mémoire allouée
    freeMatrix(&adjacencyMatrix);

    return 0;
}
