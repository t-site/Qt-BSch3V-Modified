﻿/*******************************************************************************
 *   This file is part of Qt-BSch3V Modified.
 *   Copyright (C) 2002-2005 H.Okada
 *   Copyright (C) 2010-2011 Stork Lab
 *
 *   Qt-BSch3V Modified is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Qt-BSch3V Modified is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************/

#include "stdafx.h"
#include "sptnobjpolygon.h"
#include <stdlib.h>
#include <limits.h>

SPtnObjPolygon::SPtnObjPolygon(void)
{
    m_nCount = 0;    	    //頂点の数
    m_pPt = NULL;    	    //頂点座標
    //m_nFill = -1;
}

SPtnObjPolygon::~SPtnObjPolygon(void)
{
    delete[]m_pPt;
}

//正規化 隣接した頂点が同じ座標のときに頂点を削除する
void SPtnObjPolygon::normalize()
{
    int i,j,n,m;
    m = m_nCount-1;
    n = 0;
    //隣接した頂点のスキャン
    for(i = 0; i < m; i++) {
        if(m_pPt[i] == m_pPt[i+1]) n++;
    }
    if(m_pPt[m] == m_pPt[0]) n++;
    if(n == 0) return;

    //新しいポリゴンの頂点の確保
    int newCount = m_nCount - n;
    if(newCount <= 1) {
        m_nCount = 0;
        delete[]m_pPt;
        m_pPt = NULL;
        return;
    }
    SPoint* pPt = new SPoint[newCount];

    //頂点のコピー
    j = 0;
    for(i = 0; i < m; i++) {
        if(m_pPt[i] != m_pPt[i+1]) {
            pPt[j++] = m_pPt[i];
        }
    }
    if(m_pPt[m] != m_pPt[0]) pPt[j] = m_pPt[m];

    m_nCount = newCount;
    delete[]m_pPt;
    m_pPt = pPt;
}

//占有範囲を返す
SRect SPtnObjPolygon::area()
{

    int left = INT_MAX;
    int top    = INT_MAX;
    int right = INT_MAX;
    int bottom = INT_MAX;
    if(m_nCount > 0) {
        left = right = m_pPt[0].x();
        top = bottom = m_pPt[0].y();
        int i;
        for(i = 1; i < m_nCount; i++) {
            int x = m_pPt[i].x();
            int y = m_pPt[i].y();
            if(left > x)    	left = x;
            if(right < x)    	right =x;
            if(top > y)    	top    = y;
            if(bottom < y)    bottom = y;
        }
    }
    return SRect(left,top,(right-left),(bottom-top));
}


//コピーコンストラクタ
SPtnObjPolygon::SPtnObjPolygon(const SPtnObjPolygon& obj)
{
    m_pPt = NULL;
    *this = obj;
}

//代入演算子
SPtnObjPolygon& SPtnObjPolygon::operator=(const SPtnObjPolygon& obj)
{
    if(this != &obj) {
        (SPtnObj&)*this = obj;    //基本クラスのメンバーのコピー
        m_nFill = obj.m_nFill;
        delete[]m_pPt;
        m_nCount = obj.m_nCount;
        if(m_nCount > 0) {
            m_pPt = new SPoint[m_nCount];
            int i;
            for(i = 0; i < m_nCount; i++) {
                m_pPt[i] = obj.m_pPt[i];
            }
        } else {
            m_pPt = NULL;
        }
    }
    return *this;
}

//複製を作る
SPtnObj* SPtnObjPolygon::duplicate()
{
    return new SPtnObjPolygon(*this);
}

//グリップの位置を返す
SPoint SPtnObjPolygon::gripPos(int grip)
{
    if(m_nCount == 0 || m_pPt == NULL) return SPoint(0,0);
    if(grip < 0) grip = 0;
    else if(grip >= m_nCount) grip = m_nCount;
    return m_pPt[grip];
}

//グリップの位置を動かす
void SPtnObjPolygon::setGripPos(int grip,int x,int y)
{
    if(m_nCount == 0 || m_pPt == NULL) return;
    if(grip < 0) grip = 0;
    else if(grip >= m_nCount) grip = m_nCount;
    m_pPt[grip] = SPoint(x,y);
}

//有効なデータかどうかを返す。
bool SPtnObjPolygon::qValid()
{
    normalize();
    if(m_nCount > 0) return true;
    else return false;
}

//位置を動かす
void SPtnObjPolygon::move(int offsetx,int offsety)
{
    SPoint ptOffset(offsetx,offsety);
    int i;
    for(i = 0; i < m_nCount; i++) {
        m_pPt[i] += ptOffset;
    }
}

//X座標を指定して左右反転を行う
void SPtnObjPolygon::mirrorH(int x)
{
    int i;
    for(i = 0; i < m_nCount; i++) {
        m_pPt[i] = mirrorPoint(m_pPt[i],x);
    }
}

//左右反転を行う
void SPtnObjPolygon::mirrorH()
{
    SRect rc = area();
    int x = rc.center().x();
    mirrorH(x);
}

//XY中心座標を指定して、回転を行う
void SPtnObjPolygon::rotate(const SPoint& p)
{
    int i;
    for(i = 0; i < m_nCount; i++) {
        m_pPt[i] = rotatePoint(m_pPt[i],p);
    }
}

//回転を行う
void SPtnObjPolygon::rotate()
{
    SRect rc = area();
    SPoint p = rc.center();
    rotate(p);
}

//範囲を指定して選択チェックを行う
bool SPtnObjPolygon::testIntersect(const SRect& rc)
{
    SRect rc1 = area();
    if(!rc.intersect( SPoint(rc1.l(),rc1.t()) ) ) return false;
    if(!rc.intersect( SPoint(rc1.r(),rc1.b()) ) ) return false;
    return true;
}

//点を指定して選択チェックを行う
//2点のときは線分と面積を持った点との交差で判定
//3点以上のときは、(p.x(),INT_MIN)(p.x(),p.y()) の線が多角形の線分と奇数回交差するかどうかで判定
bool SPtnObjPolygon::testIntersect(const SPoint& p)
{
    int x,y,x0,y0,x1,y1;
    normalize();
    if(m_nCount < 2) return false;
    if(m_nCount == 2) {
        return testIntersectLinePoint(m_pPt[0],m_pPt[1],p);
    }

    SRect rc = area();
    if(!rc.intersect(p)) return false;

    int i,j;
    unsigned acrossFlag = 0;
    x = p.x();
    y = p.y();
    for(i = 0; i < m_nCount; i++) {
        j = i+1;
        if(j == m_nCount)j = 0;
        if(testIntersectLinePoint(m_pPt[i],m_pPt[j],p))return true;    //点が線分上にあればtrue
        if(m_nFill < 0)continue;
        x0 = m_pPt[i].x();
        y0 = m_pPt[i].y();
        x1 = m_pPt[j].x();
        y1 = m_pPt[j].y();
        if(x0 == x1) continue;    	    	    	    //垂直の線は判定に影響しない
        if((x0 < x && x1 < x)||(x0 > x && x1 > x)) continue;    //xを通る垂直線を、またがないものは判定に影響しない。
        if(y0 > y && y1 > y) continue;    	    	    	//両端点がp点よりも下にあるものは判定に影響しない。
        if(y0 < y && y1 < y) {    	    	    	    	//両端点がp点よりも上にあるものは判定垂線と交差する
            acrossFlag ^=3;
            continue;
        }
        if(x == x0 && y > y0) {    	    //p0点が判定垂線上にある場合
            if(x1 > x) {
                acrossFlag ^=1;
            } else {
                acrossFlag ^=2;
            }
            continue;
        }
        if(x == x1 && y > y1) {    	    //p1点が判定垂線上にある場合
            if(x0 > x) {
                acrossFlag ^=1;
            } else {
                acrossFlag ^=2;
            }
            continue;
        }
        if(((y1-y0)*(x-x0))/(x1-x0)+y0 < y) acrossFlag ^=3;
    }
    if(acrossFlag == 3) return true;
    else return false;
}


bool SPtnObjPolygon::readCe3(SReadCE3& rce3)
{
    SPoint ptbuff[VECTPLOYGON_MAXNODE];

    delete[]m_pPt;
    m_pPt = NULL;
    m_nCount = 0;

    int width = 1;
    int style = 0;
    int fill = -1;
    int x,y,prevX,prevY;
    int sety = 0;

    int node,realnode;

    realnode = 0;

    node = 0;
    x = 0;

    prevX = INT_MIN;    //適当な初期値
    prevY = INT_MIN;    //適当な初期値
    string str("");

    while(rce3.ReadRecord(str) != EOF) {
        if(str[0] == '-') {
            if(str != "-PG") {
                return false;
            } else {
                break;
            }
        } else if(str[0] == '+') {
            str[0]='-';
            if(rce3.SkipTo(str) == EOF) return false;
        } else {
            int n;
            int l = str.length();
            for(n = 0; n < l; n++) {
                if(str[n] == ':') break;
            }
            if(0 < n && n<(l-1)) {    // : で分けられたレコードである
                string var = str.substr(0,n);    	    	//先頭から:の手前まで
                if(var == "W") {
                    width = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
                } else if(var == "S") {
                    style = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
                } else if(var == "F") {
                    fill = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
                } else if(var == "N") {
                    node = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
                    if(node < 2 || node > VECTPLOYGON_MAXNODE) goto PARAMERR;
                }
                if(var == "X") {
                    if(node == 0) goto PARAMERR;
                    if(sety < node) {
                        x = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
                    }
                } else if(var == "Y") {
                    if(node == 0) goto PARAMERR;
                    if(sety < node) {
                        y = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
                        if(sety == 0|| x != prevX || y != prevY) {
                            ptbuff[realnode].setX(x);
                            ptbuff[realnode].setY(y);
                            prevX = x;
                            prevY = y;
                            realnode++;
                            if(sety == (node-1)) {
                                if(x == ptbuff[0].x() && y == ptbuff[0].y()) realnode--;
                            }
                        }
                        sety++;
                    }
                }
            }
        }
    }


    if(realnode < 2) {
        return false;
    } else {
        m_width = width;
        m_style = style;
        m_nFill = fill;
        m_nCount = realnode;
        m_pPt = new SPoint[realnode];
        for(int i = 0; i < realnode; i++) {
            m_pPt[i] = ptbuff[i];
        }
        return true;
    }
PARAMERR:
    rce3.SkipTo("-PG");
    return false;

}

bool SPtnObjPolygon::writeCe3(SWriteCE3& wce3,SPoint* pOrigin)
{
    int i;
    if(m_nCount < 2 || m_nCount > VECTPLOYGON_MAXNODE)return false;
    wce3.WriteRecord("+PG");
    wce3.WriteRecordInt("W",m_width);
    wce3.WriteRecordInt("S",m_style);
    wce3.WriteRecordInt("F",m_nFill);
    wce3.WriteRecordInt("N",m_nCount);
    for(i = 0; i < m_nCount; i++) {
        if(pOrigin != NULL) {
            wce3.WriteRecordInt("X",m_pPt[i].x() - pOrigin->x());
            wce3.WriteRecordInt("Y",m_pPt[i].y() - pOrigin->y());
        } else {
            wce3.WriteRecordInt("X",m_pPt[i].x());
            wce3.WriteRecordInt("Y",m_pPt[i].y());
        }
    }
    wce3.WriteRecord("-PG");
    wce3.WriteEOL();
    return true;
}






