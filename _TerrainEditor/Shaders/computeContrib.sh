//*****************************************************************************
//**
//**   File:               computeContrib.sh
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

[Shader]
Name = computeContrib
Vertex = basicvs.vert
Fragment = computeContrib.frag

#*** Fragment shader uniforms

[Uniform : heightmap]
Components = 1
Datatype = int

[Uniform : layerBounds]
Components = 3
Datatype = float

[Uniform : bleedBounds]
Components = 3
Datatype = float
