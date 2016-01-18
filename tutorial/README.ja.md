# チュートリアル

## 実行方法
`./build.sh` でコンパイルし、`./run.sh` で実行して下さい

## リファレンス
* Epiphany SDK Reference <http://adapteva.com/docs/epiphany_sdk_ref.pdf>
* OpenMPI v1.8 documentation <http://www.open-mpi.org/doc/v1.8/>

## コンテンツ
* 0x: Epihany SDK
* 1x: OpenMPI clustering
* 2x: eSDK + OpenMPI

### 0x:Epiphany SDK
#### Prerequirement
Epiphany SDK (eSDK)は /opt/adapteva/esdk ディレクトリにインストール済みです。

#### 0.1 Hello, World
コンソールに "hello, world" と出力するだけのプログラムです。
まだeSDKは使っていませんので、build.shにも特別な記述はありません。

#### 0.2 eHAL (Epiphany Hardware Abstraction Layer)
> The Epiphany Hardware Abstraction Layer (eHAL) library provides functionality for communicating with the Epiphany chip when the application runs on a host. The host can be a PC or an embedded processor. The communication is performed using memory writes to and reads from shared buffers that the applications on both sides should define.

まずはeHALライブラリを組み込んでみましょう。

* main.c: "e-hal.h"をincludeし、e_init()とe_finalize()を呼びます
* build.sh: gccのオプションを追加し、eHALライブラリとリンクします

#### 0.3 プラットフォーム情報
eHALライブラリを利用し、プラットフォーム情報を取得します。

* main.c: e_init()後にe_reset_system()でシステムをリセットした後にe_get_platform_info()で取得します。

#### 0.4 ワークグループ
> A Workgroup is a collection of adjacent eCores on the Epiphany chip(s), normally intended for the parallel execution of a computational task. The workgroup is rectangular and its size and origin are defined at run-time by the host. Its parameters are set by a host library function and are maintained in a workgroup object.

Epiphany上のeCoreをグルーピングし、このグループ(ワークグループ)単位でプログラムを並行して実行できます。ここでは4x4のすべてのeCoreを一つのワークグループとし、一度に16の処理を並行して実行させることにします。

Epiphanyのサンプルプログラム(~/epiphany-example/...)は並行処理になっていません。

* main.c: e_open()でワークグループを定義し、e_reset()でグループをリセットし、最後にe_close()でグループを閉じます。

#### 0.5 プログラム実行
eCoreで実行するプログラムを実装します。まずは何もしない(`return 0;`するのみ)プログラムをワークグループに読み込ませます。eCoreで実行するプログラムは、e-gccでELF形式にコンパイルした後、e-objcopyでSRECイメージに変換する必要があります。

* *new* e-hello.c: eCore側のプログラムです。現時点では何もしない空のプログラムです
* main.c: e_load_group()でワークグループにプログラムを読み込ませ、即実行します
* build.sh: 新たな関数をリンクするためのgccオプションを追加します。e-hello.cをコンパイルし、SRECイメージを生成します。

#### 0.6 eLib (Epiphany Hardware Utility Library)
> The Epiphany Hardware Utility library provides functions for configuring and querying the Epiphany hardware resources. These routines automate many common programming tasks that are not provided by the C and C++ languages and are specific to the Epiphany architecture.

ホスト側ではeHALを利用しましたが、Epiphany側ではeLibを使います。ここではeCoreのIDを取得してみます。

* e-hello.c: "e-lib.h"をincludeし、e_get_coreid()関数でeCoreのIDを取得します (まだ出力等はしません)
* build.sh: e-gccのコンパイルオプションを追加します

#### 0.7 eCoreローカルメモリへの書き出し
> Each mesh node has a local, aliased, range of local memory that is accessible by the mesh node itself starting at address 0x0. On 32KB chip models, it ends at address 0x00007FFF. 
<div />
> Each core on the platform is referenced via a definition of a workgroup. Two global objects are available at each core’s space. One object, called e_group_config, contains the information about the chip type, the workgroup’s position and size, and the core’s position in the containing workgroup.

取得したeCore IDをホスト側から参照できるようにするため、eCoreのローカルメモリに書き込みます。ローカルメモリは0x40からのBank0, 0x2000からのBank1, 0x4000からのBank2, 0x6000からのBank3があります (どう使い分けるのかについてはリファレンスマニュアルに多少記述があります)。

なお、突然`e_group_config`という変数が登場しましたが、これはeLibで定義されるグローバル変数です。このプログラムを実行しているeCoreが所属するワークグループの情報を参照できます。もう一つ、`e_emem_config`というグローバル変数もあります。

* *new* addr.h: 書き込む先のアドレスを定義
* main.c: addr.hをinclude
* e-hello.c: eCore IDからワークグループ内での座標(row, col)を取得し、*eLib*のe_write()でローカルメモリに書き込みます

#### 0.8 eCoreローカルメモリからの読み出し
eCoreでの処理が完了するのを待ち、eCoreのローカルメモリに書き込んだeCore IDをホスト側から読み込みます。

eCore側からホスト側へ処理の完了を通知する手段が(少なくとも現時点では)提供されていませんので、ちょっと微妙ですがサンプルアプリと同様にusleep()関数で10,000マイクロ秒(=10ミリ秒)待って、処理を継続させています。

eCoreのローカルメモリの値をホスト側から読み出すために、*eHAL*のe_read()関数を利用します。

* main.c: eCoreでの処理が完了するのを待ち、ワークグループ内のすべてのeCoreのローカルメモリから値を読み出し、出力します
* build.sh: forループの初期化宣言に対応するため、-stdオプションを指定しています

ここではeCoreのローカルメモリを解してホストとEpiphanyとでデータを共有しましたが、すべてのeCoreおよびホストから参照できる外部メモリ領域を利用することも可能です。

