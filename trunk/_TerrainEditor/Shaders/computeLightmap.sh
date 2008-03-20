//*****************************************************************************
//**
//**   File:               computeLightmap.sh
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

[Shader]
Name = computeLightmap
Vertex = basicvs.vert
Fragment = computeLightmap.frag

#*** Fragment shader uniforms

[Uniform : heightmap]
Components = 1
Datatype = int

[Uniform : normalmap]
Components = 1
Datatype = int

[Uniform : lightdir]
Components = 3
Datatype = float

[Uniform : xyzscale]
Components = 3
Datatype = float

[Uniform : shadoweffect]
Components = 1
Datatype = float

[Uniform : texsize]
Components = 1
Datatype = float

