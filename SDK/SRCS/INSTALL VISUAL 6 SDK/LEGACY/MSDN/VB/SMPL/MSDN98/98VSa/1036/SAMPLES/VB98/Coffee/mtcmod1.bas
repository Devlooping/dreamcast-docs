Attribute VB_Name = "modMT"
Option Explicit

' Ce module montre la cr�ation par Visual Basic
'   d'une instance s�par�e de donn�es globales
'   pour chaque thread. Ainsi, la variable
'   glngGlobalData aura une valeur s�par�e pour
'   chaque thread que Visual Basic d�marre.
'
' Cet aspect est utilis� pour comptabiliser les
'   objets Coffee de chaque thread: il en ajoute
'   un � la variable glngGlobalData dans
'   l'�v�nement Initialize de l'objet Coffee; il
'   en retire un dans l'�v�nement Terminate.
'
' N'importe quel objet Coffee peut donc savoir
'   combien d'objets Coffee se trouvent sur sa
'   thread en testant la variable glngGlobalData.
'   Les clients peuvent �galement obtenir ce
'   r�sultat en appelant la propri�t� NumberOnThread
'   qui renvoie la variable glngGlobalData.
'
' Lorsque l'objet MTCoffee est ex�cut� dans
'   l'environnement de d�veloppement, qui contient
'   une seule thread, la propri�t� NumberOnThread
'   repr�sente le nombre total d'objets Coffee.
'   Lorsque MTCoffee est compil� avec l'option
'   Thread par objet, le nombre sera un (1) pour
'   chaque objet Coffee, � moins que la m�thode
'   GetCoffeeOnSameThread ne soit appel� pour
'   cr�er un deuxi�me objet Coffee sur la thread.
'
' Lorsque le projet MTCoffee est compil� avec
'   l'option Nb max de threads, la propri�t�
'   NumberOnThread sera sup�rieure � 1 sur
'   certaines threads d�s lors que le nombre
'   des objets Coffee actifs est sup�rieur
'   au nombre de threads du regroupement.
'
' Cet aspect est trait� � la rubrique
'   "�volutivit� et traitement multithread"
'   des Manuels en ligne.

Public glngGlobalData As Long

