#include "basededonnees.h"

BaseDeDonnees::BaseDeDonnees()
{

}

bool BaseDeDonnees::openDataBase()
{
    m_db = QSqlDatabase::addDatabase(CONNECT_TYPE);
    m_db.setDatabaseName(DATABASE_NAME);
    return m_db.open();
}

void BaseDeDonnees::closeDataBase()
{
    this->m_db.close();
}

bool BaseDeDonnees::isAlreadyOpen()
{
    return this->m_db.isOpen();
}
