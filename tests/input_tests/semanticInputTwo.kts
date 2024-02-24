fn add(x: number, y: number) -> void {
    let temp: number := 69;
    print(x + y); 
    print(temp);
};

{
    let foo: number := 10 + 22;
    let bar: number := 100 - 22;

    {
        let sum: number := foo + bar;
        print(sum);
    };

    let sum: number := foo - bar;

    add(bar, foo);
    add(sum, foo);
};
