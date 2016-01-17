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

