# Лабораторная работа 4 — вариант 12, «Банк»

Структура:

- `include/` — заголовки `Bank_Optional`, `Deposit`, `Credit`, `Card`.
- `src/` — реализации классов.
- `tests/` — тесты Google Test.

Ставки задаются долями: `0.1` означает 10%. Для аннуитетного кредита
месячная ставка равна `rate / 12`. При нулевой ставке годовой платёж равен
`sum / years`.

## Сборка и тесты

Требуются CMake 3.20+, компилятор C++20 и Ninja для приведённых команд.
Google Test берётся из установленного пакета или скачивается при первой
конфигурации через FetchContent.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

