#ifndef BASEDEDONNEES_H
#define BASEDEDONNEES_H

#include <QSqlDatabase>

#define DATABASE_NAME "Cmourgue_BD_S201" // Le nom du DSN permettant d'accéder à la BD
#define CONNECT_TYPE "QODBC" // Indique que l'on veut utiliser le driver odbc

class BaseDeDonnees
{
public:
    BaseDeDonnees();
    bool openDataBase();
    void closeDataBase();
    bool isAlreadyOpen();

private :
    QSqlDatabase m_db;
};

#endif // BASEDEDONNEES_H
