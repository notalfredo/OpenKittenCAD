{
    let pointOne: point := dot([5, 3, 0]);
    let lineOne: edge := dot([5, 0, 0]) |> line(%, pointOne);

    let pointTwo: point := dot([-5, 3, 0]);
    let lineTwo: edge := dot([-5, 0, 0]) |> line(%, pointTwo);

    let pointThree: point := dot([0, 5, 0]);
    let arcOne: edge := arc(pointOne, pointThree, pointTwo);

    let connectedEdges: edge := connect(lineOne, arcOne, lineTwo);


    let connectedEdgesMirror: edge := mirror(connectedEdges);

    let myFace: shape := makeFace(connectedEdgesMirror);
};