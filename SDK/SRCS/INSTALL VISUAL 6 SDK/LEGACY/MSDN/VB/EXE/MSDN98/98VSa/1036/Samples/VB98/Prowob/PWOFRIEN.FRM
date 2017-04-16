VERSION 5.00
Begin VB.Form frmFriends 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Type Friend passant des types d�finis par l'utilisateur"
   ClientHeight    =   3210
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5355
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3210
   ScaleWidth      =   5355
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtString 
      Height          =   285
      Left            =   1080
      TabIndex        =   5
      Top             =   2640
      Width           =   4095
   End
   Begin VB.TextBox txtLong 
      Height          =   285
      Left            =   1080
      MaxLength       =   9
      TabIndex        =   3
      Top             =   2160
      Width           =   1815
   End
   Begin VB.TextBox txtInteger 
      Height          =   285
      Left            =   1080
      MaxLength       =   4
      TabIndex        =   1
      Top             =   1680
      Width           =   1215
   End
   Begin VB.CommandButton cmdMethod 
      Caption         =   "&M�thode Friend"
      Height          =   375
      Left            =   3120
      TabIndex        =   7
      Top             =   2040
      Width           =   2055
   End
   Begin VB.CommandButton cmdProperty 
      Caption         =   "&Propri�t� Friend"
      Height          =   375
      Left            =   3120
      TabIndex        =   6
      Top             =   1560
      Width           =   2055
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Right Justify
      Caption         =   "&Cha�ne:"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   2640
      Width           =   855
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Right Justify
      Caption         =   "Entier &long:"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   2160
      Width           =   855
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "&Entier:"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   1680
      Width           =   855
   End
   Begin VB.Label Label1 
      Caption         =   $"PWOFrien.frx":0000
      Height          =   1215
      Left            =   120
      TabIndex        =   8
      Top             =   120
      Width           =   5175
   End
End
Attribute VB_Name = "frmFriends"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Montre les propri�t�s et les m�thodes Friend passant
'   des types d�finis par l'utilisateur entre les objets.

Private mtc1 As TestClass
Private mtc2 As TestClass
    
' Utilise des propri�t�s pour affecter/acc�der � un type d�fini par l'utilisateur.
Private Sub cmdProperty_Click()
    ' La m�thode d'assistance SetDemoParts
    '   affecte les contenus des zones de texte
    '   au type d�fini par l'utilisateur dans le
    '   premier objet TestClass; quelque chose
    '   pourra ainsi �tre pass� au deuxi�me objet
    '   TestClass.
    Call mtc1.SetDemoParts(CInt("0" & txtInteger), _
        CLng("0" & txtLong), txtString)
    '
    ' Affiche les �l�ments de type d�fini par l'utilisateur du premier objet TestClass
    '   avant d'effectuer le passage.
    Call mtc1.ShowDemo("Passage d'un UDT en utilisant une propri�t�", "Passer � partir de:")
    '
    ' Affecte directement le type d�fini par l'utilisateur du premier
    '   objet TestClass � celui du deuxi�me
    '   objet TestClass, en utilisant la
    '   propri�t� Demo.
    mtc2.Demo = mtc1.Demo
    '
    ' Affiche les �l�ments de type d�fini par l'utilisateur du
    '   deuxi�me objet TestClass.
    Call mtc2.ShowDemo("Passage d'un UDT en utilisant une propri�t�", "Pass� �:")
    '
    ' Lorsque la proc�dure se termine, mtc1 et mtc2
    '   passent hors de port�e et les objets
    '   TestClass se terminent.
End Sub

' Utilise des m�thodes pour affecter/acc�der � un type d�fini par l'utilisateur.
Private Sub cmdMethod_Click()
    ' La m�thode d'assistance SetDemoParts
    '   affecte les contenus des zones de texte
    '   au type d�fini par l'utilisateur dans le
    '   premier objet TestClass; quelque chose
    '   pourra ainsi �tre pass� au deuxi�me objet
    '   TestClass.
    Call mtc1.SetDemoParts(CInt("0" & txtInteger), _
        CLng("0" & txtLong), txtString)
    '
    ' Affiche les �l�ments de type d�fini par l'utilisateur du
    '   premier objet TestClass avant d'effectuer le passage.
    Call mtc1.ShowDemo("Passage d'un UDT en utilisant une m�thode", "Passer � partir de:")
    '
    ' La m�thode GetDemo du premier objet
    '   TestClass renvoie le type d�fini par l'utilisateur, qui est
    '   pass� � la m�thode SetDemo du deuxi�me objet TestClass.
    Call mtc2.SetDemo(mtc1.GetDemo)
    '
    ' Affiche les �l�ments de type d�fini par l'utilisateur du
    ' deuxi�me objet TestClass.
    Call mtc2.ShowDemo("Passage d'un UDT en utilisant une m�thode ", "Pass� �:")
    '
    ' Lorsque la proc�dure se termine, mtc1 et mtc2
    '   passent hors de port�e et les objets
    '   TestClass se terminent.
End Sub

Private Sub Form_Load()
    ' Cr�e les objets TestClass.
    Set mtc1 = New TestClass
    Set mtc2 = New TestClass
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' Lib�re les ressources de la feuille.
    Set frmFriends = Nothing
End Sub

Private Sub txtInteger_KeyPress(KeyAscii As Integer)
    Select Case KeyAscii
        Case 48 To 57     ' Autorise les chiffres.
        Case 8            ' Autorise les retours arri�re.
        Case Else         ' Supprime tout le reste.
            Beep
            KeyAscii = 0
    End Select
End Sub

Private Sub txtLong_KeyPress(KeyAscii As Integer)
    Select Case KeyAscii
        Case 48 To 57     ' Autorise les chiffres.
        Case 8            ' Autorise les retours arri�re.
        Case Else         ' Supprime tout le reste.
            Beep
            KeyAscii = 0
    End Select
End Sub

