# SbmBloodFoam
The **SbmBloodFoam** is an *OpenFOAM*® application that solves the *modified suspension balance model* (MSBM) equations for the flow of red blood 
cells (RBCs) and other deformable particle suspensions in confined geometries. A lift force term "LiftF" and other hemorheology features
are incorporated into the conventional suspension balance model (SBM) on top of the *SbmFoam* solver (originally developed by Dbouk et. al.)
to create the **SbmBloodFoam** solver, which is able to predict both the transient and steady-state hematocrit and velocity profiles and the 
development of the cell-free layer characteristic of blood flows.

References:
1. Castillo-Sanchez, H. A., Tuna, R., Ortiz, W., Martin, R., Rao, R., Liu, Z. L.,
    ‘A Modified Suspension-Balance Model for Blood Flows Capturing the Cell Free Layer’,
    Submitted to PRL, September 2025.
2. Dbouk, T., Lobry, L., Lemaire, E. and Moukalled, F., ‘Shear-induced Particles 
    Migration. Predictions from Experimental Determination of The Particle Stress Tensor’, 
    Journal of Non-Newtonian fluid Mechanics, 198, pp. 78-95, August 2013.

The folder "of7" contains the files needed to run the **SbmBloodFoam** application in *OpenFOAM® version 7.0*. 
Additionaly, the folder named **SbmBloodTubeFoam** (an extension of the **SbmBloodFoam** application) was 
specifically created to simulate the modified suspension balance model in tubular geometries.

The original solver *SbmFoam* can be found here: https://openfoamwiki.net/index.php/Contrib/SbmFoam

## Pre-requisites

- OpenFOAM® v7.0
- OpenFOAM® v9.0

The OpenFOAM® environment must be sourced.

## Compiling the solver

After sourcing your OpenFOAM® environment, go directly to the **SbmBloodFoam** folder, and type in the terminal the following command:
```bash
wmake
```
which will compile all the files needed to run the solvers. This process is also valid if you want to compile the **SbmBloodTubeFoam** solver

## Running one tutorial case

Go to the tutorial folder and open the folder of the flow case of your choice (_2DSimpleShearFlow_blood_ or _2DsuspensionFlowInChannel_blood_). 
Open a terminal and type the following command:
```bash
blockMesh
```
which will create the computational mesh with the dimensions specified in the _blockMeshDict_ file (found in the _system_ folder). To run the case, 
simply type the name of the solver:
```bash
SbmBloodFoam
```
The time-step and data input/output control is specified in the _controlDict_, which is also found in the _system_ folder. More information regarding 
the tutorial cases can be found in their individual README.md files.

For more information, contact

Dr. Hugo A. Castillo-Sanchez \
hc25o@fsu.com \
hugocastillocom.wordpress.com

This offering is not approved or endorsed by OpenCFD Limited, producer and distributor of the OpenFOAM software via www.openfoam.com, 
and owner of the OpenFoam® and OpenCFD® trade marks.

OpenFoam® is a registered trade mark of OpenCFD Limited, producer and distributor of the OpenFOAM software via www.openfoam.com
