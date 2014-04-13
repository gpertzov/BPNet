// Pattern.cpp: implementation of the Pattern class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////
#include <cstdarg>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "Pattern.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Pattern::~Pattern()
{

}


Pattern::Pattern() : _id(-1)
{

}


//====================================================================
// Constructor with pattern dimensions supplied
//====================================================================
Pattern::Pattern(int inSize, int outSize) : _id(-1)
{
	assert (inSize > 0 && outSize > 0);

	_inVec.resize(inSize);
	_outVec.resize(outSize);	
}


//====================================================================
// Constructor with pattern data supplied
//====================================================================
Pattern::Pattern( int inSize, int outSize, int id, ... )
{
	assert (inSize > 0 && outSize > 0);

	_inVec.resize(inSize);
	_outVec.resize(outSize);

	_id = id;
	
	va_list vl;

	va_start(vl, id );
	
	for (int i = 0; i != inSize; ++i)
	   _inVec[i] = va_arg(vl, double);

	for ( i = 0; i  != outSize; ++i)
	   _outVec[i] = va_arg(vl, double);

	va_end( vl );
}


//====================================================================
// Get input value
//====================================================================
double Pattern::getInput(int index) const
{
	assert( index >= 0 && index < _inVec.size() );

	return _inVec[index];
}


//====================================================================
// Get output value
//====================================================================
double Pattern::getOutput(int index) const
{
	assert( index >= 0 && index < _outVec.size() );

	return _outVec[index];

}


//====================================================================
// Set input value
//====================================================================
void Pattern::setInput(double value, int index)
{
	assert( index >= 0 && index < _inVec.size() );

	_inVec[index] = value;
}


//====================================================================
// Set output value
//====================================================================
void Pattern::setOutput(double value, int index)
{
	assert( index >= 0 && index < _outVec.size() );

	_outVec[index] = value;

}


//====================================================================
// Save pattern to file
//====================================================================
bool Pattern::save( ofstream &ost ) const
{
	if ( !ost.good() )
		return false;

	ost << _id << "\t"; // pattern id

	int numInputs = _inVec.size();
	for(int i = 0; i != numInputs; ++i)
		ost <<  setprecision(16) << _inVec[i] << "\t"; // input values

	int numOutputs = _outVec.size();
	for(i = 0; i != numOutputs; ++i)
	{
		ost <<  setprecision(16) << _outVec[i]; // output values
		if ( i != numOutputs-1) 
			 ost << "\t";
	}

	ost << endl;

	return true;
}


//====================================================================
// Load pattern from file
//====================================================================
bool Pattern::load( ifstream &ist )
{
	if ( !ist.good() )
		return false;

	ist >> _id; // pattern id

	int numInputs = _inVec.size();
	for(int i = 0; i != numInputs; ++i)
		ist >> _inVec[i]; // input values

	int numOutputs = _outVec.size();
	for(i = 0; i != numOutputs; ++i)
		ist >> _outVec[i]; // output values

	// "eat" white space
	char ch = ist.peek();
	while ( isspace(ch) )
	{
		ch = ist.get();
		if (ch == EOF)
			break;

		ch = ist.peek();
	}

	return true;
}
