let smallSphere: shape := sphere(1);


addShape(
    smallSphere 
        |> translate(%, [5,0,0])
);


smallSphere := sphere(4);


addShape(smallSphere);


const bigCone: shape := cone(3, .5, 5) |> translate(%, [0, 0, 5]);


addShape(bigCone);
