Ce serveur personnalis� vise � fournir un objet de recherche rapide capable d'explorer un r�seau d'entreprise � la recherche d'un client, sans indication sur la mani�re de proc�der ni l'endroit de la recherche; ce client peut se trouver dans une base de donn�es parmi cinquante bases de donn�es diff�rentes (p. ex. un gopher).

Le sc�nario utilis� par ce serveur simule une strat�gie de recherche simple, les chemins de recherche concernant une base de donn�es locale (Testdata) par opposition � une base de donn�es distante (Tastrade).

En r�alit�, nous changeons l'environnement de donn�es courant d'un formulaire et rempla�ons les propri�t�s de la source de donn�es li�e par les nouvelles propri�t�s du serveur. Cette strat�gie permet d'utiliser Visual FoxPro dans un mod�le � trois niveaux, pour lequel les services utilisateur ne sont pas aussi �troitement li�s aux services donn�es que dans la plupart des syst�mes client/serveur actuels.

Examinons l'�v�nement BeforeOpenTables de l'environnement de donn�es du formulaire.

Proc�dure � suivre:

1. Veillez � modifier le chemin de base de donn�es dans le programme SearchData afin d'y placer votre r�pertoire de travail VFP5.

2. Vous devrez r�g�n�rer le projet Foxsrch.pjx et cr�er un serveur local (EXE) pour permettre son enregistrement. Le code qui appelle le serveur se trouve dans l'�v�nement BeforeOpenTables de l'environnement de donn�es WING1.SCX.

3. Ex�cutez le formulaire.


IMPORTANT!!! Cet exemple tr�s simple a �t� con�u pour illustrer le concept de gopher et fonctionne uniquement avec un serveur local. N'essayez pas de l'utiliser pour un serveur d'automation distante; sinon, un blocage survient lorsque le client attend la r�alisation d'un appel alors que le serveur tente d'y r�pondre. L'exemple de Gestionnaire de groupe constitue un meilleur exemple de r�ponse OLE par automation distante.