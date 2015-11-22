#include "DbRecordsPage.h"

zdb::DbRecordsPage::DbRecordsPage(utils::File* file, zint32 pageNumber, zint32 schemePage, 
	bool create, Database* db)
	: Page(file, pageNumber, DbPageType::Records, create, db)
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