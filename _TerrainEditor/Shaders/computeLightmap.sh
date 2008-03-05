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

