# Comparison of processor speed policies 

# Required: g++

## Go to folder "src"
cd src

## BUILD  : Compile in the folder "src":
make

## RUN    : Execute in the folder "src":
./prog

## OUTPUT : Display in the terminal 
1) Displaying of used features: size, deadline et inter-arrival time vectors.
2) Displaying of the Overconsumption results for (PACE), (HEUR) and (OA) policies in comparison with (MDP) policy.

Default case example for jobs of fixed deadline d=3, fixed inter-arrival time \tau=1, and size distribution is P({w = (1, 2, 3, 4)}) = {0.25,0.25,0.25,0.25}

## Other folder and files
- The video "pedestrian.mp4" has been used to test our algorithm with an edge detection program.
- Folder "scriptR" contains R script Section64_Rscript_ET.R to build Figures in Section 6.4.
- Folder "output" contains a summary of results after simulation in addition to the terminal output (used for R graphs).

## In src folder

### File SimulationDataSetArticle.hpp 
It gathers all the possible configurations drawn in the article (registered by Section). In function SimulationParameter(), you can choose the simulation you want to display. By default, the CasSpec is chosen, which corresponds to jobs of fixed deadline d=3, fixed inter-arrival time \tau=1, and size distribution is P({w = (1, 2, 3, 4)}) = {0.25,0.25,0.25,0.25}. 

### File main.cpp
- MDP resolution with the state space (or reduced state space if needed) with Value Iteration algorithm [Value_iteration() function in VI_MDP.cpp]

- Online simulation 
1. For MDP [Simu_ApplyMDPFromsimulationVI(), VersionReduced_Simu_ApplyMDPFromsimulationVI() functions in CoarseFineEquivalent.cpp]
    - Iteration on T time steps
        - Simulate a task set according to the probabilities as long as there is enough space in the buffer.
        - Determine the speed to apply according to the MDP policy.
        - Store task in a vector. 
        
2. Apply {OA, PACE, HEUR} on the vector tasks [SimuOA_onMDP_VraiVecteur(), SimuPACE_onMDP_VraiVecteur(), SimuHEUR_onMDP_VraiVecteur() in CoarseFineEquivalent.cpp].
