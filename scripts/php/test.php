<?php
// 出力バッファをONにして、コールバックで UTF-8 -> SJIS 変換させる。
// これで、コマンドプロンプトにちゃんと表示される。
ob_start(function($buf){ return mb_convert_encoding($buf, 'SJIS', 'UTF-8'); });

$response = file_get_contents("http://203.104.209.71/kcs/sound/kcguwzhmxxnwol/13.mp3?version=9");

$fp = fopen("hoge.mp3", "w+b");
if ($fp) {
	fwrite($fp, $response);
	fclose($fp);
}

ob_end_flush();
?>
