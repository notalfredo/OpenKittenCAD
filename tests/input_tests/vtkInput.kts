fn first(x: number) -> number {
    return x + 20;
};

{
    let myCube: shape := box(1, 1, 1);
    let myCyl: shape := cylinder(1, 1);

    addShape(
        intersection(
            myCyl,
            myCube
        )
    );
};
