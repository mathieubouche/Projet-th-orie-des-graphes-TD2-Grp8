#include <stdio.h>
#include <stdlib.h>

// Structure pour la matrice d'adjacence
typedef struct {
    int** matrix;
    int size;
} AdjacencyMatrix;

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

// Fonction pour afficher la matrice d'adjacence
void printMatrix(const AdjacencyMatrix* adjacencyMatrix) {
    printf("Matrice d'adjacence :\n");
    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        for (int j = 1; j <= adjacencyMatrix->size; j++) {
            printf("%d ", adjacencyMatrix->matrix[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour libérer la mémoire allouée pour la matrice
void freeMatrix(AdjacencyMatrix* adjacencyMatrix) {
    for (int i = 1; i <= adjacencyMatrix->size; i++) {
        free(adjacencyMatrix->matrix[i]);
    }
    free(adjacencyMatrix->matrix);
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

    // Afficher la matrice d'adjacence
    printMatrix(&adjacencyMatrix);

    // Utiliser la matrice d'adjacence selon vos besoins...

    // Libérer la mémoire allouée
    freeMatrix(&adjacencyMatrix);

    return 0;
}
