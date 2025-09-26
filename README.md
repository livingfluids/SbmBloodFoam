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
Additionaly, the folder named **SbmBloodTubeFoam** is an extension of the **SbmBloodFoam** application 
specifically created to simulate the modified suspension balance model in tubular geometries.
The original solver *SbmFoam* can be found here: https://openfoamwiki.net/index.php/Contrib/SbmFoam

For more information, contact

Dr. Hugo A. Castillo-Sanchez \
hc25o@fsu.com \
hugocastillocom.wordpress.com

This offering is not approved or endorsed by OpenCFD Limited, producer and distributor of the OpenFOAM software via www.openfoam.com, 
and owner of the OpenFoam® and OpenCFD® trade marks.

OpenFoam® is a registered trade mark of OpenCFD Limited, producer and distributor of the OpenFOAM software via www.openfoam.com
