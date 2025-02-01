#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <time.h> // For clock() function

// Macros
#define MAX_CITIES 5
#define INF 9999 // A large value representing infinity
#define MAX_FUEL_TYPES 3
#define MAX_FUEL_COST 1000
#define MAX_CITY_NAME_LENGTH 20 // Maximum length of a city name

// City names
const char *cityNames[MAX_CITIES] = {"Mirpur", "Uttara", "Badda", "Farmgate", "Motijheel"};

// Structure to represent an edge in the graph
struct Edge {
    int src, dest, weight; // Src = index of the source city; dest= index of destination city; weight= Calculating the cost
};

// Function to find the index of a city by name
int findCityIndex(const char *cityName) {
    for (int i = 0; i < MAX_CITIES; i++) {
        if (strcmp(cityName, cityNames[i]) == 0) {   // strcmp function compares the city name.
            return i;
        }
    }
    return -1; // City not found
}

// Union-Find (Disjoint Set) functions for Kruskal's Algorithm
int parent[MAX_CITIES];
void makeSet(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i; // Each city is its own parent initially
    }
}
int findSet(int x) {
    if (parent[x] != x) {
        parent[x] = findSet(parent[x]); // Path compression
    }
    return parent[x];
}
void unionSet(int x, int y) {
    int xRoot = findSet(x);
    int yRoot = findSet(y);
    parent[yRoot] = xRoot; // Merge the two sets
}

// Kruskal's Algorithm to find the Minimum Spanning Tree (MST)
void kruskal(struct Edge edges[], int numEdges, int n) {
    clock_t start, end; // Variables to measure time
    start = clock(); // Start timer

    makeSet(n); // Initialize disjoint sets
    printf("\nMinimum Spanning Tree (Kruskal's Algorithm):\n");
    int mstWeight = 0;

    // Sort edges by weight (simple bubble sort)
    for (int i = 0; i < numEdges - 1; i++) {
        for (int j = 0; j < numEdges - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                struct Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    // Add edges to the MST
    for (int i = 0; i < numEdges; i++) {
        int src = edges[i].src;
        int dest = edges[i].dest;
        int weight = edges[i].weight;
        int srcRoot = findSet(src);
        int destRoot = findSet(dest);
        if (srcRoot != destRoot) { // If adding this edge doesn't form a cycle
            printf("Edge: %s -> %s, Weight: %d km\n", cityNames[src], cityNames[dest], weight);
            mstWeight += weight;
            unionSet(srcRoot, destRoot);
        }
    }
    printf("Total MST Weight: %d km\n", mstWeight);

    end = clock(); // End timer
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate time in seconds
    printf("Time taken by Kruskal's Algorithm: %.6f seconds\n", time_taken);
}

// Dijkstra’s Algorithm for shortest path
void dijkstra(int graph[MAX_CITIES][MAX_CITIES], int src, int n, int dist[]) {
    clock_t start, end; // Variables to measure time
    start = clock(); // Start timer

    int visited[MAX_CITIES];
    for (int i = 0; i < n; i++) {
        dist[i] = INF; // Initialize all distances to infinity
        visited[i] = 0; // Mark all cities as unvisited
    }
    dist[src] = 0; // Distance to the source city is 0

    for (int count = 0; count < n - 1; count++) {
        int min = INF, minIndex = -1;

        // Find the unvisited city with the smallest distance
        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = v;
            }
        }
        if (minIndex == -1) break; // No more reachable cities
        visited[minIndex] = 1; // Mark the city as visited

        // Update distances to neighboring cities
        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[minIndex][v] && dist[minIndex] != INF
                && dist[minIndex] + graph[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + graph[minIndex][v];
            }
        }
    }

    end = clock(); // End timer
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate time in seconds
    printf("Time taken by Dijkstra's Algorithm: %.6f seconds\n", time_taken);
}

// Dynamic Programming for minimum fuel cost (Coin Change Variant)
int minFuelCost(int fuelOptions[], int fuelCost[], int numFuelTypes, int requiredFuel) {
    clock_t start, end; // Variables to measure time
    start = clock(); // Start timer

    int dp[MAX_FUEL_COST];
    for (int i = 0; i <= requiredFuel; i++) {
        dp[i] = INF; // Initialize all costs to infinity
    }
    dp[0] = 0; // Base case: 0 fuel costs 0

    // Compute the minimum cost for each fuel amount
    for (int i = 0; i < numFuelTypes; i++) {
        for (int j = fuelOptions[i]; j <= requiredFuel; j++) {
            if (dp[j - fuelOptions[i]] != INF) {
                dp[j] = (dp[j] < dp[j - fuelOptions[i]] + fuelCost[i]) ? dp[j] : dp[j - fuelOptions[i]] + fuelCost[i];
            }
        }
    }

    end = clock(); // End timer
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate time in seconds
    printf("Time taken by Fuel Cost Optimization Algorithm: %.6f seconds\n", time_taken);

    return (dp[requiredFuel] == INF) ? -1 : dp[requiredFuel];
}

int main() {
    int n;
    printf("Enter total number of parcels: ");
    scanf("%d", &n);

    // Define the graph as a list of edges
    struct Edge edges[] = {
        {0, 1, 10}, {0, 2, 20}, {1, 2, 5}, {1, 3, 15},
        {2, 3, 30}, {2, 4, 10}, {3, 4, 5}
    };
    int numEdges = sizeof(edges) / sizeof(edges[0]);

    // Run Kruskal's Algorithm to find the MST
    kruskal(edges, numEdges, MAX_CITIES);

    // Convert the edge list back to an adjacency matrix for Dijkstra's Algorithm
    int graph[MAX_CITIES][MAX_CITIES] = {0};
    for (int i = 0; i < numEdges; i++) {
        int src = edges[i].src;
        int dest = edges[i].dest;
        int weight = edges[i].weight;
        graph[src][dest] = weight;
        graph[dest][src] = weight; // Assuming undirected graph
    }

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
    int requiredFuel = dist[MAX_CITIES - 1];      // Fuel needed for last destination
    int minCost = minFuelCost(fuelOptions, fuelCost, MAX_FUEL_TYPES, requiredFuel);
    if (minCost == -1)
        printf("\nFuel cost optimization failed: Not enough fuel options available.\n");
    else
        printf("\nMinimum fuel cost for delivery: %d\n", minCost);

    return 0;
}
