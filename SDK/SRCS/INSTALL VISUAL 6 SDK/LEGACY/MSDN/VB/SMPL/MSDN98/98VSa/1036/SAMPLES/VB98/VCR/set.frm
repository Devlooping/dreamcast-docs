VERSION 5.00
Begin VB.Form frmSetTime 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "D�finir la minuterie pour l'enregistrement"
   ClientHeight    =   1830
   ClientLeft      =   2145
   ClientTop       =   3135
   ClientWidth     =   4800
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1830
   ScaleWidth      =   4800
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Annuler"
      Height          =   375
      Left            =   3480
      MaskColor       =   &H00000000&
      TabIndex        =   7
      Top             =   1080
      Width           =   975
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   3480
      MaskColor       =   &H00000000&
      TabIndex        =   6
      Top             =   480
      Width           =   975
   End
   Begin VB.TextBox txtChannel 
      Height          =   285
      Left            =   1800
      TabIndex        =   5
      Text            =   "3"
      ToolTipText     =   "Entre la cha�ne � enregistrer"
      Top             =   1320
      Width           =   615
   End
   Begin VB.TextBox txtEnd 
      Height          =   285
      Left            =   1800
      TabIndex        =   4
      Text            =   "12:30"
      ToolTipText     =   "Entre l'heure de fin de l'enregistrement"
      Top             =   840
      Width           =   615
   End
   Begin VB.TextBox txtStart 
      Height          =   285
      Left            =   1800
      TabIndex        =   3
      Text            =   "12:00"
      ToolTipText     =   "Entre l'heure de d�but de l'enregistrement"
      Top             =   360
      Width           =   615
   End
   Begin VB.Label lblCaption 
      Alignment       =   1  'Right Justify
      Caption         =   "Cha�ne:"
      Height          =   255
      Index           =   2
      Left            =   600
      TabIndex        =   2
      Top             =   1320
      Width           =   1095
   End
   Begin VB.Label lblCaption 
      Alignment       =   1  'Right Justify
      Caption         =   "Heure de fin:"
      Height          =   255
      Index           =   1
      Left            =   600
      TabIndex        =   1
      Top             =   840
      Width           =   1095
   End
   Begin VB.Label lblCaption 
      Alignment       =   1  'Right Justify
      Caption         =   "Heure de d�but:"
      Height          =   375
      Index           =   0
      Left            =   240
      TabIndex        =   0
      Top             =   360
      Width           =   1455
   End
End
Attribute VB_Name = "frmSetTime"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'**********************************************
' Sujet: Feuille montrant � l'utilisateur comment programmer
'        l'heure d'enregistrement et la cha�ne.
'        Fait partie de l'exemple d'application VCR.
'**********************************************
Option Explicit

Private Sub cmdCancel_Click()
    ' d�charge la feuille, lib�re la r�f�rence.
    Unload Me
    Set frmSetTime = Nothing
End Sub

Private Sub cmdOK_Click()
    Dim intOK As Integer        ' Valeur de retour.
    Dim intChannel As Integer   ' Num�ro de cha�ne.
    Dim strMsg As String        ' Texte du message.
    
    ' Appelle la fonction pour valider la saisie.
    intOK = ValidateTime(txtStart.Text, txtStart)
    ' Si incorrecte, sort de la proc�dure.
    If intOK = False Then Exit Sub
        
    ' Appelle la fonction pour valider la saisie.
    intOK = ValidateTime(txtEnd.Text, txtEnd)
    ' Si incorrecte, sort de la proc�dure.
    If intOK = False Then Exit Sub
    
    ' Lit la valeur de l'affichage de la cha�ne.
    intChannel = Val(txtChannel)
    ' Si hors limites, en avertir l'utilisateur.
    If intChannel < 2 Or intChannel > 13 Then
        strMsg = "Le num�ro de cha�ne doit �tre compris entre 2 et 13!"
        MsgBox strMsg, vbOKOnly, "Num�ro de cha�ne incorrect"
        txtChannel.SetFocus
        Exit Sub
    End If
    
    ' D�finit les propri�t�s de la classe Recorder.
    Recorder.StartRecording = CVar(txtStart.Text)
    Recorder.StopRecording = CVar(txtEnd.Text)
    Recorder.Channel = intChannel
        
    ' D�charge la feuille, lib�re la r�f�rence.
    Unload Me
    Set frmSetTime = Nothing
End Sub

Private Sub Form_Load()
    ' Si la propri�t� de la classe contient une
    ' valeur, l'affecte au contr�le TextBox
    If Recorder.StartRecording <> Empty Then
        txtStart.Text = Recorder.StartRecording
    End If
    If Recorder.StopRecording <> Empty Then
        txtEnd.Text = Recorder.StopRecording
    End If
    txtChannel = Recorder.Channel
End Sub
'**********************************************
' Sujet:  Valide la cha�ne de l'heure entr�e par
'         l'utilisateur. Si elle est correcte, formate la
'         cha�ne, sinon affiche un message.
' Entr�e:   Time: La cha�ne pass�e depuis une zone de texte.
'           Field: Le nom du contr�le de la zone de texte.
' Renvoie:  True pour une heure correcte, False si
'           la cha�ne n'est pas une heure correcte.
'**********************************************
Function ValidateTime(Time As String, Field As Control) As Boolean
    Dim strMsg As String    ' Texte du message.
    
    ' La fonction IsDate v�rifie une heure correcte.
    If IsDate(Time) Then
        ' Formate la cha�ne et renvoie la valeur True.
        Field.Text = Format$(Time, "hh:mm")
        ValidateTime = True
    Else
        ' Informe l'utilisateur de l'erreur.
        strMsg = "Entrez une heure correcte, S.V.P! "
        strMsg = strMsg & "(Heure:Minute)"
        MsgBox strMsg, vbOKOnly, "Heure incorrecte"
        ' Renvoie le focus dans la zone de texte.
        Field.SetFocus
        ' Renvoie une valeur False.
        ValidateTime = False
    End If
    
End Function

Private Sub txtChannel_GotFocus()
    ' S�lectionne le texte existant.
    txtChannel.SelStart = 0
    txtChannel.SelLength = Len(txtChannel.Text)
End Sub

Private Sub txtChannel_KeyPress(KeyAscii As Integer)
    ' Utilise la fonction IsNumeric pour s'assurer
    ' que seul des chiffres seront entr�s.
    If Not IsNumeric(Chr(KeyAscii)) Then
        ' Ceci n'est pas un nombre, lui affecte donc la valeur 0.
        KeyAscii = 0
    End If
End Sub

Private Sub txtEnd_GotFocus()
    ' S�lectionne le texte existant.
    txtEnd.SelStart = 0
    txtEnd.SelLength = Len(txtEnd.Text)
End Sub

Private Sub txtEnd_LostFocus()
    Dim intOK As Integer    ' Valeur de retour.
    
    ' Appelle la fonction pour valider la saisie.
    intOK = ValidateTime(txtEnd.Text, txtEnd)
End Sub

Private Sub txtStart_GotFocus()
    ' S�lectionne le texte existant.
    txtStart.SelStart = 0
    txtStart.SelLength = Len(txtStart.Text)
End Sub

Private Sub txtStart_LostFocus()
    Dim intOK As Integer    ' Valeur de retour.
    
    ' Appelle la fonction pour valider la saisie.
    intOK = ValidateTime(txtStart.Text, txtStart)
End Sub
