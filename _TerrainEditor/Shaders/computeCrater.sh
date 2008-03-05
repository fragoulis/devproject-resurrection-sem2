[Shader]
Name = computeCrater
Vertex = basicvs.vert
Fragment = computeCrater.frag

#*** Fragment shader uniforms

[Uniform : heightmap]
Components = 1
Datatype = int

[Uniform : linbell_mix]
Components = 1
Datatype = float

[Uniform : bell_area]
Components = 1
Datatype = float

[Uniform : radius]
Components = 1
Datatype = float

[Uniform : xzscale]
Components = 1
Datatype = float

[Uniform : oldmap_mix]
Components = 1
Datatype = float

[Uniform : center]
Components = 2
Datatype = float

