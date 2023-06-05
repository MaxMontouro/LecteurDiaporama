#ifndef FORMVITESSEDEFILEMENT_H
#define FORMVITESSEDEFILEMENT_H

#include <QDialog>

namespace Ui {
class formVitesseDefilement;
}

class formVitesseDefilement : public QDialog
{
    Q_OBJECT

public:
    explicit formVitesseDefilement(QWidget *parent = nullptr, unsigned int = 2);
    ~formVitesseDefilement();

    unsigned int getVitesseDefilement(); // Getter de la vitesse de défilement
    bool m_accepte=false; // Le booléen indiquant si le résultat est envoyé et validé ou non, de base à false
    bool getAccepte(); // Getter du booléen m_accepte

private:
    Ui::formVitesseDefilement *ui;

public slots :
    void demandeAccepter(); // Passe le booléen m_accepte à true
};

#endif // FORMVITESSEDEFILEMENT_H
