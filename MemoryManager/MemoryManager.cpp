// MemoryManager.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "stdlib.h"

#define SUCCESSFUL_EXECUTION 0
#define INVALID_PARAMETERS -1
#define OUT_OF_BLOCK_RANGE -2
#define OUT_OF_MEMORY -2
#define UNKNOW_ERROR 1

typedef char* VA;						// ��� ����������� ����� ����� (� ��)

typedef struct tableCell
{
	int segmentNumber;					//����� �������� ��������
	VA* physAddr;						//��������� �� ������������ � ������
	long segmentSize;					//������ ��������
	long offset;
	unsigned int modification;			//��� ����������� ��������
	unsigned int presence;				//��� ����������� �������� � ��
	struct tableCell* next;				//��������� �� ��������� �������
} tableCell;

typedef struct segment					//�������
{
	int segmentNumber;
	struct segment* next;
} segment;

typedef struct physicalMemory			//��, �/�, ���
{
	VA* space;							//����������, ��������� �� ������
	long size;							//������
	segment* head;
} physicalMemory;



physicalMemory ram;
physicalMemory hard;
physicalMemory cash;
tableCell* table;



static int maxSegmentNumber = 0;



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



int _init_physical_memory(int memoryNumber, physicalMemory mem);
long _take_free_space(VA* ptr, size_t szBlock);
int _find_table_cell(int segmentNumber, tableCell* tc);
int _add_new_block(segment* prevSegm, tableCell* prevTC, VA* ptr, size_t szBlock);
int _find_last_segment(segment* segm);



int _tmain(int argc, _TCHAR* argv[])
{
	if(argc > 2){
		int n = (int)argv[1];
		int szPage = (int)argv[2];

		_init(n, szPage);
	}
	
	return 0;
}



int _init (int n, int szPage)
{
	int errCode, ramNumber = n * szPage, hardNumber = 65536, cashNumber = 2048;		//errCode - ��� ������

	errCode = _init_physical_memory(ramNumber, ram);		//�������������� ��

	_init_physical_memory(hardNumber, hard);		//�������������� ������ ��

	_init_physical_memory(cashNumber, cash);		//�������������� ���

	return errCode;
}


int _init_physical_memory(int memoryNumber, physicalMemory mem)
{
	int errCode, memorySize = memoryNumber * sizeof(VA);

	mem.space = (VA*) malloc(memorySize);

	if(mem.space == NULL){
		errCode = 1;

	}
	else{
		errCode = 0;

		mem.head = NULL;
		mem.size = 0;
	}

	return errCode;
}



int _malloc (VA* ptr, size_t szBlock)
{
	int errCode;

	if(szBlock <= 0){
		errCode = -1;
	}
	else{
		VA* ptr;		//OUT
		errCode = _take_free_space(ptr, szBlock);
	}

	return errCode;
}



long _take_free_space(VA* ptr, size_t szBlock)
{
	int errCode;
	
	if(ram.space == NULL) {
		return UNKNOW_ERROR;
	} else {
		if(ram.head == NULL) {
			ptr = ram.space;		//OUT

			errCode = _add_new_block(NULL, NULL, ptr, szBlock);

			if(errCode != SUCCESSFUL_EXECUTION) {
				return errCode;
			}

			return errCode;
		} else {
			long space, beginOfSpace, endOfSpace;

			segment* curSegm = ram.head;
			tableCell* curTabCell; 
			errCode = _find_table_cell(curSegm -> segmentNumber, curTabCell);

			if(errCode != SUCCESSFUL_EXECUTION) {
				return errCode;
			}

			while(curSegm -> next != NULL) {
				segment* nextSegm = curSegm -> next;
				tableCell* nextTabCell; 

				errCode = _find_table_cell(nextSegm -> segmentNumber, nextTabCell);

				if(errCode != SUCCESSFUL_EXECUTION) {
					return errCode;
				}

				beginOfSpace = curTabCell -> segmentSize + curTabCell -> offset;
				endOfSpace = nextTabCell -> offset;
				space = endOfSpace - beginOfSpace;

				if(space >= (long) szBlock) {
					errCode = _find_table_cell(curSegm -> segmentNumber, curTabCell);

					if(errCode != SUCCESSFUL_EXECUTION) {
						return errCode;
					}

					ptr = curTabCell -> physAddr;		//OUT

					errCode = _add_new_block(curSegm, curTabCell, ptr, szBlock);

					if(errCode != SUCCESSFUL_EXECUTION) {
						return errCode;
					}

					return SUCCESSFUL_EXECUTION;
				}

				curSegm = nextSegm;
			}

			beginOfSpace = curTabCell -> segmentSize + curTabCell -> offset;
			endOfSpace = ram.size;
			space = endOfSpace - beginOfSpace;

			if(space >= (long) szBlock) {
				segment* lastSegm = curSegm;
				tableCell* lastTabCell;

				errCode = _find_table_cell(lastSegm -> segmentNumber, lastTabCell);

				if(errCode != SUCCESSFUL_EXECUTION) {
					return errCode;
				}

				ptr = lastTabCell -> physAddr + lastTabCell -> segmentSize;		//OUT

				errCode = _add_new_block(curSegm, curTabCell, ptr, szBlock);

				if(errCode != SUCCESSFUL_EXECUTION) {
					return errCode;
				}

				return SUCCESSFUL_EXECUTION;
			} else {
				return OUT_OF_MEMORY;
			}
		}
	}
}



int _find_table_cell(int segmentNumber, tableCell* tc)
{
	tc = table;

	do {
		if(tc -> segmentNumber == segmentNumber) {
			return SUCCESSFUL_EXECUTION;
		}

		tc = tc -> next;
	} while(tc != NULL);

	return INVALID_PARAMETERS;
}



int _add_new_block(segment* prevSegm, tableCell* prevTC, VA* ptr, size_t szBlock)
{
	tableCell* tc = (tableCell*) malloc(sizeof(tableCell));
	segment* segm = (segment*) malloc(sizeof(segment));

	if(tc == NULL || segm == NULL) {
		return UNKNOW_ERROR;
	}

	tc -> physAddr = ptr;
	tc -> modification = 0;
	tc -> presence = 1;
	tc -> next = NULL;
	tc -> segmentSize = szBlock;

	if(prevTC != NULL) {
		prevTC -> next = tc;
	}

	segm -> segmentNumber = tc -> segmentNumber = maxSegmentNumber++;
	segm -> next = NULL;

	if(prevSegm != NULL) {	//���������, ������ �� ��� ������� ��� ���
		prevSegm -> next = segm;	//���� ��, �� ��������� � ����������� �������� �� �������
		tc -> offset = prevTC -> segmentSize;	//������������� �������� �������� ��������, ��� ������ �����������
	} else {
		tc -> offset = 0;	//���� ���, �� ������������� �������� � 0
	}

	return SUCCESSFUL_EXECUTION;
}



int _find_last_segment(segment* segm) 
{
	segm = ram.head;

	while(segm != NULL) {
		if(segm -> next == NULL) {
			return SUCCESSFUL_EXECUTION;
		}

		segm = segm -> next;
	}

	return UNKNOW_ERROR;
}