VERSION 5.00
Begin VB.Form frmHello 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Serveur Bonjour"
   ClientHeight    =   1110
   ClientLeft      =   3150
   ClientTop       =   2325
   ClientWidth     =   2820
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   1110
   ScaleWidth      =   2820
End
Attribute VB_Name = "frmHello"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_TemplateDerived = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
' Note1: les deux segments de code suivants illustrent deux options
' pour l'instanciation d'un objet par un client.
' La premi�re option utilise une liaison pr�coce et n�cessite que
' "HelloProj" soit r�f�renc� � partir de la bo�te de dialogue R�f�rences du menu Projet.
' La seconde option utilise une liaison tardive et requiert une
' instruction suppl�mentaire pour cr�er l'objet. La liaison pr�coce
' est plus rapide que la liaison tardive. La liaison tardive
' offre cependant une plus grande flexibilit� pour d�cider en cours d'ex�cution quels sont les objets � utiliser.

' Note2: ce code source du client est pr�sent dans le projet afin de
' faciliter le d�bogage. Ce code peut �tre utilis� dans ce projet
' (serveur) afin de v�rifier la syntaxe et les fonctionnalit�s.
' Il peut �tre coll� dans un projet (client) s�par� et utilis� *sans* ' changements pour acc�der au serveur depuis un ex�cutable externe.
' Le fait que la cr�ation d'objet est identique pour des projets internes ou externes peut grandement faciliter les tests et le partage de code source.

' Option de liaison pr�coce.
' Dim objNew As New HelloProj.HelloClass
' MsgBox objNew.SayHello
' Set objNew = Nothing

' Option de liaison tardive.
' Dim objNew As Object
' Set objNew = CreateObject("HelloProj.HelloClass")
' MsgBox objNew.SayHello
' Set objNew = Nothing
End Sub
