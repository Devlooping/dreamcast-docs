Attribute VB_Name = "Module1"
Option Explicit
' > Pour plus d'informations sur cet exemple d'application,
'   recherchez "Coffee" dans l'aide en ligne.
' > Le fichier AboutCof.txt, pr�sent dans le dossier
'   Documents associ�s de CoffWat2.vbp, contient �galement
'   des informations sur cet exemple.

' La variable gCoffeeMonitor maintient une r�f�rence � l'objet
' --------------   CoffeeMonitor partag�. Quand un client
'   acc�de � la propri�t� CoffeeMonitor d'un objet
'   Connector, la propri�t� Get renvoie cette r�f�rence
'   (Reportez-vous au module de la classe Connector).
Public gCoffeeMonitor As CoffeeMonitor

' La variable gCoffeeMonitor2 maintient une r�f�rence � l'objet
' --------------   CoffeeMonitor2 partag�. Quand un client
'   acc�de � la propri�t� CoffeeMonitor2 d'un objet
'   Connector2, la propri�t� Get renvoie cette r�f�rence
'   (Reportez-vous au module de la classe Connector2).
Public gCoffeeMonitor2 As CoffeeMonitor2

