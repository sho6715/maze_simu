#define FIELDX 32//2の乗数である必要があるMAP_clearMap
#define FIELDY 32
//コマンドリスト（GO1は１区画前進を意味する）
#define GO1    1//必ず１である必要がある
#define GO2    2
#define GO30  30
#define GO31  31
#define TURNR 32//GO31の次にあれば何でもよい
#define TURNL 33
#define DIA_TO_CLOTHOIDR 34
#define DIA_TO_CLOTHOIDL 35
#define DIA_FROM_CLOTHOIDR 36
#define DIA_FROM_CLOTHOIDL 37
#define DIA_TURNR 38
#define DIA_TURNL 39
#define DIA_GO1 40
#define DIA_GO63 102
#define SNODE 103//ストップを意味する

//コマンドリストここまで
int maze[FIELDY][FIELDX];
const int runtime[104]={0,//runtime[0]には意味はない．実装上そうなった
10,18,25,32,40,45,49,54,58,63,//GO1,GO2,...を行うためにかかる時間.
67,72,76,81,85,90,94,99,103,108,
112,117,121,126,130,135,139,144,148,153,157,
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

//ハッシュ番号をかえす．
//x      は0-31の5bit
//y      は0-31の5bit:y座標（左上が(0,0)）を保存
//d      は0-03の2bit:北向き，東向き，南向き，西向きに対応
//diaflagは0-2の2bit :斜めに向いていない，右斜め向き，左斜め向きに対応
int encoder(int x,int y,int d,int diaflag){
	return (diaflag<<12)+(d<<10)+(y<<5)+x;
}
void decoder(int hash, int *x, int *y, int *d, int *diaflag){//ハッシュ番号から座標をかえす
	*x =  hash      & (32-1);
	*y = (hash>> 5) & (32-1);
	*d = (hash>>10) & (4-1);
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
void addnode(int node, int lis[1<<7], int *lisnum, int prev[1<<12], int cost[1<<12]){
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
		if( 0<= nowx+dx && nowx+dx<FIELDX &&
		    0<= nowy+dy && nowy+dy<FIELDY){//範囲確認
			if(!(maze[nowy+dy][nowx+dx]&(1 << nowd))){ //壁認識
				nextnode = encoder(nowx+dx,nowy+dy, (nowd + 3) & 0x03, 1);//次ノードをひとまとめに設定
				if(cost[node]+runtime[DIA_FROM_CLOTHOIDR] < cost[nextnode] || prev[nextnode] == 0){//次のノードの計算と
					prev[nextnode] = DIA_FROM_CLOTHOIDR;
					cost[nextnode] = cost[node]+runtime[DIA_FROM_CLOTHOIDR];
					addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
				}
			}
			if(!(maze[nowy+dy][nowx+dx]&(1 << nowd))){
				nextnode = encoder(nowx+dx,nowy+dy, (nowd + 1) & 0x03, 2);
				if(cost[node]+runtime[DIA_FROM_CLOTHOIDL] < cost[nextnode] || prev[nextnode] == 0){
					prev[nextnode] = DIA_FROM_CLOTHOIDL;
					cost[nextnode] = cost[node]+runtime[DIA_FROM_CLOTHOIDL];
					addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
				}
			}
		}
		//ストレートの時間を追加
		for(i = GO1;i <= GO31;i++){
			if( 0<= nowx+dx*i && nowx+dx*i<FIELDX &&
				0<= nowy+dy*i && nowy+dy*i<FIELDY){
				if(!(maze[nowy+dy*i][nowx+dx*i]&(1 << nowd))){
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
		if( 0<= nowx+dx && nowx+dx<FIELDX &&
		    0<= nowy+dy && nowy+dy<FIELDY){
			if(nowdia == 1){
				if(!(maze[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TO_CLOTHOIDR
					nextnode = encoder(nowx+dx,nowy+dy, nowd, 0);
					if(cost[node]+runtime[DIA_TO_CLOTHOIDR] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = DIA_TO_CLOTHOIDR;
						cost[nextnode] = cost[node]+runtime[DIA_TO_CLOTHOIDR];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
				if(!(maze[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TURNR
					nextnode = encoder(nowx+dx,nowy+dy, (nowd + 3) & 0x03, 1);
					if(cost[node]+runtime[DIA_TURNR] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = DIA_TURNR;
						cost[nextnode] = cost[node]+runtime[DIA_TURNR];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
			}
			if(nowdia == 2){
				if(!(maze[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TO_CLOTHOIDL
					nextnode = encoder(nowx+dx,nowy+dy, nowd, 0);
					if(cost[node]+runtime[DIA_TO_CLOTHOIDL] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = DIA_TO_CLOTHOIDL;
						cost[nextnode] = cost[node]+runtime[DIA_TO_CLOTHOIDL];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
				if(!(maze[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TURNL
					nextnode = encoder(nowx+dx,nowy+dy, (nowd + 1) & 0x03, 2);
					if(cost[node]+runtime[DIA_TURNL] < cost[nextnode] || prev[nextnode] == 0){
						prev[nextnode] = DIA_TURNL;
						cost[nextnode] = cost[node]+runtime[DIA_TURNL];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					}
				}
			}
		}
		for(diastraight=DIA_GO1;diastraight<=DIA_GO63;diastraight++){//63
			switch(nowd){//メモ：from側の相対座標
				case 0:dx= 0;dy=+1;break;
				case 1:dx=-1;dy= 0;break;
				case 2:dx= 0;dy=-1;break;
				case 3:dx=+1;dy= 0;break;
			}
			if( 0<= nowx+dx && nowx+dx<FIELDX &&
			    0<= nowy+dy && nowy+dy<FIELDY){
				if(!(maze[nowy+dy][nowx+dx]&(1 << nowd))){//DIA_TURNR
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

int dijkstra(int startx, int starty, int startd, int goalx, int goaly, int route[100]){
	int prev[1<<14]={0};//0なら未確定ノード，1以上なら対応したコマンドが過去データ
	int cost[1<<14]={0};//コスト
	int lis[1<<7];//探索ノードをリスト構造で保持リストは未探索ノードの個数分がはいる十分な大きさを確保すること
	int lisnum=0;
	int i,t;
	int minnum;
	int minindex;
	int nowx , nowy , nowd, nowdia;
	int dx,dy;
	int nownode, nextnode;
	int routenum;
	int starttemp;
	int diadistance;
	//例外処理：すでに目的地にいるならルートは自明．（この例外処理は必要）
	if(goalx == startx && goaly == starty){
		route[0]=SNODE;
		return 1;
	}
	//例外処理：startx,startyマスだけは後ろに壁があってもいい．
	starttemp = maze[starty][startx];
	maze[starty][startx] &= 0xff - (1 << ((startd+2)&0x03));
	//初期化：ゴールノードは自明な確定ノード
	prev[encoder(goalx,goaly,0,0)]=SNODE;
	prev[encoder(goalx,goaly,1,0)]=SNODE;
	prev[encoder(goalx,goaly,2,0)]=SNODE;
	prev[encoder(goalx,goaly,3,0)]=SNODE;
	//初期化：リストにゴールノードの隣接ノードをつっこむ
	if(!(maze[goaly][goalx]&0x04)){addnode(encoder(goalx,goaly,0,0),lis,&lisnum,prev,cost);}
	if(!(maze[goaly][goalx]&0x08)){addnode(encoder(goalx,goaly,1,0),lis,&lisnum,prev,cost);}
	if(!(maze[goaly][goalx]&0x01)){addnode(encoder(goalx,goaly,2,0),lis,&lisnum,prev,cost);}
	if(!(maze[goaly][goalx]&0x02)){addnode(encoder(goalx,goaly,3,0),lis,&lisnum,prev,cost);}
	//探索開始，ベクトル場を作成する
	while(lisnum){
		//リストから最もコストが低いものを探す
		minnum = cost[lis[0]];
		minindex = 0;
		for(i = 1;i < lisnum;i++){//このループに入った段階でlisnum>0が成立するからi=1で初期化してよい
			if(minnum > cost[lis[i]]){
				minnum   = cost[lis[i]];
				minindex = i;
			}
		}
		//minindexから伸びる未探索ノードを追加
		addnode(lis[minindex],lis,&lisnum,prev,cost);
		//初期位置が確定ノードになったら探索終了
		if(lis[minindex]==encoder(startx,starty,startd,0)){
			lisnum=-1;//ここから抜けたことを番兵値で覚えておく
			break;
		}
		//minindexをリストから削除して確定ノード扱いに
		for(i = minindex;i < lisnum - 1;i++)
			lis[i]=lis[i+1];
		lisnum--;
	}
	//初期に行ったstartx,startyマスの例外処理の後片付け
	maze[starty][startx] = starttemp;
	//得られたベクトル場にそって，経路を作成する
	if(lisnum == -1){
		//prevノードをたどっていき，コマンドを確定させる
		nownode = encoder(startx,starty,startd,0);
		routenum = 0;
		while(prev[nownode]!=SNODE){
			decoder(nownode, &nowx, &nowy, &nowd, &nowdia);
			switch(nowd){
				case 0:dx = 0;dy = -1;break;
				case 1:dx = 1;dy =  0;break;
				case 2:dx = 0;dy = +1;break;
				case 3:dx =-1;dy =  0;break;
			}
			if(GO1<=prev[nownode] && prev[nownode]<=GO31){
				nextnode = encoder(nowx+dx*prev[nownode], nowy+dy*prev[nownode], nowd, 0);
			}
			if(prev[nownode] == DIA_TO_CLOTHOIDR)
				nextnode = encoder(nowx+dx, nowy+dy, nowd, 1);
			if(prev[nownode] == DIA_TO_CLOTHOIDL)
				nextnode = encoder(nowx+dx, nowy+dy, nowd, 2);
			if(prev[nownode] == DIA_FROM_CLOTHOIDR)
				nextnode = encoder(nowx-dy, nowy+dx, (nowd+1)&3, 0);
			if(prev[nownode] == DIA_FROM_CLOTHOIDL)
				nextnode = encoder(nowx+dy, nowy-dx, (nowd+3)&3, 0);
			if(prev[nownode] == DIA_TURNR)
				nextnode = encoder(nowx-dy, nowy+dx, (nowd+1)&3, 1);
			if(prev[nownode] == DIA_TURNL)
 				nextnode = encoder(nowx+dy, nowy-dx, (nowd+3)&3, 2);
			if(prev[nownode] == SNODE)
				nextnode = nownode;
			if(prev[nownode] == 0)
				return 0;//経路計算が正しく終わらなかった
			//ここから先はnowx,nowy,nowd,nowdiaの内容が壊れても問題ない．
			if(DIA_GO1<=prev[nownode] && prev[nownode]<=DIA_GO63){
				diadistance= (prev[nownode]-DIA_GO1+2)/2;
				if(nowdia==1){//右向き斜め走行
					switch(nowd){
						case 0:nowx+=diadistance;nowy-=diadistance;break;
						case 1:nowx+=diadistance;nowy+=diadistance;break;
						case 2:nowx-=diadistance;nowy+=diadistance;break;
						case 3:nowx-=diadistance;nowy-=diadistance;break;
					}
				}else{//左向き
					switch(nowd){
						case 0:nowx-=diadistance;nowy-=diadistance;break;
						case 1:nowx+=diadistance;nowy-=diadistance;break;
						case 2:nowx+=diadistance;nowy+=diadistance;break;
						case 3:nowx-=diadistance;nowy+=diadistance;break;
					}
				}
				if((prev[nownode]-DIA_GO1+1)%2){//奇数マスの斜め走行なら
					nowx-=dx;
					nowy-=dy;
					nowd =(nowd+nowdia*2-1) & 0x03;//右傾きなら右方向へ，左傾きなら左方向へnowdを回転
					nowdia = 3-nowdia;//向き反転
				}//偶数方向はnowd,nowdiaに変更の必要なし
				nextnode = encoder(nowx, nowy, nowd, nowdia);
			}
			route[routenum]=prev[nownode];
			routenum++;
			nownode = nextnode;
		}
		route[routenum]=SNODE;
	}else{//経路がみつからなかった
		return 0;
	}
	return 1;//正常に経路が求まった
}

void ReadMaze(char *filename){
	FILE *fp;
	signed int i,t;
	fp=fopen(filename,"r");
	if(fp==NULL){
		puts("cantopenfile");
	}else{
		for(i = 0;i <FIELDY ;i++){
			for(t = 0;t < FIELDX;t++){
				fscanf(fp,"%x",&maze[i][t]);
				maze[i][t]|=0xf0;
			}
			fscanf(fp,"\n");
		}
		fclose(fp);
		puts("load done");
	}
}
void printmaze(void){
	int i,t;
	for(i = 0;i <FIELDY ;i++){
		for(t = 0;t < FIELDX;t++){
			printf("%2x ",maze[i][t]);
		}
		puts("");
	}
}
main(){
	int route[100];
	int i;
	int t = 0;
	ReadMaze("maze.dat");
	if(dijkstra(0, FIELDY-1,0, 7, FIELDY-8, route)){
		puts("以下の経路が見つかりました");
		for(i = 0;route[i]!=SNODE;i++){
			if(GO1<=route[i] && route[i]<=GO31)
				printf("%dマス直進\n",route[i]);
			if(DIA_GO1<=route[i] && route[i]<=DIA_GO63)
				printf("%dマス斜め直進\n",route[i]-DIA_GO1+1);
			if(route[i]==TURNR)
				printf("右９０度方向へスラローム\n");
			if(route[i]==TURNL)
				printf("左９０度方向へスラローム\n");
			if(route[i]==DIA_TO_CLOTHOIDR)
				printf("直進から１マス使って斜め右方向へ\n");
			if(route[i]==DIA_TO_CLOTHOIDL)
				printf("直進から１マス使って斜め左方向へ\n");
			if(route[i]==DIA_FROM_CLOTHOIDR)
				printf("斜め右方向から直進へ\n");
			if(route[i]==DIA_FROM_CLOTHOIDL)
				printf("斜め左方向から直進へ\n");
			if(route[i]==DIA_TURNR)
				printf("斜めから右９０度方向ターンして斜めへ\n");
			if(route[i]==DIA_TURNL)
				printf("斜めから左９０度方向ターンして斜めへ\n");
			if(route[i]==SNODE)
				printf("おわり\n");
			t += runtime[route[i]];
		}
		printf("%d単位時間でゴールに到達します",t);
	}else{
		puts("軌道が見つからなかった");
	}
	return 0;
}