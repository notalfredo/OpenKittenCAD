{
    let mySphere: shape := sphere(1);

    let transMySphere: shape := translate(mySphere, [0, 0, 1]);

    addShape(transMySphere);
};
