const customFace: shape := [0, 0, 0]
                     |> lineTo(%, [5, 0, 0])
                     |> lineTo(%, [5, 5, 0])
                     |> lineTo(%, [0, 5, 0])
                     |> lineTo(%, [0, 0, 0])
                     |> makeFace(%)
                     |> extrude(%, [0, 0, 10])
                     |> fillet(%, 1);
                     
addShape(customFace);
