                  Exemple d'interface IObjectSafety Visual Basic 6
                                Document Lisezmoi
                                19 mars 1998

R�sum�
-------
   IObjSafe.OCX impl�mente l'interface IObjectSafety, qui expose les fonctionnalit�s 
   aux fonctions de Internet Explorer 4.x S�curis� pour le script et S�curis� pour 
   l'initialisation.

D�finition IObjectSafety
------------------------
   IObjectSafety doit �tre impl�ment� par des objets dont les interfaces prennent
   en charge les clients non fiables (par exemple, les scripts). Il permet au d�tenteur
   de l'objet de sp�cifier quelles interfaces doivent �tre prot�g�es d'une
   utilisation non fiable. Voici des exemples d'interfaces pouvant �tre prot�g�es
   de cette mani�re :

   IID_IDispatch         - "S�curis� pour l'automation avec client d'automation non fiable
			    ou script"
   IID_IPersist*         - "S�curis� pour l'initialisation avec des donn�es non fiables"
   IID_IActiveScript     - "S�curis� pour l'ex�cution des scripts non fiables"

Probl�mes de s�curit�
--------------------
   Il existe trois sc�narios de script de s�curit� �ventuels � prendre en consid�ration.

   i.   L'objet est toujours s�curis� pour le script.
        Si vous �tes s�r que toutes les m�thodes et propri�t�s de votre objet sont
        s�curis�es pour le script, vous devez notifier l'application client h�te en 
        ne renvoyant pas d'erreur dans la m�thode de l'interface
        IObjectSafety_SetInterfaceSafetyOptions. Cela permet la cr�ation et 
        l'ex�cution correctes de l'objet.

   ii.  L'objet n'est jamais s�curis� pour le script.
        Si vous �tes s�r qu'aucune m�thode et propri�t� de votre objet n'est
        s�curis�e pour le script, vous devez notifier l'application client h�te en 
        renvoyant une erreur, E_Fail, dans la m�thode de l'interface
        IObjectSafety_SetInterfaceSafetyOptions. Cela permet de refuser l'acc�s 
        � toutes les m�thodes et propri�t�s.
        
   iii. L'objet est partiellement s�curis� pour le script.
        Au moins une m�thode ou une propri�t� est s�curis�e pour le script. 
        Dans ce cas, vous avez deux options � prendre en consid�ration.  
        1.  Notifiez l'application client que l'objet n'est pas s�curis� pour le script.
            Cela permet de refuser l'acc�s � toutes les m�thodes et propri�t�s.
        2.  Notifiez l'application client que l'objet est s�curis� pour le script,
            mais d�sactivez les m�thodes et propri�t�s qui ne sont pas s�curis�es 
            pour le script. Cela permet la cr�ation correcte de l'objet et refuse 
            l'acc�s aux m�thodes et propri�t�s qui ne sont pas s�curis�es pour 
            le script.

Test et ex�cution des exemples
---------------------------
   Il est recommand� d'utiliser Internet Explorer version 4.0 ou ult�rieure pour tester
   cet exemple. Un exemple HTML est inclus pour illustrer l'utilisation et le test
   du projet Visual Basic de l'exemple IObjSafe.OCX.

Description du projet
-------------------
   IObjSafe.vbp est un exemple ActiveX OCX qui impl�mente l'interface IObjectSafety
   d�finie dans la biblioth�que de types IObjectSafety.TLB. Le fichier HTML 
   IObjSafe.HTM teste l'interface IObjectSafety impl�ment�e dans 
   IObjSafe.OCX en cr�ant et recevant le contr�le UCObjectSafety, puis r�f�rence 
   les propri�t�s Safe et UnSafe � l'aide de VBScript dans le document HTML.
