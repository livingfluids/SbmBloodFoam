# SbmBloodFoam
The **SbmBloodFoam** is an *OpenFOAM*® application that solves the *modified suspension balance model* (MSBM) equations for the flow of red blood 
cells (RBCs) and other deformable particle suspensions in confined geometries. A lift force term "LiftF" and other hemorheology features
are incorporated into the conventional suspension balance model (SBM) on top of the *SbmFoam* solver (originally developed by Dbouk et. al.)
to create the **SbmBloodFoam** solver, which is able to predict both the transient and steady-state hematocrit and velocity profiles and the 
development of the cell-free layer characteristic of blood flows.

References:
1. Castillo-Sanchez, H. A., Ortiz, W., Martin, R., Tuna, R., Rao, R., Liu, Z. L.,
    ‘_A Modified Suspension-Balance Model for Blood Flows Capturing the Cell Free Layer_’,
    Submitted to PRL, November 2025.
2. Dbouk, T., Lobry, L., Lemaire, E. and Moukalled, F., ‘_Shear-induced Particles 
    Migration. Predictions from Experimental Determination of The Particle Stress Tensor_’, 
    Journal of Non-Newtonian Fluid Mechanics, 198, pp. 78-95, August 2013.

The folder "of9" contains the files needed to run the **SbmBloodFoam** application in *OpenFOAM® version 9.0*. 
Additionaly, the folder named **SbmBloodTubeFoam** (an extension of the **SbmBloodFoam** application) was 
specifically created to simulate the modified suspension balance model in tubular geometries.

The original solver *SbmFoam* can be found here: https://openfoamwiki.net/index.php/Contrib/SbmFoam

## Pre-requisites

- OpenFOAM® v9.0 for the solvers created in this version (folder of9).

The OpenFOAM® environment must be sourced before compiling the solver files.

## Compiling the solver

After sourcing your OpenFOAM® environment, go directly to the **SbmBloodFoam** folder, and type in a terminal the following command:
```bash
wmake
```
which will compile all the files needed to run the solvers. This process is also valid if you want to compile the **SbmBloodTubeFoam** solver: 
go to the _SbmBloodTubeFoam_ folder and type _wmake_. 

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

If you are running the tubular flow case, run the following command instead:
```bash
SbmBloodTubeFoam
```

## Running a case in parallel

After creating the mesh using the _blockMesh_ command (described in the previous section), type the following command:
```bash
decomposePar
```
which will decompose the mesh and fields of a case for parallel execution. The geometry and fields are broken up according to a set of parameters that are 
in the file named _decomposeParDict_ that is located in the _system_ directory of the case of interest. For a more detailed information about the _decomposeParDict_
directory, see https://doc.cfd.direct/openfoam/user-guide-v13/running-applications-parallel.

To run the simulation in parallel, type this command:
```bash
mpirun -np N SbmBloodFoam -parallel
```
where _N_ is the number of processors, which must coincide with the number of subdomains specified in the _decomposeParDict_ directory. If the **SbmBloodTubeFoam**
solver is being used, use the command below instead:
```bash
mpirun -np N SbmBloodTubeFoam -parallel
```
To reconstruct the mesh and the fields of a parallel case at the latest time of the simulation, type the following command:
```bash
reconstructPar -latestTime
```

## Monitoring the residuals

The residuals of the simulations can be monitored on real time by typing the following command:
```bash
foamMonitor -l ./postProcessing/residuals/0/residuals.dat
```
OpenFOAM will show in a plot the residuals of the flow variables (in log scale) vs the simulation time. The flow variables that will be shown by default 
are the components of the velocity field, the pressure and the volume fraction. However, you can also include other variables (such as stresses, shear rate,
etc) in the _residuals.dat_ file that can be found in the _system_ folder.

The command above works if the simulation started at time 0. But if the simulation is being restarted at a given time *T*, run the following command:
```bash
foamMonitor -l ./postProcessing/residuals/T/residuals.dat
```

## Postprocessing results using sampleDict

The flow profiles (i.e. volume fraction, velocity field and shear rate) across the geometry can be obtained at given locations in the geometry. This information
is specified in the _sampleDict_ file (found in the _system_ folder). To get these profiles at the latest time of the simulation, type the following command:
```bash
postProcess -func sampleDict -latestTime
```
This will create an output folder that will be located in the _postProcessing/sampleDict/_ directory. If the latest time of the simulation is 10, the output folder
will be named _10_. Within this folder, files with .xy format will contain the geometry coordinates in the first column, and the flow profiles in the subsequent columns.

In the tutorial cases, the profiles will be obtained close to the outlet, from the bottom to the top wall (for the channel geometry) and from the centerline to the wall (for the tubular geometry). 

## Screenshots

Here are some results that were generated using our solvers:
![MSBMProf](https://github.com/livingfluids/SbmBloodFoam/blob/eca236d2893c06c8a21d5573daad1687de5d66fa/MSBM.jpg) 

For more information, contact

Dr. Hugo A. Castillo-Sanchez \
hc25o@fsu.com \
hugocastillocom.wordpress.com

This offering is not approved or endorsed by OpenCFD Limited, producer and distributor of the OpenFOAM software via www.openfoam.com, 
and owner of the OpenFoam® and OpenCFD® trade marks.

OpenFoam® is a registered trade mark of OpenCFD Limited, producer and distributor of the OpenFOAM software via www.openfoam.com
