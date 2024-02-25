fn first(x: number) -> number {
    return x + 20;
};

fn testing() -> number {
    return 12;
};

fn temp(first: number, second: number) -> number {
    return first + second + testing();
};

{
    print(temp(10, 10) + testing());


    print(10 |> first(%));

};
