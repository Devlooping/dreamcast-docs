			Exemple VBMAIL

L'exemple VBMail utilise les contr�les MAPISessions et MAPIMessages en conjonction avec une application de messagerie sous-jacente, comme Microsoft Exchange ou Microsoft Outlook, pour afficher et composer les messages. L'exemple utilise �galement la biblioth�que ActiveX Data Objects Recordset 2.0 pour cr�er un jeu d'enregistrements autonome. Ce jeu d'enregistrements est rempli avec les messages non lus fournis par le contr�le MAPIMessages. 

Fichiers		Description

frmLogOn.frm		Connexion et lecture des messages non lus.
frmRead.frm		Feuille utilis�e pour lire un message.
prjMapi.vbp		Fichier de projet.

Ex�cution
Le contr�le DataGrid est utilis� pour afficher les messages non lus. Un clic sur un message en permet la lecture. Les autres fonctions sont l'affichage du carnet d'adresses du syst�me de messagerie sous-jacent et la composition d'un nouveau message.

� partir de la lecture d'un message, vous pouvez r�pondre, r�pondre � tous ou transf�rer le message.


