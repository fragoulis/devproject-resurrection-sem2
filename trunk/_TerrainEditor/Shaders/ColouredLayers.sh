//*****************************************************************************
//**
//**   File:               ColouredLayers.sh
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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