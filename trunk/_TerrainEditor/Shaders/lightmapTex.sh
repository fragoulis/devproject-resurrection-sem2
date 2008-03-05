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