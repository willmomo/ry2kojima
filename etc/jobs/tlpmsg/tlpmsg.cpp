// tlpmsg.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <mbstring.h>
#include <locale.h>

#import "msxml.dll" named_guids

//------------------------------------------------------------
//�T�v
//	��������w�肵����؂蕶����Ŕz��ɂ킯��
//
//------------------------------------------------------------
std::vector<std::string> Split(const char* string1, const char* string2)
{
	bool quoted = false;
	std::string token;
	std::vector<std::string> ret;

	const char* p = string1;

	while (*p) {
		if (_ismbblead(p[0] & 0xff) && _ismbbtrail(p[1] & 0xff)) {
			
			// 2�o�C�g�����̂Ƃ�

			if (quoted || _mbschr(reinterpret_cast<const unsigned char*>(string2), ((p[0] & 0xff) << 8) | (p[1] & 0xff)) == NULL) {
				token += p[0];
				token += p[1];
			} else {
				ret.push_back(token);
				token.erase();
			}

			p += 2;
		} else if (p[0] == '\"' && p[1] == '\"') {

			// ��d�_�u���N�I�[�e�[�V�����̎�

			token += *p;
			p += 2;
		} else {

			// 1�o�C�g�����̂Ƃ�

			if (*p == '\"') {
				quoted = (quoted) ? false : true;
			} else if (quoted || _mbschr(reinterpret_cast<const unsigned char*>(string2), *p) == NULL) {
				token += *p;
			} else {
				ret.push_back(token);
				token.erase();
			}

			p++;
		}
	}

	ret.push_back(token);

	return ret;
}

//------------------------------------------------------------
//�T�v
//	�ݒ�t�@�C����ǂݍ��݂���
//
//�߂�l
//   0�F���� !0�F�ُ�
//
//------------------------------------------------------------
int GetIniFile(TELOP_MES_ALL pa, MESSAGE_ALL pb, SET_MES_TLP_ALL pc, const char* strFileName)
{
	int ret = 0;
	int i, j;
	std::vector<std::string> buf;

	try
	{
		MSXML::IXMLDOMDocumentPtr pDoc;
		HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);

		if (!pDoc->load(strFileName))
			return -1;

		// �e���b�v���̓ǂݍ���

		MSXML::IXMLDOMNodeListPtr pTelops = pDoc->getElementsByTagName("option/telops/telop");
		if (pTelops) {
			for (i = 0; i < pTelops->length; i++) {
				MSXML::IXMLDOMNodePtr pTelop = pTelops->item[i];

				int id = atoi(pTelop->attributes->getNamedItem("id")->text);
				if (!(1 <= id && id <= MAX_TELOP))
					continue;

				strcpy(pa[id-1].telop, pTelop->attributes->getNamedItem("value")->text);
			}
		}

		// ���b�Z�[�W���̓ǂݍ���

		MSXML::IXMLDOMNodeListPtr pMessages = pDoc->getElementsByTagName("option/messages/message");
		if (pMessages) {
			for (i = 0; i < pMessages->length; i++) {
				MSXML::IXMLDOMNodePtr pMessage = pMessages->item[i];

				int id = atoi(pMessage->attributes->getNamedItem("id")->text);
				if (!(1 <= id && id <= MAX_MESSAGE))
					continue;

				MESSAGE_T* p = &pb[id-1];

				buf = Split(pMessage->attributes->getNamedItem("bgcolor")->text, ",");
				if( buf.size() == 3 ){
					p->bg_R = atoi(buf[0].c_str());
					p->bg_G = atoi(buf[1].c_str());
					p->bg_B = atoi(buf[2].c_str());
				}

				buf = Split(pMessage->attributes->getNamedItem("title")->text, ",");
				if( buf.size() == 4 ){
					p->title_R = atoi(buf[0].c_str());
					p->title_G = atoi(buf[1].c_str());
					p->title_B = atoi(buf[2].c_str());
					strcpy(p->title, buf[3].c_str());
				}

				for (j = 0; j < pMessage->childNodes->length; j++) {
					MSXML::IXMLDOMNodePtr pLine = pMessage->childNodes->item[j];

					int lid = atoi(pLine->attributes->getNamedItem("id")->text);
					if (!(1 <= lid && lid <= MAX_MESLINE))
						continue;

					buf = Split(pLine->attributes->getNamedItem("value")->text, ",");
					if( buf.size() == 4 ){
						p->msg_line[lid-1].font_R = atoi(buf[0].c_str());
						p->msg_line[lid-1].font_G = atoi(buf[1].c_str());
						p->msg_line[lid-1].font_B = atoi(buf[2].c_str());
						strcpy(p->msg_line[lid-1].text, buf[3].c_str());
					}
				}
			}
		}

		// �������t�@�C�� (.INI �t�@�C��) ����A���b�Z�[�W�E�e���b�v�ݒ���擾���܂��B
		MSXML::IXMLDOMNodeListPtr pBlocks = pDoc->getElementsByTagName("option/blocks/block");
		if (pBlocks) {
			for (i = 0; i < pBlocks->length; i++) {
				MSXML::IXMLDOMNodePtr pBlock = pBlocks->item[i];

				int id = atoi(pBlock->attributes->getNamedItem("id")->text);
				if (!(1 <= id && id <= MAX_SET))
					continue;

				SET_MES_TLP_T* p = &pc[id - 1];

				strcpy(p->day, pBlock->attributes->getNamedItem("day")->text);
				strcpy(p->time, pBlock->attributes->getNamedItem("time")->text);
				strcpy(p->starttime, pBlock->attributes->getNamedItem("starttime")->text);
				strcpy(p->endtime, pBlock->attributes->getNamedItem("endtime")->text);

				for (j = 0; j < pBlock->childNodes->length; j++) {
					MSXML::IXMLDOMNodePtr pCorner = pBlock->childNodes->item[j];

					buf = Split(pCorner->attributes->getNamedItem("value")->text, ",");
					if( buf.size() == 6 ){
						p->corners[j].rdb_begin = atoi(buf[0].c_str());
						p->corners[j].rdb_end = atoi(buf[1].c_str());
						p->corners[j].telop_id = atoi(buf[2].c_str());
						p->corners[j].telop_id2 = atoi(buf[3].c_str());
						p->corners[j].msg_id = atoi(buf[4].c_str());
						p->corners[j].msg_id2 = atoi(buf[5].c_str());
					}
				}
			}
		}
	}
	catch(_com_error& e)
	{
		ret = e.Error();
	}

	return ret;
}

//------------------------------------------------------------
//�T�v
//	�ݒ�t�@�C�����������݂���
//
//�߂�l
//   0�F���� !0�F�ُ�
//
//------------------------------------------------------------
int SetIniFile(const TELOP_MES_ALL pa, const MESSAGE_ALL pb, const SET_MES_TLP_ALL pc, const char* strFileName)
{
	int ret = 0;
	int i, j;
	char workStr[256];
	std::vector<std::string> buf;

	try
	{
		MSXML::IXMLDOMDocumentPtr pDoc;
		HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);

		// ���[�g�m�[�h�쐬
		MSXML::IXMLDOMElementPtr pRoot = pDoc->appendChild(pDoc->createElement("option"));

		// �e���b�v�p�m�[�h�쐬
		MSXML::IXMLDOMElementPtr pTelops = pRoot->appendChild(pDoc->createElement("telops"));

		// �S�e���b�v�������o��
		for (i = 0; i < MAX_TELOP; i++) {

			MSXML::IXMLDOMElementPtr pTelop = pTelops->appendChild(pDoc->createElement("telop"));
			sprintf(workStr, "%d", i + 1);
			pTelop->setAttribute("id", workStr);
			pTelop->setAttribute("value", pa[i].telop);
		}

		// ���b�Z�[�W�p�m�[�h�̍쐬
		MSXML::IXMLDOMElementPtr pMessages = pRoot->appendChild(pDoc->createElement("messages"));

		// �S���b�Z�[�W�������o��
		for (i = 0; i < MAX_MESSAGE; i++) {

			MSXML::IXMLDOMElementPtr pMessage = pMessages->appendChild(pDoc->createElement("message"));
			sprintf(workStr, "%d", i + 1);
			pMessage->setAttribute("id", workStr);

			sprintf(workStr, "%d,%d,%d", pb[i].bg_R, pb[i].bg_G, pb[i].bg_B);
			pMessage->setAttribute("bgcolor", workStr);

			sprintf(workStr, "%d,%d,%d,%s", pb[i].title_R, pb[i].title_G, pb[i].title_B, pb[i].title);
			pMessage->setAttribute("title", workStr);

			for (j = 0; j < MAX_MESLINE; j++) {

				MSXML::IXMLDOMElementPtr pLine = pMessage->appendChild(pDoc->createElement("line"));

				sprintf(workStr, "%d", j + 1);
				pLine->setAttribute("id", workStr);

				sprintf(workStr, "%d,%d,%d,%s",
					pb[i].msg_line[j].font_R, pb[i].msg_line[j].font_G, pb[i].msg_line[j].font_B, pb[i].msg_line[j].text);
				pLine->setAttribute("value", workStr);
			}
		}

		// ���b�Z�[�W�E�e���b�v�m�[�h�̍쐬
		MSXML::IXMLDOMElementPtr pBlocks = pRoot->appendChild(pDoc->createElement("blocks"));

		// �S�u���b�N�������o��
		for (i = 0; i < MAX_SET; i++) {

			MSXML::IXMLDOMElementPtr pBlock = pBlocks->appendChild(pDoc->createElement("block"));

			sprintf(workStr,"%d",i + 1);
			pBlock->setAttribute("id", workStr);

			pBlock->setAttribute("day", pc[i].day);
			pBlock->setAttribute("time", pc[i].time);
			pBlock->setAttribute("starttime", pc[i].starttime);
			pBlock->setAttribute("endtime", pc[i].endtime);

			for (j = 0; j < MAX_CORNER; j++) {

				MSXML::IXMLDOMElementPtr pCorner = pBlock->appendChild(pDoc->createElement("corner"));

				sprintf(workStr, "%d", j + 1);
				pCorner->setAttribute("id", workStr);

				sprintf(workStr, "%d,%d,%d,%d,%d,%d",pc[i].corners[j].rdb_begin, pc[i].corners[j].rdb_end,
													 pc[i].corners[j].telop_id, pc[i].corners[j].telop_id2,
													 pc[i].corners[j].msg_id, pc[i].corners[j].msg_id2);
				pCorner->setAttribute("value", workStr);
			}
		}

		pDoc->save(strFileName);
	}
	catch(_com_error& e)
	{
		ret = e.Error();
	}

	return ret;
}

void main(int argc, char* argv[])
{
	setlocale(LC_ALL, "jpn");

	char strFileName[MAX_PATH];
	GetModuleFileName(NULL, strFileName, MAX_PATH);
	PathRemoveFileSpec(strFileName);
	PathAppend(strFileName, INI_FILE);

	TELOP_MES_ALL telops;
	MESSAGE_ALL messages;
	SET_MES_TLP_ALL blocks;

	memset(telops, 0, sizeof(telops));
	memset(messages, 0, sizeof(messages));
	memset(blocks, 0, sizeof(blocks));

	CoInitialize(NULL);

	DWORD dwStart = GetTickCount();

	printf("�������t�@�C����ǂݍ��݂��܂��B\n");
	// INI�t�@�C���̓Ǎ���
	GetIniFile(telops, messages, blocks, strFileName);

	printf("���v���ԁF%d\n", GetTickCount() - dwStart);
	dwStart = GetTickCount();

	//strcpy(telops[0].telop, "�e���b�v1");
	//strcpy(telops[39].telop, "�e���b�v40");
	//strcpy(messages[0].title, "\"\"�^�C�g��,�����Ƃ�\"\"");

	printf("�������t�@�C���ɏ������݂��܂��B\n");
	// INI�t�@�C���̏�������
	SetIniFile(telops, messages, blocks, strFileName);

	printf("���v���ԁF%d\n", GetTickCount() - dwStart);

	CoUninitialize();
}

