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

//�L���[�̒��g��print�o��
void printQueue(queue_t* pQueue) {
	int i;
	for (i = 0; i <= SIZE - 1; i++) {
		printf("%d ", pQueue->data[i]);
	}
	printf("\n");
}

//�L���[�̏�����
void initQueue(queue_t* pQueue) {
	int i;
	//�L���[�̒��g��0����
	for (i = 0; i <= SIZE - 1; i++) {
		pQueue->data[i] = { 0 };
	}
	//������
	pQueue->head = 0;
	pQueue->tail = 0;
	pQueue->flag = EMPTY;
//	printQueue(pQueue);
}


//enqueue�֐�
void enqueue(queue_t* pQueue, stPOSITION value) {
//	printf("enQ(%d)\n", value);
	//�L���[��Full�̏���
	if (pQueue->flag == FULL) {
		printf("Full\n");
		return;
	}
	//�L���[��Full�łȂ��̂ŁAenqueue����
	pQueue->data[pQueue->tail] = value;
	//�����O�o�b�t�@�̂��߁Atail���z��̏I�[��������0�ɂ���
	if (pQueue->tail == SIZE - 1) {
		pQueue->tail = 0;
		//�I�[�łȂ���΁Atail���C���N�������g
	}
	else {
		pQueue->tail++;
	}
	//�t���O�̍X�V
	if (pQueue->tail == pQueue->head) {
		pQueue->flag = FULL;
	}
	else {
		pQueue->flag = AVAILABLE;
	}
//	printQueue(pQueue);
}

//dequeue�֐�
stPOSITION dequeue(queue_t* pQueue) {
//	printf("deQ\n");
	stPOSITION dqueue;
	//�L���[��Empty�̏���
	if (pQueue->flag == EMPTY) {
		printf("Empty\n");
		return { 0 };
	}
	stPOSITION dequeue = pQueue->data[pQueue->head];
	//�L���[��Empty�łȂ���΁Adequeue����
	pQueue->data[pQueue->head] = { 0 };
	//�����O�o�b�t�@�̂��߁Ahead���z��̏I�[��������0�ɂ���
	if (pQueue->head == SIZE - 1) {
		pQueue->head = 0;
		//�I�[�łȂ���΁Ahead���C���N�������g
	}
	else {
		pQueue->head++;
	}
	//�t���O�̍X�V
	if (pQueue->tail == pQueue->head) {
		pQueue->flag = EMPTY;
	}
	else {
		pQueue->flag = AVAILABLE;
	}
//	printQueue(pQueue);

	return dequeue;
}