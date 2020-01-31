/*
 * FEMeval.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: eardi
 */


#define R_VERSION_

#include "fdaPDE.h"
//#include "IO_handler.h"
#include "mesh_objects.h"
#include "mesh.h"
#include "evaluator.h"
#include "projection.h"

extern "C" {
//! This function manages the various option for the solution evaluation.
/*!
	This function is then called from R code.
	Calls the walking algoritm for efficient point location inside the mesh in 2D.

	\param Rmesh an R-object containg the output mesh from Trilibrary
	\param Rlocations an R-matrix (seen as an array) containing the xyz coordinates of the points where the solution has to be evaluated
	\param RincidenceMatrix an R-matrix for the incidence matrix defining the regions in the case of areal data
	\param Rcoef an R-vector the coefficients of the solution
	\param Rorder an R integer containg the order of the solution
	\param Rfast an R integer 0 for Naive location algorithm, 1 for Walking Algorithm (can miss location for non convex meshes)
*/


SEXP eval_FEM_fd(SEXP Rmesh, SEXP Rlocations, SEXP RincidenceMatrix, SEXP Rcoef, SEXP Rorder, SEXP Rfast, SEXP Rmydim, SEXP Rndim, SEXP Rsearch)
{
	int n_X = INTEGER(Rf_getAttrib(Rlocations, R_DimSymbol))[0];
	int nRegions = INTEGER(Rf_getAttrib(RincidenceMatrix, R_DimSymbol))[0];
	int nElements = INTEGER(Rf_getAttrib(RincidenceMatrix, R_DimSymbol))[1]; //number of triangles/tetrahedron if areal data
	//Declare pointer to access data from C++
	double *X, *Y, *Z;
	UInt **incidenceMatrix;
	double *coef;
	int order, mydim, ndim, search;
	bool fast;

	coef  = REAL(Rcoef);
    order = INTEGER(Rorder)[0];
    fast  = INTEGER(Rfast)[0];
    mydim = INTEGER(Rmydim)[0];
    ndim  = INTEGER(Rndim)[0];
	search  = INTEGER(Rsearch)[0];

	X = (double*) malloc(sizeof(double)*n_X);
	Y = (double*) malloc(sizeof(double)*n_X);
	Z = (double*) malloc(sizeof(double)*n_X);
	incidenceMatrix = (UInt**) malloc(sizeof(UInt*)*nRegions);

    // Cast all computation parameters
	if (ndim==3)
	{
		for (int i=0; i<n_X; i++)
		{
			X[i] = REAL(Rlocations)[i + n_X*0];
			Y[i] = REAL(Rlocations)[i + n_X*1];
			Z[i] = REAL(Rlocations)[i + n_X*2];
		}
	}
	else //ndim==2
	{
		for (int i=0; i<n_X; i++)
		{
			X[i] = REAL(Rlocations)[i + n_X*0];
			Y[i] = REAL(Rlocations)[i + n_X*1];
			Z[i] = 0;
		}
	}
	for (int i=0; i<nRegions; i++)
	{
		incidenceMatrix[i] = (UInt*) malloc(sizeof(UInt)*nElements);
		for (int j=0; j<nElements; j++)
		{
			incidenceMatrix[i][j] = INTEGER(RincidenceMatrix)[i+nRegions*j];
		}
	}

    SEXP result;

	if (n_X>0) //pointwise data
	{
		PROTECT(result = Rf_allocVector(REALSXP, n_X));
		std::vector<bool> isinside(n_X);
		//Set the mesh
		if(order==1 && mydim==2 && ndim==2)
		{
			MeshHandler<1,2,2> mesh(Rmesh);
			Evaluator<1,2,2> evaluator(mesh);
			evaluator.eval(X, Y, n_X, coef, fast, REAL(result), isinside, search);

			//APPPPPLLLYYYY SAME WITH fdaPDE.cpp
			//********************component of the mesh
			// std::cout << "FEMeval.cpp " <<  std::endl;
			// std::cout << "Tree_Header components: " <<  std::endl;
			// std::cout << "gettreeloc : " << mesh.getTree().gettreeheader().gettreeloc() <<  std::endl; //this seems to be the number of elements
			// std::cout << "gettreelev : " << mesh.getTree().gettreeheader().gettreelev() <<  std::endl;
			// std::cout << "getndimp : " << mesh.getTree().gettreeheader().getndimp() <<  std::endl;
			// std::cout << "getndimt : " << mesh.getTree().gettreeheader().getndimt() <<  std::endl;
			// std::cout << "getnele : " << mesh.getTree().gettreeheader().getnele() <<  std::endl;
			// std::cout << "getiava : " << mesh.getTree().gettreeheader().getiava() <<  std::endl;
			// std::cout << "getiend : " << mesh.getTree().gettreeheader().getiend() <<  std::endl;
			// std::cout << "Domain (Tree_Header components) : " <<  std::endl;
			// for (int i=0;i<2;i++) {
			// 	std::cout << "domainorig : " << mesh.getTree().gettreeheader().domainorig(i) <<  std::endl;
			// 	std::cout << "domainscal : " << mesh.getTree().gettreeheader().domainscal(i) <<  std::endl;
			// }

			// std::cout<< "Tree_Node components: " <<  std::endl;
			// for (int i=0; i<10; i++) {
			// 	std::cout << "getid : " << mesh.getTree().gettreenode(i).getid() <<  std::endl;
			// 	std::cout << "getchild(0) : " << mesh.getTree().gettreenode(i).getchild(0) <<  std::endl;
			// 	std::cout << "getchild(1) : " << mesh.getTree().gettreenode(i).getchild(1) <<  std::endl;
				
			// 	std::cout << "Box (Tree_Node components) : " <<  std::endl;
			// 	std::cout << "getbox min x coord: " << mesh.getTree().gettreenode(i).getbox().get()[0]  <<  std::endl;
			// 	std::cout << "getbox min y coord : " << mesh.getTree().gettreenode(i).getbox().get()[1]  <<  std::endl;
			// 	std::cout << "getbox max x coord: " << mesh.getTree().gettreenode(i).getbox().get()[2]  <<  std::endl;
			// 	std::cout << "getbox max y coord : " << mesh.getTree().gettreenode(i).getbox().get()[3]  <<  std::endl;
			// }
			//********************component of the mesh			
		}
		else if(order==2 && mydim==2 && ndim==2)
		{
			MeshHandler<2,2,2> mesh(Rmesh);
			Evaluator<2,2,2> evaluator(mesh);
			evaluator.eval(X, Y, n_X, coef, fast, REAL(result), isinside, search);
		}
		else if(order==1 && mydim==2 && ndim==3)
		{ 
			MeshHandler<1,2,3> mesh(Rmesh);
			Evaluator<1,2,3> evaluator(mesh);
			evaluator.eval(X, Y, Z, n_X, coef, fast, REAL(result), isinside, search);
		}
		else if(order==2 && mydim==2 && ndim==3)
		{
			MeshHandler<2,2,3> mesh(Rmesh);
			Evaluator<2,2,3> evaluator(mesh);
			evaluator.eval(X, Y, Z, n_X, coef, fast, REAL(result), isinside, search);
		}
		else if(order==1 && mydim==3 && ndim==3)
		{ 
			MeshHandler<1,3,3> mesh(Rmesh);
			Evaluator<1,3,3> evaluator(mesh);
			evaluator.eval(X, Y, Z, n_X, coef, fast, REAL(result), isinside, search);
		}

		for (int i=0; i<n_X; ++i)
		{
			if(!(isinside[i]))
			{
				REAL(result)[i]=NA_REAL;
			}
		}
	}
	else //areal data
	{
		PROTECT(result = Rf_allocVector(REALSXP, nRegions));
		if(order==1 && mydim==2 && ndim==2)
		{
			MeshHandler<1,2,2> mesh(Rmesh);
			Evaluator<1,2,2> evaluator(mesh);
			evaluator.integrate(incidenceMatrix, nRegions, nElements, coef, REAL(result));
		}
		else if(order==2 && mydim==2 && ndim==2)
		{
			MeshHandler<2,2,2> mesh(Rmesh);
			Evaluator<2,2,2> evaluator(mesh);
			evaluator.integrate(incidenceMatrix, nRegions, nElements, coef, REAL(result));
		}
		else if(order==1 && mydim==2 && ndim==3)
		{ 
			MeshHandler<1,2,3> mesh(Rmesh);
			Evaluator<1,2,3> evaluator(mesh);
			evaluator.integrate(incidenceMatrix, nRegions, nElements, coef, REAL(result));
		}
		else if(order==2 && mydim==2 && ndim==3)
		{
			MeshHandler<2,2,3> mesh(Rmesh);
			Evaluator<2,2,3> evaluator(mesh);
			evaluator.integrate(incidenceMatrix, nRegions, nElements, coef, REAL(result));
		}
		else if(order==1 && mydim==3 && ndim==3)
		{ 
			MeshHandler<1,3,3> mesh(Rmesh);
			Evaluator<1,3,3> evaluator(mesh);
			evaluator.integrate(incidenceMatrix, nRegions, nElements, coef, REAL(result));

		}
	}

	free(X); free(Y); free(Z);
	for (int i=0; i<nRegions; i++)
	{
		free(incidenceMatrix[i]);
	}
	free(incidenceMatrix);
	

	UNPROTECT(1);
    // result list
    return(result);
}


SEXP data_projection(SEXP Rmesh, SEXP Rlocations)
{
	int n_X = INTEGER(Rf_getAttrib(Rlocations, R_DimSymbol))[0];
	//Declare pointer to access data from C++
	double X, Y, Z;

    // Cast all computation parameters
    std::vector<Point> deData_(n_X); // the points to be projected
    std::vector<Point> prjData_(n_X); // the projected points
    
    for (int i=0; i<n_X; i++)
	{
		X = REAL(Rlocations)[i + n_X*0];
		//Rprintf("X[%i]= %d", i, X[i]);
		Y = REAL(Rlocations)[i + n_X*1];
		Z = REAL(Rlocations)[i + n_X*2];
		deData_[i]=Point(X,Y,Z);
	}

    SEXP result;

	if (n_X>0) //pointwise data
	{
		PROTECT(result = Rf_allocMatrix(REALSXP, n_X, 3));
		
		//order: 1
		MeshHandler<1,2,3> mesh(Rmesh);
		projection<1,2,3> projector(mesh, deData_);
		prjData_ = projector.computeProjection();
	}

	for (int i=0; i<n_X; ++i)
	{
		REAL(result)[i + n_X*0]=prjData_[i][0];
		REAL(result)[i + n_X*1]=prjData_[i][1];
		REAL(result)[i + n_X*2]=prjData_[i][2];
	}

	UNPROTECT(1);
    // result matrix
    return(result);
}

}

