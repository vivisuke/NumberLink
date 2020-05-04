#include "BoardK.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <assert.h>

using namespace std;

//#define		FALSE		0
//#define		TRUE		1

enum {
	LINK_UP,
	LINK_DOWN,
	LINK_LEFT,
	LINK_RIGHT,
};

extern random_device	g_rd;
extern mt19937	g_mt;

int	g_count = 0;


string vucToText(const vector<uchar> &v)
{
	string txt;
	for(auto c : v) txt += c < 10 ? c + '0' : c - 10 + 'A';
	return txt;
}
string vcToText(const vector<char> &v)
{
	string txt;
	for(auto c : v) txt += c < 0 ? '-' : c < 10 ? c + '0' : c - 10 + 'A';
	return txt;
}
//----------------------------------------------------------------------
BoardK::BoardK(int width, int height)
{
	setSize(width, height);
}
void BoardK::setSize(int width, int height)		//	height == 0 の場合は正方形
{
	m_aryWidth = (m_width = width) + 1;
	m_aryHeight = (m_height = height) + 2;
	m_arySize = m_aryWidth * m_aryHeight;
	m_number.resize(m_arySize);
	m_work.resize(m_arySize);
	m_mate.resize(m_arySize);
	m_linkRt.resize(m_arySize);
	m_linkDn.resize(m_arySize);
	clear();
}
void BoardK::setNumbers(cchar *ptr)
{
	m_ntp = 0;
	int num;
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			const auto ch = *ptr++;
			if( ch >= '0' && ch <= '9' )
				num = ch - '0';
			else if( ch >= 'A' && ch <= 'Z' )
				num = ch - 'A' + 10;
			else if( ch >= 'a' && ch <= 'z' )
				num = ch - 'a' + 10;
			else
				num = 0;
			m_number[xyToIndex(x, y)] = num;
			m_ntp = max(m_ntp, num);
		}
	}
}
void BoardK::clear()
{
	m_ntp = 0;
	for(auto &x : m_number) x = WALL;		//	WALL で初期化
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			m_number[xyToIndex(x, y)] = EMPTY;
		}
	}
	for(auto &x : m_linkRt) x = false;		//	0 で初期化
	for(auto &x : m_linkDn) x = false;		//	0 で初期化
}
bool BoardK::isAdjoining() const			//	隣接する数字があるか？
{
	for (int i = xyToIndex(1,1); i <= xyToIndex(m_width, m_height); ++i) {
		auto n = m_number[i];
		if( n > EMPTY &&		//	番人（-1）、空欄を除外
				(m_number[i-m_aryWidth] == n || m_number[i-1] == n || 
				m_number[i+1] == n || m_number[i+m_aryWidth] == n) )
		{
			return true;
		}
	}
	return false;
}
bool BoardK::isAnsCrowded() const				//	同じ数字が４つ密集している箇所があるか？
{
	for (int i = xyToIndex(1,1); i <= xyToIndex(m_width-1, m_height-1); ++i) {
		auto n = m_workAns[i];
		if( n > EMPTY &&		//	番人（-1）、空欄を除外
			m_workAns[i+1] == n &&
			m_workAns[i+m_aryWidth] == n && 
			m_workAns[i+m_aryWidth+1] == n )
		{
			return true;
		}
	}
	return false;
}
bool BoardK::isFilled() const				//	リンクが全セルを通っている？
{
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_workAns[ix] == 0 )
				return false;
		}
	}
	return true;
}
std::string BoardK::qtext() const				//	問題形式テキスト
{
	string txt = "\"";
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			int num = m_number[xyToIndex(x, y)];
			if( !num )
				txt += ".";
			else
				txt += '0' + num;
		}
	}
	txt += "\",\n";
	return txt;
}
std::string BoardK::text() const
{
	string txt;
	for (int y = 1; y <= m_height; ++y) {
		txt += " ";
		for (int x = 1; x <= m_width; ++x) {
			int num = m_number[xyToIndex(x, y)];
			if( !num )
				txt += ".";
			else if( num < 10 )
				txt += '0' + num;
			else
				txt += 'A' + num - 10;
			txt += " ";
		}
		txt += "\n";
	}
	return txt;
}
std::string BoardK::textnl() const				//	number, link 内容を表示
{
	string txt;
	for (int y = 1; y <= m_height; ++y) {
		txt += " ";
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			int num = m_number[ix];
			if( !num )
				txt += ".";
			else
				txt += '0' + num;
			if( m_linkRt[ix] )
				txt += "-";
			else
				txt += " ";
		}
		txt += "\n ";
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_linkDn[ix] )
				txt += "| ";
			else
				txt += "  ";
		}
		txt += "\n";
	}
	return txt;
}
std::string BoardK::mate() const				//	mate 内容を表示
{
	string txt;
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			string t = to_string((int)m_mate[ix]);
			while( t.size() < 4 ) t = " " + t;		//	４桁
			txt += t;
			if( !m_mate[ix] )
				txt += "(0)";
			else
				txt += "(" + to_string((int)m_number[m_mate[ix]]) + ")";
		}
		txt += "\n";
	}
	return txt;
}
std::string BoardK::textwl() const				//	work, link 内容を表示
{
	string txt;
	for (int y = 1; y <= m_height; ++y) {
		txt += " ";
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			int num = m_number[ix] ? m_number[ix] : m_workAns[ix];
			if( !num )
				txt += ".";
			else
				txt += '0' + num;
			if( m_linkRtAns[ix] )
				txt += "-";
			else
				txt += " ";
		}
		txt += "\n ";
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_linkDnAns[ix] )
				txt += "| ";
			else
				txt += "  ";
		}
		txt += "\n";
	}
	return txt;
}
std::string BoardK::textWork()			//	work, linkAns 内容を表示
{
	m_workAns = m_work;
	m_linkRtAns = m_linkRt;
	m_linkDnAns = m_linkDn;
	return textwl();
}
void BoardK::setRandom(int NTP)						//	[1, NTP]*2 の数字をランダムに配置
{
	clear();
	m_ntp = NTP;
	int ix;
	for (int n = 1; n <= NTP; ++n) {
		while( m_number[ix = g_mt() % m_arySize] != EMPTY ) {}
		m_number[ix] = n;
		while( m_number[ix = g_mt() % m_arySize] != EMPTY ) {}
		m_number[ix] = n;
	}
}
void BoardK::setRandomNoAdj(int NTP)				//	隣接しない [1, NTP]*2 の数字をランダムに配置
{
	for (;;) {
		setRandom(NTP);
		if( !isAdjoining() )
			return;
	}
}
void BoardK::genRandomLink(int NTP)					//	[1, NTP] の数字をランダムに配置し、そこからリンクをランダムに伸ばす
{
	clear();
	m_ntp = NTP;
	vector<int> tp;		//	端点リスト
	int ix;
	for (int n = 1; n <= NTP; ++n) {
		while( m_number[ix = g_mt() % m_arySize] != EMPTY ) {}
		m_number[ix] = n;
		tp.push_back(ix);
	}
	m_work = m_number;
	vector<int> vl;				//	移動可能方向を格納配列
	while( !tp.empty() ) {		//	端点が存在する間ループ
		int r = g_mt() % tp.size();
		int ix = tp[r];					//	ix = 端点位置
		int num = m_work[ix];
		vl.clear();
		if( m_work[ix-m_aryWidth] == 0 ) vl.push_back(LINK_UP);
		if( m_work[ix-1] == 0 ) vl.push_back(LINK_LEFT);
		if( m_work[ix+1] == 0 ) vl.push_back(LINK_RIGHT);
		if( m_work[ix+m_aryWidth] == 0 ) vl.push_back(LINK_DOWN);
		if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
			tp.erase(tp.begin() + r);
		} else {
			switch( vl[g_mt() % vl.size()] ) {
			case LINK_UP:
				m_linkDn[ix-=m_aryWidth] = true;
				break;
			case LINK_DOWN:
				m_linkDn[ix] = true;
				ix += m_aryWidth;
				break;
			case LINK_LEFT:
				m_linkRt[--ix] = true;
				break;
			case LINK_RIGHT:
				m_linkRt[ix++] = true;
				break;
			}
			tp[r] = ix;
			m_work[ix] = num;
			//m_linkRt[ix] = -link;
		}
		//cout << textwl() << "\n";
	}
}
void BoardK::genRandomLink2(int NTP)				//	[1, NTP] の数字2個をランダムに配置
{
	clear();
	m_ntp = NTP;
	vector<int> tp;		//	端点リスト
	int ix;
	vector<int> vl;				//	移動可能方向を格納配列
	for (int n = 1; n <= NTP; ++n) {
		while( m_number[ix = g_mt() % m_arySize] != EMPTY ) {}
		m_number[ix] = n;
		tp.push_back(ix);
		vl.clear();
		if( m_number[ix-m_aryWidth] == 0 ) vl.push_back(-m_aryWidth);
		if( m_number[ix-1] == 0 ) vl.push_back(-1);
		if( m_number[ix+1] == 0 ) vl.push_back(+1);
		if( m_number[ix+m_aryWidth] == 0 ) vl.push_back(+m_aryWidth);
		if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
			return;				//	隣接して配置不能
		}
		int link = vl[g_mt() % vl.size()];
		m_linkRt[ix] = link;
		m_number[ix+=link] = n;
		m_linkRt[ix] = -link;
		tp.push_back(ix);
	}
	m_work = m_number;
	while( !tp.empty() ) {		//	端点が存在する間ループ
		int r = g_mt() % tp.size();
		int ix = tp[r];					//	ix = 端点位置
		int num = m_work[ix];
		vl.clear();
		if( m_work[ix-m_aryWidth] == 0 ) {
			int t = m_work[ix+m_linkRt[ix]-m_aryWidth];
			if( m_linkRt[ix] == 0 ||		//	リンクが無い場合
				m_linkRt[ix] == +m_aryWidth ||		//	直進の場合
				/*m_work[ix+m_linkRt[ix]] != num ||*/ m_work[ix+m_linkRt[ix]-m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(-m_aryWidth);
			}
		}
		if( m_work[ix-1] == 0 ) {
			int t = m_work[ix+m_linkRt[ix]-1];
			if( m_linkRt[ix] == 0 ||
				m_linkRt[ix] == +1 ||		//	直進の場合
				/*m_work[ix+m_linkRt[ix]] != num ||*/ m_work[ix+m_linkRt[ix]-1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(-1);
			}
		}
		if( m_work[ix+1] == 0 ) {
			int t = m_work[ix+m_linkRt[ix]+1];
			if( m_linkRt[ix] == 0 ||
				m_linkRt[ix] == -1 ||		//	直進の場合
				/*m_work[ix+m_linkRt[ix]] != num ||*/ m_work[ix+m_linkRt[ix]+1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+1);
			}
		}
		if( m_work[ix+m_aryWidth] == 0 ) {
			int t = m_work[ix+m_linkRt[ix]+m_aryWidth];
			if( m_linkRt[ix] == 0 ||
				m_linkRt[ix] == -m_aryWidth ||		//	直進の場合
				/*m_work[ix+m_linkRt[ix]] != num ||*/ m_work[ix+m_linkRt[ix]+m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+m_aryWidth);
			}
		}
		if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
			tp.erase(tp.begin() + r);
		} else {
			int link = vl[g_mt() % vl.size()];
			if( !m_linkRt[ix] )
				m_linkRt[ix] = link;
			else
				m_linkDn[ix] = link;
			tp[r] = ix += link;
			m_work[ix] = num;
			m_linkRt[ix] = -link;
		}
		//cout << textwl() << "\n";
	}
}
#define		DEBUG_OUT		1
//	[1, NTP] の数字2個をランダムに配置、コーナーなどの確定するリンクも配置
void BoardK::genRandomLink3(int NTP, int *ptr)
{
	clear();
	m_work = m_number;
	m_mate.resize(m_number.size());
	for (int i = 0; i < m_mate.size(); ++i) m_mate[i] = i;		//	全部非連結
	m_ntp = NTP;
	vector<int> tp;		//	端点リスト
	int ix;
	vector<int> vl;				//	移動可能方向を格納配列
	//	隣接する数字をNTP個ランダムに配置
	for (int n = 1; n <= NTP; ++n) {
		if (ptr == nullptr) {
			while (m_number[ix = g_mt() % m_arySize] != EMPTY) {}
			//if (n == 1) ix = xyToIndex(2, 1);	//	for Test
		} else
			ix = *ptr++;
		m_work[ix] = m_number[ix] = n;
		tp.push_back(ix);
	}
	//	数字が空コーナーの隣の場合は、コーナーと連結
	//	そうでなければ、ランダムな方向に連結
	vector<int> tp2;		//	端点リスト
	for(auto ix : tp) {
		tp2.push_back(ix);
		const int num = m_number[ix];
		int x = indexToX(ix);
		int y = indexToY(ix);
		//	ix が空コーナーの隣の場合は、コーナーと連結
		int ix2;
		if( y == 1 && x == 2 && m_number[xyToIndex(1, 1)] == 0 ) {
			m_linkRt[ix2 = xyToIndex(1, 1)] = true;
		} else if( y == 1 && x == m_width - 1 && m_number[xyToIndex(m_width, 1)] == 0 ) {
			m_linkRt[ix2 = xyToIndex(m_width-1, 1)] = true;
		} else if( y == 2 && x == 1 && m_number[xyToIndex(1, 1)] == 0 ) {
			m_linkDn[ix2 = xyToIndex(1, 1)] = true;
		} else if( y == 2 && x == m_width - 1 && m_number[xyToIndex(m_width, 1)] == 0 ) {
			m_linkDn[ix2 = xyToIndex(m_width,1)] = true;
		} else if( y == m_height - 1 && x == 1 && m_number[xyToIndex(1, m_height)] == 0 ) {
			m_linkDn[ix2 = xyToIndex(1, m_height-1)] = true;
		} else if( y == m_height - 1 && x == m_width - 1 && m_number[xyToIndex(m_width, m_height)] == 0 ) {
			m_linkDn[ix2 = xyToIndex(m_width, m_height-1)] = true;
		} else if( y == m_height && x == 2 && m_number[xyToIndex(1, m_height)] == 0 ) {
			m_linkRt[ix2 = xyToIndex(1, m_height)] = true;
		} else if( y == m_height && x == m_width - 1 && m_number[xyToIndex(m_width, m_height)] == 0 ) {
			m_linkRt[ix2 = xyToIndex(m_width-1, m_height)] = true;
		} else {
			vl.clear();
			if( m_number[ix-m_aryWidth] == 0 ) vl.push_back(LINK_UP);
			if( m_number[ix-1] == 0 ) vl.push_back(LINK_LEFT);
			if( m_number[ix+1] == 0 ) vl.push_back(LINK_RIGHT);
			if( m_number[ix+m_aryWidth] == 0 ) vl.push_back(LINK_DOWN);
			if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
				return;				//	隣接して配置不能
			}
			switch( vl[g_mt() % vl.size()] ) {
			case LINK_UP:
				m_linkDn[ix2 = ix-m_aryWidth] = true;
				break;
			case LINK_DOWN:
				m_linkDn[ix] = true;
				ix2 = ix + m_aryWidth;
				break;
			case LINK_LEFT:
				m_linkRt[ix2 = ix - 1] = true;
				break;
			case LINK_RIGHT:
				m_linkRt[ix] = true;
				ix2 = ix + 1;
				break;
			}
		}
		m_work[ix2] = num;
		m_mate[ix] = ix2;
		m_mate[ix2] = ix;
		tp2.push_back(ix);
	}
	tp = tp2;

#if		DEBUG_OUT
	cout << textWork();
	cout << "\n";
#endif
	setCornerLink(xyToIndex(1,1), +1, +m_aryWidth);
	setCornerLink(xyToIndex(m_width,1), -1, +m_aryWidth);
	setCornerLink(xyToIndex(1,m_height), +1, -m_aryWidth);
	setCornerLink(xyToIndex(m_width,m_height), -1, -m_aryWidth);
#if		DEBUG_OUT
	cout << textWork();
	cout << "\n";
#endif
	//	端点が存在する間ループ
	//	既にリンクがある場合は、それと接続
	int lc = 0;		//	ループカウンタ
	while( !tp.empty() ) {
		//cout << textWork() << "\n";
		int r = lc++;
		if( r >= tp.size() )
			r = g_mt() % tp.size();
		
		int ix = tp[r];					//	ix = 端点位置
		int num = m_work[ix];
		vl.clear();
		if( m_work[ix-m_aryWidth] == 0 ) {
			//int t = m_work[ix+m_linkRt[ix]-m_aryWidth];
			if( !m_linkRt[ix] && !m_linkDn[ix] ||		//	リンクが無い場合
				!m_linkRt[ix] && m_linkDn[ix] ||		//	直進の場合
				m_work[ix+m_linkRt[ix]-m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(LINK_UP);
			}
		}
		if( m_work[ix-1] == 0 ) {
			int t = m_work[ix+m_linkRt[ix]-1];
			if( m_linkRt[ix] == 0 ||
				m_linkRt[ix] == +1 ||		//	直進の場合
				/*m_work[ix+m_linkRt[ix]] != num ||*/ m_work[ix+m_linkRt[ix]-1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(-1);
			}
		}
		if( m_work[ix+1] == 0 ) {
			int t = m_work[ix+m_linkRt[ix]+1];
			if( m_linkRt[ix] == 0 ||
				m_linkRt[ix] == -1 ||		//	直進の場合
				/*m_work[ix+m_linkRt[ix]] != num ||*/ m_work[ix+m_linkRt[ix]+1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+1);
			}
		}
		if( m_work[ix+m_aryWidth] == 0 ) {
			int t = m_work[ix+m_linkRt[ix]+m_aryWidth];
			if( m_linkRt[ix] == 0 ||
				m_linkRt[ix] == -m_aryWidth ||		//	直進の場合
				/*m_work[ix+m_linkRt[ix]] != num ||*/ m_work[ix+m_linkRt[ix]+m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+m_aryWidth);
			}
		}
		if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
			tp.erase(tp.begin() + r);
			continue;
		} else {
			int link = vl[g_mt() % vl.size()];
			if( !m_linkRt[ix] )
				m_linkRt[ix] = link;
			else
				m_linkDn[ix] = link;
			tp[r] = ix += link;
			m_work[ix] = num;
			if( m_linkRt[ix] == 0 )
				m_linkRt[ix] = -link;
			else {		//	既にリンクがある場合
				//	[ix0]-> [ix] <-->[ix2]
				m_linkDn[ix] = -link;		//	ix0, ix を相互にリンク
				link = m_linkRt[ix];
				for (;;) {
					ix += link;			//	次のセルに移動
					m_work[ix] = num;
					if( m_linkDn[ix] == 0 )
						break;		//	その次のセルへのリンクが無い場合
					if (m_linkRt[ix] == -link)
						link = m_linkDn[ix];
					else
						link = m_linkRt[ix];
				}
				tp[r] = ix;
			}
		}
		cout << textwl();
		cout << "\n";
	}
	m_workAns = m_work;
	m_linkRtAns = m_linkRt;
	m_linkDnAns = m_linkDn;
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_linkRtAns[ix] != 0 && m_linkDnAns[ix] != 0 )
				m_number[ix] = 0;
			else
				m_number[ix] = m_workAns[ix];
		}
	}
}
bool BoardK::forcedLink(std::vector<int>& tp)
{
	return false;
}
void BoardK::doLink(int ix1, int ix2)		//	should be ix1 < ix2
{
	if( ix1 > ix2 ) swap(ix1, ix2);
	//assert( ix1 < ix2 );
	if( ix2 == ix1 + 1 )
		m_linkRt[ix1] = true;
	else {
		assert( ix2 == ix1 + m_aryWidth );
		m_linkDn[ix1] = true;
	}
#if	0
	int d = ix2 - ix1;
	if( m_linkRt[ix1] == 0 )
		m_linkRt[ix1] = d;
	else
		m_linkDn[ix1] = d;
	if( m_linkRt[ix2] == 0 )
		m_linkRt[ix2] = -d;
	else
		m_linkDn[ix2] = -d;
#endif
}
void BoardK::setCornerLink(int ix, int dx, int dy)
{
	if( m_work[ix] != 0 ) return;
	int num;
	if( (num = m_work[ix+dx]) == 0 ) {
		doLink(ix, ix+dx);
		doLink(ix, ix+dy);
		if( (num = m_work[ix+dy]) != 0 ) {		//	空-空-数字 の場合
			m_work[ix] = m_work[ix+dx] = num;
		}
	} else if( m_work[ix+dy] == 0 ) {		//	数字-空-空 の場合
		doLink(ix, ix+dx);
		doLink(ix, ix+dy);
		m_work[ix] = m_work[ix+dy] = num;
	}
	//if( m_work[ix+dy] == 0 )
	//	doLink(ix, ix+dy);
#if	0
	if( m_number[ix] == 0 && m_number[ix+dx] == 0 && m_number[ix+dy] == 0 ) {
		doLink(ix, ix+dx);
		doLink(ix, ix+dy);
	}
#endif
#if	0
	if( m_number[ix] != 0 ) return;
	doLink(ix, ix+dx);
	doLink(ix, ix+dy);
	if( m_number[ix+dx] != 0 )		//	数字がある場合
		m_work[ix] = m_work[ix+dy] = m_number[ix+dx];
	else if( m_number[ix+dy] != 0 )		//	数字がある場合
		m_work[ix] = m_number[ix+dx] = m_number[ix+dy];
#endif
#if	0
	m_linkRt[ix] = dx;
	if( m_linkRt[ix+dx] == 0 )
		m_linkRt[ix+dx] = -dx;
	else
		m_linkDn[ix+dx] = -dx;
	if( m_number[ix+dx] != 0 )		//	数字がある場合
		m_work[ix] = m_number[ix+dx];
	m_linkDn[ix] = dy;
	if( m_linkRt[ix+dy] == 0 )
		m_linkRt[ix+dy] = -dy;
	else
		m_linkDn[ix+dy] = -dy;
	if( m_number[ix+dy] != 0 )		//	数字がある場合
		m_work[ix] = m_number[ix+dy];
#endif
#if	0
	if( m_number[ix+dx] == 0 ) {
		m_linkRt[ix] = dx;
		m_linkRt[ix+dx] = -dx;
	}
	if( m_number[ix+dy] == 0 ) {
		m_linkDn[ix] = dy;
		m_linkDn[ix+dy] = -dy;
	}
#endif
}
bool BoardK::doSolveBT()			//	バックトラッキング探索
{
	g_count = 0;
	for(auto &x : m_linkRt) x = 0;
	for(auto &x : m_linkDn) x = 0;
	for(auto &x : m_mate) x = 0;
	return doSolveBT(xyToIndex(1, 1));
}
bool BoardK::doSolveBT(int ix)		//	ix の状態を決める
{
	cout << "#" << ++g_count << ": ix = " << ix << "\n";
	cout << textnl() << "\n";
	cout << mate() << "\n";
	if( ix > xyToIndex(m_width, m_height) ) {		//	全てのセルを無事埋めた場合
		return true;
	}
	if( m_number[ix] == WALL ) ++ix;		//	番人をスキップ
	const bool rtEdge = m_number[ix+1] == WALL;		//	右端か？
	const bool btmEdge = m_number[ix+m_aryWidth] == WALL;		//	下端か？
	//const bool up = m_linkRt[ix-m_aryWidth] == m_aryWidth || m_linkDn[ix-m_aryWidth] == m_aryWidth;	//	上からリンクが来ている
	//const bool left = m_linkRt[ix-1] == 1 || m_linkDn[ix-1] == 1;		//	左からリンクが来ている
	const bool up = m_linkDn[ix-m_aryWidth];	//	上からリンクが来ている
	const bool left = m_linkRt[ix-1];			//	左からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up || left )	//	上方向 or 左方向から既にリンク済みの場合
			return doSolveBT(ix+1);
		if( m_number[ix+1] == m_number[ix] ) {		//	右が同じ数字の場合
			m_linkRt[ix] = true;
			if( doSolveBT(ix+1) )
				return true;
			m_linkRt[ix] = false;
		} else if( m_number[ix+m_aryWidth] == m_number[ix] ) {		//	下が同じ数字の場合
			m_linkDn[ix] = true;
			if( doSolveBT(ix+1) )
				return true;
			m_linkDn[ix] = false;
		} else {
			auto num = m_number[ix];
			if( m_number[ix+1] == 0 ) {		//	右が空欄の場合
				if( m_mate[ix+1] == 0 ) {	//	右が既にどこかとリンクしていない場合
					m_linkRt[ix] = true;
					m_mate[ix+1] = ix;
					m_mate[ix] = ix + 1;
					if( doSolveBT(ix+1) )
						return true;
					m_linkRt[ix] = false;
					m_mate[ix+1] = m_mate[ix] = 0;
				} else if( m_number[m_mate[ix+1]] == 0 || m_number[m_mate[ix+1]] == num ) {
					m_linkRt[ix] = true;
					m_mate[m_mate[ix+1]] = ix;
					m_mate[ix] = m_mate[ix+1];
					if( doSolveBT(ix+1) )
						return true;
					m_linkRt[ix] = false;
					m_mate[m_mate[ix+1]] = ix + 1;
					m_mate[ix] = 0;
				}
			}
			if( m_number[ix+m_aryWidth] == 0 ) {		//	下が空欄の場合
				//	m_mate[ix + m_aryWidth] は必ず０のはず
				//if( m_mate[ix+m_aryWidth] == 0 ) {
					m_linkDn[ix] = true;
					m_mate[ix+m_aryWidth] = ix;
					m_mate[ix] = ix + m_aryWidth;
					if( doSolveBT(ix+1) )
						return true;
					m_linkDn[ix] = false;
					m_mate[ix+m_aryWidth] = m_mate[ix] = 0;
				//}
			}
		}
		return false;
	} else {	//	セルに数字が無い場合
		int num = m_mate[ix] == 0 ? 0 : m_number[m_mate[ix]];	//	当該セルが端点の場合、逆端点数字
		if( up && left ) {		//	上・左両方からリンクが来ている
			//	既に┘が入り、チェック済みのはず
			return doSolveBT(ix+1);
		} else if( up ) {		//	上からリンクが来ている → 必ず端点になっている、└ or │を入れる
			if( !rtEdge ) {		//	右端でない場合
				if( m_number[ix+1] != 0 ) {			//	右に数字がある場合
					if( !num || m_number[ix+1] == num ) {	//	右に数字あり、上側の端点数字と一致する場合
						m_linkRt[ix] = true;		//	右にリンク
						m_mate[m_mate[ix]] = ix + 1;
						m_mate[ix+1] = m_mate[ix];
						if( doSolveBT(ix+1) )
							return true;
						m_mate[m_mate[ix]] = ix;
						m_mate[ix+1] = 0;
					}
				} else if( m_mate[ix+1] == 0 ) {		//	右が端点でない場合
					m_linkRt[ix] = true;		//	右にリンク
					m_mate[ix+1] = m_mate[ix];
					m_mate[m_mate[ix]] = ix + 1;
					if( doSolveBT(ix+1) )
						return true;
					m_mate[ix + 1] = 0;
					m_mate[m_mate[ix]] = ix;
				} else if( m_number[m_mate[ix+1]] == num ) {		//	右が端点で、同じ数字の場合
					if( !num ) return false;		//	空ループの場合
					m_linkRt[ix] = true;		//	右にリンク
					//	ソルバーとしては mate[] を更新する必要はない（はず）
					if( doSolveBT(ix+1) )
						return true;
				}
				m_linkRt[ix] = false;		//	リンク削除
			}
			if( !btmEdge ) {	//	下端でない場合
				if( m_number[ix+m_aryWidth] != 0 ) {			//	下に数字がある場合
					if( !num || m_number[ix+m_aryWidth] == num ) {	//	リンク末端に数字が無いか数字が一致している場合
						m_linkDn[ix] = true;		//	下にリンク
						m_mate[m_mate[ix]] = ix + m_aryWidth;
						m_mate[ix+ m_aryWidth] = m_mate[ix];
						if( doSolveBT(ix+1) )
							return true;
						m_mate[m_mate[ix]] = ix;
						m_mate[ix+ m_aryWidth] = 0;
					}
				} else {
					m_linkDn[ix] = true;		//	下にリンク
					m_mate[ix+m_aryWidth] = m_mate[ix];
					m_mate[m_mate[ix]] = ix + m_aryWidth;
					if( doSolveBT(ix+1) )
						return true;
					m_mate[ix + m_aryWidth] = 0;
					m_mate[m_mate[ix]] = ix;
				}
				m_linkDn[ix] = false;		//	リンク削除
			}
			return false;
		} else if( left ) {		//	左からリンクが来ている
			if( !rtEdge ) {		//	右端でない場合
				if( m_number[ix+1] != 0 ) {			//	右に数字がある場合
					if( !m_mate[ix+1] && (!num || (m_mate[ix] != ix + 1 && m_number[ix+1] == num)) )
					{	//	既にリンクが無く、右に数字あり、左側の端点数字と一致する場合
						m_linkRt[ix] = true;		//	右にリンク
						m_mate[m_mate[ix]] = ix + 1;
						m_mate[ix+1] = m_mate[ix];
						if( doSolveBT(ix+1) )
							return true;
						m_mate[m_mate[ix]] = ix;
						m_mate[ix+1] = 0;
					}
				} else if( m_mate[ix+1] == 0 ) {		//	右が端点でない場合
					m_linkRt[ix] = true;		//	右にリンク
					m_mate[m_mate[ix]] = ix + 1;		//	左側の端点の mate[] 更新
					m_mate[ix+1] = m_mate[ix];			//	右側の端点の mate[] 更新
					if( doSolveBT(ix+1) )
						return true;
					m_mate[ix+1] = 0;				//	右側の端点の mate[] を元に戻す
					m_mate[m_mate[ix]] = ix;		//	左側の端点の mate[] を元に戻す
				} else if( m_number[m_mate[ix+1]] == num ) {		//	右が端点で、同じ数字の場合
					if (!num) return false;		//	空ループの場合
					m_linkRt[ix] = true;		//	右にリンク
					//	ソルバーとしては mate[] を更新する必要はない（はず）
					if( doSolveBT(ix+1) )
						return true;
				}
				m_linkRt[ix] = false;		//	右リンク削除
			}
			if( !btmEdge ) {		//	下端でない場合
				if( m_number[ix+m_aryWidth] != 0 ) {			//	下に数字がある場合
					if( !num || m_number[ix+m_aryWidth] == num ) {	//	リンク末端に数字が無いか数字が一致している場合
						m_linkDn[ix] = true;		//	下にリンク
						m_mate[m_mate[ix]] = ix + m_aryWidth;
						m_mate[ix+ m_aryWidth] = m_mate[ix];
						if( doSolveBT(ix+1) )
							return true;
						m_mate[m_mate[ix]] = ix;
						m_mate[ix+ m_aryWidth] = 0;
					}
				} else {
					m_linkDn[ix] = true;		//	下にリンク
					m_mate[m_mate[ix]] = ix + m_aryWidth;		//	左側の端点の mate[] 更新
					m_mate[ix+m_aryWidth] = m_mate[ix];			//	下側の端点の mate[] 更新
					if( doSolveBT(ix+1) )
						return true;
					m_mate[m_mate[ix]] = ix;		//	左側の端点の mate[] を元に戻す
					m_mate[ix+m_aryWidth] = 0;
				}
				m_linkDn[ix] = false;		//	リンク削除
			}
		} else {		//	上・左両方ともリンクが来ていない
			if( rtEdge || btmEdge ) return false;	//	右端 or 下端の場合はリンク不可
			if( m_number[ix+1] != 0 && m_mate[ix+1] ) return false;	//	右に連結済み数字がある
			if( m_number[ix+m_aryWidth] == 0 ||		//	下に数字が無い
				m_number[ix+1] == m_number[ix+m_aryWidth] ||	//	右と下の数字が同一
				m_mate[ix+1] == 0 ||							//	右がリンク末端ではない
				m_number[m_mate[ix+1]] == m_number[ix+m_aryWidth] )		//	右のリンクの数字が下の数字と等しい
			{
				m_linkRt[ix] = m_linkDn[ix] = true;		//	右・下にリンク
				m_mate[ix+1] = ix + m_aryWidth;			//	ix の右・下が結合された端点である情報保持
				m_mate[ix+m_aryWidth] = ix + 1;
				if( doSolveBT(ix+1) )
					return true;
				m_mate[ix+1] = m_mate[ix+m_aryWidth] = 0;
				m_linkRt[ix] = m_linkDn[ix] = false;		//	右・下リンク削除
			}
		}
	}
	return false;
}
bool BoardK::canSolve()				//	解があるかどうかを探索
{
	for(auto &x : m_linkRt) x = 0;
	for(auto &x : m_linkDn) x = 0;
	return canSolve(xyToIndex(1, 1));
}
bool BoardK::canSolve(int ix)		//	解があるかどうかを探索
{
	if( ix > xyToIndex(m_width, m_height) ) {
		//cout << textwl() << "\n";
		if( checkLink() ) {
			m_workAns = m_work;
			m_linkRtAns = m_linkRt;
			m_linkDnAns = m_linkDn;
			return true;
		} else
			return false;
	}
	if( m_number[ix] == WALL ) ++ix;		//	番人をスキップ
	const bool rtEdge = m_number[ix+1] == WALL;
	const bool btmEdge = m_number[ix+m_aryWidth] == WALL;
	const bool up = m_linkRt[ix-m_aryWidth] == m_aryWidth || m_linkDn[ix-m_aryWidth] == m_aryWidth;	//	上からリンクが来ている
	const bool left = m_linkRt[ix-1] == 1 || m_linkDn[ix-1] == 1;		//	左からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return false;		//	接続不可
		if( up ) {
			m_linkRt[ix] = -m_aryWidth;
			m_linkDn[ix] = 0;
			return canSolve(ix+1);
		}
		if( left ) {
			m_linkRt[ix] = -1;
			m_linkDn[ix] = 0;
			return canSolve(ix+1);
		}
		if( !rtEdge ) {
			m_linkRt[ix] = +1;
			m_linkDn[ix] = 0;
			if( canSolve(ix+1) ) return true;
		}
		if( !btmEdge ) {
			m_linkRt[ix] = m_aryWidth;
			m_linkDn[ix] = 0;
			return canSolve(ix+1);
		}
		return false;
	} else {
		if( up && left ) {
			m_linkRt[ix] = -m_aryWidth;
			m_linkDn[ix] = -1;
			return canSolve(ix+1);
		}
		if( up ) {
			if( !rtEdge ) {
				m_linkRt[ix] = -m_aryWidth;
				m_linkDn[ix] = +1;
				if( canSolve(ix+1) ) return true;
			}
			if( !btmEdge ) {
				m_linkRt[ix] = -m_aryWidth;
				m_linkDn[ix] = +m_aryWidth;
				return canSolve(ix+1);
			}
			return false;
		}
		if( left ) {
			if( !rtEdge ) {
				m_linkRt[ix] = -1;
				m_linkDn[ix] = +1;
				if( canSolve(ix+1) ) return true;
			}
			if( !btmEdge ) {
				m_linkRt[ix] = -1;
				m_linkDn[ix] = +m_aryWidth;
				return canSolve(ix+1);
			}
			return false;
		}
		if( rtEdge || btmEdge ) return false;
		m_linkRt[ix] = +1;
		m_linkDn[ix] = +m_aryWidth;
		return canSolve(ix+1);
	}
}
bool BoardK::checkLink()			//	数字が正しくコネクトされ、全部の空欄を埋めているかをチェック
{
	vector<char> b(m_ntp, 0);	//	探索済みフラグ
	//for(auto &x : m_work) x = 0;
	m_work = m_number;			//	壁も含めてコピー
	for (int ix = xyToIndex(1,1); ix <= xyToIndex(m_width, m_height); ++ix) {
		int n = m_number[ix];
		if( n <= EMPTY ) continue;		//	空欄・番人の場合
		if( b[n-1] ) continue;			//	既に探索済み
		b[n-1] = true;					//	探索済みフラグ ON
		m_work[ix] = n;
		auto link = m_linkRt[ix];
		if( !link ) return false;		//	リンクが無い場合
		int i = ix + link;				//	リンク先に移動
		for (;;) {
			m_work[i] = n;
			if( m_number[i] == n )		//	正しい数字に到達した場合
				break;
			if( m_number[i] != 0 )		//	異なる数字に到達した場合
				return false;
			if( m_linkRt[i] != -link )
				link = m_linkRt[i];
			else
				link = m_linkDn[i];
			if( !link ) return false;
			i += link;
		}
	}
	for(int ix = xyToIndex(1,1); ix <= xyToIndex(m_width, m_height); ++ix) {
		if( m_work[ix] == 0 )
			return false;		//	網羅していないセルがある場合
	}
	return true;
}
void BoardK::genUniqRandom(int NTP)
{
	for (;;) {
		setRandom(NTP);
		m_work = m_number;
		if( checkUniq() ) {
			//m_linkRt = m_linkRtSv;
			//m_linkDn = m_linkDnSv;
			return;
		}
	}
}
bool BoardK::checkUniq()
{
	m_nSolved = 0;
	for(auto &x : m_linkRt) x = 0;
	for(auto &x : m_linkDn) x = 0;
	checkUniq(xyToIndex(1, 1));
	return m_nSolved == 1;
}
void BoardK::checkUniq(int ix)
{
	//cout << textnl() << "\n";
	if( ix > xyToIndex(m_width, m_height) ) {
		if( checkLink() ) {
			m_workAns = m_work;
			m_linkRtAns = m_linkRt;
			m_linkDnAns = m_linkDn;
			//cout << textnl() << "\n";
			++m_nSolved;
		}
		return;
	}
	if( m_number[ix] == WALL ) ++ix;		//	番人をスキップ
	bool rtEdge = m_number[ix+1] == WALL;
	bool btmEdge = m_number[ix+m_aryWidth] == WALL;
	bool up = m_linkRt[ix-m_aryWidth] == m_aryWidth || m_linkDn[ix-m_aryWidth] == m_aryWidth;	//	上からリンクが来ている
	bool left = m_linkRt[ix-1] == 1 || m_linkDn[ix-1] == 1;		//	左からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return;		//	接続不可
		if( up ) {
			m_linkRt[ix] = -m_aryWidth;
			m_linkDn[ix] = 0;
			checkUniq(ix+1);
			return;
		}
		if( left ) {
			m_linkRt[ix] = -1;
			m_linkDn[ix] = 0;
			checkUniq(ix+1);
			return;
		}
		if( !rtEdge ) {
			m_linkRt[ix] = +1;
			m_linkDn[ix] = 0;
			checkUniq(ix+1);
			if( m_nSolved >= 2 ) return;
		}
		if( !btmEdge ) {
			m_linkRt[ix] = m_aryWidth;
			m_linkDn[ix] = 0;
			checkUniq(ix+1);
		}
	} else {
		if( up && left ) {
			m_linkRt[ix] = -m_aryWidth;
			m_linkDn[ix] = -1;
			checkUniq(ix+1);
			return;
		}
		if( up ) {
			if( !rtEdge ) {
				m_linkRt[ix] = -m_aryWidth;
				m_linkDn[ix] = +1;
				checkUniq(ix+1);
				if( m_nSolved >= 2 ) return;
			}
			if( !btmEdge ) {
				m_linkRt[ix] = -m_aryWidth;
				m_linkDn[ix] = +m_aryWidth;
				checkUniq(ix+1);
			}
			return;
		}
		if( left ) {
			if( !rtEdge ) {
				m_linkRt[ix] = -1;
				m_linkDn[ix] = +1;
				checkUniq(ix+1);
				if( m_nSolved >= 2 ) return;
			}
			if( !btmEdge ) {
				m_linkRt[ix] = -1;
				m_linkDn[ix] = +m_aryWidth;
				checkUniq(ix+1);
			}
			return;
		}
		if( rtEdge || btmEdge ) return;
		m_linkRt[ix] = +1;
		m_linkDn[ix] = +m_aryWidth;
		checkUniq(ix+1);
	}
}
class HashVC {  // ハッシュ関数オブジェクト
public:
    size_t operator()(const vector<char> &x) const {
        const int C = 997;      // 素数
        size_t t = 0;
        for (int i = 0; i != x.size(); ++i) {
            t = t * C + x[i];
        }
        return t;
    }
};
bool BoardK::checkUniqMate()		//	Mate アルゴリズムによるユニークチェック
{
	typedef unordered_map<vector<char>, int, HashVC> MapVCI;
	MapVCI mp;
	int n = m_number[xyToIndex(1, 1)];
	vector<char> v(1, n);
	mp[v] = 1;
	for (int k = 2; k <= m_width; ++k) {		//	k = x + y 
	}
	return false;
}
bool BoardK::canSolve() const		//	解が一つ以上存在するか？
{
	return numSolutions() != 0;
}
class HashVUC {  // ハッシュ関数オブジェクト
public:
    size_t operator()(const vector<uchar> &v) const {
        const int C = 997;      // 素数
        size_t t = 0;
        for(auto x : v) t = t * C + x;
        return t;
    }
};
typedef unordered_map<vector<uchar>, uchar, HashVUC> HashMate;		//	mate 用ハッシュタイプ
#define		DEBUG_OUT		0
int BoardK::numSolutions() const		//	解個数を返す
{
#if	DEBUG_OUT
	cout << "\n" << text() << "\n";
#endif
	HashMate mp;
	const int MID = 0;
	vector<uchar> mate0(m_width+2, 0);
	for (int i = 1; i <= m_width; ++i) mate0[i] = xyToIndex(i, 1);
	mp[mate0] = 1;		//	状態数 = 1
	for (int y = 1; y <= m_height; ++y) {
		int ix = xyToIndex(1, y);
		for (int x = 1; x <= m_width; ++x, ++ix) {
#if	DEBUG_OUT
			cout << "ix = " << ix << "\n";
			for (auto itr = mp.cbegin(); itr != mp.cend(); ++itr) {
				cout << vucToText(itr->first) << ":" << to_string((int)itr->second) << ", ";
			}
			cout << "\n";
#endif
			if( mp.empty() )
				return false;		//	mate がひとつも無ければ解無し
			HashMate mp2;	//	次の mate 集合
			for (auto itr = mp.cbegin(); itr != mp.cend(); ++itr) {
				const auto &mate = itr->first;
				const auto n = itr->second;
#if	DEBUG_OUT
				cout << " " << vucToText(itr->first) << ":" << to_string((int)itr->second) << ":\n";
#endif
				if( m_number[ix] != 0 ) {		//	セルに数字がある場合
					if (mate[x] == ix ) {	//	どこにも連結していない場合
						auto m = mate;
						if( y != m_height &&	//	下端ではない場合
							(m_number[ix+m_aryWidth] == 0 || m_number[ix+m_aryWidth] == m_number[ix]) )
						{
								//	下方向に連結
								m[x] = ix;				//	ix から下方向に連結
								mp2[m] += n;				//	ハッシュに追加、重複している場合は個数
						}
						if( x != m_width ) {		//	右端でない場合
							int num1 = m_number[ix+1];
							if( !num1 ) num1 = m_number[mate[x+1]];
							if (num1 == 0 || num1 == m_number[ix]) {
								assert( mate[x+1] >= 0 );
								//	右方向に連結
								m[x] = ix + m_aryWidth;	//	下方向には連結しない
								if( m[x+1] == ix + 1 ) {		//	右側が連結されていない場合
									m[x+1] = ix;		//	ix から右方向に連結
								} else {		//	右側が連結されている場合
									//	undone: 同じ数字チェック
									int t = m[x+1];			//	逆側の端点
									if( t > ix && t < ix + m_aryWidth )
										m[t % m_aryWidth] = ix;	//	逆側の端点を更新
									m[x+1] = MID;				//	途中点
								}
								mp2[m] += n;				//	ハッシュに追加、重複している場合は個数
							}
						}
					} else {	//	ix が連結済みの場合
						auto m = mate;
						m[x] = ix + m_aryWidth;	//	ix は連結済みなので、右・下方向には連結しない
						mp2[m] += n;				//	ハッシュに追加、重複している場合は個数
					}
				} else {		//	セルに数字が無い場合
					if (mate[x] == ix) {	//	どこにも連結していない場合
						if( x != m_width && y != m_height ) {		//	右端でも下端でもない場合
							//	右側が数字で連結済みであれば接続不可
							if( m_number[ix+1] == 0 || mate[x+1] == ix + 1 ) {
								int num1 = m_number[mate[x+1]];
								int num2 = m_number[ix+m_aryWidth];
								if( !num1 || !num2 || num1 == num2 ) {
									auto m = mate;
									assert( m[x+1] >= 0 );		//	途中点の場合は無い
									m[x] = m[x+1];
									if (m[x + 1] == ix + 1)		//	ix+1 が端点の場合
										m[x + 1] = ix + m_aryWidth;		//	右と下を連結
									else {
										int t = m[x+1];
										if( t > ix && t < ix + m_aryWidth )
											m[t % m_aryWidth] = ix + m_aryWidth;	//	逆側の端点を更新
										m[x + 1] = MID;
									}
									mp2[m] += n;				//	ハッシュに追加、重複している場合は個数
								}
							}
						}
					} else if( mate[x] != MID ) {			//	連結済み＆途中点で無い場合
						int num1 = m_number[mate[x]];
						if( y != m_height ) {
							int num2 = m_number[ix+m_aryWidth];
							if( !num1 || !num2 || num1 == num2 ) {
								//	下方向に連結
								auto m = mate;
								int t = m[x];			//	端点
								if( t > ix && t < ix + m_aryWidth )
									m[t % m_aryWidth] = ix + m_aryWidth;	//	逆側の端点を更新
								m[x] = t;
								mp2[m] += n;				//	ハッシュに追加、重複している場合は個数
							}
						}
						if( x != m_width ) {		//	右端でない場合
							int num2 = m_number[ix+1];
							if( !num2 ) num2 = m_number[mate[x+1]];
							if( !num1 || !num2 || num1 == num2 ) {
								//	右方向に連結
								auto m = mate;
								if( m[x+1] == ix + 1 ) {		//	右側が連結されていない場合
									int t = m[x+1] = m[x];			//	ix から右方向に連結
									if( t > ix && t < ix + m_aryWidth )
										m[t % m_aryWidth] = ix + 1;	//	逆側の端点を更新
									m[x] = ix + m_aryWidth;	//	下方向には連結しない
									mp2[m] += n;				//	ハッシュに追加、重複している場合は個数
								} else {		//	右側が連結されている場合
									//	undone: 同じ数字チェック
									int t1 = m[x];
									int t2 = m[x+1];
									if( t2 != ix ) {		//	ループしていない場合
										if( t1 > ix && t1 < ix + m_aryWidth )
											m[t1 % m_aryWidth] = t2;		//	端点を更新
										if( t2 > ix && t2 < ix + m_aryWidth )
											m[t2 % m_aryWidth] = t1;		//	端点を更新
										m[x+1] = MID;				//	途中点
										m[x] = ix + m_aryWidth;	//	下方向には連結しない
										mp2[m] += n;				//	ハッシュに追加、重複している場合は個数
									}
								}
							}
						}
					} else {
						//if ( ix == m_number.size() - 1 ) {		//	最後のセルの場合
							if (mate[x] == MID ) {		//連結済みの場合
								auto m = mate;
								m[x] = ix + m_aryWidth;
								mp2[m] += n;
							}
						//}
					}
				}
			}
			mp = mp2;		//	新しい集合をフロンティアに
		}
	}
	int cnt = 0;
#if	DEBUG_OUT
	for (auto itr = mp.cbegin(); itr != mp.cend(); ++itr) {
		cnt += itr->second;
		cout << vucToText(itr->first) << ":" << to_string((int)itr->second) << ", ";
	}
	cout << "\n";
#else
	for (auto itr = mp.cbegin(); itr != mp.cend(); ++itr) {
		cnt += itr->second;
	}
#endif
	return cnt;
}
