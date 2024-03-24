# CheatSheet


### Syntax
```txt
let varName: type := value;
fn funcName(params) -> returnType {};
funcName(arguments);
```


### Operators
```txt
a + b  Addition
a - b  Subtraction
a * b  Multiplication
a / b  Division
a |> b Pipeing
-a     Negation
```

### 2D
```txt
dot([x, y, z])
line([x, y, z], [x, y, z]) | line(point1, point2)
arc([x, y, z], [x, y, z], [x, y, z]) | arc(point1, point2, point3)
makeFace(edge)
```


### 3D 
```txt
sphere(radius) | sphere(radius, angle) | sphere(radius, a1, a2) | sphere(radius, a1, a2, angle)
cone(r1, r2, height) | cone(r1, r2, height, angle)
cylinder(radius, height) | cylinder(radius, height, angle)
box(dx, dy, dz) | box(point, dx, dy, dz) | box(point1, point2)
torus(r1, r2) | torus(r1, r2, angle) | torus(r1, r2, a1, a2) | torus(r1, r2, a1, a2, angle)
```

### Transformations 
```
rotate(shape, [x_rad, y_rad, z_rad])
translate(shape, [x_new, y_new, z_new])
mirror(edge) | mirror(shape)
extrude(face, [x_dir, y_dir, z_dir])
fillet(shape, radius)
chamfer(shape, radius)
```

### Boolean operations
```
union(shape_one, shape_two)
difference(shape_one, shape_two)
intersection(shape_one, shape_two)
```

### Functions
```
connect(edge1, edge2) | connect(edge1, edge2, edge3)
addShape(shape)
print(double)
```

