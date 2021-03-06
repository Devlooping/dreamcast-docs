VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "Connector2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
Option Explicit
' > Pour plus d'informations sur cet exemple d'application,
'   recherchez "Coffee" dans l'aide en ligne.
' > Le fichier AboutCof.Txt, pr�sent dans le dossier
'   Documents associ�s de CoffWat2.vbp, contient �galement
'   des informations sur cet exemple.

' La classe Connector2 permet � plusieurs clients de partager
' ----------------  une instance unique de CoffeeMonitor2. La
'   propri�t� Instancing de la classe Connector2 a pour
'   valeur MultiUse; ainsi, chaque client peut cr�er son
'   propre objet Connector2. Tous les objets Connector2
'   renvoient une r�f�rence � l'unique objet CoffeeMonitor2
'   partag�; ainsi, tous les clients partagent le m�me objet
'   CoffeeMonitor2 (Reportez-vous � la propri�t� CoffeeMonitor2
'   ci-dessous).

' La propri�t� CoffeeMonitor2 renvoie toujours la
' --------------   r�f�rence globale unique �
'   l'instance partag�e de CoffeeMonitor2.
'
Public Property Get CoffeeMonitor2() As CoffeeMonitor2
    ' Si l'objet partag� CoffeeMonitor2 n'est pas cr��,
    '   le cr�e et y stocke une r�f�rence globale.
    If gCoffeeMonitor2 Is Nothing Then
        Set gCoffeeMonitor2 = New CoffeeMonitor2
    End If
    
    Set CoffeeMonitor2 = gCoffeeMonitor2
End Property
    

