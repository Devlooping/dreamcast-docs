VERSION 5.00
Begin VB.Form frmProgWObMain 
   Caption         =   "Programmation � l'aide d'objets"
   ClientHeight    =   2340
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5175
   LinkTopic       =   "Form2"
   ScaleHeight     =   2340
   ScaleWidth      =   5175
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdImplements 
      Caption         =   "Polymorphisme et mot cl� &Implements"
      Height          =   375
      Left            =   240
      TabIndex        =   3
      Top             =   1680
      Width           =   4665
   End
   Begin VB.CommandButton cmdEvents 
      Caption         =   "D�clenchement et gestion des &�v�nements"
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   1200
      Width           =   4665
   End
   Begin VB.CommandButton cmdFriends 
      Caption         =   "Passages des UDT entre les objets ayant des fonctions &Friend"
      Height          =   405
      Left            =   240
      TabIndex        =   0
      Top             =   210
      Width           =   4665
   End
   Begin VB.CommandButton cmdCYOCC 
      Caption         =   "&Cr�ation de vos propres classes Collection"
      Height          =   375
      Left            =   240
      TabIndex        =   1
      Top             =   720
      Width           =   4665
   End
End
Attribute VB_Name = "frmProgWObMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' La feuille frmProgWObMain affiche d'autres
'   feuilles au moyen de variables globales
'   cach�es que Visual Basic cr�e pour chaque
'   classe de feuille (frmFriends, frmCYOCC,
'   ______ et _____, dans ce cas). La variable
'   globale cach�e est d�crite � la section
'   "Cycle de vie des feuilles Visual Basic" des
'   Manuels en ligne.
'
' Les feuilles sont affich�es de mani�re non modale,
'   et la feuille frmProgWObMain se d�finie
'   elle-m�me (Me) comme le propri�taire de
'   chaque feuille. Ainsi, ses feuilles apparaissent
'   toujours au dessus de la feuille
'   frmProgWObMain.

Private Sub cmdEvents_Click()
    frmEvents.Show vbModeless, Me
End Sub

' Membres Friend passant des types d�finis par l'utilisateur.
Private Sub cmdFriends_Click()
    frmFriends.Show vbModeless, Me
End Sub

' Cr�ation des vos propres classes Collection.
Private Sub cmdCYOCC_Click()
    frmCYOCC.Show vbModeless, Me
End Sub

Private Sub cmdImplements_Click()
    frmImplements.Show vbModeless, Me
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If frmFriends Is Nothing Then
        Unload frmFriends
        Set frmFriends = Nothing
    End If
    If frmCYOCC Is Nothing Then
        Unload frmCYOCC
        Set frmCYOCC = Nothing
    End If
    If frmEvents Is Nothing Then
        Unload frmEvents
        Set frmEvents = Nothing
    End If
    If frmImplements Is Nothing Then
        Unload frmImplements
        Set frmImplements = Nothing
    End If
End Sub
