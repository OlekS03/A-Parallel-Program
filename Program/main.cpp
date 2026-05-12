#include <iostream>
#include <chrono>
#include <cstdlib>
#include <random>
#include <mpi.h>
#include "Graph.h"
#include "AStar.h"
#include "BidirectionalAStar.h"
#include "ParallelAStar.h"
#include "WaypointAStar.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (argc != 3) {
        if (rank == 0) std::cerr << "Usage: mpirun -n <cores> " << argv[0] << " <size> <instances>\n";
        MPI_Finalize();
        return 1;
    }

    int size = std::atoi(argv[1]);
    int instances = std::atoi(argv[2]);

    if (rank == 0) {
        std::cout << "==== Results ====\n";
        std::cout << "Map size: " << size << "x" << size << "\n";
        std::cout << "Runs: " << instances << "\n";
        std::cout << "MPI Processes: " << num_procs << "\n";
    }

    // Tracking variables for all 4 algorithms
    int solvedCountAStar = 0;
    int solvedCountBidir = 0;
    int solvedCountParallel = 0;
    int solvedCountWaypointAStar = 0;
    
    double totalTimeAStarMs = 0.0;
    double totalTimeBidirMs = 0.0;
    double totalTimeParallelMs = 0.0;
    double totalTimeWaypointAStarMs = 0.0;

    std::random_device rd;

    for (int i = 0; i < instances; i++) {
        unsigned int current_seed;
        if (rank == 0) current_seed = rd();
        MPI_Bcast(&current_seed, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

        Graph gameMap(size);
        // Generate the guaranteed solvable open game map
        gameMap.generateGuaranteedGameMap(current_seed, 0.40); 

        if (rank == 0) {
            // 1. Standard A*
            auto startA = std::chrono::high_resolution_clock::now();
            bool solvedA = AStar::solve(gameMap);
            auto endA = std::chrono::high_resolution_clock::now();
            totalTimeAStarMs += std::chrono::duration<double, std::milli>(endA - startA).count();
            if (solvedA) solvedCountAStar++;

            // 2. Bidirectional A*
            auto startB = std::chrono::high_resolution_clock::now();
            bool solvedB = BidirectionalAStar::solve(gameMap);
            auto endB = std::chrono::high_resolution_clock::now();
            totalTimeBidirMs += std::chrono::duration<double, std::milli>(endB - startB).count();
            if (solvedB) solvedCountBidir++;
        }

        MPI_Barrier(MPI_COMM_WORLD); 
        auto startP1 = std::chrono::high_resolution_clock::now();
        bool localSolvedP1 = ParallelAStar::solveSegment(gameMap, rank, num_procs);
        auto endP1 = std::chrono::high_resolution_clock::now();
        
        double localDurationP1 = std::chrono::duration<double, std::milli>(endP1 - startP1).count();
        double maxDurationP1;
        MPI_Reduce(&localDurationP1, &maxDurationP1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        int localSuccessP1 = localSolvedP1 ? 1 : 0;
        int globalSuccessP1;
        MPI_Reduce(&localSuccessP1, &globalSuccessP1, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            totalTimeParallelMs += maxDurationP1;
            if (globalSuccessP1 == num_procs) solvedCountParallel++;
        }

        // 4. Waypoint A* (Smart Snapping Split)
        MPI_Barrier(MPI_COMM_WORLD); 
        auto startP2 = std::chrono::high_resolution_clock::now();
        bool localSolvedP2 = WaypointAStar::solveSegment(gameMap, rank, num_procs);
        auto endP2 = std::chrono::high_resolution_clock::now();
        
        double localDurationP2 = std::chrono::duration<double, std::milli>(endP2 - startP2).count();
        double maxDurationP2;
        MPI_Reduce(&localDurationP2, &maxDurationP2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        int localSuccessP2 = localSolvedP2 ? 1 : 0;
        int globalSuccessP2;
        MPI_Reduce(&localSuccessP2, &globalSuccessP2, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            totalTimeWaypointAStarMs += maxDurationP2;
            if (globalSuccessP2 == num_procs) solvedCountWaypointAStar++;
        }
    }

    if (rank == 0) {
        std::cout << "\n--- 1. Standard A* ---\n";
        std::cout << "Solved: " << solvedCountAStar << "/" << instances << "\n";
        std::cout << "Average solve time: " << (totalTimeAStarMs / instances) << " ms\n";

        std::cout << "\n--- 2. Bidirectional A* ---\n";
        std::cout << "Solved: " << solvedCountBidir << "/" << instances << "\n";
        std::cout << "Average solve time: " << (totalTimeBidirMs / instances) << " ms\n";

        std::cout << "\n--- 3. Parallel A* ---\n";
        std::cout << "Solved: " << solvedCountParallel << "/" << instances << "\n";
        std::cout << "Average solve time: " << (totalTimeParallelMs / instances) << " ms\n";

        std::cout << "\n--- 4. Waypoint A* ---\n";
        std::cout << "Solved: " << solvedCountWaypointAStar << "/" << instances << "\n";
        std::cout << "Average solve time: " << (totalTimeWaypointAStarMs / instances) << " ms\n";
    }

    MPI_Finalize();
    return 0;
}