#pragma once

#include <vector>
#include <string>

typedef const char cchar;
typedef unsigned char uchar;

enum {
	LINK_UP = 1,
	LINK_DOWN = 2,
	LINK_LEFT = 4,
	LINK_RIGHT = 8,
};
inline uchar revDir(uchar dir)
{
	switch( dir ) {
	case LINK_UP:		return LINK_DOWN;
	case LINK_DOWN:		return LINK_UP;
	case LINK_LEFT:		return LINK_RIGHT;
	case LINK_RIGHT:	return LINK_LEFT;
	}
	return 0;
}

class Board {
public:
	enum {
		TYPE_SAME,		//	A == B
		TYPE_ADD,		//	A + B = value
		TYPE_SUB,		//	A - B = value
	};
public:
	Board(int h = 4, int v = 0);
public:
	int		width() const { return m_nHorz; }
	int		size() const { return m_number.size(); }
	int		nSolved() const { return m_nSolved; }
	uchar	number(int ix) const { return m_number[ix]; }
	std::string	text0() const;
	std::string	text() const;
	std::string	text2() const;
	std::string	textLink2() const;
	bool	isConsecutive() const;		//	問題に数字が連続して並んでいるか？
	bool	doesIncludeMeaninglessLink() const;			//	コの字型のパスを含むか？
public:
	void	setSize(int h, int v = 0);
	void	setNumber(cchar *ptr);
	void	setAddNumber(int sum, cchar *ptr);
	void	setNumber(int ix, uchar v) { m_number[ix] = v; }
	void	xorLink(int ix, uchar v) { m_link[ix] ^= v; }
	void	orLink(int ix, uchar v) { m_link[ix] |= v; }
	
	bool	doSolve();
	bool	doSolve(int ix, int x, int y);		//	ix の状態を決める
	bool	doSolve2();
	bool	doSolve2(int ix, int x, int y);		//	ix の状態を決める
	bool	checkLink();						//	出来上がったリンクが正しいかどうかをチェック
	bool	isUniq();							//	解がユニーク？
	void	isUniq(int ix, int x, int y);		//	ix の状態を決める
	void	setRandom(int N);
	void	genRandom(int N);
	void	genLinked(int N);		//	リンクされたN種数字を生成
private:
	//bool	m_solved;
	int		m_type;
	int		m_nHorz;
	int		m_nVert;
	int		m_value;	//	A + B or A - B, > 0
	int		m_nSolved;
	std::vector<uchar>	m_number;	//	問題
	std::vector<uchar>	m_work;		//	作業領域
	std::vector<uchar>	m_link;		//	LINK_XXX の組み合わせ
	std::vector<uchar>	m_link2;	//	LINK_XXX の組み合わせ
};
