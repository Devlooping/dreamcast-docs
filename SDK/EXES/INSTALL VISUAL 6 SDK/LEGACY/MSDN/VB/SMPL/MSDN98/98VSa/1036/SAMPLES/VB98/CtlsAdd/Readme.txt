                  CtlCfg.vbp et CtlView.vbp

Les deux projets de ce dossier illustrent l'ajout de contr�le dynamique (Form1.Controls.Add), ou la possibilit� d'ajouter des contr�les non r�f�renc�s au moment de l'ex�cution � une feuille, un document ActiveX ou un contr�le utilisateur.

L'exemple CtlCfg (configuration du contr�le) vous permet d'ajouter des informations au sujet de tout contr�le (contr�le intrins�que ou utilisateur) � la base de donn�es Controls.mdb. La base de donn�es stocke la cl� de licence du contr�le si une cl� est requise pour charger et ajouter le contr�le. La base de donn�es peut aussi stocker diverses propri�t�s pouvant �tre d�finies � mesure que le contr�le est ajout�e de mani�re dynamique. 

Une fois les informations sur le contr�le ajout�es � la base de donn�es, vous pouvez ouvrir l'exemple ctlView (affichage des contr�les). Cet exemple lit simplement la base de donn�es et affiche une liste des contr�les que vous pouvez ajouter de mani�re dynamique. Cliquez sur un contr�le pour l'ajouter � l'application. 

	NOTE : Assurez-vous que vous pouvez �crire dans le fichier Controls.mdb.

Pour plus d'informations sur l'ajout de contr�le dynamique, reportez-vous aux rubriques d'aide suivantes :

Add, m�thode (collection Controls)
Add, m�thode (collection Licenses)
EventInfo, objet
Licenses, collection
ObjectEvent, �v�nement
Parameter, objet (Visual Basic)
