VERSION 5.00
Begin VB.Form frmReceiver 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Message re�u par l'�v�nement de gestion Broadcast"
   ClientHeight    =   690
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5550
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   690
   ScaleWidth      =   5550
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkReceive 
      Caption         =   "&Recevoir les messages"
      Height          =   255
      Left            =   3030
      TabIndex        =   2
      Top             =   360
      Width           =   2445
   End
   Begin VB.CheckBox chkGarble 
      Caption         =   "&Tronquer les messages"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   360
      Width           =   2895
   End
   Begin VB.Label lblMessage 
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   5355
   End
End
Attribute VB_Name = "frmReceiver"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' La variable Source est d�clar�e au
'   moyen du mot cl� WithEvents; les
'   �v�nements de l'objet frmEvents qui lui
'   ont �t� affect�s peuvent donc �tre g�r�s
'   (reportez-vous � la proc�dure de l'�v�nement
'   Source_Broadcast). Lorsque la r�f�rence est
'   affect�e, Visual Basic relie l'objet � la
'   proc�dure de l'�v�nement.
Private WithEvents Source As frmEvents
Attribute Source.VB_VarHelpID = -1

Private Sub chkReceive_Click()
    If chkReceive = vbChecked Then
        Set Source = frmEvents
    Else
        ' L'affectation de la valeur Nothing �
        '   la variable WithEvents d�connecte
        '   la proc�dure de l'�v�nement de l'objet
        '   frmEvents.
        Set Source = Nothing
    End If
End Sub

Private Sub Form_Load()
    chkReceive = vbChecked
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set Source = Nothing
End Sub

' Proc�dure d'�v�nement pour l'�v�nement Broadcast.
'   L'argument Message �tant ByRef,
'   il peut �tre endommag� -- et ces modifications
'   seront vues par tous les gestionnaires suivants
'   de l'�v�nement.
Private Sub Source_Broadcast(Message As String)
    Dim intCt As Integer
    lblMessage = Message
    '
    ' Si l'option Tronqu� est coch�e, tronque
    '   le message.
    If chkGarble = vbChecked Then
        ' Utilise une �tape al�atoire qui touche
        '   entre un caract�re sur trois (minimum) et un
        '   caract�re sur neuf (maximum).
        For intCt = 1 To Len(Message) Step (Int(7 * Rnd) + 3)
            ' Pour tous les caract�res tronqu�s,
            '   remplace la valeur ASCII par un nombre
            '   al�atoire compris entre -3 et +3.
            Mid$(Message, intCt, 1) = Chr$(Asc(Mid$(Message, intCt, 1)) + Int(7 * Rnd) - 3)
        Next
    End If
End Sub
