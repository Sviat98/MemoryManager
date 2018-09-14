// MemoryManager.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "stdlib.h"

typedef char* VA;						// ��� ����������� ����� ����� (� ��)

typedef struct physicalMemory			//��
{
	long size;							//������ ������
	VA space;							//����������, ��������� �� ������
} physicalMemory;

typedef struct virtualSegmentBlock		//���� ������������ ��������
{
	long offset;						//�������� �����
	struct virtualSegmentBlock* next;	//��������� �� ��������� ����
	struct virtualSegmentBlock* prev;	//��������� �� ���������� ����
} segmentBlock;

typedef struct virtualSegment			//����������� �������
{
	int segmNum;						//����� ��������
	virtualSegmentBlock* head;			//��������� �� ������ ��������
	virtualSegmentBlock* tail;			//��������� �� ����� ��������
	struct virtualSegment* next;		//��������� �� ��������� �������
	struct virtualSegment* prev;		//��������� �� ���������� �������
} virtualSegment;

typedef struct virtualSegmentsList		//������ ����������� ���������
{
	virtualSegment* head;				//��������� �� ������ ������
	virtualSegment* tail;				//��������� �� ����� ������
} virtualSegmentsList;

typedef struct segmentTableCell			//������ ������� ���������
{
	int segmNum;						//����� ������������ ��������
	VA physAddr;						//��������� �� ��������������� ������� � ��
	long segmentSize;					//������ ��������
	unsigned int modification;			//��� ����������� ��������
	unsigned int presence;				//��� ����������� �������� � ��
	struct segmentTableCell* next;		//��������� �� ��������� ������� �������
	struct segmentTableCell* prev;		//��������� �� ���������� ������� �������
} segmentTableCell;

typedef struct segmentTable				//������� ���������
{
	int tableSize;						//������ �������
	segmentTableCell* head;				//��������� �� ������ �������
	segmentTableCell* tail;				//��������� �� ����� �������
} segmentTable;

physicalMemory physMem;
segmentTable *table;



/**
 	@func	_malloc	
 	@brief	�������� ���� ������ ������������� �������
	
	@param	[out] ptr		������ �����
	@param	[in]  szBlock	������ �����
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	�������� ������
	@retval	1	����������� ������
 **/
int _malloc (VA* ptr, size_t szBlock);



/**
 	@func	_free
 	@brief	�������� ����� ������
	
	@param	[in] ptr		������ �����
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
int _free (VA ptr);



/**
 	@func	_read
 	@brief	������ ���������� �� ����� ������
	
	@param	[in] ptr		������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
int _read (VA ptr, void* pBuffer, size_t szBuffer);



/**
 	@func	_write
 	@brief	������ ���������� � ���� ������
	
	@param	[in] ptr		������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
int _write (VA ptr, void* pBuffer, size_t szBuffer);



/**
 	@func	_init
 	@brief	������������� ������ ��������� ������
	
	@param	[in] n		���������� �������
	@param	[in] szPage	������ ��������

	� �������� 1 � 2 ����� ����� ������ ������������� ��� n*szPage
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
int _init (int n, int szPage);



int _create_table();



int _push(VA* data, size_t szPage);



int _tmain(int argc, _TCHAR* argv[])
{
	if(argc > 2){
		int n = (int)argv[1];
		int szPage = (int)argv[2];

		_init(n, szPage);

		//FILE *f = fopen("r");
	}
	
	return 0;
}



int _init (int n, int szPage)
{
	int errCode, iter;									//errCode - ��� ������

	errCode = _init_physical_memory(n, szPage);			//�������������� ��

	errCode = _create_table();							//������ �������

	for(iter = 0; iter < n && errCode == 1; iter++){	//��������� � ������� n ����� ����� szPage
		errCode = _push(NULL, szPage);
	}

	return errCode;
}



int _init_physical_memory(int n, int szPage)
{
	int errCode;

	int memorySize = n * szPage * sizeof(VA);
	physMem.space = (VA) malloc(memorySize);

	if(physMem.space == NULL){
		errCode = 1;
	}
	else{
		errCode = 0;
	}

	return errCode;
}



int _create_table()											//�������� �������
{
	int errCode;

	table = (segmentTable*) malloc(sizeof(segmentTable));	//�������� ������ ��� �������

	if(!table){												//�������� ��������� ������
		errCode = -1;
	}
	else{
		errCode = 1;

		table -> tableSize = 0;								//� ������� ��� ���������
		table -> head = table -> tail = NULL;				//��� �� �������, �� ���������� ��������
	}

	return errCode;
}



int _pushTableCell(VA* dataPtr, size_t szPage)  //��������� ������ � ����� �������
{
	int errCode;

	segmentTableCell *tmp = (segmentTableCell*) malloc(sizeof(segmentTableCell));	//�������� ������ ��� ������ �������

	if(!tmp){										//�������� ��������� ������
		errCode = -1;
	}
	else{
		errCode = 1;

		long virtualAddr = 

		segment* segm = (segment*) malloc(sizeof(segment));	//�������� ������ ��� �������

		tmp -> segm = segm;							//�������� ����� ��������
		tmp -> segmentSize = szPage;				//�������� ������ ��������
		tmp -> modification = 0;					//������� �� ��� ������
		tmp -> presence = 0;						//������� �� ������������ � ��
		tmp -> next = NULL;							//����� ������ ��������� �� ������� ������
		tmp -> prev = table -> tail;				//� ����� �� �������� ������ �������

		if(table -> head){							//���� ������� �� �����
			table -> tail -> next = tmp;			//��������� �� ����� ������ �� ��������� ������ 
			table -> tail = tmp;					//� ��������� ����� ������ � ����� �������
		}
		else{										//���� ������� �����
			table -> head = table -> tail = tmp;	//����� ������ ���������� ������������ ������ � ���������
		}

		table -> tableSize++;						//����������� ����� �������
	}

	return errCode;
}



int _pushSegmentBlock(long virtualAddr, VA* physAddr, segment* segm)	//��������� ���� � ����� ��������
{
	int errCode;

	segmentBlock* blc = (segmentBlock*) malloc(sizeof(segmentBlock));	//�������� ������ ��� ����

	if(!segm){										//�������� ��������� ������
		errCode = -1;
	}
	else{
		errCode = 1;

		VA* physAddr = (VA*) malloc(sizeof(VA));	//�������� ���. ������

		blc -> virtualAddr = virtualAddr;			//�������� ����� ����� ��������
		blc -> physAddr = physAddr;					//�������� ������ ��������
		blc -> next = NULL;							//����� ������ ��������� �� ������� ������
		blc -> prev = segment -> tail;				//� ����� �� �������� ������ �������

		if(table -> head){							//���� ������� �� �����
			table -> tail -> next = tmp;			//��������� �� ����� ������ �� ��������� ������ 
			table -> tail = tmp;					//� ��������� ����� ������ � ����� �������
		}
		else{										//���� ������� �����
			table -> head = table -> tail = tmp;	//����� ������ ���������� ������������ ������ � ���������
		}

		table -> tableSize++;						//����������� ����� �������
	}

	return errCode;
}



int _malloc (VA* ptr, size_t szBlock){
	
}
