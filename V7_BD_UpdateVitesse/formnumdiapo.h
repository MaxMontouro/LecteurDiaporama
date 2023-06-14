#ifndef FORMNUMDIAPO_H
#define FORMNUMDIAPO_H

#include <QDialog>

#include "basededonnees.h"

namespace Ui {
class formNumDiapo;
}

class formNumDiapo : public QDialog
{
    Q_OBJECT

public:
    explicit formNumDiapo(QWidget *parent = nullptr,
                          BaseDeDonnees* = new BaseDeDonnees());
    ~formNumDiapo();

    unsigned int getNumDiapo(); // Getter de la vitesse de défilement
    bool m_accepte=false; // Le booléen indiquant si le résultat est envoyé et validé ou non, de base à false
    bool getAccepte(); // Getter du booléen m_accepte

    unsigned int m_numDiapo=0;

private:
    Ui::formNumDiapo *ui;

    BaseDeDonnees *m_db;
    void majTable();

public slots :
    void demandeAccepter(); // Passe le booléen m_accepte à true
    void majNumDiapo(int,int);
};

#endif // FORMNUMDIAPO_H
