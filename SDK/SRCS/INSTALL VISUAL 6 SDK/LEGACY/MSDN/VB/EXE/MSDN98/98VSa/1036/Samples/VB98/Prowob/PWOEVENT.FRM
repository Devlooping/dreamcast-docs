VERSION 5.00
Begin VB.Form frmEvents 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "D�clenchement et gestion des �v�nements"
   ClientHeight    =   4245
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4710
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4245
   ScaleWidth      =   4710
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdAddForm 
      Caption         =   "Ajouter un &destinataire"
      Height          =   375
      Left            =   2790
      TabIndex        =   3
      Top             =   3360
      Width           =   1785
   End
   Begin VB.TextBox txtMessage 
      Height          =   285
      Left            =   120
      TabIndex        =   2
      Top             =   3000
      Width           =   4455
   End
   Begin VB.CommandButton cmdPercentDone 
      Caption         =   "D�marrer une &t�che longue qui utilise un �v�nement pour transmettre la progression"
      Height          =   615
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   4215
   End
   Begin VB.Label Label2 
      Caption         =   "Message apr�s que tous les destinataires l'ont g�r�:"
      Height          =   375
      Left            =   120
      TabIndex        =   6
      Top             =   3360
      Width           =   2295
   End
   Begin VB.Label lblPercentDone 
      Height          =   255
      Left            =   360
      TabIndex        =   5
      Top             =   960
      Width           =   4095
   End
   Begin VB.Label lblEcho 
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   3840
      Width           =   4455
   End
   Begin VB.Label Label1 
      Caption         =   $"PWOEvent.frx":0000
      Height          =   1335
      Left            =   120
      TabIndex        =   1
      Top             =   1560
      Width           =   4455
   End
   Begin VB.Line Line1 
      X1              =   120
      X2              =   4560
      Y1              =   1440
      Y2              =   1440
   End
End
Attribute VB_Name = "frmEvents"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

' ========================================
'     D�clarations pour l'exemple Broadcast
'
' L'�v�nement Broadcast a un argument,
'   le message � envoyer. L'argument
'   est ByRef; les destinataires peuvent donc le changer.
Event Broadcast(Message As String)

' Collection des destinataires.
Private mcolReceivers As New Collection

' ========================================
'       D�clarations et code pour le
'           pourcentage d�j� r�alis� de l'exemple.
'
Private WithEvents mWidget As Widget
Attribute mWidget.VB_VarHelpID = -1
Private mblnCancel As Boolean

Private Sub mWidget_PercentDone(ByVal Percent As Double, Cancel As Boolean)
    lblPercentDone.Caption = CInt(100 * Percent) & " % effectu�"
    DoEvents
    If mblnCancel Then Cancel = True
End Sub

Private Sub cmdPercentDone_Click()
    Static blnProcessing As Boolean
    If blnProcessing Then
        mblnCancel = True
    Else
        blnProcessing = True
        cmdPercentDone.Caption = "&Annuler la t�che"
        mblnCancel = False
        lblPercentDone.Caption = "0 % effectu�"
        lblPercentDone.Refresh
        
        ' Cr�e un objet Widget et lance
        '   une t�che longue.
        Set mWidget = New Widget
        On Error Resume Next
        Call mWidget.LongTask(14.4, 0.9)
        '
        ' V�rifie si l'appel s'est termin� car il
        '   a �t� annul� (ne peut pas tester uniquement
        '   mblnCancel pour cette raison; cela aurait d�
        '   �tre d�fini au moment o� la m�thode
        '   LongTask a �t� renvoy�e).
        If Err.Number = 0 Then
            lblPercentDone.Caption = "T�che effectu�e"
        ElseIf Err.Number = vbObjectError + wdgERRTaskCanceled Then
            lblPercentDone.Caption = "T�che annul�e"
        Else
            ' (Gestion omise pour les autres erreurs.)
            lblPercentDone.Caption = "Quelque chose de mauvais s'est produit"
        End If
        Set mWidget = Nothing
        cmdPercentDone.Caption = "&D�marrer une t�che longue qui utilise un �v�nement pour transmettre la progression"
        blnProcessing = False
    End If
End Sub

' ========================================
'        Code pour l'exemple Broadcast
'
Private Sub cmdAddForm_Click()
    Dim frm As New frmReceiver
    ' Conserve le suivi des destinataires.
    mcolReceivers.Add frm
    frm.Show vbModeless, Me
    Me.SetFocus
    txtMessage.SetFocus
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Dim frm As frmReceiver
    On Error Resume Next
    Do While mcolReceivers.Count > 0
        Unload mcolReceivers(1)
        mcolReceivers.Remove 1
    Loop
End Sub

Private Sub txtMessage_Change()
    Dim strMessage As String
    strMessage = txtMessage.Text
    '
    ' D�clenche l'�v�nement Broadcast. Il n'y a
    '   aucun moyen de savoir si des destinataires
    '   g�rent l'�v�nement.
    RaiseEvent Broadcast(strMessage)
    '
    ' Affiche le message apr�s que tous les
    '   destinataires (le cas �ch�ant) l'ont g�r�.
    '   Il n'y a aucun moyen de savoir
    '   quel destinataire a pu le modifier,
    '   ou quelles valeurs interm�diaires
    '   le message a pu avoir.
    lblEcho = strMessage
End Sub
