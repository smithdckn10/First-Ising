#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <time.h>
#include <algorithm>

#define L 3
#define SIZE L*L
#define SWEEPS 10000
#define J 0.25    //If J < 0, then the model is a ferromagnet, if J > 0, then the model is an anti-ferromagnet, but I've got negative signs in the code right now so don't make it negative here

using namespace std;

//create the array of "particles",


//This initializes the array with all the spins pointing up
void orderedinit(int spins[SIZE]);

//This initializes the array with random spins
void randominit(int spins[SIZE]);


//Get values for neighborhood of each site
void neighborhood(int size[SIZE], int neighbors[SIZE*4]);

//Now this one is seriously silly
int magnetize(int spins[SIZE]);

//This outputs the spins to terminal for testing
void output(int spins[SIZE]);

//Checks if a lattice site should be flipped, and returns true or false
bool checkflip(int spins[SIZE], int pos);

double getTotEnergy(int spins[SIZE], int neighbors[SIZE*4]);

double getChangeLocEnergy(int spins[SIZE], int pos);



int main(){
    //seeds the random number generator
    srand((unsigned) time(NULL));


    //Stores spins of each lattice site, with 1 meaning up, 0 meaning down
    int spins[SIZE];

    //Stores spins of each site's neighbors, so you can calc probability of whether or not the site will flip
    int neighbors[SIZE * 4];

   
    //Energy of the system, I'll prob create a class that calcs this given an array of spins
    double energy;

    //magnetic moment of a spin, usually set to 1 for simplicity, not implemented
    int moment = 1;

    //Total magnetization of the system, not implemented
    double magnet = 0;

    //Boolean that decides whether or not the spin is flipped
    bool YoN = 0;

    //Array that stores the total energy of the system after each sweep
    double Energies[SWEEPS];
    double avgE = 0;


    /* I havent implemented this, Im just using the built-in c++ rand() function
    //This just sets up the stuff to get the random numbers, but I'm gonna use the rand() functions to start
    mt19937 gen(958431198); //Mersenne Twister RNG
	uniform_int_distribution<int> brandom(0, 1); //Get any random integer
	uniform_int_distribution<int> ran_pos(0, SIZE-1); //Get any random integer
	uniform_real_distribution<double> ran_u(0.0, 1.0); //Our uniform variable generator
    */

    

    // This sets the spin values to 1 
      orderedinit(spins);

    //This initializes the spins randomly
     // randominit(spins);

    cout << endl << "LATTICE: " << endl;
    
    //This outputs the sites in terminal
    output(spins);

    neighborhood(spins, neighbors);

    cout << "Total Energy: " << getTotEnergy(spins, neighbors) << endl << endl;

    for(int sweeps = 0; sweeps < SWEEPS; sweeps++){
        for(int i = 0; i < SIZE; i++){
            YoN = checkflip(spins, i);
            if(YoN == true){
                spins[i] = spins[i] * -1;
                
            }
        }
        neighborhood(spins, neighbors);
        Energies[sweeps] = getTotEnergy(spins, neighbors);
        avgE = avgE + Energies[sweeps];

        /* This section turns on the output to terminal

        cout << "Sweeps: " << sweeps + 1 << " , New Lattice: " << endl; 
        output(spins);
        cout << "New Total Energy: " << Energies[sweeps] << endl;
        */
    }
    avgE = avgE / SWEEPS;
    cout << "Sweeps performed: " << SWEEPS << endl;
    cout << "Average Total Energy at the end of a Sweep: " << avgE << endl;
    cout << "Last Lattice: " << endl;
    output(spins);
    return 0;

}

void orderedinit(int spins[SIZE]){
    for(int i = 0; i < SIZE; i++){
        spins[i] = 1;
    }
}

void randominit(int spins[SIZE]){
    int rando = 0;
    for(int i = 0; i < SIZE; i++){
        rando = rand() % 2;
        if(rando == 1){
            spins[i] = 1;
        }
        else{
            spins[i] = -1;
        }
    }
}

bool checkflip(int spins[SIZE], int pos){
    double prob = 0;
    double Beta = -0.25;
    double dE = getChangeLocEnergy(spins, pos);
    double Bweight = exp(-dE);
    double check = rand();
    check = check / RAND_MAX;
    //cout << "dE: " << dE << " , check: " << check << endl;
    prob = min(1.0, Bweight);
    if(check <= prob){
        return true;
    }
    else{
        return false;
    }

}

double getTotEnergy(int spins[SIZE], int neighbors[SIZE*4]){
    double Energy = 0; //Stores Total Energy/Hamiltonian I think?
    double extMagF = 0; //Contribution from external magnetic field
    double neighborF = 0; //Contribution from neighbor sites
    int B = 0; //External Magnetic Field
    for(int i = 0; i < SIZE; i++){
        neighborF = neighborF + spins[i] * neighbors[4*i]; //Get the top neigbor term
        neighborF = neighborF + spins[i] * neighbors[4*i + 3]; //Get the right neighbor term
        extMagF = extMagF - spins[i]; //Get the external magnet term 
    }    
    Energy = (- J * neighborF) + (B * extMagF);
    return Energy;
}

double getChangeLocEnergy(int spins[SIZE], int pos){
    //We need to find all the neighbors of the site to calc the energy, so uses code from the neighborhood subroutine
    int site = spins[pos];
    int antisite = site * -1;
    int top = 0;
    int bottom = 0;
    int left = 0;
    int right = 0;
    double change = 0;
    double before = 0;
    double after = 0;

    if(pos % L == 0){             //Check if its on the top side
            top = pos + L - 1;
    }
    else{
            top = pos - 1;
    }
    if(pos % L == L - 1){      //Check if its on the bottom side
            bottom = pos - L + 1;
    }
    else{
            bottom = pos+ 1;
    }
    if(pos < L){              //Check if its on the left
            left = SIZE - L + pos;
    }
    else{
            left = pos - L;
    }
    if(pos >= SIZE - L){              //Check if its on the right
            right = pos % L;
    }
    else{
            right = pos + L;
    }
    //Now that we have the positions of the neighbors, we can calc the local energy (the energy of this site and its neighbors only)
    //FIX THISSSSSS
    before = site * spins[top];
    after = antisite * spins[top];
    before = before + site * spins[bottom];
    after = after + antisite * spins[bottom];
    before = before + site * spins[left];
    after = after + antisite * spins[left];
    before = before + site * spins[right];
    after = after + antisite * spins[right];
    change = -J / 2 * ((after) - (before));

    return change;

}

void neighborhood(int spins[SIZE], int neighbors[SIZE*4]){
    int top = 0;
    int bottom = 0;
    int left = 0;
    int right = 0;
    int counter;

    for(int i = 0; i < SIZE; i++){
        
                 
        //Do the stuff to get periodic boundries, might make this a switch if it really matters

        if(i % L == 0){             //Check if its on the top side
            top = i + L - 1;
        }
        else{
            top = i - 1;
        }

        if(i % L == L - 1){      //Check if its on the bottom side
            bottom = i - L + 1;
        }
        else{
            bottom = i + 1;
        }

        if(i < L){              //Check if its on the left
            left = SIZE - L + i;
        }
        else{
            left = i - L;
        }

        if(i >= SIZE - L){              //Check if its on the right
            right = i % L;
        }
        else{
            right = i + L;
        }

        neighbors[4*i] = spins[top]; //Get the top neighbor
        neighbors[4*i + 1] = spins[bottom]; //Get the bottom neighbor
        neighbors[4*i + 2] = spins[left]; //Get the left neighbor
        neighbors[4*i + 3] = spins[right]; //Get the right neighbor


    }



}

void output(int spins[SIZE]){
    string space = "     ";
    for(int i = 0; i < L; i++){
        for(int j = 0; j < L; j++){
            if(4*j + i >= 10){
                space = "     ";    //take one space out of this if you enable the positions to show
            }
            else{
                space = "     ";
            }
            //cout << "[" << L *j + i << "]" << spins[i + L * j] << space; This just marks the position before the spins, not super necessary
            if(spins[i + L * j] > 0){
                cout << " ";
            }
            cout << spins[i + L * j] << space;
        }
        cout << endl;
    }
}

int magnetize(int spins[SIZE]){ //this is definitely not how you find the magnetization but this is where I'll put it once I figure that out
    int spinsum = 0;
    
    for(int i = 0; i < SIZE; i++){
        spinsum = spinsum + spins[i];
    }

    return spinsum;
}
