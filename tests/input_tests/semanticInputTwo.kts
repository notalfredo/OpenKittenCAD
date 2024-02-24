fn add(x: number, y: number) -> void {
    print(x + y); 
};

{
    let foo: number := 10 + 22;
    let bar: number := 10 - 22;

    {
        let sum: number := foo + bar;
        print(sum);
    };

    let sum: number := foo - bar;
    print(sum);
    add(foo, sum);
};
