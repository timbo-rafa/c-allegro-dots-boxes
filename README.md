## Compilation

```
cc dots-boxes.c -o dots-boxes $(pkg-config  allegro --libs --cflags)
```

`$(pkg-config allegro --libs --cflags)`  resolves to `-lalleg` in `Ubuntu 16.04`
