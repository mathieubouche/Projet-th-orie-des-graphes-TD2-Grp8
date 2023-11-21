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
#include <stdio.h>

// Structure pour stocker une opération avec son numéro et son temps d'exécution
struct Operation {
    int number;
    float executionTime;
};

// Fonction pour fusionner deux parties triées d'un tableau
void merge(struct Operation arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Créer des tableaux temporaires
    struct Operation L[n1], R[n2];

    // Copier les données vers les tableaux temporaires L[] et R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Fusionner les tableaux temporaires dans arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].executionTime <= R[j].executionTime) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copier les éléments restants de L[], si il y en a
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copier les éléments restants de R[], si il y en a
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Fonction principale pour trier un tableau d'opérations en utilisant le tri par fusion
void mergeSort(struct Operation arr[], int l, int r) {
    if (l < r) {
        // Trouver le point médian
        int m = l + (r - l) / 2;

        // Trier la première et la deuxième moitié
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Fusionner les moitiés triées
        merge(arr, l, m, r);
    }
}

int operation(){
    FILE *fptr;

    int operationCount = 0;
    int operationNumber;
    float executionTime;

    // Ouvrir le fichier en mode lecture
    fptr = fopen("operations.txt", "r");

    // Vérifier si le fichier est ouvert avec succès
    if (fptr == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1; // Exit with error code
    }

    // Lire chaque paire opération-temps du fichier et incrémenter le nombre d'opérations
    while (fscanf(fptr, "%d %f", &operationNumber, &executionTime) == 2) {
        operationCount++;
    }

    // Allouer un tableau pour stocker les opérations
    struct Operation operations[operationCount];

    // Rembobiner le fichier pour relire les opérations
    rewind(fptr);

    // Lire chaque paire opération-temps du fichier et stocker dans le tableau
    for (int i = 0; i < operationCount; i++) {
        fscanf(fptr, "%d %f", &operations[i].number, &operations[i].executionTime);
    }

    // Fermer le fichier
    fclose(fptr);

    // Trier les opérations en fonction de leur temps d'exécution
    mergeSort(operations, 0, operationCount - 1);

    // Afficher le nombre d'opérations
    printf("Le nombre d'operations dans le fichier est : %d\n", operationCount);

    // Afficher les opérations triées
    printf("Opérations triees en fonction du temps d'execution :\n");
    for (int i = 0; i < operationCount; i++) {
        printf("Operation %d : Temps d'execution %.2f\n", operations[i].number, operations[i].executionTime);
    }

}

int main() {
    operation();
    return 0;
}


