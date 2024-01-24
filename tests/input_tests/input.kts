fn main() {
    let myCube := cube(size = 10.)
                    |> translate(%, 10, 3, 30)
                    |> rotateX(%, 3.14);

    let myCylinder := cylinder(height = 10.1, radius = 2);
                               

    union(
        myCube,
        myCylinder
    )
}
