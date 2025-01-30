#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <time.h> // For clock function

#define MAX_CITIES 5
#define INF 9999
#define MAX_FUEL_TYPES 3
#define MAX_FUEL_COST 1000
#define MAX_CITY_NAME_LENGTH 20

// City names
const char *cityNames[MAX_CITIES] = {"Mirpur", "Uttara", "Badda", "Farmgate", "Motijheel"};

// Function to find the index of a city by name
int findCityIndex(const char *cityName) {
    for (int i = 0; i < MAX_CITIES; i++) {
        if (strcmp(cityName, cityNames[i]) == 0) {
            return i;
        }
    }
    return -1; // City not found
}

// Dijkstra’s Algorithm for shortest path
void dijkstra(int graph[MAX_CITIES][MAX_CITIES], int src, int n, int dist[]) {
    int visited[MAX_CITIES];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int min = INF, minIndex = -1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = v;
            }

        if (minIndex == -1)
            break;

        visited[minIndex] = 1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && graph[minIndex][v] && dist[minIndex] != INF
                && dist[minIndex] + graph[minIndex][v] < dist[v])
                dist[v] = dist[minIndex] + graph[minIndex][v];
    }
}

// Dynamic Programming for minimum fuel cost (Coin Change Variant)
int minFuelCost(int fuelOptions[], int fuelCost[], int numFuelTypes, int requiredFuel) {
    int dp[MAX_FUEL_COST];

    for (int i = 0; i <= requiredFuel; i++)
        dp[i] = INF;

    dp[0] = 0;

    for (int i = 0; i < numFuelTypes; i++) {
        for (int j = fuelOptions[i]; j <= requiredFuel; j++) {
            if (dp[j - fuelOptions[i]] != INF)
                dp[j] = (dp[j] < dp[j - fuelOptions[i]] + fuelCost[i]) ? dp[j] : dp[j - fuelOptions[i]] + fuelCost[i];
        }
    }
    return (dp[requiredFuel] == INF) ? -1 : dp[requiredFuel];
}

int main() {
    int n;
    printf("Enter total number of parcels: ");
    scanf("%d", &n);

    // Start clock for graph creation
    clock_t start = clock();

    // Graph for shortest path (adjacency matrix)
    int graph[MAX_CITIES][MAX_CITIES] = {
        {0, 10, 20, 0, 0},
        {10, 0, 5, 15, 0},
        {20, 5, 0, 30, 10},
        {0, 15, 30, 0, 5},
        {0, 0, 10, 5, 0}
    };

    // Stop clock for graph creation
    clock_t end = clock();
    double graphCreationTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nGraph creation runtime: %.6f seconds\n", graphCreationTime);

    char startCity[MAX_CITY_NAME_LENGTH];
    printf("Enter starting city (Mirpur,Uttara,Badda,Farmgate,Motijheel): ");
    scanf("%s", startCity);

    int startCityIndex = findCityIndex(startCity);
    if (startCityIndex == -1) {
        printf("Invalid city name. Exiting program.\n");
        return 1;
    }

    int dist[MAX_CITIES];
    dijkstra(graph, startCityIndex, MAX_CITIES, dist);

    printf("\nShortest delivery path from %s:\n", startCity);
    for (int i = 0; i < MAX_CITIES; i++) {
        printf("To %s: %d km\n", cityNames[i], dist[i]);
    }

    // Calculate the charge for each parcel
    int chargePerKm = 55;
    int totalCharge = 0;

    for (int i = 0; i < n; i++) {
        char destinationCity[MAX_CITY_NAME_LENGTH];
        printf("Enter destination city for parcel %d (Mirpur, Uttara, Badda, Farmgate, Motijheel): ", i + 1);
        scanf("%s", destinationCity);

        int destinationCityIndex = findCityIndex(destinationCity);
        if (destinationCityIndex == -1) {
            printf("Invalid city name. Please try again.\n");
            i--; // Retry for the same parcel
            continue;
        }

        int parcelCharge = dist[destinationCityIndex] * chargePerKm;
        totalCharge += parcelCharge;
        printf("Charge for parcel %d to %s: %d taka\n", i + 1, destinationCity, parcelCharge);
    }

    printf("\nTotal charge for all parcels: %d taka\n", totalCharge);

    // Fuel Optimization
    int fuelOptions[MAX_FUEL_TYPES] = {1, 5, 10}; // Fuel units
    int fuelCost[MAX_FUEL_TYPES] = {10, 45, 80};  // Cost per unit
    int requiredFuel = dist[MAX_CITIES - 1];  // Fuel needed for last destination

    int minCost = minFuelCost(fuelOptions, fuelCost, MAX_FUEL_TYPES, requiredFuel);
    if (minCost == -1)
        printf("\nFuel cost optimization failed: Not enough fuel options available.\n");
    else
        printf("\nMinimum fuel cost for delivery: %d\n", minCost);

    return 0;
}
