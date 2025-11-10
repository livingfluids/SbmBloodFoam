/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2015 ESI-OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.


Application
    SbmBloodTubeFoam

Description
    The SbmBloodTubeFoam application solves the modified suspension balance model (MSBM) equations for the flow of red blood cells
(RBCs) and other deformable particle suspensions in confined tubular geometries. A lift force term "LiftF" and other hemorheology features
are incorporated into the conventional suspension balance model (SBM) on top of the SbmFoam solver (originally developed by Dbouk et. al.)
to create the SbmBloodFoam solver, which is able to predict both the transient and steady-state hematocrit and velocity profiles and 
the development of the cell-free layer characteristic of blood flows in tubular geometries. 

References:
    1. Castillo-Sanchez, H. A., Tuna, R., Ortiz, W., Martin, R., Rao, R., Liu, Z. L.,
    ‘A Modified Suspension-Balance Model for Blood Flows Capturing the Cell Free Layer’,
    Submitted to PRL, September 2025.
    2. Dbouk, T., Lobry, L., Lemaire, E. and Moukalled, F., ‘Shear-induced Particles 
    Migration. Predictions from Experimental Determination of The Particle Stress Tensor’, 
    Journal of Non-Newtonian fluid Mechanics, 198, pp. 78-95, August 2013.


Application Written by:

Dr. Hugo A. Castillo-Sanchez
hc25o@fsu.com
hugocastillocom.wordpress.com

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "pisoControl.H"
#include "pressureReference.H"
#include "fvModels.H"
#include "fvConstraints.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "postProcess.H"
#   include "setRootCaseLists.H"
#   include "createTime.H"
#   include "createMesh.H"
#   include "createControl.H"
#   include "createFields.H"
#   include "initContinuityErrs.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

#       include "CourantNo.H"

//////////////////////////////////////////////////////////////////////////////

volTensorField Gr=fvc::grad(U); //Velocity gradient
volTensorField E = (symm(Gr))&I; //Strain rate tensor
gammadot = sqrt( (2.0*(E && E)) ); // shear rate


//nu1=nu0 * pow( (1.0- (c/cm)), -2.0); // suspension viscosity
nu1=nu0 * pow( (1.0- (c/cm)), -1.82); // suspension viscosity
nu2=0.75*nu0*sqr(c/cm)*pow( (1.0- (c/cm)), -2.0);
fc=(1.0-(c/cm))*pow((1.0-c),alpha-1.0); // Sedimentation Hindrance function


Umx=max(max(U.component(0),U.component(1)),U.component(2)); // For correction purposes

SigmaL = -nu2 * ( gammadot+ (epsilon*Umx) ) * Q; //Normal components of the particle stress tensor
SigmaR = 2.0 * (nu1 - nu0)  * E; //Shear components of the particle stress tensor

SigmaF = (-p*I) + (2.0*nu0*E); //Pressure + solvent contribution to the total stress tensor

SigmaP = (SigmaL + SigmaR); //Particle stress tensor

Sigma = SigmaP + SigmaF; // Total stress tensor
 
//Lift force term (two-wall effect)
LiftF = (1.0*((2.0*sqr(a))/(9.0*nu0))*fc* fv * ((3.0* nu0 *gammadot)/(4.0 * 3.14159))*c *Foam::vector(0,1,0))/( pow( dimensionedScalar("one",dimensionSet(0,-1,0,0,0,0,0),scalar(1.0))*(H + mesh.C().component(1)+h0), beta) );
LiftF -= (1.0*((2.0*sqr(a))/(9.0*nu0))*fc* fv * ((3.0* nu0 *gammadot)/(4.0 * 3.14159))*c *Foam::vector(0,1,0))/(pow( dimensionedScalar("one",dimensionSet(0,-1,0,0,0,0,0),scalar(1.0))*(H - mesh.C().component(1)+h0), beta) );


// Migration Flux Vector for the MSBM
J = ((2.0*sqr(a))/(9.0*nu0))*fc*fvc::div(SigmaP) + LiftF;

//*********************************************************************************************//       

        // Pressure-velocity SIMPLE corrector
        while (piso.correct())
        {

            // Momentum predictor
            fvModels.correct();
            MRF.correctBoundaryVelocity(U);

            fvVectorMatrix UEqn
            (
    
               - fvm::laplacian(nu1, U) -fvc::div(nu1*dev(fvc::grad(U)().T())) -fvc::div(SigmaL)
            );

            UEqn.relax();
            //fvOptions.constrain(UEqn);
            fvConstraints.constrain(UEqn);

            if (piso.momentumPredictor())
            {
                solve(UEqn == -fvc::grad(p));
                //fvOptions.correct(U);
                fvConstraints.constrain(U);
            }

            //p.boundaryField().updateCoeffs();
            volScalarField rAU(1.0/UEqn.A());
            volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));
            surfaceScalarField phiHbyA
            (
                "phiHbyA",
                fvc::flux(HbyA)
                + MRF.zeroFilter(fvc::interpolate(rAU)*fvc::ddtCorr(U, phi))
                //+ fvc::interpolate(rAU)*fvc::ddtCorr(U, phi)
            );

            MRF.makeRelative(phiHbyA);
            adjustPhi(phiHbyA, U, p);

            // Update the pressure BCs to ensure flux consistency
            constrainPressure(p, U, phiHbyA, rAU, MRF);

            // Store pressure for under-relaxation
            p.storePrevIter();

            // Non-orthogonal pressure corrector loop
            while (piso.correctNonOrthogonal())
            {
                // Pressure corrector
                fvScalarMatrix pEqn
                (
                    fvm::laplacian(rAU, p) == fvc::div(phiHbyA)
                );
                
                //pEqn.setReference(pRefCell, pRefValue);
                pEqn.setReference
                (
                    pressureReference.refCell(),
                    pressureReference.refValue()
                );
                //pEqn.solve(mesh.solver(p.select(piso.finalInnerIter())));
                pEqn.solve();
            
                if (piso.finalNonOrthogonalIter())
                {
                    phi = phiHbyA - pEqn.flux();
                }
            }
            #include "continuityErrs.H"

            // Explicitly relax pressure for momentum corrector
            p.relax();
            U = HbyA - rAU*fvc::grad(p);
            U.correctBoundaryConditions();
            //fvOptions.correct(U);
            fvConstraints.constrain(U);
        }



///////////////  concentration Evolution Equation ///////////////////////////

	fvScalarMatrix cEqn
        (
            
          fvm::ddt(c) + fvm::div(phi,c)
       
        );
	    

		solve (  cEqn == -fvc::div(J)  );




	    	  
///////////////////////////////////////////////////////////////////////////////////

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return(0);
}


// ************************************************************************* //
