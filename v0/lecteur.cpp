#include "lecteur.h"

Lecteur::Lecteur()
{
    _numDiaporamaCourant = 0;   // =  le lecteur est vide
    _posImageCourante = 0; // Diaporama vide
}

void Lecteur::avancer()
{
    (*this)._posImageCourante = ((*this)._posImageCourante+1)%(*this).nbImages();
}

void Lecteur::reculer()
{
    (*this)._posImageCourante = ((*this)._posImageCourante-1)%(*this).nbImages();
}

void Lecteur::changerDiaporama(unsigned int pNumDiaporama)
{
    // s'il y a un diaporama courant, le vider, puis charger le nouveau Diaporama
    if (numDiaporamaCourant() > 0)
    {
        viderDiaporama();
    }
    _numDiaporamaCourant = pNumDiaporama;
    if (numDiaporamaCourant() > 0)
    {
        chargerDiaporama(); // charge le diaporama courant
    }

}

void Lecteur::chargerDiaporama()
{
    /* Chargement des images associées au diaporama courant
       Dans une version ultérieure, ces données proviendront d'une base de données,
       et correspondront au diaporama choisi */
    Image* imageACharger;
    imageACharger = new Image(3, "personne", "Blanche Neige", "C:\\cartesDisney\\carteDisney2.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(2, "personne", "Cendrillon", "C:\\cartesDisney\\carteDisney4.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(4, "animal", "Mickey", "C:\\cartesDisney\\carteDisney1.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(1, "personne", "Grincheux", "C:\\cartesDisney\\carteDisney1.gif");
    _diaporama.push_back(imageACharger);


     // trier le contenu du diaporama par ordre croissant selon le rang de l'image dans le diaporama
     // À FAIRE
     // Tri bulle
    int pass = 0;
    bool permut = true;
   
    while (permut)
    {
        permut = false;
        pass ++;
        for (unsigned int cpt=0;static_cast<int>(cpt)<20-pass;cpt++)
        {
            if ((*this)._diaporama[cpt]>(*this)._diaporama[cpt+1])
            {
                permut = true;
                // Échange des deux éléments
                Image *tmp = (*this)._diaporama[cpt];
                (*this)._diaporama[cpt] = (*this)._diaporama[cpt+1];
                (*this)._diaporama[cpt+1] = tmp;
            }
        }
    }

	if ((*this).nbImages() != 0)
    {
        _posImageCourante = 1;
    }
    else
    {
        _posImageCourante = 0;
    }

     cout << "Diaporama num. " << numDiaporamaCourant() << " selectionne. " << endl;
     cout << nbImages() << " images chargees dans le diaporama" << endl;

}

void Lecteur::viderDiaporama()
{
    if (nbImages () > 0)
    {
        unsigned int taille = nbImages();
        for (unsigned int i = 0; i < taille ; i++)
        {
            _diaporama.pop_back(); /* Removes the last element in the vector,
                                      effectively reducing the container size by one.
                                      AND deletes the removed element */
        }
     _posImageCourante = 0;
    }
    cout << nbImages() << " images restantes dans le diaporama." << endl;

}

void Lecteur::afficher()
{
    /* affiche les information sur le lecteur :
     * 1) vide (si num. de diaporama = 0) OU BIEN  numéro de diaporama affiché
     * 2) Si un diaporama courant est chargé (num. de diaporama > 0), affiche l'image courante OU BIEN 'diaporama vide'
     *     si ce diaporama n'a aucun image */
    
    // 1)
    if ((*this)._numDiaporamaCourant == 0)
    {
        cout << endl << "vide";
    }
    else
    {
        cout << (*this).numDiaporamaCourant();
    }

    // 2)
    if ((*this)._numDiaporamaCourant > 0)
    {
        if ((*this).nbImages() != 0) // Diaporama non vide
        {
            (*(*this).imageCourante()).afficher();
        }
        else
        {
            cout << endl << "Diaporama vide";
        }
    }
}

unsigned int Lecteur::nbImages()
{
    return static_cast<unsigned int>(_diaporama.size());
}

Image *Lecteur::imageCourante()
{
    return (*this)._diaporama[(*this)._posImageCourante-1];
}

unsigned int Lecteur::numDiaporamaCourant()
{
    return (*this)._numDiaporamaCourant;
}
