#include <stdio.h>
#include <stdbool.h>



// Fonction pour vérifier si les opérations assignées à une station respectent le temps de cycle
bool checkCycleTimeConstraint(int operations[], int numOperations, int cycleTime) {
    int totalTime = 0;
    for (int i = 0; i < numOperations; ++i) {
        totalTime += operations[i];
    }
    return totalTime <= cycleTime;
}

int main() {
    int cycleTime = 10; // Temps de cycle, par exemple 10 minutes
    int operations[100] = {2, 3, 5}; // Durées des opérations pour une station donnée
    int numOperations = 3; // Nombre d'opérations pour cette station

    // Vérifie si la station respecte la contrainte de temps de cycle
    if (checkCycleTimeConstraint(operations, numOperations, cycleTime)) {
        printf("La station respecte la contrainte de temps de cycle.\n");
    } else {
        printf("La station ne respecte pas la contrainte de temps de cycle.\n");
    }

    return 0;
}
