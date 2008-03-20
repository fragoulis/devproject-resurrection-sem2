//*****************************************************************************
//**
//**   File:               lightmapTex.sh
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

[Shader]
Name = lightmapTex
Vertex = basicvs.vert
Fragment = lightmapTex.frag

#*** Fragment shader uniforms

[Uniform : coordModifier]
Components = 4
Datatype = float

[Uniform : contributionTex]
Components = 1
Datatype = int

[Uniform : lightmap]
Components = 1
Datatype = int

[Uniform : layer1map]
Components = 1
Datatype = int

[Uniform : layer2map]
Components = 1
Datatype = int

[Uniform : layer3map]
Components = 1
Datatype = int

[Uniform : layer4map]
Components = 1
Datatype = int