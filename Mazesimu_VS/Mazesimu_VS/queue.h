#pragma once
#include <stdio.h>
#define SIZE 64

typedef struct {
	uint8_t		x;
	uint8_t		y;
	uint16_t	step;
	uint8_t		direction;	//north,northeast,east,southeast,south,southwest,west,northwest
}stPOSITION;

enum status {
	EMPTY,
	AVAILABLE,
	FULL
};

typedef struct {
	stPOSITION data[SIZE];
	int head;
	int tail;
	int flag;
}queue_t;

//キューの中身をprint出力
void printQueue(queue_t* pQueue) {
	int i;
	for (i = 0; i <= SIZE - 1; i++) {
		printf("%d ", pQueue->data[i]);
	}
	printf("\n");
}

//キューの初期化
void initQueue(queue_t* pQueue) {
	int i;
	//キューの中身を0埋め
	for (i = 0; i <= SIZE - 1; i++) {
		pQueue->data[i] = { 0 };
	}
	//初期化
	pQueue->head = 0;
	pQueue->tail = 0;
	pQueue->flag = EMPTY;
//	printQueue(pQueue);
}


//enqueue関数
void enqueue(queue_t* pQueue, stPOSITION value) {
//	printf("enQ(%d)\n", value);
	//キューがFullの処理
	if (pQueue->flag == FULL) {
		printf("Full\n");
		return;
	}
	//キューがFullでないので、enqueue操作
	pQueue->data[pQueue->tail] = value;
	//リングバッファのため、tailが配列の終端だったら0にする
	if (pQueue->tail == SIZE - 1) {
		pQueue->tail = 0;
		//終端でなければ、tailをインクリメント
	}
	else {
		pQueue->tail++;
	}
	//フラグの更新
	if (pQueue->tail == pQueue->head) {
		pQueue->flag = FULL;
	}
	else {
		pQueue->flag = AVAILABLE;
	}
//	printQueue(pQueue);
}

//dequeue関数
stPOSITION dequeue(queue_t* pQueue) {
//	printf("deQ\n");
	stPOSITION dqueue;
	//キューがEmptyの処理
	if (pQueue->flag == EMPTY) {
		printf("Empty\n");
		return { 0 };
	}
	stPOSITION dequeue = pQueue->data[pQueue->head];
	//キューがEmptyでなければ、dequeue操作
	pQueue->data[pQueue->head] = { 0 };
	//リングバッファのため、headが配列の終端だったら0にする
	if (pQueue->head == SIZE - 1) {
		pQueue->head = 0;
		//終端でなければ、headをインクリメント
	}
	else {
		pQueue->head++;
	}
	//フラグの更新
	if (pQueue->tail == pQueue->head) {
		pQueue->flag = EMPTY;
	}
	else {
		pQueue->flag = AVAILABLE;
	}
//	printQueue(pQueue);

	return dequeue;
}