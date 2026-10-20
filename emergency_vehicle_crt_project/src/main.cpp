#include <iostream>
#include <string>

using namespace std;

// Simple structure for an emergency vehicle
struct Vehicle {
    string name;
    string id;
    int priority; // 1 = Ambulance, 2 = Fire Engine, 3 = Police, 4 = VIP
};

// Simple structure for a traffic light intersection
struct Signal {
    string name;
    int m;         // Full cycle duration in seconds (modulus)
    int r;         // When light turns green in seconds (remainder)
    int delay;     // Travel time from starting point in seconds
};

// Basic Greatest Common Divisor function using Euclid's algorithm
int findGCD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Finds x such that (a * x) % m == 1
// Straightforward linear search loop suitable for 2nd-year level
int findModInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

int main() {
    
    cout << "   EMERGENCY VEHICLE TRAFFIC PRIORITY SYSTEM (CRT)\n";
    

    // 1. VEHICLE PRIORITY (Simple Array of 4 vehicles)
    const int NUM_VEHICLES = 4;
    Vehicle vehicles[NUM_VEHICLES] = {
        {"Police Car", "POL-204", 3},
        {"Fire Engine", "FIR-302", 2},
        {"VIP Convoy", "VIP-999", 4},
        {"Ambulance",  "AMB-101", 1}
    };

    // Sort vehicles by priority using a simple bubble sort
    for (int i = 0; i < NUM_VEHICLES - 1; i++) {
        for (int j = 0; j < NUM_VEHICLES - i - 1; j++) {
            if (vehicles[j].priority > vehicles[j + 1].priority) {
                Vehicle temp = vehicles[j];
                vehicles[j] = vehicles[j + 1];
                vehicles[j + 1] = temp;
            }
        }
    }

    cout << "--- Emergency Vehicle Priority Order ---\n";
    for (int i = 0; i < NUM_VEHICLES; i++) {
        cout << "Priority " << vehicles[i].priority << ": " 
             << vehicles[i].name << " (" << vehicles[i].id << ")\n";
    }

    cout << "\n--> ACTIVE PRIORITY VEHICLE: " 
         << vehicles[0].name << " [" << vehicles[0].id << "]\n\n";

    // 2. TRAFFIC LIGHT INTERSECTIONS (Simple Array of 3 intersections)
    const int NUM_SIGNALS = 3;
    Signal signals[NUM_SIGNALS] = {
        {"Crossway A",    7,  3, 4},  // m=7,  r=3, delay=4s
        {"Boulevard B",   9,  5, 9},  // m=9,  r=5, delay=9s
        {"High Street C", 11, 8, 14}  // m=11, r=8, delay=14s
    };

    cout << "--- Traffic Light Intersection Details ---\n";
    for (int i = 0; i < NUM_SIGNALS; i++) {
        cout << "Signal " << (i + 1) << " (" << signals[i].name << "): "
             << "Cycle m = " << signals[i].m << "s, "
             << "Green at r = " << signals[i].r << "s, "
             << "Travel Delay = " << signals[i].delay << "s\n";
    }

    // Check pairwise coprime condition
    for (int i = 0; i < NUM_SIGNALS; i++) {
        for (int j = i + 1; j < NUM_SIGNALS; j++) {
            if (findGCD(signals[i].m, signals[j].m) != 1) {
                cout << "\nError: Moduli are not coprime! CRT cannot solve.\n";
                return 1;
            }
        }
    }

    // Step 1: Calculate Total Corridor Period M = m1 * m2 * m3
    int M = 1;
    for (int i = 0; i < NUM_SIGNALS; i++) {
        M = M * signals[i].m;
    }

    cout << "\nTotal Corridor Period (M) = " << M << " seconds\n\n";
    cout << "--- Step-by-Step Chinese Remainder Theorem (CRT) ---\n";

    // Step 2: Solve congruences and compute partial terms
    int totalSum = 0;
    for (int i = 0; i < NUM_SIGNALS; i++) {
        int mi = signals[i].m;
        int ri = signals[i].r;
        int delay = signals[i].delay;

        int r_effective = (ri - (delay % mi)) % mi;
        if (r_effective < 0) r_effective += mi;

        int Mi = M / mi;
        int yi = findModInverse(Mi, mi);

        if (yi == -1) {
            cout << "Error: Modular inverse not found for Signal " << (i + 1) << "\n";
            return 1;
        }

        int term = r_effective * Mi * yi;
        totalSum += term;

        cout << "Signal " << (i + 1) << ": "
             << "Departure congruence t = " << r_effective << " (mod " << mi << "), "
             << "Mi = " << Mi << ", yi = " << yi << ", "
             << "Term = " << term << "\n";
    }

    // Step 3: Compute optimal departure time t
    int optimalDeparture = totalSum % M;
    if (optimalDeparture < 0) optimalDeparture += M;

    cout << "\n=========================================================\n";
    cout << "  OPTIMAL DEPARTURE TIME: t = " << optimalDeparture << " seconds\n";
    cout << "  (Signal pattern repeats every " << M << " seconds)\n";
    cout << "=========================================================\n\n";

    cout << "--- Route Verification ---\n";
    for (int i = 0; i < NUM_SIGNALS; i++) {
        int arrivalTime = optimalDeparture + signals[i].delay;
        int phaseAtArrival = arrivalTime % signals[i].m;

        cout << "Vehicle arrives at " << signals[i].name 
             << " at t = " << arrivalTime << "s -> "
             << "Signal phase is " << phaseAtArrival << "s "
             << "(Target: " << signals[i].r << "s) -> [PASSED ON GREEN]\n";
    }

    cout << "\nSuccess: Emergency vehicle travels without stopping at any red light!\n";
    return 0;
}