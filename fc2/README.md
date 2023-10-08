## Dev

Assuming you've followed the [basic setup](../README.md#setup)

Enable the esp environment

```
direnv allow .
idf.py set-target esp32c
```

Run tests

```
./x.sh test
```

Build and Flash

```
idf.py build flash
```
