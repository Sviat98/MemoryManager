// MemoryManager.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "stdlib.h"

typedef char* VA;						// ��� ����������� ����� ����� (� ��)

typedef struct physicalBlock
{
	long offset;
	long size;
	struct physicalBlock* next;
} physicalBlock;

typedef struct physicalMemory			//��
{
	VA space;							//����������, ��������� �� ������
	long size;
	physicalBlock* head;
} physicalMemory;

/*

typedef struct virtualSegment			//����������� �������
{
	int number;							//����� ��������
	struct virtualSegment* next;		//��������� �� ��������� �������
} virtualSegment;

*/

typedef struct segmentsList				//������ ���������
{
	int number;							//����� ������������ �������� (����� ����� ������� ����� ���.�����???)
	VA physAddr;						//��������� �� ������������ � ��
	long segmentSize;					//������ ��������
	unsigned int modification;			//��� ����������� ��������
	unsigned int presence;				//��� ����������� �������� � ��
	struct segmentsList* next;			//��������� �� ��������� �������
} segmentsList;

physicalMemory physMem;
segmentsList* list;



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

														//�������������� ������ ��� ��

														//�������������� ������ ��� ���

	/*errCode = _create_table();							//������ �������

	for(iter = 0; iter < n && errCode == 1; iter++){	//��������� � ������� n ����� ����� szPage
		errCode = _push(NULL, szPage);
	}*/

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



int _pushSegmentBlock(long virtualAddr, VA* physAddr, virtualSegment* segm)	//��������� ���� � ����� ��������
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



int _malloc (VA* ptr, size_t szBlock)
{
	ptr = _find_free_space(szBlock);
	_add_new_block();
}



int _add_new_block()
{
	
	int errCode;

	physicalBlock* blc = phys;	//�������� ������ ��� ����

	if(!blc){										//�������� ��������� ������
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



VA* _find_free_space(size_t szBlock)
{
	if(physMem.space == NULL){
		return NULL;
	}
	else{
		if(physMem.head == NULL){
			return &physMem.space;
		}
		else{
			physicalBlock* curBlock = physMem.head;
			physicalBlock* nextBlock = curBlock -> next;
			int beginOfSpace, endOfSpace, space;
			bool searchingAnswer = false;

			while(curBlock -> next != NULL){
				beginOfSpace = curBlock -> offset + curBlock -> size;
				endOfSpace = nextBlock -> offset;
				space = endOfSpace - beginOfSpace;

				if(space >= szBlock){
					return (&physMem.space + beginOfSpace);
				}
			}

			space = physMem.size - beginOfSpace;

			if(space >= szBlock){
				return (&physMem.space + beginOfSpace);
			}
			else{
				return NULL;
			}
		}
	}
}