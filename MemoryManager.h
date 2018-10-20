#ifndef MEMORYMANAGER_H

#define SUCCESSFUL_EXECUTION 0
#define INVALID_PARAMETERS -1
#define OUT_OF_BLOCK_RANGE -2
#define OUT_OF_MEMORY -2
#define UNKNOW_ERROR 1

typedef char* VA;						// ��� ����������� ����� ����� � ����������� �������� ������������ (���)
typedef char* PA;						// ��� ����������� ����� ����� � ��

typedef struct tableCell
{
	int segmentNumber;					//����� �������� ��������
	PA physAddr;						//��������� �� ������������ � ��
	long segmentSize;					//������ ��������
	unsigned int modification;			//��� ����������� ��������
	unsigned int presence;				//��� ����������� �������� � ��
	struct tableCell* next;				//��������� �� ��������� ������
	struct tableCell* prev;				//��������� �� ���������� ������
} tableCell;

typedef struct segmentTable				//������� ���������
{
	tableCell* head;					//��������� �� ������ �������
	tableCell* tail;					//��������� �� ����� �������
} segmentTable;

typedef struct segment					//�������
{
	int segmentNumber;					//����� �������� ��������
	VA virtAddr;						//��������� �� ������������ � ���
	long segmentSize;					//������ ��������
	long offset;						//�������� ������������ ������ ���
	struct segment* next;				//��������� �� ��������� �������
	struct segment* prev;				//��������� �� ���������� �������
} segment;

typedef struct virtualAddressSpace		//��, �/�
{
	VA space;							//����������, ��������� �� ������ ���
	long ramSize;						//������ ��� ��
	long hardSize;						//������ ��� �/�
	long ramFree;						//���������� ��������� ����� ��� ��
	long hardFree;						//���������� ��������� ����� ��� �/�
	segment* head;						//��������� �� ������ �������
	segment* tail;						//��������� �� ��������� �������
} virtualAddressSpace;

virtualAddressSpace vas;
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

int _init_virtual_address_space(virtualAddressSpace* vas, long ramSize, long hardSize);
int _destroy(virtualAddressSpace* vas);

int _take_free_space(VA* ptr, size_t szBlock);
int _add_new_block(segment* prevSegm, segment* nextSegm, VA* ptr, size_t szBlock);
int _add_table_cell(tableCell* prevTC, tableCell* nextTC, size_t szBlock);
int _add_segment(segment* prevSegm, segment* nextSegm, VA* ptr, size_t szBlock);

int _find_segment_by_ptr(segment** segm, VA ptr);
int _find_table_cell_by_segment_number(tableCell** tc, int segmentNumber);

int _free_table_cell(tableCell** tc);
int _free_segment(segment** segm);

int _free_space_to_load_segment(segment** segm);
void _load_segment_to_ram(tableCell** tc);
void _load_segment_to_hard_drive(tableCell** tc);

#endif MEMORYMANAGER_H