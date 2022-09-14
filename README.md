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

### File SimulationDataSetArticle.hpp gathers all the possible configurations drawn in the article (registered by Section). In function SimulationParameter(), you can choose the simulation you want to display. By default, the CasSpec is chosen, which corresponds to jobs of fixed deadline d=3, fixed inter-arrival time \tau=1, and size distribution is P({w = (1, 2, 3, 4)}) = {0.25,0.25,0.25,0.25}. 

### File main
- MDP resolution with the state space (or reduced state space if needed) 
Value Iteration algorithm [Value_iteration() function in VI_MDP.cpp]

- Online simulation 
1. For MDP [Simu_ApplyMDPFromsimulationVI(), VersionReduced_Simu_ApplyMDPFromsimulationVI() functions in CoarseFineEquivalent.cpp]
    - Iteration on T iterations
        - Create task if place in the buffer according to the probabilities.
        - Simulate task for MDP under the true distribution
            - function reduce(WCET,WCETsimu) to determine from the true distribution the reduce value of the size if needed.
        - Store task in vector 
        
2. Apply {OA, PACE, HEUR} on the vector task [SimuOA_onMDP_VraiVecteur(), SimuPACE_onMDP_VraiVecteur(), SimuHEUR_onMDP_VraiVecteur() in CoarseFineEquivalent.cpp].
