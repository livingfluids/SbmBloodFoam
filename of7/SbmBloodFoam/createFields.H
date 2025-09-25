    Info<< "Reading transportProperties\n" << endl;

    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar nu0
    (
        transportProperties.lookup("nu0")
    );
    dimensionedScalar epsilon
    (
        transportProperties.lookup("epsilon")
    );

    dimensionedScalar cm
    (
        transportProperties.lookup("cm")
    );

    dimensionedScalar alpha
    (
        transportProperties.lookup("alpha")
    );    


    dimensionedScalar a
    (
        transportProperties.lookup("a")
    );

    dimensionedScalar h0
    (
        transportProperties.lookup("h0")
    );

    dimensionedScalar H
    (
        transportProperties.lookup("H")
    );
    
    dimensionedScalar fv
    (
        transportProperties.lookup("fv")
    );
    dimensionedScalar beta
    (
        transportProperties.lookup("beta")
    ); 



    dimensionedTensor I
    (
    "I",
    dimensionSet(0, 0, 0, 0, 0, 0, 0),
    tensor(1,0,0,0,1,0,0,0,1)
    );

    Info<< "Reading field p\n" << endl;
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );



    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

Info<< "Reading field c\n" << endl;
    volScalarField c
    (
        IOobject
        (
            "c",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );


     Info << "Reading field J\n" << endl;
    volVectorField J
    (
        IOobject
        (
            "J",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field Q\n" << endl;
    volTensorField Q
    (
        IOobject
        (
            "Q",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
	mesh
    );
///////////////////////////////////////////////////////////////////////////////////////

    volScalarField Umx
    (
        IOobject
        (
            "Umx",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
	mesh,
	dimensionedScalar("Umx",dimensionSet(0,1,-1,0,0,0,0),scalar(0.0))
    );
    volScalarField gammadot
    (
        IOobject
        (
            "gammadot",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
	mesh,
	dimensionedScalar("gammadot",dimensionSet(0,0,-1,0,0,0,0),scalar(0.0))
    );

    volScalarField nu1
    (
        IOobject
        (
            "nu1",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        nu0 * pow( (1.0- (c/cm)), -2.0)
    );

    volScalarField nu2
    (
        IOobject
        (
            "nu2",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        0.75*nu0*sqr(c/cm)*pow( (1.0- (c/cm)), -2.0)
    );

    volScalarField fc
    (
        IOobject
        (
            "fc",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        (1.0-(c/cm))*pow((1.0-c),alpha-1.0)
    );


#   include "createPhi.H"

pressureReference pressureReference(p, piso.dict());

mesh.setFluxRequired(p.name());

#include "createMRF.H"
#include "createFvModels.H"
#include "createFvConstraints.H"
///////////////////////////////////////////////////////////




    volTensorField SigmaL
    (
        IOobject
        (
            "SigmaL",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
	mesh,
	dimensionedTensor("SigmaL",dimensionSet(0,2,-2,0,0,0,0),tensor::zero)
    );



    volTensorField SigmaR
    (
        IOobject
        (
            "SigmaR",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
	mesh,
	dimensionedTensor("SigmaR",dimensionSet(0,2,-2,0,0,0,0),tensor::zero)
    );



    volTensorField SigmaP
    (
        IOobject
        (
            "SigmaP",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
	mesh,
	dimensionedTensor("SigmaP",dimensionSet(0,2,-2,0,0,0,0),tensor::zero)
    );



    volTensorField Sigma
    (
        IOobject
        (
            "Sigma",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
	mesh,
	dimensionedTensor("Sigma",dimensionSet(0,2,-2,0,0,0,0),tensor::zero)
    );


    volTensorField SigmaF
    (
        IOobject
        (
            "SigmaF",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
	mesh,
	dimensionedTensor("SigmaF",dimensionSet(0,2,-2,0,0,0,0),tensor::zero)
    );

    //Addition of the vector field for the lift force
    volVectorField LiftF
    (
        IOobject
        (
            "LiftF",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
    mesh,
    dimensionedVector("LiftF",dimensionSet(0,1,-1,0,0,0,0),Foam::vector(0,0,0))
    );

/////////////////////////////////////////////////////////////////
