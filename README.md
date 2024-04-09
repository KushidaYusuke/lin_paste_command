# lin_paste_command
pasteコマンドの作成


### 例1

```
./out  -d "&"  paste1.txt paste2.txt paste3.txt
```

- 入力
```
a001
a002
a003
```

```
b001
b002

b003
```

```

c001
c002
c003
```

- 出力
```
a001&b001&
a002&b002&c001
a003&&c002
&b003&c003
```
