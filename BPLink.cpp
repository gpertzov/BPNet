// BPLink.cpp: implementation of the BPLink class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "BPNode.h"
#include "BPLink.h"

// utility function
static double random( double low, double high )
{
	return ((double) rand() / RAND_MAX) * (high-low) + low;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// init static member
int BPLink::_idCounter = -1;


BPLink::BPLink() :	_id(++_idCounter),
					_delta(0),
					_pInNode(NULL),
					_pOutNode(NULL)
{
	initWeight();
}



BPLink::~BPLink()
{

}


//====================================================================
// Initialize weight
//====================================================================
void BPLink::initWeight()
{
	// init weight to random value between -1 and 1
	_weight = random( -1.0, 1.0 );
}


//====================================================================
// Connect two nodes
//====================================================================
void BPLink::connect(BPNode* inNode, BPNode* outNode)
{
	assert(inNode != NULL && outNode != NULL);

	// set in/out node pointers
	_pInNode	= inNode;
	_pOutNode	= outNode;
	
	// add this link to the output-links vector of the input node
	_pInNode->addOutLink(this);

	// add this link to the input-links vector of the output node
	_pOutNode->addInLink(this);
}


//====================================================================
// Save link data to file
//====================================================================
bool BPLink::save( ofstream &ost ) const
{
	assert( _pInNode != NULL && _pOutNode != NULL);

	if ( !ost.good() )
		return false;

	ost << setw(4) << _id <<  " " << setprecision(16) // id
		<< _weight << " " << setprecision(16)		  // weight
		<< _delta  << " " << setw(4)				  // delta
		<< _pInNode->id() << " "					  // in	 node ID
		<< setw(4) << _pOutNode->id() << endl;		  // out node ID


	return true;
}


//====================================================================
// Load link data from file
//====================================================================
bool BPLink::load( ifstream &ist )
{
	if ( !ist.good() )
		return false;

	ist >> _id;		// id
	ist >> _weight;	// weight
	ist >> _delta;	// delta

	// currently these values are not used.
	// the link connections are NOT changed on load
	int inNodeId, outNodeId;
	ist >> inNodeId;	// in  node ID
	ist >> outNodeId;	// out node ID		

	return true;
}