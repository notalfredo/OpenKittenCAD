fn main() {
    let myCylinder := cylinder(size = 10.)
                    |> scale(%, 10, 3, 30);

    let mySphere := sphere(radius = 2);
                               

    union(
        intersect(
            myCube,
            myCylinder
        ),
        difference(
            cube(10 + 5, 29-1),
            polyhedron(20*0, 10 % 10, 30/10)
        )
    )
}
