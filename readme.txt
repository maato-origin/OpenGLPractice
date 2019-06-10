# OpenGLPractice
C++とOpenGL及びSimple DirectMedia Layerを用いたゲームプログラミングの
学習を目的としたファイル。

# Dependency
使用言語：C++
SDLバージョン：2.0
OpenGLバージョン：3.3
FMODバージョン：1.09.09(Chapter7以降使用)

# Setup
Windows環境においてVisual Studio 2017をインストールする。
Chapter7以降はFMOD(https//fmod.com)のインストールが必要である。

# Usage
・実行ファイルの確認方法
Visual Studio 2017で各プロジェクトファイル(.sln)を開き、
デバッグ実行もしくは実行を行う。

・各種実行ファイルに関して
各チャプターディレクトリ内のテキストを参照

・各種ファイルの取り扱い
Chapter2以降に内蔵のAssetsディレクトリ
Externalディレクトリ
FMODStudioディレクトリ
これらの操作を行った場合、プロジェクト内のライブラリ指定、コードに
記述されたアセット指定も適切に変えなければ正常に動作しない。

# License
ソースコードに関するライセンスは同ディレクトリのLICENSEを参照。
ライブラリはExternalディレクトリの各種サブディレクトリ内を参照。

# References
ゲームプログラミングC++（Savjay Madhav著/吉川 邦夫訳/今給黎 隆監修）