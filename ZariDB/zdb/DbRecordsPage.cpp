#include "DbRecordsPage.h"

zdb::DbRecordsPage::DbRecordsPage(utils::File* file, zint32 pageNumber, zint32 schemePage) 
	: Page(file, pageNumber, DbPageType::Records)
{
	this->schemePage = schemePage;
	file->Write(&this->schemePage, sizeof(this->schemePage));
	file->Seek(freeBytesCountOffset);
	freeBytesCount -= sizeof(this->schemePage);
	file->Write(&freeBytesCount, sizeof(freeBytesCount));
	firstFreeByteOffset += sizeof(this->schemePage);
	file->Write(&firstFreeByteOffset, sizeof(firstFreeByteOffset));
}

zdb::DbRecordsPage::~DbRecordsPage()
{

}