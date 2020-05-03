#pragma once

#include <vector>
#include <string>

typedef const char cchar;

//#define		NQ			30

struct Question {
	int		m_nHorz;
	int		m_nVert;
	//int		m_type;			//	問題タイプ
	//int		m_value;		//	合計 or 差分
	//cchar	*m_caption;		//	問題説明
	cchar	*m_text;		//	問題テキスト
};

int numQuestions();
const Question *getQuestion(int ix);
//----------------------------------------------------------------------
enum {
	QTYPE_SAME,		//	A == B
	QTYPE_ADD,		//	A + B = value
	QTYPE_SUB,		//	A - B = value
};
struct Pack {
	cchar	*m_name;		//	パック名
	//std::string		m_name;		//	パック名
	cchar	*m_icon;		//	アイコン名
	int		m_type;			//	タイプ
	int		m_value;		//	合計 or 差分
	int		m_nPiece;		//	数字種類数
	int		m_nHorz;			//	盤面サイズ
	cchar	**m_question;
};

//extern Pack g_pack[];
const Pack *getPack();			//	g.m_type を参照し、当該パック配列を返す
int numPacks();				//	g.m_type を参照し、当該パック数を返す
cchar	*getQuestionRandom(int wd);
void	genQuestionsRandom(std::vector<std::string>&);		//	4x4 ～ 8x8 問題を生成
