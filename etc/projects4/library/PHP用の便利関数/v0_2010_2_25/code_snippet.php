<?php
//-----------------------------------------------------------------
// PHPで使用できる、ユーティリティ関数(ver.0.2010.02.25)
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 太さに応じた定義済みラインスタイルを返す
//-----------------------------------------------------------------
function user_get_linestyle($style, $col, $thickness) {
	switch ($style) {
	case "DOT":
		$base_style = array(
			$col, 
			$col, 
			$col, 
			$col, 
			$col, 
			IMG_COLOR_TRANSPARENT, 
			IMG_COLOR_TRANSPARENT, 
			IMG_COLOR_TRANSPARENT, 
			IMG_COLOR_TRANSPARENT, 
			IMG_COLOR_TRANSPARENT);
		break;

	case "DOT2":
		$base_style = array(
			$col, 
			$col, 
			IMG_COLOR_TRANSPARENT, 
			IMG_COLOR_TRANSPARENT);
		break;
		
	default:
		$base_style = array($col);
		break;
	}
	
	foreach ($base_style as $key => $value) {
		for ($i = 0; $i < $thickness; $i++) {
			$line_style[] = $value;
		}
	}
	
	return $line_style;
}

//-----------------------------------------------------------------
// 太さと定義済みラインスタイルを指定する直線描画関数
//-----------------------------------------------------------------
function user_imageline($im, $x1, $y1, $x2, $y2, $col, 
						$thickness = 1, $style = "SOLID") {
	if ($style == "SOLID") {
		$gd_color = $col;
	} else {
		$gd_color = IMG_COLOR_STYLED;
		imagesetstyle($im, user_get_linestyle($style, $col, $thickness));
	}
	
	imagesetthickness($im, $thickness);
	imageline($im, $x1, $y1, $x2, $y2, $gd_color);
}

//-----------------------------------------------------------------
// 太さと定義済みラインスタイルを指定する矩形描画関数
//-----------------------------------------------------------------
function user_imagerectangle($im, $x1, $y1, $x2, $y2, $col, 
							 $thickness = 1, $style = "SOLID") {
	if ($style == "SOLID") {
		$gd_color = $col;
	} else {
		$gd_color = IMG_COLOR_STYLED;
		imagesetstyle($im, user_get_linestyle($style, $col, $thickness));
	}
	
	imagesetthickness($im, $thickness);
	imagerectangle($im, $x1, $y1, $x2, $y2, $gd_color);
}

//-----------------------------------------------------------------
// 太さと定義済みラインスタイルを指定する円描画関数
// GDの制約でラインスタイルを指定すると、太さ1の円になります。
//-----------------------------------------------------------------
function user_imageellipse($im, $cx, $cy, $w, $h, $col, 
						   $thickness = 1, $style = "SOLID") {
	if ($style == "SOLID") {
		$gd_color = $col;
	} else {
		$gd_color = IMG_COLOR_STYLED;
		imagesetstyle($im, user_get_linestyle($style, $col, $thickness));
	}
	
	imagesetthickness($im, $thickness);
	imageellipse($im, $cx, $cy, $w, $h, $gd_color);
}
%>
