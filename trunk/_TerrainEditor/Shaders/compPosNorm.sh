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

