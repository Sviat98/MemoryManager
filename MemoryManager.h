#ifndef MEMORYMANAGER_H

#define SUCCESSFUL_EXECUTION 0
#define INVALID_PARAMETERS -1
#define OUT_OF_BLOCK_RANGE -2
#define OUT_OF_MEMORY -2
#define UNKNOW_ERROR 1

typedef char* VA;						//��� ����������� ����� ����� � ���
typedef char* PA;						//��� ����������� ����� ����� � ���������� ������

typedef struct segment					//�������
{
	int segmentNumber;					//����� �������� ��������
	VA virtAddr;						//��������� �� ������������ � ���
	long segmentSize;					//������ ��������
	long offset;						//�������� ������������ ������ ���
	struct segment* next;				//��������� �� ��������� �������
	struct segment* prev;				//��������� �� ���������� �������
} segment;

typedef struct virtualAddressSpace		//����������� �������� ������������ (���)
{
	VA space;							//����������, ��������� �� ������ ���
	long size;							//������ ���
	long ramFree;						//���������� ��������� ����� ��
	long hardFree;						//���������� ��������� ����� �/�
	segment* head;						//��������� �� ������ �������
	segment* tail;						//��������� �� ��������� �������
} virtualAddressSpace;

typedef struct tableCell				//������ ������� ���������
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
	virtualAddressSpace vas;			//����������� �������� ������������ ��������
	tableCell* head;					//��������� �� ������ �������
	tableCell* tail;					//��������� �� ����� �������
} segmentTable;

typedef struct hardSegment				//������� �� �/�
{
	int segmentNumber;					//����� ��������
	PA data;							//������������ � ������
	long segmentSize;					//������
	struct hardSegment* next;			//��������� �� ��������� �������
	struct hardSegment* prev;			//��������� �� ���������� �������
} hardSegment;

typedef struct hardDrive
{
	hardSegment* head;					//��������� �� ������ �������
	hardSegment* tail;					//��������� �� ��������� �������
} hardDrive;

typedef struct cashRecord				//������ ����
{
	PA physAddr;						//����� ������ � �������� ������
	PA data;							//������ 
	unsigned int modification;			//��� ����������� ������
	unsigned int reality;				//��� ����������� �������� � ����
	struct cashRecord* next;			//��������� �� ��������� ������
	struct cashRecord* prev;			//��������� �� ���������� ������
} cashRecord;

typedef struct cash						//���
{
	cashRecord* head;					//��������� �� ������ ������
	cashRecord* tail;					//��������� �� ��������� ������
} cash;

segmentTable table;
hardDrive drive;
cash csh;

static int curSegmentNumber = 0;
static int curRecordNumber = 0;

static const int ramSize = 8192;
static const int hardSize = 65536;
static const int maxRecordNumber = 5;
static const int maxRecordSize = 10;

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

int _init_virtual_address_space(long size);
int _destroy_virtual_address_space();
int _init_cash();
int _add_cash_record();
int _destroy_cash();

int _take_free_space(VA* ptr, size_t szBlock);
int _add_new_block(segment* prevSegm, segment* nextSegm, VA* ptr, size_t szBlock);
int _add_table_cell(tableCell* prevTC, tableCell* nextTC, size_t szBlock);
int _add_segment(segment* prevSegm, segment* nextSegm, VA* ptr, size_t szBlock);
int _add_hard_segment(size_t szBlock);

int _find_segment_by_ptr(segment** segm, VA ptr);
int _find_table_cell_by_segment_number(tableCell** tc, int segmentNumber);
int _find_hard_segment_by_segment_number(hardSegment** hardSegm, int segmNumber);
int _find_cash_record_by_physical_address(cashRecord** rec, PA physAddr);
int _find_cash_record_by_current_number(cashRecord** rec);
int _find_table_cell_by_physical_address(tableCell** tc, PA physAddr);

int _free_table_cell(tableCell** tc);
int _free_segment(segment** segm);
int _free_hard_segment(hardSegment** hardSegm);
int _free_cash_record(cashRecord** rec);

int _free_space_to_load_segment(segment** segm);
int _find_cash_record_to_load_segment(cashRecord** rec, tableCell* tc);

int _load_segment_to_ram(tableCell** tc);
int _load_segment_to_hard_drive(tableCell** tc);
int _load_segment_from_cash(cashRecord** rec);
int _load_segment_to_cash(tableCell** tc, cashRecord** rec);

#endif MEMORYMANAGER_H