#ifndef FORMNUMDIAPO_H
#define FORMNUMDIAPO_H

#include <QDialog>

#include <QSqlDatabase>

#define DATABASE_FORM_NAME "Cmourgue_BD_Form_S201" // Le nom du DSN permettant d'accéder à la BD
#define CONNECT_TYPE "QODBC" // Indique que l'on veut utiliser le driver odbc

namespace Ui {
class formNumDiapo;
}

class formNumDiapo : public QDialog
{
    Q_OBJECT

public:
    explicit formNumDiapo(QWidget *parent = nullptr);
    ~formNumDiapo();

    unsigned int getNumDiapo(); // Getter de la vitesse de défilement
    bool m_accepte=false; // Le booléen indiquant si le résultat est envoyé et validé ou non, de base à false
    bool getAccepte(); // Getter du booléen m_accepte

    unsigned int m_numDiapo=0;

private:
    Ui::formNumDiapo *ui;

    QSqlDatabase m_db;
    void majTable();
    bool openDataBase();
    void closeDataBase();

public slots :
    void demandeAccepter(); // Passe le booléen m_accepte à true
    void majNumDiapo(int,int);
};

#endif // FORMNUMDIAPO_H
