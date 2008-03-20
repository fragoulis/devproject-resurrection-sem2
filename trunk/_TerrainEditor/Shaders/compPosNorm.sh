//*****************************************************************************
//**
//**   File:               compPosNorm.sh
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

[Shader]
Name = compPosNorm
Vertex = basicvs.vert
Fragment = compPosNorm.frag

#*** Fragment shader uniforms

[Uniform : heightmap]
Components = 1
Datatype = int

[Uniform : xyzscale]
Components = 3
Datatype = float

[Uniform : dimdenom]
Components = 1
Datatype = float

