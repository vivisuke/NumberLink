#include "Board2.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

extern random_device	g_rd;
extern mt19937	g_mt;

Board2::Board2(int width, int height)
{
	setSize(width, height);
}
void Board2::setSize(int width, int height)		//	height == 0 の場合は正方形
{
	m_aryWidth = (m_width = width) + 1;
	m_aryHeight = (m_height = height) + 2;
	m_arySize = m_aryWidth * m_aryHeight;
	m_number.resize(m_arySize);
	m_work.resize(m_arySize);
	m_link1.resize(m_arySize);
	m_link2.resize(m_arySize);
	clear();
}
void Board2::setNumbers(cchar *ptr)
{
	m_ntp = 0;
	int num;
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			const auto ch = *ptr++;
			m_number[xyToIndex(x, y)] = num = isdigit(ch) ? ch - '0' : 0;
			m_ntp = max(m_ntp, num);
		}
	}
}
void Board2::clear()
{
	m_ntp = 0;
	for(auto &x : m_number) x = WALL;		//	WALL で初期化
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			m_number[xyToIndex(x, y)] = EMPTY;
		}
	}
	for(auto &x : m_link1) x = 0;		//	0 で初期化
	for(auto &x : m_link2) x = 0;		//	0 で初期化
}
bool Board2::isAdjoining() const			//	隣接する数字があるか？
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
bool Board2::isAnsCrowded() const				//	同じ数字が４つ密集している箇所があるか？
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
bool Board2::isFilled() const				//	リンクが全セルを通っている？
{
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_workAns[ix] == 0 )
				return false;
			//if( m_number[ix] != 0 ) {
			//	if( m_link1[ix] == 0 || m_link2[ix] != 0 )
			//		return false;
			//} else {
			//	if( m_link1[ix] == 0 && m_link2[ix] == 0 )
			////		return false;
			//}
		}
	}
	return true;
}
std::string Board2::qtext() const				//	問題形式テキスト
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
std::string Board2::text0() const
{
	string txt;
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			int num = m_number[xyToIndex(x, y)];
			if( !num )
				txt += ".";
			else
				txt += '0' + num;
		}
	}
	return txt;
}
std::string Board2::text() const
{
	string txt;
	for (int y = 1; y <= m_height; ++y) {
		txt += " ";
		for (int x = 1; x <= m_width; ++x) {
			int num = m_number[xyToIndex(x, y)];
			if( !num )
				txt += ".";
			else
				txt += '0' + num;
			txt += " ";
		}
		txt += "\n";
	}
	return txt;
}
std::string Board2::textnl() const				//	number, link 内容を表示
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
			if( m_link1[ix] == +1 || m_link2[ix] == +1 )
				txt += "-";
			else
				txt += " ";
		}
		txt += "\n ";
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_link1[ix] == +m_aryWidth || m_link2[ix] == +m_aryWidth )
				txt += "| ";
			else
				txt += "  ";
		}
		txt += "\n";
	}
	return txt;
}
std::string Board2::textwl() const				//	work, link 内容を表示
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
			if( m_link1Ans[ix] == +1 || m_link2Ans[ix] == +1 )
				txt += "-";
			else
				txt += " ";
		}
		txt += "\n ";
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_link1Ans[ix] == +m_aryWidth || m_link2Ans[ix] == +m_aryWidth )
				txt += "| ";
			else
				txt += "  ";
		}
		txt += "\n";
	}
	return txt;
}
std::string Board2::textWork()			//	work, linkAns 内容を表示
{
	m_workAns = m_work;
	m_link1Ans = m_link1;
	m_link2Ans = m_link2;
	return textwl();
}
void Board2::setRandom(int NTP)						//	[1, NTP]*2 の数字をランダムに配置
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
void Board2::setRandomNoAdj(int NTP)				//	隣接しない [1, NTP]*2 の数字をランダムに配置
{
	for (;;) {
		setRandom(NTP);
		if( !isAdjoining() )
			return;
	}
}
void Board2::genRandomLink(int NTP)					//	[1, NTP] の数字をランダムに配置し、そこからリンクをランダムに伸ばす
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
		if( m_work[ix-m_aryWidth] == 0 ) vl.push_back(-m_aryWidth);
		if( m_work[ix-1] == 0 ) vl.push_back(-1);
		if( m_work[ix+1] == 0 ) vl.push_back(+1);
		if( m_work[ix+m_aryWidth] == 0 ) vl.push_back(+m_aryWidth);
		if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
			tp.erase(tp.begin() + r);
		} else {
			int link = vl[g_mt() % vl.size()];
			if( !m_link1[ix] )
				m_link1[ix] = link;
			else
				m_link2[ix] = link;
			tp[r] = ix += link;
			m_work[ix] = num;
			m_link1[ix] = -link;
		}
		//cout << textwl() << "\n";
	}
}
void Board2::genRandomLink2(int NTP)				//	[1, NTP] の数字2個をランダムに配置
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
		m_link1[ix] = link;
		m_number[ix+=link] = n;
		m_link1[ix] = -link;
		tp.push_back(ix);
	}
	m_work = m_number;
	while( !tp.empty() ) {		//	端点が存在する間ループ
		int r = g_mt() % tp.size();
		int ix = tp[r];					//	ix = 端点位置
		int num = m_work[ix];
		vl.clear();
		if( m_work[ix-m_aryWidth] == 0 ) {
			int t = m_work[ix+m_link1[ix]-m_aryWidth];
			if( m_link1[ix] == 0 ||		//	リンクが無い場合
				m_link1[ix] == +m_aryWidth ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]-m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(-m_aryWidth);
			}
		}
		if( m_work[ix-1] == 0 ) {
			int t = m_work[ix+m_link1[ix]-1];
			if( m_link1[ix] == 0 ||
				m_link1[ix] == +1 ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]-1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(-1);
			}
		}
		if( m_work[ix+1] == 0 ) {
			int t = m_work[ix+m_link1[ix]+1];
			if( m_link1[ix] == 0 ||
				m_link1[ix] == -1 ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]+1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+1);
			}
		}
		if( m_work[ix+m_aryWidth] == 0 ) {
			int t = m_work[ix+m_link1[ix]+m_aryWidth];
			if( m_link1[ix] == 0 ||
				m_link1[ix] == -m_aryWidth ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]+m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+m_aryWidth);
			}
		}
		if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
			tp.erase(tp.begin() + r);
		} else {
			int link = vl[g_mt() % vl.size()];
			if( !m_link1[ix] )
				m_link1[ix] = link;
			else
				m_link2[ix] = link;
			tp[r] = ix += link;
			m_work[ix] = num;
			m_link1[ix] = -link;
		}
		//cout << textwl() << "\n";
	}
}
void Board2::genRandomLink3(int NTP)				//	[1, NTP] の数字2個をランダムに配置、コーナーなどの確定するリンクも配置
{
	clear();
	m_ntp = NTP;
	vector<int> tp;		//	端点リスト
	int ix;
	vector<int> vl;				//	移動可能方向を格納配列
	//	隣接する数字をNTP個ランダムに配置
	for (int n = 1; n <= NTP; ++n) {
		while( m_number[ix = g_mt() % m_arySize] != EMPTY ) {}
		if (n == 1) ix = xyToIndex(2, 1);	//	for Test
		m_number[ix] = n;
		tp.push_back(ix);
		int x = indexToX(ix);
		int y = indexToY(ix);
		int link;
		//	ix が空コーナーの隣の場合は、コーナーと連結
		if( y == 1 && x == 2 && m_number[xyToIndex(1,1)] == 0 )
			link = -1;
		else if( y == 1 && x == m_width - 1 && m_number[xyToIndex(m_width,1)] == 0 )
			link = 1;
		else if( y == 2 && x == 1 && m_number[xyToIndex(1,1)] == 0 ||
			y == 2 && x == m_width - 1 && m_number[xyToIndex(m_width,1)] == 0 )
		{
			link = -m_aryWidth;
		} else if( y == m_height - 1 && x == 1 && m_number[xyToIndex(1, m_height)] == 0 ||
			y == m_height - 1 && x == m_width - 1 && m_number[xyToIndex(m_width, m_height)] == 0 )
		{
			link = -m_aryWidth;
		} else if( y == m_height && x == 2 && m_number[xyToIndex(1, m_height)] == 0 )
			link = -1;
		else if( y == m_height && x == m_width - 1 && m_number[xyToIndex(m_width, m_height)] == 0 )
			link = 1;
		else {
			vl.clear();
			if( m_number[ix-m_aryWidth] == 0 ) vl.push_back(-m_aryWidth);
			if( m_number[ix-1] == 0 ) vl.push_back(-1);
			if( m_number[ix+1] == 0 ) vl.push_back(+1);
			if( m_number[ix+m_aryWidth] == 0 ) vl.push_back(+m_aryWidth);
			if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
				return;				//	隣接して配置不能
			}
			link = vl[g_mt() % vl.size()];
		}
		m_link1[ix] = link;
		m_number[ix+=link] = n;
		m_link1[ix] = -link;
		tp.push_back(ix);
	}
	//cout << textWork();
	//cout << "\n";
	m_work = m_number;
	setCornerLink(xyToIndex(1,1), +1, +m_aryWidth);
	setCornerLink(xyToIndex(m_width,1), -1, +m_aryWidth);
	setCornerLink(xyToIndex(1,m_height), +1, -m_aryWidth);
	setCornerLink(xyToIndex(m_width,m_height), -1, -m_aryWidth);
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
			int t = m_work[ix+m_link1[ix]-m_aryWidth];
			if( m_link1[ix] == 0 ||		//	リンクが無い場合
				m_link1[ix] == +m_aryWidth ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]-m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(-m_aryWidth);
			}
		}
		if( m_work[ix-1] == 0 ) {
			int t = m_work[ix+m_link1[ix]-1];
			if( m_link1[ix] == 0 ||
				m_link1[ix] == +1 ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]-1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(-1);
			}
		}
		if( m_work[ix+1] == 0 ) {
			int t = m_work[ix+m_link1[ix]+1];
			if( m_link1[ix] == 0 ||
				m_link1[ix] == -1 ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]+1] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+1);
			}
		}
		if( m_work[ix+m_aryWidth] == 0 ) {
			int t = m_work[ix+m_link1[ix]+m_aryWidth];
			if( m_link1[ix] == 0 ||
				m_link1[ix] == -m_aryWidth ||		//	直進の場合
				/*m_work[ix+m_link1[ix]] != num ||*/ m_work[ix+m_link1[ix]+m_aryWidth] != num )
			{	//	コの字型になっている場合は無視
				vl.push_back(+m_aryWidth);
			}
		}
		if( vl.empty() ) {		//	移動可能な隣接セルが無い場合
			tp.erase(tp.begin() + r);
		} else {
			int link = vl[g_mt() % vl.size()];
			if( !m_link1[ix] )
				m_link1[ix] = link;
			else
				m_link2[ix] = link;
			tp[r] = ix += link;
			m_work[ix] = num;
			if( m_link1[ix] == 0 )
				m_link1[ix] = -link;
			else {		//	既にリンクがある場合
				//	[ix0]-> [ix] <-->[ix2]
				m_link2[ix] = -link;		//	ix0, ix を相互にリンク
				link = m_link1[ix];
				for (;;) {
					ix += link;			//	次のセルに移動
					m_work[ix] = num;
					if( m_link2[ix] == 0 )
						break;		//	その次のセルへのリンクが無い場合
					if (m_link1[ix] == -link)
						link = m_link2[ix];
					else
						link = m_link1[ix];
				}
				tp[r] = ix;
			}
		}
		//cout << textwl() << "\n";
	}
	m_workAns = m_work;
	m_link1Ans = m_link1;
	m_link2Ans = m_link2;
	for (int y = 1; y <= m_height; ++y) {
		for (int x = 1; x <= m_width; ++x) {
			int ix = xyToIndex(x, y);
			if( m_link1Ans[ix] != 0 && m_link2Ans[ix] != 0 )
				m_number[ix] = 0;
			else
				m_number[ix] = m_workAns[ix];
		}
	}
}
void Board2::doLink(int ix1, int ix2)
{
	int d = ix2 - ix1;
	if( m_link1[ix1] == 0 )
		m_link1[ix1] = d;
	else
		m_link2[ix1] = d;
	if( m_link1[ix2] == 0 )
		m_link1[ix2] = -d;
	else
		m_link2[ix2] = -d;
}
void Board2::setCornerLink(int ix, int dx, int dy)
{
	if( m_number[ix] != 0 ) return;
	if( m_number[ix+dx] == 0 )
		doLink(ix, ix+dx);
	if( m_number[ix+dy] == 0 )
		doLink(ix, ix+dy);
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
	m_link1[ix] = dx;
	if( m_link1[ix+dx] == 0 )
		m_link1[ix+dx] = -dx;
	else
		m_link2[ix+dx] = -dx;
	if( m_number[ix+dx] != 0 )		//	数字がある場合
		m_work[ix] = m_number[ix+dx];
	m_link2[ix] = dy;
	if( m_link1[ix+dy] == 0 )
		m_link1[ix+dy] = -dy;
	else
		m_link2[ix+dy] = -dy;
	if( m_number[ix+dy] != 0 )		//	数字がある場合
		m_work[ix] = m_number[ix+dy];
#endif
#if	0
	if( m_number[ix+dx] == 0 ) {
		m_link1[ix] = dx;
		m_link1[ix+dx] = -dx;
	}
	if( m_number[ix+dy] == 0 ) {
		m_link2[ix] = dy;
		m_link2[ix+dy] = -dy;
	}
#endif
}
bool Board2::canSolve()				//	解があるかどうかを探索
{
	for(auto &x : m_link1) x = 0;
	for(auto &x : m_link2) x = 0;
	return canSolve(xyToIndex(1, 1));
}
bool Board2::canSolve(int ix)		//	解があるかどうかを探索
{
	if( ix > xyToIndex(m_width, m_height) ) {
		//cout << textwl() << "\n";
		if( checkLink() ) {
			m_workAns = m_work;
			m_link1Ans = m_link1;
			m_link2Ans = m_link2;
			return true;
		} else
			return false;
	}
	if( m_number[ix] == WALL ) ++ix;		//	番人をスキップ
	const bool rtEdge = m_number[ix+1] == WALL;
	const bool btmEdge = m_number[ix+m_aryWidth] == WALL;
	const bool up = m_link1[ix-m_aryWidth] == m_aryWidth || m_link2[ix-m_aryWidth] == m_aryWidth;	//	上からリンクが来ている
	const bool left = m_link1[ix-1] == 1 || m_link2[ix-1] == 1;		//	左からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return false;		//	接続不可
		if( up ) {
			m_link1[ix] = -m_aryWidth;
			m_link2[ix] = 0;
			return canSolve(ix+1);
		}
		if( left ) {
			m_link1[ix] = -1;
			m_link2[ix] = 0;
			return canSolve(ix+1);
		}
		if( !rtEdge ) {
			m_link1[ix] = +1;
			m_link2[ix] = 0;
			if( canSolve(ix+1) ) return true;
		}
		if( !btmEdge ) {
			m_link1[ix] = m_aryWidth;
			m_link2[ix] = 0;
			return canSolve(ix+1);
		}
		return false;
	} else {
		if( up && left ) {
			m_link1[ix] = -m_aryWidth;
			m_link2[ix] = -1;
			return canSolve(ix+1);
		}
		if( up ) {
			if( !rtEdge ) {
				m_link1[ix] = -m_aryWidth;
				m_link2[ix] = +1;
				if( canSolve(ix+1) ) return true;
			}
			if( !btmEdge ) {
				m_link1[ix] = -m_aryWidth;
				m_link2[ix] = +m_aryWidth;
				return canSolve(ix+1);
			}
			return false;
		}
		if( left ) {
			if( !rtEdge ) {
				m_link1[ix] = -1;
				m_link2[ix] = +1;
				if( canSolve(ix+1) ) return true;
			}
			if( !btmEdge ) {
				m_link1[ix] = -1;
				m_link2[ix] = +m_aryWidth;
				return canSolve(ix+1);
			}
			return false;
		}
		if( rtEdge || btmEdge ) return false;
		m_link1[ix] = +1;
		m_link2[ix] = +m_aryWidth;
		return canSolve(ix+1);
	}
}
bool Board2::checkLink()			//	数字が正しくコネクトされ、全部の空欄を埋めているかをチェック
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
		auto link = m_link1[ix];
		if( !link ) return false;		//	リンクが無い場合
		int i = ix + link;				//	リンク先に移動
		for (;;) {
			m_work[i] = n;
			if( m_number[i] == n )		//	正しい数字に到達した場合
				break;
			if( m_number[i] != 0 )		//	異なる数字に到達した場合
				return false;
			if( m_link1[i] != -link )
				link = m_link1[i];
			else
				link = m_link2[i];
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
void Board2::genUniqRandom(int NTP)
{
	for (;;) {
		setRandom(NTP);
		m_work = m_number;
		if( checkUniq() ) {
			//m_link1 = m_link1Sv;
			//m_link2 = m_link2Sv;
			return;
		}
	}
}
bool Board2::checkUniq()
{
	m_nSolved = 0;
	for(auto &x : m_link1) x = 0;
	for(auto &x : m_link2) x = 0;
	checkUniq(xyToIndex(1, 1));
	return m_nSolved == 1;
}
void Board2::checkUniq(int ix)
{
	//cout << textnl() << "\n";
	if( ix > xyToIndex(m_width, m_height) ) {
		if( checkLink() ) {
			m_workAns = m_work;
			m_link1Ans = m_link1;
			m_link2Ans = m_link2;
			//cout << textnl() << "\n";
			++m_nSolved;
		}
		return;
	}
	if( m_number[ix] == WALL ) ++ix;		//	番人をスキップ
	bool rtEdge = m_number[ix+1] == WALL;
	bool btmEdge = m_number[ix+m_aryWidth] == WALL;
	bool up = m_link1[ix-m_aryWidth] == m_aryWidth || m_link2[ix-m_aryWidth] == m_aryWidth;	//	上からリンクが来ている
	bool left = m_link1[ix-1] == 1 || m_link2[ix-1] == 1;		//	左からリンクが来ている
	if( m_number[ix] != 0 ) {	//	セルに数字がある場合
		if( up && left ) return;		//	接続不可
		if( up ) {
			m_link1[ix] = -m_aryWidth;
			m_link2[ix] = 0;
			checkUniq(ix+1);
			return;
		}
		if( left ) {
			m_link1[ix] = -1;
			m_link2[ix] = 0;
			checkUniq(ix+1);
			return;
		}
		if( !rtEdge ) {
			m_link1[ix] = +1;
			m_link2[ix] = 0;
			checkUniq(ix+1);
			if( m_nSolved >= 2 ) return;
		}
		if( !btmEdge ) {
			m_link1[ix] = m_aryWidth;
			m_link2[ix] = 0;
			checkUniq(ix+1);
		}
	} else {
		if( up && left ) {
			m_link1[ix] = -m_aryWidth;
			m_link2[ix] = -1;
			checkUniq(ix+1);
			return;
		}
		if( up ) {
			if( !rtEdge ) {
				m_link1[ix] = -m_aryWidth;
				m_link2[ix] = +1;
				checkUniq(ix+1);
				if( m_nSolved >= 2 ) return;
			}
			if( !btmEdge ) {
				m_link1[ix] = -m_aryWidth;
				m_link2[ix] = +m_aryWidth;
				checkUniq(ix+1);
			}
			return;
		}
		if( left ) {
			if( !rtEdge ) {
				m_link1[ix] = -1;
				m_link2[ix] = +1;
				checkUniq(ix+1);
				if( m_nSolved >= 2 ) return;
			}
			if( !btmEdge ) {
				m_link1[ix] = -1;
				m_link2[ix] = +m_aryWidth;
				checkUniq(ix+1);
			}
			return;
		}
		if( rtEdge || btmEdge ) return;
		m_link1[ix] = +1;
		m_link2[ix] = +m_aryWidth;
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
bool Board2::checkUniqMate()		//	Mate アルゴリズムによるユニークチェック
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
