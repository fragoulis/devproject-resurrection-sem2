//*****************************************************************************
//**
//**   File:               utils.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include <string>
#include <sstream>
#include <assert.h>
#include <gl/glee.h>

#ifdef _DEBUG
#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)
#else
#define CHECK_GL_ERROR() 
#endif



std::string getExtension(const char *fname);	// Gets a filename's extension
std::string truncDir(const std::string& full);
bool isPOT(const unsigned val);		// Checks if the value is a power of 2
unsigned nearestPOT(const unsigned number);
int CheckGLError(char *file, int line);	// Checks for active GL errors

unsigned componentsByInternalFormat(const GLuint ifmt);
unsigned BytesPerComponentByDatatype(const GLuint dtype);

#include "../Misc/utils.hpp"

