Option Explicit

Dim result

'result = MsgBox("[OK] ボタンのみを表示します。", vbOKOnly)
'result = MsgBox("[OK] ボタンと [キャンセル] ボタンを表示します。", vbOKCancel)
'result = MsgBox("[中止]、[再試行]、および [無視] の 3 つのボタンを表示します。", vbAbortRetryIgnore)
'result = MsgBox("[はい]、[いいえ]、および [キャンセル] の 3 つのボタンを表示します。", vbYesNoCancel)
'result = MsgBox("[はい] ボタンと [いいえ] ボタンを表示します。", vbYesNo)
'result = MsgBox("[再試行] ボタンと [キャンセル] ボタンを表示します。", vbRetryCancel)

'result = MsgBox("警告メッセージ アイコンを表示します。", vbCritical Or vbOKOnly)
'result = MsgBox("問い合わせメッセージ アイコンを表示します。", vbQuestion Or vbOKOnly)
'result = MsgBox("注意メッセージ アイコンを表示します。", vbExclamation Or vbOKOnly)
'result = MsgBox("情報メッセージ アイコンを表示します。", vbInformation Or vbOKOnly)

'result = MsgBox("第 1 ボタンを標準ボタンにします。", vbDefaultButton1 Or vbYesNoCancel)
'result = MsgBox("第 2 ボタンを標準ボタンにします。", vbDefaultButton2 Or vbYesNoCancel)
'result = MsgBox("第 3 ボタンを標準ボタンにします。", vbDefaultButton3 Or vbYesNoCancel)
'result = MsgBox("第 4 ボタンを標準ボタンにします。", vbDefaultButton4 Or vbYesNoCancel)

'result = MsgBox("アプリケーション モーダルに設定します。メッセージ ボックスに応答するまで、現在選択中のアプリケーションの実行を継続できません。", vbApplicationModal Or vbOKOnly)
'result = MsgBox("システム モーダルに設定します。メッセージ ボックスに応答するまで、すべてのアプリケーションが中断されます。", vbSystemModal Or vbOKOnly)


result = MsgBox("結構標準的な、メッセージボックスの処理。", vbQuestion Or vbYesNoCancel, WScript.ScriptName)
Select Case result
Case vbOK
	MsgBox "選択されたボタン：[OK]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbCancel
	MsgBox "選択されたボタン：[キャンセル]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbAbort
	MsgBox "選択されたボタン：[中止]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbRetry
	MsgBox "選択されたボタン：[再試行]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbIgnore
	MsgBox "選択されたボタン：[無視]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbYes
	MsgBox "選択されたボタン：[はい]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbNo
	MsgBox "選択されたボタン：[いいえ]", vbInformation Or vbOKOnly, WScript.ScriptName
Case Else
	MsgBox "選択されたボタン：不明(" & result & ")", vbInformation Or vbOKOnly, WScript.ScriptName
End Select
