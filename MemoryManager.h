#ifndef MEMORYMANAGER_H

#define SUCCESSFUL_EXECUTION 0
#define INVALID_PARAMETERS -1
#define OUT_OF_BLOCK_RANGE -2
#define OUT_OF_MEMORY -2
#define UNKNOW_ERROR 1

typedef char* VA;						// ��� ����������� ����� �����

typedef struct tableCell
{
	int segmentNumber;					//����� �������� ��������
	VA* physAddr;						//��������� �� ������������ � ������
	long segmentSize;					//������ ��������
	long offset;						//�������� ������������ ������ ������
	unsigned int modification;			//��� ����������� ��������
	unsigned int presence;				//��� ����������� �������� � ��
	struct tableCell* next;				//��������� �� ��������� ������
	struct tableCell* prev;				//��������� �� ���������� ������
} tableCell;

typedef struct segmentTable
{
	tableCell* head;					//��������� �� ������ �������
	tableCell* tail;					//��������� �� ����� �������
} segmentTable;

typedef struct segment					//�������
{
	int segmentNumber;					//����� �������� ��������
	struct segment* next;
	struct segment* prev;
} segment;

typedef struct physicalMemory			//��, �/�, ���
{
	VA* space;							//����������, ��������� �� ������
	long size;							//������
	segment* head;						//��������� �� ������ �������
	segment* tail;						//��������� �� ��������� �������
} physicalMemory;

physicalMemory ram;
physicalMemory hard;
physicalMemory cash;
segmentTable table;

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

#endif MEMORYMANAGER_H