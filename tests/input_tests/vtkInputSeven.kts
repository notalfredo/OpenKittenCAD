const myFace := [0, 0, 0]
                |> lineTo(%, [0, 5, 0])
                |> lineTo(%, [-1, 4.5, 0])
                |> lineTo(%, [-1, 4, 0])
                |> lineTo(%, [-3, 3, 0])
                |> lineTo(%, [-3, 0, 0])
                |> lineTo(%, [0, 0, 0])
                |> makeFace(%)
                |> revolve(%, [0, 0, 0], [0, 5, 0], 3.14/2);

addShape(myFace);
