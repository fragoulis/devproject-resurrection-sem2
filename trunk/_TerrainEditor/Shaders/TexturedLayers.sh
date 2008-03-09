//*****************************************************************************
//**
//**   File:               TexturedLayers.sh
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

[Shader]
Name = TexturedLayers
Vertex = basicvs.vert
Fragment = TexturedLayers.frag

#*** Fragment shader uniforms

[Uniform : texrepeats]
Components = 4
Datatype = float

[Uniform : heightmap]
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

[Uniform : layerBounds]
Components = 3
Datatype = float

[Uniform : bleedBounds]
Components = 3
Datatype = float

[Uniform : seaLevel]
Components = 1
Datatype = float