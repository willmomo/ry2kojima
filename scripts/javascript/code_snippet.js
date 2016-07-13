//---------------------------------------------------------------------
// JavaScriptで使用できる、ユーティリティ関数(ver.0.2010.03.01)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// textareaの高さ自動調整。
//
// [使い方]
// <body onload="AutoFit_TextArea(document.getElementById('idTextArea'));">
// <textarea onkeyup="AutoFit_TextArea(this)" onmousedown="AutoFit_TextArea(this)"></textarea>
//
// [備考]
// textareaが非表示状態になっている場合、clientHeightとscrollHeightは、どちらも0になる。
// この場合、bodyのonload eventでは、高さ合わせが行われない。
// textareaを表示状態にする時、
// AutoFit_TextArea(document.getElementById('idTextArea'));
// のように、自動高さ合わせを実行する必要がある。
//
// 動作確認: Firefox 3.5.8 , IE8
//---------------------------------------------------------------------
function AutoFit_TextArea(elem) {
	// rowsが未設定状態のときは、1にする。
	if (elem.rows <= 1) {
		elem.rows = 1;
	}
	
	// elem.clientHeight : コントロールの内部の高さ
	// elem.scrollHeight : ドキュメントの全体の高さ
	// clientHeightがscrollHeightより大きいときは、clientHeight == scrollHeightになる模様。
	
	if (elem.clientHeight >= elem.scrollHeight) {

		// 高さは十分ある。
		// ただし、縮める必要があるかもしれないので、
		// コントロールを縮めてから再度広げる。

		while ((elem.rows > 1) && (elem.clientHeight >= elem.scrollHeight)) {
			elem.rows--;
		}

		while (elem.clientHeight < elem.scrollHeight) {
			elem.rows++;
		}
	} else {
	
		// 高さが不十分。
		// rowsを増やしていき、clientHeightを拡大していく。
		// ただし、無限に大きくなっても困るので、ウィンドウ全体の70%までで止まる様にする。
		
		while ((elem.clientHeight < elem.scrollHeight) && (elem.clientHeight < (document.body.clientHeight * 0.7))) {
			elem.rows++;
		}
	}
}

//---------------------------------------------------------------------
// 日付妥当性チェック
//---------------------------------------------------------------------
function isDate(datestr) {
	// 正規表現による書式チェック
	if (datestr.match(/^\d{4}\/\d{2}\/\d{2}$/) == null) {
		return false;
	}

	var vYear  = datestr.substr(0, 4) - 0;
	var vMonth = datestr.substr(5, 2) - 1; // Javascriptは、0-11で表現
	var vDay   = datestr.substr(8, 2) - 0;

	// 月,日の妥当性チェック
	if (vMonth >= 0 && vMonth <= 11 && vDay >= 1 && vDay <= 31) {
		var vDt = new Date(vYear, vMonth, vDay);

		if (isNaN(vDt) == true) {
			return false;
		} else if (vDt.getFullYear() == vYear &&
				   vDt.getMonth() == vMonth &&
				   vDt.getDate() == vDay) {
			return true;
		}
	} else {
		return false;
	}
}
