//---------------------------------------------------------------------
// JavaScript�Ŏg�p�ł���A���[�e�B���e�B�֐�(ver.0.2010.03.01)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// textarea�̍������������B
//
// [�g����]
// <body onload="AutoFit_TextArea(document.getElementById('idTextArea'));">
// <textarea onkeyup="AutoFit_TextArea(this)" onmousedown="AutoFit_TextArea(this)"></textarea>
//
// [���l]
// textarea����\����ԂɂȂ��Ă���ꍇ�AclientHeight��scrollHeight�́A�ǂ����0�ɂȂ�B
// ���̏ꍇ�Abody��onload event�ł́A�������킹���s���Ȃ��B
// textarea��\����Ԃɂ��鎞�A
// AutoFit_TextArea(document.getElementById('idTextArea'));
// �̂悤�ɁA�����������킹�����s����K�v������B
//
// ����m�F: Firefox 3.5.8 , IE8
//---------------------------------------------------------------------
function AutoFit_TextArea(elem) {
	// rows�����ݒ��Ԃ̂Ƃ��́A1�ɂ���B
	if (elem.rows <= 1) {
		elem.rows = 1;
	}
	
	// elem.clientHeight : �R���g���[���̓����̍���
	// elem.scrollHeight : �h�L�������g�̑S�̂̍���
	// clientHeight��scrollHeight���傫���Ƃ��́AclientHeight == scrollHeight�ɂȂ�͗l�B
	
	if (elem.clientHeight >= elem.scrollHeight) {

		// �����͏\������B
		// �������A�k�߂�K�v�����邩������Ȃ��̂ŁA
		// �R���g���[�����k�߂Ă���ēx�L����B

		while ((elem.rows > 1) && (elem.clientHeight >= elem.scrollHeight)) {
			elem.rows--;
		}

		while (elem.clientHeight < elem.scrollHeight) {
			elem.rows++;
		}
	} else {
	
		// �������s�\���B
		// rows�𑝂₵�Ă����AclientHeight���g�債�Ă����B
		// �������A�����ɑ傫���Ȃ��Ă�����̂ŁA�E�B���h�E�S�̂�70%�܂łŎ~�܂�l�ɂ���B
		
		while ((elem.clientHeight < elem.scrollHeight) && (elem.clientHeight < (document.body.clientHeight * 0.7))) {
			elem.rows++;
		}
	}
}

//---------------------------------------------------------------------
// ���t�Ó����`�F�b�N
//---------------------------------------------------------------------
function isDate(datestr) {
	// ���K�\���ɂ�鏑���`�F�b�N
	if (datestr.match(/^\d{4}\/\d{2}\/\d{2}$/) == null) {
		return false;
	}

	var vYear  = datestr.substr(0, 4) - 0;
	var vMonth = datestr.substr(5, 2) - 1; // Javascript�́A0-11�ŕ\��
	var vDay   = datestr.substr(8, 2) - 0;

	// ��,���̑Ó����`�F�b�N
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
