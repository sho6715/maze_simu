
#include <string.h>
#include "typedefine.h"
#include "search.h"


#define DIR 4 //東西南北
#define DIAFLAG 3 //正面　右斜め　左斜
//runtimeは動作ごとの移動時間
//現状想定パラメータ
//直進MAX0.7m/s
//加速度　2.5m/s
//超信地角加速度　22.2*PI
//スラローム角加速度　10*PI
//runtime実際は変数計算化しておかないと行けないのかな
const int runtime[165]={0,
10,18,25,32,40,45,49,54,58,63,//GO1,GO2,...を行うためにかかる時間.
67,72,76,81,85,90,94,99,103,108,
112,117,121,126,130,135,139,144,148,153,
10,18,25,32,40,45,49,54,58,63,
67,72,76,81,85,90,94,99,103,108,
112,117,121,126,130,135,139,144,148,153,
111,111,
 8,//TURNRを行うためにかかる時間
 8,//TURNLを行うためにかかる時間
 8,//DIA_TO_CLOTHOIDRを行うためにかかる時間
 8,//DIA_TO_CLOTHOIDLを行うためにかかる時間
 8,//DIA_FROM_CLOTHOIDRを行うためにかかる時間
 8,//DIA_FROM_CLOTHOIDLを行うためにかかる時間
 6,//DIA_TURNRを行うためにかかる時間
 6,//DIA_TURNLを行うためにかかる時間
 7,13,19,24,29,33,40,43,46,49,//DIA_GO1-GO10
53,56,59,62,65,68,72,75,78,81,//DIA_GO11-GO20
84,88,91,94,97,100,103,107,110,113,//DIA_GO21-GO30
116,119,123,126,129,132,135,138,142,145,//DIA_GO31-GO40
148,151,154,158,161,164,167,170,173,177,//DIA_GO41-GO50
180,183,186,189,193,196,199,202,205,208,//DIA_GO51-GO60
212,215,218,                     //DIA_GO61-GO63を行うためにかかる時間
 0//ストップを意味する．何でもいい．
};



/*
//int dijkstra_counter[2][32][32];  //北、西、32*32

int16_t dijkstra_counter[32][32];


void dijkstra_init(void){
    memset( dijkstra_counter, 0, sizeof(dijkstra_counter) );

}
*/
//d      は0-03の2bit:北向き，東向き，南向き，西向きに対応
//diaflagは0-2の2bit :斜めに向いていない，右斜め向き，左斜め向きに対応
int encoder(int x, int y, int dir,int diaflag){
    return (diaflag<<12)+(dir<<10)+(y<<5)+x;
}

void decoder(int hash, int *x, int *y, int *dir, int *diaflag){//ハッシュ番号から座標をかえす
	*x =  hash      & (32-1);
	*y = (hash>> 5) & (32-1);
	*dir = (hash>>10) & (4-1);
	*diaflag = (hash>>12);
}

void addlist(int node, int lis[1<<7], int *lisnum){
	int i;
	for(i = 0;i < *lisnum;i++){
		if(lis[i] == node)break;
	}
	if(i == *lisnum){
		lis[*lisnum]=node;
		*lisnum = *lisnum + 1;
	}
}

void addnode(int node,
             int lis[1<<7],
             int *lisnum, 
             int prev[MAP_Y_SIZE][MAP_X_SIZE][4], 
             int cost[MAP_Y_SIZE][MAP_X_SIZE][4])
{
    int nowx, nowy, nowd, nowdia;
	int nextnode;
	int dx,dy;
	int i;
	int diastraight;
	decoder(node, &nowx, &nowy, &nowd, &nowdia);//ノードはパラメータの設定まとめ
	//直進側を展開
	switch(nowd){//メモ：from側の相対座標
		case 0:dx= 0;dy=+1;break;
		case 1:dx=-1;dy= 0;break;
		case 2:dx= 0;dy=-1;break;
		case 3:dx=+1;dy= 0;break;
	}

    if(nowdia==0){ //斜めかどうか　0はストレート
		//ターンの時間をノードに追加する
		if( 0<= nowx+dx && nowx+dx<MAP_X_SIZE &&
		    0<= nowy+dy && nowy+dy<MAP_Y_SIZE){//範囲確認
			if(!(g_sysMap[nowy+dy][nowx+dx]&(1 << nowd))){ //壁認識
				nextnode = encoder(nowx+dx,nowy+dy, (nowd + 3) & 0x03, 1);//次ノードをひとまとめに設定
				if(cost[node]+runtime[R90S] < cost[nextnode] || prev[nextnode] == 0){//次のノードの計算と
					prev[nextnode] = R90S;
					cost[nextnode] = cost[node]+runtime[R90S];
					addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
				}
			}
			if(!(g_sysMap[nowy+dy][nowx+dx]&(1 << nowd))){
				nextnode = encoder(nowx+dx,nowy+dy, (nowd + 1) & 0x03, 2);
				if(cost[node]+runtime[L90S] < cost[nextnode] || prev[nextnode] == 0){
					prev[nextnode] = L90S;
					cost[nextnode] = cost[node]+runtime[L90S];
					addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
				}
			}
		}
		//ストレートの時間を追加
		for(i = GO1;i <= GO31;i++){
			if( 0<= nowx+dx*i && nowx+dx*i<MAP_X_SIZE &&
				0<= nowy+dy*i && nowy+dy*i<MAP_Y_SIZE){
				if(!(g_sysMap[nowy+dy*i][nowx+dx*i]&(1 << nowd))){
					nextnode = encoder(nowx+dx*i, nowy+dy*i, nowd, 0);
					if(cost[node]+runtime[i] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = i;
						cost[nextnode] = cost[node]+runtime[i];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}else{
					break;
				}
			}else{
				break;
			}
		}
	}else{
        if( 0<= nowx+dx && nowx+dx<MAP_X_SIZE &&
		    0<= nowy+dy && nowy+dy<MAP_Y_SIZE){
			if(nowdia == 1){
				if(!(g_sysMap[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TO_CLOTHOIDR
					nextnode = encoder(nowx+dx,nowy+dy, nowd, 0);
					if(cost[node]+runtime[R90S] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = R90S;
						cost[nextnode] = cost[node]+runtime[R90S];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
				if(!(g_sysMap[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TURNR
					nextnode = encoder(nowx+dx,nowy+dy, (nowd + 3) & 0x03, 1);
					if(cost[node]+runtime[R90] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = R90;
						cost[nextnode] = cost[node]+runtime[R90];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
			}
			if(nowdia == 2){
				if(!(g_sysMap[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TO_CLOTHOIDL
					nextnode = encoder(nowx+dx,nowy+dy, nowd, 0);
					if(cost[node]+runtime[L90S] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = L90S;
						cost[nextnode] = cost[node]+runtime[L90S];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
				if(!(g_sysMap[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TURNL
					nextnode = encoder(nowx+dx,nowy+dy, (nowd + 1) & 0x03, 2);
					if(cost[node]+runtime[L90] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = L90;
						cost[nextnode] = cost[node]+runtime[L90];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
			}
		}
		for(diastraight=GO1;diastraight<=GO71;diastraight++){//63
			switch(nowd){//メモ：from側の相対座標
				case 0:dx= 0;dy=+1;break;
				case 1:dx=-1;dy= 0;break;
				case 2:dx= 0;dy=-1;break;
				case 3:dx=+1;dy= 0;break;
			}
			if( 0<= nowx+dx && nowx+dx<MAP_X_SIZE &&
			    0<= nowy+dy && nowy+dy<MAP_Y_SIZE){
				if(!(g_sysMap[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TURNR
					nowx+=dx;
					nowy+=dy;
					nowd=(nowd+nowdia*2-1) & 0x03;//右傾きなら右方向へ，左傾きなら左方向へnowdを回転
					nowdia=3-nowdia;//右傾きと左傾きを切り替える
					nextnode = encoder(nowx,nowy, nowd, nowdia);
					if(cost[node]+runtime[diastraight] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = diastraight;
						cost[nextnode] = cost[node]+runtime[diastraight];
						//nextnodeを未探索ノードとして追加する．
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつリストに追加
					}
				}else{
					break;
				}
			}else{
				break;
			}
		}
    }

}

int8_t dijkstra(int startx, 
                int starty, 
                int startd, 
                int goalx, 
                int goaly, 
                int route[200])
{
    int prev[MAP_Y_SIZE][MAP_X_SIZE][4]={0};//0なら未確定ノード，1以上なら対応したコマンドが過去データ
	int cost[MAP_Y_SIZE][MAP_X_SIZE][4]={0};//コスト
	int lis[1<<7];//探索ノードをリスト構造で保持リストは未探索ノードの個数分がはいる十分な大きさを確保すること
    //リストには座標と方向（追加可能性あり）を入れておく必要がある。ついでにコストも入れないと最小がみえないか
	int lisnum=0;
	int i,t;
	int minnum;
	int minindex;
	int nowx , nowy , nowd, nowdia;
	int dx,dy;
	int nownode, nextnode;
    //ルートはいったん省略

    if(goalx == startx && goaly == starty){
		route[0]=STOP;
		return 1;
	}

//    starttemp = g_sysMap[starty][startx];
//    g_sysMap[starty][startx] &= ;

    prev[goaly][goalx][0]=STOP;
    prev[goaly][goalx][1]=STOP;
    prev[goaly][goalx][2]=STOP;
    prev[goaly][goalx][3]=STOP;

    //初期化：リストにゴールノードの隣接ノードをつっこむ
	if(!(g_sysMap[goaly][goalx]&0x04)){addnode(encoder(goalx,goaly,0,0),lis,&lisnum,prev,cost);}
	if(!(g_sysMap[goaly][goalx]&0x08)){addnode(encoder(goalx,goaly,1,0),lis,&lisnum,prev,cost);}
	if(!(g_sysMap[goaly][goalx]&0x01)){addnode(encoder(goalx,goaly,2,0),lis,&lisnum,prev,cost);}
	if(!(g_sysMap[goaly][goalx]&0x02)){addnode(encoder(goalx,goaly,3,0),lis,&lisnum,prev,cost);}


    while(lisnum){
        int tmp_x;
        int tmp_y;
        int tmp_dir;
        int tmp_diaflag;

        decoder(lis[0],&tmp_x,&tmp_y,&tmp_dir,&tmp_diaflag);
        minnum = cost[tmp_y][tmp_x][tmp_dir];
        minindex = 0;

        for(i = 1;i<lisnum;i++){
            decoder(lis[i],&tmp_x,&tmp_y,&tmp_dir,&tmp_diaflag);
            if(minnum > cost[tmp_y][tmp_x][tmp_dir]){
                minnum = cost[tmp_y][tmp_x][tmp_dir];
                minindex = i;
            }
        }
        printf("lis[minindex] %d/r/n",lis[minindex]);
        addnode(lis[minindex],lis,&lisnum,prev,cost);

        if(lis[minindex]==encoder(startx,starty,startd,0)){
			lisnum=-1;//ここから抜けたことを番兵値で覚えておく
			break;
		}

        for(i = minindex;i < lisnum - 1;i++)
			lis[i]=lis[i+1];
		lisnum--;
    }

//テスト用のメモ
    for(int i = 0;i<MAP_Y_SIZE;i++){
        for(int j = 0;i<MAP_X_SIZE;j++){
            printf("%d %d ",cost[i][j][0],cost[i][j][1]);
        }
        printf("/n/r");
    }


}