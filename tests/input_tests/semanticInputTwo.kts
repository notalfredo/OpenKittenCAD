fn doSomething(x: number, y: number, z: number) -> void {
    let temp: number := 69;
    print(x * y);
    print(x * y - z);
    print(x * y - z + temp);
};

{

    let first: number := 12 / 4;
    let second: number := 2 * 5;

    print(first);
    print(second);

    {
        let third: number := 0 * 10 / 10;
        print(third);
        doSomething(first, second, third);
    };

    let third: number := 1;
    doSomething(first, second, third);
};
