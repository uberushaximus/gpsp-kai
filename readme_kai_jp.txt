-- gameplaySP  Gameboy Advance emulator for Playstation Portable --

-- Release log --
-UnOfficial gpSP kai 3.1 test 11 svn rev.191
	サウンドのノイズの対応
	傾きセンサの調整
	チートのROM書込に対応(6aaaaaaa 0000ddddのパターン)
		ただし、16MB以下のROMのみ
		一度有効にすると無効になりません
	アイコンの変更(frmb0001さんからの提供です)
		背景を追加するとタイトルが表示されないので、test版では無しにしておきます
	BIOSエラーのメッセージを意訳
	メッセージデータの内部フォーマットを変更して省メモリ化
	パレットを変更して多少高速化
	サウンドスレッドの調整
	ロードステート時のフレームスキップの動作を調整
	タイマーの調整
	ディレクトリの設定を相対参照に変更

-UnOfficial gpSP kai 3.1 test 10 svn rev.177
	サウンドのON/OFFを有効にした
	傾きセンサを実装(アナログPADで操作できます/使用時は割り当てを全てNONEにして下さい)
	各所微調整
	BIOSの0xD/getChecksumの実装

-UnOfficial gpSP kai 3.1 test 09 svn rev.170
	ステートセーブのデータが読めないバグを修正
		申し訳ありませんが、test 8で保存したデータは読めません
	サウンドスレッドの調整

-UnOfficial gpSP kai 3.1 test 08 svn rev.165
	サウンドの遅れの調整
	フレームスキップ処理の調整
	サウンドのバグを修正
	定期的にでていたノイズを修正
	サウンドバッファの設定を有効にした

-UnOfficial gpSP kai 3.1 test 07 svn rev.150
	タイマー処理の調整
	サウンドの遅れの調整
		サウンドバッファの設定は現在機能していません、内部固定です
		将来はサウンドバッファの設定を削除する予定です
		現在の設定値は、以前の2倍の精度になっています
		チャンネルごとのズレは現在調査中です

-UnOfficial gpSP kai 3.1 test 06 svn rev.135
	ZIPファイル読込み時の不具合を修正

-UnOfficial gpSP kai 3.1 test 05.1 svn rev.129
	サウンドの遅延を修正

-UnOfficial gpSP kai 3.1 test 05 svn rev.125
	カーネルモードで動作するように変更
	サウンド周りを非SDLにした
	statesaveの日付表示を修正
	上記に伴い、statesaveファイルのサイズを506947 -> 506951に変更
		古いファイルも読込み時に自動判別して読み込みます
		保存時に新しい形式で保存されます
	デフォルトディレクトリの変更
	分割EBOOT.PBPの廃止
		DEVHOOKやFW1.0~1.5を使用している方は各自対応して下さい
	フォーラムからのソースを元に多数修正 thanks ?????

-UnOfficial gpSP kai 3.1 test 04 svn rev.65
	プチノイズの低減
	チート数を30個に変更
	チートメニューのページ切替に対応
	チートが有効にならなかったのを修正
	game cfgファイルをチートの増加に対応
		昔のgame cfgファイルは削除して下さい、新しいものは136byteになっています

-UnOfficial gpSP kai 3.1 test 03 svn rev.45
	スリーブ時のフリーズに対応
		スリーブ復帰後はメニュー画面になります

-UnOfficial gpSP kai 3.1 test 02
	ちょこっと最適化
	サウンドバッファの設定を2048～に戻した
	サウンドの定期的なプチノイズの対応

-UnOfficial gpSP kai 3.1 test 01
	不正なBIOSコールを無視するようにした 
		一部の特殊なROMに対応
	サウンド周りを少し調整
	ARM/THUMBのSBC/RSC命令を再調整
		ドンキーコング3の音が出ていなかったのに対応 
	I/Oレジスタの0x410～0x800に仮対応 
		一部の特殊なROMに対応

-UnOfficial gpSP kai 3.0
	ソースを少し整理

-UnOfficial gpSP kai ml 3.00 test 10
	BIOSの設定をミスしていたのを修正 
	FPS表示時に余計なI/Oを表示していたのを修正

-UnOfficial gpSP kai ml 3.00 test 9
	バックアップセーブの不具合修正 
	スクリーンショットをBMPで実装 
		PNGライブラリは不必要になりました
	タイミングによっては、セーブデータが壊れるバグを修正 
	BIOS HALTを仮実装 
	BIOSコール周りを将来用に調整

-UnOfficial gpSP kai ml 3.00 test 8
	FF6Aの対応
		thumb sbc命令のc flagのバグを修正
	スクリーンショットでハングアップする。test 9にて修正予定
	romをロードしないで終了すると、一瞬ブルースクリーンになるのを修正
	Kingdom Hearts - Chain of Memoriesのムービーがフリーズしなくなった(画像はボロボロですが)

-UnOfficial gpSP kai ml 3.00 test 7
	BIOSの処理をdivArm以外は元に戻した
		不具合が多かったため(速度面で捨てがたいものがあるので、将来は何とかしたい)
	残りのメモリと、ROM用バッファサイズを表示するようにした

-UnOfficial gpSP kai ml 3.00 test 6
	サウンドのズレを修正 
	メトロイドフュージョンのセーブデータ選択画面のステータス異常を修正 
	BIOS内のdivArm,sqrt,cpuset,cpufastsetをネイティブで実装 
		将来BIOS無しで動かせるためのテスト
		キングダムハーツが起動できなくなってます
	Makefileの修正

-UnOfficial gpSP kai ml 3.00 test 5
	タイマのカウンタ処理をさらに修正 
		ユグドラユニオンの不具合修正
	Makefileの修正 
	この二点は特殊な状況でしか発生しないと思われるので、とりあえずそのまま放置
	サウンド周りをほんの少し最適化 
	今回からver1.0とver1.5を同梱

-UnOfficial gpSP kai ml 3.00 test 4
	RPGツクールアドバンスの音がおかしくなるバグを修正
		タイマのカウンタ処理を修正
	サウンドにプチプチとノイズが入っていたので、mips_stub.Sの一部を0.9に戻した

-UnOfficial gpSP kai ml 3.00 test 3
	gpSP0.91での変更点をほぼ取り込みました 
		残りはデバッグ関係の実装と、arm_stub.S/arm_emit.hをそれぞれPSP用に移植することです
	32MBのROMを実行時にPSPをスリープさせると、復帰時にハングアップするのを部分的に修正
		メニューを表示させた状態でスリープさせれば、ハングアップしない様にしました
	サウンド周りを少し最適化・高精度化
	画面表示周りを少し最適化
		上記2点により2～3FPS向上しています
	0.91のgame_config.txtに日本のROMを追加

-UnOfficial gpSP kai ml 3.00 test 2
	gpSP0.91での変更点を一部取り込みました 
		FPSの表示(キーコンフィグにて適当なキーに割当てて使用します) 
		VOL UP/DOWNは実装していません
	上記によるmsgファイルの更新 
	HOMEボタンで終了できなかったのを修正

-UnOfficial gpSP kai ml 3.00 test 1
	gpSP0.91での変更点を一部取り込みました
		cpu周りは大幅な変更が必要なため、あまり取り込んでいません
		細かい修正も行っていません
	イタリア語のメッセージファイルの追加
	アイコン画像の追加
		thanks Luca Lefebre

-UnOfficial gpSP kai ml 2.92
	マルチランゲージに対応
		PSPの言語設定で選んだ言語のファイルを自動で使用する
	message.cfg/font.cfgを*.msg/*.fntにリネーム
	dir.cfg/*.msg/*.fntをsetting/の下に移動
	日本語しかできないので、英語/日本語以外はそれぞれ書き換えて使ってください

-UnOfficial gpSP kai jp 2.91
	非正規のBIOSでも起動できるようにした
	message.cfgの追加

-UnOfficial gpSP kai jp 2.9
	メニューでの時計表示/バッテリー残量表示を追加
	終了時に222MHzに戻す様にした
	メニューから○や×ボタンでゲームに戻る時に、ゲーム内でボタンを認識させない様にした
		(ボタンを放した時点で戻ります) 
	ROM等の選択時にR/Lボタンでの5行ごとの移動を追加
	BBSに寄せられた情報による、RTC廻り/mips_stub.Sの修正
	日時の表示フォーマットにPSPのレジストリを参照
	1.5用のフォルダ名を破損ファイル非表示に対応
	正規のGBA/NDSのBIOSで起動可能
	statesaveのファイル名のROM名とスロット番号の間に"_"を入れた
	メニューのグラフィック/サウンドの設定にスクリーンショット機能を追加
		PNG RGB 24bit形式でdir.cfgで設定したフォルダに、ROMファイル名+年月日時間で保存されます
	メニュー時にアナログキーでの操作を追加
	上記に伴うmessage.cfg/dir.cfgの変更
	メニュー画面をカスタマイズしやすいように、メッセージの順番を入れ替えた
	message.cfgの翻訳の追加
	メニューにチートファイルの読み込みを追加
	非暗号のチートに対応
		フォーマットは
			direct_v? チート名
			AAAAAAAA DDDDDDDD
		direct_v?が非暗号用の識別子で、v1でPAR V1/V2用・v3でPAR V3用の暗号化前のデータが使える
		AAAAAAAAがオペコード/アドレス、DDDDDDDDがデータ

	ソースからのビルドにlibpngが必要

-UnOfficial gpSP kai jp 2.8
	gpSP0.9をベースにしました~
	各種メッセージ類をmessage.cfgに分離~
	フォントの設定ファイルfont.cfgを新設~
	fbmフォントとmessage.cfgを用意すれば多言語に対応~
	各種ディレクトリ設定ファイルdir.cfgを新設~
	サウンドバッファが2048で落ちるのを(とりあえず)修正~
	backupの保存を終了時にしている場合、HOMEキーでの終了時にもbackupを保存するようにした~
	backupの保存を終了時にしている場合、別ゲームのロードを行った際にもbackupを保存するようにした~
	8個までのidle_loop_eliminate_targetに対応~
	ステートセーブ/RTCの時刻にPSP本体の設定を使用するようにした~

	cfgファイルのフォーマットについて
		行頭が#の行はコメント文です
		行頭の!以降、改行までがデータです
		!文が続いた場合、内部で連結されます
		単純に順番に読み込んでいるだけなので、順番を変えると正常に動きません
		1データに付き511byteまでです。超えた場合正常に動きません 
		エラーチェックはほとんどしていません
		メッセージを編集する際は、データー内の%s,%d等は必ず残してください

-gpSP kai 2.7
	欠番

-gpSP kai 2.6
	kai 2.5のバグ修正
		vram読込/書込時のバグ修正(再修正)
			0x18000に読込/書込したときの修正
		vramを一部破壊していたのを修正
		サウンドのRLチャンネルの入れ替え(gbc互換サウンドも修正)(96氏より)
	高速化/最適化
		チートコードを標準でOFFにした
			必要ならばmake -e"OPT=-DCHEAT"でビルドしてください
		メインループの不要な変数操作を削除
		mips_stub.Sの一部をdelay slotを使用するようにした(5氏より)
		mips_stub.Sのマクロを展開しdelay slotを使用する様にした
	新規対応
		桃太郎電鉄を動くようにした(5氏より)

	拡張
	debugモードの追加
		make -e"OPT=-DDEBUG_MODE"でビルドすることによりdebugモードが追加されます
		画面サイズをdebugにして△+上下にて表示モード切替です
		mode 0 表示無し
		mode 1 VBALNK割込みに同期して表示
		mode 2 HBLANK割込みに同期して表示

	for eLoader
		クロックの設定を行わないようにした
		eloaderで設定したクロックで起動すると思われます

	ビルド時のオプションについて
		make kxploit で1.5用の実行ファイル作成します
		make -e"OPT=～" でオプションを設定します
			OPT=-DDEBUG_MODE	デバッグモード
			OPT=-DCHEAT			チート有効
			OPT=-DELOADER		eLoader対応
		例)1.5用実行ファイルをeLoader,デバッグ,チート対応で作成
			make kxploit -e"OPT=-DDEBUG_MODE -DCHEAT -DELOADER"

-gpSP kai 2.5
	簡易/バグの多い チート機能の追加
		アドレス0x02000000～(ewram)と、0x03000000～(iwram)に対応
		1byteの書換えのみ対応

		ファイル名は"ROMファイル名".chtです。ROMファイルと同じ場所においてください
		ROMファイル名が"rpg.gba"の場合、"rpg.cht"です

		ファイルのフォーマットは
			#コメント行
			 コード名称:アドレス:byte data(16進)です。
		例
			#cheat test
			test 01:0200123F:6F

		コードのサーチやチートのON/OFFはまだ出来ません。

	vram書込時のバグ修正(130氏より)
	sramサイズ自動判定のバグ修正
	カービィの不具合修正(5/104氏より)
	サウンドのRLチャンネルの入れ替え(96氏より)
	メダロットナビの不具合修正(5/104氏より)

	for eLoader
	8MB以上のROMの分割ロードに対応
		8MB以上のROMはZIP圧縮に対応していません
		Clock Speed 300MHz/Bus Speed 150MHzに設定

-gpSP kai 2.4
	画面の表示モードを追加。
		unscaled 3:2				(既存のモード)
		scaled 3:2(LINEAR)			(既存のモード)
		fullscreen 16:9(LINEAR)		(既存のモード)
		scaled 3:2(NEAREST)			(フィルターを使用しないモード)
		fullscreen 16:9(NEAREST)	(フィルターを使用しないモード)
		scaled 1.75(NEAREST)		(1.75バイモード/上下 4dotが切れます)

-gpSP kai 2.3
	sound I/Oのbug fix。
		いくつかのゲームが動くようになりました。

-gpSP kai 2.2
	save stateの高速化。

-gpSP kai 2.1
	idle_loop_eliminate_targetの複数値に対応。

-gpSP kai 2
	sound buffer sizeのコンフィグを追加。
		2048～12288まで設定可能。(デフォルトは2048/オリジナルは8192)

	設定したsound buffer sizeをGAME CFGファイルに保存されるようにした。
		GAME CFGファイルのサイズが12byte -> 16byteに変更。

-gpSP kai (1)
	アナログパッドのコンフィグを追加

	設定したsound buffer sizeをgpSP CFGファイルに保存されるようにした。
		gpSP CFGファイルのサイズが56byte -> 72byteに変更。

takka takka@tfact.net
http://sourceforge.jp/projects/gpsp-kai
