fn foo(a: number, b: number) -> number {
    return 10;
};


{
    let result: number := foo(10, 20)
                            |> foo(%, 10)
                            |> foo(10, %);
};
