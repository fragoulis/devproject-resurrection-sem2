//*****************************************************************************
//**
//**   File:               ColouredLayers.sh
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

[Shader]
Name = ColouredLayers
Vertex = basicvs.vert
Fragment = ColouredLayers.frag

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

[Uniform : seaLevel]
Components = 1
Datatype = float