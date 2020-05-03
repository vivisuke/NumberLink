#pragma once

#include <vector>
#include <string>
#include <random>

typedef unsigned char uchar;
typedef const char cchar;

//	上下左右に番人を持つ盤面クラス
class Board2 {
public:
	enum {
		EMPTY = 0,
		WALL = -1,		//	番人
	};
public:
	Board2(int width = 4, int height = 4);
public:
	int		xyToIndex(int x, int y) const {		//	x: [1, width], y: [1, height]
		return y * m_aryWidth + x;
	}
	int		indexToX(int ix) const {
		return ix % m_aryWidth;
	}
	int		indexToY(int ix) const {
		return ix / m_aryWidth;
	}
	bool	isAdjoining() const;			//	隣接する数字があるか？
	bool	isAnsCrowded() const;			//	同じ数字が４つ密集している箇所があるか？
	bool	isFilled() const;				//	リンクが全セルを通っている？
	//bool	isSvCrowded() const;			//	同じ数字が４つ密集している箇所があるか？
	int		nSolved() const { return m_nSolved; }
	std::string	qtext() const;				//	問題形式テキスト
	std::string	text0() const;
	std::string	text() const;
	std::string	textnl() const;				//	number, link 内容を表示
	std::string	textwl() const;				//	workAns, linkAns 内容を表示
	std::string	textWork();					//	work, link 内容を表示
public:
	void	setSize(int width, int height = 0);		//	height == 0 の場合は正方形
	void	setNumbers(cchar *ptr);
	void	clear();
	void	setRandom(int NTP);						//	[1, NTP]*2 の数字をランダムに配置
	void	setRandomNoAdj(int NTP);				//	隣接しない [1, NTP]*2 の数字をランダムに配置
	void	genRandomLink(int NTP);					//	[1, NTP] の数字をランダムに配置し、そこからリンクをランダムに伸ばす
	void	genRandomLink2(int NTP);				//	[1, NTP] の数字2個をランダムに配置
	void	genRandomLink3(int NTP);				//	[1, NTP] の数字2個をランダムに配置、コーナーなどの確定するリンクも配置
	void	setCornerLink(int ix, int link1, int link2);
	void	genUniqRandom(int NTP);
	
	bool	canSolve();				//	解があるかどうかを探索
	bool	canSolve(int ix);		//	解があるかどうかを探索
	bool	checkLink();			//	数字が正しくコネクトされ、全部の空欄を埋めているかをチェック
	bool	checkUniq();
	void	checkUniq(int ix);
	
	bool	checkUniqMate();		//	Mate アルゴリズムによるユニークチェック
	void	doLink(int ix1, int ix2);
	
private:
	int		m_width;				//	盤面横サイズ
	int		m_height;
	int		m_aryWidth;				//	番人を加えた横サイズ
	int		m_aryHeight;
	int		m_arySize;				//	盤面配列サイズ
	int		m_ntp;					//	数字種類数
	int		m_nSolved;				//	解の数、ただし3以上はカウントしない
	std::vector<char>	m_number;	//	問題セル状態、0 for 空白、[1,..] 数字
	std::vector<char>	m_work;		//	問題セル状態、0 for 空白、[1,..] 数字
	std::vector<char>	m_workAns;	//	解保存用
	std::vector<char>	m_link1;	//	連結方向 {-m_aryWidth | -1 | +1 | +m_aryWidth}, o for No Link
	std::vector<char>	m_link2;	//	
	std::vector<char>	m_link1Ans;	//	解保存用
	std::vector<char>	m_link2Ans;	//	
};
