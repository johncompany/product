# テンプレートプロジェクト
これはRISC-V (RV32I) 向けにプログラムをコンパイルするためのテンプレートプロジェクトです．

## 使い方
```
$ make
```

## 生成物
- prog.elf
- prog.dump
- prog.bin
- prog.hex
- code.hex
- data.hex

`make`すると上の6つのファイルが順に生成されます．
prog.elfはELF形式の実行ファイル、prog.dumpはprog.elfを`objdump -D`コマンドで逆アセンブルしてアセンブリ形式にしたもの、prog.binはprog.dumpをバイナリ形式にしたもの、prog.hexはprog.binをテキスト形式にしてVerilogの`$readmemh`で読めるようにしたものです．
さらに，code.hex と data.hex は実験で使うためにprog.hexをコードとデータに分離したものです．

一連の加工には`../tools`内のPythonスクリプトが使われます。
