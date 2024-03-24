{
    let mySphere: shape := sphere(1);
    let myCylinder: shape := cylinder(1, 2);


    addShape(
        myCylinder |> intersection(%, mySphere)
    );
};
